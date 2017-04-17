//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of Boost serialization support.

	Revision History	:	2006-12-22 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2006 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/HRTimeStamp.hpp>
#include <Utility/Boost/Serialization/BoostSerialization.ipp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
bool IsValidSArchiveType(SArchiveType serial_type)
{
	return(((serial_type >= SArchiveType_Min) &&
		(serial_type <= SArchiveType_Max)) ? true : false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool IsSupportedSArchiveType(SArchiveType serial_type)
{
	return((serial_type == SArchiveType_Binary) ||
		(serial_type == SArchiveType_Text) || (serial_type == SArchiveType_Xml));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SArchiveType CheckValidSArchiveType(SArchiveType serial_type)
{
	if (!IsValidSArchiveType(serial_type))
		ThrowInvalidArgument("Serialization archive type '" +
			AnyToString(serial_type) + "' is invalid.");

	return(serial_type);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SArchiveType CheckIsSupportedSArchiveType(SArchiveType serial_type)
{
	CheckValidSArchiveType(serial_type);

	if (!IsSupportedSArchiveType(serial_type))
		ThrowInvalidArgument("Serialization archive type '" +
			AnyToString(serial_type) + "' is valid, but not at present "
			"supported.");

	return(serial_type);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *SArchiveTypeToStringPtr(SArchiveType serial_type)
{
	if (serial_type == SArchiveType_Binary)
		return("binary");
	else if (serial_type == SArchiveType_Text)
		return("text");
	else if (serial_type != SArchiveType_Xml)
		ThrowInvalidArgument("Serialization archive type '" +
			AnyToString(serial_type) + "' is invalid.");

	return("xml");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &SArchiveTypeToString(SArchiveType serial_type,
	std::string &out_string)
{
	return(out_string.assign(SArchiveTypeToStringPtr(serial_type)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string SArchiveTypeToString(SArchiveType serial_type)
{
	std::string out_string;

	return(SArchiveTypeToString(serial_type, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SArchiveType StringToSArchiveType(const char *serial_type)
{
	if (serial_type == NULL)
		ThrowInvalidArgument("Serialization archive type string is 'NULL'.");
	else if ((!stricmp(serial_type, "binary")) ||
		(!stricmp(serial_type, "bin")))
		return(SArchiveType_Binary);
	else if ((!stricmp(serial_type, "text")) ||
		(!stricmp(serial_type, "txt")))
		return(SArchiveType_Text);
	else if (stricmp(serial_type, "xml"))
		ThrowInvalidArgument("Serialization archive type string '" +
			AnyToString(serial_type) + "' is invalid.");

	return(SArchiveType_Xml);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SArchiveType StringToSArchiveType(const std::string &serial_type)
{
	return(StringToSArchiveType(serial_type.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Only the XML serialization needs locking.
//	CODE NOTE: May be peculiar to MSVC++ and/or Windows. To be determined.
// ////////////////////////////////////////////////////////////////////////////
namespace {
boost::mutex SArchiveLock_Xml_Save;
boost::mutex SArchiveLock_Xml_Load;
}	//	Anonymous namespace
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
boost::mutex &GetLockForSerializeSaveXml()
{
	return(SArchiveLock_Xml_Save);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
boost::mutex &GetLockForSerializeLoadXml()
{
	return(SArchiveLock_Xml_Load);
}
// ////////////////////////////////////////////////////////////////////////////

#ifdef TEST_MAIN

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

#endif // #ifdef TEST_MAIN

} // namespace Utility

} // namespace MLB

