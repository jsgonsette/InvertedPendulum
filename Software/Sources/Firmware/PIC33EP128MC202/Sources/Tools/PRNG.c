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
/// \file	PRNG.c
/// \author	Jean-Sebastien Gonsette
/// \date	07/02/2015
///
/// This module implements a Pseudo Random Number Generator based on a
/// 16 bits Linear Feedback State Register
// ###########################################################################

#include "PRNG.h"


// ===========================================================================
// D E F I N E
// ===========================================================================

#define MAX_DWORD		0xFFFFFFFF


// ===========================================================================
// T Y P E S
// ===========================================================================


// ===========================================================================
// P R I V A T E
// ===========================================================================

/// LFSR 
static Word gs_register = 0;


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
// <PRNG_Seed>
///
/// \brief	Seed based generator initialisation
///
/// \param [in]		pSeed		Seed
/// \param [in]		seedLen		Seed length [B]
// ===========================================================================
void PRNG_Seed (const Byte* pSeed, int seedLen)
{
	int i;
	Byte b;
	
	// Init value, can be anything
	gs_register = 0x55AA;
	
	// Combine every byte of the seed
	for (i = 0; i < seedLen; i ++)
	{
		// XOR with LSB, then roll the register
		b = (Byte) ((gs_register & 0xFF00) >> 8);
		gs_register ^= pSeed [i];
		gs_register <<= 8;
		gs_register |= b;
	}
	
	// Register must never be 0
	if (gs_register == 0) gs_register ++;
}


// ===========================================================================
// <PRNG_Out>
///
/// \brief	Output random bytes
///
/// Galois LFSR implementation with x^16 + x^14 + x^13 + x^11 + 1 polynomial.
///
/// \param [out]	pOut	Output buffer
/// \param [in]		len		Number of Bytes to generate
// ===========================================================================
void PRNG_Out (Byte* pOut, int len)
{
	int i;
	
	// Generate the required length
	while (len-- > 0)
	{
		// 1 Byte generation
		pOut [0] = 0;
		for (i = 0; i < 8; i ++)
		{
			pOut [0] <<= 1;
			
			if (gs_register & 0x01)
			{
				pOut [0] |= 1;
				gs_register >>= 1;
				gs_register ^= 0xB400;
			}
			else gs_register >>= 1;
		}
		
		pOut ++;
	}
}


// ===========================================================================
// <PRNG_Rand>
///
/// \brief	Generate a random number with a uniform pdf in a given interval
///
/// \param	min		Minimum bound of the interval
/// \param	max		Maximum bound of the interval
///
/// \return	Random number in interval [min;max]
// ===========================================================================
Long PRNG_Rand (Long min, Long max)
{
	Dword range;
	Dword longInterval;
	Dword rnd;
	int safety;
	
	// Trivial cases
	if (min > max) return 0;
	if (min == max) return min;

	// Number of elements in the interval
	range = (Dword) (max-min+1);

	// Full range case
	if (range == 0) {
		PRNG_Out ((Byte*) &rnd, sizeof (rnd));
		return rnd;		
	}	
	
	// Restrict long interval so that it is a multiple of 'range'
	longInterval = (MAX_DWORD % range) +1;
	if (longInterval == range) longInterval = 0;
	longInterval = MAX_DWORD - longInterval;
	
	// Get a number in the long interval
	// As long interval min value is equal to 2^32 / 2, safety counter
	// should statistically never overflow
	safety = 0;
	do {
		PRNG_Out ((Byte*) &rnd, sizeof (rnd));
		ASSERT (safety ++ < 100);
	}
	while (rnd > longInterval);
	
	// Scale it
	rnd %= range;
	rnd += min;
	
	return ((Long) rnd);
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################

// End
