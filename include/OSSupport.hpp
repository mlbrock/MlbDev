// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the operating system support library.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH_MLB_OSSupport_h_HH

#define HH_MLB_OSSupport_h_HH	1

#define MLB_LIB_OSSupport

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file OSSupport.hpp

	\brief	The MLB Operating System Support header file.

	Contains operating system specific classes and definitions.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#ifdef MLB_LIB_OSSupport
# ifdef OSSUPPORT_DLL_EXPORTS
#  define API_OSSUPPORT		MB_LIB_EXPORT
# else
#  define API_OSSUPPORT		MB_LIB_IMPORT
# endif // #ifdef OSSUPPORT_DLL_EXPORTS
#else
#  define API_OSSUPPORT
#endif // #ifdef MLB_LIB_OSSupport

	//	Define MB_LIB_THIS_SIDE to be the {im|ex}port side for shared libs...
#ifdef MB_LIB_THIS_SIDE
#undef MB_LIB_THIS_SIDE
#endif // #ifdef MB_LIB_THIS_SIDE
#define MB_LIB_THIS_SIDE	API_OSSUPPORT

#include <Utility.hpp>

#ifdef _MSC_VER
# include <OSSupport_Win.hpp>
# else
# error "This operating system is not yet supported by the OSSupport library."
#endif // #ifdef _MSC_VER

// ////////////////////////////////////////////////////////////////////////////

/**
	\namespace MLB

	\brief The Michael L. Brock namespace.
*/
namespace MLB {

/**
	\namespace MLB::OSSupport

	\brief The OSSupport namespace.
*/
namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
API_OSSUPPORT MLB::Utility::MemorySizeType GetVirtualAddressSpaceInUse();
API_OSSUPPORT MLB::Utility::MemorySizeType GetPhysicalMemoryInUse();

API_OSSUPPORT MLB::Utility::MemorySizeType GetVirtualAddressRange(
	MLB::Utility::MemorySizeType &min_address,
	MLB::Utility::MemorySizeType &max_address);
API_OSSUPPORT MLB::Utility::MemorySizeType GetVirtualAddressRange();

API_OSSUPPORT unsigned int             ProcessNameToId(
	const std::string &process_name,
	std::vector<MLB::Utility::ProcessId> &process_id_list,
	unsigned int max_count = 0, bool throw_if_not_found = false);
API_OSSUPPORT MLB::Utility::ProcessId  ProcessNameToId(
	const std::string &process_name);
API_OSSUPPORT std::string             &ProcessIdToName(
	MLB::Utility::ProcessId process_id, std::string &process_name);
API_OSSUPPORT std::string              ProcessIdToName(
	MLB::Utility::ProcessId process_id);

API_OSSUPPORT std::string &GetProcessExecutableName(std::string &process_name);
API_OSSUPPORT std::string  GetProcessExecutableName();

API_OSSUPPORT std::pair<MLB::Utility::ProcessId, std::string>
	ResolveProcessSpecifier(const char *process_specifier);
API_OSSUPPORT std::pair<MLB::Utility::ProcessId, std::string>
	ResolveProcessSpecifier(const std::string &process_specifier);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_OSSUPPORT void ThrowNotSupported(const char *func_name,
	const char *other_text = NULL);
API_OSSUPPORT void ThrowNotSupported(const std::string &func_name,
	const std::string &other_text = "");
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#endif // #ifndef HH_MLB_OSSupport_h_HH

