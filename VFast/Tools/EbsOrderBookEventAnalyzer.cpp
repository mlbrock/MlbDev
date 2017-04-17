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
		,emit_packet_location_(false)
		,first_exchange_time_(0)
		,last_exchange_time_(std::numeric_limits<Type_UInt32>::max())
		,contract_id_set_()
	{
	}

	virtual bool ThisParseCmdLine(PacketFileInterface & /* reader */, int argc,
		char **argv, unsigned int &current_index)
	{
		std::string tmp_string;

		if (MLB::Utility::ParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-EMIT_PACKET_LOCATION")
				("-EMIT_PACKETLOCATION")
				("-EMITPACKET_LOCATION")
				("-EMITPACKETLOCATION")
				("-EMIT_PKT_LOCATION")
				("-EMIT_PKTLOCATION")
				("-EMITPKT_LOCATION")
				("-EMITPKTLOCATION")
				("-EMIT_PACKET_LOCATE")
				("-EMIT_PACKETLOCATE")
				("-EMITPACKET_LOCATE")
				("-EMITPACKETLOCATE")
				("-EMIT_PKT_LOCATE")
				("-EMIT_PKTLOCATE")
				("-EMITPKT_LOCATE")
				("-EMITPKTLOCATE")
				("-EMIT_PACKET_LOC")
				("-EMIT_PACKETLOC")
				("-EMITPACKET_LOC")
				("-EMITPACKETLOC")
				("-EMIT_PKT_LOC")
				("-EMIT_PKTLOC")
				("-EMITPKT_LOC")
				("-EMITPKTLOC")
					,
			current_index, argc, argv, emit_packet_location_))
			return(true);
		else if (MLB::Utility::ParseCmdLineArg::ParseCmdLineFollowingSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-CONTRACT_IDENTIFIERS")
				("-CONTRACTIDENTIFIERS")
				("-CONTRACT_IDENTIFIER")
				("-CONTRACTIDENTIFIER")
				("-CONTRACT_IDS")
				("-CONTRACTIDS")
				("-CONTRACT_ID")
				("-CONTRACTID")
					,
			current_index, argc, argv, tmp_string)) {
			try {
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
						MLB::Utility::ThrowInvalidArgument("Empty contract "
							"identifer string encountered.");
					MLB::Utility::ParseCmdLineArg::ParseCmdLineNumeric(
						this_element, tmp_value);
					contract_id_set_.insert(tmp_value);
				}
			}
			catch (const std::exception &except) {
				MLB::Utility::Rethrow(except, "Failed to parse the list of "
					"contract identifiers for the '" +
					std::string(argv[current_index - 1]) + "' parameter: " +
					std::string(except.what()));
			}
			return(true);
		}
		else if (MLB::Utility::ParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-FIRST_EXCHANGE_TIMESTAMP")
				("-FIRST_EXCHANGETIMESTAMP")
				("-FIRSTEXCHANGE_TIMESTAMP")
				("-FIRSTEXCHANGETIMESTAMP")
				("-FIRST_EXCHANGE_TIME")
				("-FIRST_EXCHANGETIME")
				("-FIRSTEXCHANGE_TIME")
				("-FIRSTEXCHANGETIME")
				("-FIRST_EXCH_TIMESTAMP")
				("-FIRST_EXCHTIMESTAMP")
				("-FIRSTEXCH_TIMESTAMP")
				("-FIRSTEXCHTIMESTAMP")
				("-FIRST_EXCH_TIME")
				("-FIRST_EXCHTIME")
				("-FIRSTEXCH_TIME")
				("-FIRSTEXCHTIME")
					,
			current_index, argc, argv, first_exchange_time_))
			return(true);
		else if (MLB::Utility::ParseCmdLineArg::ParseCmdLineDatumSpec(
			MLB::Utility::MakeInlineVector<std::string>
				("-LAST_EXCHANGE_TIMESTAMP")
				("-LAST_EXCHANGETIMESTAMP")
				("-LASTEXCHANGE_TIMESTAMP")
				("-LASTEXCHANGETIMESTAMP")
				("-LAST_EXCHANGE_TIME")
				("-LAST_EXCHANGETIME")
				("-LASTEXCHANGE_TIME")
				("-LASTEXCHANGETIME")
				("-LAST_EXCH_TIMESTAMP")
				("-LAST_EXCHTIMESTAMP")
				("-LASTEXCH_TIMESTAMP")
				("-LASTEXCHTIMESTAMP")
				("-LAST_EXCH_TIME")
				("-LAST_EXCHTIME")
				("-LASTEXCH_TIME")
				("-LASTEXCHTIME")
					,
			current_index, argc, argv, last_exchange_time_))
			return(true);

		return(false);
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
		if ((exc_results.template_id_ != 6) && (exc_results.template_id_ != 7))
			return(true);

if (exc_results.template_id_ == 6)
	return(true);

		if (!seen_ins_context_) {
			const MLB::VFast::InsContext &ins_context(pfi_state.ins_context_);
			//	Resolve the snapshot fields...
			timestamp_6         = ins_context.GetItemRef(6, 0, "timestamp",
				MLB::VFast::DataType_UInt32).item_index_;
			srcId_6             = ins_context.GetItemRef(6, 0, "srcId",
				MLB::VFast::DataType_UInt32).item_index_;
			seqNum_6            = ins_context.GetItemRef(6, 0, "consolSeqNum",
				MLB::VFast::DataType_UInt32).item_index_;
			cntrId_6            = ins_context.GetItemRef(6, 0, "cntrId",
				MLB::VFast::DataType_UInt32).item_index_;
			cntrStatus_6        = ins_context.GetItemRef(6, 0, "cntrStatus",
				MLB::VFast::DataType_Ascii).item_index_;
			noOfEntries_6       = ins_context.GetItemRef(6, 0, "noOfEntries",
				MLB::VFast::DataType_UInt32).item_index_;
/*
			updateAction_6      = ins_context.GetItemRef(6, 0, "updateAction",
				MLB::VFast::DataType_Ascii).item_index_;
*/
			entryType_6         = ins_context.GetItemRef(6, 0, "entryType",
				MLB::VFast::DataType_Ascii).item_index_;
			entryPrcLvl_6       = ins_context.GetItemRef(6, 0, "entryPrcLvl",
				MLB::VFast::DataType_UInt32).item_index_;
			entryPrc_6          = ins_context.GetItemRef(6, 0, "entryPrc",
				MLB::VFast::DataType_Decimal).item_index_;
			entryQty_6          = ins_context.GetItemRef(6, 0, "entryQty",
				MLB::VFast::DataType_UInt32).item_index_;
			entryPrcSrc_6       = ins_context.GetItemRef(6, 0, "entryPrcSrc",
				MLB::VFast::DataType_Ascii).item_index_;
			//	Resolve the delta fields...
			timestamp_7         = ins_context.GetItemRef(7, 0, "timestamp",
				MLB::VFast::DataType_UInt32).item_index_;
			srcId_7             = ins_context.GetItemRef(7, 0, "srcId",
				MLB::VFast::DataType_UInt32).item_index_;
			seqNum_7            = ins_context.GetItemRef(7, 0, "seqNum",
				MLB::VFast::DataType_UInt32).item_index_;
			cntrId_7            = ins_context.GetItemRef(7, 0, "cntrId",
				MLB::VFast::DataType_UInt32).item_index_;
			cntrStatus_7        = ins_context.GetItemRef(7, 0, "cntrStatus",
				MLB::VFast::DataType_Ascii).item_index_;
			noOfEntries_7       = ins_context.GetItemRef(7, 0, "noOfEntries",
				MLB::VFast::DataType_UInt32).item_index_;
			updateAction_7      = ins_context.GetItemRef(7, 0, "updateAction",
				MLB::VFast::DataType_Ascii).item_index_;
			entryType_7         = ins_context.GetItemRef(7, 0, "entryType",
				MLB::VFast::DataType_Ascii).item_index_;
			entryPrcLvl_7       = ins_context.GetItemRef(7, 0, "entryPrcLvl",
				MLB::VFast::DataType_UInt32).item_index_;
			entryPrc_7          = ins_context.GetItemRef(7, 0, "entryPrc",
				MLB::VFast::DataType_Decimal).item_index_;
			entryQty_7          = ins_context.GetItemRef(7, 0, "entryQty",
				MLB::VFast::DataType_UInt32).item_index_;
			entryPrcSrc_7       = ins_context.GetItemRef(7, 0, "entryPrcSrc",
				MLB::VFast::DataType_Ascii).item_index_;
			seen_ins_context_ = true;
		}

		exc_results.PopulateExcEntryLookupList();

		if (exc_results.template_id_ == 6)
			return(PostDecodeMsg_6(exc_results, pfi_state));
		else if (exc_results.template_id_ == 7)
			return(PostDecodeMsg_7(exc_results, pfi_state));
		else
			std::cout
				<< std::setw(10) << pfi_state.pkt_index_ << " "
				<< std::setw( 5) << pfi_state.msg_index_ << " "
				<< std::setw( 5) << exc_results.template_id_
				<< std::endl;

		return(true);
	}

	bool PostDecodeMsg_6(ExcResults &exc_results,
		const MLB::VFast::PacketFileInterfaceState &pfi_state)
	{

		MLB::VFast::Type_UInt32  tmp_timestamp;
		MLB::VFast::Type_UInt32  tmp_srcId;
		MLB::VFast::Type_UInt32  tmp_seqNum;
		MLB::VFast::Type_UInt32  tmp_cntrId;
		char                     tmp_cntrStatus[VFastMaxStringLength + 1];
		MLB::VFast::Type_UInt32  tmp_noOfEntries;

		tmp_cntrStatus[0] = '\0';

		if (exc_results.LookupValueFirst(cntrId_6, tmp_cntrId, false) == NULL)
			return(true);

		//	Check for the specified contract identifiers...
		if ((!contract_id_set_.empty()) &&
			(contract_id_set_.find(tmp_cntrId) == contract_id_set_.end()))
			return(true);

		exc_results.LookupValueFirst(timestamp_6, tmp_timestamp, false);

		//	Check for the specified exchange timestamp range...
		if ((tmp_timestamp < first_exchange_time_) ||
			(tmp_timestamp > last_exchange_time_))
			return(true);

		exc_results.LookupValueFirst(srcId_6, tmp_srcId, false);
		exc_results.LookupValueFirst(seqNum_6, tmp_seqNum, false);
		exc_results.LookupValueFirst(cntrStatus_6, tmp_cntrStatus, true,
			sizeof(cntrStatus_6) - 1);
		exc_results.LookupValueFirst(noOfEntries_6, tmp_noOfEntries, true);

		if (!tmp_noOfEntries)
			return(true);
/*
		MLB::VFast::ExcEntryLookup lookup_updateAction =
			exc_results.lookup_list_[updateAction_6];
*/
		MLB::VFast::ExcEntryLookup lookup_entryType    =
			exc_results.lookup_list_[entryType_6];
		MLB::VFast::ExcEntryLookup lookup_entryPrcLvl  =
			exc_results.lookup_list_[entryPrcLvl_6];
		MLB::VFast::ExcEntryLookup lookup_entryPrc     =
			exc_results.lookup_list_[entryPrc_6];
		MLB::VFast::ExcEntryLookup lookup_entryQty     =
			exc_results.lookup_list_[entryQty_6];
		MLB::VFast::ExcEntryLookup lookup_entryPrcSrc  =
			exc_results.lookup_list_[entryPrcSrc_6];

		for (unsigned int count_1 = 0; count_1 < tmp_noOfEntries; ++count_1) {
/*
			char                     tmp_updateAction[VFastMaxStringLength + 1];
*/
			char                     tmp_entryType[VFastMaxStringLength + 1];
			MLB::VFast::Type_UInt32  tmp_entryPrcLvl;
			MLB::VFast::Type_Decimal tmp_entryPrc;
			MLB::VFast::Type_UInt32  tmp_entryQty;
			char                     tmp_entryPrcSrc[VFastMaxStringLength + 1];
/*
			exc_results.LookupValueAt(updateAction_6,
				lookup_updateAction.first_, tmp_updateAction, true,
				sizeof(tmp_updateAction) - 1);
*/
			exc_results.LookupValueAt(entryType_6,
				lookup_entryType.first_, tmp_entryType, true,
				sizeof(tmp_entryType) - 1);
			exc_results.LookupValueAt(entryPrcLvl_6,
				lookup_entryPrcLvl.first_, tmp_entryPrcLvl, true);
			exc_results.LookupValueAt(entryPrc_6,
				lookup_entryPrc.first_, tmp_entryPrc, true);
			exc_results.LookupValueAt(entryQty_6,
				lookup_entryQty.first_, tmp_entryQty, true);
			exc_results.LookupValueAt(entryPrcSrc_6,
				lookup_entryPrcSrc.first_, tmp_entryPrcSrc, true,
				sizeof(tmp_entryPrcSrc) - 1);
/*
			exc_results.MoveFirstToNext(lookup_updateAction);
*/
			exc_results.MoveFirstToNext(lookup_entryType);
			exc_results.MoveFirstToNext(lookup_entryPrcLvl);
			exc_results.MoveFirstToNext(lookup_entryPrc);
			exc_results.MoveFirstToNext(lookup_entryQty);
			exc_results.MoveFirstToNext(lookup_entryPrcSrc);
			if (emit_packet_location_)
				std::cout
					<< std::setw(10) << pfi_state.pkt_index_ << " "
					<< std::setw( 5) << pfi_state.msg_index_ << " "
						;
			std::cout
				<< std::setw( 5) << exc_results.template_id_ << " "
				<< std::setw(10) << tmp_timestamp    << " "
				<< std::setw( 5) << tmp_srcId        << " "
				<< std::setw( 8) << tmp_seqNum       << " "
				<< std::setw( 8) << tmp_cntrId       << " "
				<< std::setw( 6) << tmp_cntrStatus   << " "
				<< std::setw( 3) << tmp_noOfEntries  << " "
				<< std::setw( 3) << count_1          << " "
				<< std::setw( 6) << "N/A"            << " "

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
	bool PostDecodeMsg_7(ExcResults &exc_results,
		const MLB::VFast::PacketFileInterfaceState &pfi_state)
	{

		MLB::VFast::Type_UInt32  tmp_timestamp;
		MLB::VFast::Type_UInt32  tmp_srcId;
		MLB::VFast::Type_UInt32  tmp_seqNum;
		MLB::VFast::Type_UInt32  tmp_cntrId;
		char                     tmp_cntrStatus[VFastMaxStringLength + 1];
		MLB::VFast::Type_UInt32  tmp_noOfEntries;

		tmp_cntrStatus[0] = '\0';

		if (exc_results.LookupValueFirst(cntrId_7, tmp_cntrId, false) == NULL)
			return(true);

		//	Check for the specified contract identifiers...
		if ((!contract_id_set_.empty()) &&
			(contract_id_set_.find(tmp_cntrId) == contract_id_set_.end()))
			return(true);

		exc_results.LookupValueFirst(timestamp_7, tmp_timestamp, false);

		//	Check for the specified exchange timestamp range...
		if ((tmp_timestamp < first_exchange_time_) ||
			(tmp_timestamp > last_exchange_time_))
			return(true);

		exc_results.LookupValueFirst(srcId_7, tmp_srcId, false);
		exc_results.LookupValueFirst(seqNum_7, tmp_seqNum, false);
		exc_results.LookupValueFirst(cntrStatus_7, tmp_cntrStatus, true,
			sizeof(cntrStatus_7) - 1);
		exc_results.LookupValueFirst(noOfEntries_7, tmp_noOfEntries, true);

		if (!tmp_noOfEntries)
			return(true);
		MLB::VFast::ExcEntryLookup lookup_updateAction =
			exc_results.lookup_list_[updateAction_7];
		MLB::VFast::ExcEntryLookup lookup_entryType    =
			exc_results.lookup_list_[entryType_7];
		MLB::VFast::ExcEntryLookup lookup_entryPrcLvl  =
			exc_results.lookup_list_[entryPrcLvl_7];
		MLB::VFast::ExcEntryLookup lookup_entryPrc     =
			exc_results.lookup_list_[entryPrc_7];
		MLB::VFast::ExcEntryLookup lookup_entryQty     =
			exc_results.lookup_list_[entryQty_7];
		MLB::VFast::ExcEntryLookup lookup_entryPrcSrc  =
			exc_results.lookup_list_[entryPrcSrc_7];

		for (unsigned int count_1 = 0; count_1 < tmp_noOfEntries; ++count_1) {
			char                     tmp_updateAction[VFastMaxStringLength + 1];
			char                     tmp_entryType[VFastMaxStringLength + 1];
			MLB::VFast::Type_UInt32  tmp_entryPrcLvl;
			MLB::VFast::Type_Decimal tmp_entryPrc;
			MLB::VFast::Type_UInt32  tmp_entryQty;
			char                     tmp_entryPrcSrc[VFastMaxStringLength + 1];
			exc_results.LookupValueAt(updateAction_7,
				lookup_updateAction.first_, tmp_updateAction, true,
				sizeof(tmp_updateAction) - 1);
			exc_results.LookupValueAt(entryType_7,
				lookup_entryType.first_, tmp_entryType, true,
				sizeof(tmp_entryType) - 1);
			exc_results.LookupValueAt(entryPrcLvl_7,
				lookup_entryPrcLvl.first_, tmp_entryPrcLvl, true);
			exc_results.LookupValueAt(entryPrc_7,
				lookup_entryPrc.first_, tmp_entryPrc, true);
			exc_results.LookupValueAt(entryQty_7,
				lookup_entryQty.first_, tmp_entryQty, true);
			exc_results.LookupValueAt(entryPrcSrc_7,
				lookup_entryPrcSrc.first_, tmp_entryPrcSrc, true,
				sizeof(tmp_entryPrcSrc) - 1);
			exc_results.MoveFirstToNext(lookup_updateAction);
			exc_results.MoveFirstToNext(lookup_entryType);
			exc_results.MoveFirstToNext(lookup_entryPrcLvl);
			exc_results.MoveFirstToNext(lookup_entryPrc);
			exc_results.MoveFirstToNext(lookup_entryQty);
			exc_results.MoveFirstToNext(lookup_entryPrcSrc);
			if (emit_packet_location_)
				std::cout
					<< std::setw(10) << pfi_state.pkt_index_ << " "
					<< std::setw( 5) << pfi_state.msg_index_ << " "
						;
			std::cout
				<< std::setw( 5) << exc_results.template_id_ << " "
				<< std::setw(10) << tmp_timestamp    << " "
				<< std::setw( 5) << tmp_srcId        << " "
				<< std::setw( 8) << tmp_seqNum       << " "
				<< std::setw( 8) << tmp_cntrId       << " "
				<< std::setw( 6) << tmp_cntrStatus   << " "
				<< std::setw( 3) << tmp_noOfEntries  << " "
				<< std::setw( 3) << count_1          << " "
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
	bool                              emit_packet_location_;
	Type_UInt32                       first_exchange_time_;
	Type_UInt32                       last_exchange_time_;
	std::set<MLB::VFast::Type_UInt32> contract_id_set_;

	//	Snapshot fields...
	MLB::VFast::Type_UInt32           timestamp_6;
	MLB::VFast::Type_UInt32           srcId_6;
	MLB::VFast::Type_UInt32           seqNum_6;
	MLB::VFast::Type_UInt32           cntrId_6;
	MLB::VFast::Type_UInt32           cntrStatus_6;
	MLB::VFast::Type_UInt32           noOfEntries_6;
/*
	MLB::VFast::Type_UInt32           updateAction_6;
*/
	MLB::VFast::Type_UInt32           entryType_6;
	MLB::VFast::Type_UInt32           entryPrcLvl_6;
	MLB::VFast::Type_UInt32           entryPrc_6;
	MLB::VFast::Type_UInt32           entryQty_6;
	MLB::VFast::Type_UInt32           entryPrcSrc_6;

	//	Delta fields...
	MLB::VFast::Type_UInt32           timestamp_7;
	MLB::VFast::Type_UInt32           srcId_7;
	MLB::VFast::Type_UInt32           seqNum_7;
	MLB::VFast::Type_UInt32           cntrId_7;
	MLB::VFast::Type_UInt32           cntrStatus_7;
	MLB::VFast::Type_UInt32           noOfEntries_7;
	MLB::VFast::Type_UInt32           updateAction_7;
	MLB::VFast::Type_UInt32           entryType_7;
	MLB::VFast::Type_UInt32           entryPrcLvl_7;
	MLB::VFast::Type_UInt32           entryPrc_7;
	MLB::VFast::Type_UInt32           entryQty_7;
	MLB::VFast::Type_UInt32           entryPrcSrc_7;
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
	-progress 1000 -debug false -content_type EUREX -packet_format raw          -timer true -xml_file C:\DevEnv\Tests\FastCommon\Templates\CEF\CEF_template.20081112_A.xml -packet_file C:\DevEnv\Tests\FastCommon\PacketData\CEF\EEBS_samples_151008\EEBS_Simu_151008\EEBS_ODBDelta.dat  > C:\DevEnv\Tests\VFast\UnitTests\EbsOrderBookEventAnalyzer.CEF.EEBS_ODBDelta.dat.txt

	-progress 1000 -debug false -content_type EUREX -packet_format length       -timer true -xml_file C:\DevEnv\Tests\FastCommon\Templates\CEF\CEF_template.20081112_A.xml -packet_file C:\DevEnv\Tests\FastCommon\PacketData\CEF\20090122_A\CEF_Channel.___224.0.29.205___50000.bin > C:\DevEnv\Tests\VFast\UnitTests\EbsOrderBookEventAnalyzer.CEF.CEF_Channel.___224.0.29.205___50000.bin.txt

	From 2009-05-18:
	---- -----------
	-progress 1000 -debug false -content_type EUREX -packet_format length       -timer true -xml_file C:\DevEnv\Tests\FastCommon\Templates\CEF\CEF_template.20081112_A.xml -packet_file C:\DevEnv\Tests\FastCommon\PacketData\CEF\20090518_A\EBS_20090518_A.Delta___._224_0_29_25_50032.pkt > C:\DevEnv\Tests\VFast\UnitTests\EbsOrderBookEventAnalyzer.EBS_20090518_A.Delta___._224_0_29_25_50032.pkt.txt

	-progress 1000 -debug false -content_type EUREX -packet_format length       -timer true -xml_file C:\DevEnv\Tests\FastCommon\Templates\CEF\CEF_template.20081112_A.xml -packet_file C:\DevEnv\Tests\FastCommon\PacketData\CEF\20090518_A\EBS_20090518_A.Snapshot._224_0_29_24_50033.pkt > C:\DevEnv\Tests\VFast\UnitTests\EbsOrderBookEventAnalyzer.EBS_20090518_A.Snapshot._224_0_29_24_50033.pkt.txt

	-progress 1000 -debug false -content_type EUREX -packet_format length       -timer true -xml_file C:\DevEnv\Tests\FastCommon\Templates\CEF\CEF_template.20081112_A.xml -packet_file C:\DevEnv\Tests\FastCommon\PacketData\CEF\20090518_A\EBS_20090518_A.Snapshot.224_0_29_200_50001.pkt > C:\DevEnv\Tests\VFast\UnitTests\EbsOrderBookEventAnalyzer.EBS_20090518_A.Snapshot.224_0_29_200_50001.pkt.txt

	-progress 1000 -debug false -content_type EUREX -packet_format length       -timer true -xml_file C:\DevEnv\Tests\FastCommon\Templates\CEF\CEF_template.20081112_A.xml -packet_file C:\DevEnv\Tests\FastCommon\PacketData\CEF\20090518_A\EBS_20090518_A.Snapshot.224_0_29_204_50001.pkt > C:\DevEnv\Tests\VFast\UnitTests\EbsOrderBookEventAnalyzer.EBS_20090518_A.Snapshot.224_0_29_204_50001.pkt.txt
*/

