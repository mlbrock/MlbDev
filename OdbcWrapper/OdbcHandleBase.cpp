// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the OdbcHandleBase class.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OdbcWrapper/OdbcHandle.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Force instantiation for our handle types...
// ////////////////////////////////////////////////////////////////////////////
template class OdbcHandleBase<SQLHSTMT>;
template class OdbcHandleBase<SQLHDBC>;
template class OdbcHandleBase<SQLHENV>;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	OdbcHandleBase<HandleType>::OdbcHandleBase(SQLSMALLINT handle_type,
		SQLHANDLE parent_handle, const char *type_base_name,
		HandleGetAttrFunc get_attr_function, HandleSetAttrFunc set_attr_function)
	:handle_type_(handle_type)
	,parent_handle_(parent_handle)
	,this_handle_(0)
	,get_attr_function_name_("::SQLGet" + std::string(type_base_name) + "Attr")
	,set_attr_function_name_("::SQLSet" + std::string(type_base_name) + "Attr")
	,get_attr_function_(get_attr_function)
	,set_attr_function_(set_attr_function)
{
	//	CODE NOTE: Check handle_type
	//	CODE NOTE: Check parent handle
	AllocHandle(handle_type_, parent_handle_, &this_handle_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	OdbcHandleBase<HandleType>::~OdbcHandleBase()
{
	FreeHandle(handle_type_, this_handle_, OdbcThrowFlags::None);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	HandleType OdbcHandleBase<HandleType>::GetNativeHandle() const
{
	return(this_handle_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::GetAttr(SQLINTEGER Attribute,
		SQLPOINTER ValuePtr, SQLINTEGER BufferLength, SQLINTEGER *StringLengthPtr,
		OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(GetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		BufferLength, StringLengthPtr, get_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::GetAttr(SQLINTEGER Attribute,
	SQLINTEGER &ValuePtr, OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(GetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		get_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::GetAttr(SQLINTEGER Attribute,
	SQLUINTEGER &ValuePtr, OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(GetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		get_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::GetAttr(SQLINTEGER Attribute,
	SQLSMALLINT &ValuePtr, OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(GetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		get_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::GetAttr(SQLINTEGER Attribute,
	SQLUSMALLINT &ValuePtr, OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(GetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		get_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::GetAttr(SQLINTEGER Attribute,
	OdbcAttrFlags32 &ValuePtr, OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(GetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		get_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::GetAttr(SQLINTEGER Attribute,
	OdbcAttrWindowHandle &ValuePtr, OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(GetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		get_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

/*
	CODE NOTE: Attribute integral type pointer forms obsoleted. To be removed.
// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::GetAttr(SQLINTEGER Attribute,
	SQLINTEGER *ValuePtr, OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(GetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		get_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::GetAttr(SQLINTEGER Attribute,
	SQLUINTEGER *ValuePtr, OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(GetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		get_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::GetAttr(SQLINTEGER Attribute,
		char *ValuePtr, SQLINTEGER BufferLength, SQLINTEGER *StringLengthPtr,
		OdbcThrowFlags::Flags throw_flags = OdbcThrowFlags::Default) const
{
	CheckAttributeFuncs();

	return(GetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		BufferLength, StringLengthPtr, get_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::GetAttr(SQLINTEGER Attribute,
		std::string &ValuePtr, OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(GetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		get_attr_function_name_.c_str(), get_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::SetAttr(SQLINTEGER Attribute,
		SQLPOINTER ValuePtr, SQLINTEGER StringLength,
		OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(SetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		StringLength, set_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::SetAttr(SQLINTEGER Attribute,
		SQLINTEGER ValuePtr, OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(SetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		set_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::SetAttr(SQLINTEGER Attribute,
		SQLUINTEGER ValuePtr, OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(SetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		set_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::SetAttr(SQLINTEGER Attribute,
		SQLSMALLINT ValuePtr, OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(SetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		set_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::SetAttr(SQLINTEGER Attribute,
		SQLUSMALLINT ValuePtr, OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(SetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		set_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::SetAttr(SQLINTEGER Attribute,
		OdbcAttrFlags32 ValuePtr, OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(SetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		set_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::SetAttr(SQLINTEGER Attribute,
		OdbcAttrWindowHandle ValuePtr, OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(SetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		set_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::SetAttr(SQLINTEGER Attribute,
		const char *ValuePtr, SQLINTEGER StringLength,
		OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(SetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		StringLength, set_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	SQLRETURN OdbcHandleBase<HandleType>::SetAttr(SQLINTEGER Attribute,
		const std::string &ValuePtr, OdbcThrowFlags::Flags throw_flags) const
{
	CheckAttributeFuncs();

	return(SetAttrForHandle(GetNativeHandle(), Attribute, ValuePtr,
		set_attr_function_name_.c_str(), set_attr_function_, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename HandleType>
	void OdbcHandleBase<HandleType>::CheckAttributeFuncs() const
{
	if ((get_attr_function_ == NULL) || (set_attr_function_ == NULL))
		MLB::Utility::ThrowException("Attribute accessor functions are not "
		"supported for ODBC handle type " +
		MLB::Utility::AnyToString(handle_type_) + ".");
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

