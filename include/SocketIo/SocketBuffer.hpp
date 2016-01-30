//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	SocketIo Portable Socket Library Include File.
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for class SocketBuffer.

	Revision History	:	2002-01-26 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2002 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__SocketIo__SocketBuffer_hpp__HH

#define HH__MLB__SocketIo__SocketBuffer_hpp__HH		1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files . . .
//	////////////////////////////////////////////////////////////////////////////

#include <SocketIo.hpp>

#ifdef _Windows
# pragma warning(disable:4668)
# pragma warning(push)
# pragma warning(disable:4217)
#endif // #ifdef _Windows

#include <boost/shared_ptr.hpp>

#ifdef _Windows
# pragma warning(pop)
#endif // #ifdef _Windows

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace SocketIo {

// ////////////////////////////////////////////////////////////////////////////
class SocketBuffer {
	struct MyNullDeleter {
		void operator()(void const *) const {}
	};
public:
	SocketBuffer(std::size_t buffer_size,
		boost::shared_ptr<char> &buffer_sptr)
		:buffer_size_(CheckSize(buffer_size))
		,buffer_sptr_(buffer_sptr)
	{
		CheckPtr(buffer_sptr_.get());
	}
	SocketBuffer(std::size_t buffer_size, char *buffer_ptr)
		:buffer_size_(CheckSize(buffer_size))
		,buffer_sptr_(static_cast<char *>(CheckPtr(buffer_ptr)), MyNullDeleter())
	{
	}
	SocketBuffer(std::size_t buffer_size)
		:buffer_size_(CheckSize(buffer_size))
		,buffer_sptr_(new char[buffer_size_])
	{
	}

	inline std::size_t                    GetSize() const
	{
		return(buffer_size_);
	}
	inline const boost::shared_ptr<char> &GetSPtr() const
	{
		return(buffer_sptr_);
	}
	inline const void                    *GetPtr() const
	{
		return(buffer_sptr_.get());
	}
	inline boost::shared_ptr<char>       &GetSPtr()
	{
		return(buffer_sptr_);
	}
	inline void                          *GetPtr()
	{
		return(buffer_sptr_.get());
	}

	std::size_t             buffer_size_;
	boost::shared_ptr<char> buffer_sptr_;

	static void *CheckPtr(void *buffer_ptr)
	{
		if (buffer_ptr == NULL)
			MLB::Utility::ThrowInvalidArgument("Invalid asynchronous buffer "
				"specified: the buffer pointer is 'NULL'.");

		return(buffer_ptr);
	}

	static std::size_t CheckSize(std::size_t buffer_size)
	{
		if (!buffer_size)
			MLB::Utility::ThrowInvalidArgument("Invalid asynchronous buffer "
				"specified: the buffer size is zero.");

		return(buffer_size);
	}
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace SocketIo

} // namespace MLB

#endif // #ifndef HH__MLB__SocketIo__SocketBuffer_hpp__HH

