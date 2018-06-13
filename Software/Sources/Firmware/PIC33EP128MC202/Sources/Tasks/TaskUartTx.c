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
/// \file	TaskUartTx.c
/// \author	Jean-Sebastien Gonsette
/// \date	17/09/2014
///
/// This module sends formatted messages through the UART port.
// ###########################################################################

#include "NanoScale.h"
#include "NanoScaleDrivers.h"

#include "BSP.h"
#include "Global.h"
#include "Debug.h"
#include "TaskUartTx.h"


// ===========================================================================
// D E F I N E
// ===========================================================================

/// Number of slots in the message queue
#define NUM_SLOTS		20


// ===========================================================================
// T Y P E S
// ===========================================================================

/// Queue item describing a message
typedef struct
{
	char* pMsg_1;			///< Message, part 1
	int msgLen_1;			///< Len of message, part1
	char* pMsg_2;			///< Message, part 2
	int msgLen_2;			///< Len of message, part2
	
	unsigned int sigMask;	///< Signalisation mask
}
S_Queue_Item;


// ===========================================================================
// P R I V A T E
// ===========================================================================

/// MQ to store the UART outgoing messages
static HANDLE gs_queue;

/// Event to signal message emission
static HANDLE gs_event;



// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Internal functions
//@{
//@}



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <TUARTTX_Init>
///
/// \brief		UART - Rx module initialisation
// ===========================================================================
void TUARTTX_Init ()
{
	// Create a MQ for incoming messages
	gs_queue = CreateMq (sizeof (S_Queue_Item), NUM_SLOTS);
	
	// Create event to signal message emission
	gs_event = CreateEvent ();
}


// ===========================================================================
// <TUARTTX_EntryPoint>
///
/// \brief		UART - Tx entry point
///
/// Messages are extracted from a dedicted queue, then written through the UART
// ===========================================================================
void TUARTTX_EntryPoint (void* param)
{
	S_Queue_Item qi;
	
	while (1)
	{
		// Get the next message to send
		ReadMq (gs_queue, (Byte*) &qi, sizeof (S_Queue_Item));
		
		// Write it to the UART (can be in 2 parts)
		UART_Write (g_UART, (Byte*) qi.pMsg_1, qi.msgLen_1, -1);
		if (qi.msgLen_2 > 0) UART_Write (g_UART, (Byte*) qi.pMsg_2, qi.msgLen_2, -1);

		// Then erase first byte of the message to give a clue it has been processed
		qi.pMsg_1 [0] = 0;		
		
		// Signalisation that message is sent
		SetEvent (gs_event, qi.sigMask);
	}
}


// ===========================================================================
// <TUARTTX_PushMessage>
///
/// \brief		Psuh a new message to send through the UART
///
/// This function is completely asynchronous and doesn't block.
///
/// \param		pMsg		Message to send
/// \param		msgLen		Length of the message [B]
/// \param		sigMask		Expected mask for signalisation when message is sent
///
/// \return		True if the message could be added to the queue
// ===========================================================================
Bool TUARTTX_PushMessage (char* pMsg, int msgLen, unsigned int sigMask)
{
	return TUARTTX_PushSplittedMessage (pMsg, msgLen, null, 0, sigMask);
}


// ===========================================================================
// <TUARTTX_PushSplittedMessage>
///
/// \brief		Psuh a new message (in 2 parts) to send through the UART
///
/// This function is completely asynchronous and doesn't block.
///
/// \param		pMsg_1		First part of the message to send
/// \param		msgLen_1	Length of the message, part 1 [B]
/// \param		pMsg_2		Second part of the message to send
/// \param		msgLen_2	Length of the message, part 2 [B]
/// \param		sigMask		Expected mask for signalisation when message is sent
///
/// \return		True if the message could be added to the queue
// ===========================================================================
Bool TUARTTX_PushSplittedMessage (char* pMsg_1, int msgLen_1, char* pMsg_2, int msgLen_2, unsigned int sigMask)
{
	S_Queue_Item qi;
	Word n;
	
	// Write the queue item and push it to the send queue (no timeout)
	qi.pMsg_1 = pMsg_1;
	qi.msgLen_1 = msgLen_1;
	qi.pMsg_2= pMsg_2;
	qi.msgLen_2 = msgLen_2;
	qi.sigMask = sigMask;
	
	n = WriteMqTo (gs_queue, (Byte*) &qi, sizeof (qi), 0);
	
	// If no enough room in the queue
	if (n != sizeof (qi)) 
	{
		ThrowError (ERR_UART_QUEUE);
		return False;
	}
	
	// Ok
	return True;
}


// ===========================================================================
// <TUARTTX_WaitMessageSent>
///
/// \brief		Wait until a given message is sent through the UART
///
/// The message is identified by a signalisation flags
///
/// \param		sigMask		Signalisation flags generated when message is sent
// ===========================================================================
void TUARTTX_WaitMessageSent (unsigned int sigMask)
{
	WaitEvent (gs_event, EV_WAIT_ALL, EV_RESET, sigMask);
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################


// End
