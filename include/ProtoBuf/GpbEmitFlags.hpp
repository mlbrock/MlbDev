//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Google Protocol Buffers Support Library
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	GpbEmitFlags.hpp

	File Description	:	Definition of the GpbEmitFlags class.

	Revision History	:	2016-06-06 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2016 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////

#ifndef HH___MLB__ProtoBuf__GpbEmitFlags_hpp___HH

#define HH___MLB__ProtoBuf__GpbEmitFlags_hpp___HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Include necessary header files...
//	////////////////////////////////////////////////////////////////////////////

#include <ProtoBuf/ProtoBuf.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace ProtoBuf {

//	////////////////////////////////////////////////////////////////////////////
struct GpbEmitFlags {
	enum EmitFlags {
		None           = 0x0000,
		Csv            = 0x0001,
		IndentType     = 0x0002,
		IndentName     = 0x0004,
		EnumValues     = 0x0008,
		FullName       = 0x0010,
		TypeFileName   = 0x0020,
		MemberFileName = 0x0040,
		Maximum        = MemberFileName,
		Default        = IndentType | IndentName,
		Mask           = (Maximum | (Maximum - 1))
	};

	static inline EmitFlags SetFlag(EmitFlags flags, EmitFlags set_flag)
	{
		return(static_cast<EmitFlags>(flags | set_flag));
	}

	static inline EmitFlags ClearFlag(EmitFlags flags, EmitFlags clear_flag)
	{
		return(static_cast<EmitFlags>(flags & ~clear_flag));
	}
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace ProtoBuf

} // namespace MLB

#endif // #ifndef HH___MLB__ProtoBuf__GpbEmitFlags_hpp___HH

