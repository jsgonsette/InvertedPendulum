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
/// \file	ISZ2510.c
/// \author	Jean-Sebastien Gonsette
/// \date	17/10/2014
///
/// ISZ2510 device driver (Digital Gyroscope)
// ###########################################################################

#include "ISZ2510.h"


// ===========================================================================
// I S Z   R E G I S T E R S
// ===========================================================================

#define REG_XG_OFFS_TC_H				0x04
#define REG_XG_OFFS_TC_L				0x05
#define REG_YG_OFFS_TC_H				0x07
#define REG_YG_OFFS_TC_L				0x08
#define REG_ZG_OFFS_TC_H				0x0A
#define REG_ZG_OFFS_TC_L				0x0B
#define REG_XG_OFFS_USRH				0x13
#define REG_XG_OFFS_USRL				0x14
#define REG_YG_OFFS_USRH				0x15
#define REG_YG_OFFS_USRL				0x16
#define REG_ZG_OFFS_USRH				0x17
#define REG_ZG_OFFS_USRL				0x18
#define REG_SMPLRT_DIV					0x19
#define REG_CONFIG						0x1A
#define REG_GYRO_CONFIG					0x1B
#define REG_FIFO_EN						0x23
#define REG_INT_PIN_CFG					0x37
#define REG_INT_ENABLE					0x38
#define REG_INT_STATUS					0x3A
#define REG_TEMP_OUT_H					0x41
#define REG_TEMP_OUT_L					0x42
#define REG_GYRO_XOUT_H					0x43
#define REG_GYRO_XOUT_L					0x44
#define REG_GYRO_YOUT_H					0x45
#define REG_GYRO_YOUT_L					0x46
#define REG_GYRO_ZOUT_H					0x47
#define REG_GYRO_ZOUT_L					0x48
#define REG_USER_CTRL					0x6A
#define REG_PWR_MGMT_1					0x6B
#define REG_PWR_MGMT_2					0x6C
#define REG_FIFO_COUNTH					0x72
#define REG_FIFO_COUNTL					0x73
#define REG_FIFO_R_W					0x74
#define REG_WHO_AM_I					0x75


// ===========================================================================
// I S Z  I R Q
// ===========================================================================

// INT_ENABLE/STATUS register
#define IRQ_FIFO_OVFL					0x10
#define IRQ_FSYNC_INT					0x04
#define IRQ_DATA_RDY					0x01


// ===========================================================================
// D E F I N E
// ===========================================================================

// INT_PIN_CFG register
#define INT_LEVEL						0x80
#define INT_OPEN						0x40
#define LATCH_INT_EN					0x20
#define INT_RD_CLEAR					0x10
#define FSYNC_INT_LEVEL					0x08
#define FSYNC_INT_MODE_EN				0x04


// ===========================================================================
// T Y P E S
// ===========================================================================


// ===========================================================================
// P R I V A T E 
// ===========================================================================

/// Driver ISZ2510
static HANDLE gs_driverISZ2510 = INVALID_HANDLE;


// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Internal functions
//@{
static void KernelCallbackKER (HANDLE device);

static void GetSpi (ISZ2510_DCB* pDevice);
static void ReleaseSpi (ISZ2510_DCB* pDevice);

static void Write (ISZ2510_DCB* pDevice, Byte reg, Byte data);
static Byte Read (ISZ2510_DCB* pDevice, Byte reg);
static void ReadVector (ISZ2510_DCB* pDevice, Byte reg, Byte* pBuffer, Byte len);
//@}



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <ISZ2510_OpenDriver>
///
/// \brief	Record "ISZ2510" driver to the system
///
/// \return		Driver handle
///				INVALID_HANDLE in case of failure
// ===========================================================================
HANDLE ISZ2510_OpenDriver ()
{
	// Register driver at OS level
	gs_driverISZ2510 = Drv_OpenDriver (0, "Ivensense_ISZ2510", "1.0.0", KernelCallbackKER);
	
	return gs_driverISZ2510;	
}


// ===========================================================================
// <ISZ2510_CreateDevice>
///
/// \brief		Create a ISZ2510 device
///
/// \param[in]	name	Device name
/// \param[in]	pConfig	Configuration structure
///
/// \return		Device HANDLE
///				INVALID_HANDLE in case of failure
// ===========================================================================
HANDLE ISZ2510_CreateDevice (const char* name, ISZ2510_Config* pConfig)
{
	HANDLE h;
	Word size;
	ISZ2510_DCB* pDevice;

	if (gs_driverISZ2510 == INVALID_HANDLE) return INVALID_HANDLE;
	
	// Device registration
	size = AlignUp (sizeof (ISZ2510_DCB), sizeof (int));
	h = Drv_OpenDevice (gs_driverISZ2510, size, name, False);
		
	// Get back our data structure
	if (h != INVALID_HANDLE)
		pDevice = (ISZ2510_DCB*) Drv_HandleToDevice (h);
	else 
	{
		ThrowError (ERR_DEVICE_NOMEM);
		return INVALID_HANDLE;
	}

	// Configuration
	pDevice->spi = pConfig->spi;
	pDevice->fCS = pConfig->fCS;
	
	// Default scale = 250°/s
	pDevice->scale = ISZ2510_SCALE_250;
	
	// Default scaler = x1
	pDevice->scaler = 0x8000;
	
	return h;	
}


// ===========================================================================
// <ISZ2510_CloseDevice>
///
/// \brief	Close a ISZ2510 device
///
/// \param[in]		device		Device to close
// ===========================================================================
void ISZ2510_CloseDevice (HANDLE device)
{
	ISZ2510_DCB* pDevice;
	pDevice = (ISZ2510_DCB*) Drv_HandleToDevice (device);
	
	EnterCritical ();
	{
		Drv_CloseDevice (device);
	}
	LeaveCritical ();
}


// ===========================================================================
// <ISZ2510_Init>
///
/// \brief	Initialize the device
///
/// \param[in]		device		Device to initialize
///
/// \return		0 if initialization is successful
// ===========================================================================
int ISZ2510_Init (HANDLE device)
{
	Byte id;
	ISZ2510_DCB* pDevice;
	pDevice = (ISZ2510_DCB*) Drv_HandleToDevice (device);

 	GetSpi (pDevice);
	{
		// Get Who Am I ID
		id = Read (pDevice, REG_WHO_AM_I);	

		// Disable interrupts
		Write (pDevice, REG_INT_ENABLE, 0);
		
		// Configure interrupt usage:
		// Interrupt active low, push pull, held until cleared, cleared by reading status
		Write (pDevice, REG_INT_PIN_CFG, INT_LEVEL | LATCH_INT_EN);
		
		// Clear interrupt status
		Read (pDevice, REG_INT_STATUS);	
		
		//
		//Write (pDevice, REG_INT_ENABLE, IRQ_DATA_RDY);
	}
	ReleaseSpi (pDevice);
	
	// Check ID
	if (id == 0x68) return 0;
	return -1;
}


// ===========================================================================
// <ISZ2510_Reset>
///
/// \brief	Reset the device
///
/// \param[in]		device		Device to reset
// ===========================================================================
void ISZ2510_Reset (HANDLE device)
{
	Byte pwmg1;	
	ISZ2510_DCB* pDevice;
	pDevice = (ISZ2510_DCB*) Drv_HandleToDevice (device);

 	GetSpi (pDevice);
	{
		// Set the Reset bit
		pwmg1 = Read (pDevice, REG_PWR_MGMT_1);
		pwmg1 |= 0x80;		
		Write (pDevice, REG_PWR_MGMT_1, pwmg1);
	}
	ReleaseSpi (pDevice);
}


// ===========================================================================
// <ISZ2510_Selftest>
///
/// \brief	Perform gyro selftest measurement and return the status
///
/// \param[in]		device		Device to selftest
///
/// \return			True if the selftest is successful
// ===========================================================================
Bool ISZ2510_Selftest (HANDLE device)
{
	/// \todo
	return False;
}


// ===========================================================================
// <ISZ2510_SetMode>
///
/// \brief	Select the device power mode
///
/// \param[in]		device		Device to initialize
/// \param[in]		mode		Power mode: sleep, idle, normal
// ===========================================================================
void ISZ2510_SetMode (HANDLE device, ISZ2510_Mode mode)
{
	Byte pwmg1, pwmg2;	
	ISZ2510_DCB* pDevice;
	pDevice = (ISZ2510_DCB*) Drv_HandleToDevice (device);

 	GetSpi (pDevice);
	{
		// Get power management configuration
		pwmg1 = Read (pDevice, REG_PWR_MGMT_1);
		pwmg2 = Read (pDevice, REG_PWR_MGMT_2);
		
		if (mode == ISZ2510_MODE_SLEEP) pwmg1 |= 0x40;
		else if (mode == ISZ2510_MODE_STANDBY)
		{
			// No sleep, select internal clock, Gyros in standby
			pwmg1 &= ~0x40;
			pwmg1 &= ~0x07;
			pwmg2 |= 0x07;
		}
		else if (mode == ISZ2510_MODE_MEASURE) 
		{
			// No sleep, select PLL clock, Gyro on
			pwmg2 = 0;
			pwmg1 &= ~0x40;
			pwmg1 |= 0x01;
		}

		// Write power management configuration.
		// If going in standby, change the clock first
		if (mode == ISZ2510_MODE_STANDBY)
		{
			Write (pDevice, REG_PWR_MGMT_1, pwmg1);
			Write (pDevice, REG_PWR_MGMT_2, pwmg2);
		}
		else
		{
			Write (pDevice, REG_PWR_MGMT_2, pwmg2);
			Write (pDevice, REG_PWR_MGMT_1, pwmg1);
		}
	}
	ReleaseSpi (pDevice);
}


// ===========================================================================
// <ISZ2510_SetScale>
///
/// \brief	Select the gyro scale/sensitivity
///
/// \param[in]		device			Device to configure
/// \param[in]		scale			Full scale: 250, 500, 1000 or 2000 °/s
// ===========================================================================
void ISZ2510_SetScale (HANDLE device, ISZ2510_Scale scale)
{
	Byte gyroConfig;
	ISZ2510_DCB* pDevice;
	pDevice = (ISZ2510_DCB*) Drv_HandleToDevice (device);

 	GetSpi (pDevice);
	{
		// Get configuration
		gyroConfig = Read (pDevice, REG_GYRO_CONFIG);
		
		// Change scale
		gyroConfig &= ~0x18;
		gyroConfig |= (Byte) (scale << 3);
		pDevice->scale = scale;
	
		// Write configuration
		Write (pDevice, REG_GYRO_CONFIG, gyroConfig);
	}
	ReleaseSpi (pDevice);
}


// ===========================================================================
// <ISZ2510_GetTemperature>
///
/// \brief	Return the device internal temperature
///
/// \param[in]		device		Target device 
///
/// \return			Temperature in 1/100 °
// ===========================================================================
Short ISZ2510_GetTemperature (HANDLE device)
{
	Word raw;
	Long temp;
	ISZ2510_DCB* pDevice;
	pDevice = (ISZ2510_DCB*) Drv_HandleToDevice (device);

 	GetSpi (pDevice);
	{
		// Get raw temperature
		ReadVector (pDevice, REG_TEMP_OUT_H, (Byte*) &raw, 2);
		raw = BigEndian16 (raw);
	}
	ReleaseSpi (pDevice);
	
	// 321.4 LSB/°C
	temp = (Long)((Short)raw) * 20391;
	temp >>= 16;
	
	return (Short)temp;
}


// ===========================================================================
// <ISZ2510_SetBandwidth>
///
/// \brief	Select the internal lowpass filter bandwidth
///
/// \param[in]		device		Target device 
/// \param[in]		bw			Lpf bandwidth
// ===========================================================================
void ISZ2510_SetBandwidth (HANDLE device, ISZ2510_Bandwidth bw)
{
	Byte dlpf;
	Byte fchoice;
	ISZ2510_DCB* pDevice;
	pDevice = (ISZ2510_DCB*) Drv_HandleToDevice (device);

 	GetSpi (pDevice);
	{
		// Get configuration
		dlpf = Read (pDevice, REG_CONFIG);
		fchoice = Read (pDevice, REG_GYRO_CONFIG);
		
		// Select bandwidth
		if (bw <= ISZ2510_BW_5 && bw >= ISZ2510_BW_250)
		{
			dlpf &= ~0x07;
			dlpf |= (bw & 0x07);
			fchoice &= ~0x03;
		}		
		else if (bw == ISZ2510_BW_3600)
		{
			if ((fchoice & 0x03) == 0) dlpf = ISZ2510_BW_3600;
			else 
			{
				fchoice &= ~0x03;
				fchoice |= 0x02;
			}
		}		
		else if (bw == ISZ2510_BW_8800)
		{
			fchoice &= ~0x03;
			fchoice |= 0x01;
		}
		
		// Set configuration
		Write (pDevice, REG_CONFIG, dlpf);
		Write (pDevice, REG_GYRO_CONFIG, fchoice);
	}
	ReleaseSpi (pDevice);
}


// ===========================================================================
// <ISZ2510_SetBaudrate>
///
/// \brief	Select the output data rate
///
/// Achievable baudrate is closely related to the selected bandwdith (see datasheet).
/// Therefore, bandwidth must be selected first.
/// If BW = 3600, BR is either 8k or 32k
/// If BW = 8800, BR is only 32k
/// If BW = 250, BR is only 8k
/// For other BW, BR must be less or equal to 1k
///
/// \param[in]		device		Target device 
/// \param[in]		br			Desired baudrate in Hz
///
/// \return			Actual baudrate programmed in the device
// ===========================================================================
Word ISZ2510_SetBaudrate (HANDLE device, Word br)
{
	Byte config, gyroConfig;
	Byte dlpf, fchoice;
	Word temp;
	ISZ2510_DCB* pDevice;
	pDevice = (ISZ2510_DCB*) Drv_HandleToDevice (device);

 	GetSpi (pDevice);
	{
		// Get configuration
		config = Read (pDevice, REG_CONFIG);
		gyroConfig = Read (pDevice, REG_GYRO_CONFIG);
		
		// Extract dlpf and fchoice fields
		dlpf = config & 0x07;
		fchoice = gyroConfig & 0x03;
		
		// Erase dlpf and fchoice
		config &= ~0x07;
		gyroConfig &= ~0x03;
		
		// Configurable output data rate
		if (fchoice == 0 && dlpf != ISZ2510_BW_250 && dlpf != ISZ2510_BW_3600)
		{
			if (br > 1000) br = 1000;
			
			// Compute and program divider
			temp = 1000/br;
			temp --;
			if (temp > 0xFF) temp = 0xFF;
			Write (pDevice, REG_SMPLRT_DIV, (Byte) temp);
			
			// Compute actual br
			br = 1000/(temp+1);			
		}
		
		// Not configurable output data rate
		else
		{
			if (fchoice == 0 && dlpf == ISZ2510_BW_250) br = 8000;
			else if (fchoice == 1 || fchoice == 3) br = 32000;
			else
			{
				if (br == 32000) fchoice = 2;
				else
				{
					br = 8000;
					fchoice = 0;
					dlpf = ISZ2510_BW_3600;
				}
			}
		}
		
		// Set configuration
		config |= dlpf;
		gyroConfig |= fchoice;
		Write (pDevice, REG_CONFIG, config);
		Write (pDevice, REG_GYRO_CONFIG, gyroConfig);
	}
	ReleaseSpi (pDevice);
		
	return br;
}


// ===========================================================================
// <ISZ2510_GetAngularSpeed>
///
/// \brief	Return the gryo output value
///
/// \param[in]		device		Target device
///
/// \return			Gyro output [1/1000 °/s]
// ===========================================================================
Long ISZ2510_GetAngularSpeed (HANDLE device)
{
	Short data;
	Long temp;
	
	ISZ2510_DCB* pDevice;
	pDevice = (ISZ2510_DCB*) Drv_HandleToDevice (device);

 	GetSpi (pDevice);
	{
		// Read the gyro Z output
		ReadVector (pDevice, REG_GYRO_ZOUT_H, (Byte*) &data, 2);
		data = BigEndian16 (data);
	}
	ReleaseSpi (pDevice);

	// Extend value on 4 Bytes
	temp = (Long) ((Short) data);
	
	// Scaling (correction)
	temp = (temp * pDevice->scaler) >> 15;

	// Scaling (nominal coeff. depends on scale)
	switch (pDevice->scale)
	{
		case (ISZ2510_SCALE_250): temp = (temp * 10000) / 1310; break;
		case (ISZ2510_SCALE_500): temp = (temp * 10000) / 655; break;
		case (ISZ2510_SCALE_1000): temp = (temp * 10000) / 328; break;
		case (ISZ2510_SCALE_2000): temp = (temp * 10000) / 164; break;
		default: temp = 0; break;
	}

	return temp;
}


// ===========================================================================
// <ISZ2510_SetCalibrationOffset>
///
/// \brief	Specify the gyro bias measured by an external calibration process
///
/// \param[in]		device				Device to calibrate
/// \param[in]		offset				Measured bias [1/1000 °/s]
// ===========================================================================
void ISZ2510_SetCalibrationOffset (HANDLE device, Long offset)
{
	Short offC;
	ISZ2510_DCB* pDevice;
	pDevice = (ISZ2510_DCB*) Drv_HandleToDevice (device);
	
	GetSpi (pDevice);
	{
		// Scale bias
		offC = (Short) ((-offset * 328) / 10000);
		Write (pDevice, REG_ZG_OFFS_USRH, (Byte) (offC >> 8));
		Write (pDevice, REG_ZG_OFFS_USRL, (Byte) (offC & 0xFF));
	}
	ReleaseSpi (pDevice);
}


// ===========================================================================
// <ISZ2510_GetCalibrationValues>
///
/// \brief	Get calibration values for backup purpose
///
/// \param[in]		device				Target device
/// \param[out]		calib				Calibration values
// ===========================================================================
void ISZ2510_GetCalibrationValues (HANDLE device, ISZ2510_CalibValues* calib)
{
	Word offset;
	ISZ2510_DCB* pDevice;
	pDevice = (ISZ2510_DCB*) Drv_HandleToDevice (device);

	// Get offset correction stored in the device
	GetSpi (pDevice);
	{
		ReadVector (pDevice, REG_ZG_OFFS_USRH, (Byte*) &offset, 2);
		offset = BigEndian16 (offset);
	}
	ReleaseSpi (pDevice);

	// Fill the calibration structure
	calib->scaler = pDevice->scaler;
	calib->offset = offset;
}


// ===========================================================================
// <ISZ2510_SetCalibrationValues>
///
/// \brief	Set/restore calibration values.
///
/// \param[in]		device				Target device
/// \param[in]		calib				Calibration values
// ===========================================================================
void ISZ2510_SetCalibrationValues (HANDLE device, ISZ2510_CalibValues calib)
{
	ISZ2510_DCB* pDevice;
	pDevice = (ISZ2510_DCB*) Drv_HandleToDevice (device);

	// Get scalers
	pDevice->scaler = calib.scaler;

	// Write offset correction into the device
	GetSpi (pDevice);
	{
		Write (pDevice, REG_ZG_OFFS_USRH, (Byte) (calib.offset >> 8));
		Write (pDevice, REG_ZG_OFFS_USRL, (Byte) (calib.offset & 0xFF));
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
/// \brief	SPI bus reservation
///
/// \param[in]		pDevice		Target device
// ===========================================================================
static void GetSpi (ISZ2510_DCB* pDevice)
{
	// Lock the SPI for our usage
//	DeviceGetLock (pDevice->spi);
	
	// Configuration
	// Mode 0, 8 bits, 1MHz
	SPI_SetMode (pDevice->spi, 0);
	SPI_SetDataWidth (pDevice->spi, 8);
	SPI_SetBaudRate (pDevice->spi, 1000000);
}


// ===========================================================================
// <ReleaseSpi>
///
/// \brief	SPI bus release
///
/// \param[in]		pDevice		Target device
// ===========================================================================
static void ReleaseSpi (ISZ2510_DCB* pDevice)
{
	// Release lock on the SPI device
//	DeviceReleaseLock (pDevice->spi);
}
	
	
// ===========================================================================
// <Write>
///
/// \brief	Elementary writing on the ISZ2510
///
/// \param[in]	pDevice			Target device
/// \param[in]	reg				Register address
/// \param[in]	data			Data to write
// ===========================================================================
static void Write (ISZ2510_DCB* pDevice, Byte reg, Byte data)
{
	// MSB bit clear for writing
	reg &= ~0x80;
	
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
/// \brief	Elementary reading on the ISZ2510
///
/// \param[in]	pDevice			Target device
/// \param[in]	reg				Register address
///
/// \return		Value
// ===========================================================================
static Byte Read (ISZ2510_DCB* pDevice, Byte reg)
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
/// \brief	Multiple reading on the ISZ2510
///
/// \param[in]	pDevice			Target device
/// \param[in]	reg				Register address
/// \param[out]	pBuffer			Values
/// \param[in]	len				Number of registers to read
// ===========================================================================
static void ReadVector (ISZ2510_DCB* pDevice, Byte reg, Byte* pBuffer, Byte len)
{
	// MSB bit set for reading
	reg |= 0x80;

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
