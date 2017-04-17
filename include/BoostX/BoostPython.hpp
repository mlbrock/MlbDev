// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB BoostX Python Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the Boost Python support library.

	Revision History	:	2005-08-23 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH_MLB_BoostX_BoostPython_hpp_HH

#define HH_MLB_BoostX_BoostPython_hpp_HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <BoostX.hpp>

#ifdef _Windows
# pragma warning(disable: 4100 4121 4217 4244 4267 4511 4512 4625 4626 4640 4668 4686)
#endif // #ifdef _Windows

#include <boost/python.hpp>

#ifdef _Windows
# pragma warning(default: 4100 4121 4217 4244 4267 4511 4512 4625 4626 4640 4668 4686)
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace BoostX {

// ////////////////////////////////////////////////////////////////////////////
class PythonEvalSaveRestore {
public:
	PythonEvalSaveRestore()
#ifdef WITH_THREAD
		:saved_context_sptr_(::PyEval_SaveThread(), ::PyEval_RestoreThread)
#else
		:saved_context_sptr_()
#endif // #ifdef WITH_THREAD
	{
	}
	virtual ~PythonEvalSaveRestore()
	{
		RestoreThread();
	}

	//	Use this to re-acquire the GIL before the end of block scope...
	void RestoreThread()
	{
		saved_context_sptr_.reset();
	}

protected:
	boost::shared_ptr<PyThreadState> saved_context_sptr_;
};
// ////////////////////////////////////////////////////////////////////////////

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

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainedType>
	struct ContainedTypeConverter {
	typedef ContainedType type;
};
template <>
	struct ContainedTypeConverter<std::string> {
	typedef const char * type;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	ContainerType &PyListToContainer(const boost::python::list &src_list,
		ContainerType &dst_list)
{
	int tmp_count = boost::python::extract<int>(src_list.attr("__len__")());

	if (tmp_count > 0) {
		int count_1;
		for (count_1 = 0; count_1 < tmp_count; ++count_1)
			dst_list.insert(dst_list.end(),
				boost::python::extract<ContainerType::value_type>(src_list[count_1]));
	}

	return(dst_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	ContainerType PyListToContainer(const boost::python::list &src_list)
{
	ContainerType dst_list;

	return(PyListToContainer(src_list, dst_list));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename IteratorType>
	boost::python::list &IteratorsToPyList(IteratorType src_begin,
		IteratorType src_end, boost::python::list &dst_list)
{
	for ( ; src_begin != src_end; ++src_begin)
		dst_list.append(*src_begin);

	return(dst_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename IteratorType>
	boost::python::list IteratorsToPyList(IteratorType src_begin,
		IteratorType src_end)
{
	boost::python::list dst_list;

	return(IteratorsToPyList(src_begin, src_end, dst_list));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	boost::python::list &ContainerToPyList(const ContainerType &src_list,
		boost::python::list &dst_list)
{
	return(IteratorsToPyList(src_list.begin(), src_list.end(), dst_list));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	boost::python::list ContainerToPyList(const ContainerType &src_list)
{
	return(IteratorsToPyList(src_list.begin(), src_list.end()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	ContainerType &PyDictToContainer(const boost::python::dict &src_dict,
		ContainerType &dst_dict)
{
	int tmp_count = boost::python::extract<int>(src_dict.attr("__len__")());

	if (tmp_count > 0) {
		ContainerType             tmp_dst_dict;
		const boost::python::list src_keys(src_dict.keys());
		const boost::python::list src_vals(src_dict.values());
		int count_1;
		for (count_1 = 0; count_1 < tmp_count; ++count_1) {
			ContainedTypeConverter<ContainerType::key_type>::type    this_key =
				boost::python::extract<ContainedTypeConverter<
				ContainerType::key_type>::type>(src_keys[count_1]);
			ContainedTypeConverter<ContainerType::mapped_type>::type this_val =
				boost::python::extract<ContainedTypeConverter<
				ContainerType::mapped_type>::type>(src_vals[count_1]);
			tmp_dst_dict.insert(ContainerType::value_type(this_key, this_val));
		}
		dst_dict.swap(tmp_dst_dict);
	}

	return(dst_dict);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	ContainerType PyDictToContainer(const boost::python::dict &src_dict)
{
	ContainerType dst_dict;

	return(PyDictToContainer(src_dict, dst_dict));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	boost::python::dict &ContainerToPyDict(const ContainerType &src_dict,
		boost::python::dict &dst_dict)
{
	if (!src_dict.empty()) {
		ContainerType::const_iterator iter_b(src_dict.begin());
		ContainerType::const_iterator iter_e(src_dict.end());
		for ( ; iter_b != iter_e; ++iter_b)
			dst_dict[iter_b->first] = iter_b->second;
	}

	return(dst_dict);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	boost::python::dict ContainerToPyDict(const ContainerType &src_dict)
{
	boost::python::dict dst_dict;

	return(ContainerToPyDict(src_dict, dst_dict));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace BoostX

} // namespace MLB

#endif // #ifndef HH_MLB_BoostX_BoostPython_hpp_HH

