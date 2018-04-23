//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	GpbElementInfoMaxLengths.hpp

	File Description	:	Definition of the GpbElementInfoMaxLengths class.

	Revision History	:	2016-06-06 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH___MLB__ProtoBuf__GpbElementInfoMaxLengths_hpp___HH

#define HH___MLB__ProtoBuf__GpbElementInfoMaxLengths_hpp___HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files...
//	////////////////////////////////////////////////////////////////////////////

#include <ProtoBuf/GpbEmitFlags.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

//	////////////////////////////////////////////////////////////////////////////
struct GpbElementInfoMaxLengths {
	enum MaxLengthsIndex {
		TypeNameFull,
		TypeName,
		CppNameFull,
		CppName,
		MemberName,
		Name,
		NameFull,
		TypeFileName,
		MemberFileName,
		FileName,
		LabelName,
		Count,
		Minimum = TypeNameFull,
		Maximum = Count - 1
	};

	GpbElementInfoMaxLengths();

	void FixMaxLengths(std::size_t max_depth,
		GpbEmitFlags::EmitFlags emit_flags);

	std::size_t CheckedIndex(MaxLengthsIndex idx) const;

	const std::size_t & operator [](MaxLengthsIndex idx) const;

	std::size_t & operator [](MaxLengthsIndex idx);

	std::streamsize width(MaxLengthsIndex idx) const;

	std::size_t max_length_[Count];
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB

#endif // #ifndef HH___MLB__ProtoBuf__GpbElementInfoMaxLengths_hpp___HH

