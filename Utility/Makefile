# #############################################################################
# #############################################################################
# Multiple Architecture Source Code Production System (MASCaPS) Version 3
# 	Utility Library Make File
# #############################################################################
#
# File Name       : Utility/Makefile
#
# File Description: Utility library make file.
#
# Revision History: 2000-01-13 --- Creation.
#                       Michael L. Brock
#                   2015-03-29 --- Modified for use with MASCaPS Version 3.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2000 - 2018.
#
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

include ../.MASCaPS/MakePrefixFirst.mk

TARGET_LIBS	=	libUtility.a

TARGET_BINS	=

PENDING_SRCS	=	\
			OSFileSupport.cpp		\
			ProcessMemLock.cpp		\
			SimpleMMapFile.cpp

SRCS		=	\
			C_Memory.cpp			\
			C_MemoryX.cpp			\
			C_StdLib.cpp			\
			C_String.cpp			\
			CompiledAlignmentInfo.cpp	\
			ConfigFileSupport.cpp		\
			CriticalEventHandler.cpp	\
			DemangleCppName.cpp		\
			EmitRuledBuffer.cpp		\
			EnvElement.cpp			\
			ExecProcess.cpp			\
			ExecProcessInfo.cpp		\
			FilesystemSupport.cpp		\
			GetEnvironment.cpp		\
			HostArchitectureInfo.cpp	\
			HostName.cpp			\
			HostVirtualMemoryPageInfo.cpp	\
			HRTimer.cpp			\
			HRTimeStamp.cpp			\
			InlineContainer.cpp		\
			IntToString.cpp			\
			Ita2ToAscii.cpp			\
			LogFileSupport.cpp		\
			LogHandlerFile.cpp		\
			LogHandlerFileBase.cpp		\
			LogHandlerFileMMap.cpp		\
			LogManager.cpp			\
			MACAddress.cpp			\
			MapUtf8ToAscii.cpp		\
			nstrcat.cpp			\
			nstrcpy.cpp			\
			OStreamMultiplexer.cpp		\
			PageSize.cpp			\
			ParseCmdLineArg.cpp		\
			PathName.cpp			\
			PosixCRegex.cpp			\
			ProcessId.cpp			\
			ReadFile.cpp			\
			RegexParamNameAdaptor.cpp	\
			ReplaceChars.cpp		\
			ResourceLimits.cpp		\
			Rethrow.cpp			\
			RsrcUsage.cpp			\
			SignalHandler.cpp		\
			Sleep.cpp			\
			StringRadixOps.cpp		\
			StringSupport.cpp		\
			SystemError.cpp			\
			TabularReportSupport.cpp	\
			ThreadId.cpp			\
			ThrowError.cpp			\
			Time.cpp			\
			TimeSpec.cpp			\
			TimeT.cpp			\
			TimeTM.cpp			\
			TimeVal.cpp			\
			ToHexString.cpp			\
			trim.cpp			\
			TZSetInfo.cpp			\
			UniqueId.cpp			\
			UserName.cpp			\
			UtilityException.cpp		\
			VersionNumber.cpp		\
			WaitProcessInfo.cpp		\
			WideChar.cpp

#LINK_STATIC	=	${LINK_STATIC_BIN}

# Only the libraries below are needed for TEST_MAIN unit tests in this library.
MLB_LIB_NAMES	=	Utility

include ../.MASCaPS/MakeSuffixFirst.mk
# ###################################################################

