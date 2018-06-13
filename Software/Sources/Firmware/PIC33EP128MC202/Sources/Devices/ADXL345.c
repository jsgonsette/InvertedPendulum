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
/// \file	ADXL345.c
/// \author	Jean-Sebastien Gonsette
/// \date	11/10/2014
///
/// ADXL345 device driver (Digital Accelerometer)
// ###########################################################################

#include "ADXL345.h"


// ===========================================================================
// D E F I N E
// ===========================================================================


// ===========================================================================
// A D X L   R E G I S T E R S
// ===========================================================================

#define REG_ADXL345_DEVID					0x00
#define REG_ADXL345_THRESH_TAP				0x1D
#define REG_ADXL345_OFSX					0x1E
#define REG_ADXL345_OFSY					0x1F
#define REG_ADXL345_OFSZ					0x20
#define REG_ADXL345_DUR						0x21
#define REG_ADXL345_Latent					0x22
#define REG_ADXL345_Window					0x23
#define REG_ADXL345_THRESH_ACT				0x24
#define REG_ADXL345_THRESH_INACT			0x25
#define REG_ADXL345_TIME_INACT				0x26
#define REG_ADXL345_ACT_INACT_CTL			0x27
#define REG_ADXL345_THRESH_FF				0x28
#define REG_ADXL345_TIME_FF					0x29
#define REG_ADXL345_TAP_AXES				0x2A
#define REG_ADXL345_ACT_TAP_STATUS			0x2B
#define REG_ADXL345_BW_RATE					0x2C
#define REG_ADXL345_POWER_CTL				0x2D
#define REG_ADXL345_INT_ENABLE				0x2E
#define REG_ADXL345_INT_MAP					0x2F
#define REG_ADXL345_INT_SOURCE				0x30
#define REG_ADXL345_DATA_FORMAT				0x31
#define REG_ADXL345_DATAX0					0x32
#define REG_ADXL345_DATAX1					0x33
#define REG_ADXL345_DATAY0					0x34
#define REG_ADXL345_DATAY1					0x35
#define REG_ADXL345_DATAZ0					0x36
#define REG_ADXL345_DATAZ1					0x37
#define REG_ADXL345_FIFO_CTL				0x38
#define REG_ADXL345_FIFO_STATUS				0x39


// ===========================================================================
// A D X L   I R Q
// ===========================================================================

#define IRQ_ADXL345_OVERRUN					0x01
#define IRQ_ADXL345_WATERMARK				0x02
#define IRQ_ADXL345_FREE_FALL				0x04
#define IRQ_ADXL345_INACTIVITY				0x08
#define IRQ_ADXL345_ACTIVITY				0x10
#define IRQ_ADXL345_DOUBLE_TAP				0x20
#define IRQ_ADXL345_SINGLE_TAP				0x40
#define IRQ_ADXL345_DATA_READY				0x80


// ===========================================================================
// T Y P E S
// ===========================================================================


// ===========================================================================
// P R I V A T E 
// ===========================================================================

/// Driver ADXL345
static HANDLE gs_driverADXL345 = INVALID_HANDLE;


// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Internal functions
//@{
static void KernelCallbackKER (HANDLE device);

static void GetSpi (ADXL345_DCB* pDevice);
static void ReleaseSpi (ADXL345_DCB* pDevice);

static void Write (ADXL345_DCB* pDevice, Byte reg, Byte data);
static Byte Read (ADXL345_DCB* pDevice, Byte reg);
static void ReadVector (ADXL345_DCB* pDevice, Byte reg, Byte* pBuffer, Byte len);
//@}



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <ADXL345_OpenDriver>
///
/// \brief	Record "ADXL345" driver to the system
///
/// \return		Driver handle
///				INVALID_HANDLE in case of failure
// ===========================================================================
HANDLE ADXL345_OpenDriver ()
{
	// Register this driver to the OS
	gs_driverADXL345 = Drv_OpenDriver (0, "AnalogDevice_ADXL345", "1.0.0", KernelCallbackKER);
	
	return gs_driverADXL345;	
}


// ===========================================================================
// <ADXL345_CreateDevice>
///
/// \brief		Create a ADXL345 device
///
/// \param[in]	name	Device name
/// \param[in]	pConfig	Configuration structure
///
/// \return		Device HANDLE
///				INVALID_HANDLE in case of failure
// ===========================================================================
HANDLE ADXL345_CreateDevice (const char* name, ADXL345_Config* pConfig)
{
	HANDLE h;
	Word size;
	ADXL345_DCB* pDevice;

	if (gs_driverADXL345 == INVALID_HANDLE) return INVALID_HANDLE;
	
	// Device registration
	size = AlignUp (sizeof (ADXL345_DCB), sizeof (int));
	h = Drv_OpenDevice (gs_driverADXL345, size, name, False);
		
	// Get back our data structure
	if (h != INVALID_HANDLE)
		pDevice = (ADXL345_DCB*) Drv_HandleToDevice (h);
	else 
	{
		ThrowError (ERR_DEVICE_NOMEM);
		return INVALID_HANDLE;
	}

	// Configuration
	pDevice->spi = pConfig->spi;
	pDevice->fCS = pConfig->fCS;
	
	// Default scalers = x1
	pDevice->scalers [0] = 0x8000;
	pDevice->scalers [1] = 0x8000;
	pDevice->scalers [2] = 0x8000;

	return h;	
}


// ===========================================================================
// <ADXL345_CloseDevice>
///
/// \brief	Close a ADXL345 device
///
/// \param[in]		device		Device to close
// ===========================================================================
void ADXL345_CloseDevice (HANDLE device)
{
	ADXL345_DCB* pDevice;
	pDevice = (ADXL345_DCB*) Drv_HandleToDevice (device);
	
	EnterCritical ();
	{
		Drv_CloseDevice (device);
	}
	LeaveCritical ();
}


// ===========================================================================
// <ADXL345_Init>
///
/// \brief	Initialize the device
///
/// \param[in]		device		Device to initialize
///
/// \return		0 if initialization is successful
// ===========================================================================
int ADXL345_Init (HANDLE device)
{
	Byte id, b;
	ADXL345_DCB* pDevice;
	pDevice = (ADXL345_DCB*) Drv_HandleToDevice (device);

 	GetSpi (pDevice);
	{
		// Get ID
		id = Read (pDevice, REG_ADXL345_DEVID);
		
		// Put the device in 4wire spi, full resolution, 16g range, Little Endian
		b = 0x0B;
		Write (pDevice, REG_ADXL345_DATA_FORMAT, b);
		
		// Disable all interrupt mask
		b = 0x00;
		Write (pDevice, REG_ADXL345_INT_ENABLE, b);
	}
	ReleaseSpi (pDevice);
	
	// Check device ID
	if (id == 0xE5) return 0;
	return -1;
}


// ===========================================================================
// <ADXL345_SetLowPowerMode>
///
/// \brief	Set device in reduced power mode (but with higher noise)
///
/// \param[in]		device		target device
/// \param[in]		state		>0: Low power mode
// ===========================================================================
void ADXL345_SetLowPowerMode (HANDLE device, int state)
{
	Byte data;
	ADXL345_DCB* pDevice;
	pDevice = (ADXL345_DCB*) Drv_HandleToDevice (device);

 	GetSpi (pDevice);
	{
		// Change the power mode bits in the BW register
		data = Read (pDevice, REG_ADXL345_BW_RATE);
		data &= ~0x10;
		if (state > 0) data |= 0x10;
		Write (pDevice, REG_ADXL345_BW_RATE, data);
	}
	ReleaseSpi (pDevice);
}


// ===========================================================================
// <ADXL345_SetMode>
///
/// \brief	Select device mode for data acquisition
///
/// \param[in]		device		target device
/// \param[in]		mode		Idle/Sleep/Acquisition
// ===========================================================================
void ADXL345_SetMode (HANDLE device, ADXL345_Mode mode)
{
	Byte b;
	ADXL345_DCB* pDevice;
	pDevice = (ADXL345_DCB*) Drv_HandleToDevice (device);

 	GetSpi (pDevice);
	{
		b = Read (pDevice, REG_ADXL345_POWER_CTL);
		b &= ~0x0C;
		
		// When going from sleep to measurement mode, make a transition in idle
		if (mode == ADXL345_MODE_MEASURE)
			Write (pDevice, REG_ADXL345_POWER_CTL, b);
		
		switch (mode)
		{
			case ADXL345_MODE_SLEEP: b |= 0x04; break;
			case ADXL345_MODE_MEASURE: b |= 0x08; break;
			case ADXL345_MODE_IDLE: break;
		}		

		Write (pDevice, REG_ADXL345_POWER_CTL, b);
	}
	ReleaseSpi (pDevice);
}


// ===========================================================================
// <ADXL345_SetBaudrate>
///
/// \brief	Define acquisition baudrate
///
/// \param[in]		device		target device
/// \param[in]		baudrate	Selected baudrate
// ===========================================================================
void ADXL345_SetBaudrate (HANDLE device, ADXL345_Baudrate baudrate)
{
	Byte data;
	ADXL345_DCB* pDevice;
	pDevice = (ADXL345_DCB*) Drv_HandleToDevice (device);

 	GetSpi (pDevice);
	{
		// Change the Baudrate bits in the BW register
		data = Read (pDevice, REG_ADXL345_BW_RATE);
		data &= ~0x0F;
		data |= (Byte) baudrate & 0x0F;
		Write (pDevice, REG_ADXL345_BW_RATE, data);
	}
	ReleaseSpi (pDevice);
}


// ===========================================================================
// <ADXL345_GetAccelerations>
///
/// \brief	Read last accelerometer samples, on the 3 axis
///
/// \param[in]		device		target device
/// \param[out]		accX		Acceleration along axis X [mg]
/// \param[out]		accY		Acceleration along axis Y [mg]
/// \param[out]		accZ		Acceleration along axis Z [mg]
///
/// \return			Acquisition baudrate
// ===========================================================================
void ADXL345_GetAccelerations (HANDLE device, Short* accX, Short* accY, Short* accZ)
{
	int i;
	Short data [3];
	Long temp;
	
	ADXL345_DCB* pDevice;
	pDevice = (ADXL345_DCB*) Drv_HandleToDevice (device);

 	GetSpi (pDevice);
	{
		// Read the 3 axis
		ReadVector (pDevice, REG_ADXL345_DATAX0, (Byte*) data, 6);
	}
	ReleaseSpi (pDevice);

	for (i = 0; i < 3; i ++)
	{
		// Order Bytes if needed and extend value on 4 Bytes
		temp = (Long) ((Short) LittleEndian16 (data [i]));
	
		// Scaling (nominal value: 256 LSB/g)
		// Scaling (correction)
		temp = (temp * 1000) >> 8;
		temp = (temp * pDevice->scalers [i]) >> 15;
		
		data [i] = (Short) temp;
	}
	
	if (accX != null) *accX = data [0];
	if (accY != null) *accY = data [1];
	if (accZ != null) *accZ = data [2];
}


// ===========================================================================
// <ADXL345_SetCalibrationOffset>
///
/// \brief	Specify the accelerometer bias measured by an external calibration process
///
/// \param[in]		device				Device to calibrate
/// \param[in]		axis				Axis to calibrate (0: X, 1: Y, 2: Z)
/// \param[in]		offset				Measured bias [mg]
// ===========================================================================
void ADXL345_SetCalibrationOffset (HANDLE device, int axis, Short offset)
{
	Long temp;
	ADXL345_DCB* pDevice;
	pDevice = (ADXL345_DCB*) Drv_HandleToDevice (device);

	// Conversion 256LSB/g
	temp = -offset;
	temp <<= 8;
	temp /= 1000;
	
	// Offset factor: division by 4 with rounding
	temp += 2;
	temp >>= 2;

	// Truncation
	offset = (Short) temp;
	if (offset > (Short) 0x007F) offset = 0x7F;
	if (offset < (Short) 0xFF80) offset = 0xFF80;

	GetSpi (pDevice);
	{
		Write (pDevice, REG_ADXL345_OFSX + axis, (Byte) offset);
	}
	ReleaseSpi (pDevice);
}



// ===========================================================================
// <ADXL345_SetCalibrationScaler>
///
/// \brief	Specify the accelerometer scaler measured by an external calibration process
///
/// \param[in]		device				Device to calibrate
/// \param[in]		axis				Axis to calibrate (0: X, 1: Y, 2: Z)
/// \param[in]		scaler				Scale factor [-0.5; 2]
// ===========================================================================
void ADXL345_SetCalibrationScaler (HANDLE device, int axis, float scaler)
{
	Word temp;
	ADXL345_DCB* pDevice;
	pDevice = (ADXL345_DCB*) Drv_HandleToDevice (device);
	
	// Code the scaler on 15bits signed value
	if (scaler > 0.5f && scaler < 2.0f)
	{
		temp = (Word) (32768.0f/scaler);
		pDevice->scalers [axis] = temp;
	}
}


// ===========================================================================
// <ADXL345_GetCalibrationValues>
///
/// \brief	Get calibration values for backup purpose
///
/// \param[in]		device				Device to calibrate
/// \param[out]		calib				Calibration values
// ===========================================================================
void ADXL345_GetCalibrationValues (HANDLE device, ADXL345_CalibValues* calib)
{
	Byte offsets [3];
	ADXL345_DCB* pDevice;
	pDevice = (ADXL345_DCB*) Drv_HandleToDevice (device);

	// Get offset correction stored in the device
	GetSpi (pDevice);
	{
		ReadVector (pDevice, REG_ADXL345_OFSX, offsets, 3);
	}
	ReleaseSpi (pDevice);

	// Fill the calibration structure
	calib->scalers [0] = pDevice->scalers [0];
	calib->scalers [1] = pDevice->scalers [1];
	calib->scalers [2] = pDevice->scalers [2];

	calib->offsets [0] = offsets [0];
	calib->offsets [1] = offsets [1];
	calib->offsets [2] = offsets [2];
}


// ===========================================================================
// <ADXL345_SetCalibrationValues>
///
/// \brief	Set/restore calibration values.
///
/// \param[in]		device				Device to calibrate
/// \param[in]		calib				Calibration values
// ===========================================================================
void ADXL345_SetCalibrationValues (HANDLE device, ADXL345_CalibValues calib)
{
	ADXL345_DCB* pDevice;
	pDevice = (ADXL345_DCB*) Drv_HandleToDevice (device);

	// Get scalers
	pDevice->scalers [0] = calib.scalers [0];
	pDevice->scalers [1] = calib.scalers [1];
	pDevice->scalers [2] = calib.scalers [2];

	// Write offset correction into the device
	GetSpi (pDevice);
	{
		Write (pDevice, REG_ADXL345_OFSX, (Byte) (calib.offsets [0]));
		Write (pDevice, REG_ADXL345_OFSY, (Byte) (calib.offsets [1]));
		Write (pDevice, REG_ADXL345_OFSZ, (Byte) (calib.offsets [2]));
	}
	ReleaseSpi (pDevice);
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################

// ===========================================================================
// <KernelCallbackKER>
///
/// \brief	Callback called by the Kernel when the device rise an IRQ
///
/// \param[in]		device		Target device
// ===========================================================================
static void KernelCallbackKER (HANDLE device)
{
	// Interruptions not used
	return;
}



// ###########################################################################
//
// P R I V A T E   -   S P I
//
// ###########################################################################

// ===========================================================================
// <GetSpi>
///
/// \brief	Reserve SPI bus
///
/// \param[in]		pDevice		Target device
// ===========================================================================
static void GetSpi (ADXL345_DCB* pDevice)
{
	// Lock the SPI for our usage
//	DeviceGetLock (pDevice->spi);
	
	// Configuration
	// Mode 3, 8 bits, 2MHz
	SPI_SetMode (pDevice->spi, 3);
	SPI_SetDataWidth (pDevice->spi, 8);
	SPI_SetBaudRate (pDevice->spi, 2000000);
}


// ===========================================================================
// <ReleaseSpi>
///
/// \brief	Free SPI bus
///
/// \param[in]		pDevice		Target device
// ===========================================================================
static void ReleaseSpi (ADXL345_DCB* pDevice)
{
	// Release lock on the SPI device
//	DeviceReleaseLock (pDevice->spi);
}
	
	
// ===========================================================================
// <Write>
///
/// \brief	Elementary writing on the ADXL345
///
/// \param[in]	pDevice			Target device
/// \param[in]	reg				Register address
/// \param[in]	data			Data to write
// ===========================================================================
static void Write (ADXL345_DCB* pDevice, Byte reg, Byte data)
{
	pDevice->fCS (1);
	{
		// Register address
		SPI_Write (pDevice->spi, (unsigned int*) &reg, 1);
		while (SPI_GetStatus (pDevice->spi) != SPI_ST_IDLE);
		
		// Set data
		SPI_Write (pDevice->spi, (unsigned int*) &data,  1);
		while (SPI_GetStatus (pDevice->spi) != SPI_ST_IDLE);
	}
	pDevice->fCS (0);
}


// ===========================================================================
// <Read>
///
/// \brief	Elementary reading on the ADXL345
///
/// \param[in]	pDevice			Target device
/// \param[in]	reg				Register address
///
/// \return		Value
// ===========================================================================
static Byte Read (ADXL345_DCB* pDevice, Byte reg)
{
	Byte data;

	// MSB bit set for reading
	reg |= 0x80;
	
	pDevice->fCS (1);
	{
		// Register address
		SPI_Write (pDevice->spi, (unsigned int*) &reg, 1);
		while (SPI_GetStatus (pDevice->spi) != SPI_ST_IDLE);
		
		// Get response
		SPI_Read (pDevice->spi, (unsigned int*) &data,  1);
		while (SPI_GetStatus (pDevice->spi) != SPI_ST_IDLE);
	}
	pDevice->fCS (0);

	return data;
}


// ===========================================================================
// <ReadVector>
///
/// \brief	Multiple reading on the ADXL345
///
/// \param[in]	pDevice			Target device
/// \param[in]	reg				Register address
/// \param[out]	pBuffer			Values
/// \param[in]	len				Number of registers to read
// ===========================================================================
static void ReadVector (ADXL345_DCB* pDevice, Byte reg, Byte* pBuffer, Byte len)
{
	// MSB set for reading
	// Bit 7 set for multiple reading
	reg |= 0xC0;

	pDevice->fCS (1);
	{
		// Register address
		SPI_Write (pDevice->spi, (unsigned int*) &reg, 1);
		while (SPI_GetStatus (pDevice->spi) != SPI_ST_IDLE);

		// Get response
		SPI_Read (pDevice->spi, (unsigned int*) pBuffer,  len);
		while (SPI_GetStatus (pDevice->spi) != SPI_ST_IDLE);
	}
	pDevice->fCS (0);
}

// END
