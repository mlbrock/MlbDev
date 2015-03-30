// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB RapidXmlUtils Utilities Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	RapidXmlContext.hpp

	File Description	:	Definition of the RapidXmlContext class.

	Revision History	:	2015-03-21 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__RapidXmlUtils__RapidXmlContext_hpp__HH

#define HH__MLB__RapidXmlUtils__RapidXmlContext_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RapidXmlUtils/RapidXmlUtils.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RapidXmlUtils {

// ////////////////////////////////////////////////////////////////////////////
struct API_RAPIDXMLUTILS RapidXmlContext {
	RapidXmlContext();

	//**	Virtual to permit use as a base class to guarantee first construction.
	virtual ~RapidXmlContext();

	RapidXmlContext(const RapidXmlContext &other);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace RapidXmlUtils

} // namespace MLB

#endif // #ifndef HH__MLB__RapidXmlUtils__RapidXmlContext_hpp__HH
