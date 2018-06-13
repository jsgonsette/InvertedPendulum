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
/// \file 	Message.h
/// \author	Jean-Sebastien Gonsette
/// \date	20/10/2014
///
// ###########################################################################

#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "NanoScale.h"



// ###########################################################################
//
// T Y P E S
//
// ###########################################################################

// ===========================================================================
/// Message buffer management
// ===========================================================================
typedef struct
{
	Byte* pBuffer;			///< Raw buffer
	int len;				///< Raw buffer length

	int idxFree;			///< First free byte index
	int idxData;			///< First data byte index
	int idxMsgStart;		///< Current message place holder for length
	
	int numParam;			///< Number of parameters in the message
	unsigned int sigFlags;	///< Signalisation flags when a message is sent
}
MSG_Buffer;



// ###########################################################################
//
// P R O T O T Y P E S
//
// ###########################################################################

void MSG_InitBuffer (MSG_Buffer *pMsgBuffer, Byte* pBuffer, int buffLen);
void MSG_StartResponse (MSG_Buffer *pMsgBuffer, const char* tokens);
void MSG_StartTM (MSG_Buffer *pMsgBuffer, const char* tokens);
void MSG_AddParamLong (MSG_Buffer *pMsgBuffer, Long v);
void MSG_AddParamString (MSG_Buffer *pMsgBuffer, const char* str);
void MSG_FinishAndSend (MSG_Buffer *pMB);

#endif