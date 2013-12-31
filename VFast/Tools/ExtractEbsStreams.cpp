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

		Copyright Michael L. Brock 2008 - 2014.
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

// ////////////////////////////////////////////////////////////////////////////

using namespace MLB::VFast;

namespace {
// ////////////////////////////////////////////////////////////////////////////
struct EbsProdRefStream {
	bool operator < (const EbsProdRefStream &other) const
	{
		int cmp;

		if ((cmp = ::memcmp(&streamAddr, &other.streamAddr,
			sizeof(streamAddr))) == 0) {
			if ((cmp = MLB::Utility::BasicCompare(streamPort,
				other.streamPort)) == 0) {
				if ((cmp == MLB::Utility::BasicCompare(streamType,
					other.streamType)) == 0) {
					if ((cmp == MLB::Utility::BasicCompare(streamService,
						other.streamService)) == 0)
						cmp = MLB::Utility::BasicCompare(mktDpth, other.mktDpth);
				}
			}
		}

		return(cmp < 0);
	}

	char                         streamType;
	char                         streamService;
	MLB::SocketIo::IpAddressType streamAddr;
	MLB::SocketIo::IpPortType    streamPort;
	unsigned int                 mktDpth;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const EbsProdRefStream &datum)
{
	o_str
		<< std::left
		<< std::setw(3 + 1 + 3 + 1 + 3 + 1 + 3) <<
			boost::asio::ip::address_v4(datum.streamAddr).to_string() << " "
		<< std::right
		<< std::setw(5) << datum.streamPort << " "
		<< datum.streamType << " "
		<< datum.streamService << " "
		<< std::setw(10) << datum.mktDpth
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class MyMsgHandlerFunc : public BaseMsgHandlerFunc {
	struct MyNullDeleter {
		void operator()(void const *) const {}
	};
public:
	typedef std::set<EbsProdRefStream> MySet;

	MyMsgHandlerFunc()
		:seen_ins_context_(false)
		,stream_set_()
		,streamType(0)
		,streamService(0)
		,streamAddr(0)
		,streamPort(0)
	{
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

	virtual bool PostDecodeMsg(PacketFileInterface & /* reader */,
      const PacketFileInterfaceState &pfi_state, ExcResults &exc_results)
	{
		if (exc_results.template_id_ != 3)
			return(true);

		if (!seen_ins_context_) {
			const InsContext &ins_context(pfi_state.ins_context_);
			noOfStreams       = ins_context.GetItemRef(3, 0, "noOfStreams",
				MLB::VFast::DataType_UInt32).item_index_;
			streamType        = ins_context.GetItemRef(3, 0, "streamType",
				MLB::VFast::DataType_Ascii).item_index_;
			streamService     = ins_context.GetItemRef(3, 0, "streamService",
				MLB::VFast::DataType_Ascii).item_index_;
			streamAddr        = ins_context.GetItemRef(3, 0, "streamAddr",
				MLB::VFast::DataType_Ascii).item_index_;
			streamPort        = ins_context.GetItemRef(3, 0, "streamPort",
				MLB::VFast::DataType_UInt32).item_index_;
			mktDpth           = ins_context.GetItemRef(3, 0, "mktDpth",
				MLB::VFast::DataType_UInt32).item_index_;
			seen_ins_context_ = true;
		}

		exc_results.PopulateExcEntryLookupList();

		MLB::VFast::Type_UInt32 stream_count;

		exc_results.LookupValueFirst(noOfStreams, stream_count, true);

		if (!stream_count)
			return(true);

		MLB::VFast::ExcEntryLookup lookup_streamType    =
			exc_results.lookup_list_[streamType];
		MLB::VFast::ExcEntryLookup lookup_streamService =
			exc_results.lookup_list_[streamService];
		MLB::VFast::ExcEntryLookup lookup_streamAddr    =
			exc_results.lookup_list_[streamAddr];
		MLB::VFast::ExcEntryLookup lookup_streamPort    =
			exc_results.lookup_list_[streamPort];
		MLB::VFast::ExcEntryLookup lookup_mktDpth       =
			exc_results.lookup_list_[mktDpth];

		for (unsigned int count_1 = 0; count_1 < stream_count; ++count_1) {
			char             tmp_string[MLB::VFast::VFastMaxStringLength + 1];
			EbsProdRefStream tmp_stream;
			exc_results.LookupValueAt(streamType,
				lookup_streamType.first_, tmp_string, true,
				sizeof(tmp_string) - 1);
			tmp_stream.streamType = *tmp_string;
			exc_results.LookupValueAt(streamService,
				lookup_streamService.first_, tmp_string, true,
				sizeof(tmp_string) - 1);
			tmp_stream.streamService = *tmp_string;
			exc_results.LookupValueAt(streamAddr,
				lookup_streamAddr.first_, tmp_string, true,
				sizeof(tmp_string) - 1);
			tmp_stream.streamAddr =
				boost::asio::ip::address_v4::from_string(tmp_string).to_ulong();
			exc_results.LookupValueAt(streamPort,
				lookup_streamPort.first_, tmp_stream.streamPort, true);
			exc_results.LookupValueAt(mktDpth,
				lookup_mktDpth.first_, tmp_stream.mktDpth, true);
			stream_set_.insert(tmp_stream);
			exc_results.MoveFirstToNext(lookup_streamType);
			exc_results.MoveFirstToNext(lookup_streamService);
			exc_results.MoveFirstToNext(lookup_streamAddr);
			exc_results.MoveFirstToNext(lookup_streamPort);
			exc_results.MoveFirstToNext(lookup_mktDpth);
		}

		return(true);
	}

	bool                    seen_ins_context_;
	MySet                   stream_set_;
	MLB::VFast::Type_UInt32 noOfStreams;
	MLB::VFast::Type_UInt32 streamType;
	MLB::VFast::Type_UInt32 streamService;
	MLB::VFast::Type_UInt32 streamAddr;
	MLB::VFast::Type_UInt32 streamPort;
	MLB::VFast::Type_UInt32 mktDpth;
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
		compat_check.ParseCmdLine(argc, argv);
		compat_check.Run(handler_func);
		std::copy(handler_func.stream_set_.begin(),
			handler_func.stream_set_.end(),
			std::ostream_iterator<EbsProdRefStream>(std::cout, "\n"));
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
	-debug true -content_type CME   -packet_format length                   -xml_file C:\DevEnv\Tests\FastCommon\Templates\CME\CME_templates.20080925_A.xml -packet_file C:\DevEnv\Tests\FastCommon\PacketData\CME\CME_PacketData.20080925_A.BIN.txt > C:\DevEnv\Tests\VFast\UnitTests\ExtractEbsStreams.CME.20080925_A.txt

	-debug true -content_type EUREX -packet_format raw          -timer true -xml_file C:\DevEnv\Tests\FastCommon\Templates\CEF\CEF_template.20081112_A.xml -packet_file C:\DevEnv\Tests\FastCommon\PacketData\CEF\EEBS_samples_151008\EEBS_Simu_151008\EEBS_ProdRefData_S.dat  > C:\DevEnv\Tests\VFast\UnitTests\ExtractEbsStreams.CEF.EEBS_ProdRefData_S.txt

	            -content_type EUREX -packet_format length       -timer true -xml_file C:\DevEnv\Tests\FastCommon\Templates\CEF\CEF_template.20081112_A.xml -packet_file C:\DevEnv\Tests\FastCommon\PacketData\CEF\20090123_A\CEF_SecDef.20090123_A.bin > C:\DevEnv\Tests\VFast\UnitTests\ExtractEbsStreams.CEF.CEF_SecDef.20090123_A.txt

	            -content_type EUREX -packet_format ipsourcetlen -timer true -xml_file C:\DevEnv\Tests\FastCommon\Templates\CEF\CEF_template.20081112_A.xml -packet_file C:\DevEnv\Tests\FastCommon\PacketData\CEF\20090121_A\NewComprehensiveCapture.CEF_20090121_A.ip_bin > C:\DevEnv\Tests\VFast\UnitTests\ExtractEbsStreams.NewComprehensiveCapture.CEF_20090121_A.ip_bin.txt
*/

