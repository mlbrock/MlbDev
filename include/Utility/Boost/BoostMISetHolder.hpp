// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Utility Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	BoostMISetHolder.hpp

	File Description	:	Definition of the BoostMISetHolder class template.

	Revision History	:	2015-07-14 --- Creation as the file
													MlbDev/include/VFix/VFixMISetHolder.hpp.
									Michael L. Brock
	Revision History	:	2016-08-06 --- Copied into MlbDev/include/Utility as
													file BoostMISetHolder.hpp.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__BoostMISetHolder_hpp__HH

#define HH__MLB__Utility__BoostMISetHolder_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file BoostMISetHolder.hpp

	\brief	The Utility header file containing the definition and
				implementation of the BoostMISetHolder class template.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <cstddef>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
template <typename MISetType> class BoostMISetHolder {
public:
	typedef typename MISetType::value_type value_type;

	BoostMISetHolder()
	{
	}

	MISetType &Get()
	{
		return(the_set_);
	}

	const MISetType &Get() const
	{
		return(the_set_);
	}

	void swap(BoostMISetHolder &other)
	{
		the_set_.swap(other.the_set_);
	}

	std::size_t size() const
	{
		return(the_set_.size());
	}

private:
	MISetType the_set_;
};
// //////////////////////////////////////////////////////////////////////////// 

} // namespace Utility

} // namespace MLB

#endif // #ifdef HH__MLB__Utility__BoostMISetHolder_hpp__HH

