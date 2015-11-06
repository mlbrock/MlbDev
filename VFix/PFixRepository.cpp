// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixRepository.cpp

	File Description	:	Implementation of the PFixRepository class.

	Revision History	:	2015-08-02 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFix/PFixRepository.hpp>
#include <VFix/VFixMISetHolder.hpp>

#include <Utility/C_StringSupport.hpp>
#include <Utility/InlineContainer.hpp>
#include <Utility/ParseNumericString.hpp>
#include <Utility/TabularReportSupport.hpp>
#include <Utility/Utility_Exception.hpp>

#include <boost/io/ios_state.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

namespace {

// ////////////////////////////////////////////////////////////////////////////
template <typename SetType>
	std::ostream &ShimEmitTabular(const SetType &in_set,
		std::ostream &o_str = std::cout)
{
	return(SetType::value_type::EmitTabular(in_set, o_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SetType>
	SetType &ShimParseXmlFile(const std::string &file_name, SetType &out_set)
{
	typename SetType::value_type sample_type;

	return(SetType::value_type::ParseXmlFile(file_name, out_set));
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
PFixRepository::PFixRepository()
	:component_set_ptr_(new PFixComponentSet)
	,enum_set_ptr_(new PFixEnumSet)
	,field_set_ptr_(new PFixFieldSet)
	,fieldtype_set_ptr_(new PFixFieldTypeSet)
	,message_set_ptr_(new PFixMessageSet)
	,msgcontent_set_ptr_(new PFixMsgContentSet)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixRepository::PFixRepository(const std::string &directory_name)
	:component_set_ptr_(new PFixComponentSet)
	,enum_set_ptr_(new PFixEnumSet)
	,field_set_ptr_(new PFixFieldSet)
	,fieldtype_set_ptr_(new PFixFieldTypeSet)
	,message_set_ptr_(new PFixMessageSet)
	,msgcontent_set_ptr_(new PFixMsgContentSet)
{
	std::string dir_name((!directory_name.empty()) ? directory_name : "./");
#ifdef _WIN32
	if (*(dir_name.end() - 1) == '\\')
		*(dir_name.end() - 1) = '/';
#endif // #ifdef _WIN32
	if (*(dir_name.end() - 1) != '/')
		dir_name += '/';

	ShimParseXmlFile(dir_name + "Components.xml",  *component_set_ptr_);
	ShimParseXmlFile(dir_name + "Enums.xml",       *enum_set_ptr_);
	ShimParseXmlFile(dir_name + "Fields.xml",      *field_set_ptr_);
	ShimParseXmlFile(dir_name + "DataTypes.xml",   *fieldtype_set_ptr_);
	ShimParseXmlFile(dir_name + "Messages.xml",    *message_set_ptr_);
	ShimParseXmlFile(dir_name + "MsgContents.xml", *msgcontent_set_ptr_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PFixRepository::swap(PFixRepository &other)
{
	std::swap(component_set_ptr_,  other.component_set_ptr_);
	std::swap(enum_set_ptr_,       other.enum_set_ptr_);
	std::swap(field_set_ptr_,      other.field_set_ptr_);
	std::swap(fieldtype_set_ptr_,  other.fieldtype_set_ptr_);
	std::swap(message_set_ptr_,    other.message_set_ptr_);
	std::swap(msgcontent_set_ptr_, other.msgcontent_set_ptr_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PFixRepository::LoadFromDirectory(const std::string &directory_name)
{
	PFixRepository(directory_name).swap(*this);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &PFixRepository::EmitTabular(std::ostream &o_str) const
{
	ShimEmitTabular(*component_set_ptr_,  o_str);
	ShimEmitTabular(*enum_set_ptr_,       o_str);
	ShimEmitTabular(*field_set_ptr_,      o_str);
	ShimEmitTabular(*fieldtype_set_ptr_,  o_str);
	ShimEmitTabular(*message_set_ptr_,    o_str);
	ShimEmitTabular(*msgcontent_set_ptr_, o_str);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PFixRepository &datum)
{
	o_str << *datum.component_set_ptr_  << std::endl;
	o_str << *datum.enum_set_ptr_       << std::endl;
	o_str << *datum.field_set_ptr_      << std::endl;
	o_str << *datum.fieldtype_set_ptr_  << std::endl;
	o_str << *datum.message_set_ptr_    << std::endl;
	o_str << *datum.msgcontent_set_ptr_ << std::endl;

	return(o_str);
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
void TEST_RunTest(const char *directory_name)
{
	PFixRepository test_repo(directory_name);

	std::cout << test_repo << std::endl << std::endl;

	test_repo.EmitTabular(std::cout);
	std::cout << std::endl;
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
				"   " << argv[0] << " <fix-repository-xml-file-directory> "
				"[ <fix-repository-xml-file-directory> ... ]" <<
				std::endl << std::endl;
			return(EXIT_SUCCESS);
		}
		else if (argc < 2)
			ThrowInvalidArgument("No XML files were specified. "
				"Use '-h' for help.");
		for (int count_1 = 1; count_1 < argc; ++count_1) {
			TEST_RunTest(argv[count_1]);
		}
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
	XML/FIX.5.0SP2/Base > TestResults.MLB.VFix.PFixRepository.txt

	Testing command line for Windows:
	------- ------- ---- --- --------
	..\..\..\..\..\VFix\XML\FIX.5.0SP2\Base > TestResults.MLB.VFix.PFixRepository.txt
*/

#endif // #ifdef TEST_MAIN

