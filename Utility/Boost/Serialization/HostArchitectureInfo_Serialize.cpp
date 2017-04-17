//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of Boost serialization for the class
								HostArchitectureInfo.

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

#include <Utility/HostArchitectureInfo.hpp>
#include <Utility/Boost/Serialization/BoostSerialization.ipp>

//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Use macros to get declarations and forced instantiations done...
// ////////////////////////////////////////////////////////////////////////////

	//	Declare the template functions...
MLB_BOOSTX_SERIAL_TEMPLATE(MLB::Utility::HostArchitectureInfo)

	//	Force instantiation of the serialize free function...
MLB_BOOSTX_SERIAL_DECL_Serialize(MLB::Utility::HostArchitectureInfo)

	// Force instantiation of the Serialize{Load|Save}{Binary|Text|Xml} funcs.
MLB_BOOSTX_SerializeLoadSave(MLB::Utility::HostArchitectureInfo)

// ////////////////////////////////////////////////////////////////////////////

namespace boost {

namespace serialization {

// ////////////////////////////////////////////////////////////////////////////
template <typename SArchive>
	void serialize(SArchive &s_archive, MLB::Utility::HostArchitectureInfo &datum,
		const unsigned int)
{
	s_archive
		& boost::serialization::make_nvp("charBits", datum.char_bits_)
		& boost::serialization::make_nvp("sizeOfUnsignedChar", datum.sizeof_u_char_)
		& boost::serialization::make_nvp("sizeOfUnsignedShort", datum.sizeof_u_short_)
		& boost::serialization::make_nvp("sizeOfUnsignedInt", datum.sizeof_u_int_)
		& boost::serialization::make_nvp("sizeOfUnsignedLong", datum.sizeof_u_long_)
		& boost::serialization::make_nvp("sizeOfUnsignedLongLong", datum.sizeof_u_long_long_)
		& boost::serialization::make_nvp("sizeOfFloat", datum.sizeof_float_)
		& boost::serialization::make_nvp("sizeOfDouble", datum.sizeof_double_)
		& boost::serialization::make_nvp("sizeOfLongDouble", datum.sizeof_long_double_)
		& boost::serialization::make_nvp("isBigEndian", datum.is_big_endian_)
		& boost::serialization::make_nvp("isNetworkByteOrder", datum.is_network_byte_order_);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace serialization

} // namespace boost

#ifdef TEST_MAIN

using namespace MLB::Utility;

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	std::cout <<
		"Test routine for Boost Serialization of class 'HostArchitectureInfo'" <<
		std::endl;
	std::cout <<
		"---- ------- --- ----- ------------- -- ----- ----------------------" <<
		std::endl;

	try {
		HostArchitectureInfo test_datum;
		TEST_Serialization(test_datum, "HostArchitectureInfo");
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

