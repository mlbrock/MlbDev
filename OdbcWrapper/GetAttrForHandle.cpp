// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the 'GetAttrForHandle()' functions
								which provide generic support for the retrieval of
								ODBC handle attribute data.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/EnumFlagOps.hpp>
#include <Utility/IncrementalBuffer.hpp>

#include <OdbcWrapper/OdbcWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	SQLPOINTER ValuePtr, SQLINTEGER BufferLength, SQLINTEGER *StringLengthPtr,
	HandleGetAttrFunc get_attr_function, OdbcThrowFlags::Flags throw_flags)
{
	return((*get_attr_function)(Handle, Attribute, ValuePtr, BufferLength,
		StringLengthPtr, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	SQLINTEGER &ValuePtr, HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags)
{
	return((*get_attr_function)(Handle, Attribute,
		reinterpret_cast<SQLPOINTER>(&ValuePtr), 0, NULL, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	SQLUINTEGER &ValuePtr, HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags)
{
	return((*get_attr_function)(Handle, Attribute,
		reinterpret_cast<SQLPOINTER>(&ValuePtr), 0, NULL, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	SQLSMALLINT &ValuePtr, HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags)
{
	return((*get_attr_function)(Handle, Attribute,
		reinterpret_cast<SQLPOINTER>(&ValuePtr), 0, NULL, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	SQLUSMALLINT &ValuePtr, HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags)
{
	return((*get_attr_function)(Handle, Attribute,
		reinterpret_cast<SQLPOINTER>(&ValuePtr), 0, NULL, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	OdbcAttrFlags32 &ValuePtr, HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags)
{
	return((*get_attr_function)(Handle, Attribute,
		reinterpret_cast<SQLPOINTER>(&ValuePtr), 0, NULL, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	OdbcAttrWindowHandle &ValuePtr, HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags)
{
	return((*get_attr_function)(Handle, Attribute,
		reinterpret_cast<SQLPOINTER>(&ValuePtr), 0, NULL, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

/*
	CODE NOTE: Attribute integral type pointer forms obsoleted. To be removed.
// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	SQLINTEGER *ValuePtr, HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags)
{
	return((*get_attr_function)(Handle, Attribute,
		reinterpret_cast<SQLPOINTER>(ValuePtr), 0, NULL, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	SQLUINTEGER *ValuePtr, HandleGetAttrFunc get_attr_function,
	OdbcThrowFlags::Flags throw_flags)
{
	return((*get_attr_function)(Handle, Attribute,
		reinterpret_cast<SQLPOINTER>(ValuePtr), 0, NULL, throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	char *ValuePtr, SQLINTEGER BufferLength, SQLINTEGER *StringLengthPtr,
	HandleGetAttrFunc get_attr_function, OdbcThrowFlags::Flags throw_flags)
{
	return((*get_attr_function)(Handle, Attribute,
		reinterpret_cast<SQLPOINTER>(ValuePtr), BufferLength, StringLengthPtr,
		throw_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SQLRETURN GetAttrForHandle(SQLHANDLE Handle, SQLINTEGER Attribute,
	std::string &ValuePtr, const char *odbc_function_name,
	HandleGetAttrFunc get_attr_function, OdbcThrowFlags::Flags throw_flags)
{
	CheckStringArgInLengthDomain<SQLINTEGER>(ValuePtr, "ValuePtr",
		odbc_function_name);

	SQLRETURN                                      return_code;
	MLB::Utility::IncrementalBuffer<SQLCHAR, 4096> buffer;

	throw_flags = MLB::Utility::EnumFlagAnd(throw_flags,
		static_cast<OdbcThrowFlags::Flags>(~(OdbcThrowFlags::SuccessWithInfo)));

	for ( ; ; ) {
		SQLINTEGER actual_len;
		return_code = get_attr_function(Handle, Attribute,
			reinterpret_cast<SQLPOINTER>(buffer.GetPtr()),
			buffer.GetAllocationCountAsType<SQLINTEGER>(), &actual_len,
			throw_flags);
		if (return_code == SQL_SUCCESS) {
			std::string(reinterpret_cast<const char *>(buffer.GetPtr()),
				actual_len).swap(ValuePtr);
			break;
		}
		else if (return_code == SQL_SUCCESS_WITH_INFO) {
			if (!buffer.SetCount(actual_len, false))
				throw OdbcException("Call to '" + std::string(odbc_function_name) +
					"' returned 'SQL_SUCCESS_WITH_INFO', but no returned lengths "
					"qualified for that return code.");
		}
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

