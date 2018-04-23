// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Wrapper functions for the TibrvTransportCm class.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX/Cm.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

//	////////////////////////////////////////////////////////////////////////////
TibrvCmTransport &CreateTransport(TibrvCmTransport &cm_transport,
	TibrvNetTransport &net_transport)
{
	try {
		TransportCheckIsValid(net_transport);
		TransportCheckIsNotValid(cm_transport);
		RvUtilX_THROW_TIBRV_STATUS_IF(cm_transport.create,
			(&net_transport));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to create a Tib/Rv certified "
			"messaging transport: " + std::string(except.what()));
	}

	return(cm_transport);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
TibrvCmTransport &CreateTransport(TibrvCmTransport &cm_transport,
	TibrvNetTransport &net_transport, const std::string &cm_name,
	bool request_old, const std::string &ledger_name, bool sync_ledger,
	const std::string &relay_agent)
{
	try {
		TransportCheckIsValid(net_transport);
		TransportCheckIsNotValid(cm_transport);
		RvUtilX_THROW_TIBRV_STATUS_IF(cm_transport.create,
			(&net_transport, cm_name.c_str(),
			static_cast<tibrv_bool>(request_old),
			(ledger_name.empty()) ? NULL : ledger_name.c_str(),
			static_cast<tibrv_bool>(sync_ledger),
			(relay_agent.empty()) ? NULL : relay_agent.c_str()));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to create a Tib/Rv certified "
			"messaging transport: " + std::string(except.what()));
	}

	return(cm_transport);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
TibrvNetTransport *GetTransport(const TibrvCmTransport &cm_transport)
{
	TibrvTransport *out_transport = cm_transport.getTransport();

	if (out_transport == NULL)
		throw RvException("Invocation of 'TibrvCmTransport::getTransport()' "
			"failed because the method returned 'NULL' --- the transport has "
			"probably not been created.");

	//	Only TibrvNetTransports can be used in the create function...
	return(static_cast<TibrvNetTransport *>(out_transport));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

