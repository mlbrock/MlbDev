// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Tools Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Checks to ensure that all fields in an instruction
								context are fully-resolvable to the dispatch methods
								of ExcContext.

	Revision History	:	2008-02-23 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/ExcContext.hpp>

#include <Utility/StringSupport.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
struct AFS_ByFieldOp {
	AFS_ByFieldOp()
		:item_ptr_()
		,template_ptr_()
		,field_operator_(FieldOperator_None)
		,data_type_(DataType_Invalid)
		,flags_(InsItemFlag_None)
		,first_field_index_(0)
		,field_index_set_()
	{
	}

	AFS_ByFieldOp(const InsContext &ins_context, const InsItem &item)
		:item_ptr_(&item)
		,template_ptr_(&(ins_context.ins_item_list_[item.template_index_]))
		,field_operator_(item.field_operator_)
		,data_type_(item.data_type_)
		,flags_(static_cast<InsItemFlag>(item.flags_ & InsItemFlag_ModifiesOp))
		,first_field_index_(item.item_index_)
		,field_index_set_()
	{
		field_index_set_.insert(item.item_index_);
	}

	bool operator < (const AFS_ByFieldOp &other) const
	{
		if (field_operator_ < other.field_operator_)
			return(true);
		else if (field_operator_ == other.field_operator_) {
			if (data_type_ < other.data_type_)
				return(true);
			else if ((data_type_ == other.data_type_) && (flags_ < other.flags_))
				return(true);
		}

		return(false);
	}

	const InsItem          *item_ptr_;
	const InsItem          *template_ptr_;
	FieldOperator           field_operator_;
	DataType                data_type_;
	InsItemFlag             flags_;
	unsigned int            first_field_index_;
	std::set<unsigned int>  field_index_set_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::set<AFS_ByFieldOp>          AFS_ByFieldOpSet;
typedef AFS_ByFieldOpSet::iterator       AFS_ByFieldOpSetIter;
typedef AFS_ByFieldOpSet::const_iterator AFS_ByFieldOpSetIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const AFS_ByFieldOp &datum)
{
	o_str
		<< std::left
		<< std::setw(10) << datum.field_operator_                 << " "
		<< std::setw(14) << DataTypeToString(datum.data_type_)    << " "
		<< std::setw(13) << InsItemFlagBitsToString(datum.flags_) << " "
		<< std::right
		<< std::setw( 6) << datum.field_index_set_.size()
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
AFS_ByFieldOpSet GetAFS_ByFieldOpSet(const InsContext &ins_context)
{
	AFS_ByFieldOpSet out_set;
	InsItemListIterC iter_b(ins_context.ins_item_list_.begin());
	InsItemListIterC iter_e(ins_context.ins_item_list_.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		if (iter_b->data_type_ != DataType_Template) {
			AFS_ByFieldOp        datum(ins_context, *iter_b);
			AFS_ByFieldOpSetIter iter_f(out_set.find(datum));
			if (iter_f == out_set.end())
            out_set.insert(datum);
			else
				const_cast<AFS_ByFieldOp &>(*iter_f).
					field_index_set_.insert(iter_b->item_index_);
		}
	}

	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void EmitAFSByFieldOpSetHeader()
{
	std::cout
		<< std::left
		<< std::setw(10) << "Operator"   << " "
		<< std::setw(14) << "Data Type"  << " "
		<< std::setw(13) << "Flags"      << " "
		<< std::setw( 6) << "Total"      << " "
		<< std::setw( 6) << "Index"      << " "
		<< std::setw( 6) << "Aux Id"     << " "
		<< std::setw(32) << "Field Name" << " "
		<< std::setw(40) << "Template Name"
		<< std::right
		<< std::endl
			;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void EmitAFSByFieldOpSetSeparator(char fill_char)
{
	std::cout
		<< std::left
		<< std::setfill(fill_char)
		<< std::setw(10) << "" << " "
		<< std::setw(14) << "" << " "
		<< std::setw(13) << "" << " "
		<< std::setw( 6) << "" << " "
		<< std::setw( 6) << "" << " "
		<< std::setw( 6) << "" << " "
		<< std::setw(32) << "" << " "
		<< std::setw(40) << ""
		<< std::setfill(' ')
		<< std::right
		<< std::endl
			;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void EmitAFSByFieldOpSet(const InsContext &ins_context,
	const AFS_ByFieldOpSet &afs_bfo_set)
{
	AFS_ByFieldOpSetIterC iter_set_b(afs_bfo_set.begin());
	AFS_ByFieldOpSetIterC iter_set_e(afs_bfo_set.end());

	for ( ; iter_set_b != iter_set_e; ++iter_set_b) {
		EmitAFSByFieldOpSetSeparator('=');
		EmitAFSByFieldOpSetHeader();
		EmitAFSByFieldOpSetSeparator('-');
		std::set<unsigned int>::const_iterator iter_item_b(
			iter_set_b->field_index_set_.begin());
		std::set<unsigned int>::const_iterator iter_item_e(
			iter_set_b->field_index_set_.end());
		for ( ; iter_item_b != iter_item_e; ++iter_item_b) {
			const InsItem *item_ptr = &(ins_context.ins_item_list_[*iter_item_b]);
			std::cout
				<< *iter_set_b                              << " "
				<< std::setw( 6) << *iter_item_b            << " "
				<< std::setw( 6) << item_ptr->auxiliary_id_ << " "
				<< std::left
				<< std::setw(32) << item_ptr->field_name_   << " "
				<< ins_context.ins_item_list_[item_ptr->template_index_].field_name_
				<< std::right
				<< std::endl
					;
		}
		EmitAFSByFieldOpSetSeparator('=');
		std::cout << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

using namespace MLB::VFast;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"<XML-template-file> [ <XML-template-file> ... ] " <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}

	try {
		if (argc < 2)
			MLB::Utility::ThrowInvalidArgument("Expected at least one XML "
				"template file name on the command line. Use '-h' for help.");
		unsigned int total_errors = 0;
		int          count_1;
		for (count_1 = 1; count_1 < argc; ++count_1) {
			std::cout << MLB::Utility::PadLeft("", 190, '*') << std::endl;
			std::cout << MLB::Utility::PadLeft("", 190, '*') << std::endl;
			std::cout << "Template File: " << argv[count_1] << std::endl;
			std::cout << MLB::Utility::PadLeft("", 190, '*') << std::endl;
			try {
				std::vector<std::string> error_list;
				InsContext               ins_context(argv[count_1]);
				AFS_ByFieldOpSet         results(
					GetAFS_ByFieldOpSet(ins_context));
				EmitAFSByFieldOpSet(ins_context, results);
			}
			catch (const std::exception &except) {
				std::cout << "Error occurred while processing XML template "
					"file '" << argv[count_1] << "': " << except.what() << std::endl;
			}
			std::cout << MLB::Utility::PadLeft("", 190, '*') << std::endl;
			std::cout << std::endl;
		}
		std::cout << "Encountered " << total_errors << " error(s) in " <<
			(argc - 1) << " processed XML template file(s)." << std::endl <<
			std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

/*
	Testing command line for Windows:
	------- ------- ---- --- --------
	C:\DevEnv\Tests\FastCommon\Templates\CEF\CEF_template.20081112_A.xml C:\DevEnv\Tests\FastCommon\Templates\CME\CME_templates.20080925_A.xml C:\DevEnv\Tests\FastCommon\Templates\CME\CME_templates.20081007_A.xml C:\DevEnv\Tests\FastCommon\Templates\ISE\ISE_fast_templates_v2.0.ORIGINAL.xml > C:\DevEnv\Tests\VFast\UnitTests\CheckFieldDispatch.txt
*/

