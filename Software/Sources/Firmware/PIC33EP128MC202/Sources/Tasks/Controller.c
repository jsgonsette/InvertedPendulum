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
/// \file	Controller.c
/// \author	Jean-Sebastien Gonsette
/// \date	12/10/2014
///
/// Perform the dynamic control of the pendulum. This module acquires
/// all the sensors and calls the right controller sub module to drive the motor
// ###########################################################################

#include "Version.h"
#include "BSP.h"
#include "Global.h"
#include "Debug.h"
#include "Controller.h"
#include "Pendulum.h"
#include "TaskLED.h"
#include "./Devices/ADXL345.h"
#include "./Devices/ISZ2510.h"
#include "./Algorithms/KalmanFree.h"
#include "./Algorithms/CtrlJump.h"
#include "./Algorithms/CtrlFall.h"
#include "./Algorithms/CtrlEquilibrium.h"
#include "./Algorithms/Trajectory.h"
#include "./Interfaces/Motor.h"
#include "./Tools/FastTrigo.h"
#include "./Tools/PRNG.h"


// ===========================================================================
// D E F I N E
// ===========================================================================

/// Controller period [ms]
#define PERIOD		10

/// Time to wait to consider the pendulum is still [s]
#define STILL_DELAY		4

/// Max angle the pendulum can move when it is considered to be still [°]
#define STILL_MAX_MOVE	1.0f

/// Minimum swing amplitude [deg]
#define SWING_AMPL_MIN	4

/// Maximum swing amplitude [deg]
#define SWING_AMPL_MAX	6

/// Minimum swing periodicity [ms]
#define SWING_PER_MIN		1000

/// Maximum swing amplitude [ms]
#define SWING_PER_MAX		1200


// ===========================================================================
// T Y P E S
// ===========================================================================

/// Data for 'still' state identification
typedef struct
{
	unsigned int stillCount;
	float angle;
}
Still;


/// Controller actions when pendulum is under control
typedef enum
{
	ST_Wait,			///< Wait pendulum is still
	ST_Jump,			///< Jumping
	ST_Move,			///< Equilibrium and/or move
	ST_Fall				///< Fall on the ground
}
E_CtrlAction;


/// Movement set when pendulum is in equilibrium
typedef enum
{
	MOVE_Still,			///< Simple equilibrium
	MOVE_Swing			///< Swinging
}
E_CtrlMovement;


/// Control management structure
typedef struct
{
	CTRL_Mode mode;			///< Controller mode
	E_CtrlAction action;	///< Controller action when pendulum is under control
	E_CtrlMovement movment;	///< Controller movement when pendulum is in equilibrium
	Word nextMoveTime;		///< Time left before next movement [ms]
	Word nextFallTime;		///< Time left before falling [ms]
		
	CTRL_State targetState;	///< Pendulum target state (for trajectory)
	CTRL_State state;		///< Pendulum state estimation
	CTRL_Command command;	///< Command to drive the motor
	CTRL_Sensors sensors;	///< Sensor values
}
Control;


// ===========================================================================
// P R I V A T E
// ===========================================================================

/// Cycle count
static Dword gs_cycleCount;

/// Control management
static Control gs_control;


/// Periodic timer for controller processing
static HANDLE gs_timer;

/// Event for cycle signalisation
static HANDLE gs_event;


/// Still estimation
static Still gs_stillEval;

/// Counter for wheel speed update
static Word gs_wheelCounter;

/// Counter for verticality check
static Word gs_verticalCounter;

/// Counter to wait for kalman stabilization at startup
static Word gs_initCounter;



// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Internal functions
//@{
static void TimerProcess (void* param);
static void DriveMotor (CTRL_State state, CTRL_Command command);
static CTRL_State ObserverProcess (CTRL_Sensors sensors, CTRL_Command command, Word wheelCounter);
static CTRL_Command ControlProcess (CTRL_State state, CTRL_State *targetState);
static CTRL_Command StopMotor (Short rpm);

static void GoWait ();
static void GoJump ();
static void GoFall ();
static void GoEquilibrium ();
static void SelectNextMovement ();

static Bool IsStill (CTRL_State state);

static Short ComputeVerticality (CTRL_Sensors sensors);
//@}



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <CTRL_Init>
///
/// \brief		Initialize the controller module
// ===========================================================================
void CTRL_Init ()
{
	gs_control.mode = CTRL_Stop;
	gs_cycleCount = 0;
	gs_initCounter = 1000/PERIOD;
	
	// Initialize the different algorithms
	KFREE_Init ();
	
	// Periodic timer for controller processing
	// Event for processing signalisation
	gs_timer = CreateTimer (TimerProcess, TM_PERIODIC, DELAY_MS (PERIOD));
	gs_event = CreateEvent ();
}


// ===========================================================================
// <CTRL_SetMode>
///
/// \brief	Set the controller working mode (stopped, acq only, free, controlling)
///
/// \param[in]	mode	Controller mode
// ===========================================================================
void CTRL_SetMode (CTRL_Mode mode)
{
	EnterCritical ();
	{
		// Save mode and start the timer if needed
		gs_control.mode = mode;
		if (gs_control.mode != CTRL_Stop) StartTimer (gs_timer, null);
		else 
		{
			gs_cycleCount = 0;
			StopTimer (gs_timer, TM_SIG_NOT);
			MOTOR_RawCommand (MOTOR_Disabled, 0);
		}
		
		// First state when pendulum is under control
		if (gs_control.mode == CTRL_Control) GoWait ();
		
		// No command to the motor by default
		gs_control.command.torque = 0;
		gs_control.command.brake = False;
		gs_control.command.end = False;
	}
	LeaveCritical ();
}


// ===========================================================================
// <CTRL_WaitCycle>
///
/// \brief	Wait for the end of the next controller cycle
///
/// \param[in]	timeout		Timeout for waiting
/// \param[out]	pCount		Cycle counter
///
/// \return		False if timeout expired before next cycle comes
// ===========================================================================
Bool CTRL_WaitCycle (Long timeout, Dword *pCount)
{
	// Wait the event get signalized
	if (WaitEventTo (gs_event, EV_WAIT_ANY, EV_NO_RESET, 0x01, timeout) == 0x01) 
	{
		*pCount = gs_cycleCount;
		return True;
	}
	
	*pCount = 0;	
	return False;
}


// ===========================================================================
// <CTRL_ManualMotorCommand>
///
/// \brief	Specify a manual motor command to use in 'free' mode
///
/// \param	torque		Torque reference [-100..100%]
/// \param	brake		True to activate braking
// ===========================================================================
void CTRL_ManualMotorCommand (int torque, Bool brake)
{
	EnterCritical ();
	{
		// Manual motor command possible in free mode only
		if (gs_control.mode == CTRL_Free)
		{
			gs_control.command.stop = False;
			gs_control.command.brake = brake;
			gs_control.command.torque = 0;
			
			// Convert in mN: 100% = 50mN.m
			gs_control.command.torque = (Long)torque*500;
			
			if (torque == 0) {
				gs_control.command.stop = True;
				gs_control.command.torque = 0;
			}
		}
	}
	LeaveCritical ();
}


// ===========================================================================
// <CTRL_GetPendulumSensors>
///
/// \brief	Return the sensors value acquired during the previous controller cycle
///
/// \param[out]	sensors		Sensor values
// ===========================================================================
void CTRL_GetPendulumSensors (CTRL_Sensors* sensors)
{
	EnterCritical ();
	{
		*sensors = gs_control.sensors;
	}
	LeaveCritical ();
}


// ===========================================================================
// <CTRL_GetPendulumState>
///
/// \brief	Return the pendulum state estimated by the controller.
///
/// \param[out]	state		Pendulum state
// ===========================================================================
void CTRL_GetPendulumState (CTRL_State* state)
{
	EnterCritical ();
	{
		*state = gs_control.state;
	}
	LeaveCritical ();
}


// ===========================================================================
// <CTRL_GetPendulumTargetState>
///
/// \brief	Return the pendulum target state for trajectory following.
///
/// \param[out]	targetState		Pendulum target state
// ===========================================================================
void CTRL_GetPendulumTargetState (CTRL_State* targetState)
{
	EnterCritical ();
	{
		*targetState = gs_control.targetState;
	}
	LeaveCritical ();
}


// ===========================================================================
// <CTRL_GetPendulumCommand>
///
/// \brief	Return the pendulum command generated by the controller.
///
/// \param[out]	command		Pendulum command
// ===========================================================================
void CTRL_GetPendulumCommand (CTRL_Command* command)
{
	EnterCritical ();
	{
		*command = gs_control.command;
	}
	LeaveCritical ();
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################

// ===========================================================================
// <TimerProcess>
///
/// \brief	Timer callback activated periodically to perform the controller job
// ===========================================================================
static void TimerProcess (void* param)
{
	Word counter;
	Short verticality;
	Bool vertical;

	// Count cycles
	gs_cycleCount ++;
	if (gs_initCounter > 0) gs_initCounter --;
	
	// Sensors acquisition
	gs_control.sensors.bldcSpeed = BSP_BLDCGetSpeed (&counter);
	gs_control.sensors.gyroZ = ISZ2510_GetAngularSpeed (g_ISZ2510);
	ADXL345_GetAccelerations (g_ADXL345, &gs_control.sensors.accXYZ [0], 
								&gs_control.sensors.accXYZ [1], 
								&gs_control.sensors.accXYZ [2]);
								
	// Reset state for this cycle
	gs_control.state.bodyAngle = 0;
	gs_control.state.bodySpeed = 0;
	gs_control.state.wheelSpeed = 0;
	gs_control.targetState.bodyAngle = 0;
	gs_control.targetState.bodySpeed = 0;
	gs_control.targetState.wheelSpeed = 0;

	// Is the pendulum vertical ?
	verticality = ComputeVerticality (gs_control.sensors);
	vertical = (verticality > -450 && verticality < 450);
		
	// Apply the right controller processing
	switch (gs_control.mode)
	{
	default: ASSERT (0);

	// Nothing to do. Motor stopped
	case CTRL_AcqOnly:
	case CTRL_Stop: 
	
		gs_control.command.stop = True;
	break;

	// Pendulum can be freely manipulated
	case CTRL_Free: 
	
		// Estimation of pendulum state only
		if (vertical) gs_control.state = ObserverProcess (gs_control.sensors, gs_control.command, counter);
		gs_control.state.verticality = verticality;
	break;		

	// Pendulum is under control
	case CTRL_Control: 
	
		if (vertical)
		{
			// Estimation of pendulum state 
			gs_control.state = ObserverProcess (gs_control.sensors, gs_control.command, counter);
			gs_control.state.verticality = verticality;
			
			// Then compute the ideal command to reach a given state
			if (gs_initCounter == 0)
				gs_control.command = ControlProcess (gs_control.state, &gs_control.targetState);
			else 
				gs_control.command.stop = True;
		}
		
		// Stop the motor if verticality is beyond acceptable range
		else gs_control.command = StopMotor (0);
	break;		
	}
	
	// Drive the motor
	DriveMotor (gs_control.state, gs_control.command);

	// Signalization of the end of the periodic processing
	PulseEvent (gs_event, 0x01);	
}


// ===========================================================================
// <DriveMotor>
///
/// \brief	Give a command to control the motor
///
/// \param		state			Current pendulum estimated state
/// \param		command			Motor command to apply
// ===========================================================================
static void DriveMotor (CTRL_State state, CTRL_Command command)
{
	if (command.stop) MOTOR_RawCommand (MOTOR_Disabled, 0);
	else if (command.brake) MOTOR_RawCommand (MOTOR_Brake, 0);
	else if (command.torque == 0) MOTOR_RawCommand (MOTOR_Disabled, 0);
	else MOTOR_SetTorque (command.torque, state.wheelSpeed);
}


// ===========================================================================
// <ObserverProcess>
///
/// \brief	Estimation of the pendulum state at this time step
///
/// \param	sensors			Current sensors value
/// \param	command			Current motor command
/// \param	wheelCounter	Counter incremented each time we get a motor speed update
///
/// \return	New observer estimated sate
// ===========================================================================
static CTRL_State ObserverProcess (CTRL_Sensors sensors, CTRL_Command command, Word wheelCounter)
{
	CTRL_State state;
	
	// Update observer according to the gyro dynamic and motor torque
	KFREE_Update (sensors.gyroZ, command.torque, command.brake, PERIOD);
	
	// Correct observer according to acc. measurements
	KFREE_Measure (sensors.accXYZ [0], sensors.accXYZ [1]);	
	
	// Correct observer according to wheel speed measurement (if we get an update)
	if (gs_wheelCounter != wheelCounter) 
	{
		KFREE_MeasureWheel (sensors.bldcSpeed);
		gs_wheelCounter = wheelCounter;
	}
	
	// Get new estimated state
	KFREE_GetState (&state);
	
	return state;
}


// ===========================================================================
// <ControlProcess>
///
/// \brief	Compute the motor command for the current action at this time step
///
/// \param		state			Current pendulum estimated state	
/// \param[out]	targetState		Target state for trajectory following
///
/// \return	New motor command for this time step
// ===========================================================================
static CTRL_Command ControlProcess (CTRL_State state, CTRL_State* targetState)
{	
	CTRL_Command command;
	Long trjTorque;
	Bool vertical;

	// Check verticality
	vertical = (gs_control.state.verticality > -100 && gs_control.state.verticality < 100);
	
	// Filter false events during a jump
	if (vertical == False)
	{
		vertical = True;
		if (gs_verticalCounter >= PERIOD) gs_verticalCounter -= PERIOD;		
		else vertical = False;
	}
	else gs_verticalCounter = 500;


	switch (gs_control.action)
	{
	default: ASSERT (0);

	// Wait to jump or to start equilibrium
	case ST_Wait:

		// Can jump ?
		if (IsStill (state)) GoJump ();		

		// Can start equilibrium ?
		else if (state.bodyAngle < 200 && state.bodyAngle > -200) GoEquilibrium ();
		
		command = StopMotor (state.wheelSpeed);
		break;
	
	// Jumping phase
	case ST_Jump:
		command = CTRLJUMP_ComputeCommand (state);
		
		if (state.bodyAngle < 200 && state.bodyAngle > -200) 
			GoEquilibrium ();
		else if (command.end) 
			GoWait ();
		break;
	
	// Falling phase
	case ST_Fall:
		command = CTRLFALL_ComputeCommand (state, PERIOD);
		if (command.end) GoWait ();
		break;

	// Pendulum held in equilibrium
	case ST_Move:
	
		// Select next movement if needed
		if (gs_control.nextMoveTime <= PERIOD) SelectNextMovement ();
		else gs_control.nextMoveTime -= PERIOD;
		
		// Fall on the ground when time is coming
		if (gs_control.nextFallTime <= PERIOD) GoFall ();
		else gs_control.nextFallTime -= PERIOD;
		
		// Compute motor command according to the current movement
		if (gs_control.movment == MOVE_Still) 
		{
			// Simple equilibrium
			command = CTRLEQUI_ComputeSteadyCommand (state);
		}
		else
		{
			// Follow a trajectory
			*targetState = TRJ_NextStep (PERIOD, &trjTorque);
			command = CTRLEQUI_ComputeCommand (state, *targetState, trjTorque);
		}
		
		// Try to recover by managing the fall
		if (command.end) GoFall ();

		break;
	}
	
	// Stop the motor if we are not vertical enough
	if (vertical == False) 
	{
		GoWait ();
		command = StopMotor (state.wheelSpeed);
	}
	
	return command;
}


// ===========================================================================
// <StopMotor>
///
/// \brief	Compute a command to deccelerate the motor gently
/// \param	rpm		Motor speed [RPM]
///
/// \return	Motor command to apply to stop the motor
// ===========================================================================
static CTRL_Command StopMotor (Short rpm)
{
	CTRL_Command command;
	
	command.end = False;
	command.brake = False;
	command.stop = False;
	command.torque = 0;

	if (rpm > 500) command.torque = -20000;
	else if (rpm < -500) command.torque = 20000;
	else command.brake = True;
	
	return command;
}


// ===========================================================================
// <GoWait>
///
/// \brief	Go in control / Wait state
// ===========================================================================
static void GoWait ()
{
	// Switch leds off
	TLED_Off (LED_GREEN);
	TLED_Off (LED_YELLOW);
	TLED_Off (LED_RED);

	gs_control.action = ST_Wait;
	gs_stillEval.stillCount = 0;
}


// ===========================================================================
// <GoJump>
///
/// \brief	Go in control / Jump state
// ===========================================================================
static void GoJump ()
{
	// Make leds roll in the direction of wheel rotation
	if (gs_control.state.bodyAngle > 0) TLED_RollRight (1);
	else TLED_RollLeft (1);

	gs_control.action = ST_Jump;
	CTRLJUMP_Restart (PERIOD);
}


// ===========================================================================
// <GoFall>
///
/// \brief	Go in control / Falling state
// ===========================================================================
static void GoFall ()
{
	TLED_Off (LED_YELLOW);
	TLED_Off (LED_RED);
	TLED_Off (LED_GREEN);
	
	gs_control.action = ST_Fall;
	CTRLFALL_Restart ();
}


// ===========================================================================
// <GoEquilibrium>
///
/// \brief	Go in control / Equilibrium state
// ===========================================================================
static void GoEquilibrium ()
{
	// Make led green blink
	TLED_Blink (LED_GREEN, 1);
	TLED_Off (LED_YELLOW);
	TLED_Off (LED_RED);

	// Start with simple equilibrium
	gs_control.action = ST_Move;
	gs_control.movment = MOVE_Still;
	
	// Chose delay before next actions
	gs_control.nextMoveTime = PRNG_Rand (5000, 10000);
	gs_control.nextFallTime = PRNG_Rand (60000, 90000);

	CTRLEQUI_Restart (PERIOD);	
}


// ===========================================================================
// <SelectNextMovement>
///
/// \brief	Select the next movement randomly and initialize the corresponding
///			trajectory
// ===========================================================================
static void SelectNextMovement ()
{
	const E_CtrlMovement movements [] = {MOVE_Still, MOVE_Swing};
	const int weights [] = {1, 1};
	int totalWeight;
	int i, numMoves;
	Dword random;
	
	// Compute sum of movement weights
	numMoves = sizeof (weights) / sizeof (int);
	totalWeight = 0;
	for (i = 0; i < numMoves; i ++) totalWeight += weights [i];
	
	if (gs_control.movment != MOVE_Still) gs_control.movment = MOVE_Still;
	else
	{
		// Select a movement randomly
		random = PRNG_Rand (0, totalWeight-1);
		totalWeight = 0;
		for (i = 0; i < numMoves; i ++) 
		{		
			totalWeight += weights [i];
			if (random < totalWeight) break;
		}
		
		gs_control.movment = movements [i];
	}
	
	// Select a duration randomly
	gs_control.nextMoveTime = PRNG_Rand (5000, 15000);
	
	// Initialize the parameters of the movement randomly
	switch (gs_control.movment)
	{
		case MOVE_Still: break;
		case MOVE_Swing:
			TRJ_InitSwing (PRNG_Rand (SWING_PER_MIN, SWING_PER_MAX), 
						PRNG_Rand (SWING_AMPL_MIN, SWING_AMPL_MAX));
			break;
	}	
}


// ===========================================================================
// <IsStill>
///
/// \brief	Check if the pendulum is still on its left or right edge
// ===========================================================================
static Bool IsStill (CTRL_State state)
{
	float e;
	float bodyAngle;
	
	bodyAngle = DECI_DEG (state.bodyAngle);
	
	// Angle must be less than 50°
	if (bodyAngle > DEG(60)) return False;
	if (bodyAngle < DEG(-60)) return False;
	
	// Angle must be greater than 30°
	if (bodyAngle >= 0 && bodyAngle < DEG(30)) return False;
	if (bodyAngle < 0 && bodyAngle > DEG(-30)) return False;
	
	// Current position must be close to previous one
	e = bodyAngle - gs_stillEval.angle;
	if (e < 0.0f) e = -e;
	
	if (e > DEG(STILL_MAX_MOVE))
	{
		// Reset reference position
		gs_stillEval.angle = bodyAngle;
		gs_stillEval.stillCount = 0;
	}
	else gs_stillEval.stillCount++;
	
	// Pendulum must not have moved for 'STILL_DELAY' seconds
	if ((gs_stillEval.stillCount*PERIOD) >= (STILL_DELAY*1000)) return True;
	return False;
}


// ===========================================================================
// <ComputeVerticality>
///
/// \brief	Compute vertical angle of the pendulum
///
/// \param	sensors	The 3 accelerometer values of the accelerometer
///
/// \return	Vertical angle: [-90; 90°] [1/10 °]
// ===========================================================================
static Short ComputeVerticality (CTRL_Sensors sensors)
{
	float ax, ay, az;
	float xy, angle;
	
	// Convert acceleration to m/s^2
	ax = MILLI_G (sensors.accXYZ [0]);
	ay = MILLI_G (sensors.accXYZ [1]);
	az = MILLI_G (sensors.accXYZ [2]);
	
	// Angle measurement
	xy = FT_Sqrt (ax*ax + ay*ay);
	angle = FT_Atan2 (az, xy);
	if (angle > PI) angle -= 2.0f*PI;
	
	return (Short) _2_DECI_DEG (angle);
}

// END
