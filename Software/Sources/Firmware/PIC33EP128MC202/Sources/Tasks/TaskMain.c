// ###########################################################################
//
// Copyright (C) 2015 GenId SPRL
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
/// \file	TaskMAIN.c
/// \author	Jean-Sebastien Gonsette
/// \date	20/10/2014
///
/// This is the main application task for housekeeping purpose.
/// Its role is to send the telemetries and to proceed to the device calibration.
// ###########################################################################

#include "BSP.h"
#include "Config.h"
#include "Global.h"
#include "./Tasks/Controller.h"
#include "./Tools/Message.h"
#include "./Tools/PRNG.h"
#include "./Devices/ADXL345.h"
#include "./Devices/ISZ2510.h"
#include "./Tasks/TaskLED.h"
#include "./Tasks/TaskUartTx.h"
#include "./Tasks/TaskMain.h"
#include "./Interfaces/Motor.h"
#include "./Interfaces/Battery.h"


// ===========================================================================
// T Y P E S
// ===========================================================================


// ===========================================================================
// P R I V A T E
// ===========================================================================

/// Application mode
static TMAIN_Mode gs_mode;

/// Telemetry speed (0: stopped)
static int gs_tm;

/// Telemetry counter
static int gs_tmCounter;

/// Config need to be saved
static Bool gs_saveConfig;

/// Telemetries raw buffer
static Byte gs_rawBuffer [200];

/// Circular message buffer for telemetries
static MSG_Buffer gs_tmBuffer;


// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Internal functions
//@{
static Word InitApplication ();
static void InitPnrg ();
static void SaveConfig ();
static void SendTelemetry (Dword cycleCounter);
static int TestMotor ();
static int SetupAccelerometer ();
static int SetupGyroscope ();
//@}



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <TMAIN_EntryPoint>
///
/// \brief		Main application task entry point
///
// ===========================================================================
void TMAIN_EntryPoint (void* param)
{
	Word error;
	Dword ms, msNext = 0;
	Dword cycleCount;
	
	// Init application
	error = InitApplication ();
	
	// Show critical error (RED pulses the error number)
	if (error != 0)
	{
		TLED_Off (LED_GREEN);
		TLED_Off (LED_YELLOW);
		TLED_Pulse (LED_RED, error);
		return;
	}
	
//TMAIN_SetMode (MODE_Free);

	// Main loop
	while (1)
	{
		// Wait next controller cycle
		if (CTRL_WaitCycle (DELAY_MS (500), &cycleCount)) 
		{
			// Send telemetries if needed
			if (gs_tm > 0) SendTelemetry (cycleCount);
			
			// Save config if needed
			if (gs_saveConfig) SaveConfig ();
		}
		
		// Check battery level every minute
		ms = GetMsCount ();
		if (ms >= msNext)
		{
			msNext = ms + 60000;
			if (BAT_GetVoltage () < BAT_MIN_VOLTAGE) break;
		}
	}
	
	// No more battery. Stop the pendulum
	SuspendTask (g_taskUartTx);
	SuspendTask (g_taskUartRx);
	CTRL_SetMode (CTRL_Stop);
	
	// Warning
	TLED_Off (LED_GREEN);
	TLED_Off (LED_YELLOW);
	TLED_Pulse (LED_RED, 1);
	
	SleepTask (DELAY_S (10));
	
	// Shut down everything to minimize consumption
	/// \todo
}


// ===========================================================================
// <TMAIN_CalibrateBattery>
///
/// \brief	Calibrate the battery with a new reference voltage
///
/// \param[in]	refVoltage 		Reference voltage measurement [1/100 V]
// ===========================================================================
void TMAIN_CalibrateBattery (Word refVoltage)
{
	BAT_Calibrate (refVoltage);
	gs_saveConfig = True;
}


// ===========================================================================
// <TMAIN_CalibrateIMU>
///
/// \brief	Calibrate the IMU with a new bias and scale factors correction
///
/// \param[in]	accScales		Measured acc scale factorsbias (x, y) [1/1000]
/// \param[in]	accBias			Measured acc bias (x, y, z) [µG]
/// \param[in]	gyroBias		Measured gyro bias [1/1000 °/s]
// ===========================================================================
void TMAIN_CalibrateIMU (Long accScales [2], Long accBias [3], Long gyroBias)
{
	ISZ2510_SetCalibrationOffset (g_ISZ2510, gyroBias);
	
	ADXL345_SetCalibrationOffset (g_ADXL345, 0, (Short) (accBias [0] / 1000));
	ADXL345_SetCalibrationOffset (g_ADXL345, 1, (Short) (accBias [1] / 1000));
	ADXL345_SetCalibrationOffset (g_ADXL345, 2, (Short) (accBias [2] / 1000));
	
	ADXL345_SetCalibrationScaler (g_ADXL345, 0, accScales [0] / 1000.0f);
	ADXL345_SetCalibrationScaler (g_ADXL345, 1, accScales [1] / 1000.0f);
	
	gs_saveConfig = True;
}


// ===========================================================================
// <TMAIN_SetTelemetry>
///
/// \brief	Enable or disable telemetry uploading
///
/// \param[in]	s	0: No telemetry
///					n: Telemetry sent 1 cycle out of n
// ===========================================================================
void TMAIN_SetTelemetry (int n)
{
	EnterCritical ();
	{	
		gs_tm = n;
		gs_tmCounter = 0;

		// Configure the controller if needed: it cannot be stopped
		// if we have to send the telemetries
		if (gs_mode == MODE_Stop)
		{
			if (gs_tm > 0) CTRL_SetMode (CTRL_AcqOnly);
			else CTRL_SetMode (CTRL_Stop);
		}
	}
	LeaveCritical ();
}


// ===========================================================================
// <TMAIN_Mode>
///
/// \brief	Set the application working mode (stopped, free, equilibrium)
///
/// \param[in]	mode	Application mode
// ===========================================================================
void TMAIN_SetMode (TMAIN_Mode mode)
{
	TLED_Off (LED_GREEN);
	TLED_Off (LED_RED);
	TLED_Off (LED_YELLOW);

	EnterCritical ();
	{	
		gs_mode = mode;
		
		// Configure the controller accordingly
		switch (mode)
		{
		default:
		case MODE_Stop:
			if (gs_tm == 0) CTRL_SetMode (CTRL_Stop);
			else CTRL_SetMode (CTRL_AcqOnly);
			break;
			
		case MODE_Free: 
			CTRL_SetMode (CTRL_Free);
			TLED_On (LED_GREEN);
			break;
			
		case MODE_Control: 
			CTRL_SetMode (CTRL_Control);
			break;
		}	
	}
	LeaveCritical ();
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################

// ===========================================================================
// <InitApplication>
///
/// \brief	Initialize the whole application
///
/// \return	0 in case of success
// ===========================================================================
static Word InitApplication ()
{
	Word error = 0;
	
	// Basic tests and setup
	if (SetupGyroscope () != 0) error = 4;
	else if (SetupAccelerometer () != 0) error = 3;
	else if (TestMotor () != 0) error = 2;
	if (error != 0) return error;
	
	// Restore application settings, if any
	CFG_Restore ();

	// Init our buffer to send the telemetries
	MSG_InitBuffer (&gs_tmBuffer, gs_rawBuffer, sizeof (gs_rawBuffer));
	
	// Everything stopped
	gs_tm = 0;
	gs_saveConfig = False;
	
	// Initialize the Pseudo Random Number Generator
	InitPnrg ();

	// Start the controller
	TMAIN_SetMode (MODE_Control);

	// OK, we release the other tasks
	ResumeTask (g_taskUartTx);
	ResumeTask (g_taskUartRx);
	
	return 0;
}


// ===========================================================================
// <InitPnrg>
///
/// \brief	Initialize the random number generator thanks to the sensors noise
// ===========================================================================
static void InitPnrg ()
{
	Short acc [3];
	
	// Seed the generator with random data
	ADXL345_GetAccelerations (g_ADXL345, acc, acc+1, acc+2);
	PRNG_Seed ((Byte*) acc, sizeof (acc));
}


// ===========================================================================
// <SaveConfig>
///
/// \brief	Save the current configuration in EEPROM
// ===========================================================================
static void SaveConfig ()
{
	gs_saveConfig = False;
	CFG_Save ();
}


// ===========================================================================
// <SendTelemetry>
///
/// \brief	Collect and push all the telemetries through the UART
///
/// \param	cycleCounter		Value of the control process cycle counter
// ===========================================================================
static void SendTelemetry (Dword cycleCounter)
{
	CTRL_Sensors sensors;
	CTRL_State state;
	CTRL_State targetState;
	CTRL_Command command;
	Dword timestamp;

	// Telemetries are sent 1 time out of n
	if (++gs_tmCounter >= gs_tm) gs_tmCounter = 0;
	if (gs_tmCounter != 0) return;
	
	// Get a copy of the last sensor values and pendulum state estimation
	CTRL_GetPendulumSensors (&sensors);
	CTRL_GetPendulumState (&state);
	CTRL_GetPendulumTargetState (&targetState);
	CTRL_GetPendulumCommand (&command);
	timestamp = GetMsCount ();

	// Start of telemetries
	MSG_StartTM (&gs_tmBuffer, "TM.Start");
	MSG_AddParamLong (&gs_tmBuffer, cycleCounter);
	MSG_AddParamLong (&gs_tmBuffer, timestamp);
	MSG_FinishAndSend (&gs_tmBuffer);

	// Send sensors
	MSG_StartTM (&gs_tmBuffer, "SENSORS");
	MSG_AddParamLong (&gs_tmBuffer, sensors.bldcSpeed);
	MSG_AddParamLong (&gs_tmBuffer, sensors.gyroZ);
	MSG_AddParamLong (&gs_tmBuffer, sensors.accXYZ [0]);
	MSG_AddParamLong (&gs_tmBuffer, sensors.accXYZ [1]);
	MSG_AddParamLong (&gs_tmBuffer, sensors.accXYZ [2]);
	MSG_FinishAndSend (&gs_tmBuffer);

	// Send state
	MSG_StartTM (&gs_tmBuffer, "STATE");
	MSG_AddParamLong (&gs_tmBuffer, state.bodyAngle);
	MSG_AddParamLong (&gs_tmBuffer, state.bodySpeed);
	MSG_AddParamLong (&gs_tmBuffer, state.wheelSpeed);
	MSG_AddParamLong (&gs_tmBuffer, targetState.bodyAngle);
	MSG_AddParamLong (&gs_tmBuffer, targetState.bodySpeed);
	MSG_AddParamLong (&gs_tmBuffer, targetState.wheelSpeed);
	MSG_FinishAndSend (&gs_tmBuffer);

	// Send motor command
	MSG_StartTM (&gs_tmBuffer, "CTRL");
	MSG_AddParamLong (&gs_tmBuffer, command.brake? 1:0);
	MSG_AddParamLong (&gs_tmBuffer, command.torque);
	MSG_FinishAndSend (&gs_tmBuffer);

	// End of telemetries
	MSG_StartTM (&gs_tmBuffer, "TM.End");
	MSG_FinishAndSend (&gs_tmBuffer);
}


// ===========================================================================
// <TestMotor>
///
/// \brief	Test if we can drive the motor and get feedback
///
/// \return		0 if successful
// ===========================================================================
static int TestMotor ()
{
	Word speed0, speed1;
	
	// Apply a small torque (5%) on the motor
	MOTOR_RawCommand (MOTOR_Forward, 50);

	// Wait in order to get some speed
	// Measure speed
	SleepTask (DELAY_MS (500));
	speed0 = BSP_BLDCGetSpeed (NULL);

	// Brake, wait and check motor is stopped
	MOTOR_RawCommand (MOTOR_Disabled, 0);
	
	SleepTask (DELAY_MS (1500));
	speed1 = BSP_BLDCGetSpeed (NULL);

	if (speed0 < 300 || speed1 != 0) return -1;
	return 0;
}


// ===========================================================================
// <SetupAccelerometer>
///
/// \brief	Get access to the ADXL345 accelerometers and perform initialization
///
/// \return 0 if successful
// ===========================================================================
static int SetupAccelerometer ()
{
	// Basic setup of the device
	if (ADXL345_Init (g_ADXL345) != 0) return -1;

	// Put the device in normal measurement mode @ 100 samples/s
	ADXL345_SetBaudrate (g_ADXL345, ADXL345_BR_100);	
	ADXL345_SetLowPowerMode (g_ADXL345, 0);
	ADXL345_SetMode (g_ADXL345, ADXL345_MODE_MEASURE);

	return 0;
}


// ===========================================================================
// <SetupGyroscope>
///
/// \brief	Get access to the ISZ2510 Gyro and perform initialization
///
/// \return 0 if successful
// ===========================================================================
static int SetupGyroscope ()
{
	Word d;
	
	// Basic setup of the device
	if (ISZ2510_Init (g_ISZ2510) != 0) return -1;

	// Configure bandwidth and sample rate
	ISZ2510_SetBandwidth (g_ISZ2510, ISZ2510_BW_41);
	d = ISZ2510_SetBaudrate (g_ISZ2510, 50);

	return 0;
}

// End
