// ###########################################################################
//
// N A N O S C A L E   R T O S
//
/// \file		NanoScaleMacro_PIC16.h
/// \version	2.3.1
/// \date		31/08/2012
/// \author		GenId SPRL
///
/// This file extends NanoScale API by including MACRO definition
/// dedicated to the Microchip PIC24 and dsPIC famillies.
//
//  ###########################################################################

#if defined(__C30__) || defined (__XC)
	#define NS_MICROCHIP
	#define	NS_API
	#define NS_SDK

	#if defined (__HAS_DSP__)
		#define NS_DSP
	#endif
#endif



// ###########################################################################
//
// M A C R O
//
// ###########################################################################

// --> Change this setting according to your configuration. <--
#define	CFG_CPU_CLOCK_HZ		40000000


// ===========================================================================
// T I M E O U T
// ===========================================================================
#define DELAY_S(s)			\
	((Long)(s) * (CFG_CPU_CLOCK_HZ))

#define DELAY_MS(ms)		\
	((Long)(ms) * (CFG_CPU_CLOCK_HZ / 1000))

#define DELAY_US(us)		\
	((Long)(us) * (CFG_CPU_CLOCK_HZ / 1000000))

	
// ===========================================================================
// I S R   M a n a g e m e n t
// ===========================================================================
extern unsigned int	_gs_osStack;
extern unsigned int	_gs_osStackLimit;

#if defined (NS_DSP)

	#define SystemStackISRDef(fn,psv,sh)		\
	void __attribute__((						\
		__interrupt__(__preprologue__(			\
			"	push	SPLIM;\n"				\
			"	push	W0;\n"					\
			"	mov		#0xFFFE, W0;\n"			\
			"	mov		W0, SPLIM;\n"			\
			"	mov		W15, W0;\n"				\
			"	cp		_gs_osStackLimit;\n"	\
			"	bra		gtu, 0f;\n"				\
			"	mov		_gs_osStack, W15;\n"	\
			"0: push	W0;\n"				 	\
			"	push	MODCON;\n"				\
			"	mov		#0x0FFF, W0;\n"			\
			"	mov		W0, MODCON;\n"			\
			"	rcall	1f;\n"				 	\
			"	pop		MODCON;\n"				\
			"	pop		W15;\n"					\
			"	pop		W0;\n"					\
			"	pop		SPLIM;\n"				\
			"	retfie;\n"						\
			"1:	mov		SR, w0;\n"				\
			"	mov.b	w0, [w15-1];"			\
		)),				 						\
		psv, 									\
		sh)) 									\
	fn (void)

#else

	#define SystemStackISRDef(fn,psv,sh)		\
	void __attribute__((						\
		__interrupt__(__preprologue__(			\
			"	push	SPLIM;\n"				\
			"	push	W0;\n"					\
			"	mov		#0xFFFE, W0;\n"			\
			"	mov		W0, SPLIM;\n"			\
			"	mov		W15, W0;\n"				\
			"	cp		_gs_osStackLimit;\n"	\
			"	bra		gtu, 0f;\n"				\
			"	mov		_gs_osStack, W15;\n"	\
			"0: push	W0;\n"				 	\
			"	rcall	1f;\n"				 	\
			"	pop		W15;\n"					\
			"	pop		W0;\n"					\
			"	pop		SPLIM;\n"				\
			"	retfie;\n"						\
			"1:	mov		SR, w0;\n"				\
			"	mov.b	w0, [w15-1];"			\
		)),				 						\
		psv, 									\
		sh)) 									\
	fn (void)

#endif

#define SystemStackISR(fn,psv)			SystemStackISRDef(fn,psv,)
#define SystemStackISRShadow(fn,psv)	SystemStackISRDef(fn,psv,shadow)

// End
