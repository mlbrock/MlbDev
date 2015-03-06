// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the UniqueId class.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__UniqueId_hpp__HH

#define HH__MLB__Utility__UniqueId_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file UniqueId.hpp

	\brief	The MLB UniqueId header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/IntToString.hpp>

#include <vector>

#include <boost/version.hpp>

#ifdef MLB_UNIQUEID_FORCE_BOOST_UUID
# if BOOST_VERSION < 104200
#  error "Manifest constant MLB_UNIQUEID_FORCE_BOOST_UUID is defined, but Boost version is less that 1.42!"
# else
#  define MLB_UNIQUE_ID_USES_BOOST_UUID	1
# endif // # if BOOST_VERSION < 104200
#endif // #ifdef MLB_UNIQUEID_FORCE_BOOST_UUID

#ifdef MLB_UNIQUE_ID_USES_BOOST_UUID
# include <boost/uuid/uuid.hpp>
#else
# ifdef _MSC_VER
#  include <Rpc.h>
# elif BOOST_VERSION >= 104200
#  define MLB_UNIQUE_ID_USES_BOOST_UUID	1
#  include <boost/uuid/uuid.hpp>
# else
	//	Else we default to Theodore Ts'o's implmentation.
#  include <uuid/uuid.h>
# endif // #ifndef _MSC_VER
#endif // #ifdef MLB_UNIQUE_ID_USES_BOOST_UUID

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
#ifdef MLB_UNIQUE_ID_USES_BOOST_UUID
typedef boost::uuids::uuid NativeUuidType;
#else
# ifdef _MSC_VER
typedef UUID NativeUuidType;
# else
	//	Else we fallback to Theodore Ts'o's implementation.
union NativeUuidType {
	uuid_t impl_;
};
# endif // #ifdef _MSC_VER
#endif // #ifdef MLB_UNIQUE_ID_USES_BOOST_UUID
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	The length of a unique identifier as a human-readable string (terminating
//	ASCII NUL character not included).
//	////////////////////////////////////////////////////////////////////////////
const unsigned int UniqueIdStringLength = 36;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	The length of a unique identifier as a human-readable string in radix 64
//	(terminating ASCII NUL character not included).
//	////////////////////////////////////////////////////////////////////////////
const unsigned int UniqueIdStringLengthRadix64 =
	(sizeof(NativeUuidType) / sizeof(long)) * IntToString_MaxLen_Unsigned_64;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/**
	\brief A wrapper class for Universally Unique IDentifier (UUID).

	This class wraps the UUID specified in RFC 4122.
*/
struct API_UTILITY UniqueId {
	         UniqueId();
	explicit UniqueId(bool generate_uuid_flag);
	explicit UniqueId(const char *unique_id);
	explicit UniqueId(const std::string &unique_id);

	std::string      ToString() const;
	std::string     &ToString(std::string &out_string) const;
	char            *ToString(char *out_string) const;

	std::string      ToStringByteOrder() const;
	std::string     &ToStringByteOrder(std::string &out_string) const;
	char            *ToStringByteOrder(char *out_string) const;

	std::string      ToStringRadix64() const;
	std::string     &ToStringRadix64(std::string &out_string) const;
	char            *ToStringRadix64(char *out_string) const;
	
	static UniqueId  FromString(const std::string &uuid_string);
	static UniqueId  FromString(const char *uuid_string);

	bool operator  < (const UniqueId &other) const;
	bool operator == (const UniqueId &other) const;
	bool operator != (const UniqueId &other) const;

	/**
		Returns true if all 16 bytes of the UUID are zero.
	*/
	bool IsZero() const;

	void swap(UniqueId &other);

	static NativeUuidType GetSystemUUID();

	NativeUuidType id_;
};
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const UniqueId &datum);
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<UniqueId>          UniqueIdVector;
typedef UniqueIdVector::iterator       UniqueIdVectorIter;
typedef UniqueIdVector::const_iterator UniqueIdVectorIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY void CreateUniqueIdVector(UniqueIdVector &uid_list,
	unsigned int element_count);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	void CreateUniqueIdContainer(ContainerType &uid_container,
	unsigned int element_count)
{
	UniqueIdVector tmp_uid_list;

	CreateUniqueIdVector(tmp_uid_list, element_count);

	ContainerType tmp_uid_container(tmp_uid_list.begin(), tmp_uid_list.end());

	uid_container.swap(tmp_uid_container);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Specialization for a vector of unique ids because the implementation of the
//	template uses a vector to populate other container types...
template <>
	inline API_UTILITY void CreateUniqueIdContainer<UniqueIdVector>(
	UniqueIdVector &uid_container, unsigned int element_count)
{
	CreateUniqueIdVector(uid_container, element_count);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__UniqueId_hpp__HH

