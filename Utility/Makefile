# #############################################################################
# #############################################################################
# Utility Library Make File
# #############################################################################
#
# File Description: Utility library make file.
#
# Revision History: 2000-01-13 --- Creation.
#                       Michael L. Brock
#
#       Copyright Michael L. Brock 2000 - 2015.
#       Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#       http://www.boost.org/LICENSE_1_0.txt)
#
# #############################################################################

include ../.make/MakePrefix.mk

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
			StringSupport.cpp		\
			SystemError.cpp			\
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

#TEST_ExecProcess.cpp	:	ExecProcess.cpp
#	cp -p ExecProcess.cpp TEST_ExecProcess.cpp
#
#
#TARGET_BINS	=	TEST_ExecProcess

include ../.make/MakeSuffix.mk

