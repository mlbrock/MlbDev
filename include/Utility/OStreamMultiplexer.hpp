// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the OStreamMultiplexer and related
								classes.

	Revision History	:	2006-11-15 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2006 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__Utility_OStreamMultiplexer_hpp__HH

#define HH__MLB__Utility__Utility_OStreamMultiplexer_hpp__HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

#include <fstream>
#include <iomanip>
#include <set>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# if _MSC_VER < 1400
#  pragma warning(disable:4061 4242 4244 4302 4512)
# endif // # if _MSC_VER < 1400
# pragma warning(disable:4217 4275 4625 4626)
# if _MSC_VER >= 1500
#  pragma warning(disable:4061 4242 4244 4365)
# endif // # if _MSC_VER >= 1500
#endif // #if defined(_Windows) && !defined(__MINGW32__)

#include <boost/thread/mutex.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
class API_UTILITY OStreamPortList {
	typedef boost::mutex                   LockType;
	typedef LockType::scoped_lock          ScopedLockType;
	typedef std::set<std::ostream *>       OStreamPortSet;
	typedef OStreamPortSet::iterator       OStreamPortSetIter;
	typedef OStreamPortSet::const_iterator OStreamPortSetIterC;

public:
	OStreamPortList();
	~OStreamPortList();

	void AddOStream(std::ostream *ostream_ptr);
	void RemoveOStream(std::ostream *ostream_ptr);
	void RemoveAll();
	void RemoveAllExcept(std::ostream *ostream_ptr);

	int DoPutChar(int chr);
	int DoSync();

private:
	MLB_Utility_NonCopyable_Macro(OStreamPortList);

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4251)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
	LockType       the_lock_;
	OStreamPortSet ostream_port_set_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

	void CheckNullPtrArg(std::ostream *ostream_ptr) const;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_UTILITY OStreamMultiplexerBuffer : public std::streambuf {
public:
	OStreamMultiplexerBuffer(OStreamPortList &ostream_port_list);

protected:
	std::streambuf::int_type overflow(int c);
	int                      sync();

private:

	OStreamPortList &ostream_port_list_;

	MLB_Utility_NonCopyable_Macro(OStreamMultiplexerBuffer);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_UTILITY OStreamMultiplexer : public std::ostream {
public:
	OStreamMultiplexer(std::ostream *ostream_ptr = NULL);

	void AddOStream(std::ostream *ostream_ptr);
	void RemoveOStream(std::ostream *ostream_ptr);

private:
	MLB_Utility_NonCopyable_Macro(OStreamMultiplexer);

	OStreamPortList ostream_port_list_;

	OStreamPortList &GetOStreamPortListRef();
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__Utility_OStreamMultiplexer_hpp__HH

