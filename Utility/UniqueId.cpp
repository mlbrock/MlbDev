//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the UniqueId class (and a few of the
								related free functions in the Utility library).

	Revision History	:	1993-10-02 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/UniqueId.hpp>
#include <Utility/Utility_Exception.hpp>

#include <iomanip>

#ifdef __GNUC__
# include <string.h>
#endif // #ifdef __GNUC__

#ifdef MLB_UNIQUE_ID_USES_BOOST_UUID
#	if defined(_Windows) && !defined(__MINGW32__)
#	 pragma warning(push)
# 	pragma warning(disable:4365 4640)
# endif // #if defined(_Windows) && !defined(__MINGW32__)

# include <boost/random.hpp>
# include <boost/uuid/uuid_generators.hpp>
# include <boost/uuid/uuid_io.hpp>

# if defined(_Windows) && !defined(__MINGW32__)
#  pragma warning(pop)
# endif // #if defined(_Windows) && !defined(__MINGW32__)
#endif // #ifdef MLB_UNIQUE_ID_USES_BOOST_UUID

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
UniqueId::UniqueId()
	:id_(GetSystemUUID())
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
UniqueId::UniqueId(bool generate_uuid_flag)
{
	if (generate_uuid_flag)
		id_ = GetSystemUUID();
	else
		memset(&id_, '\0', sizeof(id_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
UniqueId::UniqueId(const char *unique_id)
	:id_(FromString(unique_id).id_)
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
UniqueId::UniqueId(const std::string &unique_id)
	:id_(FromString(unique_id).id_)
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string UniqueId::ToString() const
{
	std::string out_string;

	return(ToString(out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &UniqueId::ToString(std::string &out_string) const
{
	char buffer[UniqueIdStringLength + 1];

	return(out_string.assign(ToString(buffer)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *UniqueId::ToString(char *out_string) const
{
#ifdef MLB_UNIQUE_ID_USES_BOOST_UUID
	std::ostringstream uuid_string;

	uuid_string << id_;

	return(::strcpy(out_string, uuid_string.str().c_str()));
#else
# ifdef _MSC_VER
	std::ostringstream uuid_string;

	uuid_string << std::hex << std::setfill('0') <<
		std::setw(8) << id_.Data1 << "-" <<
		std::setw(4) << id_.Data2 << "-" <<
		std::setw(4) << id_.Data3 << "-" <<
		std::setw(2) << static_cast<unsigned int>(id_.Data4[0]) <<
		std::setw(2) << static_cast<unsigned int>(id_.Data4[1]) << "-" <<
		std::setw(2) << static_cast<unsigned int>(id_.Data4[2]) <<
		std::setw(2) << static_cast<unsigned int>(id_.Data4[3]) <<
		std::setw(2) << static_cast<unsigned int>(id_.Data4[4]) <<
		std::setw(2) << static_cast<unsigned int>(id_.Data4[5]) <<
		std::setw(2) << static_cast<unsigned int>(id_.Data4[6]) <<
		std::setw(2) << static_cast<unsigned int>(id_.Data4[7]) <<
		std::setfill(' ') << std::dec;

	return(::strcpy(out_string, uuid_string.str().c_str()));
# else
	::uuid_unparse(id_.impl_, out_string);

	return(out_string);
# endif // #ifdef _MSC_VER
#endif // #ifdef MLB_UNIQUE_ID_USES_BOOST_UUID
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string UniqueId::ToStringByteOrder() const
{
	std::string out_string;

	return(ToStringByteOrder(out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &UniqueId::ToStringByteOrder(std::string &out_string) const
{
	char buffer[UniqueIdStringLength + 1];

	return(out_string.assign(ToStringByteOrder(buffer)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *UniqueId::ToStringByteOrder(char *out_string) const
{
	const unsigned char *tmp_ptr_i =
		reinterpret_cast<const unsigned char *>(&id_);
	char                *tmp_ptr_o = out_string;
	unsigned int         count_1;

	for (count_1 = 0; count_1 < UniqueIdStringLength; ++count_1) {
		if ((count_1 == 8) || (count_1 == 13) || (count_1 == 18) ||
			(count_1 == 23))
			*tmp_ptr_o++ = '-';
		else {
			*tmp_ptr_o++ = "0123456789abcdef"[(*tmp_ptr_i >> 4) & 0x0F];
			*tmp_ptr_o++ = "0123456789abcdef"[*tmp_ptr_i        & 0x0F];
			++tmp_ptr_i;
			++count_1;
		}
	}

	*tmp_ptr_o = '\0';

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string UniqueId::ToStringRadix64() const
{
	std::string out_string;

	return(ToStringRadix64(out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &UniqueId::ToStringRadix64(std::string &out_string) const
{
	char buffer[UniqueIdStringLengthRadix64 + 1];

	return(out_string.assign(ToStringRadix64(buffer)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *UniqueId::ToStringRadix64(char *out_string) const
{
	NativeUuidType tmp_id = id_;

#ifndef MLB_UNIQUE_ID_USES_BOOST_UUID
# if _MSC_VER
	tmp_id.Data1 = htonl(id_.Data1);
	tmp_id.Data2 = htons(id_.Data2);
	tmp_id.Data3 = htons(id_.Data3);
# endif // #if _MSC_VER
#endif // #ifndef MLB_UNIQUE_ID_USES_BOOST_UUID

	unsigned int  count_1;
	char         *out_ptr = out_string;

	for (count_1 = 0; count_1 < sizeof(tmp_id);
		count_1 += sizeof(long), out_ptr += IntToString_MaxLen_Unsigned_64)
		IntToString(out_ptr,
			*reinterpret_cast<unsigned long *>(
			reinterpret_cast<char *>(&tmp_id) + count_1), 64, "0");

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
UniqueId UniqueId::FromString(const std::string &uuid_string)
{
	return(FromString(uuid_string.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
UniqueId UniqueId::FromString(const char *uuid_string)
{
	if ((uuid_string == NULL) || (!(*uuid_string)))
		ThrowInvalidArgument("UUID string is empty.");

	if ((strlen(uuid_string) != UniqueIdStringLength) ||
		(uuid_string[8] != '-') || (uuid_string[13] != '-') ||
		(uuid_string[18] != '-') || (uuid_string[23] != '-')) {
		std::ostringstream error_text;
		error_text << "UUID string '" << uuid_string <<
			"' has an invalid format.";
		ThrowInvalidArgument(error_text);
	}

	unsigned int count_1;
	for (count_1 = 0; count_1 < UniqueIdStringLength; ++count_1) {
		if ((count_1 != 8) && (count_1 != 13) && (count_1 != 18) &&
			(count_1 != 23)) {
			if (!isxdigit(*uuid_string)) {
				std::ostringstream error_text;
				error_text << "UUID string '" << uuid_string <<
					"' has an invalid format.";
				ThrowInvalidArgument(error_text);
			}
		}
	}

	UniqueId tmp_uuid(false);

#ifdef MLB_UNIQUE_ID_USES_BOOST_UUID
	std::stringstream io_str;

	io_str << uuid_string;
	io_str >> tmp_uuid.id_;
#else
# if _MSC_VER
	tmp_uuid.id_.Data1 = strtoul(uuid_string, NULL, 16);
	tmp_uuid.id_.Data2 =
			static_cast<unsigned short>(strtoul(uuid_string + 9, NULL, 16));
	tmp_uuid.id_.Data3 =
		static_cast<unsigned short>(strtoul(uuid_string + 14, NULL, 16));

	const char    *tmp_ptr_i = uuid_string + 19;
	unsigned char *tmp_ptr_o = tmp_uuid.id_.Data4;

	while (*tmp_ptr_i) {
		if (isxdigit(*tmp_ptr_i)) {
			*tmp_ptr_o  = static_cast<unsigned char>(
				((((isdigit(tmp_ptr_i[0])) ? (tmp_ptr_i[0] - '0') :
				(std::toupper(tmp_ptr_i[0]) - ('A' - 10))) & 0x0F) << 4) |
				(((isdigit(tmp_ptr_i[1])) ? (tmp_ptr_i[1] - '0') :
				(std::toupper(tmp_ptr_i[1]) - ('A' - 10))) & 0x0F));
			tmp_ptr_i  += 2;
			++tmp_ptr_o;
		}
		else
			++tmp_ptr_i;
	}
# else
	if (::uuid_parse(uuid_string, tmp_uuid.id_.impl_) != 0)
		ThrowErrno("Invocation of '::uuid_parse()' with the valid-seeming "
			"universally unique identifier '" + std::string(uuid_string) +
			"' failed.");
# endif // #if _MSC_VER
#endif // #ifdef MLB_UNIQUE_ID_USES_BOOST_UUID

	return(tmp_uuid);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool UniqueId::operator < (const UniqueId &other) const
{
	return(memcmp(&id_, &other.id_, sizeof(id_)) < 0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool UniqueId::operator == (const UniqueId &other) const
{
	return(memcmp(&id_, &other.id_, sizeof(id_)) == 0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool UniqueId::operator != (const UniqueId &other) const
{
	return(memcmp(&id_, &other.id_, sizeof(id_)) != 0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool UniqueId::IsZero() const
{
	const char   *tmp_ptr = reinterpret_cast<const char *>(this);
	unsigned int  count_1 = sizeof(*this);

	while (count_1--) {
		if (*tmp_ptr++)
			return(false);
	}

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void UniqueId::swap(UniqueId &other)
{
	NativeUuidType tmp_uuid(id_);

	id_       = other.id_;
	other.id_ = tmp_uuid;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
NativeUuidType UniqueId::GetSystemUUID()
{
	NativeUuidType uuid_data;

#ifdef MLB_UNIQUE_ID_USES_BOOST_UUID
	uuid_data = boost::uuids::random_generator()();
#else
# ifdef _MSC_VER
	RPC_STATUS return_code = UuidCreate(&uuid_data);

	if ((return_code != RPC_S_OK) && (return_code != RPC_S_UUID_LOCAL_ONLY) &&
		(return_code != RPC_S_UUID_NO_ADDRESS))
		ThrowErrorCode(return_code,
			"Invocation of UuidCreate() resulted in an error");
# else
	::uuid_generate(uuid_data.impl_);
# endif // #ifdef _MSC_VER
#endif // #ifdef MLB_UNIQUE_ID_USES_BOOST_UUID

	return(uuid_data);
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CreateUniqueIdVector(UniqueIdVector &uid_list, unsigned int element_count)
{
	UniqueIdVector tmp_uid_list(element_count);

	while (element_count)
		tmp_uid_list[--element_count] = UniqueId();

	uid_list.swap(tmp_uid_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const UniqueId &datum)
{
	char buffer[UniqueIdStringLength + 1];

	o_str <<
		datum.ToString(buffer);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::Utility;

# if BOOST_VERSION > 104200

# ifndef MLB_UNIQUE_ID_USES_BOOST_UUID
#   if defined(_Windows) && !defined(__MINGW32__)
#	  pragma warning(push)
#  	pragma warning(disable:4365 4640)
#  endif // #if defined(_Windows) && !defined(__MINGW32__)

#  include <boost/random.hpp>
#  include <boost/uuid/uuid_generators.hpp>
#  include <boost/uuid/uuid_io.hpp>

#  if defined(_Windows) && !defined(__MINGW32__)
#   pragma warning(pop)
#  endif // #if defined(_Windows) && !defined(__MINGW32__)
# endif // #ifndef MLB_UNIQUE_ID_USES_BOOST_UUID

//	////////////////////////////////////////////////////////////////////////////
void TEST_BoostUuid()
{
	try {
		boost::uuids::uuid uuid_empty = boost::uuids::nil_uuid();
		boost::uuids::uuid uuid_datum_1 = boost::uuids::random_generator()();
		std::cout << "UUID Empty     : " << uuid_empty   << std::endl;
		std::cout << "UUID Datum 1   : " << uuid_datum_1 << std::endl;
		std::string uuid_datum_1_string;
		std::stringstream o_str;
		o_str << uuid_datum_1;
		uuid_datum_1_string = o_str.str();
		boost::uuids::uuid uuid_datum_2;
		o_str >> uuid_datum_2;
		std::cout << "UUID Datum 2   : " << uuid_datum_2 << std::endl;
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "The test of the boost::uuid library "
			"failed: " + std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

# endif // #if BOOST_VERSION > 104200

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	std::cout << "Test routine for class 'UniqueId'" << std::endl;
	std::cout << "---- ------- --- ----- ----------" << std::endl;

	try {
# if BOOST_VERSION > 104200
		TEST_BoostUuid();
# endif // #if BOOST_VERSION > 104200
		UniqueId    request_id;
		UniqueId    request_id_tmp(false);
		std::cout << "UUID Mine      : " <<
			request_id << std::endl;
		std::cout << "UUID Converted : " <<
			UniqueId::FromString(request_id.ToString()) << std::endl;
#if defined(_MSC_VER) && (!defined(MLB_UNIQUE_ID_USES_BOOST_UUID))
		unsigned char *uuid_string;
		UuidToString(&request_id.id_, &uuid_string);
		std::cout << "UUID Win32     : " <<
			uuid_string << std::endl;
#endif // #if defined(_MSC_VER) && (!defined(MLB_UNIQUE_ID_USES_BOOST_UUID))
		std::cout << "UUID Byte Order: " <<
			request_id.ToStringByteOrder() << std::endl;
		std::cout << "UUID Radix 64  : " <<
			request_id.ToStringRadix64() << std::endl;
		std::cout << "---------------- --------------------------------------" <<
			std::endl << std::endl;
#if defined(_MSC_VER) && (!defined(MLB_UNIQUE_ID_USES_BOOST_UUID))
		if (stricmp(request_id.ToString().c_str(),
			reinterpret_cast<char *>(uuid_string))) {
			std::cout << std::endl << std::endl << "ERROR: " <<
				"MLB UUID string '" << request_id <<
				"' differs from Win32 UUID string '" << uuid_string << "'." <<
				std::endl << std::endl;
			return_code = EXIT_FAILURE;
		}
		RpcStringFree(&uuid_string);
#endif // #if defined(_MSC_VER) && (!defined(MLB_UNIQUE_ID_USES_BOOST_UUID))
	}
	catch (const std::exception &except) {
		std::cout << std::endl << std::endl;
		std::cout << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

