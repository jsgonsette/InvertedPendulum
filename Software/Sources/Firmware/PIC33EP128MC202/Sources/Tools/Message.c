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
/// \file	Message.c
/// \author	Jean-Sebastien Gonsette
/// \date	20/10/2014
///
/// This module formats messages in a circular buffer, for the purpose
/// of being sent through the UART. These messages include Telemetries and Responses. 
// ###########################################################################

#include <string.h>
#include <stdlib.h>

#include "Version.h"
#include "Debug.h"
#include "Message.h"
#include "./Tasks/TaskUartTx.h"

#include "Global.h"


// ===========================================================================
// D E F I N E
// ===========================================================================


// ===========================================================================
// T Y P E S
// ===========================================================================


// ===========================================================================
// P R I V A T E
// ===========================================================================

/// Signalisation flags for the next message buffer
static unsigned int gs_sigFlags = 1;


// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Internal functions
//@{
static void WriteStart (MSG_Buffer *pMB);
static void WriteEnd (MSG_Buffer *pMB);
static void WriteString (MSG_Buffer *pMB, const char* str, int len);
static void BufferWrite (MSG_Buffer *pMB, const char* str, int len);
static int BufferFreeSpace (MSG_Buffer *pMB);
static void PopFree (MSG_Buffer *pMB);
//@}



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <MSG_InitBuffer>
///
/// \brief	Initialize a message buffer structure
///
/// \param		pMsgBuffer		Messages buffer to initialize
/// \param[in]	pBuffer			Raw buffer to use as a circular buffer
/// \param[in]	buffLen			Buffer length
// ===========================================================================
void MSG_InitBuffer (MSG_Buffer *pMsgBuffer, Byte* pBuffer, int buffLen)
{
	pMsgBuffer->pBuffer = pBuffer;
	pMsgBuffer->len = buffLen;
	pMsgBuffer->idxFree = 0;
	pMsgBuffer->idxData = 0;	
	pMsgBuffer->idxMsgStart = 0;	
	pMsgBuffer->sigFlags = gs_sigFlags;
	
	gs_sigFlags <<= 1;
	pBuffer [0] = 0;
}


// ===========================================================================
// <MSG_StartResponse>
///
/// \brief	Write the header of a Response into a buffer
///
/// A response has the following form:
/// =Token_1.Token_2...Token_n.{param_1,param_2, ...}\r\\n
///
/// \param		pMsgBuffer		Messages buffer
/// \param[in]	tokens			Tokens string: "Token_1.Token_2...Token_n"
// ===========================================================================
void MSG_StartResponse (MSG_Buffer *pMsgBuffer, const char* tokens)
{
	WriteStart (pMsgBuffer);
	
	// Build response up to the first parameter
	WriteString (pMsgBuffer, "=", 1);
	WriteString (pMsgBuffer, tokens, strlen (tokens));
	WriteString (pMsgBuffer, ".{", 2);
	
	pMsgBuffer->numParam = 0;
}


// ===========================================================================
// <MSG_StartTM>
///
/// \brief	Write the header of a Telemetry into a buffer
///
/// A telemetry has the following form:
/// #Token_1.Token_2...Token_n.{param_1,param_2, ...}\r\\n
///
/// \param		pMsgBuffer		Messages buffer
/// \param[in]	tokens			Tokens string: "Token_1.Token_2...Token_n"
// ===========================================================================
void MSG_StartTM (MSG_Buffer *pMsgBuffer, const char* tokens)
{
	WriteStart (pMsgBuffer);

	// Build response up to the first parameter
	WriteString (pMsgBuffer, "#", 1);
	WriteString (pMsgBuffer, tokens, strlen (tokens));
	WriteString (pMsgBuffer, ".{", 2);
	
	pMsgBuffer->numParam = 0;
}


// ===========================================================================
// <MSG_AddParamLong>
///
/// \brief	Add an integer parameter to a message being built
///
/// \param		pMsgBuffer		Messages buffer
/// \param[in]	v				Integer value
// ===========================================================================
void MSG_AddParamLong (MSG_Buffer *pMsgBuffer, Long v)
{
	int n;
	char buffer [12];
	
	// Add a separation ',' if necessary
	if (pMsgBuffer->numParam > 0) WriteString (pMsgBuffer, ",", 1);
	pMsgBuffer->numParam ++;
	
	// Convert parameter 
	ltoa (buffer, v, 10);
	n = strlen (buffer);	
	WriteString (pMsgBuffer, buffer, n);
}


// ===========================================================================
// <MSG_AddParamString>
///
/// \brief	Add an string parameter to a message being built
///
/// \param		pMsgBuffer		Messages buffer
/// \param[in]	str				String parameter
// ===========================================================================
void MSG_AddParamString (MSG_Buffer *pMsgBuffer, const char* str)
{
	// Add a separation ',' if necessary
	if (pMsgBuffer->numParam > 0) WriteString (pMsgBuffer, ",", 1);
	pMsgBuffer->numParam ++;
	
	// Convert parameter 
	WriteString (pMsgBuffer, str, strlen(str));
}


// ===========================================================================
// <MSG_FinishAndSend>
///
/// \brief	Finish the preparation of a message and send it to the UART Tx task
// ===========================================================================
void MSG_FinishAndSend (MSG_Buffer *pMB)
{
	char *p1, *p2;
	int l1, l2, len;
	int idx;
	
	// Write end of message
	WriteString (pMB, "}\r\n", 3);
	WriteEnd (pMB);

	// Get message length
	len = pMB->pBuffer [pMB->idxMsgStart];

	// Get message data location
	idx = (pMB->idxMsgStart +1) % pMB->len;
	
	p2 = (char*) pMB->pBuffer;
	p1 = (char*) pMB->pBuffer + idx;
	l1 = len;
	if (l1 > pMB->len - idx) 
	{
		l1 = pMB->len - idx;
		l2 = len - l1;
	}
	else l2 = 0;

	// Push message
	if (l2 == 0) TUARTTX_PushMessage (p1, l1, pMB->sigFlags);
	else TUARTTX_PushSplittedMessage (p1, l1, p2, l2, pMB->sigFlags);	
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################

// ===========================================================================
// <WriteStart>
///
/// \brief	Start the construction of the next message
///
/// \param		pMB		Circular buffer
// ===========================================================================
static void WriteStart (MSG_Buffer *pMB)
{
	Byte len;
	
	// Message length will start at the next free spot
	pMB->idxMsgStart = pMB->idxFree;
	
	// We don't know the final length now, just reserve space
	len = 0;
	WriteString (pMB, (char*) &len, 1);
}


// ===========================================================================
// <WriteEnd>
///
/// \brief	Finnish the construction of the current message
///
/// Total length is computed and written at the beginning of the message
///
/// \param		pMB		Circular buffer
// ===========================================================================
static void WriteEnd (MSG_Buffer *pMB)
{
	int len = 0;
	
	// Get message length
	if (pMB->idxFree > pMB->idxMsgStart) len = (pMB->idxFree - pMB->idxMsgStart -1);
	else len = (pMB->len -1) - (pMB->idxMsgStart - pMB->idxFree);
	
	ASSERT (len > 0);
	ASSERT (len < pMB->len);
	
	// Write message length
	pMB->pBuffer [pMB->idxMsgStart] = (Byte) len;
}


// ===========================================================================
// <WriteString>
///
/// \brief	Write a full string in the buffer. 
///
/// Function blocks until the write operation can be terminated
///
/// \param		pMB		Circular buffer
/// \param [in]	str		String to write in the buffer
/// \param [in]	len		String length
// ===========================================================================
static void WriteString (MSG_Buffer *pMB, const char* str, int len)
{
	int n;

	// Loop until we could push everything
	while (1)
	{
		// Remove messages that are no longer needed
		PopFree (pMB);
		
		// Get space and write what we can
		n = BufferFreeSpace (pMB);
		if (n > len) n = len;
		BufferWrite (pMB, str, n);

		str += n;
		len -= n;
		if (len == 0) break;
	
		// Wait some room is made in the buffer
		TUARTTX_WaitMessageSent (pMB->sigFlags);
	}
}


// ===========================================================================
// <BufferWrite>
///
/// \brief	Write content in the buffer with circular wrapping
///
/// This function doesn't check if there is enough room to write the content
///
/// \param		pMB		Circular buffer
/// \param [in]	str		String to write in the buffer
/// \param [in]	len		String length
// ===========================================================================
static void BufferWrite (MSG_Buffer *pMB, const char* str, int len)
{
	int space;
	
	ASSERT (pMB->idxFree >= 0);
	ASSERT (pMB->idxFree < pMB->len);
	ASSERT (len < pMB->len);

	// Space until end of buffer
	space = (pMB->len - pMB->idxFree);
	
	// Copy all at once if possible
	if (len <= space) 
	{
		memcpy (pMB->pBuffer + pMB->idxFree, str, len);
		pMB->idxFree += len;
		if (pMB->idxFree == pMB->len) pMB->idxFree = 0;
	}
	// Wrap around end of buffer
	else
	{
		memcpy (pMB->pBuffer + pMB->idxFree, str, space);
		memcpy (pMB->pBuffer, str + space, len-space);
		pMB->idxFree = len-space;
	}
	
	ASSERT (pMB->idxFree >= 0);
	ASSERT (pMB->idxFree < pMB->len);
}


// ===========================================================================
// <BufferFreeSpace>
///
/// \brief	Return available space in the buffer
///
/// \param		pMB		Circular buffer
///
/// \return Free space
// ===========================================================================
static int BufferFreeSpace (MSG_Buffer *pMB)
{
	int free;
	
	if (pMB->idxFree >= pMB->idxData) 
		free = (pMB->len -1) - (pMB->idxFree - pMB->idxData);
	
	else free = (pMB->idxData - pMB->idxFree -1);
		
	ASSERT (free >= 0 && free < pMB->len);
	return free;
}


// ===========================================================================
// <PopFree>
///
/// \brief	Make room in the circular buffer by removing messages that have been sent
///
/// Message is flagged as 'sent' when its first byte is 0
/// Message is under construction when length field is 0
///
/// \param		pMB		Circular buffer
// ===========================================================================
static void PopFree (MSG_Buffer *pMB)
{
	int idx;
	Byte len;
	Byte firstData;

	while (1)
	{
		// Get message length
		idx = pMB->idxData;
		len = pMB->pBuffer [idx];
		
		// Get message first data
		idx = (idx +1) % pMB->len;
		firstData = pMB->pBuffer [idx];
		
		// Stop if we reach a message not sent already, or if we loopback
		if (pMB->idxData == pMB->idxFree || len == 0 || firstData != 0) break;

		// Erase length field
		pMB->pBuffer [pMB->idxData] = 0;

		// Skip current message
		ASSERT (len < pMB->len);
		pMB->idxData = (pMB->idxData + len + 1) % pMB->len;		
	}
}

// END
