//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for some generic floating-point support
								functionality.

	Revision History	:	2010-10-23 --- Creation.
									Michael L. Brock

		Copyright (c) 1998-2000 Theodore C. Belding
		University of Michigan Center for the Study of Complex Systems
		<mailto:Ted.Belding@umich.edu>
		<http://www-personal.umich.edu/~streak/>		

		This file is part of the fcmp distribution. fcmp is free software;
		you can redistribute and modify it under the terms of the GNU Library
		General Public License (LGPL), version 2 or later.  This software
		comes with absolutely no warranty. See the file COPYING for details
		and terms of copying.

		Portions Copyright Michael L. Brock 2010 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
#ifndef HH__MLB__Utility__FloatingPointSupport_hpp__HH

#define HH__MLB__Utility__FloatingPointSupport_hpp__HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <mbcompat.h>

#include <cmath>
#include <limits>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
/**
	A template version of the fcmp() function by Theodore C. Belding.

	Copyright (c) 1998-2000 Theodore C. Belding
	University of Michigan Center for the Study of Complex Systems
	<mailto:Ted.Belding@umich.edu>
	<http://www-personal.umich.edu/~streak/>		

	This file is part of the fcmp distribution. fcmp is free software;
	you can redistribute and modify it under the terms of the GNU Library
	General Public License (LGPL), version 2 or later.  This software
	comes with absolutely no warranty. See the file COPYING for details
	and terms of copying.
*/
template <typename FPType>
	int FloatingPointCompare(FPType x1, FPType x2, FPType epsilon =
		std::numeric_limits<FPType>::epsilon())
{
	int    exponent;
	FPType delta;
	FPType difference;
  
	/* Get exponent(max(fabs(x1), fabs(x2))) and store it in exponent. */

	/* If neither x1 nor x2 is 0, */
	/* this is equivalent to max(exponent(x1), exponent(x2)). */

	/* If either x1 or x2 is 0, its exponent returned by frexp would be 0, */
	/* which is much larger than the exponents of numbers close to 0 in */
	/* magnitude. But the exponent of 0 should be less than any number */
	/* whose magnitude is greater than 0. */
  
	/* So we only want to set exponent to 0 if both x1 and */
	/* x2 are 0. Hence, the following works for all x1 and x2. */

	::frexp(::fabs(x1) > ::fabs(x2) ? x1 : x2, &exponent);

	/* Do the comparison. */

	/* delta = epsilon * pow(2, exponent) */

	/* Form a neighborhood around x2 of size delta in either direction. */
	/* If x1 is within this delta neighborhood of x2, x1 == x2. */
	/* Otherwise x1 > x2 or x1 < x2, depending on which side of */
	/* the neighborhood x1 is on. */
  
	delta = ::ldexp(epsilon, exponent); 
  
	difference = x1 - x2;

	if (difference > delta)
		return 1; /* x1 > x2 */
	else if (difference < -delta) 
		return -1;  /* x1 < x2 */
	else /* -delta <= difference <= delta */
		return 0;  /* x1 == x2 */
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename FPType = double> struct FloatingPointCompareLess {
	FloatingPointCompareLess(FPType epsilon =
		std::numeric_limits<FPType>::epsilon())
		:epsilon_(epsilon)
	{
	}

	bool operator () (const FPType &lhs, const FPType &rhs) const
	{
		return(FloatingPointCompare<FPType>(lhs, rhs, epsilon_));
	}

	FPType epsilon_;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__FloatingPointSupport_hpp__HH

