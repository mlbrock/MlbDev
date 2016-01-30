//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SocketIo Portable Socket Library Source Module
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation file for resolving a service.

								Based on the original MLB NPSL code as modified for
								use in the MLB SockLibX library.

	Revision History	:	2008-07-31 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SocketIo/ResolveService.hpp>

#include <Utility/ParseCmdLineArg.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

//	////////////////////////////////////////////////////////////////////////////
#if defined(__SVR4)
# define MLB_SOCKET_IO_RESOLVE_SERV_CPP_HAS__getservbyname_r
# define MLB_SOCKET_IO_RESOLVE_SERV_CPP_HAS__getservbyport_r
# define MLB_SOCKET_IO_RESOLVE_SERV_CPP_HAS_REENTRANT_FUNCTION
#elif __linux__
# define MLB_SOCKET_IO_RESOLVE_SERV_CPP_HAS__getservbyname_r
# define MLB_SOCKET_IO_RESOLVE_SERV_CPP_HAS__getservbyport_r
# define MLB_SOCKET_IO_RESOLVE_SERV_CPP_HAS_REENTRANT_FUNCTION
#endif // #if defined(__SVR4)
//	////////////////////////////////////////////////////////////////////////////

#ifdef MLB_SOCKET_IO_RESOLVE_SERV_CPP_HAS_REENTRANT_FUNCTION
namespace {
//	////////////////////////////////////////////////////////////////////////////
struct ResolveServiceNullDeleter {
	void operator()(void const *) const {}
};
//	////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace
#endif // #ifdef MLB_SOCKET_IO_RESOLVE_SERV_CPP_HAS_REENTRANT_FUNCTION

namespace {
//	////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	IpPortType ResolveServiceFromInt(DatumType service)
{
	if ((service < std::numeric_limits<IpPortType>::min()) ||
		(service > std::numeric_limits<IpPortType>::max()))
		MLB::Utility::ThrowInvalidArgument("Specified service/port value (" +
			MLB::Utility::AnyToString(service) + ") is outside of the "
			"permissible range (from " + 
			MLB::Utility::AnyToString(std::numeric_limits<IpPortType>::min()) +
			" to " +
			MLB::Utility::AnyToString(std::numeric_limits<IpPortType>::max()) +
			", inclusive).");

	return(static_cast<IpPortType>(service));
}
//	////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
ServiceEntry ServiceNameToEntry(const char *service, const char *protocol)
{
	if ((service == NULL) || (!(*service)))
		MLB::Utility::ThrowInvalidArgument("The service name specified for "
			"resolution is " + std::string((service == NULL) ? "NULL." :
			"an empty string."));

	ServiceEntry out_service_entry;

	try {
		servent                 *service_entry_ptr = NULL;
#ifdef MLB_SOCKET_IO_RESOLVE_SERV_CPP_HAS__getservbyname_r
		servent                  service_entry;
		char                     tmp_buffer[0xFFFF + 1];
		std::size_t              buffer_length = sizeof(tmp_buffer);
		boost::shared_ptr<char>  buffer_sptr(tmp_buffer,
			ResolveServiceNullDeleter());
		for ( ; ; ) {
# if defined(__SVR4)
			if ((service_entry_ptr = ::getservbyname_r(service, protocol,
				&service_entry, buffer_sptr.get(),
				static_cast<int>(buffer_length))) != NULL)
				break;
			else if (errno != ERANGE)
				MLB::Utility::ThrowSystemError("Call to 'getservbyname_r()' "
					"failed");
			else if (buffer_length >= (1 << 20))
				MLB::Utility::ThrowSystemError("Call to 'getservbyname_r()' "
					"failed with 'ERANGE' and the maximum provisional buffer size "
					"was reached");
# elif __linux__
			if (::getservbyname_r(service, protocol, &service_entry,
				buffer_sptr.get(), static_cast<int>(buffer_length),
				&service_entry_ptr) == 0) {
				if (service_entry_ptr == NULL)
					MLB::Utility::ThrowSystemError("Unable to locate service with "
						"'getservbyname_r()'.");
				break;
			}
			else if (errno != ERANGE)
				MLB::Utility::ThrowSystemError("Call to 'getservbyname_r()' "
					"failed");
			else if (buffer_length >= (1 << 20))
				MLB::Utility::ThrowSystemError("Call to 'getservbyname_r()' "
					"failed with 'ERANGE' and the maximum provisional buffer size "
					"was reached");
# endif // #if defined(__SVR4)
			buffer_length += 0xFFFF + 1;
			buffer_sptr.reset(new char[buffer_length]);
		}
#else
		if ((service_entry_ptr = ::getservbyname(service, protocol)) == NULL)
			MLB::Utility::ThrowSystemError("Unable to locate service with "
				"'getservbyname()'.");
#endif // #ifdef MLB_SOCKET_IO_RESOLVE_SERV_CPP_HAS__getservbyname_r
		ServiceEntry(service_entry_ptr).swap(out_service_entry);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to resolve service name '" +
			std::string(service) + "', protocol name '" +
			std::string((protocol == NULL) ? "" : protocol) + "': " +
			std::string(except.what()));
	}

	return(out_service_entry);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ServiceEntry ServiceNameToEntry(const std::string &service,
	const std::string &protocol)
{
	return(ServiceNameToEntry(service.c_str(),
		(protocol.empty()) ? NULL : protocol.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ServiceEntry ServicePortToEntry(int service, const char *protocol)
{
	ServiceEntry out_service_entry;

	try {
		servent                 *service_entry_ptr = NULL;
#ifdef MLB_SOCKET_IO_RESOLVE_SERV_CPP_HAS__getservbyport_r
		servent                  service_entry;
		char                     tmp_buffer[0xFFFF + 1];
		std::size_t              buffer_length = sizeof(tmp_buffer);
		boost::shared_ptr<char>  buffer_sptr(tmp_buffer,
			ResolveServiceNullDeleter());
		for ( ; ; ) {
# if defined(__SVR4)
			if ((service_entry_ptr = ::getservbyport_r(service, protocol,
				&service_entry, buffer_sptr.get(),
				static_cast<int>(buffer_length))) != NULL)
				break;
			else if (errno != ERANGE)
				MLB::Utility::ThrowSystemError("Call to 'getservbyport_r()' "
					"failed");
			else if (buffer_length >= (1 << 20))
				MLB::Utility::ThrowSystemError("Call to 'getservbyport_r()' "
					"failed with 'ERANGE' and the maximum provisional buffer size "
					"was reached");
# elif __linux__
			if (::getservbyport_r(service, protocol, &service_entry,
				buffer_sptr.get(), static_cast<int>(buffer_length),
				&service_entry_ptr) == 0) {
				if (service_entry_ptr == NULL)
					MLB::Utility::ThrowSystemError("Unable to locate service with "
						"'getservbyport_r()'.");
				break;
			}
			else if (errno != ERANGE)
				MLB::Utility::ThrowSystemError("Call to 'getservbyport_r()' "
					"failed");
			else if (buffer_length >= (1 << 20))
				MLB::Utility::ThrowSystemError("Call to 'getservbyport_r()' "
					"failed with 'ERANGE' and the maximum provisional buffer size "
					"was reached");
# endif // #if defined(__SVR4)
			buffer_length += 0xFFFF + 1;
			buffer_sptr.reset(new char[buffer_length]);
		}
#else
		if ((service_entry_ptr = ::getservbyport(service, protocol)) == NULL)
			MLB::Utility::ThrowSystemError("Unable to locate service with "
				"'getservbyport()'.");
#endif // #ifdef MLB_SOCKET_IO_RESOLVE_SERV_CPP_HAS__getservbyport_r
		ServiceEntry(service_entry_ptr).swap(out_service_entry);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to resolve service port number '" +
			MLB::Utility::AnyToString(service) + "', protocol name '" +
			std::string((protocol == NULL) ? "" : protocol) + "': " +
			std::string(except.what()));
	}

	return(out_service_entry);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ServiceEntry ServicePortToEntry(int service, std::string &protocol)
{
	return(ServicePortToEntry(service,
		(protocol.empty()) ? NULL : protocol.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ServiceEntry ServicePortToEntry(const char *service, const char *protocol)
{
	if ((service == NULL) || (!(*service)))
		MLB::Utility::ThrowInvalidArgument("The service port number specified "
			"for resolution is " + std::string((service == NULL) ? "NULL." :
			"an empty string."));

	int tmp_service = 0;

	try {
		tmp_service = MLB::Utility::CheckIsNumericString<int>(service);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to resolve the service port "
			"number '" + std::string(service) + "': " +
			std::string(except.what()));
	}

	return(ServicePortToEntry(tmp_service, protocol));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ServiceEntry ServicePortToEntry(const std::string &service,
	const std::string &protocol)
{
	return(ServicePortToEntry(service.c_str(),
		(protocol.empty()) ? NULL : protocol.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ServiceEntry ServiceToEntry(const char *service, const char *protocol)
{
	return(((service != NULL) && *service && std::isdigit(*service)) ?
		ServicePortToEntry(service, protocol) :
		ServiceNameToEntry(service, protocol));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
ServiceEntry ServiceToEntry(const std::string &service,
	const std::string &protocol)
{
	return(ServiceToEntry(service.c_str(),
		(protocol.empty()) ? NULL : protocol.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
IpPortType ResolveService(const char *service, const char *protocol)
{
	if (std::isdigit(*service)) {
		try {
			return(MLB::Utility::CheckIsNumericString<IpPortType>(service));
		}
		catch (const std::exception &except) {
			MLB::Utility::Rethrow(except, "Unable to resolve service name '" +
				std::string(service) + "', protocol name '" +
				std::string((protocol == NULL) ? "" : protocol) + "': " +
				std::string(except.what()));
		}
	}

	return(ntohs(static_cast<IpPortType>(
		ServiceToEntry(service, protocol).s_port)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
IpPortType ResolveService(const std::string &service,
	const std::string &protocol)
{
	return(ResolveService(service.c_str(),
		(protocol.empty()) ? NULL : protocol.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
IpPortType ResolveService(unsigned int service)
{
	return(ResolveServiceFromInt(service));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
IpPortType ResolveService(int service)
{
	return(ResolveServiceFromInt(service));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::SocketIo;

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		for (int count_1 = 1; count_1 < argc; ++count_1)
			std::cout << "Service name [" << argv[count_1] << "] = [" <<
				ResolveService(argv[count_1]) << "]" << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

