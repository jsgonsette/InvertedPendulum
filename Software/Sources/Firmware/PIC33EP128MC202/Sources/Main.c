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
//
/// \file	Main.c
/// \author	Jean-Sebastien Gonsette
/// \date	17/09/2014
///
/// Entry point for iPendulum application. This module loads the configuration,
/// set up the hardware and the OS. Then it launches the different tasks.
// ###########################################################################

#include "Main.h"
#include "Version.h"
#include "BSP.h"
#include "Global.h"
#include "Pendulum.h"
#include "./Interfaces/Battery.h"
#include "./Interfaces/Motor.h"
#include "./Tasks/Controller.h"
#include "./Tasks/TaskUartTx.h"
#include "./Tasks/TaskUartRx.h"
#include "./Tasks/TaskLED.h"
#include "./Tasks/TaskMain.h"


// ===========================================================================
// D E F I N E
// ===========================================================================

// Task priority levels
#define TASK_PRIO_LED           1
#define TASK_PRIO_MAIN          2
#define TASK_PRIO_UART_TX      	3
#define TASK_PRIO_UART_RX      	4


// ===========================================================================
// P R I V A T E 
// ===========================================================================


// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Module private functions
//@{
//@}



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <main>
///
/// \brief		Entry point: Set up the hardware, os and application
// ===========================================================================
int main ()
{
	const PDL_Physics* p;

	// Hardware, OS, drivers and devices initialization
	BSP_Init ();

	// Application initialization
	PDL_Init ();
	TUARTRX_Init ();
	TUARTTX_Init ();
	CTRL_Init ();

	// Motor and Battery initialization
	p = PDL_GetPhysics ();

	MOTOR_Init (p->motor.I);
	BAT_Init (BAT_CAPACITY);

	// Create all applicative tasks
	g_taskMain = CreateTask (TMAIN_EntryPoint, "Task_Main", 600, null, TASK_PRIO_MAIN, 0);
	g_taskUartTx = CreateTask (TUARTTX_EntryPoint, "Task_UART_Tx", 500, null, TASK_PRIO_UART_TX, 0);
	g_taskUartRx = CreateTask (TUARTRX_EntryPoint, "Task_UART_Rx", 500, null, TASK_PRIO_UART_RX, 0);
	g_taskLED = CreateTask (TLED_EntryPoint, "Task_LED", 300, null, TASK_PRIO_LED, 0);

	SuspendTask (g_taskUartTx);
	SuspendTask (g_taskUartRx);
	
	// Start OS
	StartKernel ();

    // ...
	// Reset
	__asm__ ("reset");
	return 0;
}



// End
