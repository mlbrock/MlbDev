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
								TimeSpec class.

	Revision History	:	2007-10-19 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2007 - 2017.
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
std::string WRAP_TimeSpec_ToString(TimeSpec &datum,
	unsigned int max_length = Length_TimeSpec)
{
	return(datum.ToString(max_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string WRAP_TimeSpec_ToStringLocal(TimeSpec &datum,
	unsigned int max_length = Length_TimeSpec)
{
	return(datum.ToStringLocal(max_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string WRAP_TimeSpec_ToStringInterval(TimeSpec &datum,
	unsigned int max_length = LengthInterval_TimeSpec)
{
	return(datum.ToStringInterval(max_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_FUNCTION_OVERLOADS(OLS_TimeSpec_ToString,         WRAP_TimeSpec_ToString,         1, 2)
BOOST_PYTHON_FUNCTION_OVERLOADS(OLS_TimeSpec_ToStringLocal,    WRAP_TimeSpec_ToStringLocal,    1, 2)
BOOST_PYTHON_FUNCTION_OVERLOADS(OLS_TimeSpec_ToStringInterval, WRAP_TimeSpec_ToStringInterval, 1, 2)
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
void MLB_Utility_TimeSpecPython()
{
	boost::python::class_<TimeSpec>("TimeSpec")
  		.def(boost::python::init<>())
  		.def(boost::python::init<
			time_t,
			boost::python::optional<long> >())
  		.def(boost::python::init<
			std::string>())
  		.def(boost::python::init<TimeSpec>())
		.def("__str__",
			WRAP_TimeSpec_ToString,
			OLS_TimeSpec_ToString())

		.def(boost::python::self == boost::python::other<TimeSpec>())
		.def(boost::python::self != boost::python::other<TimeSpec>())
		.def(boost::python::self <  boost::python::other<TimeSpec>())
		.def(boost::python::self >  boost::python::other<TimeSpec>())
		.def(boost::python::self <= boost::python::other<TimeSpec>())
		.def(boost::python::self >= boost::python::other<TimeSpec>())

		.def("SetToNow",
			TimeSpec::SetToNow,
			boost::python::return_self<>())
		.def("SetToMinimumValue",
			TimeSpec::SetToMinimumValue,
			boost::python::return_self<>())
		.def("SetToMaximumValue",
			TimeSpec::SetToMaximumValue,
			boost::python::return_self<>())

		.def("IsZero",
			TimeSpec::IsZero)

		.def("AddSeconds",
			TimeSpec::AddSeconds,
			boost::python::return_self<>())
		.def("AddMilliseconds",
			TimeSpec::AddMilliseconds,
			boost::python::return_self<>())
		.def("AddMicroseconds",
			TimeSpec::AddMicroseconds,
			boost::python::return_self<>())
		.def("AddNanoseconds",
			TimeSpec::AddNanoseconds,
			boost::python::return_self<>())

		.def("ToTimeT",
			TimeSpec::ToTimeT)
		.def("ToTimeVal",
			TimeSpec::ToTimeVal)

		.def("GetDoubleEquivalent",
			TimeSpec::GetDoubleEquivalent)
		.def("GetDoubleTicks",
			TimeSpec::GetDoubleTicks)

		.def("ToString",
			WRAP_TimeSpec_ToString,
			OLS_TimeSpec_ToString())
		.def("ToStringLocal",
			WRAP_TimeSpec_ToStringLocal,
			OLS_TimeSpec_ToStringLocal())
		.def("ToStringInterval",
			WRAP_TimeSpec_ToStringInterval,
			OLS_TimeSpec_ToStringInterval())

		.def("GetDifferenceTicksDouble",
			TimeSpec::GetDifferenceTicksDouble)
		.staticmethod("GetDifferenceTicksDouble")
		.def("GetDifferenceDouble",
			TimeSpec::GetDifferenceDouble)
		.staticmethod("GetDifferenceDouble")

		.def("GetDifference",
			TimeSpec::GetDifference)
		.staticmethod("GetDifference")
		.def("GetDifferenceAbs",
			TimeSpec::GetDifferenceAbs)
		.staticmethod("GetDifferenceAbs")

		.def("Now",
			TimeSpec::Now,
			boost::python::return_value_policy<boost::python::return_by_value>())
		.staticmethod("Now")

		.def("GetMinimumValue",
			TimeSpec::GetMinimumValue,
			boost::python::return_value_policy<boost::python::return_by_value>())
		.staticmethod("GetMinimumValue")
		.def("GetMaximumValue",
			TimeSpec::GetMaximumValue,
			boost::python::return_value_policy<boost::python::return_by_value>())
		.staticmethod("GetMaximumValue")
		;
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

