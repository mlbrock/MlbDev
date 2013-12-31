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
								TimeT class.

	Revision History	:	2007-10-19 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2007 - 2014.
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
std::string WRAP_TimeT_ToString(TimeT &datum,
	unsigned int max_length = Length_TimeT)
{
	return(datum.ToString(max_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string WRAP_TimeT_ToStringLocal(TimeT &datum,
	unsigned int max_length = Length_TimeT)
{
	return(datum.ToStringLocal(max_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string WRAP_TimeT_ToStringInterval(TimeT &datum,
	unsigned int max_length = LengthInterval_TimeT)
{
	return(datum.ToStringInterval(max_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_FUNCTION_OVERLOADS(OLS_TimeT_ToString,         WRAP_TimeT_ToString,         1, 2)
BOOST_PYTHON_FUNCTION_OVERLOADS(OLS_TimeT_ToStringLocal,    WRAP_TimeT_ToStringLocal,    1, 2)
BOOST_PYTHON_FUNCTION_OVERLOADS(OLS_TimeT_ToStringInterval, WRAP_TimeT_ToStringInterval, 1, 2)
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
void MLB_Utility_TimeTPython()
{
	boost::python::class_<TimeT>("TimeT")
  		.def(boost::python::init<>())
  		.def(boost::python::init<
			time_t>())
  		.def(boost::python::init<
			std::string>())
  		.def(boost::python::init<TimeT>())
		.def("__str__",
			WRAP_TimeT_ToString,
			OLS_TimeT_ToString())

		.def(boost::python::self == boost::python::other<TimeT>())
		.def(boost::python::self != boost::python::other<TimeT>())
		.def(boost::python::self <  boost::python::other<TimeT>())
		.def(boost::python::self >  boost::python::other<TimeT>())
		.def(boost::python::self <= boost::python::other<TimeT>())
		.def(boost::python::self >= boost::python::other<TimeT>())

		.def("SetToNow",
			TimeT::SetToNow,
			boost::python::return_self<>())
		.def("SetToMinimumValue",
			TimeT::SetToMinimumValue,
			boost::python::return_self<>())
		.def("SetToMaximumValue",
			TimeT::SetToMaximumValue,
			boost::python::return_self<>())

		.def("IsZero",
			TimeT::IsZero)

		.def("AddSeconds",
			TimeT::AddSeconds,
			boost::python::return_self<>())
		.def("AddMilliseconds",
			TimeT::AddMilliseconds,
			boost::python::return_self<>())
		.def("AddMicroseconds",
			TimeT::AddMicroseconds,
			boost::python::return_self<>())
		.def("AddNanoseconds",
			TimeT::AddNanoseconds,
			boost::python::return_self<>())

		.def("ToString",
			WRAP_TimeT_ToString,
			OLS_TimeT_ToString())
		.def("ToStringLocal",
			WRAP_TimeT_ToStringLocal,
			OLS_TimeT_ToStringLocal())
		.def("ToStringInterval",
			WRAP_TimeT_ToStringInterval,
			OLS_TimeT_ToStringInterval())

		.def("Now",
			TimeT::Now,
			boost::python::return_value_policy<boost::python::return_by_value>())
		.staticmethod("Now")

		.def("GetMinimumValue",
			TimeT::GetMinimumValue,
			boost::python::return_value_policy<boost::python::return_by_value>())
		.staticmethod("GetMinimumValue")
		.def("GetMaximumValue",
			TimeT::GetMaximumValue,
			boost::python::return_value_policy<boost::python::return_by_value>())
		.staticmethod("GetMaximumValue")
		;
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

