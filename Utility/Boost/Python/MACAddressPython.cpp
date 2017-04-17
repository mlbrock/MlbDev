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
								MACAddress class.

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
#include <Utility/MACAddress.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {

// ////////////////////////////////////////////////////////////////////////////
std::string WRAP_MACAddress_ToString(MACAddress &datum, int separator = ':')
{
	return(datum.ToString(separator));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_FUNCTION_OVERLOADS(OLS_MACAddress_ToString,         WRAP_MACAddress_ToString,         1, 2)
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
void MLB_Utility_MACAddressPython()
{
	boost::python::class_<MACAddress>("MACAddress")
  		.def(boost::python::init<>())
  		.def(boost::python::init<MACAddress>())
		.def("__str__",
			WRAP_MACAddress_ToString,
			OLS_MACAddress_ToString())

		.def(boost::python::self == boost::python::other<MACAddress>())
		.def(boost::python::self != boost::python::other<MACAddress>())
		.def(boost::python::self <  boost::python::other<MACAddress>())
		.def(boost::python::self >  boost::python::other<MACAddress>())
		.def(boost::python::self <= boost::python::other<MACAddress>())
		.def(boost::python::self >= boost::python::other<MACAddress>())

		.def("SetToMinimumValue",
			MACAddress::SetToMinimumValue,
			boost::python::return_self<>())
		.def("SetToMaximumValue",
			MACAddress::SetToMaximumValue,
			boost::python::return_self<>())

		.def("IsZero",
			MACAddress::IsZero)

		.def("Get",
			MACAddress::Get)
		.def("Clear",
			MACAddress::Clear)

		.def("ToString",
			WRAP_MACAddress_ToString,
			OLS_MACAddress_ToString())

		.def("GetMinimumValue",
			MACAddress::GetMinimumValue,
			boost::python::return_value_policy<boost::python::return_by_value>())
		.staticmethod("GetMinimumValue")
		.def("GetMaximumValue",
			MACAddress::GetMaximumValue,
			boost::python::return_value_policy<boost::python::return_by_value>())
		.staticmethod("GetMaximumValue")
		;
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

