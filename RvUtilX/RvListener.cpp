// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Support for the manipulation of TibrvListen instances.

	Revision History	:	2007-09-06 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2007 - 2014.
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
TibrvListener &CreateListener(TibrvQueue &queue_ref,
	TibrvCallback &callback_ref, TibrvTransport &transport_ref,
	TibrvListener &listener_ref, const char *subject_name,
	const void *closure)
{
	try {
		SubjectNameWork::SubjectNameWork(subject_name);
		RvUtilX_THROW_TIBRV_STATUS_IF(listener_ref.create,
			(&queue_ref, &callback_ref, &transport_ref, subject_name, closure));
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to create a Tib/Rv listener: " +
			std::string(except.what()));
	}

	return(listener_ref);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
TibrvListener &CreateListener(TibrvQueue &queue_ref,
	TibrvCallback &callback_ref, TibrvTransport &transport_ref,
	TibrvListener &listener_ref, const std::string &subject_name,
	const void *closure)
{
	return(CreateListener(queue_ref, callback_ref, transport_ref, listener_ref,
		subject_name.c_str(), closure));
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *GetSubjectPtr(const TibrvListener &listener)
{
	const char  *subject_name;
	TibrvStatus  rv_status =
		const_cast<TibrvListener &>(listener).getSubject(subject_name);

	if (rv_status != TIBRV_OK)
		MLB::RvUtilX::ThrowTibrvStatus(rv_status,
			"Attempt to retrieve listener subject name failed.");

	return(subject_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &GetSubject(const TibrvListener &listener, std::string &out_subject)
{
	return(out_subject.assign(GetSubjectPtr(listener)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string GetSubject(const TibrvListener &listener)
{
	std::string out_subject;

	return(GetSubject(listener, out_subject));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\note Function taking a TibrvListener pointer is kept so as to maintain
			compatibility with existing code.
*/
TibrvTransport *GetTransport(TibrvListener *listener)
{
	TibrvTransport *out_transport =
		const_cast<TibrvTransport *>(listener->getTransport());

	if (out_transport == NULL)
		throw RvException("Invocation of 'TibrvListener::getTransport()' failed "
			"because the method returned 'NULL' --- the transport has probably "
			"not been created.");

	return(out_transport);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TibrvTransport *GetTransport(TibrvListener &listener)
{
	return(GetTransport(&listener));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

