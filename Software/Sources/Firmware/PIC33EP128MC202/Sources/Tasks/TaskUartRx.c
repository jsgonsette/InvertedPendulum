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
/// \file	TaskUartRx.c
/// \author	Jean-Sebastien Gonsette
/// \date	17/09/2014
///
/// This module manages incoming commands from the UART port.
// ###########################################################################

#include <string.h>
#include <stdlib.h>

#include "Main.h"
#include "BSP.h"
#include "Global.h"
#include "Version.h"
#include "./Tools/Message.h"
#include "./Interfaces/Battery.h"
#include "./Interfaces/Motor.h"
#include "TaskMain.h"
#include "TaskUartTx.h"
#include "TaskUartRx.h"
#include "Controller.h"


// ===========================================================================
// D E F I N E
// ===========================================================================



// ===========================================================================
// T Y P E S
// ===========================================================================


// ===========================================================================
// P R I V A T E
// ===========================================================================

/// Raw buffer for builing a response to a query
static Byte gs_rawBuffer [100];

/// Circular message buffer for builing a response to a query
static MSG_Buffer gs_buffer;



// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Internal functions
//@{
static void ProcessCommandOs (const char* str);
static void ProcessCommandSystem (const char* str);
static void ProcessCommandBattery (const char* str);
static void ProcessCommandCalibration (const char* str);
static Long ExtractParamLong ();
//@}



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <TUARTRX_Init>
///
/// \brief		UART - Rx module initialisation
// ===========================================================================
void TUARTRX_Init ()
{
	// Initialize the circular buffer to write responses
	MSG_InitBuffer (&gs_buffer, gs_rawBuffer, sizeof (gs_rawBuffer));
}


// ===========================================================================
// <TUARTRX_EntryPoint>
///
/// \brief		UART - Rx entry point
///
/// Incoming commands are dispatched for processing. 
// ===========================================================================
void TUARTRX_EntryPoint (void* param)
{
	char cmd [15];
	int cmdIdx;
    int i;
    Byte c;

	// Task loop
	while (1)
	{
		// Wait for the command line symbol ':'
		cmdIdx = -1;
		while (1)
		{
			UART_Read (g_UART, &c, 1, -1);
			if (c == ':') break;
		}

		// Read first command label
	    i = 0;
		while (1)
		{
			UART_Read (g_UART, &c, 1, -1);
			if (c == '.' || c == '\r' || c == '\n') break;
			if (i < sizeof (cmd)) cmd [i++] = (char) c;
		}
		cmd [i] = 0;

		// Parse the command
		if (strcmp (cmd, "OS") == 0) cmdIdx = 0;
		else if (strcmp (cmd, "SYS") == 0) cmdIdx = 1;
		else if (strcmp (cmd, "BATTERY") == 0) cmdIdx = 2;
		else if (strcmp (cmd, "CALIB") == 0) cmdIdx = 3;

		if (c != '.') cmdIdx = -1;
		if (cmdIdx < 0) continue;

		// Read second command label
		i = 0;
		while (1)
		{
			UART_Read (g_UART, &c, 1, -1);
			if (c == '.'  || c == '\r' || c == '\n') break;
			if (i < sizeof (cmd)) cmd [i++] = (char) c;
		}
		cmd [i] = 0;

		// Dispatch command
		switch (cmdIdx)
		{
			case 0: ProcessCommandOs (cmd); break;
			case 1: ProcessCommandSystem (cmd); break;
			case 2: ProcessCommandBattery (cmd); break;
			case 3: ProcessCommandCalibration (cmd); break;
		}
	}
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################

// ===========================================================================
// <ProcessCommandOs>
///
/// \brief	Process console command related to the Operating System
///
/// \param[in]	str		command = {Version | Memory | Task}
// ===========================================================================
static void ProcessCommandOs (const char* cmd)
{
	Dword dw;
	Dword free;
	Word kernelStat, idleStat;
	HANDLE task;
	NS_TaskInfo info;
	const NS_Version* pVersion;
	char c;
	int idx;
	int cmdIdx = -1;

	if (strcmp (cmd, "Version") == 0) cmdIdx = 0;
	else if (strcmp (cmd, "Memory") == 0) cmdIdx = 1;
	else if (strcmp (cmd, "Kernel") == 0) cmdIdx = 2;
	else if (strcmp (cmd, "Task") == 0) cmdIdx = 3;

	// Skip silently unknown commands
	if (cmdIdx < 0) return;
	
	// OS version
	if (cmdIdx == 0)
	{
		MSG_StartResponse (&gs_buffer, "OS.Version");
	
		pVersion = GetKernelVersion ();
		MSG_AddParamLong (&gs_buffer, pVersion->major);
		MSG_AddParamLong (&gs_buffer, pVersion->minor);
		MSG_AddParamLong (&gs_buffer, pVersion->revision);
		MSG_AddParamLong (&gs_buffer, pVersion->build);
	}
	
	// Memory status
	else if (cmdIdx == 1)
	{
		MSG_StartResponse (&gs_buffer, "OS.Memory");
		
		// Return static usage, dynamic usage, max dynamic usage and kernel stack usage
		MSG_AddParamLong (&gs_buffer, HeapGetStaticUsage ());
		MSG_AddParamLong (&gs_buffer, HeapGetUsage (&free));
		MSG_AddParamLong (&gs_buffer, HeapGetMaxUsage ());
		MSG_AddParamLong (&gs_buffer, GetKernelStackUsage ());
		MSG_AddParamLong (&gs_buffer, free);
	}

	// Kernel status
	else if (cmdIdx == 2)
	{
		MSG_StartResponse (&gs_buffer, "OS.Kernel");
		
		// Return kernel and idle processor usage
		GetKernelStat (&kernelStat, &idleStat);
		MSG_AddParamLong (&gs_buffer, kernelStat);
		MSG_AddParamLong (&gs_buffer, idleStat);

		// Return max memory consumption on the kernel stack
		MSG_AddParamLong (&gs_buffer, GetKernelStackUsage ());
	}
	
	// Information about a task (given by an index)
	else if (cmdIdx == 3)
	{
		// Read Task number
		idx = 0;
		while (1)
		{
			UART_Read (g_UART, (Byte*) &c, 1, -1);
			if (c < '0'  || c > '9') break;
			idx *= 10;
			idx += c - '0';
		}

		MSG_StartResponse (&gs_buffer, "OS.Task.0");

		task = GetTaskHandle (idx);
		if (task != INVALID_HANDLE)
		{	
			// Return task name, priority, stack size and max stack usage
			GetTaskInfo (task, &info);
			MSG_AddParamString (&gs_buffer, info.pName);
			MSG_AddParamLong (&gs_buffer, info.priority);
			MSG_AddParamLong (&gs_buffer, info.stackSize);

			dw = GetTaskStakUsage (task);
			MSG_AddParamLong (&gs_buffer, dw);

			MSG_AddParamLong (&gs_buffer, info.timeLog);
		}	
	}
	
	// Send the response
	MSG_FinishAndSend (&gs_buffer);
}


// ===========================================================================
// <ProcessCommandSystem>
///
/// \brief	Process console command related to system
///
/// \param[in]	str		command: 
// ===========================================================================
static void ProcessCommandSystem (const char* cmd)
{
	int cmdIdx = -1;
	int n;
	Long torque;

	if (strcmp (cmd, "Version") == 0) cmdIdx = 0;
	else if (strcmp (cmd, "SetMode") == 0) cmdIdx = 1;
	else if (strcmp (cmd, "SetTM") == 0) cmdIdx = 2;
	else if (strcmp (cmd, "SetMotor") == 0) cmdIdx = 3;

	// Skip silently unknown commands
	if (cmdIdx < 0) return;
	
	// System version
	if (cmdIdx == 0)
	{
		MSG_StartResponse (&gs_buffer, "SYS.Version");
		MSG_AddParamLong (&gs_buffer, MAJOR);
		MSG_AddParamLong (&gs_buffer, MINOR);
		MSG_AddParamLong (&gs_buffer, REVISION);

		MSG_FinishAndSend (&gs_buffer);
	}
	
	// Change control mode
	else if (cmdIdx == 1)
	{
		n = ExtractParamLong ();
		switch (n)
		{
		default:
		case 0: TMAIN_SetMode (MODE_Stop); break;
		case 1: TMAIN_SetMode (MODE_Free); break;
		case 2: TMAIN_SetMode (MODE_Control); break;
		}
	}	

	// Change telemetry uploading rate
	else if (cmdIdx == 2)
	{
		n = ExtractParamLong ();
		if (n < 0) n = 0;
		TMAIN_SetTelemetry (n);
	}	
	
	// Manual motor control
	else if (cmdIdx == 3)
	{
		// 0: disable, 1: brake, 2: forward, 3: reverse
		n = ExtractParamLong ();
		
		// Torque in %
		torque = ExtractParamLong ();

		// Set the command
		if (n == 0) CTRL_ManualMotorCommand (0, False);
		else if (n == 1) CTRL_ManualMotorCommand (0, True);
		else if (n == 2) CTRL_ManualMotorCommand (torque, False);
		else CTRL_ManualMotorCommand (-torque, False);
	}
}


// ===========================================================================
// <ProcessCommandBattery>
///
/// \brief	Process console command related to the battery
///
/// \param[in]	str		command:
// ===========================================================================
static void ProcessCommandBattery (const char* cmd)
{
	int cmdIdx = -1;
	Dword voltage;
	Word SOC;

	if (strcmp (cmd, "Voltage") == 0) cmdIdx = 0;
	else if (strcmp (cmd, "VoltageCalib") == 0) cmdIdx = 1;

	// Skip silently unknown commands
	if (cmdIdx < 0) return;

	// Battery voltage
	if (cmdIdx == 0)
	{
		MSG_StartResponse (&gs_buffer, "BATTERY.Voltage");

		// Read battery voltage [1/100V]
		voltage = BAT_GetVoltage ();
		MSG_AddParamLong (&gs_buffer, voltage);
		
		SOC = BAT_GetLipoSOC (voltage, 80);
		MSG_AddParamLong (&gs_buffer, SOC);

		// Send the response
		MSG_FinishAndSend (&gs_buffer);
	}

	// Voltage calibration
	else if (cmdIdx == 1)
	{
		voltage = ExtractParamLong ();
		TMAIN_CalibrateBattery (voltage);
	}
}


// ===========================================================================
// <ProcessCommandCalibration>
///
/// \brief	Process console command related to the calibration
///
/// \param[in]	str		command:
// ===========================================================================
static void ProcessCommandCalibration (const char* cmd)
{
	int cmdIdx = -1;
	Long gyroBias;
	Long accBias [3];
	Long accScale [2];

	if (strcmp (cmd, "IMU") == 0) cmdIdx = 0;
	
	// Skip silently unknown commands
	if (cmdIdx < 0) return;
	
	// Specify measured IMU bias
	if (cmdIdx == 0)
	{
		accScale [0] = ExtractParamLong ();
		accScale [1] = ExtractParamLong ();

		accBias [0] = ExtractParamLong ();
		accBias [1] = ExtractParamLong ();
		accBias [2] = ExtractParamLong ();

		gyroBias = ExtractParamLong ();
		
		TMAIN_CalibrateIMU (accScale, accBias, gyroBias);
	}
}


// ===========================================================================
// <ExtractParamLong>
///
/// \brief	Read the UART Rx flow in order to extract a Long parameter value
///
/// Parameters are enclosed like in the following pattern: ".{v1,v2,v3,...}"
///
/// \return Parameter value
// ===========================================================================
static Long ExtractParamLong ()
{
	Byte c;
	Long val = 0;
	Bool neg = False;	
	Bool err = False;
	Bool start = False;
	
	while (1)
	{
		UART_Read (g_UART, &c, 1, -1);
		if (start == False && (c == '.' || c == '{')) continue;
		if (c == ',' || c == '}' || c == '\r' || c == '\n') break;
		
		if (c == '-') neg = True;
		else if (c >= '0' && c <= '9') 
		{
			val *= 10;
			val += c - '0';
		}
		else err = True;

		start = True;
	}
	
	if (err) return 0;
	if (neg) return -val;
	return val;
}

// End
