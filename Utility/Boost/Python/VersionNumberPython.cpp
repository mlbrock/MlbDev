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
								VersionNumber class.

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
#include <Utility/VersionNumber.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {

// ////////////////////////////////////////////////////////////////////////////
std::string WRAP_VersionNumber_ToString(VersionNumber &datum)
{
	return(datum.ToString());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_FUNCTION_OVERLOADS(OLS_VersionNumber_ToString,         WRAP_VersionNumber_ToString,         1, 1)
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
void MLB_Utility_VersionNumberPython()
{
	boost::python::class_<VersionNumber>("VersionNumber")
  		.def(boost::python::init<>())
  		.def(boost::python::init<
			unsigned int,
			boost::python::optional<
				unsigned int,
				unsigned int,
				unsigned int> >())
  		.def(boost::python::init<std::string>())
  		.def(boost::python::init<VersionNumber>())
		.def("__str__",
			WRAP_VersionNumber_ToString,
			OLS_VersionNumber_ToString())

		.def(boost::python::self == boost::python::other<VersionNumber>())
		.def(boost::python::self != boost::python::other<VersionNumber>())
		.def(boost::python::self <  boost::python::other<VersionNumber>())
		.def(boost::python::self >  boost::python::other<VersionNumber>())
		.def(boost::python::self <= boost::python::other<VersionNumber>())
		.def(boost::python::self >= boost::python::other<VersionNumber>())


		.def("GetMajor",
			VersionNumber::GetMajor)
		.def("GetMinor",
			VersionNumber::GetMinor)
		.def("GetRelease",
			VersionNumber::GetRelease)
		.def("GetBuild",
			VersionNumber::GetBuild)

		.def("SetMajor",
			VersionNumber::SetMajor)
		.def("SetMinor",
			VersionNumber::SetMinor)
		.def("SetRelease",
			VersionNumber::SetRelease)
		.def("SetBuild",
			VersionNumber::SetBuild)

		.def("SetToMinimumValue",
			VersionNumber::SetToMinimumValue,
			boost::python::return_self<>())
		.def("SetToMaximumValue",
			VersionNumber::SetToMaximumValue,
			boost::python::return_self<>())

		.def("ToString",
			WRAP_VersionNumber_ToString,
			OLS_VersionNumber_ToString())

		.def("GetMinimumValue",
			VersionNumber::GetMinimumValue,
			boost::python::return_value_policy<boost::python::return_by_value>())
		.staticmethod("GetMinimumValue")
		.def("GetMaximumValue",
			VersionNumber::GetMaximumValue,
			boost::python::return_value_policy<boost::python::return_by_value>())
		.staticmethod("GetMaximumValue")

		.def("GetElementName",
			VersionNumber::GetElementName)
		.staticmethod("GetElementName")
		;
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

