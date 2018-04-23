// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for utility library.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__Utility_hpp__HH

#define HH__MLB__Utility__Utility_hpp__HH	1

#define MLB__LIB__Utility

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file Utility.hpp

	\brief	The MLB Utility header file.

	Contains general-purpose and utility classes and definitions.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <mbcompat.h>

#if defined(__MINGW32__)
# define _WIN32_WINNT 0x0500
#endif // #if defined(__MINGW32__)

#ifdef MLB__LIB__Utility
# ifdef UTILITY_DLL_EXPORTS
#  define API_UTILITY		MB_LIB_EXPORT
# else
#  define API_UTILITY		MB_LIB_IMPORT
# endif // #ifdef UTILITY_DLL_EXPORTS
#else
#  define API_UTILITY
#endif // #ifdef MLB__LIB__Utility

	//	Define MB_LIB_THIS_SIDE to be the {im|ex}port side for shared libs...
#ifdef MB_LIB_THIS_SIDE
#undef MB_LIB_THIS_SIDE
#endif // #ifdef MB_LIB_THIS_SIDE
#define MB_LIB_THIS_SIDE	API_UTILITY

#ifdef _MSC_VER
# if !defined(__MINGW32__)
#  pragma warning(push)
#  pragma warning(disable:4018 4100 4146 4244 4290 4511 4512 4663)
#  if (_MSC_VER == 1600)	//	Needed for new and delete that one version.
#   pragma warning(disable:4986)
#  endif // #if (_MSC_VER == 1600)
# endif // #if !defined(__MINGW32__)
# include <Windows.h>
# include <bitset>
# include <cctype>
# include <climits>
# include <iostream>
# include <limits>
# include <iomanip>
# include <ostream>
# include <set>
# include <sstream>
# include <string>
# include <errno.h>
# include <math.h>
# include <vector>
# if !defined(__MINGW32__)
#  pragma warning(pop)
# endif // #if !defined(__MINGW32__)
#else
# include <bitset>
# include <cctype>
# include <climits>
# include <iostream>
# include <limits>
# include <iomanip>
# include <ostream>
# include <set>
# include <sstream>
# include <string>
# include <errno.h>
# include <math.h>
# include <vector>
# include <netdb.h>
#endif // #ifdef _MSC_VER

// ////////////////////////////////////////////////////////////////////////////

/**
	\namespace MLB

	\brief The Michael L. Brock namespace.
*/
namespace MLB {

/**
	\namespace MLB::Utility

	\brief The Utility namespace.
*/
namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
typedef std::set<std::string>            StringSet;
typedef StringSet::iterator              StringSetIter;
typedef StringSet::const_iterator        StringSetIterC;

typedef std::vector<std::string>         StringVector;
typedef StringVector::iterator           StringVectorIter;
typedef StringVector::const_iterator     StringVectorIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::pair<std::string, std::string> StringPair;

typedef std::vector<StringPair>             StringPairVector;
typedef StringPairVector::iterator          StringPairVectorIter;
typedef StringPairVector::const_iterator    StringPairVectorIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	int BasicCompare(const DataType &datum_1, const DataType &datum_2)
{
	return((datum_1 < datum_2) ? -1 : ((datum_2 < datum_1) ? 1 : 0));
}
// ////////////////////////////////////////////////////////////////////////////

//	Specialization for int because a simple subtraction suffices...
// ////////////////////////////////////////////////////////////////////////////
template <>
	inline int BasicCompare(const int &datum_1, const int &datum_2)
{
	return(datum_1 - datum_2);
}
// ////////////////////////////////////////////////////////////////////////////

//	Specialization for std::string uses the class 'compare()' method...
// ////////////////////////////////////////////////////////////////////////////
template <>
	inline int BasicCompare(const std::string &datum_1,
		const std::string &datum_2)
{
	return(datum_1.compare(datum_2));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType> DataType IncrementToMax(DataType &datum)
{
	DataType old_value = datum;

	datum = (datum < std::numeric_limits<DataType>::max()) ? (datum + 1) : datum;

	return(old_value);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType> DataType DecrementToMin(DataType &datum)
{
	DataType old_value = datum;

	datum = (datum > std::numeric_limits<DataType>::min()) ? (datum - 1) : datum;

	return(old_value);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	unsigned int IntegralValueMaxLengthDecUnsigned()
{
	return((sizeof(DataType) % 2) ? ((((sizeof(DataType) - 1) / 2) * 5) + 3) :
		((sizeof(DataType) / 2) * 5));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	unsigned int IntegralValueMaxLengthDec()
{
	return(IntegralValueMaxLengthDecUnsigned<DataType>() +
		((std::numeric_limits<DataType>::is_signed) ? 1 : 0));
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	std::string AnyToString(const DataType &datum)
{
	std::ostringstream tmp_text;

	tmp_text << datum;

	return(tmp_text.str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <>
	inline std::string AnyToString<bool>(const bool &datum)
{
	return((datum) ? "true" : "false");
}
//	////////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
template <>
	inline std::string AnyToString<unsigned char>(const unsigned char &datum)
{
	return(AnyToString<unsigned long>(static_cast<unsigned long>(datum)));
}
//	//////////////////////////////////////////////////////////////////////////

//	//////////////////////////////////////////////////////////////////////////
template <>
	inline std::string AnyToString<signed char>(const signed char &datum)
{
	return(AnyToString<signed long>(static_cast<signed long>(datum)));
}
//	//////////////////////////////////////////////////////////////////////////

//	Specialization work-around for MSVC 64-bit std::size_t compatibility...
//	CODE NOTE: May not be required under MSVC++ 8.x. To be determined.
#ifdef _Windows
# if (_MSC_VER >= 1300) && (_MSC_VER <= 1310)
//	////////////////////////////////////////////////////////////////////////////
template <>
	inline std::string AnyToString<std::size_t>(const std::size_t &datum)
{
	return(AnyToString<unsigned long long>(
		static_cast<unsigned long long>(datum)));
}
//	////////////////////////////////////////////////////////////////////////////
# endif // # if (_MSC_VER >= 1300) && (_MSC_VER <= 1310)
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////
#ifdef _Windows
typedef DWORD SystemErrorCode;
static const SystemErrorCode SystemErrorCodeNone = ERROR_SUCCESS;
#elif sun
typedef int   SystemErrorCode;
static const SystemErrorCode SystemErrorCodeNone = EZERO;
#else
typedef int   SystemErrorCode;
static const SystemErrorCode SystemErrorCodeNone = 0;
#endif // #ifdef _Windows
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY int             GetLastErrnoCode();
API_UTILITY void            SetLastErrnoCode(int error_code);

API_UTILITY SystemErrorCode GetLastSystemErrorCode();
API_UTILITY void            SetLastSystemErrorCode(SystemErrorCode error_code);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY std::string GetErrnoString(int errno_code = errno,
	bool long_flag = false);
API_UTILITY std::string GetSystemErrorString(
	SystemErrorCode error_code = GetLastSystemErrorCode(),
	bool long_flag = false);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#ifdef _Windows
typedef HANDLE FileHandleNative;
const FileHandleNative FileHandleNativeValueNull = INVALID_HANDLE_VALUE;
#else
typedef int FileHandleNative;
const FileHandleNative FileHandleNativeValueNull = -1;
#endif // #ifdef _Windows
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef unsigned long long MemorySizeType;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef unsigned long long FileOffsetType;
typedef unsigned long long FileSizeType;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
// Macros for use in your classes. Note that the access is changed to
//	to 'private' by these macros.
//	////////////////////////////////////////////////////////////////////////////
#define  MLB_Utility_NonCopyable_MacroCopyCTor(type_name)		\
	private: type_name (const type_name &other)
#define  MLB_Utility_NonCopyable_MacroAssignment(type_name)		\
	private: type_name & operator = (const type_name &other)
#define  MLB_Utility_NonCopyable_Macro(type_name)					\
	 MLB_Utility_NonCopyable_MacroCopyCTor(type_name);				\
	 MLB_Utility_NonCopyable_MacroAssignment(type_name)			\
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/*
	This brash template is intended to manage efficiently the update of STL
	containers of pointers in implementations where the objects to which the
	pointers refer are in managed memory subject to being re-based.

	Consider the case in which we manage a memory-mapped file of Object
	instances and want to provide fast lookup using a std::set<>. We could
	use std::set<Object>, but this would require two updates when a datum
	changed (the first update to the memory-mapped file and the second to
	the std::set<>). It's also not very memory-efficient as each Object
	instance is resident in two places in memory.

	A better solution is to provide indexed access through std::set<Object *>,
	where the Object pointer points to the instance within the memory-mapped
	area.

	The issue this template addresses is where the memory-mapped file needs
	to be re-mapped (for example, to increase the size of the file to
	accomodate more instances). It may not be possible to map the area at
	the same base address.

	One approach to this problem would be to completely re-build the std::set<>
	after each re-mapping. This works, but discards the critical information
	stored in the std::set<> red-black tree. Even after the re-mapping, the
	relative ordering of the instances hasn't changed, only the virtual
	addresses of those instances. The computational cost required to traverse
	the std::set<> of our example and perform the pointer arithmetic required
	to update the pointers is several orders of magnitude lower than the
	computational cost to entirely re-build the red-black tree.

	std::map<> and std::multimap<> are not supported because of the number
	of different cases one would have to cover to provide fully general
	support (std::pair<K *, V>, std::pair<K, V *>, std::pair<K *, V *>).
	If you need this functionality for a std::[multi]map<>, you'll have to
	cut-and-paste this code.

	Nota bene: The reference to the container is 'const' because we don't
	actually change the container data or relative ordering... Just the
	virtual address...
*/
//	////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	void UpdateContainedPointersInPlace(const void *old_base_ptr,
		const void *new_base_ptr, const ContainerType &container)
{
	if (container.empty() || (old_base_ptr == new_base_ptr))
		return;

	typename ContainerType::difference_type ptr_difference =
		static_cast<const char *>(new_base_ptr) -
		static_cast<const char *>(old_base_ptr);

	typename ContainerType::const_iterator iter_b(container.begin());
	typename ContainerType::const_iterator iter_e(container.end());

	while (iter_b != iter_e) {
		char **datum_ptr =
			reinterpret_cast<char **>(
				const_cast<typename ContainerType::value_type *>(&(*iter_b)));
		*datum_ptr += ptr_difference;
		++iter_b;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY char *nstrcat(char *to_string, const char *from_string,
	unsigned int string_length);
API_UTILITY char *nstrcpy(char *to_string, const char *from_string,
	unsigned int string_length);
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef unsigned int ProcessId;
typedef unsigned int ThreadId;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY ProcessId CurrentProcessId();
API_UTILITY ThreadId  CurrentThreadId();
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY unsigned int GetPageSize();
API_UTILITY unsigned int GetPageAllocGranularitySize();
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY void MemLockProcess(bool continue_flag = false);
API_UTILITY void MemUnlockProcess(bool continue_flag = false);
API_UTILITY void MemLockArea(const void *area_ptr, unsigned int area_length);
API_UTILITY void MemUnlockArea(const void *area_ptr, unsigned int area_length);
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// Added 255 to max host names so as to ensure space to handle FQDNs.
#ifdef _Windows
const unsigned int MaxHostNameLength = MAX_COMPUTERNAME_LENGTH + 255;
const unsigned int MaxPathNameLength = _MAX_PATH;
#elif __MSDOS__
const unsigned int MaxHostNameLength = 64;
const unsigned int MaxPathNameLength = 255;
#elif sun
const unsigned int MaxHostNameLength = MAXHOSTNAMELEN + 255;
const unsigned int MaxPathNameLength = MAXPATHLEN;
#else
const unsigned int MaxHostNameLength = HOST_NAME_MAX + 255;
const unsigned int MaxPathNameLength = PATH_MAX;
#endif // #ifdef _Windows
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#ifdef __MSDOS__
const unsigned int UserNameLength = 63;		// Whatever!
#elif _Windows
# include <Lmcons.h>
const unsigned int UserNameLength = UNLEN;
#else
const unsigned int UserNameLength = 127;		// And, one hopes, sufficient.
#endif // #ifdef __MSDOS__
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY std::string  &ReadFileData(const std::string &file_name,
	std::string &file_data);
API_UTILITY std::string   ReadFileData(const std::string &file_name);
API_UTILITY unsigned int  ReadFileLines(const std::string &file_name,
	StringVector &file_lines);
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY std::string  GetCurrentUserName();
API_UTILITY char        *GetCurrentUserName(char *user_name,
	unsigned int user_name_length);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY std::string  GetHostName();
API_UTILITY std::string  GetHostNameCanonical();
API_UTILITY char        *GetHostName(char *host_name,
	unsigned int host_name_length = MaxHostNameLength);
API_UTILITY void         SetHostName(const std::string &host_name);
API_UTILITY void         SetHostName(const char *host_name);
API_UTILITY std::string  ConvertHostNameToCanonical(
	const std::string &host_name);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline API_UTILITY bool IsBigEndianArchitecture()
{
	int test_int = 1;

	return((*reinterpret_cast<char *>(&test_int)) ? false : true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline API_UTILITY bool HostByteOrderEqualNetworkByteOrder()
{
	return(static_cast<unsigned long>(0x12345678) ==
		htonl(static_cast<unsigned long>(0x12345678)));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__Utility_hpp__HH

