// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the AttrInfoBasic class.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/IncrementalBuffer.hpp>

#include <OdbcWrapper/OdbcWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
AttrInfoBasic::AttrInfoBasic(const SQLCHAR *description_ptr,
	SQLSMALLINT description_length, const SQLCHAR *attributes_list_ptr,
	SQLSMALLINT attributes_list_length)
	:description_(reinterpret_cast<const char *>(description_ptr),
		description_length)
	,attributes_list_()
{
	while (*attributes_list_ptr && attributes_list_length) {
		attributes_list_.push_back(
			reinterpret_cast<const char *>(attributes_list_ptr));
		SQLSMALLINT this_length = static_cast<SQLSMALLINT>(strlen(
			reinterpret_cast<const char *>(attributes_list_ptr))) + 1;
		//	Completely spurious warning under MSVC++ 7.1:
		//		warning C4244: '-=' : conversion from 'int' to 'SQLSMALLINT',
		//		possible loss of data
#if _MSC_VER <= 1310
# pragma warning(push)
# pragma warning(disable:4244)
#endif // #if _MSC_VER <= 1310
		attributes_list_length -= this_length;
#if _MSC_VER <= 1310
# pragma warning(pop)
#endif // #if _MSC_VER <= 1310
		attributes_list_ptr    += this_length;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
AttrInfoBasicVector &AttrInfoBasic::GetEnvAttrList(
	const char *get_attr_row_func_name,
	GetHandleAttrRowFunc get_attr_row_func_ptr, SQLHENV EnvironmentHandle,
	AttrInfoBasicVector &out_list, SQLUSMALLINT Direction)
{
	MLB::Utility::IncrementalBuffer<SQLCHAR, 4096> buff_1;
	MLB::Utility::IncrementalBuffer<SQLCHAR, 4096> buff_2;
	AttrInfoBasicVector                            tmp_list;

	for ( ; ; ) {
		SQLRETURN   return_code;
		SQLSMALLINT actual_len_1;
		SQLSMALLINT actual_len_2;
		for ( ; ; ) {
			return_code = (*get_attr_row_func_ptr)(EnvironmentHandle, Direction,
				buff_1.GetPtr(), buff_1.GetAllocationCountAsType<SQLSMALLINT>(),
				&actual_len_1,
				buff_2.GetPtr(), buff_2.GetAllocationCountAsType<SQLSMALLINT>(),
				&actual_len_2, OdbcThrowFlags::Ok_SWI_or_NoData);
			if ((return_code == SQL_SUCCESS) || (return_code == SQL_NO_DATA))
				break;
			else if (return_code == SQL_SUCCESS_WITH_INFO) {
				bool found_flag = false;
				if (buff_1.SetCount(actual_len_1, false))
					found_flag = true;
				if (buff_2.SetCount(actual_len_2, false))
					found_flag = true;
				if (!found_flag)
					throw OdbcException("Call to '" +
						std::string(get_attr_row_func_name) + "' returned '"
						"SQL_SUCCESS_WITH_INFO', but no returned lengths qualified "
						"for that return code.");
			}
		}
		if (return_code == SQL_NO_DATA)
			break;
		tmp_list.push_back(AttrInfoBasic(buff_1.GetPtr(), actual_len_1,
			buff_2.GetPtr(), actual_len_2));
		Direction = SQL_FETCH_NEXT;
	}

	out_list.swap(tmp_list);

	return(out_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
AttrInfoBasicVector &AttrInfoBasic::GetAttrInfoBasicList(
	SQLHENV EnvironmentHandle, AttrInfoBasicVector &out_list,
	SQLUSMALLINT Direction)
{
	return(GetEnvAttrList("::SQLDrivers()", Drivers, EnvironmentHandle,
		out_list, Direction));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
AttrInfoBasicVector AttrInfoBasic::GetAttrInfoBasicList(
	SQLHENV EnvironmentHandle, SQLUSMALLINT Direction)
{
	AttrInfoBasicVector out_list;

	return(GetAttrInfoBasicList(EnvironmentHandle, out_list, Direction));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
AttrInfoBasicVector &AttrInfoBasic::GetDataSourceBasicList(
	SQLHENV EnvironmentHandle, std::vector<AttrInfoBasic> &out_list,
	SQLUSMALLINT Direction)
{
	return(GetEnvAttrList("::SQLDataSources()", DataSources,
		EnvironmentHandle, out_list, Direction));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
AttrInfoBasicVector AttrInfoBasic::GetDataSourceBasicList(
	SQLHENV EnvironmentHandle, SQLUSMALLINT Direction)
{
	std::vector<AttrInfoBasic> out_list;

	return(GetEnvAttrList("::SQLDataSources()", DataSources,
		EnvironmentHandle, out_list, Direction));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const AttrInfoBasic &datum)
{
	o_str
		<< datum.description_ << " ("
		<< static_cast<unsigned int>(datum.attributes_list_.size())
		<< " attributes)";

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

