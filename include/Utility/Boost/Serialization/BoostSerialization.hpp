// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for boost serialization support for
								class and structures in the Utility library.

	Revision History	:	2005-02-18 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__Utility_BoostSerialization_hpp__HH

#define HH__MLB__Utility__Utility_BoostSerialization_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/CompiledAlignmentInfo.hpp>
#include <Utility/ExecProcess.hpp>
#include <Utility/GetEnvironment.hpp>
#include <Utility/HostArchitectureInfo.hpp>
#include <Utility/HostVirtualMemoryPageInfo.hpp>
#include <Utility/HRTimeStamp.hpp>
#include <Utility/MACAddress.hpp>
#include <Utility/RsrcUsage.hpp>
#include <Utility/TimeSupport.hpp>
#include <Utility/UniqueId.hpp>
#include <Utility/VersionNumber.hpp>

#ifdef _Windows
# pragma warning(disable: 4061 4100 4267 4511 4512 4625 4626 4640 4668)
#endif // #ifdef _Windows

	//	Needed to handle std::vector<std::string> serialization.
#include <boost/config.hpp>
#define BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP	1

#include <boost/archive/basic_archive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/vector.hpp>

	//	Need this for split save/load methods.
#include <boost/serialization/split_member.hpp>

	// Need this for split save/load free functions.
#include <boost/serialization/split_free.hpp>

	//	Need these for exception definitions.
#include <boost/archive/archive_exception.hpp>
#include <boost/archive/basic_xml_archive.hpp>

#ifdef _Windows
# pragma warning(default: 4061 4100 4267 4511 4512 4625 4626 4640 4668)
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

} // namespace Utility

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	NOTE: For use with free serialization functions.
//	Template for data types with unified save/load serialization...
// ////////////////////////////////////////////////////////////////////////////
#define MLB_BOOSTX_SERIAL_TEMPLATE(data_type)							\
	namespace boost {																\
		namespace serialization {												\
			template <typename SArchive>										\
				void serialize(SArchive &s_archive, data_type &datum,	\
					const unsigned int o_version);							\
		}																				\
	}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	NOTE: For use with free serialization functions.
//	Template for data types with split save/load serialization...
// ////////////////////////////////////////////////////////////////////////////
#define MLB_BOOSTX_SERIAL_TEMPLATE_SPLIT(data_type)						\
	namespace boost {																	\
		namespace serialization {													\
			template <typename SArchive>											\
				void save(SArchive &s_archive, const data_type &datum,	\
					const unsigned int o_version);								\
			template <typename SArchive>											\
				void load(SArchive &s_archive, data_type &datum,			\
					const unsigned int o_version);								\
			template <typename SArchive>											\
				void serialize(SArchive &s_archive, data_type &datum,		\
					const unsigned int o_version);								\
		}																					\
	}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	NOTE: For use with member serialization functions.
//	Template for data types with unified save/load serialization...
// ////////////////////////////////////////////////////////////////////////////
#define MLB_BOOSTX_SERIAL_TEMPLATE_M(data_type)			\
	template <typename SArchive>								\
		void data_type::serialize(SArchive &s_archive,	\
			const unsigned int o_version);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	NOTE: For use with member serialization functions.
//	Template for data types with split save/load serialization...
// ////////////////////////////////////////////////////////////////////////////
#define MLB_BOOSTX_SERIAL_TEMPLATE_SPLIT_M(data_type)	\
	template <typename SArchive>								\
		void data_type::save(SArchive &s_archive,			\
			const unsigned int o_version);					\
	template <typename SArchive>								\
		void data_type::load(SArchive &s_archive,			\
			const unsigned int o_version);					\
	template <typename SArchive>								\
		void data_type::serialize(SArchive &s_archive,	\
			const unsigned int o_version);
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
enum SArchiveType {
	SArchiveType_Text     = 0,
	SArchiveType_TextWide = 1,
	SArchiveType_Binary   = 2,
	SArchiveType_Xml      = 3,
	SArchiveType_XmlWide  = 4,
	SArchiveType_Min      = 0,
	SArchiveType_Max      = 4
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
API_UTILITY bool         IsValidSArchiveType(SArchiveType serial_type);
API_UTILITY bool         IsSupportedSArchiveType(SArchiveType serial_type);

API_UTILITY SArchiveType CheckValidSArchiveType(SArchiveType serial_type);
API_UTILITY SArchiveType CheckIsSupportedSArchiveType(SArchiveType serial_type);

API_UTILITY const char  *SArchiveTypeToStringPtr(SArchiveType serial_type);
API_UTILITY std::string &SArchiveTypeToString(SArchiveType serial_type,
	std::string &out_string);
API_UTILITY std::string  SArchiveTypeToString(SArchiveType serial_type);

API_UTILITY SArchiveType StringToSArchiveType(const char *serial_type);
API_UTILITY SArchiveType StringToSArchiveType(const std::string &serial_type);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	std::string &SerializeSaveBinary(const DataType &in_datum,
		std::string &out_datum, unsigned int flags = 0,
		const char *nvp_tag = NULL);
template <typename DataType>
	std::string &SerializeSaveText(const DataType &in_datum,
		std::string &out_datum, unsigned int flags = 0,
		const char *nvp_tag = NULL);
template <typename DataType>
	std::string &SerializeSaveXml(const DataType &in_datum,
		std::string &out_datum, unsigned int flags = 0,
		const char *nvp_tag = NULL);

template <typename DataType>
	DataType &SerializeLoadBinary(const std::string &in_datum,
		DataType &out_datum, unsigned int flags = 0, const char *nvp_tag = NULL);
template <typename DataType>
	DataType &SerializeLoadText(const std::string &in_datum,
		DataType &out_datum, unsigned int flags = 0, const char *nvp_tag = NULL);
template <typename DataType>
	DataType &SerializeLoadXml(const std::string &in_datum, DataType &out_datum,
		unsigned int flags = 0, const char *nvp_tag = NULL);
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

//	////////////////////////////////////////////////////////////////////////////
BOOST_CLASS_IMPLEMENTATION(MLB::Utility::StringSet,
	boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(MLB::Utility::StringVector,
	boost::serialization::object_serializable)

BOOST_CLASS_IMPLEMENTATION(MLB::Utility::StringPairVector,
	boost::serialization::object_serializable)

BOOST_CLASS_IMPLEMENTATION(MLB::Utility::EnvElementList,
	boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(MLB::Utility::EnvElementSet,
	boost::serialization::object_serializable)
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BOOST_CLASS_IMPLEMENTATION(MLB::Utility::CompiledAlignmentInfo,
	boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(MLB::Utility::EnvElement,
	boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(MLB::Utility::ExecProcessInfo,
	boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(MLB::Utility::HostArchitectureInfo,
	boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(MLB::Utility::HostVirtualMemoryPageInfo,
	boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(MLB::Utility::HRTimeStamp,
	boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(MLB::Utility::RsrcUsage,
	boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(MLB::Utility::TimeSpec,
	boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(MLB::Utility::TimeT,
	boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(MLB::Utility::TimeVal,
	boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(MLB::Utility::UniqueId,
	boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(MLB::Utility::VersionNumber,
	boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(MLB::Utility::WaitProcessInfo,
	boost::serialization::object_serializable)
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MLB_BOOSTX_SERIAL_TEMPLATE(MLB::Utility::CompiledAlignmentInfo)
MLB_BOOSTX_SERIAL_TEMPLATE(MLB::Utility::EnvElement)
MLB_BOOSTX_SERIAL_TEMPLATE(MLB::Utility::ExecProcessInfo)
MLB_BOOSTX_SERIAL_TEMPLATE(MLB::Utility::HostArchitectureInfo)
MLB_BOOSTX_SERIAL_TEMPLATE(MLB::Utility::HostVirtualMemoryPageInfo)
MLB_BOOSTX_SERIAL_TEMPLATE(MLB::Utility::HRTimeStamp)
MLB_BOOSTX_SERIAL_TEMPLATE(MLB::Utility::RsrcUsage)
MLB_BOOSTX_SERIAL_TEMPLATE(MLB::Utility::TimeSpec)
MLB_BOOSTX_SERIAL_TEMPLATE(MLB::Utility::TimeT)
MLB_BOOSTX_SERIAL_TEMPLATE(MLB::Utility::TimeVal)
MLB_BOOSTX_SERIAL_TEMPLATE(MLB::Utility::UniqueId)
MLB_BOOSTX_SERIAL_TEMPLATE(MLB::Utility::VersionNumber)
MLB_BOOSTX_SERIAL_TEMPLATE(MLB::Utility::WaitProcessInfo)
// ////////////////////////////////////////////////////////////////////////////

//	CODE NOTE: TO-DO: Serialization for TimeTM
//	CODE NOTE: TO-DO: Serialization for MACAddress
//	CODE NOTE: TO-DO: FromString() logic for TimeTM

#ifdef TEST_MAIN

#include <Utility/Utility_Exception.hpp>

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
template <typename SaveArchive, typename LoadArchive, typename DataType>
	void TEST_ArchiveType(const DataType &datum, const std::string &data_name,
	const std::string &archive_name, int flags = boost::archive::no_header)
{
	std::cout << data_name << ": " << std::setw(9) << archive_name <<
		" ---> " << std::flush;

	try {
		std::string datum_saved;
		try {
			try {
				std::ostringstream output_stream;
				SaveArchive        s_archive(output_stream, flags);
				s_archive & boost::serialization::make_nvp("MLBTest", datum);
				datum_saved.assign(output_stream.str());
			}
			catch (const std::exception &except) {
				ThrowException("Serialization save attempt failed: " +
					std::string(except.what()));
			}
			DataType datum_loaded;
			try {
				std::istringstream input_stream(datum_saved);
				LoadArchive        l_archive(input_stream, flags);
				l_archive & boost::serialization::make_nvp("MLBTest", datum_loaded);
			}
			catch (const std::exception &except) {
				ThrowException("Serialization load attempt failed: " +
					std::string(except.what()));
			}
			if ((datum < datum_loaded) || (datum_loaded < datum)) {
				std::ostringstream o_str;
				o_str <<
					"Comparison of original data (" << datum << ") with loaded "
					"result of serialization (" << datum_loaded << ") revealed "
					"differences.";
				ThrowException(o_str.str());
			}
		}
		catch (const std::exception &except) {
			Rethrow(except, "Failure in regression test for boost::serialization "
				"of type '" + data_name + "' with serialization archive type '" +
				archive_name + "': " + std::string(except.what()));
		}
		std::cout << "OK (serialized size = " <<
			static_cast<unsigned int>(datum_saved.size()) << ")" << std::endl;
	}
	catch (const std::exception &except) {
		std::cout << "FAILED: " << except.what() << std::endl;
		throw;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	void TEST_Serialization(const DataType &datum, const std::string &data_name)
{
	TEST_ArchiveType<boost::archive::binary_oarchive,
		boost::archive::binary_iarchive, DataType>(datum, data_name, "binary");
	TEST_ArchiveType<boost::archive::text_oarchive,
		boost::archive::text_iarchive,   DataType>(datum, data_name, "text");
	TEST_ArchiveType<boost::archive::xml_oarchive,
		boost::archive::xml_iarchive,    DataType>(datum, data_name, "xml");
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifdef TEST_MAIN

#endif // #ifndef HH__MLB__Utility__Utility_BoostSerialization_hpp__HH

