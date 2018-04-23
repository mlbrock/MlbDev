// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Tools Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Message parser tool.

	Revision History	:	2008-12-27 --- Creation.
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

#include <VFast/PacketFileInterface.hpp>

// CODE NOTE: These are needed for the .cpp only.
#include <Utility/InlineContainer.hpp>
#include <Utility/HRTimer.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/ParseCmdLineArg.hpp>

// ////////////////////////////////////////////////////////////////////////////

using namespace MLB::VFast;

namespace {
// ////////////////////////////////////////////////////////////////////////////
class MyMsgHandlerFunc : public BaseMsgHandlerFunc {
	struct MyNullDeleter {
		void operator()(void const *) const {}
	};
public:
	typedef std::set<MLB::VFast::Type_UInt32> MySet;

	MyMsgHandlerFunc()
		:seen_ins_context_(false)
		,contract_id_set_()
	{
	}

	virtual bool ThisParseCmdLine(PacketFileInterface & /* reader */,
		int argc, char **argv, unsigned int &current_index)
	{
		try {
			std::string tmp_string;
			if (!MLB::Utility::ParseCmdLineArg::ParseCmdLineFollowingSpec(
				MLB::Utility::MakeInlineVector<std::string>
					("-CONTRACT_IDENTIFIERS")
					("-CONTRACTIDENTIFIERS")
					("-CONTRACT_IDENTIFIER")
					("-CONTRACTIDENTIFIER")
					("-CONTRACT_IDS")
					("-CONTRACTIDS")
					("-CONTRACT_ID")
					("-CONTRACTID"), current_index, argc, argv, tmp_string))
				return(false);
			std::vector<std::string> tmp_list;
			MLB::Utility::SplitString(tmp_string, ",", tmp_list, 0, false);
			std::vector<std::string>::iterator iter_b(tmp_list.begin());
			std::vector<std::string>::iterator iter_e(tmp_list.end());
			for ( ; iter_b != iter_e; ++iter_b) {
				std::string             this_element;
				MLB::VFast::Type_UInt32 tmp_value;
				MLB::Utility::UpperCase(
					MLB::Utility::TrimString(*iter_b, this_element));
				if (this_element.empty())
					MLB::Utility::ThrowInvalidArgument("Empty contract identifer "
						"string encountered.");
				MLB::Utility::ParseCmdLineArg::ParseCmdLineNumeric(
					this_element, tmp_value);
				contract_id_set_.insert(tmp_value);
			}
		}
		catch (const std::exception &except) {
			MLB::Utility::Rethrow(except, "Failed to parse the list of contract "
				"identifiers for the '" + std::string(argv[current_index - 1]) +
				"' parameter: " + std::string(except.what()));
		}

		return(true);
	}

	virtual void PostParseCmdLine(PacketFileInterface &reader,
		int /* argc */, char ** /* argv */)
	{
		if (reader.content_type_ != PacketDecodeRegime_EBS) {
			std::cout << "Coerced the packet file context type to Eurex "
				"Enhanced Broadcast Solution (EBS)." << std::endl;
			reader.content_type_ = PacketDecodeRegime_EBS;
		}
	}

	virtual bool PostDecodeMsg(MLB::VFast::PacketFileInterface & /* reader */,
		const MLB::VFast::PacketFileInterfaceState &pfi_state,
		ExcResults &exc_results)
	{
		if (exc_results.template_id_ != 7)
			return(true);

		if (!seen_ins_context_) {
			const MLB::VFast::InsContext &ins_context(pfi_state.ins_context_);
			timestamp         = ins_context.GetItemRef(7, 0, "timestamp",
				MLB::VFast::DataType_UInt32).item_index_;
			srcId             = ins_context.GetItemRef(7, 0, "srcId",
				MLB::VFast::DataType_UInt32).item_index_;
			seqNum            = ins_context.GetItemRef(7, 0, "seqNum",
				MLB::VFast::DataType_UInt32).item_index_;
			cntrId            = ins_context.GetItemRef(7, 0, "cntrId",
				MLB::VFast::DataType_UInt32).item_index_;
			cntrStatus        = ins_context.GetItemRef(7, 0, "cntrStatus",
				MLB::VFast::DataType_Ascii).item_index_;
			noOfEntries       = ins_context.GetItemRef(7, 0, "noOfEntries",
				MLB::VFast::DataType_UInt32).item_index_;
			updateAction      = ins_context.GetItemRef(7, 0, "updateAction",
				MLB::VFast::DataType_Ascii).item_index_;
			entryType         = ins_context.GetItemRef(7, 0, "entryType",
				MLB::VFast::DataType_Ascii).item_index_;
			entryPrcLvl       = ins_context.GetItemRef(7, 0, "entryPrcLvl",
				MLB::VFast::DataType_UInt32).item_index_;
			entryPrc          = ins_context.GetItemRef(7, 0, "entryPrc",
				MLB::VFast::DataType_Decimal).item_index_;
			entryQty          = ins_context.GetItemRef(7, 0, "entryQty",
				MLB::VFast::DataType_UInt32).item_index_;
			entryPrcSrc       = ins_context.GetItemRef(7, 0, "entryPrcSrc",
				MLB::VFast::DataType_Ascii).item_index_;
			seen_ins_context_ = true;
		}

		exc_results.PopulateExcEntryLookupList();

		MLB::VFast::Type_UInt32  tmp_timestamp;
		MLB::VFast::Type_UInt32  tmp_srcId;
		MLB::VFast::Type_UInt32  tmp_seqNum;
		MLB::VFast::Type_UInt32  tmp_cntrId;
		char                     tmp_cntrStatus[VFastMaxStringLength + 1];
		MLB::VFast::Type_UInt32  tmp_noOfEntries;

		tmp_cntrStatus[0] = '\0';

		if (exc_results.LookupValueFirst(cntrId, tmp_cntrId, false) == NULL)
			return(true);

		if ((!contract_id_set_.empty()) &&
			(contract_id_set_.find(tmp_cntrId) == contract_id_set_.end()))
			return(true);

		exc_results.LookupValueFirst(timestamp, tmp_timestamp, false);
		exc_results.LookupValueFirst(srcId, tmp_srcId, false);
		exc_results.LookupValueFirst(seqNum, tmp_seqNum, false);
		exc_results.LookupValueFirst(cntrStatus, tmp_cntrStatus, true,
			sizeof(cntrStatus) - 1);
		exc_results.LookupValueFirst(noOfEntries, tmp_noOfEntries, true);

		if (!tmp_noOfEntries)
			return(true);
		MLB::VFast::ExcEntryLookup lookup_updateAction =
			exc_results.lookup_list_[updateAction];
		MLB::VFast::ExcEntryLookup lookup_entryType    =
			exc_results.lookup_list_[entryType];
		MLB::VFast::ExcEntryLookup lookup_entryPrcLvl  =
			exc_results.lookup_list_[entryPrcLvl];
		MLB::VFast::ExcEntryLookup lookup_entryPrc     =
			exc_results.lookup_list_[entryPrc];
		MLB::VFast::ExcEntryLookup lookup_entryQty     =
			exc_results.lookup_list_[entryQty];
		MLB::VFast::ExcEntryLookup lookup_entryPrcSrc  =
			exc_results.lookup_list_[entryPrcSrc];

		for (unsigned int count_1 = 0; count_1 < tmp_noOfEntries; ++count_1) {
			char                     tmp_updateAction[VFastMaxStringLength + 1];
			char                     tmp_entryType[VFastMaxStringLength + 1];
			MLB::VFast::Type_UInt32  tmp_entryPrcLvl;
			MLB::VFast::Type_Decimal tmp_entryPrc;
			MLB::VFast::Type_UInt32  tmp_entryQty;
			char                     tmp_entryPrcSrc[VFastMaxStringLength + 1];
			exc_results.LookupValueAt(updateAction,
				lookup_updateAction.first_, tmp_updateAction, true,
				sizeof(tmp_updateAction) - 1);
			exc_results.LookupValueAt(entryType,
				lookup_entryType.first_, tmp_entryType, true,
				sizeof(tmp_entryType) - 1);
			exc_results.LookupValueAt(entryPrcLvl,
				lookup_entryPrcLvl.first_, tmp_entryPrcLvl, true);
			exc_results.LookupValueAt(entryPrc,
				lookup_entryPrc.first_, tmp_entryPrc, true);
			exc_results.LookupValueAt(entryQty,
				lookup_entryQty.first_, tmp_entryQty, true);
			exc_results.LookupValueAt(entryPrcSrc,
				lookup_entryPrcSrc.first_, tmp_entryPrcSrc, true,
				sizeof(tmp_entryPrcSrc) - 1);
			exc_results.MoveFirstToNext(lookup_updateAction);
			exc_results.MoveFirstToNext(lookup_entryType);
			exc_results.MoveFirstToNext(lookup_entryPrcLvl);
			exc_results.MoveFirstToNext(lookup_entryPrc);
			exc_results.MoveFirstToNext(lookup_entryQty);
			exc_results.MoveFirstToNext(lookup_entryPrcSrc);
			std::cout
				<< std::setw(10) << tmp_timestamp    << " "
				<< std::setw( 5) << tmp_srcId        << " "
				<< std::setw( 8) << tmp_seqNum       << " "
				<< std::setw( 8) << tmp_cntrId       << " "
				<< std::setw( 6) << tmp_cntrStatus   << " "
				<< std::setw( 3) << tmp_noOfEntries  << " "
				<< std::setw( 6) << tmp_updateAction << " "
				<< std::setw( 6) << tmp_entryType    << " "
				<< std::setw( 6) << tmp_entryPrcLvl  << " "
				<< std::setw(16) << tmp_entryPrc     << " "
				<< std::setw( 6) << tmp_entryQty     << " "
				<< std::setw( 6) << tmp_entryPrcSrc
				<< std::endl
				;
		}

		return(true);
	}

	bool                              seen_ins_context_;
	std::set<MLB::VFast::Type_UInt32> contract_id_set_;
	MLB::VFast::Type_UInt32           timestamp;
	MLB::VFast::Type_UInt32           srcId;
	MLB::VFast::Type_UInt32           seqNum;
	MLB::VFast::Type_UInt32           cntrId;
	MLB::VFast::Type_UInt32           cntrStatus;
	MLB::VFast::Type_UInt32           noOfEntries;
	MLB::VFast::Type_UInt32           updateAction;
	MLB::VFast::Type_UInt32           entryType;
	MLB::VFast::Type_UInt32           entryPrcLvl;
	MLB::VFast::Type_UInt32           entryPrc;
	MLB::VFast::Type_UInt32           entryQty;
	MLB::VFast::Type_UInt32           entryPrcSrc;
};
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		PacketFileInterface compat_check;
		MyMsgHandlerFunc    handler_func;
		compat_check.ParseCmdLine(handler_func, argc, argv);
		compat_check.Run(handler_func);
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
	-progress 1000 -debug false -content_type EUREX -packet_format raw          -timer true -xml_file C:\DevEnv\Tests\FastCommon\Templates\CEF\CEF_template.20081112_A.xml -packet_file C:\DevEnv\Tests\FastCommon\PacketData\CEF\EEBS_samples_151008\EEBS_Simu_151008\EEBS_ODBDelta.dat  > C:\DevEnv\Tests\VFast\UnitTests\EbsDeltaAnalyzer.CEF.EEBS_ODBDelta.dat.txt

	-progress 1000 -debug false -content_type EUREX -packet_format length       -timer true -xml_file C:\DevEnv\Tests\FastCommon\Templates\CEF\CEF_template.20081112_A.xml -packet_file C:\DevEnv\Tests\FastCommon\PacketData\CEF\20090122_A\CEF_Channel.___224.0.29.205___50000.bin > C:\DevEnv\Tests\VFast\UnitTests\EbsDeltaAnalyzer.CEF.CEF_Channel.___224.0.29.205___50000.bin.txt
*/

