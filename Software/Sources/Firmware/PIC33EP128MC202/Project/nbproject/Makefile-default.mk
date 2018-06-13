#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Project.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Project.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../Sources/Algorithms/KalmanFree.c ../Sources/Algorithms/CtrlJump.c ../Sources/Algorithms/CtrlEquilibrium.c ../Sources/Algorithms/Trajectory.c ../Sources/Algorithms/CtrlFall.c ../Sources/Devices/ADXL345.c ../Sources/Devices/ISZ2510.c ../Sources/Interfaces/Battery.c ../Sources/Interfaces/Motor.c ../Sources/Tasks/TaskUartTx.c ../Sources/Tasks/TaskUartRx.c ../Sources/Tasks/TaskLED.c ../Sources/Tasks/TaskMain.c ../Sources/Tasks/Controller.c ../Sources/Tools/Matrix.c ../Sources/Tools/Message.c ../Sources/Tools/FastTrigo.c ../Sources/Tools/PRNG.c ../Sources/Main.c ../Sources/BSP.c ../Sources/Global.c ../Sources/Config.c ../Sources/Pendulum.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/907789942/KalmanFree.o ${OBJECTDIR}/_ext/907789942/CtrlJump.o ${OBJECTDIR}/_ext/907789942/CtrlEquilibrium.o ${OBJECTDIR}/_ext/907789942/Trajectory.o ${OBJECTDIR}/_ext/907789942/CtrlFall.o ${OBJECTDIR}/_ext/494984951/ADXL345.o ${OBJECTDIR}/_ext/494984951/ISZ2510.o ${OBJECTDIR}/_ext/906585600/Battery.o ${OBJECTDIR}/_ext/906585600/Motor.o ${OBJECTDIR}/_ext/265448488/TaskUartTx.o ${OBJECTDIR}/_ext/265448488/TaskUartRx.o ${OBJECTDIR}/_ext/265448488/TaskLED.o ${OBJECTDIR}/_ext/265448488/TaskMain.o ${OBJECTDIR}/_ext/265448488/Controller.o ${OBJECTDIR}/_ext/265861749/Matrix.o ${OBJECTDIR}/_ext/265861749/Message.o ${OBJECTDIR}/_ext/265861749/FastTrigo.o ${OBJECTDIR}/_ext/265861749/PRNG.o ${OBJECTDIR}/_ext/2037729719/Main.o ${OBJECTDIR}/_ext/2037729719/BSP.o ${OBJECTDIR}/_ext/2037729719/Global.o ${OBJECTDIR}/_ext/2037729719/Config.o ${OBJECTDIR}/_ext/2037729719/Pendulum.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/907789942/KalmanFree.o.d ${OBJECTDIR}/_ext/907789942/CtrlJump.o.d ${OBJECTDIR}/_ext/907789942/CtrlEquilibrium.o.d ${OBJECTDIR}/_ext/907789942/Trajectory.o.d ${OBJECTDIR}/_ext/907789942/CtrlFall.o.d ${OBJECTDIR}/_ext/494984951/ADXL345.o.d ${OBJECTDIR}/_ext/494984951/ISZ2510.o.d ${OBJECTDIR}/_ext/906585600/Battery.o.d ${OBJECTDIR}/_ext/906585600/Motor.o.d ${OBJECTDIR}/_ext/265448488/TaskUartTx.o.d ${OBJECTDIR}/_ext/265448488/TaskUartRx.o.d ${OBJECTDIR}/_ext/265448488/TaskLED.o.d ${OBJECTDIR}/_ext/265448488/TaskMain.o.d ${OBJECTDIR}/_ext/265448488/Controller.o.d ${OBJECTDIR}/_ext/265861749/Matrix.o.d ${OBJECTDIR}/_ext/265861749/Message.o.d ${OBJECTDIR}/_ext/265861749/FastTrigo.o.d ${OBJECTDIR}/_ext/265861749/PRNG.o.d ${OBJECTDIR}/_ext/2037729719/Main.o.d ${OBJECTDIR}/_ext/2037729719/BSP.o.d ${OBJECTDIR}/_ext/2037729719/Global.o.d ${OBJECTDIR}/_ext/2037729719/Config.o.d ${OBJECTDIR}/_ext/2037729719/Pendulum.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/907789942/KalmanFree.o ${OBJECTDIR}/_ext/907789942/CtrlJump.o ${OBJECTDIR}/_ext/907789942/CtrlEquilibrium.o ${OBJECTDIR}/_ext/907789942/Trajectory.o ${OBJECTDIR}/_ext/907789942/CtrlFall.o ${OBJECTDIR}/_ext/494984951/ADXL345.o ${OBJECTDIR}/_ext/494984951/ISZ2510.o ${OBJECTDIR}/_ext/906585600/Battery.o ${OBJECTDIR}/_ext/906585600/Motor.o ${OBJECTDIR}/_ext/265448488/TaskUartTx.o ${OBJECTDIR}/_ext/265448488/TaskUartRx.o ${OBJECTDIR}/_ext/265448488/TaskLED.o ${OBJECTDIR}/_ext/265448488/TaskMain.o ${OBJECTDIR}/_ext/265448488/Controller.o ${OBJECTDIR}/_ext/265861749/Matrix.o ${OBJECTDIR}/_ext/265861749/Message.o ${OBJECTDIR}/_ext/265861749/FastTrigo.o ${OBJECTDIR}/_ext/265861749/PRNG.o ${OBJECTDIR}/_ext/2037729719/Main.o ${OBJECTDIR}/_ext/2037729719/BSP.o ${OBJECTDIR}/_ext/2037729719/Global.o ${OBJECTDIR}/_ext/2037729719/Config.o ${OBJECTDIR}/_ext/2037729719/Pendulum.o

# Source Files
SOURCEFILES=../Sources/Algorithms/KalmanFree.c ../Sources/Algorithms/CtrlJump.c ../Sources/Algorithms/CtrlEquilibrium.c ../Sources/Algorithms/Trajectory.c ../Sources/Algorithms/CtrlFall.c ../Sources/Devices/ADXL345.c ../Sources/Devices/ISZ2510.c ../Sources/Interfaces/Battery.c ../Sources/Interfaces/Motor.c ../Sources/Tasks/TaskUartTx.c ../Sources/Tasks/TaskUartRx.c ../Sources/Tasks/TaskLED.c ../Sources/Tasks/TaskMain.c ../Sources/Tasks/Controller.c ../Sources/Tools/Matrix.c ../Sources/Tools/Message.c ../Sources/Tools/FastTrigo.c ../Sources/Tools/PRNG.c ../Sources/Main.c ../Sources/BSP.c ../Sources/Global.c ../Sources/Config.c ../Sources/Pendulum.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Project.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP128MC202
MP_LINKER_FILE_OPTION=,--script=p33EP128MC202.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/907789942/KalmanFree.o: ../Sources/Algorithms/KalmanFree.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/907789942" 
	@${RM} ${OBJECTDIR}/_ext/907789942/KalmanFree.o.d 
	@${RM} ${OBJECTDIR}/_ext/907789942/KalmanFree.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Algorithms/KalmanFree.c  -o ${OBJECTDIR}/_ext/907789942/KalmanFree.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/907789942/KalmanFree.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/907789942/KalmanFree.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/907789942/CtrlJump.o: ../Sources/Algorithms/CtrlJump.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/907789942" 
	@${RM} ${OBJECTDIR}/_ext/907789942/CtrlJump.o.d 
	@${RM} ${OBJECTDIR}/_ext/907789942/CtrlJump.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Algorithms/CtrlJump.c  -o ${OBJECTDIR}/_ext/907789942/CtrlJump.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/907789942/CtrlJump.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/907789942/CtrlJump.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/907789942/CtrlEquilibrium.o: ../Sources/Algorithms/CtrlEquilibrium.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/907789942" 
	@${RM} ${OBJECTDIR}/_ext/907789942/CtrlEquilibrium.o.d 
	@${RM} ${OBJECTDIR}/_ext/907789942/CtrlEquilibrium.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Algorithms/CtrlEquilibrium.c  -o ${OBJECTDIR}/_ext/907789942/CtrlEquilibrium.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/907789942/CtrlEquilibrium.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/907789942/CtrlEquilibrium.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/907789942/Trajectory.o: ../Sources/Algorithms/Trajectory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/907789942" 
	@${RM} ${OBJECTDIR}/_ext/907789942/Trajectory.o.d 
	@${RM} ${OBJECTDIR}/_ext/907789942/Trajectory.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Algorithms/Trajectory.c  -o ${OBJECTDIR}/_ext/907789942/Trajectory.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/907789942/Trajectory.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/907789942/Trajectory.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/907789942/CtrlFall.o: ../Sources/Algorithms/CtrlFall.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/907789942" 
	@${RM} ${OBJECTDIR}/_ext/907789942/CtrlFall.o.d 
	@${RM} ${OBJECTDIR}/_ext/907789942/CtrlFall.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Algorithms/CtrlFall.c  -o ${OBJECTDIR}/_ext/907789942/CtrlFall.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/907789942/CtrlFall.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/907789942/CtrlFall.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/494984951/ADXL345.o: ../Sources/Devices/ADXL345.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/494984951" 
	@${RM} ${OBJECTDIR}/_ext/494984951/ADXL345.o.d 
	@${RM} ${OBJECTDIR}/_ext/494984951/ADXL345.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Devices/ADXL345.c  -o ${OBJECTDIR}/_ext/494984951/ADXL345.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/494984951/ADXL345.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/494984951/ADXL345.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/494984951/ISZ2510.o: ../Sources/Devices/ISZ2510.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/494984951" 
	@${RM} ${OBJECTDIR}/_ext/494984951/ISZ2510.o.d 
	@${RM} ${OBJECTDIR}/_ext/494984951/ISZ2510.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Devices/ISZ2510.c  -o ${OBJECTDIR}/_ext/494984951/ISZ2510.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/494984951/ISZ2510.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/494984951/ISZ2510.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/906585600/Battery.o: ../Sources/Interfaces/Battery.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/906585600" 
	@${RM} ${OBJECTDIR}/_ext/906585600/Battery.o.d 
	@${RM} ${OBJECTDIR}/_ext/906585600/Battery.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Interfaces/Battery.c  -o ${OBJECTDIR}/_ext/906585600/Battery.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/906585600/Battery.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/906585600/Battery.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/906585600/Motor.o: ../Sources/Interfaces/Motor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/906585600" 
	@${RM} ${OBJECTDIR}/_ext/906585600/Motor.o.d 
	@${RM} ${OBJECTDIR}/_ext/906585600/Motor.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Interfaces/Motor.c  -o ${OBJECTDIR}/_ext/906585600/Motor.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/906585600/Motor.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/906585600/Motor.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265448488/TaskUartTx.o: ../Sources/Tasks/TaskUartTx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265448488" 
	@${RM} ${OBJECTDIR}/_ext/265448488/TaskUartTx.o.d 
	@${RM} ${OBJECTDIR}/_ext/265448488/TaskUartTx.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tasks/TaskUartTx.c  -o ${OBJECTDIR}/_ext/265448488/TaskUartTx.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265448488/TaskUartTx.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265448488/TaskUartTx.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265448488/TaskUartRx.o: ../Sources/Tasks/TaskUartRx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265448488" 
	@${RM} ${OBJECTDIR}/_ext/265448488/TaskUartRx.o.d 
	@${RM} ${OBJECTDIR}/_ext/265448488/TaskUartRx.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tasks/TaskUartRx.c  -o ${OBJECTDIR}/_ext/265448488/TaskUartRx.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265448488/TaskUartRx.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265448488/TaskUartRx.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265448488/TaskLED.o: ../Sources/Tasks/TaskLED.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265448488" 
	@${RM} ${OBJECTDIR}/_ext/265448488/TaskLED.o.d 
	@${RM} ${OBJECTDIR}/_ext/265448488/TaskLED.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tasks/TaskLED.c  -o ${OBJECTDIR}/_ext/265448488/TaskLED.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265448488/TaskLED.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265448488/TaskLED.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265448488/TaskMain.o: ../Sources/Tasks/TaskMain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265448488" 
	@${RM} ${OBJECTDIR}/_ext/265448488/TaskMain.o.d 
	@${RM} ${OBJECTDIR}/_ext/265448488/TaskMain.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tasks/TaskMain.c  -o ${OBJECTDIR}/_ext/265448488/TaskMain.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265448488/TaskMain.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265448488/TaskMain.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265448488/Controller.o: ../Sources/Tasks/Controller.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265448488" 
	@${RM} ${OBJECTDIR}/_ext/265448488/Controller.o.d 
	@${RM} ${OBJECTDIR}/_ext/265448488/Controller.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tasks/Controller.c  -o ${OBJECTDIR}/_ext/265448488/Controller.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265448488/Controller.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265448488/Controller.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265861749/Matrix.o: ../Sources/Tools/Matrix.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265861749" 
	@${RM} ${OBJECTDIR}/_ext/265861749/Matrix.o.d 
	@${RM} ${OBJECTDIR}/_ext/265861749/Matrix.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tools/Matrix.c  -o ${OBJECTDIR}/_ext/265861749/Matrix.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265861749/Matrix.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265861749/Matrix.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265861749/Message.o: ../Sources/Tools/Message.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265861749" 
	@${RM} ${OBJECTDIR}/_ext/265861749/Message.o.d 
	@${RM} ${OBJECTDIR}/_ext/265861749/Message.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tools/Message.c  -o ${OBJECTDIR}/_ext/265861749/Message.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265861749/Message.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265861749/Message.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265861749/FastTrigo.o: ../Sources/Tools/FastTrigo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265861749" 
	@${RM} ${OBJECTDIR}/_ext/265861749/FastTrigo.o.d 
	@${RM} ${OBJECTDIR}/_ext/265861749/FastTrigo.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tools/FastTrigo.c  -o ${OBJECTDIR}/_ext/265861749/FastTrigo.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265861749/FastTrigo.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265861749/FastTrigo.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265861749/PRNG.o: ../Sources/Tools/PRNG.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265861749" 
	@${RM} ${OBJECTDIR}/_ext/265861749/PRNG.o.d 
	@${RM} ${OBJECTDIR}/_ext/265861749/PRNG.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tools/PRNG.c  -o ${OBJECTDIR}/_ext/265861749/PRNG.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265861749/PRNG.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265861749/PRNG.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2037729719/Main.o: ../Sources/Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2037729719" 
	@${RM} ${OBJECTDIR}/_ext/2037729719/Main.o.d 
	@${RM} ${OBJECTDIR}/_ext/2037729719/Main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Main.c  -o ${OBJECTDIR}/_ext/2037729719/Main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2037729719/Main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/2037729719/Main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2037729719/BSP.o: ../Sources/BSP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2037729719" 
	@${RM} ${OBJECTDIR}/_ext/2037729719/BSP.o.d 
	@${RM} ${OBJECTDIR}/_ext/2037729719/BSP.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/BSP.c  -o ${OBJECTDIR}/_ext/2037729719/BSP.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2037729719/BSP.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/2037729719/BSP.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2037729719/Global.o: ../Sources/Global.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2037729719" 
	@${RM} ${OBJECTDIR}/_ext/2037729719/Global.o.d 
	@${RM} ${OBJECTDIR}/_ext/2037729719/Global.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Global.c  -o ${OBJECTDIR}/_ext/2037729719/Global.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2037729719/Global.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/2037729719/Global.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2037729719/Config.o: ../Sources/Config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2037729719" 
	@${RM} ${OBJECTDIR}/_ext/2037729719/Config.o.d 
	@${RM} ${OBJECTDIR}/_ext/2037729719/Config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Config.c  -o ${OBJECTDIR}/_ext/2037729719/Config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2037729719/Config.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/2037729719/Config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2037729719/Pendulum.o: ../Sources/Pendulum.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2037729719" 
	@${RM} ${OBJECTDIR}/_ext/2037729719/Pendulum.o.d 
	@${RM} ${OBJECTDIR}/_ext/2037729719/Pendulum.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Pendulum.c  -o ${OBJECTDIR}/_ext/2037729719/Pendulum.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2037729719/Pendulum.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/2037729719/Pendulum.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/907789942/KalmanFree.o: ../Sources/Algorithms/KalmanFree.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/907789942" 
	@${RM} ${OBJECTDIR}/_ext/907789942/KalmanFree.o.d 
	@${RM} ${OBJECTDIR}/_ext/907789942/KalmanFree.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Algorithms/KalmanFree.c  -o ${OBJECTDIR}/_ext/907789942/KalmanFree.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/907789942/KalmanFree.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/907789942/KalmanFree.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/907789942/CtrlJump.o: ../Sources/Algorithms/CtrlJump.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/907789942" 
	@${RM} ${OBJECTDIR}/_ext/907789942/CtrlJump.o.d 
	@${RM} ${OBJECTDIR}/_ext/907789942/CtrlJump.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Algorithms/CtrlJump.c  -o ${OBJECTDIR}/_ext/907789942/CtrlJump.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/907789942/CtrlJump.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/907789942/CtrlJump.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/907789942/CtrlEquilibrium.o: ../Sources/Algorithms/CtrlEquilibrium.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/907789942" 
	@${RM} ${OBJECTDIR}/_ext/907789942/CtrlEquilibrium.o.d 
	@${RM} ${OBJECTDIR}/_ext/907789942/CtrlEquilibrium.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Algorithms/CtrlEquilibrium.c  -o ${OBJECTDIR}/_ext/907789942/CtrlEquilibrium.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/907789942/CtrlEquilibrium.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/907789942/CtrlEquilibrium.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/907789942/Trajectory.o: ../Sources/Algorithms/Trajectory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/907789942" 
	@${RM} ${OBJECTDIR}/_ext/907789942/Trajectory.o.d 
	@${RM} ${OBJECTDIR}/_ext/907789942/Trajectory.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Algorithms/Trajectory.c  -o ${OBJECTDIR}/_ext/907789942/Trajectory.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/907789942/Trajectory.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/907789942/Trajectory.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/907789942/CtrlFall.o: ../Sources/Algorithms/CtrlFall.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/907789942" 
	@${RM} ${OBJECTDIR}/_ext/907789942/CtrlFall.o.d 
	@${RM} ${OBJECTDIR}/_ext/907789942/CtrlFall.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Algorithms/CtrlFall.c  -o ${OBJECTDIR}/_ext/907789942/CtrlFall.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/907789942/CtrlFall.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/907789942/CtrlFall.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/494984951/ADXL345.o: ../Sources/Devices/ADXL345.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/494984951" 
	@${RM} ${OBJECTDIR}/_ext/494984951/ADXL345.o.d 
	@${RM} ${OBJECTDIR}/_ext/494984951/ADXL345.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Devices/ADXL345.c  -o ${OBJECTDIR}/_ext/494984951/ADXL345.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/494984951/ADXL345.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/494984951/ADXL345.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/494984951/ISZ2510.o: ../Sources/Devices/ISZ2510.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/494984951" 
	@${RM} ${OBJECTDIR}/_ext/494984951/ISZ2510.o.d 
	@${RM} ${OBJECTDIR}/_ext/494984951/ISZ2510.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Devices/ISZ2510.c  -o ${OBJECTDIR}/_ext/494984951/ISZ2510.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/494984951/ISZ2510.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/494984951/ISZ2510.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/906585600/Battery.o: ../Sources/Interfaces/Battery.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/906585600" 
	@${RM} ${OBJECTDIR}/_ext/906585600/Battery.o.d 
	@${RM} ${OBJECTDIR}/_ext/906585600/Battery.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Interfaces/Battery.c  -o ${OBJECTDIR}/_ext/906585600/Battery.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/906585600/Battery.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/906585600/Battery.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/906585600/Motor.o: ../Sources/Interfaces/Motor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/906585600" 
	@${RM} ${OBJECTDIR}/_ext/906585600/Motor.o.d 
	@${RM} ${OBJECTDIR}/_ext/906585600/Motor.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Interfaces/Motor.c  -o ${OBJECTDIR}/_ext/906585600/Motor.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/906585600/Motor.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/906585600/Motor.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265448488/TaskUartTx.o: ../Sources/Tasks/TaskUartTx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265448488" 
	@${RM} ${OBJECTDIR}/_ext/265448488/TaskUartTx.o.d 
	@${RM} ${OBJECTDIR}/_ext/265448488/TaskUartTx.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tasks/TaskUartTx.c  -o ${OBJECTDIR}/_ext/265448488/TaskUartTx.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265448488/TaskUartTx.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265448488/TaskUartTx.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265448488/TaskUartRx.o: ../Sources/Tasks/TaskUartRx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265448488" 
	@${RM} ${OBJECTDIR}/_ext/265448488/TaskUartRx.o.d 
	@${RM} ${OBJECTDIR}/_ext/265448488/TaskUartRx.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tasks/TaskUartRx.c  -o ${OBJECTDIR}/_ext/265448488/TaskUartRx.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265448488/TaskUartRx.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265448488/TaskUartRx.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265448488/TaskLED.o: ../Sources/Tasks/TaskLED.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265448488" 
	@${RM} ${OBJECTDIR}/_ext/265448488/TaskLED.o.d 
	@${RM} ${OBJECTDIR}/_ext/265448488/TaskLED.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tasks/TaskLED.c  -o ${OBJECTDIR}/_ext/265448488/TaskLED.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265448488/TaskLED.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265448488/TaskLED.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265448488/TaskMain.o: ../Sources/Tasks/TaskMain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265448488" 
	@${RM} ${OBJECTDIR}/_ext/265448488/TaskMain.o.d 
	@${RM} ${OBJECTDIR}/_ext/265448488/TaskMain.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tasks/TaskMain.c  -o ${OBJECTDIR}/_ext/265448488/TaskMain.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265448488/TaskMain.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265448488/TaskMain.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265448488/Controller.o: ../Sources/Tasks/Controller.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265448488" 
	@${RM} ${OBJECTDIR}/_ext/265448488/Controller.o.d 
	@${RM} ${OBJECTDIR}/_ext/265448488/Controller.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tasks/Controller.c  -o ${OBJECTDIR}/_ext/265448488/Controller.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265448488/Controller.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265448488/Controller.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265861749/Matrix.o: ../Sources/Tools/Matrix.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265861749" 
	@${RM} ${OBJECTDIR}/_ext/265861749/Matrix.o.d 
	@${RM} ${OBJECTDIR}/_ext/265861749/Matrix.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tools/Matrix.c  -o ${OBJECTDIR}/_ext/265861749/Matrix.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265861749/Matrix.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265861749/Matrix.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265861749/Message.o: ../Sources/Tools/Message.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265861749" 
	@${RM} ${OBJECTDIR}/_ext/265861749/Message.o.d 
	@${RM} ${OBJECTDIR}/_ext/265861749/Message.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tools/Message.c  -o ${OBJECTDIR}/_ext/265861749/Message.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265861749/Message.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265861749/Message.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265861749/FastTrigo.o: ../Sources/Tools/FastTrigo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265861749" 
	@${RM} ${OBJECTDIR}/_ext/265861749/FastTrigo.o.d 
	@${RM} ${OBJECTDIR}/_ext/265861749/FastTrigo.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tools/FastTrigo.c  -o ${OBJECTDIR}/_ext/265861749/FastTrigo.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265861749/FastTrigo.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265861749/FastTrigo.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265861749/PRNG.o: ../Sources/Tools/PRNG.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265861749" 
	@${RM} ${OBJECTDIR}/_ext/265861749/PRNG.o.d 
	@${RM} ${OBJECTDIR}/_ext/265861749/PRNG.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Tools/PRNG.c  -o ${OBJECTDIR}/_ext/265861749/PRNG.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/265861749/PRNG.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/265861749/PRNG.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2037729719/Main.o: ../Sources/Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2037729719" 
	@${RM} ${OBJECTDIR}/_ext/2037729719/Main.o.d 
	@${RM} ${OBJECTDIR}/_ext/2037729719/Main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Main.c  -o ${OBJECTDIR}/_ext/2037729719/Main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2037729719/Main.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/2037729719/Main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2037729719/BSP.o: ../Sources/BSP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2037729719" 
	@${RM} ${OBJECTDIR}/_ext/2037729719/BSP.o.d 
	@${RM} ${OBJECTDIR}/_ext/2037729719/BSP.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/BSP.c  -o ${OBJECTDIR}/_ext/2037729719/BSP.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2037729719/BSP.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/2037729719/BSP.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2037729719/Global.o: ../Sources/Global.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2037729719" 
	@${RM} ${OBJECTDIR}/_ext/2037729719/Global.o.d 
	@${RM} ${OBJECTDIR}/_ext/2037729719/Global.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Global.c  -o ${OBJECTDIR}/_ext/2037729719/Global.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2037729719/Global.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/2037729719/Global.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2037729719/Config.o: ../Sources/Config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2037729719" 
	@${RM} ${OBJECTDIR}/_ext/2037729719/Config.o.d 
	@${RM} ${OBJECTDIR}/_ext/2037729719/Config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Config.c  -o ${OBJECTDIR}/_ext/2037729719/Config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2037729719/Config.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/2037729719/Config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2037729719/Pendulum.o: ../Sources/Pendulum.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2037729719" 
	@${RM} ${OBJECTDIR}/_ext/2037729719/Pendulum.o.d 
	@${RM} ${OBJECTDIR}/_ext/2037729719/Pendulum.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Sources/Pendulum.c  -o ${OBJECTDIR}/_ext/2037729719/Pendulum.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2037729719/Pendulum.o.d"      -mno-eds-warn  -g -omf=elf -no-legacy-libc  -mlarge-code -mlarge-data -O0 -I"../Lib" -I"../Sources" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/2037729719/Pendulum.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Project.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../Lib/libNanoScaleDrivers-33EP128MC202-elf.a ../Lib/libNanoScale-DEBUG-DEMO-dsPIC33E-elf.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Project.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ..\Lib\libNanoScaleDrivers-33EP128MC202-elf.a ..\Lib\libNanoScale-DEBUG-DEMO-dsPIC33E-elf.a  -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -no-legacy-libc   -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Project.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../Lib/libNanoScaleDrivers-33EP128MC202-elf.a ../Lib/libNanoScale-DEBUG-DEMO-dsPIC33E-elf.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Project.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ..\Lib\libNanoScaleDrivers-33EP128MC202-elf.a ..\Lib\libNanoScale-DEBUG-DEMO-dsPIC33E-elf.a  -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -no-legacy-libc  -Wl,--local-stack,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/Project.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
