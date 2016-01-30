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
								UniqueId class (and a few of the related free functions
								in the Utility library).

	Revision History	:	2007-10-19 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2007 - 2016.
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
#include <Utility/UniqueId.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {

// ////////////////////////////////////////////////////////////////////////////
std::string WRAP_UniqueId_ToString(UniqueId &datum)
{
	return(datum.ToString());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string WRAP_UniqueId_ToStringByteOrder(UniqueId &datum)
{
	return(datum.ToStringByteOrder());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string WRAP_UniqueId_ToStringRadix64(UniqueId &datum)
{
	return(datum.ToStringRadix64());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_FUNCTION_OVERLOADS(OLS_UniqueId_ToString,          WRAP_UniqueId_ToString,          1, 1)
BOOST_PYTHON_FUNCTION_OVERLOADS(OLS_UniqueId_ToStringByteOrder, WRAP_UniqueId_ToStringByteOrder, 1, 1)
BOOST_PYTHON_FUNCTION_OVERLOADS(OLS_UniqueId_ToStringRadix64,   WRAP_UniqueId_ToStringRadix64,   1, 1)
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
void MLB_Utility_UniqueIdPython()
{
	boost::python::class_<UniqueId>("UniqueId")
  		.def(boost::python::init<>())
  		.def(boost::python::init<bool>())
  		.def(boost::python::init<std::string>())
		.def("__str__",
			WRAP_UniqueId_ToString,
			OLS_UniqueId_ToString())

		.def(boost::python::self <  boost::python::other<UniqueId>())
		.def(boost::python::self == boost::python::other<UniqueId>())
		.def(boost::python::self != boost::python::other<UniqueId>())

		.def("IsZero",
			UniqueId::IsZero)

		.def("ToString",
			WRAP_UniqueId_ToString,
			OLS_UniqueId_ToString())
		.def("ToStringByteOrder",
			WRAP_UniqueId_ToStringByteOrder,
			OLS_UniqueId_ToStringByteOrder())
		.def("ToStringRadix64",
			WRAP_UniqueId_ToStringRadix64,
			OLS_UniqueId_ToStringRadix64())
		;
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

