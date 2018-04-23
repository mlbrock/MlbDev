//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Boost Python Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the Boost::Python interface to the
								TimeVal class.

	Revision History	:	2007-10-19 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2007 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <BoostX/BoostPython.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {

// ////////////////////////////////////////////////////////////////////////////
std::string WRAP_TimeVal_ToString(TimeVal &datum,
	unsigned int max_length = Length_TimeVal)
{
	return(datum.ToString(max_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string WRAP_TimeVal_ToStringLocal(TimeVal &datum,
	unsigned int max_length = Length_TimeVal)
{
	return(datum.ToStringLocal(max_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string WRAP_TimeVal_ToStringInterval(TimeVal &datum,
	unsigned int max_length = LengthInterval_TimeVal)
{
	return(datum.ToStringInterval(max_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_FUNCTION_OVERLOADS(OLS_TimeVal_ToString,         WRAP_TimeVal_ToString,         1, 2)
BOOST_PYTHON_FUNCTION_OVERLOADS(OLS_TimeVal_ToStringLocal,    WRAP_TimeVal_ToStringLocal,    1, 2)
BOOST_PYTHON_FUNCTION_OVERLOADS(OLS_TimeVal_ToStringInterval, WRAP_TimeVal_ToStringInterval, 1, 2)
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
void MLB_Utility_TimeValPython()
{
	boost::python::class_<TimeVal>("TimeVal")
  		.def(boost::python::init<>())
  		.def(boost::python::init<
			time_t,
			boost::python::optional<long> >())
  		.def(boost::python::init<
			std::string>())
  		.def(boost::python::init<TimeVal>())
		.def("__str__",
			WRAP_TimeVal_ToString,
			OLS_TimeVal_ToString())

		.def(boost::python::self == boost::python::other<TimeVal>())
		.def(boost::python::self != boost::python::other<TimeVal>())
		.def(boost::python::self <  boost::python::other<TimeVal>())
		.def(boost::python::self >  boost::python::other<TimeVal>())
		.def(boost::python::self <= boost::python::other<TimeVal>())
		.def(boost::python::self >= boost::python::other<TimeVal>())

		.def("SetToNow",
			TimeVal::SetToNow,
			boost::python::return_self<>())
		.def("SetToMinimumValue",
			TimeVal::SetToMinimumValue,
			boost::python::return_self<>())
		.def("SetToMaximumValue",
			TimeVal::SetToMaximumValue,
			boost::python::return_self<>())

		.def("IsZero",
			TimeVal::IsZero)

		.def("AddSeconds",
			TimeVal::AddSeconds,
			boost::python::return_self<>())
		.def("AddMilliseconds",
			TimeVal::AddMilliseconds,
			boost::python::return_self<>())
		.def("AddMicroseconds",
			TimeVal::AddMicroseconds,
			boost::python::return_self<>())
		.def("AddNanoseconds",
			TimeVal::AddNanoseconds,
			boost::python::return_self<>())

		.def("ToTimeT",
			TimeVal::ToTimeT)

		.def("GetDoubleEquivalent",
			TimeVal::GetDoubleEquivalent)
		.def("GetDoubleTicks",
			TimeVal::GetDoubleTicks)

		.def("ToString",
			WRAP_TimeVal_ToString,
			OLS_TimeVal_ToString())
		.def("ToStringLocal",
			WRAP_TimeVal_ToStringLocal,
			OLS_TimeVal_ToStringLocal())
		.def("ToStringInterval",
			WRAP_TimeVal_ToStringInterval,
			OLS_TimeVal_ToStringInterval())

		.def("GetDifferenceTicksDouble",
			TimeVal::GetDifferenceTicksDouble)
		.staticmethod("GetDifferenceTicksDouble")
		.def("GetDifferenceDouble",
			TimeVal::GetDifferenceDouble)
		.staticmethod("GetDifferenceDouble")

		.def("GetDifference",
			TimeVal::GetDifference)
		.staticmethod("GetDifference")
		.def("GetDifferenceAbs",
			TimeVal::GetDifferenceAbs)
		.staticmethod("GetDifferenceAbs")

		.def("Now",
			TimeVal::Now,
			boost::python::return_value_policy<boost::python::return_by_value>())
		.staticmethod("Now")

		.def("GetMinimumValue",
			TimeVal::GetMinimumValue,
			boost::python::return_value_policy<boost::python::return_by_value>())
		.staticmethod("GetMinimumValue")
		.def("GetMaximumValue",
			TimeVal::GetMaximumValue,
			boost::python::return_value_policy<boost::python::return_by_value>())
		.staticmethod("GetMaximumValue")
		;
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

