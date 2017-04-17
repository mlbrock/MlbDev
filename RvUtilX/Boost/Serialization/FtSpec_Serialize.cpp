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
								FtSpec.

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

#include <Utility/Boost/Serialization/BoostSerialization.ipp>
#include <RvUtilX/Boost/Serialization/BoostSerialization.hpp>

//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Use macros to get declarations and forced instantiations done...
// ////////////////////////////////////////////////////////////////////////////

	//	Declare the template functions...
MLB_BOOSTX_SERIAL_TEMPLATE(MLB::RvUtilX::FtSpec)

	//	Force instantiation of the serialize free function...
MLB_BOOSTX_SERIAL_DECL_Serialize(MLB::RvUtilX::FtSpec)

	// Force instantiation of the Serialize{Load|Save}{Binary|Text|Xml} funcs.
MLB_BOOSTX_SerializeLoadSave(MLB::RvUtilX::FtSpec)

// ////////////////////////////////////////////////////////////////////////////

namespace boost {

namespace serialization {

// ////////////////////////////////////////////////////////////////////////////
template <typename SArchive>
	void serialize(SArchive &s_archive, MLB::RvUtilX::FtSpec &datum,
		const unsigned int)
{
	s_archive
		& boost::serialization::make_nvp("groupName",  datum.group_name_)
		& boost::serialization::make_nvp("weight",     datum.weight_)
		& boost::serialization::make_nvp("activeGoal", datum.active_goal_)
		& boost::serialization::make_nvp("heartbeatInterval",
			datum.heartbeat_interval_)
		& boost::serialization::make_nvp("preparationInterval",
			datum.preparation_interval_)
		& boost::serialization::make_nvp("activationInterval",
			datum.activation_interval_);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace serialization

} // namespace boost

#ifdef TEST_MAIN

using namespace MLB::RvUtilX;

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	std::cout <<
		"Test routine for Boost Serialization of class 'FtSpec'" << std::endl;
	std::cout <<
		"---- ------- --- ----- ------------- -- ----- --------" << std::endl;

	try {
		FtSpec test_datum("MyFTGroupName", 1, 1, 1.1, 2,2, 3.3);
		TEST_Serialization(test_datum, "FtSpec");
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

