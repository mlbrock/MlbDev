// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Wrapper functions for the TibrvTransportNet class.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

namespace {

//	////////////////////////////////////////////////////////////////////////////
const unsigned int AnSNDComponent_Service = 0;
const unsigned int AnSNDComponent_Network = 1;
const unsigned int AnSNDComponent_Daemon  = 2;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &GetAnSNDComponentBasic(const TibrvNetTransport &transport_ref,
	const std::string &snd_component_name, unsigned int snd_component_type,
	std::string &out_string)
{
	try {
		TransportCheckIsValid(transport_ref);
		const char *tmp_ptr = NULL;
		if (snd_component_type == AnSNDComponent_Service) {
			RvUtilX_THROW_TIBRV_STATUS_IF(transport_ref.getService,
				(tmp_ptr));
		}
		else if (snd_component_type == AnSNDComponent_Network) {
			RvUtilX_THROW_TIBRV_STATUS_IF(transport_ref.getNetwork,
				(tmp_ptr));
		}
		else {
			RvUtilX_THROW_TIBRV_STATUS_IF(transport_ref.getDaemon,
				(tmp_ptr));
		}
		out_string.assign((tmp_ptr == NULL) ? "" : tmp_ptr);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to get the " + snd_component_name +
			" for a Tib/Rv network transport: " + std::string(except.what()));
	}

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &GetAnSNDComponent(const TibrvNetTransport &transport_ref,
	unsigned int snd_component_type, std::string &out_string)
{
	std::string snd_component_name;

	if (snd_component_type == AnSNDComponent_Service)
		snd_component_name = "service";
	else if (snd_component_type == AnSNDComponent_Network)
		snd_component_name = "network";
	else
		snd_component_name = "daemon";

	return(GetAnSNDComponentBasic(transport_ref, snd_component_name,
		snd_component_type, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
TibrvNetTransport &CreateTransport(TibrvNetTransport &transport,
	const std::string &service, const std::string &network,
	const std::string &daemon)
{
	try {
		TransportCheckIsNotValid(transport);
		RvUtilX_THROW_TIBRV_STATUS_IF(transport.create,
			(service.c_str(), network.c_str(), daemon.c_str()));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to create a Tib/Rv transport: " +
			std::string(except.what()));
	}

	return(transport);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
TibrvNetTransport &CreateLicensedTransport(TibrvNetTransport &transport,
	const std::string &service, const std::string &network,
	const std::string &daemon, const std::string &license_ticket)
{
	try {
		TransportCheckIsNotValid(transport);
		RvUtilX_THROW_TIBRV_STATUS_IF(transport.createLicensed,
			(service.c_str(), network.c_str(), daemon.c_str(),
			license_ticket.c_str()));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to create a Tib/Rv transport: " +
			std::string(except.what()));
	}

	return(transport);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &TransportGetService(const TibrvNetTransport &transport_ref,
	std::string &out_string)
{
	return(GetAnSNDComponent(transport_ref, AnSNDComponent_Service,
		out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string TransportGetService(const TibrvNetTransport &transport_ref)
{
	std::string out_string;

	return(TransportGetService(transport_ref, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &TransportGetNetwork(const TibrvNetTransport &transport_ref,
	std::string &out_string)
{
	return(GetAnSNDComponent(transport_ref, AnSNDComponent_Network,
		out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string TransportGetNetwork(const TibrvNetTransport &transport_ref)
{
	std::string out_string;

	return(TransportGetNetwork(transport_ref, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &TransportGetDaemon(const TibrvNetTransport &transport_ref,
	std::string &out_string)
{
	return(GetAnSNDComponent(transport_ref, AnSNDComponent_Daemon,
		out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string TransportGetDaemon(const TibrvNetTransport &transport_ref)
{
	std::string out_string;

	return(TransportGetDaemon(transport_ref, out_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SNDSpec &GetSNDSpec(const TibrvNetTransport &transport_ref, SNDSpec &out_spec)
{
	SNDSpec(TransportGetService(transport_ref),
		TransportGetNetwork(transport_ref),
		TransportGetDaemon(transport_ref)).swap(out_spec);

	return(out_spec);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SNDSpec GetSNDSpec(const TibrvNetTransport &transport_ref)
{
	SNDSpec out_spec;

	return(GetSNDSpec(transport_ref, out_spec));
}
//	////////////////////////////////////////////////////////////////////////////

#ifdef MLB_RvUtilX_Support_TransportSetBatchMode
//	////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION NOTE: Tib/Rv 7.0 and above only.
void TransportSetBatchMode(TibrvNetTransport &transport_ref,
	tibrvTransportBatchMode batch_mode)
{
	RvUtilX_THROW_TIBRV_STATUS_IF(transport_ref.setBatchMode,
		(batch_mode));
}
//	////////////////////////////////////////////////////////////////////////////
#endif // #ifdef MLB_RvUtilX_Support_TransportSetBatchMode

} // namespace RvUtilX

} // namespace MLB

