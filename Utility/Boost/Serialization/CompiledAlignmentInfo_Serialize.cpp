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
								CompiledAlignmentInfo.

	Revision History	:	2006-12-22 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2006 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/CompiledAlignmentInfo.hpp>
#include <Utility/Boost/Serialization/BoostSerialization.ipp>

//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Use macros to get declarations and forced instantiations done...
// ////////////////////////////////////////////////////////////////////////////

	//	Declare the template functions...
MLB_BOOSTX_SERIAL_TEMPLATE(MLB::Utility::CompiledAlignmentInfo)

	//	Force instantiation of the serialize free function...
MLB_BOOSTX_SERIAL_DECL_Serialize(MLB::Utility::CompiledAlignmentInfo)

	// Force instantiation of the Serialize{Load|Save}{Binary|Text|Xml} funcs.
MLB_BOOSTX_SerializeLoadSave(MLB::Utility::CompiledAlignmentInfo)

// ////////////////////////////////////////////////////////////////////////////

namespace boost {

namespace serialization {

// ////////////////////////////////////////////////////////////////////////////
template <typename SArchive>
	void serialize(SArchive &s_archive,
	MLB::Utility::CompiledAlignmentInfo &datum, const unsigned int)
{
	s_archive
		& boost::serialization::make_nvp("alignmentOfUnsignedChar",
			datum.align_u_char_)
		& boost::serialization::make_nvp("alignmentOfUnsignedShort",
			datum.align_u_short_)
		& boost::serialization::make_nvp("alignmentOfUnsignedInt",
			datum.align_u_int_)
		& boost::serialization::make_nvp("alignmentOfUnsignedLong",
			datum.align_u_long_)
		& boost::serialization::make_nvp("alignmentOfUnsignedLongLong",
			datum.align_u_long_long_)
		& boost::serialization::make_nvp("alignmentOfFloat",
			datum.align_float_)
		& boost::serialization::make_nvp("alignmentOfDouble",
			datum.align_double_)
		& boost::serialization::make_nvp("alignmentOfLongDouble",
			datum.align_long_double_)
		& boost::serialization::make_nvp("alignmentOfStructure",
			datum.align_structure_);
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
		"Test routine for Boost Serialization of class "
		"'CompiledAlignmentInfo'" << std::endl;
	std::cout <<
		"---- ------- --- ----- ------------- -- ----- "
		"-----------------------" << std::endl;

	try {
		CompiledAlignmentInfo test_datum;
		TEST_Serialization(test_datum, "CompiledAlignmentInfo");
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

