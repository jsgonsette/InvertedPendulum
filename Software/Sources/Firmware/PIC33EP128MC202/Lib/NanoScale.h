// ###########################################################################
//
// N A N O S C A L E   R T O S
//
/// \file		NanoScale.h
/// \version	2.3.1
/// \date		31/08/2012
/// \author		GenId SPRL
///
/// This file contains the API definition of NanoScale. 
//
//  ###########################################################################

#ifndef _NANOSCALE_H
#define _NANOSCALE_H



// ###########################################################################
//
// P R O C E S S O R
//
// ###########################################################################

#if defined(__dsPIC30F__) || 	\
	defined(__dsPIC33F__) ||	\
	defined(__dsPIC33E__) ||	\
	defined(__PIC24F__) ||	\
	defined(__PIC24FK__) ||	\
	defined(__PIC24H__) ||	\
	defined(__PIC24E__)
	#include "NanoScaleMacro_PIC16.h"

#elif defined(__PIC32MX__) ||	\
	  defined(__PIC32MZ__)
	#include "NanoScaleMacro_PIC32.h"

#endif



// ###########################################################################
//
// E R R O R   C O D E S
//
// ###########################################################################

// General errors
#define ERR_SUCCESS				0		///< Successfull operation
#define	ERR_INVALID_DATA		1		///< Invalid data
#define ERR_STACK_OVERFLOW		3		///< Task stack overflow
#define ERR_OS_STACK_OVERFLOW	4		///< Kernel stack overflow
#define ERR_HANDLE_INVALID		5		///< Invalid handle
#define ERR_ISR					6		///< Function can not be called from an ISR
#define ERR_NO_ISR				7		///< Function can only be called from an ISR
#define ERR_FIBER				8		///< Invalid call for a fiber (mutex or timeout != 0)
#define ERR_FIBER_QUEUE			9		///< Fiber queue is full
#define ERR_LICENSE				10		///< Not permited with this license
#define ERR_HEAP_CORRUPTED		11		///< Heap is corrupted

// Task errors
#define	ERR_TASK_DELETED		20		///< This object has been deleted
#define	ERR_TASK_NOMEM			21		///< No enough memory to create the object

// Mem pool errors
#define	ERR_POOL_DELETED		30		///< This object has been deleted
#define	ERR_POOL_NOMEM			31		///< No enough memory to create the object
#define ERR_POOL_USED			32		///< Pool can not be deleted because it is still being used
#define ERR_POOL_CORRUPTED		33		///< Pool memory block is corrupted

// Event errors
#define	ERR_EV_DELETED			40		///< This object has been deleted
#define	ERR_EV_NOMEM			41		///< No enough memory to create the object

// MQ errors
#define	ERR_MQ_DELETED			50		///< This object has been deleted
#define	ERR_MQ_NOMEM			51		///< No enough memory to create the object
#define ERR_MQ_MSGSIZE			52		///< Invalid message size

// Mutex errors
#define	ERR_MUTEX_DELETED		60		///< This object has been deleted
#define	ERR_MUTEX_NOMEM			61		///< No enough memory to create the object

// Pipe errors	
#define	ERR_PIPE_DELETED		70		///< This object has been deleted
#define	ERR_PIPE_NOMEM			71		///< No enough memory to create the object

// Timer errors
#define	ERR_TIMER_DELETED		80		///< This object has been deleted
#define	ERR_TIMER_NOMEM			81		///< No enough memory to create the object



// ###########################################################################
//
// P R I M I T I V E S
//
// ###########################################################################

typedef unsigned char Bool;			///< Boolean value
typedef unsigned char Byte;			///< Unsigned value, 8 bits
typedef unsigned short int Word;	///< Unsigned value, 16 bits
typedef unsigned long Dword;		///< Unsigned value, 32 bits
typedef unsigned long long Qword;	///< Unsigned value, 64 bits

typedef short int Short;			///< Signed value, 16 bits
typedef long Long;					///< Signed value, 32 bits
typedef long long Llong;			///< Signed value, 64 bits

typedef void* HANDLE;				///< Kernel object handle



// ###########################################################################
//
// D E F I N E S
//
// ###########################################################################

#define INVALID_HANDLE	((HANDLE)-1)		///< Invalid Handle value
#define null			0x0000				///< Null pointer

#define True	1							///< Booelan True 
#define False 	0							///< Booelan False 



// ###########################################################################
//
// T Y P E S
//
// ###########################################################################

/// RTCC configuration modes
typedef enum
{
	RTCC_NONE = 0,			///< No RTCC feature
	RTCC_ENABLED = 1,		///< RTCC enabled
	RTCC_EMULATED = 2		///< RTCC emulated through system clock
}
NS_RtccMode;


/// Timer type
typedef enum
{
	TM_PERIODIC = 0,		///< Periodic timer
	TM_ONESHOT				///< One shot timer
}
NS_TimerType;


/// Behavior when stopping a timer
typedef enum
{
	TM_SIG_NOT = 0,		///< Don't signal the timer
	TM_SIG_NOW,			///< Signal timer immediately
	TM_SIG_END			///< Signal timer at expiration
}
NS_TimerStop;


/// Events waiting modes
typedef enum
{
	EV_WAIT_ALL = 0,	///< Wait until all the event flags are set
	EV_WAIT_ANY			///< Wait until any of the event flags are set
}
NS_EventMode;


/// Behavior when an event occure
typedef enum
{
	EV_NO_RESET = 0,	///< Don't reset event flags
	EV_RESET			///< Reset event flags
}
NS_EventReset;

/// Power reduction mode
typedef enum
{
	POWER_REDUCTION_NONE = 0,	///< No power reduction
	POWER_REDUCTION_L1,			///< First level of power reduction (lowest)
	POWER_REDUCTION_L2			///< Second level of power reduction
}
NS_PowerReductionMode;


/// NanoScale configuration 
typedef struct
{
	Dword cpuClockHz;		///< Number of CPU cycles by seconds
	Word kernelStackSize;	///< Size of the kernel stack
	NS_RtccMode rtccMode;	///< Real Time Clock and Calendar configuration mode
	Byte fiberQueueLen;		///< Length of the LSR queue
	Byte priorityLevels;	///< Number of task priority levels
}
NS_Config;


/// System date time definition
typedef struct
{
	Byte sec;	///< 0..60
	Byte min;	///< 0..59
	Byte hour;	///< 0..23
	Byte mday;	///< 1..31, day in month
	Byte mon;	///< 1..12, month
	Byte wday;	///< 0..6, day in week (0 is monday)
	Word year;	///< >= 1970
	Word yday;	///< 0..365, day in year (0 is 1st January)
} 
SystemTime;


/// Task information
typedef struct
{
	const char* pName;					///< Task name
	Word stackSize;						///< Stack size
	Word timeLog;						///< Processor time, 16 bits fraction
	Byte priority;						///< Priority
	Byte timeSlice;						///< Time slice [ms]
}
NS_TaskInfo;


/// Kernel version definition
typedef struct
{
	Byte major;
	Byte minor;
	Byte revision;
	Word build;
}
NS_Version;


/// Kernel error definition
typedef struct
{
	Word error;					///< Error number
	const char* taskName;		///< Last scheduled thread name
	HANDLE kernelObject;		///< Kernel object in which error happened (if applicable)
	Dword time;					///< System time when error happened
	Byte area;					///< Software area in which error happened (0: normal code, 1: kernel code, >1: ISR level)
}
NS_Error;


/// Timer callback/Fiber entry point
typedef void (*FIBER_START) (void*);

/// Task entry point
typedef void (*TASK_START) (void*);

/// User callback for error notifications
typedef Word (*ERROR_CALLBACK) (const NS_Error* pError);

/// User callback for power reduction notifications
typedef void (*POWER_CALLBACK) (const NS_PowerReductionMode mode, const Long nextEvent);

	
	
// ###########################################################################
//
// P R O T O T Y P E S
//
// ###########################################################################

// K e r n e l   M a n a g e m e n t //

NS_API const NS_Version* GetKernelVersion ();

NS_API int InitKernel (const NS_Config* pConfig);

NS_API void StartKernel ();

NS_API void StopKernel ();

NS_API Dword GetKernelStackUsage ();

NS_API void GetKernelStat (Word *pKernelStat, Word* pIdleStat);


// E r r o r   M a n a g e m e n t //

NS_API void SetErrorCallback (ERROR_CALLBACK func);

NS_API void ThrowError (Word error);

NS_API const NS_Error* GetLastError ();


// P o w e r   M a n a g e m e n t //

NS_API void SetPowerSavingCallback (POWER_CALLBACK func);

NS_API void SetPowerSavingMode (NS_PowerReductionMode mode);

NS_API void SetPowerSavingModeISR (NS_PowerReductionMode mode);


// T r a c k i n g //

NS_API void TrackTask (HANDLE task, unsigned int* pAdd, unsigned int b);

NS_API void TrackFibers (unsigned int* pAdd, unsigned int b);


// F i b e r s

NS_API void RunFiber (FIBER_START pFiber, void* param);

NS_API void RunFiberISR (FIBER_START pFiber, void* param);


// C r i t i c a l   S e c t i o n s //

NS_API void EnterCritical ();

NS_API void LeaveCritical ();


// T a s k s   M a n a g e m e n t //

NS_API HANDLE CreateTask (TASK_START pThread, const char* name, Word stackSize, void* param, Byte priority, Byte timeSlice);

NS_API void CloseTask (HANDLE task);

NS_API Word GetTaskStakUsage (HANDLE task);

NS_API void SwitchTask ();

NS_API void SuspendTask (HANDLE task);

NS_API void SuspendTaskISR (HANDLE task);

NS_API void ResumeTask (HANDLE task);

NS_API void ResumeTaskISR (HANDLE task);

NS_API void SleepTask (Long timeout);

NS_API HANDLE GetCurrentTask ();

NS_API HANDLE GetTaskHandle (int idx);

NS_API Byte GetTaskPriority (HANDLE task);

NS_API void SetTaskPriority (HANDLE task, Byte priority);

NS_API void GetTaskInfo (HANDLE task, NS_TaskInfo* pInfo);


// H e a p   M a n a g e m e n t //

NS_API void* HeapAlloc (Dword size);

NS_API void HeapFree (void* pMem);

NS_API Dword HeapGetStaticUsage ();

NS_API Dword HeapGetUsage (Dword *pFree);

NS_API Dword HeapGetMaxUsage ();


// F i x e d - B l o c k - S i z e   P o o l   M a n a g e m e n t //

NS_API HANDLE CreatePool (Word blockSize, Word numBlocks);

NS_API int ExtendPool (HANDLE pool, Word numBlocks);

NS_API void ClosePool (HANDLE pool);

NS_API void* PoolAlloc (HANDLE pool);

NS_API void* PoolAllocTo (HANDLE pool, Long timeout);

NS_API void* PoolAllocISR (HANDLE pool);

NS_API void PoolFree (HANDLE pool, void* pBloc);

NS_API void PoolFreeISR (HANDLE pool, void* pBloc);

NS_API Word PoolGetUsage (HANDLE pool);

NS_API Word PoolGetMaxUsage (HANDLE pool);


// T i m e r   M a n a g e m e n t //

NS_API HANDLE CreateTimer (FIBER_START func, NS_TimerType type, Long timeout);

NS_API void StartTimer (HANDLE timer, void* param);

NS_API void StartTimerISR (HANDLE timer, void* param);

NS_API void CloseTimer (HANDLE timer, NS_TimerStop stopMode);

NS_API void StopTimer (HANDLE timer, NS_TimerStop stopMode);

NS_API void StopTimerISR (HANDLE timer, NS_TimerStop stopMode);

NS_API void ConfigureTimer (HANDLE timer, NS_TimerType type, Long timeout);


// T i m e   M a n a g e m e n t //

NS_API void GetSystemTime (SystemTime* pTime);

NS_API int SetSystemTime (SystemTime* pTime);

NS_API Dword TimeToTimestamp (SystemTime* pTime);

NS_API void TimestampToTime (Dword timestamp, SystemTime* pTime);

NS_API Dword GetMsCount ();

NS_API Dword GetTickCount ();


// E v e n t s   M a n a g e m e n t //

NS_API HANDLE CreateEvent ();

NS_API void CloseEvent (HANDLE event);

NS_API void SetEvent (HANDLE event, unsigned int flags);

NS_API void SetEventISR (HANDLE event, unsigned int flags);

NS_API void PulseEvent (HANDLE event, unsigned int flags);

NS_API void PulseEventISR (HANDLE event, unsigned int flags);

NS_API unsigned int ResetEvent (HANDLE event, unsigned int flags);

NS_API unsigned int WaitEvent (HANDLE event, NS_EventMode mode, NS_EventReset reset, unsigned int flags);

NS_API unsigned int WaitEventTo (HANDLE event, NS_EventMode mode, NS_EventReset reset, unsigned int flags, Long timeout);


// M u t e x   M a n a g e m e n t //

NS_API HANDLE CreateMutex (Byte ceiling, Bool inheritance);

NS_API void CloseMutex (HANDLE mutex);

NS_API Bool EnterMutex (HANDLE mutex);

NS_API Bool EnterMutexTo (HANDLE mutex, Long timeout);

NS_API void LeaveMutex (HANDLE mutex);


// P i p e s   M a n a g e m e n t //

NS_API HANDLE CreatePipe (Word size);

NS_API void ClosePipe (HANDLE pipe);

NS_API void FlushPipe (HANDLE pipe);

NS_API Word WritePipe (HANDLE pipe, Byte* pData, Word num, Bool atomic);

NS_API Word WritePipeTo (HANDLE pipe, Byte* pData, Word num, Bool atomic, Long timeout);

NS_API Word WritePipeISR (HANDLE pipe, Byte* pData, Word num, Bool atomic);

NS_API Word ReadPipe (HANDLE pipe, Byte* pBuffer, Word num, Bool atomic);

NS_API Word ReadPipeTo (HANDLE pipe, Byte* pBuffer, Word num, Bool atomic, Long timeout);

NS_API Word ReadPipeISR (HANDLE pipe, Byte* pBuffer, Word num, Bool atomic);

NS_API Word GetPipeUsage (HANDLE pipe);

NS_API Word GetPipeMaxUsage (HANDLE pipe);


// M Q   M a n a g e m e n t //

NS_API HANDLE CreateMq (Word slotSize, Word numSlots);

NS_API void CloseMq (HANDLE mq);

NS_API void FlushMq (HANDLE mq);

NS_API Word WriteMq (HANDLE mq, Byte* pData, Word num);

NS_API Word WriteMqTo (HANDLE mq, Byte* pData, Word num, Long timeout);

NS_API Word WriteMqISR (HANDLE mq, Byte* pData, Word num);

NS_API Word ReadMq (HANDLE mq, Byte* pData, Word num);

NS_API Word ReadMqTo (HANDLE mq, Byte* pData, Word num, Long timeout);

NS_API Word ReadMqISR (HANDLE mq, Byte* pData, Word num);

NS_API Word GetMqUsage (HANDLE mq);

NS_API Word GetMqMaxUsage (HANDLE mq);

#endif

// End

