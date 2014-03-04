// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Generic interface to reading FAST packet files.

	Revision History	:	2009-04-11 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2009 - 2014.
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
#include <VFast/VFastParseCmdLineArg.hpp>
#include <VFast/VFastUtility.hpp>
#include <VFast/XdpSupport.hpp>

#include <SocketIo/PacketFileReaderTmp.hpp>

#include <Utility/StringSupport.hpp>
#include <Utility/InlineContainer.hpp>
#include <Utility/HRTimer.hpp>
#include <Utility/ParseCmdLineArg.hpp>
#include <Utility/RegexParamNameAdaptor.hpp>

#ifdef _Windows
# pragma warning(push)
# pragma warning(disable:4244 4245)
#endif // #ifdef _Windows

#include <boost/crc.hpp>

#ifdef _Windows
# pragma warning(pop)
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

namespace {
// ////////////////////////////////////////////////////////////////////////////
void EmitAddedPacketHeaderForXdp(const XdpAddedPacketHeader &added_header_type,
	std::size_t &packet_length, const StreamDatum *&begin_ptr,
	const StreamDatum *& /* end_ptr */, bool emit_header_flag)
{
	std::size_t added_packet_length = 0;
	char        SourceID[20 + 1];

	switch (added_header_type.packet_type_) {
		case     1:
			added_packet_length = sizeof(XdpAPH_SequenceNumberReset);
			CheckAddedPacketHeaderForXdp(added_header_type, added_packet_length,
				packet_length);
			if (emit_header_flag)
				std::cout << "[" << added_header_type.packet_type_name_ <<
					":NextSeqNumber=" << std::setw(10) <<
						reinterpret_cast<const XdpAPH_SequenceNumberReset *>(
						begin_ptr)->NextSeqNumber << "]";
			break;
		case     2:
			if (emit_header_flag)
				std::cout << "[" << added_header_type.packet_type_name_ << "]";
			break;
		case    10:
			added_packet_length = sizeof(XdpAPH_RetransmissionResponseMessage);
			CheckAddedPacketHeaderForXdp(added_header_type, added_packet_length,
				packet_length);
			MLB::Utility::nstrcpy(SourceID,
				reinterpret_cast<const XdpAPH_RetransmissionResponseMessage *>
				(begin_ptr)->SourceID, sizeof(SourceID) - 1);
			if (emit_header_flag)
				std::cout << "[" << added_header_type.packet_type_name_ <<
					":SourceSeqNum=" << std::setw(10) <<
						reinterpret_cast<const XdpAPH_RetransmissionResponseMessage *>
						(begin_ptr)->SourceSeqNum <<
					":SourceID=" << SourceID <<
					":Status=" <<
						reinterpret_cast<const XdpAPH_RetransmissionResponseMessage *>
						(begin_ptr)->Status <<
					":RejectReason=" << std::setw(3) <<
						static_cast<unsigned int>(
						reinterpret_cast<const XdpAPH_RetransmissionResponseMessage *>
						(begin_ptr)->RejectReason) <<
					"]";
			break;
		case    20:
			added_packet_length = sizeof(XdpAPH_RetransmissionRequestMessage);
			CheckAddedPacketHeaderForXdp(added_header_type, added_packet_length,
				packet_length);
			MLB::Utility::nstrcpy(SourceID,
				reinterpret_cast<const XdpAPH_RetransmissionRequestMessage *>
				(begin_ptr)->SourceID, sizeof(SourceID) - 1);
			if (emit_header_flag)
				std::cout << "[" << added_header_type.packet_type_name_ <<
					":BeginSeqNum=" << std::setw(10) <<
						reinterpret_cast<const XdpAPH_RetransmissionRequestMessage *>
						(begin_ptr)->BeginSeqNum <<
					":EndSeqNum=" << std::setw(10) <<
						reinterpret_cast<const XdpAPH_RetransmissionRequestMessage *>
						(begin_ptr)->EndSeqNum <<
					":SourceID=" << SourceID <<
					"]";
			break;
		case    22:
			added_packet_length = sizeof(XdpAPH_RefreshRequestMessage);
			CheckAddedPacketHeaderForXdp(added_header_type, added_packet_length,
				packet_length);
			MLB::Utility::nstrcpy(SourceID,
				reinterpret_cast<const XdpAPH_RefreshRequestMessage *>
				(begin_ptr)->SourceID, sizeof(SourceID) - 1);
			if (emit_header_flag)
				std::cout << "[" << added_header_type.packet_type_name_ <<
					":SourceID=" << SourceID <<
					"]";
			break;
		case    23:
			added_packet_length = sizeof(XdpAPH_RefreshResponseMessage);
			CheckAddedPacketHeaderForXdp(added_header_type, added_packet_length,
				packet_length);
			MLB::Utility::nstrcpy(SourceID,
				reinterpret_cast<const XdpAPH_RefreshResponseMessage *>
				(begin_ptr)->SourceID, sizeof(SourceID) - 1);
			if (emit_header_flag)
				std::cout << "[" << added_header_type.packet_type_name_ <<
					":SourceSeqNum=" << std::setw(10) <<
						reinterpret_cast<const XdpAPH_RefreshResponseMessage *>
						(begin_ptr)->SourceSeqNum <<
					":SourceID=" << SourceID <<
					":Status=" <<
						reinterpret_cast<const XdpAPH_RefreshResponseMessage *>
						(begin_ptr)->Status <<
					":RejectReason=" << std::setw(3) <<
						static_cast<unsigned int>(
						reinterpret_cast<const XdpAPH_RefreshResponseMessage *>
						(begin_ptr)->RejectReason) <<
					"]";
			break;
		case    24:
			added_packet_length = sizeof(XdpAPH_HeartbeatResponseMessage);
			CheckAddedPacketHeaderForXdp(added_header_type, added_packet_length,
				packet_length);
			MLB::Utility::nstrcpy(SourceID,
				reinterpret_cast<const XdpAPH_HeartbeatResponseMessage *>
				(begin_ptr)->SourceID, sizeof(SourceID) - 1);
			if (emit_header_flag)
				std::cout << "[" << added_header_type.packet_type_name_ <<
					":SourceID=" << SourceID <<
					"]";
			break;
		case   799:
			break;
		default   :
			break;
	}

	packet_length -= added_packet_length;
	begin_ptr     += added_packet_length;
}
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
void BaseMsgHandlerFunc::PreParseCmdLine(PacketFileInterface & /* reader */,
	int /* argc */, char ** /* argv */)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool BaseMsgHandlerFunc::ThisParseCmdLine(PacketFileInterface & /* reader */,
	int /* argc */, char ** /* argv */, unsigned int & /* current_index */)
{
	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void BaseMsgHandlerFunc::PostParseCmdLine(PacketFileInterface & /* reader */,
	int /* argc */, char ** /* argv */)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void BaseMsgHandlerFunc::PreIterLoop(PacketFileInterface & /* reader */)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void BaseMsgHandlerFunc::PostIterLoop(PacketFileInterface & /* reader */,
	const PacketFileInterfaceState & /* pfi_state */)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool BaseMsgHandlerFunc::PreDecodePkt(PacketFileInterface &reader,
	PacketFileInterfaceState &pfi_state, const StreamDatum *&begin_ptr,
	const StreamDatum *&end_ptr)
{
	if (reader.content_type_ == PacketDecodeRegime_CME_2_0) {
		std::size_t packet_length = static_cast<std::size_t>(end_ptr - begin_ptr);
		if (packet_length < CmePacketHeader2_0::PacketHeaderLength) {
			std::ostringstream o_str;
			o_str << "The length of the packet (" << packet_length << ") is less "
				"than the size of the packet header (" <<
				CmePacketHeader2_0::PacketHeaderLength << ").";
			MLB::Utility::ThrowLogicError(o_str.str());
		}
		pfi_state.pkt_header_ptr_     = begin_ptr;
		pfi_state.pkt_header_length_  = CmePacketHeader2_0::PacketHeaderLength;
		begin_ptr                    += CmePacketHeader2_0::PacketHeaderLength;
		if (reader.packet_header_flag_) {
			pfi_state.cme_packet_header_ptr_ =
				reinterpret_cast<CmePacketHeader2_0 *>(
				const_cast<void *>(pfi_state.pkt_header_ptr_));
			pfi_state.cme_packet_header_ = *pfi_state.cme_packet_header_ptr_;
			pfi_state.cme_packet_header_.NetToHostOrder();
			std::cout << "********** HEADER [" << pfi_state.cme_packet_header_ <<
				"]";
		}
	}
	else if (reader.content_type_ == PacketDecodeRegime_XDP) {
		std::size_t packet_length = static_cast<std::size_t>(end_ptr - begin_ptr);
		if (packet_length < XdpPacketHeader::PacketHeaderLength) {
			std::ostringstream o_str;
			o_str << "The length of the packet (" << packet_length << ") is less "
				"than the size of the packet header (" <<
				XdpPacketHeader::PacketHeaderLength << ").";
			MLB::Utility::ThrowLogicError(o_str.str());
		}
		pfi_state.pkt_header_ptr_         = begin_ptr;
		pfi_state.pkt_header_length_      = XdpPacketHeader::PacketHeaderLength;
		begin_ptr                        += XdpPacketHeader::PacketHeaderLength;
		pfi_state.xdp_packet_header_ptr_  = reinterpret_cast<XdpPacketHeader *>(
			const_cast<void *>(pfi_state.pkt_header_ptr_));
		pfi_state.xdp_packet_header_      = *pfi_state.xdp_packet_header_ptr_;
		pfi_state.xdp_packet_header_.NetToHostOrder();
		const XdpAddedPacketHeader *added_header_type =
			GetXdpAddedPacketHeaderInfoPtr(pfi_state.xdp_packet_header_.PacketType,
			true);
		packet_length -= XdpPacketHeader::PacketHeaderLength;
		if (reader.packet_header_flag_) {
			std::cout << "********** HEADER [" << pfi_state.xdp_packet_header_ <<
				"]";
		}
		EmitAddedPacketHeaderForXdp(*added_header_type, packet_length, begin_ptr,
			end_ptr, reader.packet_header_flag_);
	}
	else {
		pfi_state.pkt_header_ptr_    = begin_ptr;
		pfi_state.pkt_header_length_ = 0;
		if (reader.packet_header_flag_)
			std::cout << "********** HEADER []";
	}


	if (reader.packet_header_flag_)
		std::cout << std::endl;

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool BaseMsgHandlerFunc::PreDecodeMsgDebug(PacketFileInterface &reader,
	const PacketFileInterfaceState &pfi_state)
{
	if (reader.debug_flag_) {
		std::cout
			 << "********** PACKET [";
		if (!reader.output_compatible_flag_)
			std::cout << MLB::Utility::TimeSpec(pfi_state.time_secs_,
				pfi_state.time_nsecs_) << "][";
		std::cout
			<< std::setw(10) << pfi_state.all_index_ << "]["
			<< std::setw(10) << pfi_state.pkt_index_ << "]["
			<< std::setw(10) << pfi_state.msg_index_ << "]" << std::endl;
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool BaseMsgHandlerFunc::PreDecodeMsg(PacketFileInterface & /* reader */,
	const PacketFileInterfaceState & /* pfi_state */)
{
	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool BaseMsgHandlerFunc::PostDecodeMsgDebug(PacketFileInterface &reader,
	const PacketFileInterfaceState &pfi_state, ExcResults &exc_results)
{
	if (!reader.debug_flag_)
		return(true);

	std::cout << "PMap Byte Count = " <<
		pfi_state.exc_context_.GetLastPMapLength() << std::endl;
	std::cout << "Template Id     = " <<
		pfi_state.exc_context_.GetLastTemplateId() <<
		std::endl;

	exc_results.EmitListDebug();

	std::cout << "Value items = " << exc_results.entry_count_ << std::endl;

	exc_results.EmitList();

	std::cout << "********** PACKET [";
	if (!reader.output_compatible_flag_)
		std::cout
			<< MLB::Utility::TimeSpec(pfi_state.time_secs_, pfi_state.time_nsecs_)
			<< "][";
	std::cout
		<< std::setw(10) << pfi_state.all_index_ << "]["
		<< std::setw(10) << pfi_state.pkt_index_ << "]["
		<< std::setw(10) << pfi_state.msg_index_ << "][SIZE = "
		<< std::setw(10) << pfi_state.exc_context_.GetLastSrcDataLength()
		<< "][" << ToHexString(pfi_state.msg_begin_ptr_, pfi_state.msg_end_ptr_)
		<< "]" << std::endl;

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool BaseMsgHandlerFunc::PostDecodeMsgChecksum(PacketFileInterface &reader,
	const PacketFileInterfaceState &pfi_state, ExcResults &exc_results)
{
	if (!reader.checksum_flag_)
		return(true);

	std::ostringstream o_str;

	exc_results.EmitList(o_str);

	boost::crc_32_type tmp_crc32;

	tmp_crc32.process_bytes(o_str.str().c_str(), o_str.str().size());

	std::cout << "********** CHECKSUMMED PACKET [";
	std::cout << MLB::Utility::TimeSpec(pfi_state.time_secs_,
		pfi_state.time_nsecs_) << "][";
	std::cout
		<< std::setw(10) << pfi_state.all_index_ << "]["
		<< std::setw(10) << pfi_state.pkt_index_ << "]["
		<< std::setw(10) << pfi_state.msg_index_ << "][SIZE = "
		<< std::setw(10) << pfi_state.exc_context_.GetLastSrcDataLength()
		<< "][" << std::setfill('0') << std::hex << std::setw(8)
		<< tmp_crc32.checksum()
		<< "]" << std::dec << std::setfill(' ') << std::endl;

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool BaseMsgHandlerFunc::PostDecodeMsg(
	MLB::VFast::PacketFileInterface & /* reader */,
	const PacketFileInterfaceState & /* pfi_state */,
	ExcResults & /* exc_results */)
{
	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void BaseMsgHandlerFunc::DecodeMsgException(PacketFileInterface & /* reader */,
	const PacketFileInterfaceState &pfi_state, const ExcResults &exc_results,
	const std::exception &except)
{
	std::cout << "PMap Byte Count = " <<
		pfi_state.exc_context_.GetLastPMapLength() << std::endl;
	std::cout << "Template Id     = " <<
		pfi_state.exc_context_.GetLastTemplateId() << std::endl;

	if (exc_results.entry_count_)
		exc_results.EmitListDebug();
	else {
		ExcResults tmp_exc_results;
		pfi_state.exc_context_.GetDecodeResults(tmp_exc_results);
		tmp_exc_results.EmitListDebug();
	}

	std::size_t byte_length =
		static_cast<std::size_t>(pfi_state.msg_end_ptr_ -
		pfi_state.msg_begin_ptr_);

	std::cout << "RAW BYTES  : [" <<
		ToHexString(std::min<std::size_t>(byte_length, 128),
		pfi_state.msg_begin_ptr_) << "]" << std::endl;

	std::cout << "***** ERROR: " << except.what() << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketFileInterfaceState::PacketFileInterfaceState(
	const std::string &xml_file_name, const ExcContextInfo &exc_context_info)
	:all_index_(0)
	,pkt_index_(0)
	,msg_index_(0)
	,pkt_begin_ptr_(NULL)
	,pkt_end_ptr_(NULL)
	,msg_begin_ptr_(NULL)
	,msg_end_ptr_(NULL)
	,pkt_header_ptr_(NULL)
	,pkt_header_length_(0)
	,ip_address_(0)
	,ip_port_(0)
	,time_secs_(0)
	,time_nsecs_(0)
	,ins_context_(xml_file_name)
	,exc_context_(ins_context_, exc_context_info)
	,cme_packet_header_ptr_(NULL)
	,xdp_packet_header_ptr_(NULL)
	,cme_packet_header_()
	,xdp_packet_header_()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketFileInterface::PacketFileInterface()
	:xml_file_name_()
	,packet_file_name_()
	,content_type_(PacketDecodeRegime_None)
	,packet_format_(MLB::SocketIo::PacketFormat_Length)
	,first_packet_index_(0)
	,max_message_count_(0)
	,iteration_count_(1)
	,progress_counter_(false)
	,xml_flag_(false)
	,fix_flag_(false)
	,debug_flag_(false)
	,output_compatible_flag_(false)
	,source_flag_(false)
	,checksum_flag_(false)
	,packet_header_flag_(false)
	,output_header_flag_(true)
	,output_trailer_flag_(true)
	,timer_flag_(true)
	,cancel_packets_flag_(false)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PacketFileInterface::~PacketFileInterface()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketFileInterface::Run()
{
	BaseMsgHandlerFunc msg_handler_func;

	Run(msg_handler_func);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketFileInterface::Run(BaseMsgHandlerFunc &msg_handler_func)
{
	Native_UInt64 nsecs_start = 0;
	Native_UInt64 nsecs_end   = std::numeric_limits<Native_UInt64>::max();

/*
	CODE NOTE: The parameterization of this class is to be enhanced to support
	the specification of a timestamp range for the packets to be examined. Until
	that's implemented, you can modifiy this file with lines such as the
	following to achieve that result.

	In this example, we restrict packets to those with timestamps within the
	three-second range beginning at 12:45:43 UTC/GMT on 2012-02-15.

	nsecs_start = MLB::Utility::TimeSpec("2012-02-15 12:45:43.000000000").ToNanoseconds();
	nsecs_end   = MLB::Utility::TimeSpec("2012-02-15 12:45:45.999999999").ToNanoseconds();
*/

	PacketFileInterfaceState pfi_state(xml_file_name_,
		ExcContextInfo(content_type_ == PacketDecodeRegime_EBS));

	std::cerr << "Loading message packet file '" << packet_file_name_ <<
		"'..." << std::flush;

	MLB::SocketIo::PacketFileReaderTmp msg_packet_file(packet_file_name_,
		packet_format_);

	std::cerr << " done (total file size " << msg_packet_file.GetFileSize() <<
		")." << std::endl;

	if (output_header_flag_) {
		std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
		std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
		std::cout << "First packet index: " << first_packet_index_ << std::endl;
		std::cout << "Total iterations  : " << iteration_count_ << std::endl;
		std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
	}

	unsigned long long first_packet_offset = 0;

	if (first_packet_index_) {
		if (!msg_packet_file.SeekToPacket(first_packet_index_)) {
			std::cout << "Specified first packet index (" <<
				first_packet_index_ << ") is beyond the end of the file. Done." <<
				std::endl;
			return;
		}
		first_packet_offset = msg_packet_file.GetPacketOffset();
	}

	unsigned int          count_1;
	unsigned long long    processed_count = 0;
	unsigned long long    failure_count   = 0;
	MLB::Utility::HRTimer hr_timer;

	if (progress_counter_)
		std::cerr
			<< "["
			<< std::setw(18) << 0 << "]["
			<< std::setw(18) << 0 << "]["
			<< std::setw(10) << 0 << "]["
			<< std::setw(10) << 0 << "]["
			<< std::setw(10) << 0 << "]"
			<< std::flush;

	msg_handler_func.PreIterLoop(*this);

	hr_timer.Start();

	for (count_1 = 0; count_1 < iteration_count_; ++count_1) {
		msg_packet_file.SeekToOffset(first_packet_offset);
unsigned int this_msg_count = 0;
		pfi_state.all_index_ = 0;
		pfi_state.pkt_index_ = 0;
		for ( ; ; ++pfi_state.pkt_index_) {
			pfi_state.msg_index_ = 0;
			bool                          use_previous_id = false;
			unsigned int                  template_id     = 0;
			const StreamDatum            *begin_ptr       = NULL;
			const StreamDatum            *end_ptr         = NULL;
			unsigned long long            packet_offset;
			packet_offset = msg_packet_file.GetFileOffset();
			if (!msg_packet_file.GetPacketTyped(begin_ptr, end_ptr,
				pfi_state.ip_address_, pfi_state.ip_port_, pfi_state.time_secs_,
				pfi_state.time_nsecs_))
				break;
			Native_UInt64 nsecs_packet =
				(static_cast<Native_UInt64>(pfi_state.time_secs_) *
				static_cast<Native_UInt64>(1000000000)) +
				static_cast<Native_UInt64>(pfi_state.time_nsecs_);
			if (nsecs_packet < nsecs_start)
				continue;
			else if (nsecs_packet > nsecs_end)
				break;
/*
	CODE NOTE: The parameterization of this class is to be enhanced to support
	the specification of one or more multicast groups for the packets to be
	examined. Until that's implemented, you can modifiy this file with lines
	such as the following to achieve that result.

	In this example, we restrict packets to those which arrived on the Eurex
	production data FDAX A channel (which at the time of this writing was
	224.0.29.205 on port 50000):

	//	Only packets on 3758104013 = E0.00.1D.CD = 224.0.29.205 = FDAX A
	if ((pfi_state.ip_address_ != 3758104013) || (pfi_state.ip_port_ != 50000))
		continue;
*/

//std::cout << "********* HERE:[" << pfi_state.ip_address_ << "][" << pfi_state.ip_port_ << "]" << std::endl;

			pfi_state.pkt_begin_ptr_ = begin_ptr;
			pfi_state.pkt_end_ptr_   = end_ptr;
			bool process_packet =
				msg_handler_func.PreDecodePkt(*this, pfi_state, begin_ptr, end_ptr);
			if ((!process_packet) || (begin_ptr == end_ptr)) {
				msg_handler_func.PreDecodeMsg(*this, pfi_state);
				msg_handler_func.PreDecodeMsgDebug(*this, pfi_state);
				if (source_flag_)
					std::cout << "********** SOURCE: [OFFSET = " <<
						std::setw(22) << packet_offset << "][IP = " <<
						std::setw(3 + 1 + 3 + 1 + 3 + 1 + 3) <<
						boost::asio::ip::address_v4(pfi_state.ip_address_) <<
						"/" << std::setw(5) << pfi_state.ip_port_ << "]" <<
						std::endl;
				ExcResults exc_results;
				if (debug_flag_) {
					std::cout << "PMap Byte Count = N/A" << std::endl;
					std::cout << "Template Id     = N/A" << std::endl;
					std::cout << "Value items = " << 0 << std::endl;
					std::cout << "********** PACKET [";
					if (output_compatible_flag_)
						std::cout << MLB::Utility::TimeSpec(pfi_state.time_secs_,
							pfi_state.time_nsecs_) << "][";
					std::cout
						<< std::setw(10) << pfi_state.all_index_ << "]["
						<< std::setw(10) << pfi_state.pkt_index_ << "]["
						<< std::setw(10) << pfi_state.msg_index_ << "][SIZE = "
						<< std::setw(10) << 0 << "][]" << std::endl;
				}
				if (checksum_flag_) {
					std::cout << "********** CHECKSUMMED PACKET [";
					if (output_compatible_flag_)
						std::cout << MLB::Utility::TimeSpec(pfi_state.time_secs_,
							pfi_state.time_nsecs_) << "][";
					std::cout
						<< std::setw(10) << pfi_state.all_index_ << "]["
						<< std::setw(10) << pfi_state.pkt_index_ << "]["
						<< std::setw(10) << pfi_state.msg_index_ << "][SIZE = "
						<< std::setw(10) << 0 << "][" << std::setfill('0')
						<< std::hex << std::setw(8) << 0 << "]" << std::dec
						<< std::setfill(' ') << std::endl;
				}
				++pfi_state.all_index_;
				if (xml_flag_) {
					std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
					exc_results.EmitXml();
					std::cout << std::endl;
				}
				if (fix_flag_) {
					std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
					std::cout << "********** PACKET [";
					if (output_compatible_flag_)
						std::cout << MLB::Utility::TimeSpec(pfi_state.time_secs_,
							pfi_state.time_nsecs_) << "][";
					std::cout
						<< std::setw(10) << pfi_state.all_index_ << "]["
						<< std::setw(10) << pfi_state.pkt_index_ << "]["
						<< std::setw(10) << pfi_state.msg_index_ << "][SIZE = "
						<< std::setw(10) << 0 << "]";
					exc_results.EmitFix();
					std::cout << std::endl;
				}
				if (debug_flag_)
					std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
				++pfi_state.msg_index_;
++this_msg_count;
				++processed_count;
				if ((progress_counter_) && (!(processed_count % progress_counter_)))
					std::cerr
						<< std::setfill('\b')
						<< std::setw(1 + 18 + 2 + 18 + 2 + 10 + 2 + 10 + 2 + 10 + 1)
						<< "" << std::setfill(' ')
						<< "["
						<< std::setw(18) << processed_count << "]["
						<< std::setw(18) << failure_count   << "]["
						<< std::setw(10) << pfi_state.all_index_       << "]["
						<< std::setw(10) << pfi_state.pkt_index_       << "]["
						<< std::setw(10) << pfi_state.msg_index_       << "]"
						<< std::flush;
			}
			while (begin_ptr < end_ptr) {
				ExcResults exc_results;
				bool       core_parse_done   = false;
				bool       debug_header_done = false;
				try {
					pfi_state.msg_begin_ptr_ = begin_ptr;
					pfi_state.msg_end_ptr_   = end_ptr;
					if (!msg_handler_func.PreDecodeMsg(*this, pfi_state))
						break;
					if (!msg_handler_func.PreDecodeMsgDebug(*this, pfi_state))
						break;
					debug_header_done = true;
					const StreamDatum *new_begin_ptr =
						pfi_state.exc_context_.FieldDecode(begin_ptr, end_ptr,
						exc_results);
					core_parse_done        = true;
					pfi_state.msg_end_ptr_ = new_begin_ptr;
					template_id = pfi_state.exc_context_.GetLastTemplateId();
					if (source_flag_)
						std::cout << "********** SOURCE: [OFFSET = " <<
							std::setw(22) << packet_offset << "][IP = " <<
							std::setw(3 + 1 + 3 + 1 + 3 + 1 + 3) <<
							boost::asio::ip::address_v4(pfi_state.ip_address_) <<
							"/" << std::setw(5) << pfi_state.ip_port_ << "]" <<
							std::endl;
					if (!msg_handler_func.PostDecodeMsg(*this, pfi_state,
						exc_results))
						break;
					if (!msg_handler_func.PostDecodeMsgDebug(*this, pfi_state,
						exc_results))
						break;
					if (!msg_handler_func.PostDecodeMsgChecksum(*this, pfi_state,
						exc_results))
						break;
					++pfi_state.all_index_;
					if (xml_flag_) {
						std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
						exc_results.EmitXml();
						std::cout << std::endl;
					}
					if (fix_flag_) {
						std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
						std::cout << "********** PACKET [";
						std::cout << MLB::Utility::TimeSpec(pfi_state.time_secs_,
							pfi_state.time_nsecs_) << "][";
						std::cout
							<< std::setw(10) << pfi_state.all_index_ << "]["
							<< std::setw(10) << pfi_state.pkt_index_ << "]["
							<< std::setw(10) << pfi_state.msg_index_ << "][SIZE = "
							<< std::setw(10) << 0 << "]";
						exc_results.EmitFix();
						std::cout << std::endl;
					}
					if ((content_type_ == PacketDecodeRegime_CME) ||
						 (content_type_ == PacketDecodeRegime_CME_2))
						pfi_state.exc_context_.ResetLastTemplateDictionary();
					else if (content_type_ == PacketDecodeRegime_ISE) {
						if (template_id == 120) {
							pfi_state.exc_context_.ResetDictionaries();
							use_previous_id = false;
						}
						else
							use_previous_id = true;
					}
					else if (content_type_ == PacketDecodeRegime_EBS) {
						if (template_id == 120)
							pfi_state.exc_context_.ResetDictionaries();
					}
					begin_ptr = new_begin_ptr;
				}
				catch (const std::exception &except) {
					msg_handler_func.DecodeMsgException(*this, pfi_state,
						exc_results, except);
					++failure_count;
					if (!core_parse_done)
						begin_ptr = end_ptr;	//	Have to skip the rest of the packet.
				}
				if (debug_flag_)
					std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
				++pfi_state.msg_index_;
++this_msg_count;
				++processed_count;
				if ((progress_counter_) && (!(processed_count % progress_counter_)))
					std::cerr
						<< std::setfill('\b')
						<< std::setw(1 + 18 + 2 + 18 + 2 + 10 + 2 + 10 + 2 + 10 + 1)
						<< "" << std::setfill(' ')
						<< "["
						<< std::setw(18) << processed_count << "]["
						<< std::setw(18) << failure_count   << "]["
						<< std::setw(10) << pfi_state.all_index_       << "]["
						<< std::setw(10) << pfi_state.pkt_index_       << "]["
						<< std::setw(10) << pfi_state.msg_index_       << "]"
						<< std::flush;
				if (max_message_count_ && (this_msg_count >= max_message_count_))
					break;
			}
			if (content_type_ == PacketDecodeRegime_XDP)
				pfi_state.exc_context_.ResetDictionaries();
			if (cancel_packets_flag_)
				break;
			if (max_message_count_ && (this_msg_count >= max_message_count_))
				break;
		}
		if (progress_counter_)
			std::cerr
				<< std::setfill('\b')
				<< std::setw(1 + 18 + 2 + 18 + 2 + 10 + 2 + 10 + 2 + 10 + 1) << ""
				<< std::setfill(' ')
				<< "["
				<< std::setw(18) << processed_count << "]["
				<< std::setw(18) << failure_count   << "]["
				<< std::setw(10) << pfi_state.all_index_       << "]["
				<< std::setw(10) << pfi_state.pkt_index_       << "]["
				<< std::setw(10) << pfi_state.msg_index_       << "]"
				<< std::flush;
	}

	hr_timer.End();

	msg_handler_func.PostIterLoop(*this, pfi_state);

	if (progress_counter_)
		std::cerr
			<< std::setfill('\b')
			<< std::setw(1 + 18 + 2 + 18 + 2 + 10 + 2 + 10 + 2 + 10 + 1) << ""
			<< std::setfill(' ')
			<< "["
			<< std::setw(18) << processed_count << "]["
			<< std::setw(18) << failure_count   << "]["
			<< std::setw(10) << pfi_state.all_index_       << "]["
			<< std::setw(10) << pfi_state.pkt_index_       << "]["
			<< std::setw(10) << pfi_state.msg_index_       << "]"
			<< std::endl;

	if (output_trailer_flag_) {
		std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
		std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
		std::cout << "First Packet Index     : " << std::setw(10) <<
			first_packet_index_ << std::endl;
		std::cout << "Total Packets Parsed   : " << std::setw(10) <<
			processed_count << std::endl;
		std::cout << "Number of Failed Parses: " << std::setw(10) <<
			failure_count<< std::endl;
		if (timer_flag_) {
			std::cout << "Elapsed time           : " << std::setprecision(16) <<
				hr_timer.GetTickDiffInSeconds() << std::endl;
			std::cout << "Time per packet        : " <<
				(hr_timer.GetTickDiffInSeconds() /
				static_cast<double>(processed_count)) << std::endl;
		}
		std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
		std::cout << MLB::Utility::PadLeft("", 79, '*') << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketFileInterface::ParseCmdLine(int argc, char **argv)
{
	BaseMsgHandlerFunc msg_handler_func;

	ParseCmdLine(msg_handler_func, argc, argv);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PacketFileInterface::ParseCmdLine(BaseMsgHandlerFunc &msg_handler_func,
	int argc, char **argv)
{
	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			GetHelpStringShort(false) << std::endl << std::endl <<
"\
   -help\n\
      Produces this output.\n\n" <<
			GetHelpStringLong() << std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}

	unsigned int count_1;
	std::string  tmp_string;

	msg_handler_func.PreParseCmdLine(*this, argc, argv);

	for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
		if (!ParseCmdSingle(msg_handler_func, count_1, argc, argv))
			MLB::Utility::ParseCmdLineArg::InvalidArgument(argv[count_1]);
	}

	msg_handler_func.PostParseCmdLine(*this, argc, argv);

	if (xml_file_name_.empty())
		MLB::Utility::ThrowInvalidArgument("The XML template file name was "
			"not specified.");

	if (packet_file_name_.empty())
		MLB::Utility::ThrowInvalidArgument("The message packet file name was "
			"not specified.");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PacketFileInterface::ParseCmdSingle(unsigned int &current_index, int argc,
	char **argv)
{
	BaseMsgHandlerFunc msg_handler_func;

	return(ParseCmdSingle(msg_handler_func, current_index, argc, argv));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Defined for use only within method PacketFileInterface::ParseCmdSingle().
#ifdef INTERNAL_VFAST_PFI_CHECK_CONFLICT
# error "Unexpected definition of preprocessor macro 'INTERNAL_VFAST_PFI_CHECK_CONFLICT' encountered."
#endif // #ifdef INTERNAL_VFAST_PFI_CHECK_CONFLICT

#define INTERNAL_VFAST_PFI_CHECK_CONFLICT(f_name, f_val, good_val)			\
	if (f_val != good_val)	{																\
		std::ostringstream o_str;															\
		o_str << "'-" << f_name << "' flag (" << AnyToString(f_val) << ").";	\
		MLB::Utility::ThrowInvalidArgument(o_str.str());							\
	}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PacketFileInterface::ParseCmdSingle(BaseMsgHandlerFunc &msg_handler_func,
	unsigned int &current_index, int argc, char **argv)
{
	using namespace MLB::Utility;

	std::string tmp_string;

	msg_handler_func.PreParseCmdLine(*this, argc, argv);

	if (msg_handler_func.ThisParseCmdLine(*this, argc, argv, current_index))
		;
	else if (VFastParseCmdLineArg::ParseCmdLineXmlTemplateFile(current_index,
		argc, argv, xml_file_name_))
		;
	else if (ParseCmdLineArg::ParseCmdLineFollowingSpec(
		MLB::Utility::MakeInlineVector<std::string>
			("-PACKET_FILE_NAME")
			("-PACKET_FILENAME")
			("-PACKETFILE_NAME")
			("-PACKETFILENAME")
			("-PACKET_FILE")
			("-PACKETFILE"), current_index, argc, argv, packet_file_name_))
		;
	else if (VFastParseCmdLineArg::ParseCmdLinePacketDecodeRegime(current_index,
		argc, argv, content_type_))
		;
	else if (MLB::SocketIo::SocketIoParseCmdLineArg::ParseCmdLinePacketFormat(
		current_index, argc, argv, packet_format_))
		;
	else if (VFastParseCmdLineArg::ParseCmdLineFirstPacketIndex(current_index,
		argc, argv, first_packet_index_))
		;
	//	CODE NOTE: At present packets and messages are synonomous.
	else if (VFastParseCmdLineArg::ParseCmdLineMaxPacketCount(current_index,
		argc, argv, max_message_count_))
		;
	//	CODE NOTE: At present packets and messages are synonomous.
	else if (VFastParseCmdLineArg::ParseCmdLineMaxMessageCount(current_index,
		argc, argv, max_message_count_))
		;
	else if (VFastParseCmdLineArg::ParseCmdLineIterationCount(current_index,
		argc, argv, iteration_count_))
		;
	else if (ParseCmdLineArg::ParseCmdLineNumeric(
		MLB::Utility::MakeInlineVector<std::string>
		("-PROGRESSION_COUNTER")
		("-PROGRESSIONCOUNTER")
		("-PROGRESSION_COUNT")
		("-PROGRESSIONCOUNT")
		("-PROGRESS_COUNTER")
		("-PROGRESSCOUNTER")
		("-PROGRESS_COUNT")
		("-PROGRESSCOUNT")
		("-PROGRESSION")
		("-PROGRESS"),
		current_index, argc, argv, progress_counter_))
		;
	else if (ParseCmdLineArg::ParseCmdLineDatumSpec(
		MLB::Utility::MakeInlineVector<std::string>
		("-XML_FLAG")
		("-XMLFLAG")
		("-XML")
			,
		current_index, argc, argv, xml_flag_))
		;
	else if (ParseCmdLineArg::ParseCmdLineDatumSpec(
		MLB::Utility::MakeInlineVector<std::string>
		("-FIX_FLAG")
		("-FIXFLAG")
		("-FIX")
			,
		current_index, argc, argv, fix_flag_))
		;
	else if (VFastParseCmdLineArg::ParseCmdLineDebugFlag(current_index,
		argc, argv, debug_flag_))
		;
	else if (ParseCmdLineArg::ParseCmdLineDatum(
		RegexParamNameAdaptor(
		"^\\-\\-*(DEBUG_*)*OUTPUT_*COMPAT((IBLE)|(IBILITY))*$",
		current_index, argc, argv),
		current_index, argc, argv, output_compatible_flag_))
		;
	else if (ParseCmdLineArg::ParseCmdLineDatumSpec(
		MLB::Utility::MakeInlineVector<std::string>
		("-SOURCES_FLAG")
		("-SOURCESFLAG")
		("-SOURCE_FLAG")
		("-SOURCEFLAG")
		("-SRCS_FLAG")
		("-SRCSFLAG")
		("-SRC_FLAG")
		("-SRCFLAG")
		("-SOURCES")
		("-SOURCE")
		("-SRCS")
		("-SRC"),
		current_index, argc, argv, source_flag_))
		;
	else if (ParseCmdLineArg::ParseCmdLineDatumSpec(
		MLB::Utility::MakeInlineVector<std::string>
		("-CHECK_SUM")
		("-CHECKSUM")
		("-CHK_SUM")
		("-CHKSUM"),
		current_index, argc, argv, checksum_flag_))
		;
	else if (ParseCmdLineArg::ParseCmdLineDatum(
		RegexParamNameAdaptor(
		"^\\-(((EMIT)|(OUTPUT))_*)*((PACKET)|(PKT))S*_*HEAD(ER)*S*$",
		current_index, argc, argv),
		current_index, argc, argv, packet_header_flag_))
		;
	else if (ParseCmdLineArg::ParseCmdLineDatum(
		RegexParamNameAdaptor(
		"^\\-(((EMIT)|(OUTPUT))_*)*HEAD(ER)*S*$",
		current_index, argc, argv),
		current_index, argc, argv, output_header_flag_))
		;
	else if (ParseCmdLineArg::ParseCmdLineDatum(
		RegexParamNameAdaptor(
		"^\\-(((EMIT)|(OUTPUT))_*)*TRAIL(ER)*S*$",
		current_index, argc, argv),
		current_index, argc, argv, output_trailer_flag_))
		;
	else if (VFastParseCmdLineArg::ParseCmdLineElapsedTimer(current_index,
		argc, argv, timer_flag_))
		;
	else
		return(false);

	if (output_compatible_flag_) {
		try {
			INTERNAL_VFAST_PFI_CHECK_CONFLICT("debug",           debug_flag_,          true);
			INTERNAL_VFAST_PFI_CHECK_CONFLICT("sources",         source_flag_,         false);
			INTERNAL_VFAST_PFI_CHECK_CONFLICT("checksum",        checksum_flag_,       false);
			INTERNAL_VFAST_PFI_CHECK_CONFLICT("packet_headers",  packet_header_flag_,  false);
			INTERNAL_VFAST_PFI_CHECK_CONFLICT("output_headers",  output_header_flag_,  true);
			INTERNAL_VFAST_PFI_CHECK_CONFLICT("output_trailers", output_trailer_flag_, true);
			INTERNAL_VFAST_PFI_CHECK_CONFLICT("elapsed_timer_",  timer_flag_,          true);
		}
		catch (const std::exception &except) {
			std::ostringstream o_str;
			o_str << "The '-output_compatibility' flag was specified as true, "
				"but that setting is in conflict with the value of the " <<
				except.what();
			Rethrow(except, o_str.str());
		}
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	End-of-life.
#undef INTERNAL_VFAST_PFI_CHECK_CONFLICT
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string PacketFileInterface::GetHelpStringShort(bool all_params_optional)
	 const
{
	std::ostringstream o_str;

	o_str <<
		((all_params_optional) ? "" : "[ ") <<
		"-xml_template_file <XML-template-file> " <<
		((all_params_optional) ? "" : "] ") <<
		((all_params_optional) ? "" : "[ ") <<
		"-packet_file <packet-file> " <<
		((all_params_optional) ? "" : "] ") <<
		"[ -decode_type " <<
			"{ NONE | { EUREX | CEF | EBS } | CME | CME2 | ISE | XDP } ] " <<
		"[ -packet_format <packet-format> ] " <<
		"[ -first_packet_index <first-packet-index> ] " <<
		"[ -max_packet_count <maximum-number-of-packets> ] " <<
		"[ -iteration_count <number-of-times-to-iterate> ] " <<
		"[ -progression_counter <message-granularity> ] " <<
		"[ -xml <boolean> ] " <<
		"[ -fix <boolean> ] " <<
		"[ -debug <boolean> ] " <<
		"[ -output_compatible <boolean> ] " <<
		"[ -sources <boolean> ] " <<
		"[ -checksum <boolean> ] " <<
		"[ -packet_headers <boolean> ] " <<
		"[ -output_header <boolean> ] " <<
		"[ -output_trailer <boolean> ] " <<
		"[ -elapsed_timer <boolean> ]";

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string PacketFileInterface::GetHelpStringLong() const
{
	std::ostringstream o_str;

	o_str << "\
   -xml_template_file <XML-template-file>\n\
      Specifies the name of the XML template file to be used for decoding\n\
      packets.\n\n\
   -packet_file <packet-file>\n\
      Specifies the name of the packet file which contains the packets to be\n\
      decoded.\n\n\
   -decode_type { NONE | { EUREX | CEF | EBS } | CME | ISE }\n\
      Optional parameter which specifies the exchange-specific semantics to\n\
      be used when decoding the packets.\n\n\
      If not specified, this parameter defaults to 'NONE'.\n\n\
   -packet_format { RAW | LENGTH | TIME[IPSOURCE] | IPSOURCE[TLEN] }\n\
      Optional parameter which specifies the format in which packets are\n\
      stored in the packet file:\n\n\
         o RAW          - Each packet immediately follows its predecessor\n\
                          without preamble.\n\n\
         o LENGTH       - Each packet is preceded by its 16-bit packet length\n\
                          in network byte order.\n\n\
         o TIME         - Each packet is preceded by its 16-bit packet length\n\
                          followed by the operating systen timestamp of the\n\
                          time the packet was captured. The timestamp is\n\
                          stored as two 32-bit equivalents of the two member\n\
                          data which constitute a 'struct timespec', but are\n\
                          in network byte order.\n\n\
         o IPSOURCE     - Each packet is preceded by it 16-bit packet length\n\
                          followed by the 32-bit IP address of the packet\n\
                          source and the 16-bit port of the packet source in\n\
                          network byte order.\n\n\
         o TIMEIPSOURCE - Each packet is preceded by its 16-bit packet length\n\
                          followed by the operating system timestamp stored\n\
                          as described in the 'TIME' format above. That in\n\
                          turn is followed by the IP address and port number\n\
                          as described in the 'IPSOURCE' format.\n\n\
         o IPSOURCETLEN - As above in the 'IPSOURCE' format except that the\n\
                          length is after the IP address and port (TLEN =\n\
                          'Trailing Length'; backwards compatibility only).\n\n\
      If not specified, this parameter defaults to 'LENGTH'.\n\n"
<<
"\
   -first_packet_index <first-packet-index>\n\
      Optional parameter which specifies the index of the first packet to be\n\
      decoded.\n\n\
      Numbering for packet indices begins at '0'.\n\n\
      If not specified, this parameter defaults to 0 (the first packet).\n\n\
   -max_packet_count <maximum-number-of-packets>\n\
      Optional parameter which specifies the maximum number of packets to\n\
      decode.\n\n\
      If not specified, all packets will be decoded.\n\n\
   -iteration_count <number-of-times-to-iterate>\n\
      Optional parameter which specifies the number of times to iterate over\n\
      the packets in the file.\n\n\
      If not specified, this parameter defaults to '1'.\n\n\
   -progression_counter <message-granularity>\n\
      Optional parameter which specifies the granularity at which progress is\n\
      emitted to stderr.\n\n\
      If specified as '0', progress will not be emitted. Otherwise, progress\n\
      will be reported every time <message-granularity> number of messages\n\
      have been processed.\n\n\
      Progress is reported in the following format:\n\n\
         [<total>][<fails>][<all-msgs>][<packets>][<msgs-in-current-packet>]\n\n\
   -xml <boolean>\n\
      If true, specifies that decoded packet contents will be written to\n\
      stdout in XML format.\n\n\
      If not specified, this parameter defaults to '" <<
		MLB::Utility::AnyToString(xml_flag_) << "'.\n\n\
   -fix <boolean>\n\
      If true, specifies that decoded packet contents will be written to\n\
      stdout in FIX standard format.\n\n\
      If not specified, this parameter defaults to '" <<
		MLB::Utility::AnyToString(fix_flag_) << "'.\n\n\
   -debug <boolean>\n\
      If true, specifies that decoded packet contents will be written to\n\
      stdout.\n\n\
      If not specified, this parameter defaults to '" <<
		MLB::Utility::AnyToString(debug_flag_) << "'.\n\n\
   -output_compatible <boolean>\n\
      If true, specifies that the output written will be compatible with\n\
      the original version of this program.\n\n\
      If this parameter is specified as true, the value of several other\n\
      parameters are checked to ensure they do not conflict. The flags, and\n\
      their non-conflicting values, are:\n\n\
         -debug          true\n\
         -sources        false\n\
         -checksum       false\n\
         -packet_headers false\n\
         -output_header  true\n\
         -output_trailer true\n\
         -elapsed_timer  true\n\n\
      If not specified, this parameter defaults to '" <<
		MLB::Utility::AnyToString(output_compatible_flag_) << "'.\n\n\
   -sources <boolean>\n\
      If true, specifies that the source of the packet will be written to\n\
      stdout. The source includes the following data:\n\n\
         o The offset of the packet within the packet file.\n\
         o The IP address and port source of the packet (if recorded within\n\
           the packet file).\n\n\
      If not specified, this parameter defaults to '" <<
		MLB::Utility::AnyToString(source_flag_) << "'.\n\n\
   -checksum <boolean>\n\
      Specifies that decoded packet contents will be checksummed and the\n\
      checksum value will be written to stdout.\n\n\
      If not specified, this parameter defaults to '" <<
		MLB::Utility::AnyToString(checksum_flag_) << "'.\n\n\
   -packet_headers <boolean>\n\
      Specifies that packet headers, if supported by the decode type, are to\n\
      be output.\n\n\
      If not specified, this parameter defaults to '" <<
		MLB::Utility::AnyToString(packet_header_flag_) << "'.\n\n\
      As of this writing, the only decode types which use packet headers are\n\
      'CME2' and 'XDP'.\n\n\
   -output_header <boolean>\n\
      Specifies that output of that packet data is preceeded by a header\n\
      which briefly describes some basic parameters of the output to be\n\
      performed.\n\n\
      If not specified, this parameter defaults to '" <<
		MLB::Utility::AnyToString(output_header_flag_) << "'.\n\n\
   -output_trailer <boolean>\n\
      Specifies that output of that packet data is followed by a trailer\n\
      which briefly describes the output performed.\n\n\
      If not specified, this parameter defaults to '" <<
		MLB::Utility::AnyToString(output_trailer_flag_) << "'.\n\n\
   -elapsed_timer <boolean>\n\
      If true, specifies that the elapsed time is to be emitted to stdout\n\
      after packet decoding is complete.\n\n\
      If not specified, this parameter defaults to '" <<
		MLB::Utility::AnyToString(timer_flag_) << "'.";

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

