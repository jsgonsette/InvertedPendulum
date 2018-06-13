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
//
/// \file	Bsp.c
/// \author	Jean-Sebastien Gonsette
/// \date	17/09/2014
///
/// Board Support Package:
/// Initialize the processor and the OS.
/// Provide low level API to handle the iPendulum board functionalities.
// ###########################################################################

#include <xc.h>
#include <libpic30.h>

#include "NanoScale.h"
#include "NanoScaleDrivers_PIC16.h"
#include "BSP.h"
#include "Global.h"
#include "Devices/ADXL345.h"
#include "Devices/ISZ2510.h"



// ===========================================================================
// PIC24E CONFIGURATION BITS
// ===========================================================================

// FICD
#pragma config ICS = PGD2               // ICD Communication Channel Select bits (Communicate on PGEC2 and PGED2)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FPOR
#pragma config ALTI2C1 = OFF            // Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF            // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)
#pragma config WDTWIN = WIN25           // Watchdog Window Select bits (WDT Window is 25% of WDT period)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON              // PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (Watchdog timer disabled)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function bit (OSC2 is general purpose digital I/O pin)
#pragma config IOL1WAY = ON             // Peripheral pin select configuration (Allow only one reconfiguration)
#pragma config FCKSM = CSECMD           // Clock Switching Mode bits (Clock switching is enabled,Fail-safe Clock Monitor is disabled)

// FOSCSEL
#pragma config FNOSC = FRC              // Oscillator Source Selection (Internal Fast RC (FRC))
#pragma config PWMLOCK = OFF            // PWM Lock Enable bit (PWM registers may be written without key sequence)
#pragma config IESO = OFF               // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF                // General Segment Code-Protect bit (General Segment Code protect is Disabled)


// ===========================================================================
// D E F I N E
// ===========================================================================

/// Flash memory page size
#define PAGE_SIZE		1024

/// Processor frequency [Hz]
#define PROC_FCY			39812000

/// Frequency for the BLDC voltage ref PWM [Hz]
#define F_PWM_VREF			100000

/// Frequency for the BLDC Fwd/Reverse pin PWM [Hz]
#define F_PWM_FWD			35000

// IO Modes
#define INPUT		1		///< IO configuration to be an input
#define OUTPUT		0		///< IO configuration to be an output

// IO Configuration
#define IOM_VBAT			TRISAbits.TRISA0		///< Battery voltage
#define IOM_CS_ADX			TRISAbits.TRISA1        ///< Chip Select for ADXL345
#define IOM_CS_ISZ			TRISAbits.TRISA2        ///< Chip Select for ISZ2510
#define IOM_INT_ADX			TRISAbits.TRISA3        ///< Interrupt for ADXL345
#define IOM_INT_ISZ			TRISAbits.TRISA4        ///< Interrupt for ISZ2510

#define IOM_LED_RED			TRISBbits.TRISB0		///< Led Red
#define IOM_LED_YEL			TRISBbits.TRISB1		///< Led Yellow
#define IOM_LED_GRE			TRISBbits.TRISB4		///< Led Green

#define IOM_BLDC_FWD		TRISBbits.TRISB10		///< BLDC Forward/Backward
#define IOM_BLDC_BRAKE		TRISBbits.TRISB11		///< BLDC Brake
#define IOM_BLDC_EN			TRISBbits.TRISB12		///< BLDC Enable/disable
#define IOM_BLDC_TACHO		TRISBbits.TRISB13		///< BLDC Tachometer
#define IOM_BLDC_DIAG		TRISBbits.TRISB14		///< BLDC Diagnostic
#define IOM_BLDC_VREF		TRISBbits.TRISB15		///< BLDC Voltage reference

// Outputs
#define IO_CS_ADX			LATAbits.LATA1			///< Chip Select for ADXL345
#define IO_CS_ISZ			LATAbits.LATA2			///< Chip Select for ISZ2510

#define IO_LED_RED			LATBbits.LATB0			///< Led Red
#define IO_LED_YEL			LATBbits.LATB1			///< Led Yellow
#define IO_LED_GRE			LATBbits.LATB4			///< Led Green

#define IO_BLDC_FWD			LATBbits.LATB10			///< BLDC Forward/Backward
#define IO_BLDC_BRAKE		LATBbits.LATB11			///< BLDC Brake
#define IO_BLDC_EN			LATBbits.LATB12			///< BLDC Enable/disable

// Inputs
#define IO_INT_ADX			PORTAbits.RA3			///< Interrupt for ADXL345
#define IO_INT_ISZ			PORTAbits.RA4			///< Interrupt for ISZ2510
#define IO_BLDC_TACHO		PORTBbits.RB13			///< BLDC Tachometer
#define IO_BLDC_DIAG		PORTBbits.RB14			///< BLDC Diagnostic

// ISR priority level
#define ISR_LEVEL_UART		3			///< UART interrupt priority (lowest)
#define ISR_LEVEL_SPI		4			///< SPI interrupt priority
#define ISR_LEVEL_ADXL		4			///< ADXL345 interrupt priority
#define ISR_LEVEL_ISZ		4			///< ISZ2510 interrupt priority
#define ISR_LEVEL_TACHO		5			///< Tacho ouput interrupt priority

// Virtual connections
#define RP20				20			///< Virtual connection number (pin RP 20)
#define RPI34				34			///< Virtual connection number (pin RPI 34)
#define RPI45				45			///< Virtual connection number (pin RPI 45)
#define U1TX				1			///< Virtual connection function number (U1 Tx)

#define RPI_ISZ_IRQ			RP20		///< ISZ 2510 IRQ tied to RP20
#define RPI_BLDC_TACHO_IRQ	RPI45		///< BLDC tacho IRQ tied to RPI45
#define RPI_UART1_RX		RPI34		///< UART1 Rx tied to RPI24
#define RP_UART1_TX			_RP35R		///< UART1 Tx tied to RP35


// ===========================================================================
// P R I V A T E 
// ===========================================================================

/// EEPROM emulation is done by using one page of flash memory
unsigned int gs_eeprom [PAGE_SIZE] __attribute__((section(".eeprom"), aligned(PAGE_SIZE*sizeof (int)), space(prog)));

/// Kernel configuration
static NS_Config config;

/// Last error;
static NS_Error error;

/// Last tacho pulse timer 1 value
static Word gs_lastTachoTmr = 0;

/// Motor speed [RPM]
static Word gs_motorSpeed = 0;

/// Motor pulses (1pulse = 1/8 turn)
static Word gs_motorPulses = 0;


// ===========================================================================
// P R I V A T E   P R O T O
// ===========================================================================

/// \name Module private functions
//@{
static Word ErrorCallback (const NS_Error* pError);
static void CS_ADXL345 (int s);
static void CS_ISZ2510 (int s);
static void ConfigureClock ();
static void ConfigureProcessor ();
static void ConfigureTachoTimer (Dword Fcy, Word minResolution);
static void ConfigurePwm3Period (Dword Fcy, Dword Fpwm);
static void CreateDevices ();
//@}



// ###########################################################################
//
// I S R
//
// ###########################################################################

#define ISR		__attribute__((__interrupt__, no_auto_psv))


// ===========================================================================
/// \brief Default handlers to catch trap exceptions
// ===========================================================================
void ISR _OscillatorFail(void)
{
	INTCON1bits.OSCFAIL = 0;
	while(1);
}

void ISR _AddressError(void)
{
	INTCON1bits.ADDRERR = 0;
	while(1);
}

void ISR _MathError(void)
{
	INTCON1bits.MATHERR = 0;
	while(1);
}

void ISR _DefaultInterrupt(void)
{
	while(1);
}


// ===========================================================================
// <_CNInterrupt>
///
/// \brief CN Interrupt tied to ADXL345 interrupt
// ===========================================================================
void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt (void)
{
	_CNIF = 0;
	if (IO_INT_ADX == 1) DeviceIrqISR (g_ADXL345);
}	


// ===========================================================================
// <_INT1Interrupt>
///
/// \brief INT1 Interrupt tied to ISZ2510 interrupt
// ===========================================================================
void __attribute__((__interrupt__, no_auto_psv)) _INT1Interrupt (void)
{
	_INT1IF = 0;

    // Warn kernel that device raised an interrupt
	DeviceIrqISR (g_ISZ2510);	
}


// ===========================================================================
// <_T1Interrupt>
///
/// \brief Timer 1 interrupt
///
/// When this interrupt happens, we consider the motor is stopped.
// ===========================================================================
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt (void)
{
	_T1IF = 0;

	// Force the speed to 0 and disable the timer interrupt
	gs_motorSpeed = 0;
	gs_lastTachoTmr = 0;
	_T1IE = 0;
}


// ===========================================================================
// <_INT2Interrupt>
///
/// \brief INT2 Interrupt tied to BLDC tacho pulse
// ===========================================================================
void __attribute__((__interrupt__, no_auto_psv)) _INT2Interrupt (void)
{
	Dword inc;
	Dword speed;
	Word timer;
	Word lastTimer;

	_INT2IF = 0;

	lastTimer = gs_lastTachoTmr;
	timer = TMR1;

	// Compute number of processor ticks between last and current pulse
	inc = (Dword) (timer - gs_lastTachoTmr);
	gs_lastTachoTmr = timer;
	
	// Change the timer interrupt for an additional full timer roll
	// ! PR1 cannot be 0 !
	if (timer != 1) PR1 = timer-1;
	else PR1 = 0xFFFF;

	// Don't compute speed if the motor was considered at rest just before
	// (first pulse is ignored because speed computation would be invalid)
	if (gs_motorSpeed == 0 && lastTimer == 0) 
	{
		// Re-enable the timer interrupt
		_T1IF = 0;
		_T1IE = 1;
		return;
	}

	// Scaling
	switch (T1CONbits.TCKPS)
	{
	case 3: inc <<= 8; break;
	case 2: inc <<= 6; break;
	case 1: inc <<= 3; break;
	case 0: break;
	}

	// Compute speed in RPM (1 pulse = 1/8 turn)
	speed = ((Dword) 60 * PROC_FCY) / (inc * 8);

	// Update speed if it seems correct (< 5000 RPM)
	if (speed < 5000) 
	{
		gs_motorSpeed = (Word) speed;
		gs_motorPulses ++;
	}
}	



// ###########################################################################
//
// P U B L I C
//
// ###########################################################################

// ===========================================================================
// <BSP_Init>
///
/// \brief	Board initialization
// ===========================================================================
int BSP_Init (void)
{
	int r;	
	
	// Change proc clock
	ConfigureClock ();

	// Configure proc I/O
	ConfigureProcessor ();

	// NanoScale OS init
	config.kernelStackSize = 800;
	config.cpuClockHz = PROC_FCY;
	config.fiberQueueLen = 20;
	config.priorityLevels = 16;
	config.rtccMode = RTCC_EMULATED;
	
	r = InitKernel (&config);
	if (r != 0) while (1);

	// Error callback setup
	SetErrorCallback (ErrorCallback);

    // Power saving mode: IDLE
    SetPowerSavingMode (POWER_REDUCTION_L1);
	
	// Initialize drivers and create devices
	CreateDevices ();
	
	return 0;
}


// ===========================================================================
// <BSP_BLDCSetEnable>
///
/// \brief	Enable BLDC controller
///
/// \param[in]	enable		1: Controller enabled
///							0: Controller disabled
// ===========================================================================
void BSP_BLDCSetEnable (int enable)
{
	IO_BLDC_EN = (enable > 0 ? 1 : 0);
}


// ===========================================================================
// <BSP_BLDCSetBrake>
///
/// \brief	Activate BLDC brake
///
/// \param[in]	brake		1: Brake enabled
///							0: Brake disabled
// ===========================================================================
void BSP_BLDCSetBrake (int brake)
{
	// ! Reversed logic
	IO_BLDC_BRAKE = (brake > 0 ? 0 : 1);
}


// ===========================================================================
// <BSP_BLDCSetForward>
///
/// \brief	Select BLDC rotation direction.
///
/// This function makes a normal usage of the Forward/Reverse pin.
///
/// \param[in]	fwd			1: Forward direction
///							0: Reverse direction
// ===========================================================================
void BSP_BLDCSetForward (int fwd)
{
	// Drive the pin as a normal GPIO
	IOCON3bits.PENH = 0;
	IO_BLDC_FWD = (fwd > 0 ? 1 : 0);
}


// ===========================================================================
// <BSP_BLDCSetForwardDuty>
///
/// \brief	Apply a PWM signal on the BLDC Forward/Reverse I/O.
///
/// This function enables to modulate the torque applied on the motor thanks to
/// the Reference I/O. It enables to drive the BLDC motor in full quadrant mode.
///
/// \param[in]	duty		0: Torque balanced -> net torque is 0
///							>0: Net torque is positive (forward direction)
///							<0: Net torque is negative (reverse direction)
// ===========================================================================
void BSP_BLDCSetForwardDuty (Short duty)
{
	Long pdc;
	
	// Compute required duty cycle
	pdc = ((Long) PHASE3 * duty) / 0xFFFF;
	pdc += PHASE3/2;
	PDC3 = (Word) pdc;

	// Apply the PWM signal on this I/O
	IOCON3bits.PENH = 1;
}


// ===========================================================================
// <BSP_BLDCSetRef>
///
/// \brief	Set the BLDC current reference that the BLDC controller try to regulate
///
/// \param[in]	ref			[0..0xFFFF] for [0..2.16A] motor output current
// ===========================================================================
void BSP_BLDCSetRef (Word ref)
{
	Dword v;
	
	// Compute the duty cycle register according to the periodicity register
	v = (PTPER * (Dword) ref);
	MDC = (Word) (v >> 16);
}


// ===========================================================================
// <BSP_BLDCGetSpeed>
///
/// \brief	Return motor speed
///
/// \param[out]	counter		Counter incremented at each new valid measured speed
///
/// \return	Motor speed [RPM]
// ===========================================================================
Word BSP_BLDCGetSpeed (Word *counter)
{
	Word speed;

	_INT2IE = 0;
	{
		speed = gs_motorSpeed;
		if (counter != NULL) *counter = gs_motorPulses;
	}
	_INT2IE = 1;

	return speed;
}


// ===========================================================================
// <BSP_LEDSetRed>
///
/// \brief	Set the Red LED state
///
/// \param[in]	s			1: LED on
///							0: LED off
// ===========================================================================
void BSP_LEDSetRed (int s)
{
	IO_LED_RED = (s > 0 ? 1 : 0);
}


// ===========================================================================
// <BSP_LEDSetGreen>
///
/// \brief	Set the Green LED state
///
/// \param[in]	s			1: LED on
///							0: LED off
// ===========================================================================
void BSP_LEDSetGreen (int s)
{
	IO_LED_GRE = (s > 0 ? 1 : 0);
}


// ===========================================================================
// <BSP_LEDSetYellow>
///
/// \brief	Set the Yellow LED state
///
/// \param[in]	s			1: LED on
///							0: LED off
// ===========================================================================
void BSP_LEDSetYellow (int s)
{
	IO_LED_YEL = (s > 0 ? 1 : 0);
}


// ===========================================================================
// <BSP_BatGetVoltage>
///
/// \brief	Sample and return the battery voltage
///
/// \return	Battery voltage on 12 bits: 0=0V; 0x0FFF=10V
// ===========================================================================
Word BSP_BatGetVoltage ()
{
	Word value;
	int i = 1000;

	// Start sampling AN0
	AD1CON1bits.DONE = 0;
	AD1CON1bits.SAMP = 1;

	// Wait end of sampling and conversion
	while (AD1CON1bits.DONE == 0 && i -- > 0);
	
	// Read the result (12 bits)
	value = ADC1BUF0;
	return value;
}


// ===========================================================================
// <BSP_GetClockRate>
///
/// \brief	Returns CPU clock rate
// ===========================================================================
Dword BSP_GetClockRate ()
{
    return PROC_FCY;
}


// ===========================================================================
// <BSP_WriteEeprom>
///
/// \brief		Write EEPROM memory
///
///	EEPROM is emulated thanks to a dedicated flash page
///
/// \param[in]	data	Data to write into EEPROM
/// \param[in]	len		Data len
// ===========================================================================
Bool BSP_WriteEeprom (const Byte* data, int len)
{
	_prog_addressT p;
	int n;

	if (len > PAGE_SIZE*sizeof (int)) return False;

	EnterCritical ();
	{
		// Erase flash page
		_init_prog_address (p, gs_eeprom);
		_erase_flash (p);
	}
	LeaveCritical ();

	// Write data, 32bits at a time
	n = (len +3) >> 2;
	while (n -- > 0)
	{
		EnterCritical ();
		{
			_write_flash_word32 (p, *((int*)data), *((int*)(data+2)));
		}
		LeaveCritical ();
		
		data += 4;
		p += 4;
	}
	return True;
}


// ===========================================================================
// <BSP_ReadEeprom>
///
/// \brief	Read EEPROM memory
///
/// \param[in]	data	Buffer to store data from eeprom
/// \param[in]	len		Data len
// ===========================================================================
void BSP_ReadEeprom (Byte* data, int len)
{
	_prog_addressT p;

	// Read data from EEPROM
	EnterCritical ();
	{
		_init_prog_address (p, gs_eeprom);
		_memcpy_p2d16 (data, p, len);
	}
	LeaveCritical ();
}



// ###########################################################################
//
// P R I V A T E
//
// ###########################################################################

// ===========================================================================
// <ErrorCallback>
///
/// \brief	Callback in case of error
// ===========================================================================
static Word ErrorCallback (const NS_Error* pError)
{
	// Save the error
	error = *pError;
	
	// Switch red LED on
	BSP_LEDSetRed (1);
	BSP_LEDSetYellow (0);
	BSP_LEDSetGreen (0);

	// If we get here, error should be a critical one.
	// Depending on the error, it may be possible to do something.
	// However, we just put an infinite loop on which a breakpoint could be added
	while (1);
	
	return 0;
}


// ===========================================================================
// <ConfigureClock>
///
/// \brief	Change processor clock configuration
///
/// Fin is FRC @ 7.35MHz
// ===========================================================================
static void ConfigureClock ()
{
	// Fref = Fin / N1 =  2.45 MHz (0.8 < Fref < 8 MHz)
	// Fvco = Fin x M / N1 = 159.25 (120 < Fvco < 340 MHz)
	// Fosc = Fvco / N2 = 79.625 Mhz
	// Fcy = Fosc / 2 = 39.8125 MHz
	CLKDIVbits.PLLPRE = 1;		// N1 = 3
	PLLFBD = 63;				// M  = 65
	CLKDIVbits.PLLPOST = 0;		// N2 = 2
	OSCTUN = 0;			

	// Clock switch to FRC with PLL
	__builtin_write_OSCCONH (0x01);		
	__builtin_write_OSCCONL (0x01);

	// Wait for new clock	
	while (OSCCONbits.COSC != 0x1);

	// Wait for PLL lock
	while (OSCCONbits.LOCK == 0);
}


// ===========================================================================
// <ConfigurePwmClock>
///
/// \brief	Configure the PWM module master clock in order to achieve a given
///			frequency and minimal duty cycle resolution.
///
/// \param[in]	Fcy				Processor clock rate [Hz]
/// \param[in]	Fpwm			Desired PWM output periodicity [Hz]
/// \param[in]	numDutySteps	Minimum number of steps for the duty cycle selection
// ===========================================================================
static void ConfigurePwmClock (Dword Fcy, Dword Fpwm, Word numDutySteps)
{
	Dword step, step2;
	int i;

	// Compute the number of steps available for the duty cycle selection, clock pre-scaler x1
	step = 2*Fcy/Fpwm;

	// Select the maximum pwm clock divider that still allow required resolution
	for (i = 6; i >=0; i --)
	{
		step2 = step >> i;
		if (step2 >= numDutySteps) break;
	}
	if (i < 0) i = 0;
	PTCON2bits.PCLKDIV = i;

	// According to the pre-scaler, set the periodicity register
	PTPER = (Word) step2;
}


// ===========================================================================
// <ConfigurePwm3Period>
///
/// \brief	Configure the PWM module 3 clock in order to achieve a given
///			frequency.
///
/// \param[in]	Fcy				Processor clock rate [Hz]
/// \param[in]	Fpwm			Desired PWM output periodicity [Hz]
// ===========================================================================
static void ConfigurePwm3Period (Dword Fcy, Dword Fpwm)
{
	Dword step;
	
	// Compute the number of steps 1 one period, pre-scaler x1
	// Then take the pre-scaler into account
	step = 2*Fcy/Fpwm;
	step >>= PTCON2bits.PCLKDIV;
	
	if (step > 0xFFFF) step = 0xFFFF;
	PHASE3 = (Word) step;
}


// ===========================================================================
// <ConfigureTachoTimer>
///
/// \brief	Configure Timer1 in order to measure time between 2 tacho pulses
///
/// \param[in]	Fcy				Processor clock rate [Hz]
/// \param[in]	minResolution	Minimum timer resolution [us]
// ===========================================================================
static void ConfigureTachoTimer (Dword Fcy, Word minResolution)
{
	Dword step;

	T1CONbits.TSIDL = 0;
	T1CONbits.TCS = 0;
	T1CONbits.TGATE = 0;

	// Select the highest pre-scaler that ensure the given resolution
	step = (Fcy*minResolution) / 1000000;
	if (step >= 256) T1CONbits.TCKPS = 3;
	else if (step >= 64) T1CONbits.TCKPS = 2;
	else if (step >= 8) T1CONbits.TCKPS = 1;
	else T1CONbits.TCKPS = 0;

	gs_lastTachoTmr = 0;

	// Activate Timer1 interrupt
	PR1 = 0xFFFF;
	_T1IP = ISR_LEVEL_TACHO;
	_T1IE = 1;
	T1CONbits.TON = 1;
}


// ===========================================================================
// <CS_ADXL345>
///
/// \brief	Function used for ADXL345 Chip Selection
///
/// \param[in]	s		1: Assert ADXL345
///						0: De-assert ADXL345
// ===========================================================================
static void CS_ADXL345 (int s)
{
	IO_CS_ADX = (s > 0 ? 0 : 1);
}


// ===========================================================================
// <CS_ISZ2510>
///
/// \brief	Function used for ISZ2510 Chip Selection
///
/// \param[in]	s		1: Assert ISZ2510
///						0: De-assert ISZ2510
// ===========================================================================
static void CS_ISZ2510 (int s)
{
	IO_CS_ISZ = (s > 0 ? 0 : 1);
}


// ===========================================================================
// <ConfigureProcessor>
///
/// \brief	Initialize ADC, PWM and I/O and set up initial output level
// ===========================================================================
static void ConfigureProcessor ()
{
	// Configure analog inputs
	// AN0 is the only analog input
	_ANSA0 = 1;
	_ANSA1 = 0;
	_ANSA4 = 0;

	_ANSB0 = 0;
	_ANSB1 = 0;
	_ANSB2 = 0;
	_ANSB3 = 0;
	_ANSB8 = 0;

	// Configure inputs
	IOM_VBAT = INPUT;
	IOM_INT_ADX = INPUT;
	IOM_INT_ISZ = INPUT;

	IOM_BLDC_TACHO = INPUT;
	IOM_BLDC_DIAG = INPUT;

	// Configure outputs
	IOM_CS_ADX = OUTPUT;
	IOM_CS_ISZ = OUTPUT;

	IOM_LED_RED = OUTPUT;
	IOM_LED_YEL = OUTPUT;
	IOM_LED_GRE = OUTPUT;

	IOM_BLDC_FWD = OUTPUT;
	IOM_BLDC_BRAKE = OUTPUT;
	IOM_BLDC_EN = OUTPUT;
	IOM_BLDC_VREF = OUTPUT;

	// Initial output level
	IO_CS_ADX = 1;
	IO_CS_ISZ = 1;

	IO_LED_RED = 0;
	IO_LED_YEL = 0;
	IO_LED_GRE = 0;

	IO_BLDC_FWD = 0;
	IO_BLDC_BRAKE = 0;
	IO_BLDC_EN = 0;

	// Map UART 1 Tx/Rx
	RPINR18bits.U1RXR = RPI_UART1_RX;
	RP_UART1_TX = U1TX;

	// Map Int2 (BLDC tacho: positive edge)
	_INT2R = RPI_BLDC_TACHO_IRQ;
	_INT2EP = 0;
	_INT2IE = 1;
	_INT2IP = ISR_LEVEL_TACHO;

	// Map Int1 (ISZ 2510 IRQ: negative edge)
	_INT1R = RPI_ISZ_IRQ;
	_INT1EP = 1;
	_INT1IE = 0;
	_INT1IP = ISR_LEVEL_ISZ;

	// Enable Change Notification Interrupt for ADXL 345 IRQ
	_CNIEA3 = 1;
	_CNIE = 0;
	_CNIP = ISR_LEVEL_ADXL;

	// Configure the ADC
	AD1CON1bits.ADON = 0;		// ADC module is OFF
	AD1CON1bits.ADSIDL = 0;		// Continue in Idle
	AD1CON1bits.AD12B = 1;		// 12 bits mode
	AD1CON1bits.FORM = 0;		// Unsigned integer
	AD1CON1bits.ASAM = 0;		// Start sampling manually
	AD1CON1bits.SSRC = 3;		// Automatic  conversion
	AD1CON1bits.SSRCG = 0;		// Automatic  conversion

	AD1CON2bits.VCFG = 0;		// vss,vdd as ref voltage
	AD1CON2bits.CSCNA = 0;		// No scan
	AD1CON2bits.CHPS = 0;		// Use channel 0 only
	AD1CON2bits.ALTS = 0;		// Mux A

	AD1CON3bits.ADRC = 1;		// RC clock (TAD = 250ns)
	AD1CON3bits.SAMC = 10;		// Auto sample = 10 TAD

	AD1CON4bits.ADDMAEN = 0;	// No DMA
	AD1CHS0bits.CH0SA = 0;		// Channel 0 connected to AN0
	AD1CHS0bits.CH0NA = 0;		// Channel 0 negative input is Vrefl
	AD1CSSH = 0;				// No input scan
	AD1CSSL = 0;				// No input scan

	AD1CON1bits.ADON = 1;		// ADC module is ON

	// Configure the PWM generator
	PTCONbits.PTEN = 0;			// PWM module is OFF
	PTCONbits.PTSIDL = 0;		// Continue in Idle
	PTCONbits.SEIEN = 0;		// Disable event interrupt
	PTCONbits.EIPU = 0;			// Wait end of current cycle before updating Period
	PTCONbits.SYNCOEN = 0;		// No SYNCO output
	PTCONbits.SYNCEN = 0;		// No external synchronisation

	CHOPbits.CHPCLKEN = 0;		// No chop clock

	PWMCON1bits.FLTIEN = 0;		// Fault interrupt disabled
	PWMCON1bits.TRGIEN = 0;		// Trigger interrupt disabled
	PWMCON1bits.ITB = 0;		// Use master time base
	PWMCON1bits.MDCS = 1;		// Use master duty cycle
	PWMCON1bits.DTC = 2;		// No dead time compensation
	PWMCON1bits.MTBS = 0;		// Use primary master time base
	PWMCON1bits.CAM = 0;		// Edge aligned mode
	PWMCON1bits.XPRES = 0;		//
	PWMCON1bits.IUE = 1;		// Duty Cycle update is applied immediately
	
	PWMCON3bits.FLTIEN = 0;		// Fault interrupt disabled
	PWMCON3bits.TRGIEN = 0;		// Trigger interrupt disabled
	PWMCON3bits.ITB = 1;		// Use PHASE3 time base
	PWMCON3bits.MDCS = 0;		// Use PDC3 duty cycle
	PWMCON3bits.DTC = 2;		// No dead time compensation
	PWMCON3bits.MTBS = 0;		// Use primary master time base
	PWMCON3bits.CAM = 0;		// Edge aligned mode
	PWMCON3bits.XPRES = 0;		//
	PWMCON3bits.IUE = 1;		// Duty Cycle is applied immediately

	IOCON1bits.PENH = 0;		// PWM1H is GPIO
	IOCON1bits.PENL = 1;		// PMW1L is controlled by PWM module
	IOCON1bits.POLL = 0;		// PWM1L is active high
	IOCON1bits.PMOD = 1;		// Redundant mode
	IOCON1bits.OVRENL = 0;		// PWM1L is driven by PWM generator

	IOCON2bits.PENH = 0;		// PWM2H is GPIO
	IOCON2bits.PENL = 0;		// PMW2L is GPIO
	
	IOCON3bits.PENH = 0;		// PWM3H is GPIO for now
	IOCON3bits.PENL = 0;		// PMW3L is GPIO
	IOCON3bits.POLH = 0;		// PWM3H is active high
	IOCON3bits.PMOD = 1;		// Redundant mode
	IOCON3bits.OVRENH = 0;		// PWM3H is driven by PWM generator

	FCLCON1bits.CLMOD = 0;		// Current limit mode disabled
	FCLCON1bits.FLTMOD = 3;		// Fault input disabled
	FCLCON2bits.CLMOD = 0;		// Current limit mode disabled
	FCLCON2bits.FLTMOD = 0;		// Fault input disabled
	FCLCON3bits.CLMOD = 0;		// Current limit mode disabled
	FCLCON3bits.FLTMOD = 3;		// Fault input disabled

	
	// Configure the PWM clock in order to achieve 100kHz with 256 steps
	// This is needed to drive PWM1L for motor voltage reference
	ConfigurePwmClock (PROC_FCY, F_PWM_VREF, 256);
	MDC = 0x0000;				// Master duty cycle: 0%

	// Configure PWM3 period to drive the BLDC driver in full quadrant 
	// through the FWD/REVERSE pin 
	ConfigurePwm3Period (PROC_FCY, F_PWM_FWD);
	PDC3 = PHASE3/2;			// Default Duty cycle: 50% (0 torque)

	PTCONbits.PTEN = 1;			// PWM module is ON

	// Configure Timer1 to measure time between 2 tacho pulses
	// At full speed (6000 rpm) a pulse is received every ~1ms.
	// 10us resolution ensures to have enough accuracy
	ConfigureTachoTimer(PROC_FCY, 10);
}


// ===========================================================================
// <CreateDevices>
///
/// \brief	Initialize drivers and create associated devices
// ===========================================================================
static void CreateDevices ()
{
	UartPIC16_Config configUART;
	SpiPIC16_Config configSPI;
	ADXL345_Config configADXL345;
	ISZ2510_Config configISZ2510;
	Dword baudrate;

    // Set up the required drivers
	UartPIC16_OpenDriver ();
	SpiPIC16_OpenDriver ();
	ADXL345_OpenDriver ();
	ISZ2510_OpenDriver ();

	// UART device - UART
	configUART.StopInIdle = 0;
	configUART.EnableWakeUp = 0;
	configUART.EnableLoopBack = 0;
	configUART.InverseRxPolarity = 0;
	configUART.InverseTxPolarity = 0;
	configUART.EnableTx = 1;
	configUART.Mode = UART_MODE_8BITS_EVEN_PARITY;
	configUART.StopBits = UART_STOPBITS_1;
	configUART.RtsMode = UART_RTS_READY_TO_RECEIVE;
	configUART.ControlFlow = UART_CFLOW_NONE;
	configUART.IsrPriority = ISR_LEVEL_UART;
	configUART.txFifoLength = 100;
	configUART.rxFifoLength = 100;
	
	g_UART = UartPIC16_CreateDevice ("UART", 1, &configUART);

	if (g_UART == INVALID_HANDLE) ThrowError (ERR_UART);
	baudrate = UART_SetBaudRate (g_UART, UART_BAUDRATE);

	// SPI device - SPI1, Master
	configSPI.StopInIdle = 0;
	configSPI.DisableSDO =	0;
	configSPI.EnableSS = 0;
	configSPI.IsrPriority = ISR_LEVEL_SPI;
	g_SPI = SpiPIC16_CreateDevice ("SPI", 1, &configSPI);

	if (g_SPI == INVALID_HANDLE) ThrowError (ERR_SPI);
	SPI_SetMasterMode (g_SPI, 1);

	// ADXL345 Accelerometer device
	configADXL345.spi = g_SPI;
	configADXL345.fCS = CS_ADXL345;
	g_ADXL345 = ADXL345_CreateDevice ("XYZ Acc.", &configADXL345);

	if (g_ADXL345 == INVALID_HANDLE) ThrowError (ERR_ADXL345);

	_CNIF = 0;
	_CNIE = 1;

	// ISZ2510 Z Gyro device
	configISZ2510.spi = g_SPI;
	configISZ2510.fCS = CS_ISZ2510;
	g_ISZ2510 = ISZ2510_CreateDevice ("Z Gyro", &configISZ2510);

	if (g_ISZ2510 == INVALID_HANDLE) ThrowError (ERR_ISZ2510);
	
	_INT1IF = 0;
	_INT1IE = 1;
}




// End
