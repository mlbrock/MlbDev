// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	ODBC Wrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the OdbcHandleDbc class.

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

#include <OdbcWrapper/OdbcHandle.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OdbcWrapper {

// ////////////////////////////////////////////////////////////////////////////

template <SQLINTEGER AttrSelector>
	struct AttrStmtTypeInfo {
	enum { is_string = 0 };
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_APP_PARAM_DESC> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_APP_ROW_DESC> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ASYNC_ENABLE> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_CONCURRENCY> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_CURSOR_SCROLLABLE> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_CURSOR_SENSITIVITY> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_CURSOR_TYPE> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ENABLE_AUTO_IPD> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_FETCH_BOOKMARK_PTR> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_IMP_PARAM_DESC> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_IMP_ROW_DESC> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_KEYSET_SIZE> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_MAX_LENGTH> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_MAX_ROWS> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_METADATA_ID> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_NOSCAN> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAM_BIND_OFFSET_PTR> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAM_BIND_TYPE> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAM_OPERATION_PTR> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAM_STATUS_PTR> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAMS_PROCESSED_PTR> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_PARAMSET_SIZE> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_QUERY_TIMEOUT> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_RETRIEVE_DATA> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_ARRAY_SIZE> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_BIND_OFFSET_PTR> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_BIND_TYPE> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_NUMBER> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_OPERATION_PTR> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROW_STATUS_PTR> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_ROWS_FETCHED_PTR> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_SIMULATE_CURSOR> {
	typedef SQLUINTEGER type;
};
template <> struct AttrStmtTypeInfo<SQL_ATTR_USE_BOOKMARKS> {
	typedef SQLUINTEGER type;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <SQLINTEGER AttrSelector>
	struct SavedStmtAttr : public AttrStmtTypeInfo<AttrSelector> {
	typedef typename AttrStmtTypeInfo<AttrSelector>::type AttrType;
	SavedStmtAttr(SQLHSTMT stmt_handle)
		:attr_selector_(AttrSelector)
	{
		GetStmtAttr(stmt_handle, attr_selector_, saved_value_);
std::cout << saved_value_ << std::endl;
	}
	SavedStmtAttr(OdbcHandleStmt &handle_stmt)
		:attr_selector_(AttrSelector)
	{
		handle_stmt.GetAttr(attr_selector_, saved_value_);
std::cout << saved_value_ << std::endl;
	}

	SQLINTEGER  attr_selector_;
	AttrType    saved_value_;
//	AttrStmtTypeInfo<AttrSelector>::type        saved_value_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename SqlType>
	struct OdbcTypeInfo {
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct OdbcBinderCol {
	OdbcBinderCol()
		:col_number_(0)
		,col_offset_(0)
		,col_length_(0)
		,col_length_used_(0)
		,col_indicator_offset_(0)
	{
	}

	const void *GetPointer(const void *row_pointer) const
	{
		return(static_cast<const char *>(row_pointer) + col_offset_);
	}

	SQLSMALLINT col_number_;
	std::size_t col_offset_;
	std::size_t col_length_;
	std::size_t col_length_used_;
	std::size_t col_indicator_offset_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<OdbcBinderCol>        OdbcBinderColList;
typedef OdbcBinderColList::iterator       OdbcBinderColListIter;
typedef OdbcBinderColList::const_iterator OdbcBinderColListIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct OdbcBinderRow {
	OdbcBinderRow(OdbcHandleStmtSPtr &odbc_stmt_sptr,
		const std::vector<SQLSMALLINT> &col_number_list =
		std::vector<SQLSMALLINT>());
	OdbcBinderRow(unsigned int row_alloc_count,
		OdbcHandleStmtSPtr &odbc_stmt_sptr,
		const std::vector<SQLSMALLINT> &col_number_list =
		std::vector<SQLSMALLINT>());

	OdbcHandleStmtSPtr odbc_stmt_sptr_;
	unsigned int       row_alloc_count_;
	SQLSMALLINT        total_col_count_;
	std::size_t        total_row_size_;
	OdbcBinderColList  col_list_;

private:
	void Initialize(unsigned int row_alloc_count,
		OdbcHandleStmtSPtr &odbc_stmt_sptr,
		const std::vector<SQLSMALLINT> &col_number_list =
		std::vector<SQLSMALLINT>());
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
OdbcBinderRow::OdbcBinderRow(OdbcHandleStmtSPtr &odbc_stmt_sptr,
	const std::vector<SQLSMALLINT> &col_number_list)
	:odbc_stmt_sptr_(odbc_stmt_sptr)
	,row_alloc_count_(0)
	,total_col_count_(0)
	,total_row_size_(0)
	,col_list_()
{
	Initialize(1, odbc_stmt_sptr, col_number_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
OdbcBinderRow::OdbcBinderRow(unsigned int row_alloc_count,
	OdbcHandleStmtSPtr &odbc_stmt_sptr,
	const std::vector<SQLSMALLINT> &col_number_list)
	:odbc_stmt_sptr_(odbc_stmt_sptr)
	,row_alloc_count_(0)
	,total_col_count_(0)
	,total_row_size_(0)
	,col_list_()
{
	Initialize(row_alloc_count, odbc_stmt_sptr, col_number_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OdbcBinderRow::Initialize(unsigned int row_alloc_count,
	OdbcHandleStmtSPtr &odbc_stmt_sptr,
	const std::vector<SQLSMALLINT> &col_number_list)
{
	try {
		odbc_stmt_sptr->NumResultCols(&total_col_count_);
		if (total_col_count_ < 1)
			MLB::Utility::ThrowRuntimeError("Invocation of '::SQLNumResultCols()' "
				"reports " + MLB::Utility::AnyToString(total_col_count_) +
				" columns in the result set.");
		std::vector<SQLSMALLINT>        tmp_col_number_list;
		const std::vector<SQLSMALLINT> &col_number_list_ref =
			(col_number_list.empty()) ? tmp_col_number_list : col_number_list;
		if (col_number_list.empty()) {
			tmp_col_number_list.resize(total_col_count_);
			SQLSMALLINT count_1;
			for (count_1 = 0; count_1 < total_col_count_; ++count_1)
				tmp_col_number_list[count_1] = count_1 + 1;
		}
		else {
			std::vector<SQLSMALLINT>::size_type count_1;
			std::vector<SQLSMALLINT>::size_type col_count = col_number_list.size();
			for (count_1 = 0; count_1 < col_count; ++count_1) {
				if ((col_number_list[count_1] < 1) ||
					(col_number_list[count_1] > total_col_count_))
					MLB::Utility::ThrowRuntimeError("The element at index " +
						MLB::Utility::AnyToString(count_1) + " of the column number "
						"vector (" +
						MLB::Utility::AnyToString(col_number_list[count_1]) +
						") is invalid --- the valid range is from the 1 to " +
						MLB::Utility::AnyToString(total_col_count_) +
						", inclusive.");
			}
		}
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to construct an 'OdbcBinderRow' "
			"instance failed: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OdbcWrapper

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::OdbcWrapper;

// ////////////////////////////////////////////////////////////////////////////
void TEST_RawBinder(int &return_code, OdbcHandleDbcSPtr &odbc_dbc_sptr)
{
	try {
		OdbcHandleStmtSPtr odbc_stmt_sptr(odbc_dbc_sptr->CreateStatement());
		SavedStmtAttr<SQL_ATTR_ROW_BIND_TYPE>   saved_bind_type(*odbc_stmt_sptr);
		SavedStmtAttr<SQL_ATTR_ASYNC_ENABLE>    async_enabled(*odbc_stmt_sptr);
		SavedStmtAttr<SQL_ATTR_QUERY_TIMEOUT>   query_timeout(*odbc_stmt_sptr);
		SavedStmtAttr<SQL_ATTR_ROW_ARRAY_SIZE>  row_array_size(*odbc_stmt_sptr);
//SavedStmtAttr<987654321> saved_bind_type;
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		DriverConnectParams connect_params("sa", "S_a_0", "GDSP_LON_TRD_TST",
			"(local)", "{SQL Native Client}");
		connect_params.ParseDriverParams(1, argc, argv);
		OdbcHandleEnv odbc_env;
		odbc_env.SetAttr(SQL_ATTR_ODBC_VERSION, SQL_OV_ODBC3);
		OdbcHandleDbcSPtr odbc_dbc_sptr(odbc_env.AddConnection());
		odbc_dbc_sptr->Connect(connect_params);
		TEST_RawBinder(return_code, odbc_dbc_sptr);
	}
	catch (const std::exception &except) {
		std::cerr << "ERROR IN REGRESSION TEST FOR "
			"'MLB::OdbcWrapper::OdbcBinderRowRaw.cpp': " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

