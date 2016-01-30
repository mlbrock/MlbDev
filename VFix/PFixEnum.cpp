// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixEnum.cpp

	File Description	:	Implementation of the PFixEnum class.

	Revision History	:	2015-08-02 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFix/PFixEnum.hpp>
#include <VFix/VFixMISetHolder.hpp>

#include <Utility/InlineContainer.hpp>
#include <Utility/ParseNumericString.hpp>
#include <Utility/TabularReportSupport.hpp>
#include <Utility/Utility_Exception.hpp>

#include <boost/io/ios_state.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

// ////////////////////////////////////////////////////////////////////////////
PFixEnum::PFixEnum()
	:tag_(0)
	,value_()
	,symbolic_name_()
	,sort_order_(0)
	,fix_version_()
	,description_()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Temporary; pending implementation.
std::ostream &PFixEnum::EmitTabular(const PFixEnumSet & /* in_set */,
	std::ostream &o_str)
{
	o_str << "PFixEnum::EmitTabular() not yet implemented.";

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Temporary; pending implementation.
PFixEnumSet &PFixEnum::ParseXmlFile(const std::string & /* file_name */,
	PFixEnumSet &out_set)
{
	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Temporary; pending implementation.
std::ostream & operator << (std::ostream &o_str, const PFixEnumSet &datum)
{
	return(PFixEnum::EmitTabular(datum, o_str));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/StringSupport.hpp>
#include <Utility/ParseCmdLineArg.hpp>

using namespace MLB::VFix;

namespace {

// ////////////////////////////////////////////////////////////////////////////
void TEST_RunTest(const char * /* field_file_name */,
	const char * /* data_type_file_name */)
{
	throw std::logic_error("Class PFixEnum not yet implemented.");
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	using namespace MLB::Utility;

	int return_code = EXIT_SUCCESS;

	try {
		if (ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
			std::cout << "USAGE: " << std::endl <<
				"   " << argv[0] << " <fix-xml-enums-file> "
				"[ <fix-xml-enums-file> ... ]" <<
				std::endl << std::endl;
			return(EXIT_SUCCESS);
		}
		else if (argc < 2)
			ThrowInvalidArgument("No XML files were specified. "
				"Use '-h' for help.");
		else if (argc > 3)
			ThrowInvalidArgument("Unexpected arguments encountered. "
				"Use '-h' for help.");
		TEST_RunTest(argv[1], (argc == 3) ? argv[2] : NULL);
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

/*
	Testing command line for Linux:
	------- ------- ---- --- ------
	XML/FIX.5.0SP2/Base/Enums.xml > TestResults.MLB.VFix.PFixEnum.txt

	Testing command line for Windows:
	------- ------- ---- --- --------
	..\..\..\..\..\VFix\XML\FIX.5.0SP2\Base\Enums.xml > TestResults.MLB.VFix.PFixEnum.txt
*/

#endif // #ifdef TEST_MAIN

