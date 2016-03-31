// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	IoCharInfo.hpp

	File Description	:	Definition of the IoCharInfo class.

	Revision History	:	2016-02-18 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__IoCharInfo_hpp__HH

#define HH__MLB__Utility__IoCharInfo_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <mbcompat.h>

#include <iosfwd>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
template <typename CharType> struct IoCharInfo {
private:
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <> struct IoCharInfo<char> {
	typedef char               char_type;
	typedef std::filebuf       filebuf_type;
	typedef std::fstream       fstream_type;
	typedef std::ifstream      ifstream_type;
	typedef std::ios           ios_type;
	typedef std::iostream      iostream_type;
	typedef std::istream       istream_type;
	typedef std::istringstream istringstream_type;
	typedef std::ofstream      ofstream_type;
	typedef std::ostream       ostream_type;
	typedef std::ostringstream ostringstream_type;
	typedef std::streambuf     streambuf_type;
	typedef std::streampos     streampos_type;
	typedef std::string        string_type;
	typedef std::stringbuf     stringbuf_type;
	typedef std::stringstream  stringstream_type;

	static istream_type &Cin()  { return(std::cin);  }
	static ostream_type &Cout() { return(std::cout); }
	static ostream_type &Cerr() { return(std::cerr); }
	static ostream_type &Clog() { return(std::clog); }
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <> struct IoCharInfo<wchar_t> {
	typedef wchar_t             char_type;
	typedef std::wfilebuf       filebuf_type;
	typedef std::wfstream       fstream_type;
	typedef std::wifstream      ifstream_type;
	typedef std::wios           ios_type;
	typedef std::wiostream      iostream_type;
	typedef std::wistream       istream_type;
	typedef std::wistringstream istringstream_type;
	typedef std::wofstream      ofstream_type;
	typedef std::wostream       ostream_type;
	typedef std::wostringstream ostringstream_type;
	typedef std::wstreambuf     streambuf_type;
	typedef std::wstreampos     streampos_type;
	typedef std::wstring        string_type;
	typedef std::wstringbuf     stringbuf_type;
	typedef std::wstringstream  stringstream_type;

	static istream_type &Cin()  { return(std::wcin);  }
	static ostream_type &Cout() { return(std::wcout); }
	static ostream_type &Cerr() { return(std::wcerr); }
	static ostream_type &Clog() { return(std::wclog); }
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__IoCharInfo_hpp__HH

