// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Boost Python Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Main implementation file for the MLB Utility Python
								interface.

	Revision History	:	2007-10-19 --- Creation.
									Michael L. Brock

	Notes					:	Sample code for use if this interface may be found in
								MLB/Utility/Boost/Python/Test_MLBUtilityPython.py

		Copyright Michael L. Brock 2007 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <BoostX/BoostPython.hpp>

// ////////////////////////////////////////////////////////////////////////////

/*
namespace MLB {
namespace BoostX {

// ////////////////////////////////////////////////////////////////////////////

template <typename except_type>
	void Python_ExceptionTranslator(const except_type &except)
{
	PyErr_SetString(PyExc_UserWarning,
		std::string("Unspecified C++ exception thrown: " +
			std::string(except.what())).c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Use this macro outside of any C++ namespace to provide default C++-to-Python
//	exception translation behavior. Or provide your own specialization...
#define MLB_BOOSTX_PYTHON_DEFINE_EXCEPTION_TRANSLATOR(except_type)				\
	template <>																						\
		void MLB::BoostX::Python_ExceptionTranslator(const except_type &except)	\
	{																									\
			::PyErr_SetString(::PyExc_UserWarning,											\
				std::string(std::string(#except_type) + ": " +							\
					std::string(except.what())).c_str());									\
	}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Use this macro inside of the BOOST_PYTHON_MODULE() scope...
#define MLB_BOOSTX_PYTHON_REGISTER_EXCEPTION_TRANSLATOR(except_type)				\
	boost::python::register_exception_translator<										\
		except_type>(MLB::BoostX::Python_ExceptionTranslator<except_type>)
// ////////////////////////////////////////////////////////////////////////////

} // namespace BoostX
} //	namespace MLB
*/

// ////////////////////////////////////////////////////////////////////////////
MLB_BOOSTX_PYTHON_DEFINE_EXCEPTION_TRANSLATOR(MLB::Utility::ExceptionGeneral)
MLB_BOOSTX_PYTHON_DEFINE_EXCEPTION_TRANSLATOR(MLB::Utility::ExceptionErrno)
MLB_BOOSTX_PYTHON_DEFINE_EXCEPTION_TRANSLATOR(MLB::Utility::ExceptionSystemError)
MLB_BOOSTX_PYTHON_DEFINE_EXCEPTION_TRANSLATOR(MLB::Utility::ExceptionCriticalEvent)
MLB_BOOSTX_PYTHON_DEFINE_EXCEPTION_TRANSLATOR(MLB::Utility::ExceptionMMapVMFailure)
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {

// ////////////////////////////////////////////////////////////////////////////
void TEST_MLB_Utility_Python_ThrowExceptionGeneral()
{
	throw MLB::Utility::ExceptionGeneral("MLB_Utility_Python_Test");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_MLB_Utility_Python_ThrowExceptionErrno()
{
	throw MLB::Utility::ExceptionErrno("MLB_Utility_Python_Test");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_MLB_Utility_Python_ThrowExceptionSystemError()
{
	throw MLB::Utility::ExceptionSystemError("MLB_Utility_Python_Test");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_MLB_Utility_Python_ThrowExceptionCriticalEvent()
{
	throw MLB::Utility::ExceptionCriticalEvent("MLB_Utility_Python_Test");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_MLB_Utility_Python_ThrowExceptionMMapVMFailure()
{
	throw MLB::Utility::ExceptionMMapVMFailure("MLB_Utility_Python_Test");
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
void MLB_Utility_MACAddressPython();
void MLB_Utility_TimeSpecPython();
void MLB_Utility_TimeTPython();
void MLB_Utility_TimeValPython();
void MLB_Utility_UniqueIdPython();
void MLB_Utility_VersionNumberPython();
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(MLBUtilityPython)
{
	MLB_BOOSTX_PYTHON_REGISTER_EXCEPTION_TRANSLATOR(MLB::Utility::ExceptionGeneral);
	MLB_BOOSTX_PYTHON_REGISTER_EXCEPTION_TRANSLATOR(MLB::Utility::ExceptionErrno);
	MLB_BOOSTX_PYTHON_REGISTER_EXCEPTION_TRANSLATOR(MLB::Utility::ExceptionSystemError);
	MLB_BOOSTX_PYTHON_REGISTER_EXCEPTION_TRANSLATOR(MLB::Utility::ExceptionCriticalEvent);
	MLB_BOOSTX_PYTHON_REGISTER_EXCEPTION_TRANSLATOR(MLB::Utility::ExceptionMMapVMFailure);

	MLB_Utility_MACAddressPython();
	MLB_Utility_TimeSpecPython();
	MLB_Utility_TimeTPython();
	MLB_Utility_TimeValPython();
	MLB_Utility_UniqueIdPython();
	MLB_Utility_VersionNumberPython();

	boost::python::def("TEST_MLB_Utility_Python_ThrowExceptionGeneral",
		TEST_MLB_Utility_Python_ThrowExceptionGeneral);

	boost::python::def("TEST_MLB_Utility_Python_ThrowExceptionErrno",
		TEST_MLB_Utility_Python_ThrowExceptionErrno);

	boost::python::def("TEST_MLB_Utility_Python_ThrowExceptionSystemError",
		TEST_MLB_Utility_Python_ThrowExceptionSystemError);

	boost::python::def("TEST_MLB_Utility_Python_ThrowExceptionCriticalEvent",
		TEST_MLB_Utility_Python_ThrowExceptionCriticalEvent);

	boost::python::def("TEST_MLB_Utility_Python_ThrowExceptionMMapVMFailure",
		TEST_MLB_Utility_Python_ThrowExceptionMMapVMFailure);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

