// ###########################################################################
//
// N A N O S C A L E   R T O S
//
/// \file		NanoScaleDrivers.h
/// \version	2.3.1
/// \date		11/04/2013
/// \author		GenId SPRL
///
/// This file contains the API definition of NanoScale Drivers. 
//
// ###########################################################################

#ifndef _NANOSCALEDRIVERS_H
#define _NANOSCALEDRIVERS_H

#include "NanoScale.h"



// ###########################################################################
//
// E R R O R   C O D E S
//
// ###########################################################################

// General errors
#define	ERR_DEVICE_NOMEM			200	///< No enough memory to create the device
#define	ERR_DEVICE_EXIST			201	///< Device already exist
#define	ERR_DEVICE_DELETED			202	///< Device deleted



// ###########################################################################
//
// D E F I N E S
//
// ###########################################################################

/// I2C events flags
#define I2C_FLAG_SLV_MSG_RECEIVED		1
#define I2C_FLAG_SLV_RSP_SENT			2
#define I2C_FLAG_MST_MSG_SENT			4
#define I2C_FLAG_MST_RSP_RECEIVED		8
#define I2C_FLAG_MST_TRF_COMPLETE		16

/// MAC events flags
#define MAC_FLAG_PKT_SENT				1
#define MAC_FLAG_PKT_RECEIVED			2
#define MAC_FLAG_LINK_CHANGED			4

/// PHY events flags
#define PHY_FLAG_MODE_CHANGED			1
#define PHY_FLAG_LINK_CHANGED			2
#define PHY_FLAG_AUTO_NEG_COMPLETE		4



// ###########################################################################
//
// T Y P E S
//
// ###########################################################################

// ===========================================================================
// S P I
// ===========================================================================
/// SPI device status
typedef enum 
{
	SPI_ST_IDLE = 0,			///< Ready for new transaction
	SPI_ST_ON_HOLD,				///< Waiting for master
	SPI_ST_PENDING				///< Transaction ongoing
}
E_SPI_STATUS;

/// Last SPI transaction status
typedef enum 
{
	SPI_TF_COMPLETED = 0,			///< Last transaction sucessfull
	SPI_TF_FAILED,					///< Last transaction failed
	SPI_TF_CANCELLED				///< Last transaction cancelled
}
E_SPI_TF_STATUS;


// ===========================================================================
// U A R T 
// ===========================================================================
/// UART Tx device status
typedef enum 
{
	UART_TX_IDLE = 0,			///< Prêt pour une nouvelle transaction
	UART_TX_PENDING				///< Transaction en cours
}
E_UART_TX_STATUS;


// ===========================================================================
// I 2 C
// ===========================================================================
/// I2C master device status
typedef enum 
{
	I2C_MST_IDLE = 0,			///< Prêt pour une nouvelle transaction
	I2C_MST_START,				///< Acquisition du bus
	I2C_MST_WRITING,			///< Transaction en cours (écriture)
	I2C_MST_READING				///< Transaction en cours (lecture)
}
E_I2C_MST_STATUS;

/// I2C slave device status
typedef enum 
{
	I2C_SLV_STOP = 0,			///< L'esclave n'est pas activé
	I2C_SLV_LISTENING,			///< En attente d'un message
	I2C_SLV_READING,			///< Transaction en cours (lecture)
	I2C_SLV_ON_HOLD,			///< En attente de la réponse à renvoyer
	I2C_SLV_WRITING				///< Transaction en cours (écriture)
}
E_I2C_SLV_STATUS;

/// Last master I2C device transfer status
typedef enum 
{
	I2C_TF_COMPLETED = 0,			///< La dernière transaction a réussie
	I2C_TF_COLLISION,				///< Echec pour cause de collision
	I2C_TF_NO_RESP,					///< Pas de réponse de l'esclave
	I2C_TF_NACK						///< L'esclave a refusé la commande
}
E_I2C_MST_TF_STATUS;
	

	
// ===========================================================================
// M A C
// ===========================================================================
typedef void (*pMAC_Notify) (HANDLE device, unsigned int flags);

/// Protocole using the MAC layer
typedef enum 
{
	MAC_IP = 0x0800,			///< Protocole IP
	MAC_ARP = 0x0806,			///< Protocole ARP
	MAC_HOMEPLUGAV = 0x88E1,	///< Home Plug AV
	MAC_UNKNOWN = 0xFF			///< Protocole inconnu
}
E_MAC_TYPE;

/// MAC address
typedef struct __attribute__((__packed__))
{
    Byte v[6];
} MAC_ADDR;
	
/// Link status
typedef enum
{
	MAC_LINK_DOWN = 0,		///< Link is down
	MAC_LINK_UP,			///< Link is up
	MAC_LINK_UP_DOWN		///< Link is up, but was previously down
}
MAC_LINK;


// ===========================================================================
// P h y 
// ===========================================================================
/// PHY ID
typedef struct
{
	Dword OUI;			///< Organizationally Unique Identifier
	Byte VMN;			///< Vendor Model Number
	Byte Rev;			///< Major revision	
}
PHY_ID;

/// Speed and duplex mode
typedef enum
{
	HALF_DPX_10 = 1,
	FULL_DPX_10 = 2,
	HALF_DPX_100 = 4,
	FULL_DPX_100 = 8,
}
PHY_DPX_Mode;
	

	
// ###########################################################################
//
// P R O T O T Y P E S
//
// ###########################################################################

// G E N E R A L //

NS_API void DeviceIrqISR (HANDLE device);

NS_API void DeviceGetLock (HANDLE device);

NS_API Bool DeviceGetLockKER (HANDLE device);

NS_API void DeviceReleaseLock (HANDLE device);

NS_API void DeviceReleaseLockKER (HANDLE device);



// S P I //

NS_API void SPI_SetMode (HANDLE device, int mode);

NS_API void SPI_SetDataWidth (HANDLE device, int dataWidth);

NS_API Dword SPI_SetBaudRate (HANDLE device, Dword baudRate);

NS_API void SPI_SetMasterMode (HANDLE device, int master);

NS_API E_SPI_STATUS SPI_GetStatus (HANDLE device);

NS_API E_SPI_TF_STATUS SPI_GetTransferStatus (HANDLE device);

NS_API void SPI_Write (HANDLE device, const unsigned int* pData, int length);

NS_API void SPI_Read (HANDLE device, unsigned int* pBuffer, int length);

NS_API void SPI_Cancel (HANDLE device);

NS_API unsigned int SPI_Wait (HANDLE device, Long timeout);


// U A R T //

NS_API Dword UART_SetBaudRate (HANDLE device, Dword baudRate);

NS_API void UART_Flush (HANDLE device);

NS_API E_UART_TX_STATUS UART_GetTxStatus (HANDLE device);

NS_API int UART_GetRxErrors (HANDLE device);

NS_API int UART_Write (HANDLE device, const Byte* pData, int length, Long timeout);

NS_API int UART_Read (HANDLE device, Byte* pBuffer, int length, Long timeout);


// I 2 C //

NS_API Dword I2C_SetBaudRate (HANDLE device, Dword baudRate);

NS_API Bool I2C_BusIsIdle (HANDLE device);

NS_API void I2C_MasterTransfer (HANDLE device, Word address, const Byte* pCmd, int cmdLength, Byte* pResp, int respLength);

NS_API E_I2C_MST_TF_STATUS I2C_GetTransferStatus (HANDLE device);

NS_API E_I2C_MST_STATUS I2C_GetMasterStatus (HANDLE device);

NS_API void I2C_SlaveListen (HANDLE device, Word address, Word mask, Byte* pBuffer, int buffLength);

NS_API void I2C_SlaveReply (HANDLE device, Byte* pResp, int respLength);

NS_API void I2C_SlaveStopListen (HANDLE device);

NS_API E_I2C_SLV_STATUS I2C_GetSlaveStatus (HANDLE device);

NS_API unsigned int I2C_Wait (HANDLE device, unsigned int flags, Long timeout);


// M A C //

NS_API Bool MAC_StartUsage (HANDLE device);

NS_API void MAC_EndUsage (HANDLE device);

NS_API MAC_ADDR MAC_GetMac (HANDLE device);

NS_API MAC_LINK MAC_GetLinkStatus (HANDLE device);

NS_API Word MAC_GetTxFree (HANDLE device);

NS_API Bool MAC_NewTxPkt (HANDLE device, MAC_ADDR* remote, E_MAC_TYPE type, Word dataLen);

NS_API void MAC_WriteTxPkt (HANDLE device, Byte* data, Word offset, Word len);

NS_API void MAC_FlushTxPkt (HANDLE device);

NS_API Word MAC_GetRxPkt (HANDLE device, MAC_ADDR *remote, E_MAC_TYPE* type);

NS_API void MAC_ReadRxPkt (HANDLE device, Byte* buffer, Word offset, Word len);

NS_API void MAC_DiscardRxPkt (HANDLE device);

NS_API void MAC_SetNotifyCallback (HANDLE device, pMAC_Notify f);


// P H Y //

NS_API PHY_ID PHY_GetId (HANDLE device);

NS_API void PHY_SetLoopback (HANDLE device, Bool state);

NS_API Byte PHY_GetPhyAddr (HANDLE device);


#endif

// End
