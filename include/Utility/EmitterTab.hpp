// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	EmitterTab.hpp

	File Description	:	Definition of the EmitterTab class.

	Revision History	:	2016-02-18 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__EmitterTab_hpp__HH

#define HH__MLB__Utility__EmitterTab_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/EmitterSep.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
template <typename CharType = char>
	class basic_EmitterTab {
public:
	typedef CharType type;

	explicit basic_EmitterTab(std::size_t tab_count, bool use_tabs = true,
		std::size_t tab_size = 8)
		:tab_count_(tab_count)
		,use_tabs_(use_tabs)
		,tab_size_(tab_size)
	{
	}

	std::size_t TabCount() const
	{
		return(tab_count_);
	}

	bool UseTabs() const
	{
		return(use_tabs_);
	}

	std::size_t TabSize() const
	{
		return(tab_size_);
	}

	CharType FillChar() const
	{
		return(static_cast<CharType>((UseTabs()) ? '\t' : ' '));
	}

	basic_EmitterTab operator() (std::size_t tab_count)
	{
		tab_count_ = tab_count;

		return(*this);
	}

private:
	std::size_t tab_count_;
	bool        use_tabs_;
	std::size_t tab_size_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef basic_EmitterTab<char> EmitterTab;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename CharType>
	inline typename IoCharInfo<CharType>::ostream_type & operator << (
		typename IoCharInfo<CharType>::ostream_type &o_str,
		const basic_EmitterTab<CharType> &datum)
{
	if (datum.TabCount() && (datum.UseTabs() || datum.TabSize())) {
		boost::io::basic_ios_all_saver<CharType> io_state(o_str);
		if (datum.UseTabs())
			o_str << std::setfill(datum.FillChar()) <<
				std::setw(static_cast<std::streamsize>(datum.TabCount())) <<
				typename IoCharInfo<CharType>::string_type();
		else
			o_str << std::setfill(datum.FillChar()) <<
				std::setw(static_cast<std::streamsize>(
				datum.TabCount() * datum.TabSize())) <<
				typename IoCharInfo<CharType>::string_type();
	}

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__EmitterTab_hpp__HH

