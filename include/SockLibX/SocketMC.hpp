//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SockLibX Portable Socket Library Include File.
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for class SocketMC.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SockLibX__SocketMC_hpp__HH

#define HH__MLB__SockLibX__SocketMC_hpp__HH		1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SockLibX/SocketX.hpp>
#include <SockLibX/SocketSpec.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4217 4668)
# if _MSC_VER >= 1500
#  pragma warning(disable:4244 4571)
# endif // # if _MSC_VER >= 1500
#endif // #if defined(_Windows) && !defined(__MINGW32__)

#include <boost/shared_ptr.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SockLibX {

//	////////////////////////////////////////////////////////////////////////////
class SocketMC {
public:
	         SocketMC();
	explicit SocketMC(const SocketSpec &socket_spec);
	~SocketMC();

	bool       HasMCSpec() const;
	bool       HasMCJoined() const;
	void       CheckHasMCSpec() const;
	void       CheckHasMCJoined() const;
	SocketSpec GetSocketSpec() const;
	SocketSpec SetSocketSpec(const SocketSpec &socket_spec);
	void       JoinMulticastGroup();
	void       LeaveMulticastGroup();

	NativeSocketHandle GetSocketHandle() const;
	unsigned int       RecvFrom(char *buffer_ptr, unsigned int buffer_length,
		int flags, NativeSockAddr *addr, NativeSockLen_T *addrlen) const;
	unsigned int       SendTo(const char *buffer_ptr, unsigned int buffer_length,
		int flags, const NativeSockAddr *addr, NativeSockLen_T addrlen) const;

//private:
	SocketSpec                 socket_spec_;
	boost::shared_ptr<SocketX> mc_socket_sptr_;
	bool                       group_join_done_flag_;

	void ResetSocket();
	void AddDropSupport(int opt_level);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef std::vector<SocketMC>        SocketMCList;
typedef SocketMCList::iterator       SocketMCListIter;
typedef SocketMCList::const_iterator SocketMCListIterC;
//	////////////////////////////////////////////////////////////////////////////

} // namespace SockLibX

} // namespace MLB

#endif // #ifndef HH__MLB__SockLibX__SocketMC_hpp__HH

