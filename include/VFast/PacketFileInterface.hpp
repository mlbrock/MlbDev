// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
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

#ifndef HH__MLB__VFast__PacketFileInterface_hpp__HH

#define HH__MLB__VFast__PacketFileInterface_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file PacketFileInterface.hpp

	\brief	Generic interface to reading FAST packet files.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <SocketIo/SocketIoParseCmdLineArg.hpp>
#include <SocketIo/PacketFileWriter.hpp>

#include <VFast/ExcContext.hpp>
#include <VFast/VFastUtility.hpp>
#include <VFast/PacketDecodeRegime.hpp>
#include <VFast/CmePacketHeader.hpp>
#include <VFast/XdpPacketHeader.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
class PacketFileInterface;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct PacketFileInterfaceState {
	explicit PacketFileInterfaceState(const std::string &xml_file_name,
		const ExcContextInfo &exc_context_info = ExcContextInfo());

	void WritePacketToFile(const std::string &out_file_name,
		MLB::SocketIo::PacketFormat packet_format) const
	{
		MLB::SocketIo::PacketFileWriter out_file(out_file_name, packet_format);
		WritePacketToFile(out_file);
	}
	void WritePacketToFile(MLB::SocketIo::PacketFileWriter &out_file) const
	{
		out_file.WritePacket(
			static_cast<std::size_t>(pkt_end_ptr_ - pkt_begin_ptr_),
			pkt_begin_ptr_);
	}
	void WriteMessageToFile(const std::string &out_file_name,
		MLB::SocketIo::PacketFormat packet_format) const
	{
		MLB::SocketIo::PacketFileWriter out_file(out_file_name, packet_format);
		WriteMessageToFile(out_file);
	}
	void WriteMessageToFile(MLB::SocketIo::PacketFileWriter &out_file) const
	{
		out_file.WritePacket(
			static_cast<std::size_t>(msg_end_ptr_ - msg_begin_ptr_),
			msg_begin_ptr_);
	}

	unsigned int                     all_index_;
	unsigned int                     pkt_index_;
	unsigned int                     msg_index_;
	const StreamDatum               *pkt_begin_ptr_;
	const StreamDatum               *pkt_end_ptr_;
	const StreamDatum               *msg_begin_ptr_;
	const StreamDatum               *msg_end_ptr_;
	const void                      *pkt_header_ptr_;
	unsigned int                     pkt_header_length_;
	MLB::SocketIo::IpAddressType     ip_address_;
	MLB::SocketIo::IpPortType        ip_port_;
	MLB::SocketIo::MsgTimeSecsType   time_secs_;
	MLB::SocketIo::MsgTimeNSecsType  time_nsecs_;
	InsContext                       ins_context_;
	ExcContext                       exc_context_;
	const CmePacketHeader2_0        *cme_packet_header_ptr_;
	const XdpPacketHeader           *xdp_packet_header_ptr_;
	CmePacketHeader2_0               cme_packet_header_;
	XdpPacketHeader                  xdp_packet_header_;

private:
	PacketFileInterfaceState(const PacketFileInterfaceState &);
	PacketFileInterfaceState & operator = (const PacketFileInterfaceState &);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class BaseMsgHandlerFunc {
public:
	virtual ~BaseMsgHandlerFunc()
	{
	}

	virtual void PreParseCmdLine(PacketFileInterface &reader, int argc,
		char **argv);

	/**
		Called for each command line parameter not recognized as one of the
		established parameters.

		\return True if the command line parameter was recognized and
		parsed successfully.
	*/
	virtual bool ThisParseCmdLine(PacketFileInterface &reader, int argc,
		char **argv, unsigned int &current_index);

	/**
		Called after all command line parameters have been parsed.
	*/
	virtual void PostParseCmdLine(PacketFileInterface &reader, int argc,
		char **argv);

	/**
		Called before the iteration loop begins.
	*/
	virtual void PreIterLoop(PacketFileInterface &reader);

	/**
		Called after the iteration loop has ended.
	*/
	virtual void PostIterLoop(PacketFileInterface &reader,
		const PacketFileInterfaceState &pfi_state);

	/**
		Called before any FAST messages within the packet are decoded.

		This method handles any adjustment required for packet headers and/or
		trailers.

		\return True if you want to proceed to message decoding. If you
		return \e false, processing will continue with the next packet.
	*/
	virtual bool PreDecodePkt(PacketFileInterface &reader,
		PacketFileInterfaceState &pfi_state, const StreamDatum *&begin_ptr,
		const StreamDatum *&end_ptr);

	/**
		Called before the FAST message is decoded to provide the opportunity for
		debugging output.

		\return True if you want to continue with message decoding. If you
		return \e false, processing will continue with the next packet.
	*/
	virtual bool PreDecodeMsgDebug(PacketFileInterface &reader,
		const PacketFileInterfaceState &pfi_state);

	/**
		Called before the FAST message is decoded.

		\return True if you want to continue with message decoding. If you
		return \e false, processing will continue with the next packet.
	*/
	virtual bool PreDecodeMsg(PacketFileInterface &reader,
		const PacketFileInterfaceState &pfi_state);

	/**
		Called after the FAST message is decoded to provide the opportunity for
		debugging output. The results of message decoding are in the
		\e exc_results parameter.

		\return True if you want to continue with message decoding. If you
		return \e false, processing will continue with the next packet.
	*/
	virtual bool PostDecodeMsgDebug(PacketFileInterface &reader,
		const PacketFileInterfaceState &pfi_state, ExcResults &exc_results);

	/**
		Called after the FAST message is decoded to provide the opportunity for
		checksum debugging output. The results of message decoding are in the
		\e exc_results parameter.

		\return True if you want to continue with message decoding. If you
		return \e false, processing will continue with the next packet.
	*/
	virtual bool PostDecodeMsgChecksum(PacketFileInterface &reader,
		const PacketFileInterfaceState &pfi_state, ExcResults &exc_results);

	/**
		Called after the FAST message is decoded. The results of message
		decoding are in the \e exc_results parameter.

		\return True if you want to continue with message decoding. If you
		return \e false, processing will continue with the next packet.
	*/
	virtual bool PostDecodeMsg(MLB::VFast::PacketFileInterface & /* reader */,
      const PacketFileInterfaceState &pfi_state, ExcResults &exc_results);

	/**
		Called if an exception is thrown by the message decoding logic.
	*/
	virtual void DecodeMsgException(PacketFileInterface &reader,
		const PacketFileInterfaceState &pfi_state, const ExcResults &exc_results,
		const std::exception &except);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class PacketFileInterface {
public:
	PacketFileInterface();
	virtual ~PacketFileInterface();

	virtual void Run();
	virtual void Run(BaseMsgHandlerFunc &msg_handler_func);

   void ParseCmdLine(int argc, char **argv);
	void ParseCmdLine(BaseMsgHandlerFunc &msg_handler_func, int argc,
		char **argv);
   bool ParseCmdSingle(unsigned int &current_index, int argc, char **argv);
	bool ParseCmdSingle(BaseMsgHandlerFunc &msg_handler_func,
		unsigned int &current_index, int argc, char **argv);

	std::string GetHelpStringShort(bool all_params_optional) const;
	std::string GetHelpStringLong() const;

	std::string                 xml_file_name_;
	std::string                 packet_file_name_;
	PacketDecodeRegime          content_type_;
	MLB::SocketIo::PacketFormat packet_format_;
	unsigned int                first_packet_index_;
	unsigned int                max_message_count_;
	unsigned int                iteration_count_;
	unsigned int                progress_counter_;
	bool                        xml_flag_;
	bool                        fix_flag_;
	bool                        debug_flag_;
	bool                        source_flag_;
	bool                        checksum_flag_;
	bool                        packet_header_flag_;
	bool                        output_header_flag_;
	bool                        output_trailer_flag_;
	bool                        timer_flag_;
	bool                        cancel_packets_flag_;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifdef HH__MLB__VFast__PacketFileInterface_hpp__HH

