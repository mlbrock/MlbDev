// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Wrapper functions for the TibrvTransport class.

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

//	////////////////////////////////////////////////////////////////////////////
std::string TransportCreateInbox(TibrvTransport &transport_ref)
{
	std::string subject_name;

	return(TransportCreateInbox(transport_ref, subject_name));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &TransportCreateInbox(TibrvTransport &transport_ref,
	std::string &subject_name)
{
	char tmp_subject_name[MaxSubjectSize + 1];

	try {
		TransportCheckIsValid(transport_ref);
		RvUtilX_THROW_TIBRV_STATUS_IF(transport_ref.createInbox,
			(tmp_subject_name, sizeof(tmp_subject_name) - 1));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to create an inbox subject name "
			"for a Tib/Rv transport: " + std::string(except.what()));
	}

	return(subject_name.assign(tmp_subject_name));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TransportDestroy(TibrvTransport &transport_ref)
{
	TransportDestroy(&transport_ref);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TransportDestroy(TibrvTransport *transport_ptr)
{
	try {
		RvUtilX_THROW_TIBRV_STATUS_IF(transport_ptr->destroy,
			());
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to destroy a Tib/Rv transport "
			"failed: " + std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &TransportGetDescription(const tibrvTransport &transport_ref,
	std::string &description)
{
	const char *tmp_ptr = NULL;

	RvUtilX_THROW_TIBRV_STATUS_IF(tibrvTransport_GetDescription,
		(transport_ref, &tmp_ptr));

	return(description.assign((tmp_ptr == NULL) ? "" : tmp_ptr));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string TransportGetDescription(const tibrvTransport &transport_ref)
{
	std::string description;

	return(TransportGetDescription(transport_ref, description));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &TransportGetDescription(const TibrvTransport &transport_ref,
	std::string &description)
{
	try {
		TransportCheckIsValid(transport_ref);
		//	The description for the default transport can't be retrieved...
		if (TransportIsProcessTransport(transport_ref))
			description.clear();
		else {
			try {
				const char *tmp_ptr = NULL;
				RvUtilX_THROW_TIBRV_STATUS_IF(transport_ref.getDescription,
					(&tmp_ptr));
				description.assign((tmp_ptr == NULL) ? "" : tmp_ptr);
			}
			catch (const RvExceptionStatus &except) {
				//	We've already checked to ensure that the transport is valid
				//	above. So if the tibrv status isn't TIBRV_INVALID_TRANSPORT
				//	(which would indicate the intra-process transport), some other
				//	error must have occurred...
				if (except.GetStatus() != TIBRV_INVALID_TRANSPORT)
					throw;
				description.clear();
			}
		}
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to get the description for a "
			"Tib/Rv transport: " + std::string(except.what()));
	}

	return(description);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string TransportGetDescription(const TibrvTransport &transport_ref)
{
	std::string description;

	return(TransportGetDescription(transport_ref, description));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
tibrvTransport TransportGetHandle(const TibrvTransport &transport_ref)
{
	return(transport_ref.getHandle());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool TransportIsValid(const TibrvTransport &transport_ref)
{
	return(transport_ref.isValid() == TIBRV_TRUE);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TransportSend(TibrvTransport &transport_ref, const TibrvMsg &msg)
{
	try {
		RvUtilX_THROW_TIBRV_STATUS_IF(transport_ref.send,
			(msg));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to send a Tib/Rv message "
			"failed: " + std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TransportSendReply(TibrvTransport &transport_ref,
	const TibrvMsg &reply_msg, const TibrvMsg &request_msg)
{
	try {
		RvUtilX_THROW_TIBRV_STATUS_IF(transport_ref.sendReply,
			(reply_msg, request_msg));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to send a reply to a Tib/Rv "
			"message failed: " + std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TransportSendRequest(TibrvTransport &transport_ref,
	const TibrvMsg &request_msg, TibrvMsg &reply_msg, double time_out)
{
	try {
		TibrvStatus rv_status =
			transport_ref.sendRequest(request_msg, reply_msg, time_out);
		if (rv_status == TIBRV_TIMEOUT)
			ThrowTibrvTimeOut("Wait for a reply timed-out.", time_out);
		else
			ThrowTibrvStatus(rv_status, "Invocation of 'sendRequest()' failed.");
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to send a request and wait " +
			MLB::Utility::AnyToString(time_out) + " seconds for a reply to a "
			"Tib/Rv message request failed: " + std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TransportSetDescription(TibrvTransport &transport_ref,
	const char *description)
{
	try {
		TransportCheckIsValid(transport_ref);
		RvUtilX_THROW_TIBRV_STATUS_IF(transport_ref.setDescription,
			(description));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to set the description for a "
			"Tib/Rv transport: " + std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TransportSetDescription(TibrvTransport &transport_ref,
	const std::string &description)
{
	return(TransportSetDescription(transport_ref, description.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool TransportIsProcessTransport(const TibrvTransport &transport_ref)
{
	return(&transport_ref == GetProcessTransportPtr());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool TransportCheckIsValid(const TibrvTransport &transport_ref)
{
	if (!transport_ref.isValid())
		throw RvException("The specified Tib/Rv transport is not valid.");

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool TransportCheckIsNotValid(const TibrvTransport &transport_ref)
{
	if (transport_ref.isValid())
		throw RvException("The specified Tib/Rv transport is already valid.");

	return(true);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

