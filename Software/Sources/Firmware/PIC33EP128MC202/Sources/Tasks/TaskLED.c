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
/// \file	TaskLED.c
/// \author	Jean-Sebastien Gonsette
/// \date	08/10/2014
///
/// This module drives the Red, Yellow and Green LED with configurable 
///	blinking patterns.
// ###########################################################################

#include "BSP.h"
#include "Global.h"
#include "TaskLED.h"


// ===========================================================================
// D E F I N E
// ===========================================================================


// ===========================================================================
// T Y P E S
// ===========================================================================

/// LED state machine
typedef struct
{
	Byte nOn;		///< Config: number of steps with LED on;
	Byte nOff;		///< Config: number of steps with LED off;
	Byte nRep;		///< Config: number of on/off pattern repetition (0: infinite)
	Byte nSep;		///< Config: number of steps between 2 on/off patterns
	
	Byte step0;		///< Pattern counter
	Byte step1;		///< Repetition counter
}
S_LSM;


// ===========================================================================
// P R I V A T E
// ===========================================================================

/// LED state machines (green, yellow, red)
static S_LSM gs_lsm [3];


// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Internal functions
//@{
static void ProcessLedPause (int ledNumber);
static void ProcessLedBlink (int ledNumber);
static void SwitchLedOn (E_LED led);
static void SwitchLedOff (E_LED led);
//@}



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <TLED_EntryPoint>
///
/// \brief		LED management entry point
///
/// This task drives the different LED according to configurable blinking speeds
// ===========================================================================
void TLED_EntryPoint (void* param)
{
	int i;
	
	while (1)
	{
		// Cadencing
		SleepTask (DELAY_MS (LED_SM_PERIOD));

		// State Machine for the 3 LED
		for (i = 0; i < 3; i ++)
		{
			// Pause phase
			if (gs_lsm [i].nRep > 0 && gs_lsm [i].step1 == gs_lsm [i].nRep) ProcessLedPause (i);
		
			// ON/OFF phase
			else ProcessLedBlink (i);
		}
	}
}


// ===========================================================================
// <TLED_Off>
///
/// \brief		Switch a LED off
///
/// \param[in]	led		Target LED
// ===========================================================================
void TLED_Off (E_LED led)
{
	gs_lsm [led].nOff = 1;
	gs_lsm [led].nOn = 0;
	gs_lsm [led].nRep = 0;

	gs_lsm [led].step0 = 0;
	gs_lsm [led].step1 = 0;
}


// ===========================================================================
// <TLED_On>
///
/// \brief		Switch a LED on
///
/// \param[in]	led		Target LED
// ===========================================================================
void TLED_On (E_LED led)
{
	gs_lsm [led].nOff = 0;
	gs_lsm [led].nOn = 1;
	gs_lsm [led].nRep = 0;

	gs_lsm [led].step0 = 0;
	gs_lsm [led].step1 = 0;
}


// ===========================================================================
// <TLED_Blink>
///
/// \brief		Make a LED blink
///
/// \param[in]	led		Target LED
/// \param[in]	T		Periodicity (Minimum is 1)
// ===========================================================================
void TLED_Blink (E_LED led, Byte T)
{
	gs_lsm [led].nOff = T;
	gs_lsm [led].nOn = T;
	gs_lsm [led].nRep = 0;

	gs_lsm [led].step0 = 0;
	gs_lsm [led].step1 = 0;
}


// ===========================================================================
// <TLED_Pulse>
///
/// \brief		Make a LED pulse
///
/// The LED pulses a given number of time, then a blank is inserted, then 
/// this process restart.
///
/// \param[in]	led		Target LED
/// \param[in]	numRep	Number of repetitions of the pulse
// ===========================================================================
void TLED_Pulse (E_LED led, Byte numRep)
{
	gs_lsm [led].nOff = 2;
	gs_lsm [led].nOn = 1;
	gs_lsm [led].nSep = 9;
	gs_lsm [led].nRep = numRep;

	gs_lsm [led].step0 = 0;
	gs_lsm [led].step1 = 0;
}


// ===========================================================================
// <TLED_PingPongOut>
///
/// \brief		Configure the LED SM in order to get the following pattern:
///				[*__][_*_][__*][___][__*][_*_][*__][___]
// ===========================================================================
void TLED_PingPongOut (Byte T)
{
	gs_lsm [LED_RED].nOn = 1;
	gs_lsm [LED_RED].nOff = 1;
	gs_lsm [LED_RED].nRep = 2;
	gs_lsm [LED_RED].nSep = 4;

	gs_lsm [LED_YELLOW].nOn = 1;
	gs_lsm [LED_YELLOW].nOff = 3;
	gs_lsm [LED_YELLOW].nRep = 0;
	gs_lsm [LED_YELLOW].nSep = 0;

	gs_lsm [LED_GREEN].nOn = 1;
	gs_lsm [LED_GREEN].nOff = 1;
	gs_lsm [LED_GREEN].nRep = 2;
	gs_lsm [LED_GREEN].nSep = 4;

	gs_lsm [LED_RED].step0 = 1;
	gs_lsm [LED_RED].step1 = 0;

	gs_lsm [LED_YELLOW].step0 = 2;
	gs_lsm [LED_YELLOW].step1 = 0;

	gs_lsm [LED_GREEN].step0 = 1;
	gs_lsm [LED_GREEN].step1 = 2;
}


// ===========================================================================
// <TLED_PingPongIn>
///
/// \brief		Configure the LED SM in order to get the following pattern:
///				[*__][_*_][__*][_*_]
// ===========================================================================
void TLED_PingPongIn (Byte T)
{
	gs_lsm [LED_RED].nOn = 1;
	gs_lsm [LED_RED].nOff = 3;
	gs_lsm [LED_RED].nRep = 0;

	gs_lsm [LED_YELLOW].nOn = 1;
	gs_lsm [LED_YELLOW].nOff = 1;
	gs_lsm [LED_YELLOW].nRep = 0;

	gs_lsm [LED_GREEN].nOn = 1;
	gs_lsm [LED_GREEN].nOff = 3;
	gs_lsm [LED_GREEN].nRep = 0;

	gs_lsm [LED_RED].step0 = 0;
	gs_lsm [LED_GREEN].step0 = 2;
	gs_lsm [LED_YELLOW].step0 = 1;
}


// ===========================================================================
// <TLED_RollRight>
///
/// \brief		Configure the LED SM in order to get the following pattern:
///				[*__][_*_][__*][___][___]
// ===========================================================================
void TLED_RollRight (Byte T)
{
	gs_lsm [LED_RED].nOn = 1;
	gs_lsm [LED_RED].nOff = 4;
	gs_lsm [LED_RED].nRep = 0;
	gs_lsm [LED_RED].step0 = 4;

	gs_lsm [LED_YELLOW].nOn = 1;
	gs_lsm [LED_YELLOW].nOff = 4;
	gs_lsm [LED_YELLOW].nRep = 0;
	gs_lsm [LED_YELLOW].step0 = 3;

	gs_lsm [LED_GREEN].nOn = 1;
	gs_lsm [LED_GREEN].nOff = 4;
	gs_lsm [LED_GREEN].nRep = 0;
	gs_lsm [LED_GREEN].step0 = 2;
}


// ===========================================================================
// <TLED_RollLeft>
///
/// \brief		Configure the LED SM in order to get the following pattern:
///				[__*][_*_][*__][___][___]
// ===========================================================================
void TLED_RollLeft (Byte T)
{
	gs_lsm [LED_RED].nOn = 1;
	gs_lsm [LED_RED].nOff = 4;
	gs_lsm [LED_RED].nRep = 0;
	gs_lsm [LED_RED].step0 = 3;

	gs_lsm [LED_YELLOW].nOn = 1;
	gs_lsm [LED_YELLOW].nOff = 4;
	gs_lsm [LED_YELLOW].nRep = 0;
	gs_lsm [LED_YELLOW].step0 = 4;

	gs_lsm [LED_GREEN].nOn = 1;
	gs_lsm [LED_GREEN].nOff = 4;
	gs_lsm [LED_GREEN].nRep = 0;
	gs_lsm [LED_GREEN].step0 = 0;
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################

// ===========================================================================
// <ProcessLedPause>
///
/// \brief		LED state machine processing during the 'pause' phase
///
/// \param[in]	ledNumber		Index of the target LED
// ===========================================================================
static void ProcessLedPause (int ledNumber)
{
	// LED is off during the pause
	SwitchLedOff (ledNumber);

	// Advance state machine counter
	// Pause ends when 'step0' reach the configurable param 'nSep'
	gs_lsm [ledNumber].step0 ++;
	if (gs_lsm [ledNumber].step0 >= gs_lsm [ledNumber].nSep)
	{
		gs_lsm [ledNumber].step0 = 0;
		gs_lsm [ledNumber].step1 = 0;
	}	
}


// ===========================================================================
// <ProcessLedBlink>
///
/// \brief		LED state machine processing during the 'blinking' phase
///
/// \param[in]	ledNumber		Index of the target LED
// ===========================================================================
static void  ProcessLedBlink (int ledNumber)
{
	// LED is either ON of OFF according to 'step0' counter
	if (gs_lsm [ledNumber].step0 < gs_lsm [ledNumber].nOn)
		SwitchLedOn (ledNumber);
	else if (gs_lsm [ledNumber].step0 < gs_lsm [ledNumber].nOn + gs_lsm [ledNumber].nOff)
		SwitchLedOff (ledNumber);

	// Advance state machine counter
	// If current LED blink is finished, we increment counter 'step1' and do it again
	gs_lsm [ledNumber].step0 ++;
	if (gs_lsm [ledNumber].step0 >= gs_lsm [ledNumber].nOn + gs_lsm [ledNumber].nOff)
	{
		gs_lsm [ledNumber].step0 = 0;
		if (gs_lsm [ledNumber].nRep > 0) gs_lsm [ledNumber].step1 ++;
	}
}


// ===========================================================================
// <SwitchLedOn>
///
/// \brief		Switch a given LED on
///
/// \param[in]	led		Target LED
// ===========================================================================
static void SwitchLedOn (E_LED led)
{
	switch (led)
	{
	case LED_RED: BSP_LEDSetRed (1); break;
	case LED_YELLOW: BSP_LEDSetYellow (1); break;
	case LED_GREEN: BSP_LEDSetGreen (1); break;
	default: break;
	}
}


// ===========================================================================
// <SwitchLedOff>
///
/// \brief		Switch a given LED off
///
/// \param[in]	led		Target LED
// ===========================================================================
static void SwitchLedOff (E_LED led)
{
	switch (led)
	{
	case LED_RED: BSP_LEDSetRed (0); break;
	case LED_YELLOW: BSP_LEDSetYellow (0); break;
	case LED_GREEN: BSP_LEDSetGreen (0); break;
	default: break;
	}
}

// End
