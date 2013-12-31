// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	29 West LBM API Wrapper Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the LbmStatsRcv class.

	Revision History	:	2008-08-03 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__LbmWrapper__LbmStatsRcv_hpp__HH

#define HH__MLB__LbmWrapper__LbmStatsRcv_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <LbmWrapper/LbmWrapper.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LbmWrapper {

//	////////////////////////////////////////////////////////////////////////////
class LbmStatsRcv : public LbmStatsBase<LbmRcv> {
	void Initialize(const char *source);

public:
	typedef std::vector<LbmStatsRcv>           LbmStatsRcvVectorI;
	typedef LbmStatsRcvVectorI::iterator       LbmStatsRcvVectorIIter;
	typedef LbmStatsRcvVectorI::const_iterator LbmStatsRcvVectorIIterC;

	explicit LbmStatsRcv(MyDatumType &datum, const char *source = NULL);
	explicit LbmStatsRcv(MyLbmApiType *datum, const char *source = NULL);
	         LbmStatsRcv(const MyLbmApiStatsType &stats);

	std::string ToString(unsigned int text_width = MaxStatsTextWidth,
		bool fixup_min = true) const;

	static LbmStatsRcvVectorI GetStatsForContext(LbmContext &context);
	static LbmStatsRcvVectorI GetStatsForContext(lbm_context_t *context);
	static LbmStatsRcvVectorI GetImStatsForContext(LbmContext &context);
	static LbmStatsRcvVectorI GetImStatsForContext(lbm_context_t *context);
	static LbmStatsRcvVectorI GetStatsForRcv(LbmRcv &receiver);
	static LbmStatsRcvVectorI GetStatsForRcv(lbm_rcv_t *receiver);

	static const unsigned int MaxStatsTextWidth = 65;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef LbmStatsRcv::LbmStatsRcvVectorI      LbmStatsRcvVector;
typedef LbmStatsRcv::LbmStatsRcvVectorIIter  LbmStatsRcvVectorIter;
typedef LbmStatsRcv::LbmStatsRcvVectorIIterC LbmStatsRcvVectorIterC;
//	////////////////////////////////////////////////////////////////////////////

} // namespace LbmWrapper

} // namespace MLB

#endif // #ifndef HH__MLB__LbmWrapper__LbmStatsRcv_hpp__HH

