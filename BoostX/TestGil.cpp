// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of DetermineFileInfo().

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2015.
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
#include <Utility/UniqueId.hpp>

#include <map>

// ////////////////////////////////////////////////////////////////////////////

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
void PyX_UniqueId()
{
	boost::python::class_<UniqueId>("UniqueId")
  		.def(boost::python::init<>())
  		.def(boost::python::init<bool>())
  		.def(boost::python::init<std::string>())
		.def("__str__",
			WRAP_UniqueId_ToString,
			OLS_UniqueId_ToString())

		.def(boost::python::self < boost::python::other<UniqueId>())
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
void PyX_TimeVal()
{
	boost::python::class_<TimeVal>("TimeVal")
  		.def(boost::python::init<>())
  		.def(boost::python::init<
			time_t,
			boost::python::optional<long> >())
  		.def(boost::python::init<
			std::string>())
		.def("__str__",
			WRAP_TimeVal_ToString,
			OLS_TimeVal_ToString())

//		.def(boost::python::self < boost::python::other<TimeVal>())

		.def("SetToNow",
			TimeVal::SetToNow,
			boost::python::return_internal_reference<>())
		.def("SetToMinimumValue",
			TimeVal::SetToMinimumValue,
			boost::python::return_internal_reference<>())
		.def("SetToMaximumValue",
			TimeVal::SetToMaximumValue,
			boost::python::return_internal_reference<>())

		.def("IsZero",
			TimeVal::IsZero)

		.def("AddSeconds",
			TimeVal::AddSeconds,
			boost::python::return_internal_reference<>())
		.def("AddMilliseconds",
			TimeVal::AddMilliseconds,
			boost::python::return_internal_reference<>())
		.def("AddMicroseconds",
			TimeVal::AddMicroseconds,
			boost::python::return_internal_reference<>())
		.def("AddNanoseconds",
			TimeVal::AddNanoseconds,
			boost::python::return_internal_reference<>())

/*
	CODE NOTE: Pending implementation of PyX_TimeT.
		.def("ToTimeT",
			TimeVal::ToTimeT)
*/

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

namespace MLB {

namespace BoostX {

// ////////////////////////////////////////////////////////////////////////////
template <typename CppContainerType, typename PyContainerType>
	CppToPyContainer(const CppContainerType &src_datum,
	PyContainerType &dst_datum)
{
	CppContainerType::const_iterator iter_b(src_datum.begin());
	CppContainerType::const_iterator iter_e(src_datum.end());

	for ( ; iter_b != iter_e; ++iter_b)
		dst_datum.append(*iter_b);

	return(dst_datum);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	void TEST_CompareContainersBasic(const ContainerType &src_container,
	const ContainerType &dst_container)
{
	if (src_container.size() != dst_container.size())
		MLB::Utility::ThrowException("Sizes differ (source container = " +
			MLB::Utility::AnyToString(src_container.size()) +
			", destination container = " +
			MLB::Utility::AnyToString(dst_container.size()) + ").");

	if (!std::equal(src_container.begin(), src_container.end(),
		dst_container.begin()))
		MLB::Utility::ThrowException("Container contents differ as reported by "
			"'std::equal()'.");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	void TEST_CompareContainers(const ContainerType &src_container,
	const ContainerType &dst_container, const std::string &test_name,
	bool pass_flag)
{
	try {
		try {
			TEST_CompareContainersBasic(src_container, dst_container);
		}
		catch (const std::exception &except) {
			if (pass_flag)
				throw;																	
			std::cout << MLB::Utility::PadRight(test_name, 28) << ": Conversion "
				"test result: SUCCEEDED: Conversion failed, which was expected (" <<
				"difference was [" << except.what() << "]" << std::endl;
			return;
		}
		if (!pass_flag)
			MLB::Utility::ThrowException("Container contents are the same as "
				"reported by 'std::equal()' --- should be different.");
		std::cout << MLB::Utility::PadRight(test_name, 28) << ": Conversion "
			"test result: SUCCEEDED: Containers are identical." << std::endl;
	}
	catch (const std::exception &except) {
		std::cout << MLB::Utility::PadRight(test_name, 28) << ": Conversion "
			"test result: FAILED   : " << except.what() << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	void TEST_AssocToDictSpecific(const ContainerType &src_container,
	const std::string &test_name, bool pass_flag)
{
	ContainerType       dst_container;
	boost::python::dict python_dict;

	ContainerToPyDict(src_container, python_dict);
	PyDictToContainer(python_dict, dst_container);
	TEST_CompareContainers(src_container, dst_container, test_name, pass_flag);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	void TEST_NonMapContainer(const ContainerType &src_container,
	const std::string &test_name, bool pass_flag)
{
	ContainerType       dst_container;
	boost::python::list python_list;

	ContainerToPyList(src_container, python_list);
	PyListToContainer(python_list, dst_container);
	TEST_CompareContainers(src_container, dst_container, test_name, pass_flag);
}
// ////////////////////////////////////////////////////////////////////////////

/*
// ////////////////////////////////////////////////////////////////////////////
void TEST_SetToDict()
{
	std::set<std::string> src_set;

	src_set.insert("First");
	src_set.insert("Second");

	TEST_AssocToDictSpecific(src_set, "std::set<> <---> PyDict", true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_MultiSetToDict()
{
	std::multiset<std::string> src_set;

	src_set.insert("First");
	src_set.insert("Second");

	//	Should succeed...
	TEST_AssocToDictSpecific(src_set, "std::multiset<> <---> PyDict", true);

	//	Should fail because of duplicate key 'First'...
	src_set.insert("First");
	TEST_AssocToDictSpecific(src_set, "std::multiset<> <---> PyDict", false);
}
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
void TEST_MapToDict()
{
	std::map<std::string, int> src_map;

	src_map["First"]  = 1;
	src_map["Second"] = 2;

	TEST_AssocToDictSpecific(src_map, "std::map<> <---> PyDict", true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_MultiMapToDict()
{
	std::multimap<std::string, int> src_map;

	src_map.insert(std::multimap<std::string, int>::value_type("First",  1));
	src_map.insert(std::multimap<std::string, int>::value_type("Second", 2));

	//	Should succeed...
	TEST_AssocToDictSpecific(src_map, "std::multimap<> <---> PyDict", true);

	src_map.insert(std::multimap<std::string, int>::value_type("First",  3));

	//	Should fail because of duplicate key 'First'...
	TEST_AssocToDictSpecific(src_map, "std::multimap<> <---> PyDict", false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_PopulateVector(std::vector<std::string> &the_vector)
{
	the_vector.push_back("Element 0");
	the_vector.push_back("Element 1");
	the_vector.push_back("Element 2");
	the_vector.push_back("Element 3");
	the_vector.push_back("Element 4");
	the_vector.push_back("Element 5");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_PopulateVector(std::vector<MLB::Utility::UniqueId> &the_vector)
{
	the_vector.push_back(MLB::Utility::UniqueId());
	the_vector.push_back(MLB::Utility::UniqueId());
	the_vector.push_back(MLB::Utility::UniqueId());
	the_vector.push_back(MLB::Utility::UniqueId());
	the_vector.push_back(MLB::Utility::UniqueId());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_PopulateVector(std::vector<MLB::Utility::TimeVal> &the_vector)
{
	the_vector.push_back(MLB::Utility::TimeVal::FromString("2001-01-01"));
	the_vector.push_back(MLB::Utility::TimeVal::FromString("2002-01-01"));
	the_vector.push_back(MLB::Utility::TimeVal::FromString("2003-01-01"));
	the_vector.push_back(MLB::Utility::TimeVal::FromString("2004-01-01"));
	the_vector.push_back(MLB::Utility::TimeVal::FromString("2005-01-01"));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ValueType>
	void TEST_RunTests_1()
{
	//	std::vector<>...
	std::vector<ValueType> src_vector;
	TEST_PopulateVector(src_vector);
	TEST_NonMapContainer(src_vector, "std::vector<> <---> PyList", true);

	//	std::deque<>...
	std::deque<ValueType> src_deque(src_vector.begin(), src_vector.end());
	TEST_NonMapContainer(src_deque, "std::deque<> <---> PyList", true);

	//	std::list<>...
	std::list<ValueType> src_list(src_vector.begin(), src_vector.end());
	TEST_NonMapContainer(src_list, "std::list<> <---> PyList", true);

	//	std::set<>...
	std::set<ValueType> src_set(src_vector.begin(), src_vector.end());
	TEST_NonMapContainer(src_set, "std::set<> <---> PyList", true);

	//	std::multiset<>...
	std::multiset<ValueType> src_mset(src_vector.begin(), src_vector.end());
	TEST_NonMapContainer(src_mset, "std::multiset<> <---> PyList", true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct TestGil {
	void                RunContainerTests_1();
//	void                RunContainerTests_2();
	void                RunContainerTests_3();
	boost::python::list RunTestAcquired(unsigned int sleep_ms = 0);
	boost::python::list RunTestReleased(unsigned int sleep_ms = 0);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TestGil::RunContainerTests_1()
{
/*
	//	std::vector<>...
	std::vector<std::string> src_vector;
	src_vector.push_back("Element 0");
	src_vector.push_back("Element 1");
	src_vector.push_back("Element 2");
	src_vector.push_back("Element 3");
	src_vector.push_back("Element 4");
	src_vector.push_back("Element 5");
	TEST_NonMapContainer(src_vector, "std::vector<> <---> PyList", true);

	//	std::deque<>...
	std::deque<std::string> src_deque(src_vector.begin(), src_vector.end());
	TEST_NonMapContainer(src_deque, "std::deque<> <---> PyList", true);

	//	std::list<>...
	std::list<std::string> src_list(src_vector.begin(), src_vector.end());
	TEST_NonMapContainer(src_list, "std::list<> <---> PyList", true);

	//	std::set<>...
	std::set<std::string> src_set(src_vector.begin(), src_vector.end());
	TEST_NonMapContainer(src_set, "std::set<> <---> PyList", true);

	//	std::multiset<>...
	std::multiset<std::string> src_mset(src_vector.begin(), src_vector.end());
	TEST_NonMapContainer(src_mset, "std::multiset<> <---> PyList", true);
*/
/*
	//	std::vector<>...
	std::vector<MLB::Utility::TimeVal> src_vector;
	src_vector.push_back(MLB::Utility::TimeVal::FromString("2001-01-01"));
	src_vector.push_back(MLB::Utility::TimeVal::FromString("2002-01-01"));
	src_vector.push_back(MLB::Utility::TimeVal::FromString("2003-01-01"));
	src_vector.push_back(MLB::Utility::TimeVal::FromString("2004-01-01"));
	src_vector.push_back(MLB::Utility::TimeVal::FromString("2005-01-01"));
	TEST_NonMapContainer(src_vector, "std::vector<> <---> PyList", true);

	//	std::deque<>...
	std::deque<MLB::Utility::TimeVal> src_deque(src_vector.begin(), src_vector.end());
	TEST_NonMapContainer(src_deque, "std::deque<> <---> PyList", true);

	//	std::list<>...
	std::list<MLB::Utility::TimeVal> src_list(src_vector.begin(), src_vector.end());
	TEST_NonMapContainer(src_list, "std::list<> <---> PyList", true);

	//	std::set<>...
	std::set<MLB::Utility::TimeVal> src_set(src_vector.begin(), src_vector.end());
	TEST_NonMapContainer(src_set, "std::set<> <---> PyList", true);

	//	std::multiset<>...
	std::multiset<MLB::Utility::TimeVal> src_mset(src_vector.begin(), src_vector.end());
	TEST_NonMapContainer(src_mset, "std::multiset<> <---> PyList", true);
*/

   TEST_RunTests_1<std::string>();
   TEST_RunTests_1<MLB::Utility::UniqueId>();
   TEST_RunTests_1<MLB::Utility::TimeVal>();
}
// ////////////////////////////////////////////////////////////////////////////

/*
// ////////////////////////////////////////////////////////////////////////////
void TestGil::RunContainerTests_2()
{
	TEST_SetToDict();
	TEST_MultiSetToDict();
}
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
void TestGil::RunContainerTests_3()
{
	TEST_MapToDict();
	TEST_MultiMapToDict();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
boost::python::list TestGil::RunTestAcquired(unsigned int sleep_ms)
{
	std::vector<std::string> out_list;

	{
		PythonEvalSaveRestore gil_context;
		::Sleep(sleep_ms);
		out_list.push_back("Acquired");
		out_list.push_back(MLB::Utility::AnyToString(sleep_ms) + " milliseconds");
	}

	return(MLB::BoostX::ContainerToPyList(out_list));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
boost::python::list TestGil::RunTestReleased(unsigned int sleep_ms)
{
	std::vector<std::string> out_list;

	{
		PythonEvalSaveRestore gil_context;
		::Sleep(sleep_ms);
		out_list.push_back("Released");
		out_list.push_back(MLB::Utility::AnyToString(sleep_ms) + " milliseconds");
	}

	return(MLB::BoostX::ContainerToPyList(out_list));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TestGilPython()
{
	boost::python::class_<TestGil>("TestGil")
		.def("RunContainerTests_1",
			TestGil::RunContainerTests_1)
/*
		.def("RunContainerTests_2",
			TestGil::RunContainerTests_2)
*/
		.def("RunTestAcquired",
			TestGil::RunTestAcquired)
		.def("RunTestReleased",
			TestGil::RunTestReleased)
		;

	MLB::Utility::PyX_UniqueId();
	MLB::Utility::PyX_TimeVal();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(TestGilPython)
{
	TestGilPython();
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace BoostX

} // namespace MLB

/*
# #############################################################################
# #############################################################################
# Test Code:
# ---- -----
# #############################################################################
import TestGilPython

guid_1=TestGilPython.UniqueId(False)

print "Null Guid:", guid_1
print ">>> IsZero()             =", guid_1.IsZero()
print ">>> ToStringByteOrder() =", guid_1.ToStringByteOrder()
print ">>> ToStringRadix64()   =", guid_1.ToStringRadix64()

guid_2=TestGilPython.UniqueId()

print "Null Guid:", guid_2
print ">>> IsZero()             =", guid_2.IsZero()
print ">>> ToStringByteOrder() =", guid_2.ToStringByteOrder()
print ">>> ToStringRadix64()   =", guid_2.ToStringRadix64()

guid_3=TestGilPython.UniqueId("12345678-1234-1234-1234-1234567890AB")

print "Null Guid:", guid_2
print ">>> IsZero()             =", guid_3.IsZero()
print ">>> ToStringByteOrder() =", guid_3.ToStringByteOrder()
print ">>> ToStringRadix64()   =", guid_3.ToStringRadix64()
# #############################################################################
*/

/*
# #############################################################################
# Test Code:
# ---- -----
import time
import threading
import TestGilPython

class SleepyThread(threading.Thread):
    def __init__(self, sleep_secs):
        threading.Thread.__init__(self)
        self.sleep_time = sleep_secs
        print "Python thread will sleep for", self.sleep_time, "seconds."
    def run(self):
        print "*** Python thread started", self.sleep_time, "second sleep."
        time.sleep(self.sleep_time)
        print "*** Python thread completed", self.sleep_time, "second sleep."

xxx=TestGilPython.TestGil()

python_thread = SleepyThread(5)
python_thread.start()

print xxx.RunTestAcquired(10000)
print xxx.RunTestReleased(10000)
*/

/*
# #############################################################################
# Test Code:
# ---- -----
import time
import threading
import TestGilPython

guid_1=TestGilPython.UniqueId(False)
guid_2=TestGilPython.UniqueId()
#guid_3=TestGilPython.UniqueId()

print guid_1
print guid_2

print guid_1 < guid_2
print guid_2 < guid_1


time_1=TestGilPython.TimeVal(98766, 123456)
time_2=TestGilPython.TimeVal()
print time_1 < time_2
print time_2 < time_1


class SleepyThread(threading.Thread):
    def __init__(self, sleep_secs):
        threading.Thread.__init__(self)
        self.sleep_time = sleep_secs
        print "Python thread will sleep for", self.sleep_time, "seconds."
    def run(self):
        print "*** Python thread started", self.sleep_time, "second sleep."
        time.sleep(self.sleep_time)
        print "*** Python thread completed", self.sleep_time, "second sleep."

xxx=TestGilPython.TestGil()

python_thread = SleepyThread(5)
python_thread.start()

xxx.RunContainerTests_1()
xxx.RunContainerTests_2()
xxx.RunContainerTests_3()

print xxx.RunTestAcquired(10000)
print xxx.RunTestReleased(10000)
*/

