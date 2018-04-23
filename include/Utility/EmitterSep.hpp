// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	EmitterSep.hpp

	File Description	:	Definition of the EmitterSep class.

	Revision History	:	2016-02-18 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__EmitterSep_hpp__HH

#define HH__MLB__Utility__EmitterSep_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>
#include <Utility/IoCharInfo.hpp>

#include <iomanip>
#include <ostream>

#if defined(_MSC_VER) && (_MSC_VER <= 1500)
# pragma warning(push)
# pragma warning(disable: 4512)
#endif // #if defined(_MSC_VER) && (_MSC_VER <= 1500)

#include <boost/io/ios_state.hpp>

#if defined(_MSC_VER) && (_MSC_VER <= 1500)
# pragma warning(pop)
#endif // #if defined(_MSC_VER) && (_MSC_VER <= 1500)

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
template <typename CharType = char>
	class basic_EmitterSep {
public:
	typedef CharType type;

	explicit basic_EmitterSep(CharType sep_char = '*',
		std::size_t sep_length = 79, bool end_line = true)
		:sep_char_(sep_char)
		,sep_length_(sep_length)
		,end_line_(end_line)
	{
	}

	CharType    SepChar() const
	{
		return(sep_char_);
	}

	std::size_t SepLength() const
	{
		return(sep_length_);
	}

	bool        EndLine() const
	{
		return(end_line_);
	}

private:
	CharType    sep_char_;
	std::size_t sep_length_;
	bool        end_line_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef basic_EmitterSep<char> EmitterSep;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename CharType>
	inline typename IoCharInfo<CharType>::ostream_type & operator << (
		typename IoCharInfo<CharType>::ostream_type &o_str,
		const basic_EmitterSep<CharType> &datum)
{
	if (datum.SepLength()) {
		boost::io::basic_ios_all_saver<CharType> io_state(o_str);
		o_str << std::setfill(datum.SepChar()) <<
			std::setw(static_cast<std::streamsize>(datum.SepLength())) <<
			typename IoCharInfo<CharType>::string_type();
		if (datum.EndLine())
			o_str << std::endl;
	}

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__EmitterSep_hpp__HH

