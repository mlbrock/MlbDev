// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Serialization Template Implementation File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Template implementation file for Boost serialization
								support.

	Revision History	:	2005-02-18 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__Utility_BoostSerialization_ipp__HH

#define HH__MLB__Utility__Utility_BoostSerialization_ipp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Boost/Serialization/BoostSerialization.hpp>
#include <Utility/Utility_Exception.hpp>

#ifdef _Windows
#pragma warning(disable: 4100 4242 4244 4267 4511 4512 4625 4626 4640 4668)
#endif // #ifdef _Windows

#include <boost/archive/polymorphic_text_iarchive.hpp>
#include <boost/archive/polymorphic_text_oarchive.hpp>

#include <boost/archive/polymorphic_binary_iarchive.hpp>
#include <boost/archive/polymorphic_binary_oarchive.hpp>

#include <boost/archive/polymorphic_xml_iarchive.hpp>
#include <boost/archive/polymorphic_xml_oarchive.hpp>

#ifdef _Windows
#pragma warning(default: 4100 4242 4244 4267 4511 4512 4625 4626 4640 4668)
#endif // #ifdef _Windows

#ifdef _Windows
# pragma warning(disable:4217 4668 4625 4626 4275)
#endif // #ifdef _Windows

#include <boost/thread/mutex.hpp>

#ifdef _Windows
# pragma warning(default:4217 4668 4625 4626 4275)
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Only the XML serialization needs locking.
//	CODE NOTE: May be peculiar to MSVC++ and/or Windows. To be determined.
// ////////////////////////////////////////////////////////////////////////////
API_UTILITY boost::mutex &GetLockForSerializeSaveXml();
API_UTILITY boost::mutex &GetLockForSerializeLoadXml();
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	std::string &SerializeSaveBinary(const DataType &in_datum,
		std::string &out_datum, unsigned int flags, const char *nvp_tag)
{
	std::ostringstream output_stream;

	try {
		boost::archive::binary_oarchive s_archive(output_stream, flags);
		s_archive & boost::serialization::make_nvp(
			((nvp_tag != NULL) && *nvp_tag) ? nvp_tag : "polaris", in_datum);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Error in serialization attempt: " +
			std::string(except.what()));
	}

	return(out_datum.assign(output_stream.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	std::string &SerializeSaveText(const DataType &in_datum,
		std::string &out_datum, unsigned int flags, const char *nvp_tag)
{
	std::ostringstream output_stream;

	try {
		boost::archive::text_oarchive   s_archive(output_stream, flags);
		s_archive & boost::serialization::make_nvp(
			((nvp_tag != NULL) && *nvp_tag) ? nvp_tag : "polaris", in_datum);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Error in serialization attempt: " +
			std::string(except.what()));
	}

	return(out_datum.assign(output_stream.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	std::string &SerializeSaveXml(const DataType &in_datum,
		std::string &out_datum, unsigned int flags, const char *nvp_tag)
{
	std::ostringstream output_stream;

	try {
		//	Need locking for XML save serialization...
		boost::mutex::scoped_lock       my_lock(GetLockForSerializeSaveXml());
		boost::archive::xml_oarchive    s_archive(output_stream, flags);
		s_archive & boost::serialization::make_nvp(
			((nvp_tag != NULL) && *nvp_tag) ? nvp_tag : "polaris", in_datum);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Error in serialization attempt: " +
			std::string(except.what()));
	}

	return(out_datum.assign(output_stream.str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	DataType &SerializeLoadBinary(const std::string &in_datum,
		DataType &out_datum, unsigned int flags, const char *nvp_tag)
{
	std::istringstream input_stream(in_datum);

	try {
		boost::archive::binary_iarchive s_archive(input_stream, flags);
		nvp_tag = (nvp_tag != NULL) ? nvp_tag : "";
		s_archive & boost::serialization::make_nvp(
			((nvp_tag != NULL) && *nvp_tag) ? nvp_tag : "polaris", out_datum);
	}
	catch (const std::exception &except) {
		Rethrow(except, "Error in de-serialization attempt: " +
			std::string(except.what()));
	}

	return(out_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	DataType &SerializeLoadText(const std::string &in_datum,
		DataType &out_datum, unsigned int flags, const char *nvp_tag)
{
	std::istringstream input_stream(in_datum);

	try {
		boost::archive::text_iarchive s_archive(input_stream, flags);
		nvp_tag = (nvp_tag != NULL) ? nvp_tag : "";
		s_archive & boost::serialization::make_nvp(
			((nvp_tag != NULL) && *nvp_tag) ? nvp_tag : "polaris", out_datum);
	}
	catch (const std::exception &except) {
		Rethrow(except, "Error in de-serialization attempt: " +
			std::string(except.what()));
	}

	return(out_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename DataType>
	DataType &SerializeLoadXml(const std::string &in_datum, DataType &out_datum,
		unsigned int flags, const char *nvp_tag)
{
	std::istringstream input_stream(in_datum);

	try {
		//	Need locking for XML load serialization...
		boost::mutex::scoped_lock       my_lock(GetLockForSerializeLoadXml());
		boost::archive::xml_iarchive    s_archive(input_stream, flags);
		nvp_tag = (nvp_tag != NULL) ? nvp_tag : "";
		s_archive & boost::serialization::make_nvp(
			((nvp_tag != NULL) && *nvp_tag) ? nvp_tag : "polaris", out_datum);
	}
	catch (const std::exception &except) {
		Rethrow(except, "Error in de-serialization attempt: " +
			std::string(except.what()));
	}

	return(out_datum);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	NOTE: For use with free serialization functions.
//	Supporting macros not used directly by API users...
// ////////////////////////////////////////////////////////////////////////////
#define MLB_BOOSTX_SERIAL_DECL_X(func_name, archive_name, data_type)			\
	template MB_LIB_THIS_SIDE																\
		void func_name<archive_name>(archive_name &s_archive,						\
			data_type &datum, const unsigned int o_version);
#define MLB_BOOSTX_SERIAL_DECL_X_const(func_name, archive_name, data_type)	\
	template MB_LIB_THIS_SIDE																\
		void func_name<archive_name>(archive_name &s_archive,						\
			const data_type &datum, const unsigned int o_version);

#define MLB_BOOSTX_SERIAL_DECL_SaveX(func_name, archive_name, data_type)	\
	MLB_BOOSTX_SERIAL_DECL_X_const(func_name, archive_name, data_type)
#define MLB_BOOSTX_SERIAL_DECL_LoadX(func_name, archive_name, data_type)	\
	MLB_BOOSTX_SERIAL_DECL_X(func_name, archive_name, data_type)

#define MLB_BOOSTX_SERIAL_DECL_SerializeX(iarchive, oarchive, data_type)	\
	MLB_BOOSTX_SERIAL_DECL_X(boost::serialization::serialize,					\
		oarchive, data_type)																	\
	MLB_BOOSTX_SERIAL_DECL_X(boost::serialization::serialize,					\
		iarchive, data_type)

#define MLB_BOOSTX_SERIAL_DECL_SplitX(iarchive, oarchive, data_type)			\
	MLB_BOOSTX_SERIAL_DECL_SaveX(boost::serialization::save,						\
		oarchive, data_type)																	\
	MLB_BOOSTX_SERIAL_DECL_LoadX(boost::serialization::load,						\
		iarchive, data_type)																	\
	MLB_BOOSTX_SERIAL_DECL_SerializeX(iarchive, oarchive, data_type)
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	NOTE: For use with free serialization functions.
//	Use these macros for data types with unified save/load serialization...
// ////////////////////////////////////////////////////////////////////////////
#define MLB_BOOSTX_SERIAL_DECL_Serialize_text(data_type)							\
	MLB_BOOSTX_SERIAL_DECL_SerializeX(boost::archive::text_iarchive,			\
		boost::archive::text_oarchive, data_type)
#define MLB_BOOSTX_SERIAL_DECL_Serialize_binary(data_type)						\
	MLB_BOOSTX_SERIAL_DECL_SerializeX(boost::archive::binary_iarchive,		\
		boost::archive::binary_oarchive, data_type)
#define MLB_BOOSTX_SERIAL_DECL_Serialize_xml(data_type)							\
	MLB_BOOSTX_SERIAL_DECL_SerializeX(boost::archive::xml_iarchive,			\
		boost::archive::xml_oarchive, data_type)

	//	Macro to force instantiation for all supported archive types...
#define MLB_BOOSTX_SERIAL_DECL_Serialize(data_type)								\
	MLB_BOOSTX_SERIAL_DECL_Serialize_text(data_type)								\
	MLB_BOOSTX_SERIAL_DECL_Serialize_binary(data_type)								\
	MLB_BOOSTX_SERIAL_DECL_Serialize_xml(data_type)
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	NOTE: For use with free serialization functions.
//	Use these macros for data types with split save/load serialization...
// ////////////////////////////////////////////////////////////////////////////
#define MLB_BOOSTX_SERIAL_DECL_Split_text(data_type)							\
	MLB_BOOSTX_SERIAL_DECL_SplitX(boost::archive::text_iarchive,			\
		boost::archive::text_oarchive, data_type)
#define MLB_BOOSTX_SERIAL_DECL_Split_binary(data_type)						\
	MLB_BOOSTX_SERIAL_DECL_SplitX(boost::archive::binary_iarchive,			\
		boost::archive::binary_oarchive, data_type)
#define MLB_BOOSTX_SERIAL_DECL_Split_xml(data_type)							\
	MLB_BOOSTX_SERIAL_DECL_SplitX(boost::archive::xml_iarchive,				\
		boost::archive::xml_oarchive, data_type)

	//	Macro to force instantiation for all supported archive types...
#define MLB_BOOSTX_SERIAL_DECL_Split(data_type)									\
	MLB_BOOSTX_SERIAL_DECL_Split_text(data_type)									\
	MLB_BOOSTX_SERIAL_DECL_Split_binary(data_type)								\
	MLB_BOOSTX_SERIAL_DECL_Split_xml(data_type)
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	NOTE: For use with member serialization functions.
//	Supporting macros not used directly by API users...
// ////////////////////////////////////////////////////////////////////////////
#define MLB_BOOSTX_SERIAL_DECL_M_X(func_name, archive_name, data_type)			\
	template																							\
		void data_type::func_name<archive_name>(archive_name &s_archive,			\
			const unsigned int o_version);
#define MLB_BOOSTX_SERIAL_DECL_M_X_const(func_name, archive_name, data_type)	\
	template																							\
		void data_type::func_name<archive_name>(archive_name &s_archive,			\
			const unsigned int o_version);

#define MLB_BOOSTX_SERIAL_DECL_M_SaveX(func_name, archive_name, data_type)		\
	MLB_BOOSTX_SERIAL_DECL_M_X_const(func_name, archive_name, data_type)
#define MLB_BOOSTX_SERIAL_DECL_M_LoadX(func_name, archive_name, data_type)		\
	MLB_BOOSTX_SERIAL_DECL_M_X(func_name, archive_name, data_type)

#define MLB_BOOSTX_SERIAL_DECL_M_SerializeX(iarchive, oarchive, data_type)		\
	MLB_BOOSTX_SERIAL_DECL_M_X(serialize, oarchive, data_type)						\
	MLB_BOOSTX_SERIAL_DECL_M_X(serialize, iarchive, data_type)

#define MLB_BOOSTX_SERIAL_DECL_M_SplitX(iarchive, oarchive, data_type)			\
	MLB_BOOSTX_SERIAL_DECL_M_SaveX(save, oarchive, data_type)						\
	MLB_BOOSTX_SERIAL_DECL_M_LoadX(load, iarchive, data_type)						\
	MLB_BOOSTX_SERIAL_DECL_M_SerializeX(iarchive, oarchive, data_type)
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	NOTE: For use with member serialization functions.
//	Use these macros for data types with unified save/load serialization...
// ////////////////////////////////////////////////////////////////////////////
#define MLB_BOOSTX_SERIAL_DECL_M_Serialize_text(data_type)							\
	MLB_BOOSTX_SERIAL_DECL_M_SerializeX(boost::archive::text_iarchive,			\
		boost::archive::text_oarchive, data_type)
#define MLB_BOOSTX_SERIAL_DECL_M_Serialize_binary(data_type)						\
	MLB_BOOSTX_SERIAL_DECL_M_SerializeX(boost::archive::binary_iarchive,			\
		boost::archive::binary_oarchive, data_type)
#define MLB_BOOSTX_SERIAL_DECL_M_Serialize_xml(data_type)							\
	MLB_BOOSTX_SERIAL_DECL_M_SerializeX(boost::archive::xml_iarchive,				\
		boost::archive::xml_oarchive, data_type)

	//	Macro to force instantiation for all supported archive types...
#define MLB_BOOSTX_SERIAL_DECL_M_Serialize(data_type)									\
	MLB_BOOSTX_SERIAL_DECL_M_Serialize_text(data_type)									\
	MLB_BOOSTX_SERIAL_DECL_M_Serialize_binary(data_type)								\
	MLB_BOOSTX_SERIAL_DECL_M_Serialize_xml(data_type)
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	NOTE: For use with member serialization functions.
//	Use these macros for data types with split save/load serialization...
// ////////////////////////////////////////////////////////////////////////////
#define MLB_BOOSTX_SERIAL_DECL_M_Split_text(data_type)							\
	MLB_BOOSTX_SERIAL_DECL_M_SplitX(boost::archive::text_iarchive,				\
		boost::archive::text_oarchive, data_type)
#define MLB_BOOSTX_SERIAL_DECL_M_Split_binary(data_type)							\
	MLB_BOOSTX_SERIAL_DECL_M_SplitX(boost::archive::binary_iarchive,			\
		boost::archive::binary_oarchive, data_type)
#define MLB_BOOSTX_SERIAL_DECL_M_Split_xml(data_type)								\
	MLB_BOOSTX_SERIAL_DECL_M_SplitX(boost::archive::xml_iarchive,				\
		boost::archive::xml_oarchive, data_type)

	//	Macro to force instantiation for all supported archive types...
#define MLB_BOOSTX_SERIAL_DECL_M_Split(data_type)									\
	MLB_BOOSTX_SERIAL_DECL_M_Split_text(data_type)									\
	MLB_BOOSTX_SERIAL_DECL_M_Split_binary(data_type)								\
	MLB_BOOSTX_SERIAL_DECL_M_Split_xml(data_type)
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	The macro MLB_BOOSTX_SerializeLoadSave forces instantion of the following
//	functions:
//		SerializeSaveBinary()
//		SerializeSaveText()
//		SerializeSaveXml()
//		SerializeLoadBinary()
//		SerializeLoadText()
//		SerializeLoadXml()
// ////////////////////////////////////////////////////////////////////////////
#define MLB_BOOSTX_SerializeSaveFunc(data_type, func_name)					\
	template MB_LIB_THIS_SIDE std::string &										\
		MLB::Utility::func_name<data_type >(const data_type &in_datum,		\
		std::string &out_datum, unsigned int flags, const char *nvp_tag);
#define MLB_BOOSTX_SerializeLoadFunc(data_type, func_name)					\
	template MB_LIB_THIS_SIDE data_type &											\
		MLB::Utility::func_name<data_type >(const std::string &in_datum,	\
		data_type &out_datum, unsigned int flags, const char *nvp_tag);

#define MLB_BOOSTX_SerializeSave(data_type)										\
	MLB_BOOSTX_SerializeSaveFunc(data_type, SerializeSaveBinary)			\
	MLB_BOOSTX_SerializeSaveFunc(data_type, SerializeSaveText)				\
	MLB_BOOSTX_SerializeSaveFunc(data_type, SerializeSaveXml)
#define MLB_BOOSTX_SerializeLoad(data_type)										\
	MLB_BOOSTX_SerializeLoadFunc(data_type, SerializeLoadBinary)			\
	MLB_BOOSTX_SerializeLoadFunc(data_type, SerializeLoadText)				\
	MLB_BOOSTX_SerializeLoadFunc(data_type, SerializeLoadXml)

#define MLB_BOOSTX_SerializeLoadSaveBinary(data_type)							\
	MLB_BOOSTX_SerializeSaveFunc(data_type, SerializeSaveBinary)			\
	MLB_BOOSTX_SerializeLoadFunc(data_type, SerializeLoadBinary)
#define MLB_BOOSTX_SerializeLoadSaveText(data_type)							\
	MLB_BOOSTX_SerializeSaveFunc(data_type, SerializeSaveText)				\
	MLB_BOOSTX_SerializeLoadFunc(data_type, SerializeLoadText)
#define MLB_BOOSTX_SerializeLoadSaveXml(data_type)								\
	MLB_BOOSTX_SerializeSaveFunc(data_type, SerializeSaveXml)				\
	MLB_BOOSTX_SerializeLoadFunc(data_type, SerializeLoadXml)

	//	Forces instantiation of the Serialize{Load|Save}{Binary|Text|Xml} funcs.
#define MLB_BOOSTX_SerializeLoadSave(data_type)									\
	MLB_BOOSTX_SerializeSave(data_type)												\
	MLB_BOOSTX_SerializeLoad(data_type)
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifndef HH__MLB__Utility__Utility_BoostSerialization_ipp__HH

