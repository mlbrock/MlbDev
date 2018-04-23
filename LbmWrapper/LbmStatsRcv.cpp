// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	29 West LBM API Wrapper Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the LbmStatsRcv class.

	Revision History	:	2008-08-03 --- Creation.
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

#include <LbmWrapper/LbmStatsRcv.hpp>
#include <LbmWrapper/LbmStatsSupport.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LbmWrapper {

//	****************************************************************************
//	****************************************************************************
//	****************************************************************************

//	////////////////////////////////////////////////////////////////////////////
LbmStatsRcv::LbmStatsRcv(MyDatumType &datum, const char *source)
	:LbmStatsBase<MyDatumType>(datum.GetObjectPtr())
{
	Initialize(source);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmStatsRcv::LbmStatsRcv(MyLbmApiType *datum, const char *source)
	:LbmStatsBase<MyDatumType>(datum)
{
	Initialize(source);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmStatsRcv::LbmStatsRcv(const MyLbmApiStatsType &stats)
	:LbmStatsBase<MyDatumType>(stats)
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string LbmStatsRcv::ToString(unsigned int text_width, bool fixup_min) const
{
	using namespace MLB::Utility;

	text_width = (text_width) ? text_width : MaxStatsTextWidth;

	std::ostringstream o_str;

	EmitStatNameValue("Transport type", text_width, stats_datum_.type, o_str);

	switch (stats_datum_.type) {
		case LBM_TRANSPORT_STAT_TCP		:
			o_str
				<< " (TCP)" << std::endl
					;
			EmitStatNameValueEndL("Transport source", text_width,
				stats_datum_.source, o_str);
			EmitStatNameValueEndL("Transport bytes received", text_width,
				stats_datum_.transport.tcp.bytes_rcved, o_str);
			EmitStatNameValueEndL("LBM messages received", text_width,
				stats_datum_.transport.tcp.lbm_msgs_rcved, o_str);
			EmitStatNameValueEndL("Messages received without a topic", text_width,
				stats_datum_.transport.tcp.lbm_msgs_no_topic_rcved, o_str);
			EmitStatNameValue("LBM requests received", text_width,
				stats_datum_.transport.tcp.lbm_reqs_rcved, o_str);
			break;
		case LBM_TRANSPORT_STAT_LBTRM		:
			o_str
				<< " (LBT-RM)" << std::endl
					;
			EmitStatNameValueEndL("Transport source", text_width,
				stats_datum_.source, o_str);
			EmitStatNameValueEndL("Transport messages received", text_width,
				stats_datum_.transport.lbtrm.msgs_rcved, o_str);
			EmitStatNameValueEndL("Total packet bytes received", text_width,
				stats_datum_.transport.lbtrm.bytes_rcved, o_str);
			EmitStatNameValueEndL("Number of NAK packets sent", text_width,
				stats_datum_.transport.lbtrm.nak_pckts_sent, o_str);
			EmitStatNameValueEndL("Number of individual NAKs sent", text_width,
				stats_datum_.transport.lbtrm.naks_sent, o_str);
			EmitStatNameValueEndL("Number of transport messages lost", text_width,
				stats_datum_.transport.lbtrm.lost, o_str);
			EmitStatNameValueEndL("Number of NCFs with ignored status", text_width,
				stats_datum_.transport.lbtrm.ncfs_ignored, o_str);
			EmitStatNameValueEndL("Number of NCFs with shed status", text_width,
				stats_datum_.transport.lbtrm.ncfs_shed, o_str);
			EmitStatNameValueEndL("Number of NCFs with rx_delay status", text_width,
				stats_datum_.transport.lbtrm.ncfs_rx_delay, o_str);
			EmitStatNameValueEndL("Number of NCFs with unknown status", text_width,
				stats_datum_.transport.lbtrm.ncfs_unknown, o_str);
			EmitStatNameValueEndL("Minimum loss recovery msecs", text_width,
				FixupMinStat(stats_datum_.transport.lbtrm.nak_stm_min, fixup_min), o_str);
			EmitStatNameValueEndL("Mean loss recovery msecs", text_width,
				stats_datum_.transport.lbtrm.nak_stm_mean, o_str);
			EmitStatNameValueEndL("Maximum loss recovery msecs", text_width,
				stats_datum_.transport.lbtrm.nak_stm_max, o_str);
			EmitStatNameValueEndL("Minimum individual NAK transmits", text_width,
				FixupMinStat(stats_datum_.transport.lbtrm.nak_tx_min, fixup_min), o_str);
			EmitStatNameValueEndL("Mean individual NAK transmits", text_width,
				stats_datum_.transport.lbtrm.nak_tx_mean, o_str);
			EmitStatNameValueEndL("Maximum individual NAK transmits", text_width,
				stats_datum_.transport.lbtrm.nak_tx_max, o_str);
			EmitStatNameValueEndL("Number of duplicate transport messages", text_width,
				stats_datum_.transport.lbtrm.duplicate_data, o_str);
			EmitStatNameValueEndL("Number of unrecovered transport messages due to window advance", text_width,
				stats_datum_.transport.lbtrm.unrecovered_txw, o_str);
			EmitStatNameValueEndL("Number of unrecovered transport messages due to NAK interval", text_width,
				stats_datum_.transport.lbtrm.unrecovered_tmo, o_str);
			EmitStatNameValueEndL("Number of application messages received", text_width,
				stats_datum_.transport.lbtrm.lbm_msgs_rcved, o_str);
			EmitStatNameValueEndL("Number of messages received on topics not of interest", text_width,
				stats_datum_.transport.lbtrm.lbm_msgs_no_topic_rcved, o_str);
			EmitStatNameValueEndL("Number of LBM request messages received", text_width,
				stats_datum_.transport.lbtrm.lbm_reqs_rcved, o_str);
			EmitStatNameValueEndL("Number of datagrams dropped due to the datagram size", text_width,
				stats_datum_.transport.lbtrm.dgrams_dropped_size, o_str);
			EmitStatNameValueEndL("Number of datagrams discarded due to bad packet type", text_width,
				stats_datum_.transport.lbtrm.dgrams_dropped_type, o_str);
			EmitStatNameValueEndL("Number of datagrams discarded due to version mismatch", text_width,
				stats_datum_.transport.lbtrm.dgrams_dropped_version, o_str);
			EmitStatNameValueEndL("Number of datagrams discarded due to bad header type", text_width,
				stats_datum_.transport.lbtrm.dgrams_dropped_hdr, o_str);
			EmitStatNameValue("Number of datagrams discarded due to other reasons", text_width,
				stats_datum_.transport.lbtrm.dgrams_dropped_other, o_str);
#if LBM_VERS_MAJOR >= 4
			o_str << std::endl;
			EmitStatNameValue("Number of out-of-order LBT-RM transport datagrams received", text_width,
				stats_datum_.transport.lbtrm.out_of_order, o_str);
#endif // #if LBM_VERS_MAJOR >= 4
			break;
		case LBM_TRANSPORT_STAT_DAEMON	:
			o_str
				<< " (DAEMON)" << std::endl
					;
			EmitStatNameValueEndL("Transport source", text_width,
				stats_datum_.source, o_str);
			EmitStatNameValue("Bytes received from LBM daemon", text_width,
				stats_datum_.transport.daemon.bytes_rcved, o_str);
			break;
		case LBM_TRANSPORT_STAT_LBTRU		:
			o_str
				<< " (LBT-RU)" << std::endl
					;
			EmitStatNameValueEndL("Transport source", text_width,
				stats_datum_.source, o_str);
			EmitStatNameValueEndL("Transport messages received", text_width,
				stats_datum_.transport.lbtru.msgs_rcved, o_str);
			EmitStatNameValueEndL("Total packet bytes received", text_width,
				stats_datum_.transport.lbtru.bytes_rcved, o_str);
			EmitStatNameValueEndL("Number of NAK packets sent", text_width,
				stats_datum_.transport.lbtru.nak_pckts_sent, o_str);
			EmitStatNameValueEndL("Number of individual NAKs sent", text_width,
				stats_datum_.transport.lbtru.naks_sent, o_str);
			EmitStatNameValueEndL("Number of transport messages lost", text_width,
				stats_datum_.transport.lbtru.lost, o_str);
			EmitStatNameValueEndL("Number of NCFs with ignored status", text_width,
				stats_datum_.transport.lbtru.ncfs_ignored, o_str);
			EmitStatNameValueEndL("Number of NCFs with shed status", text_width,
				stats_datum_.transport.lbtru.ncfs_shed, o_str);
			EmitStatNameValueEndL("Number of NCFs with rx_delay status", text_width,
				stats_datum_.transport.lbtru.ncfs_rx_delay, o_str);
			EmitStatNameValueEndL("Number of NCFs with unknown status", text_width,
				stats_datum_.transport.lbtru.ncfs_unknown, o_str);
			EmitStatNameValueEndL("Minimum loss recovery msecs", text_width,
				FixupMinStat(stats_datum_.transport.lbtru.nak_stm_min, fixup_min), o_str);
			EmitStatNameValueEndL("Mean loss recovery msecs", text_width,
				stats_datum_.transport.lbtru.nak_stm_mean, o_str);
			EmitStatNameValueEndL("Maximum loss recovery msecs", text_width,
				stats_datum_.transport.lbtru.nak_stm_max, o_str);
			EmitStatNameValueEndL("Minimum individual NAK transmits", text_width,
				FixupMinStat(stats_datum_.transport.lbtru.nak_tx_min, fixup_min), o_str);
			EmitStatNameValueEndL("Mean individual NAK transmits", text_width,
				stats_datum_.transport.lbtru.nak_tx_mean, o_str);
			EmitStatNameValueEndL("Maximum individual NAK transmits", text_width,
				stats_datum_.transport.lbtru.nak_tx_max, o_str);
			EmitStatNameValueEndL("Number of duplicate transport messages", text_width,
				stats_datum_.transport.lbtru.duplicate_data, o_str);
			EmitStatNameValueEndL("Number of unrecovered transport messages due to window advance", text_width,
				stats_datum_.transport.lbtru.unrecovered_txw, o_str);
			EmitStatNameValueEndL("Number of unrecovered transport messages due to NAK interval", text_width,
				stats_datum_.transport.lbtru.unrecovered_tmo, o_str);
			EmitStatNameValueEndL("Number of application messages received", text_width,
				stats_datum_.transport.lbtru.lbm_msgs_rcved, o_str);
			EmitStatNameValueEndL("Number of messages received on topics not of interest", text_width,
				stats_datum_.transport.lbtru.lbm_msgs_no_topic_rcved, o_str);
			EmitStatNameValueEndL("Number of LBM request messages received", text_width,
				stats_datum_.transport.lbtru.lbm_reqs_rcved, o_str);
			EmitStatNameValueEndL("Number of datagrams dropped due to the datagram size", text_width,
				stats_datum_.transport.lbtru.dgrams_dropped_size, o_str);
			EmitStatNameValueEndL("Number of datagrams discarded due to bad packet type", text_width,
				stats_datum_.transport.lbtru.dgrams_dropped_type, o_str);
			EmitStatNameValueEndL("Number of datagrams discarded due to version mismatch", text_width,
				stats_datum_.transport.lbtru.dgrams_dropped_version, o_str);
			EmitStatNameValueEndL("Number of datagrams discarded due to bad header type", text_width,
				stats_datum_.transport.lbtru.dgrams_dropped_hdr, o_str);
			EmitStatNameValueEndL("Number of datagrams discarded due to session ID mismatch", text_width,
				stats_datum_.transport.lbtru.dgrams_dropped_sid, o_str);
			EmitStatNameValue("Number of datagrams discarded due to other reasons", text_width,
				stats_datum_.transport.lbtru.dgrams_dropped_other, o_str);
			break;
#if (LBM_VERS_MAJOR > 3) || ((LBM_VERS_MAJOR == 3) && (LBM_VERS_MINOR >= 5))
		case LBM_TRANSPORT_STAT_LBTIPC	:
			o_str
				<< " (LBT-IPC)" << std::endl
					;
			EmitStatNameValueEndL("Transport source", text_width,
				stats_datum_.source, o_str);
			EmitStatNameValueEndL("Number of transport messages received", text_width,
				stats_datum_.transport.lbtipc.msgs_rcved, o_str);
			EmitStatNameValueEndL("Total number of bytes received", text_width,
				stats_datum_.transport.lbtipc.bytes_rcved, o_str);
			EmitStatNameValueEndL("Number of application messages received", text_width,
				stats_datum_.transport.lbtipc.lbm_msgs_rcved, o_str);
			EmitStatNameValueEndL("Number of application messages received on topics not of interest", text_width,
				stats_datum_.transport.lbtipc.lbm_msgs_no_topic_rcved, o_str);
			EmitStatNameValue("Number of LBM request messages received", text_width,
				stats_datum_.transport.lbtipc.lbm_reqs_rcved, o_str);
			break;
#endif // #if (LBM_VERS_MAJOR > 3) || ((LBM_VERS_MAJOR == 3) && (LBM_VERS_MINOR >= 5))
#if (LBM_VERS_MAJOR > 4) || ((LBM_VERS_MAJOR == 4) && (LBM_VERS_MINOR >= 1))
		case LBM_TRANSPORT_TYPE_LBTRDMA	:
			EmitStatNameValueEndL("Transport messages received", text_width,
				stats_datum_.transport.lbtrdma.msgs_rcved, o_str);
			EmitStatNameValueEndL("Transport bytes received", text_width,
				stats_datum_.transport.lbtrdma.bytes_rcved, o_str);
			EmitStatNameValueEndL("Application messages received", text_width,
				stats_datum_.transport.lbtrdma.lbm_msgs_rcved, o_str);
			EmitStatNameValueEndL("Messages received without a topic", text_width,
				stats_datum_.transport.lbtrdma.lbm_msgs_no_topic_rcved, o_str);
			EmitStatNameValue("LBM requests received", text_width,
				stats_datum_.transport.lbtrdma.lbm_reqs_rcved, o_str);
			break;
#endif // #if (LBM_VERS_MAJOR > 4) || ((LBM_VERS_MAJOR == 4) && (LBM_VERS_MINOR >= 1))
		default									:
			std::ostringstream o_str_2;
			o_str_2 << stats_datum_.type;
			ThrowInvalidArgument("Invalid transport type " + o_str_2.str() +
				" encountered.");
			break;
	}

	return(o_str.str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LbmStatsRcv::Initialize(const char *source)
{
	WRAP29_LBM_THROW_IF_NEGATIVE_ONE(
		::lbm_rcv_retrieve_transport_stats,
		(datum_ptr_, source, &stats_datum_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmStatsRcvVector LbmStatsRcv::GetStatsForContext(LbmContext &context)
{
	return(GetStatsForContext(context.GetObjectPtr()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmStatsRcvVector LbmStatsRcv::GetStatsForContext(lbm_context_t *context)
{
	GetStatsForAllAdapter<LbmStatsRcv, lbm_context_t> stat_list(context,
		"transport receivers", "context",
		"::lbm_context_retrieve_rcv_transport_stats_ex",
		::lbm_context_retrieve_rcv_transport_stats_ex);

	return(stat_list.stat_list_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmStatsRcvVector LbmStatsRcv::GetImStatsForContext(LbmContext &context)
{
	return(GetImStatsForContext(context.GetObjectPtr()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmStatsRcvVector LbmStatsRcv::GetImStatsForContext(lbm_context_t *context)
{
	GetStatsForAllAdapter<LbmStatsRcv, lbm_context_t> stat_list(context,
		"IM transport receivers", "context",
		"::lbm_context_retrieve_im_rcv_transport_stats",
		::lbm_context_retrieve_im_rcv_transport_stats);

	return(stat_list.stat_list_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmStatsRcvVector LbmStatsRcv::GetStatsForRcv(LbmRcv &receiver)
{
	return(GetStatsForRcv(receiver.GetObjectPtr()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmStatsRcvVector LbmStatsRcv::GetStatsForRcv(lbm_rcv_t *receiver)
{
	GetStatsForAllAdapter<LbmStatsRcv, lbm_rcv_t> stat_list(receiver,
		"transport receivers", "receiver",
		"::lbm_rcv_retrieve_all_transport_stats_ex",
		::lbm_rcv_retrieve_all_transport_stats_ex);

	return(stat_list.stat_list_);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace LbmWrapper

} // namespace MLB

