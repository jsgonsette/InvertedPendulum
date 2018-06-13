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
/// \file 	TaskUartTx.h
/// \author	Jean-Sebastien Gonsette
/// \date	17/09/2014
///
// ###########################################################################

#ifndef _TASKUARTTX_H
#define _TASKUARTTX_H


// ===========================================================================
// I N C L U D E
// ===========================================================================



// ###########################################################################
//
// T Y P E S
//
// ###########################################################################



// ###########################################################################
//
// P R O T O T Y P E S
//
// ###########################################################################

void TUARTTX_Init ();
void TUARTTX_EntryPoint (void* param);

Bool TUARTTX_PushMessage (char* pMsg, int msgLen, unsigned int sigMask);
Bool TUARTTX_PushSplittedMessage (char* pMsg_1, int msgLen_1, char* pMsg_2, int msgLen_2, unsigned int sigMask);
void TUARTTX_WaitMessageSent (unsigned int sigMask);

#endif
