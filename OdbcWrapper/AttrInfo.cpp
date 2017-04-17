// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the AttrInfo class.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OdbcWrapper/OdbcWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
AttrInfo::AttrInfo()
	:description_()
	,attributes_set_()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
AttrInfo::AttrInfo(const AttrInfoBasic &driver_info_basic)
	:description_(driver_info_basic.description_)
	,attributes_set_()
{
	std::vector<std::string>::const_iterator iter_b(
		driver_info_basic.attributes_list_.begin());
	std::vector<std::string>::const_iterator iter_e(
		driver_info_basic.attributes_list_.end());

	for ( ; iter_b != iter_e; ++iter_b)
		attributes_set_.insert(AttrDatum(iter_b->c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
AttrInfoVector &AttrInfo::GetAttrInfoList(SQLHENV EnvironmentHandle,
	AttrInfoVector &out_list, SQLUSMALLINT Direction)
{
	AttrInfoBasicVector tmp_basic_list;

	AttrInfoBasic::GetAttrInfoBasicList(EnvironmentHandle, tmp_basic_list,
		Direction);

	AttrInfoVector tmp_list;

	tmp_list.reserve(tmp_basic_list.size());

	AttrInfoBasicVectorIterC iter_b(tmp_basic_list.begin());
	AttrInfoBasicVectorIterC iter_e(tmp_basic_list.end());

	for ( ; iter_b != iter_e; ++iter_b)
		tmp_list.push_back(AttrInfo(*iter_b));

	out_list.swap(tmp_list);

	return(out_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
AttrInfoVector AttrInfo::GetAttrInfoList(SQLHENV EnvironmentHandle,
	SQLUSMALLINT Direction)
{
	AttrInfoVector out_list;

	return(GetAttrInfoList(EnvironmentHandle, out_list, Direction));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
AttrInfoVector &AttrInfo::GetDataSourcesList(SQLHENV EnvironmentHandle,
	std::vector<AttrInfo> &out_list, SQLUSMALLINT Direction)
{
	AttrInfoBasicVector tmp_basic_list;

	AttrInfoBasic::GetEnvAttrList("::SQLDataSources()", DataSources,
		EnvironmentHandle, tmp_basic_list, Direction);

	AttrInfoVector tmp_list;

	tmp_list.reserve(tmp_basic_list.size());

	AttrInfoBasicVectorIterC iter_b(tmp_basic_list.begin());
	AttrInfoBasicVectorIterC iter_e(tmp_basic_list.end());

	for ( ; iter_b != iter_e; ++iter_b)
		tmp_list.push_back(AttrInfo(*iter_b));

	out_list.swap(tmp_list);

	return(out_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
AttrInfoVector AttrInfo::GetDataSourcesList(SQLHENV EnvironmentHandle,
	SQLUSMALLINT Direction)
{
	AttrInfoVector out_list;

	return(GetDataSourcesList(EnvironmentHandle, out_list, Direction));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const AttrInfo &datum)
{
	o_str
		<< datum.description_ << " ("
		<< static_cast<unsigned int>(datum.attributes_set_.size())
		<< " attributes)";

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

