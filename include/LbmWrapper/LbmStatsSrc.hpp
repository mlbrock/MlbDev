// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	29 West LBM API Wrapper Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the LbmStatsSrc class.

	Revision History	:	2008-08-03 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__LbmWrapper__LbmStatsSrc_hpp__HH

#define HH__MLB__LbmWrapper__LbmStatsSrc_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <LbmWrapper/LbmWrapper.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LbmWrapper {

//	////////////////////////////////////////////////////////////////////////////
class LbmStatsSrc : public LbmStatsBase<LbmSrc> {
	void Initialize();

public:
	typedef std::vector<LbmStatsSrc>           LbmStatsSrcVectorI;
	typedef LbmStatsSrcVectorI::iterator       LbmStatsSrcVectorIIter;
	typedef LbmStatsSrcVectorI::const_iterator LbmStatsSrcVectorIIterC;

	explicit LbmStatsSrc(MyDatumType &datum);
	explicit LbmStatsSrc(MyLbmApiType *datum);
	         LbmStatsSrc(const MyLbmApiStatsType &stats);

	std::string ToString(unsigned int text_width = MaxStatsTextWidth,
		bool fixup_min = true) const;

	static LbmStatsSrcVectorI GetStatsForContext(LbmContext &context);
	static LbmStatsSrcVectorI GetStatsForContext(lbm_context_t *context);
	static LbmStatsSrcVectorI GetImStatsForContext(LbmContext &context);
	static LbmStatsSrcVectorI GetImStatsForContext(lbm_context_t *context);

	static const unsigned int MaxStatsTextWidth = 50;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef LbmStatsSrc::LbmStatsSrcVectorI      LbmStatsSrcVector;
typedef LbmStatsSrc::LbmStatsSrcVectorIIter  LbmStatsSrcVectorIter;
typedef LbmStatsSrc::LbmStatsSrcVectorIIterC LbmStatsSrcVectorIterC;
//	////////////////////////////////////////////////////////////////////////////

} // namespace LbmWrapper

} // namespace MLB

#endif // #ifndef HH__MLB__LbmWrapper__LbmStatsSrc_hpp__HH

