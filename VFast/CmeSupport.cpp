// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Miscellaneous supporting logic for use with the CME.

	Revision History	:	2008-11-14 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/CmeSupport.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
const char *DetermineCmeMessageType(const InsContext &ins_context,
	const InsItem &template_ref)
{
	const InsItem *item_ptr =
		ins_context.GetItemPtr(template_ref, CmeFid_MessageType);

	if ((item_ptr != NULL) && (item_ptr->data_type_ == DataType_Ascii) &&
		(item_ptr->field_operator_ == FieldOperator_Constant))
      return(ins_context.dict_value_list_[item_ptr->dict_value_index_].string_);

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *DetermineCmeMessageType(const InsContext &ins_context,
	unsigned int template_id)
{
	return(DetermineCmeMessageType(ins_context,
		ins_context.ins_item_list_[ins_context.GetTemplateIndex(template_id)]));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CmeTidToMessageTypeMap &ConstructCmeTidToMessageTypeMap(
	const InsContext &ins_context, CmeTidToMessageTypeMap &out_map)
{
	CmeTidToMessageTypeMap tmp_map;
	InsItemMapIterC        iter_b(ins_context.template_map_.begin());
	InsItemMapIterC        iter_e(ins_context.template_map_.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		try {
			const char *msg_type = DetermineCmeMessageType(ins_context,
				ins_context.ins_item_list_[iter_b->second.first]);
			if (msg_type == NULL)
				MLB::Utility::ThrowLogicError("The 'MessageType' field could not "
					"be located.");
			else if (!(*msg_type))
				MLB::Utility::ThrowLogicError("The 'MessageType' field was empty.");
			else if (::strlen(msg_type) > 1)
				MLB::Utility::ThrowLogicError("The length of the 'MessageType' "
					"field is " + MLB::Utility::AnyToString(::strlen(msg_type)) + 
					" --- the maximum valid length is 1.");
			tmp_map[iter_b->first] = *msg_type;
		}
		catch (const std::exception &except) {
			MLB::Utility::Rethrow(except, "Attempt to resolve the 'MsgType' "
				"field (FID " + MLB::Utility::AnyToString(CmeFid_MessageType) +
				") for template id " + MLB::Utility::AnyToString(iter_b->first) +
				" ('" +
				ins_context.ins_item_list_[iter_b->second.first].field_name_ +
				"') failed: " + std::string(except.what()));
		}
	}

	out_map.swap(tmp_map);

	return(out_map);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
CmeTidToMessageTypeMap ConstructCmeTidToMessageTypeMap(
	const InsContext &ins_context)
{
	CmeTidToMessageTypeMap out_map;

	return(ConstructCmeTidToMessageTypeMap(ins_context, out_map));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
char CmeTidToMessageType(const CmeTidToMessageTypeMap &msg_type_map,
	unsigned int template_id)
{
	CmeTidToMessageTypeMapIterC iter_f(msg_type_map.find(template_id));

	return((iter_f != msg_type_map.end()) ? iter_f->second : '\0');
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#ifdef TEST_MAIN

#include <VFast/VFastParseCmdLineArg.hpp>

#include <Utility/StringSupport.hpp>

using namespace MLB::VFast;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	using namespace MLB::VFast;
	
	int return_code = EXIT_SUCCESS;

	try {
		std::string xml_template_file;
		for (unsigned int count_1 = 1; count_1 < static_cast<unsigned int>(argc);
			++count_1) {
			if (VFastParseCmdLineArg::ParseCmdLineXmlTemplateFile(
				count_1, argc, argv, xml_template_file))
				;
			else
				VFastParseCmdLineArg::InvalidArgument(argv[count_1]);
		}
		if (xml_template_file.empty())
			MLB::Utility::ThrowInvalidArgument("The FAST XML template file was "
				"not specified.");
		InsContext                  ins_context(xml_template_file);
		// ----------------------------------------------------------------
		std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
		std::cout << "Complete Template Message Type List:" << std::endl;
		std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
		CmeTidToMessageTypeMap      msg_type_map(
			ConstructCmeTidToMessageTypeMap(ins_context));
		CmeTidToMessageTypeMapIterC iter_b(msg_type_map.begin());
		CmeTidToMessageTypeMapIterC iter_e(msg_type_map.end());
		for ( ; iter_b != iter_e; ++iter_b)
			std::cout << "Template Id: " << std::setw(10) << iter_b->first <<
				": Message Type: " << iter_b->second << std::endl;
		std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
		std::cout << std::endl;
		// ----------------------------------------------------------------
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

/*
	Testing command line for Windows:
	------- ------- ---- --- --------
	-xml_file C:\DevEnv\Tests\FastCommon\Templates\CME\CME_templates.20081007_A.xml > C:\DevEnv\Tests\VFast\UnitTests\CmeSupport.txt
*/

