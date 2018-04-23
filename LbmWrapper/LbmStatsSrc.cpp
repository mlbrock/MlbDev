// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	29 West LBM API Wrapper Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the LbmStatsSrc class.

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

#include <LbmWrapper/LbmStatsSrc.hpp>
#include <LbmWrapper/LbmStatsSupport.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LbmWrapper {

//	////////////////////////////////////////////////////////////////////////////
LbmStatsSrc::LbmStatsSrc(MyDatumType &datum)
	:LbmStatsBase<MyDatumType>(datum.GetObjectPtr())
{
	Initialize();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmStatsSrc::LbmStatsSrc(MyLbmApiType *datum)
	:LbmStatsBase<MyDatumType>(datum)
{
	Initialize();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmStatsSrc::LbmStatsSrc(const MyLbmApiStatsType &stats)
	:LbmStatsBase<MyDatumType>(stats)
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string LbmStatsSrc::ToString(unsigned int text_width,
	bool /* fixup_min */) const
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
			EmitStatNameValueEndL("Number of current clients", text_width,
				stats_datum_.transport.tcp.num_clients, o_str);
			EmitStatNameValue("Number of current bytes buffered", text_width,
				stats_datum_.transport.tcp.bytes_buffered, o_str);
			break;
		case LBM_TRANSPORT_STAT_LBTRM		:
			o_str
				<< " (LBT-RM)" << std::endl
					;
			EmitStatNameValueEndL("Transport source", text_width,
				stats_datum_.source, o_str);
			EmitStatNameValueEndL("Transport messages sent", text_width,
				stats_datum_.transport.lbtrm.msgs_sent, o_str);
			EmitStatNameValueEndL("Total packet bytes sent", text_width,
				stats_datum_.transport.lbtrm.bytes_sent, o_str);
			EmitStatNameValueEndL("Current number of messages in transmission window", text_width,
				stats_datum_.transport.lbtrm.txw_msgs, o_str);
			EmitStatNameValueEndL("Current number of bytes in transmission window", text_width,
				stats_datum_.transport.lbtrm.txw_bytes, o_str);
			EmitStatNameValueEndL("Number of NAK packets received", text_width,
				stats_datum_.transport.lbtrm.nak_pckts_rcved, o_str);
			EmitStatNameValueEndL("Number of individual NAKs received", text_width,
				stats_datum_.transport.lbtrm.naks_rcved, o_str);
			EmitStatNameValueEndL("Number of NAKs ignored", text_width,
				stats_datum_.transport.lbtrm.naks_ignored, o_str);
			EmitStatNameValueEndL("Number of NAKs shed", text_width,
				stats_datum_.transport.lbtrm.naks_shed, o_str);
			EmitStatNameValueEndL("Number of NAKs delayed", text_width,
				stats_datum_.transport.lbtrm.naks_rx_delay_ignored, o_str);
			EmitStatNameValueEndL("Number of messages retransmitted", text_width,
				stats_datum_.transport.lbtrm.rxs_sent, o_str);
			EmitStatNameValueEndL("Number of messages queued", text_width,
				stats_datum_.transport.lbtrm.rctlr_data_msgs, o_str);
			EmitStatNameValueEndL("Number of retransmissions queued", text_width,
				stats_datum_.transport.lbtrm.rctlr_rx_msgs, o_str);
			EmitStatNameValue("Number of bytes retransmitted", text_width,
				stats_datum_.transport.lbtrm.rx_bytes_sent, o_str);
			break;
		case LBM_TRANSPORT_STAT_DAEMON	:
			o_str
				<< " (DAEMON)" << std::endl
					;
			EmitStatNameValueEndL("Transport source", text_width,
				stats_datum_.source, o_str);
			EmitStatNameValue("Number of current bytes buffered", text_width,
				stats_datum_.transport.daemon.bytes_buffered, o_str);
			break;
		case LBM_TRANSPORT_STAT_LBTRU		:
			o_str
				<< " (LBT-RU)" << std::endl
					;
			EmitStatNameValueEndL("Transport source", text_width,
				stats_datum_.source, o_str);
			EmitStatNameValueEndL("Transport messages sent", text_width,
				stats_datum_.transport.lbtru.msgs_sent, o_str);
			EmitStatNameValueEndL("Total packet bytes sent", text_width,
				stats_datum_.transport.lbtru.bytes_sent, o_str);
			EmitStatNameValueEndL("Number of NAK packets received", text_width,
				stats_datum_.transport.lbtru.nak_pckts_rcved, o_str);
			EmitStatNameValueEndL("Number of individual NAKs received", text_width,
				stats_datum_.transport.lbtru.naks_rcved, o_str);
			EmitStatNameValueEndL("Number of NAKs ignored", text_width,
				stats_datum_.transport.lbtru.naks_ignored, o_str);
			EmitStatNameValueEndL("Number of NAKs shed", text_width,
				stats_datum_.transport.lbtru.naks_shed, o_str);
			EmitStatNameValueEndL("Number of NAKs delayed", text_width,
				stats_datum_.transport.lbtru.naks_rx_delay_ignored, o_str);
			EmitStatNameValueEndL("Number of messages retransmitted", text_width,
				stats_datum_.transport.lbtru.rxs_sent, o_str);
			EmitStatNameValueEndL("Number of current clients", text_width,
				stats_datum_.transport.lbtru.num_clients, o_str);
			EmitStatNameValue("Number of bytes retransmitted", text_width,
				stats_datum_.transport.lbtru.rx_bytes_sent, o_str);
			break;
#if (LBM_VERS_MAJOR > 3) || ((LBM_VERS_MAJOR == 3) && (LBM_VERS_MINOR >= 5))
		case LBM_TRANSPORT_STAT_LBTIPC	:
			o_str
				<< " (LBT-IPC)" << std::endl
					;
			EmitStatNameValueEndL("Transport source", text_width,
				stats_datum_.source, o_str);
			EmitStatNameValueEndL("Number of current clients", text_width,
				stats_datum_.transport.lbtipc.num_clients, o_str);
			EmitStatNameValueEndL("Transport messages sent", text_width,
				stats_datum_.transport.lbtipc.msgs_sent, o_str);
			EmitStatNameValue("Total packet bytes sent", text_width,
				stats_datum_.transport.lbtipc.bytes_sent, o_str);
			break;
#endif // #if (LBM_VERS_MAJOR > 3) || ((LBM_VERS_MAJOR == 3) && (LBM_VERS_MINOR >= 5))
#if (LBM_VERS_MAJOR > 4) || ((LBM_VERS_MAJOR == 4) && (LBM_VERS_MINOR >= 1))
		case LBM_TRANSPORT_TYPE_LBTRDMA	:
			EmitStatNameValueEndL("Number of current clients", text_width,
				stats_datum_.transport.lbtrdma.num_clients, o_str);
			EmitStatNameValueEndL("Transport messages sent", text_width,
				stats_datum_.transport.lbtrdma.msgs_sent, o_str);
			EmitStatNameValue("Total packet bytes sent", text_width,
				stats_datum_.transport.lbtrdma.bytes_sent, o_str);
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
void LbmStatsSrc::Initialize()
{
	WRAP29_LBM_THROW_IF_NEGATIVE_ONE(
		::lbm_src_retrieve_transport_stats,
		(datum_ptr_, &stats_datum_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmStatsSrcVector LbmStatsSrc::GetStatsForContext(LbmContext &context)
{
	return(GetStatsForContext(context.GetObjectPtr()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmStatsSrcVector LbmStatsSrc::GetStatsForContext(lbm_context_t *context)
{
	GetStatsForAllAdapter<LbmStatsSrc, lbm_context_t> stat_list(context,
		"transport sources", "context",
		"::lbm_context_retrieve_src_transport_stats_ex",
		::lbm_context_retrieve_src_transport_stats_ex);

	return(stat_list.stat_list_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmStatsSrcVector LbmStatsSrc::GetImStatsForContext(LbmContext &context)
{
	return(GetImStatsForContext(context.GetObjectPtr()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmStatsSrcVector LbmStatsSrc::GetImStatsForContext(lbm_context_t *context)
{
	GetStatsForAllAdapter<LbmStatsSrc, lbm_context_t> stat_list(context,
		"IM transport sources", "context",
		"::lbm_context_retrieve_im_src_transport_stats",
		::lbm_context_retrieve_im_src_transport_stats);

	return(stat_list.stat_list_);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace LbmWrapper

} // namespace MLB

