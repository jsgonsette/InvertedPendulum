// ###########################################################################
//
// N A N O S C A L E   R T O S
//
/// \file		SDK.h
/// \version	2.3.1
/// \date		31/08/2012
/// \author		GenId SPRL
//
//  ###########################################################################

#ifndef _NANO_SDK_H
#define _NANO_SDK_H

#include <stddef.h>
#include "NanoScale.h"
#include "NanoScaleDrivers.h"

#define low_word(dw) ((Word)dw)
#define low_byte(w) ((Byte)w)
#define high_word(dw) ((Word) (dw>>16))
#define high_byte(w) ((Byte) (w>>8))
			
			
Word BigEndian16 (Word value);
Dword BigEndian32 (Dword value);
Word LittleEndian16 (Word value);
Dword LittleEndian32 (Dword value);
Word MakeBigEndian (Byte MSB, Byte LSB);
Word MakeLittleEndian (Byte MSB, Byte LSB);
Bool System_IsKernel ();



// ###########################################################################
//
// T Y P E S
//
// ###########################################################################

// ===========================================================================
// P H Y
// ===========================================================================
/// MII Mode
typedef enum
{
	PHY_MII = 0,		///< MII interface
	PHY_RMII = 1		///< RMII interface
}
PHY_MII_MODE;

/// Pause option
typedef enum
{
	PAUSE_TX = 1,		///< Pause Transmit should be enabled in the MAC
	PAUSE_RX = 2		///< Pause Receive should be enabled in the MAC
}
PHY_PAUSE_AN;


// ===========================================================================
// M A C
// ===========================================================================

/// Ethernet MAC header
typedef struct  __attribute__((aligned(2), packed))
{
	MAC_ADDR        DestMACAddr;		///< Adresse MAC du destinataire.
	MAC_ADDR        SourceMACAddr;		///< Adresse MAC de l'expéditeur.
	Word	        Type;				///< Protocole utilisant le service MAC (Ex: IP, ARP)
} ETHER_HEADER;



// ###########################################################################
//
// V i r t u a l   F u n c t i o n s
//
// ###########################################################################

// ===========================================================================
// D e v i c e
// ===========================================================================
typedef void (*pKernelCallback) (HANDLE device);


// ===========================================================================
// P H Y
// ===========================================================================
typedef void (*pPHY_Init) (HANDLE device, PHY_MII_MODE miiMode);
typedef Dword (*pPHY_GetClockSpeed) (HANDLE device);
typedef void (*pPHY_GetNegotiationResult) (HANDLE device, PHY_DPX_Mode* pDpxMode, PHY_PAUSE_AN* pPauseAn);
typedef void (*pPHY_Callback) (HANDLE device);

/// Virtual functions set
typedef struct
{
	pPHY_Init InitCS;									///< Initialize device
	pPHY_GetClockSpeed GetClockSpeedCS;					///< Get device clock speed
	pPHY_GetNegotiationResult GetNegotiationResultCS;	///< Get Auto negotiation results
	pPHY_Callback CallbackKER;							///< Callback in case of interrupt
}
DrvPhy_Virtual;


// ===========================================================================
// M A C
// ===========================================================================
typedef Bool (*pMAC_StartUsage) (HANDLE device);
typedef void (*pMAC_EndUsage) (HANDLE device);
typedef MAC_LINK (*pMAC_GetLinkStatus) (HANDLE device);
typedef Word (*pMAC_GetTxFree) (HANDLE device);
typedef Bool (*pMAC_NewTxPkt) (HANDLE device, MAC_ADDR* remote, E_MAC_TYPE type, Word dataLen);
typedef void (*pMAC_WriteTxPkt) (HANDLE device, Byte* data, Word offset, Word len);
typedef void (*pMAC_FlushTxPkt) (HANDLE device);
typedef Word (*pMAC_GetRxPkt) (HANDLE device, MAC_ADDR *remote, E_MAC_TYPE* type);
typedef void (*pMAC_ReadRxPkt) (HANDLE device, Byte* buffer, Word offset, Word len);
typedef void (*pMAC_DiscardRxPkt) (HANDLE device);
typedef void (*pMAC_Callback) (HANDLE device);

/// Virtual functions set
typedef struct
{
	pMAC_StartUsage StartUsage;		///< Lock Device
	pMAC_EndUsage EndUsage;			///< Unlock device
	pMAC_GetLinkStatus GetLinkStatus;///< Get MAC device link status
	pMAC_GetTxFree GetTxFree;		///< Free space in Tx buffer
	pMAC_NewTxPkt NewTxPkt;			///< Start packet creation (Tx)
	pMAC_WriteTxPkt WriteTxPkt;		///< Write data in current packet (Tx)
	pMAC_FlushTxPkt FlushTxPkt;		///< Discard and send current packet (Tx)
	pMAC_GetRxPkt GetRxPkt;			///< Start reading next queued packet (Rx)
	pMAC_ReadRxPkt ReadRxPkt;		///< Read data from current packet (Rx)
	pMAC_DiscardRxPkt DiscardRxPkt;	///< Disacard current packet (Rx)
	pMAC_Callback CallbackKER;		///< Callback in case of interrupt
}
DrvMac_Virtual;



// ###########################################################################
//
// P R O T O T Y P E S
//
// ###########################################################################

// D E V I C E

NS_SDK HANDLE Drv_OpenDevice (HANDLE driver, Word userSize, const char* name, Bool lock);

NS_SDK HANDLE Drv_OpenDriver (Word userSize, const char* name, const char* version, pKernelCallback callback);

NS_SDK HANDLE Drv_GetDriver (HANDLE device);

NS_SDK void Drv_CloseDevice (HANDLE device);

NS_SDK void* Drv_HandleToDevice (HANDLE device);

NS_SDK void* Drv_HandleToDriver (HANDLE driver);

NS_SDK void Drv_DeviceDirty (HANDLE device);

NS_SDK void Drv_DeviceBusyISR (HANDLE device, Bool busy);


// P H Y

NS_SDK HANDLE PHY_OpenDriver (Word userSize, const char* name, const char* version, DrvPhy_Virtual* funcs);

NS_SDK HANDLE PHY_OpenDevice (HANDLE driver, Word userSize, const char* name, Byte phyAddr);

NS_SDK void PHY_CloseDevice (HANDLE device);

NS_SDK void* PHY_HandleToDevice (HANDLE device);

NS_SDK void* PHY_HandleToDriver (HANDLE driver);


// M A C

NS_SDK HANDLE MAC_OpenDriver (Word userSize, const char* name, const char* version, DrvMac_Virtual* funcs);

NS_SDK HANDLE MAC_OpenDevice (HANDLE driver, Word userSize, const char* name, Bool lock);

NS_SDK void MAC_CloseDevice (HANDLE device);

NS_SDK void MAC_SignalKER (HANDLE device, unsigned int flags);

NS_SDK void MAC_SignalMac (HANDLE device, MAC_ADDR mac);

NS_SDK void* MAC_HandleToDevice (HANDLE device);

NS_SDK void* MAC_HandleToDriver (HANDLE driver);

NS_SDK Word MAC_MiiRead (Byte phy, Byte reg);

NS_SDK void MAC_MiiWrite (Byte phy, Byte reg, Word value);

NS_SDK void MAC_MiiSignalKER (unsigned int flags);


#endif

// End
