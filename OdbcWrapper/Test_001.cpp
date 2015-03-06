// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for TibCo/Rendezvous 6 and above.

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

#include <OdbcWrapper/OdbcHandle.hpp>

// ////////////////////////////////////////////////////////////////////////////

#include <Utility/InlineContainer.hpp>
#include <Utility/IncrementalBuffer.hpp>

#ifdef _Windows
# pragma warning(disable:4217 4668)
#endif // #ifdef _Windows

#include <boost/shared_array.hpp>

#ifdef _Windows
# pragma warning(default:4217 4668)
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////
template <typename BufferType>
	void TEST_IncrementalBufferCycle(std::size_t alloc_gran,
	const std::vector<int> &size_list)
{
	BufferType tmp_buffer(alloc_gran);

	unsigned int                     char_index = 0;
	std::vector<int>::const_iterator iter_b(size_list.begin());
	std::vector<int>::const_iterator iter_e(size_list.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		std::string src_data(*iter_b, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[char_index++]);
		char_index %= 26;
		tmp_buffer.SetValue(src_data.c_str(), src_data.size() + 1);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_IncrementalBuffer()
{
	TEST_IncrementalBufferCycle<MLB::Utility::IncrementalBuffer<char, 16> >(
		2, MLB::Utility::MakeInlineVector<int>
		(14)
		(15)
		(16)
		(17)
		(18)
		(19));
}
// ////////////////////////////////////////////////////////////////////////////

using namespace MLB::OdbcWrapper;

// ////////////////////////////////////////////////////////////////////////////
void TEST_Drivers___Basic(SQLHENV env, int &return_code)
{
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << "TEST_Drivers___Basic()" << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	try {
		SQLUSMALLINT direction = SQL_FETCH_FIRST;
		for ( ; ; ) {
			SQLSMALLINT driver_ret;
			SQLSMALLINT attr_ret;
			SQLCHAR     driver[256];
			SQLCHAR     attr[256];
			SQLRETURN ret = Drivers(env, direction, driver, sizeof(driver),
				&driver_ret, attr, sizeof(attr), &attr_ret,
				OdbcThrowFlags::Ok_SWI_or_NoData);
			if (ret == SQL_NO_DATA) {
				std::cout << "*** All drivers enumerated." << std::endl;
				break;
			}
			direction = SQL_FETCH_NEXT;
			std::cout << driver << " ---> " << attr;
			if (ret == SQL_SUCCESS_WITH_INFO)
				std::cout << " (data truncation)";
			std::cout << std::endl;
		}
	}
	catch (const std::exception &except) {
		std::cout << "ERROR in regression test TEST_Drivers___Basic(): " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_Drivers___AttrInfoBasic(OdbcHandleEnv &env, int &return_code)
{
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << "TEST_Drivers___AttrInfoBasic()" << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	try {
		AttrInfoBasicVector driver_list;
		env.GetAttrInfoBasicList(driver_list);
		AttrInfoBasicVectorIterC iter_b(driver_list.begin());
		AttrInfoBasicVectorIterC iter_e(driver_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			std::cout << iter_b->description_ << std::endl;
			std::vector<std::string>::const_iterator
				iter_al_b(iter_b->attributes_list_.begin());
			std::vector<std::string>::const_iterator
				iter_al_e(iter_b->attributes_list_.end());
			for ( ; iter_al_b != iter_al_e; ++iter_al_b)
				std::cout << "   " << *iter_al_b << std::endl;
			std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
		}
	}
	catch (const std::exception &except) {
		std::cout << "ERROR in regression test "
			"TEST_Drivers___AttrInfoBasic(): " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_Drivers___DataSourceBasic(OdbcHandleEnv &env, int &return_code)
{
	const char *this_name = "TEST_Drivers___DataSourceBasic()";

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << this_name << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	try {
		AttrInfoBasicVector driver_list;
		env.GetDataSourceBasicList(driver_list);
		AttrInfoBasicVectorIterC iter_b(driver_list.begin());
		AttrInfoBasicVectorIterC iter_e(driver_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			std::cout << iter_b->description_ << std::endl;
			std::vector<std::string>::const_iterator
				iter_al_b(iter_b->attributes_list_.begin());
			std::vector<std::string>::const_iterator
				iter_al_e(iter_b->attributes_list_.end());
			for ( ; iter_al_b != iter_al_e; ++iter_al_b)
				std::cout << "   " << *iter_al_b << std::endl;
			std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
		}
	}
	catch (const std::exception &except) {
		std::cout << "ERROR in regression test " << this_name << ": " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_OdbcHandleDbc(int &return_code)
{
	const char *this_name = "TEST_OdbcHandleDbc()";

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << this_name << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	try {
		OdbcHandleEnv odbc_env;
		odbc_env.SetAttr(SQL_ATTR_ODBC_VERSION, SQL_OV_ODBC3);
		OdbcHandleDbcSPtr odbc_dbc_sptr(odbc_env.AddConnection());
		odbc_dbc_sptr->Connect(
			"Database=AdventureWorksDW;Driver={SQL Native Client};Uid=sa;Server=(local);Pwd=S_a_0");
		std::cout << "   Connection created." << std::endl;
	}
	catch (const std::exception &except) {
		std::cout << "ERROR in regression test " << this_name << ": " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_OdbcHandleStmt(int &return_code)
{
	const char *this_name = "TEST_OdbcHandleStmt()";

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << this_name << std::endl;
	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	try {
		OdbcHandleEnv odbc_env;
		odbc_env.SetAttr(SQL_ATTR_ODBC_VERSION, SQL_OV_ODBC3);
		OdbcHandleDbcSPtr odbc_dbc_sptr(odbc_env.AddConnection());
		odbc_dbc_sptr->Connect(
			"Database=AdventureWorksDW;Driver={SQL Native Client};Uid=sa;Server=(local);Pwd=S_a_0");
		std::cout << "   Connection created." << std::endl;
		std::vector<std::string> table_list;
		{
			std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
			std::cout << "Test of '::SQLGetData()'..." << std::endl;
			std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
			OdbcHandleStmtSPtr odbc_stmt_sptr(odbc_dbc_sptr->CreateStatement());
			std::cout << "   Statement created." << std::endl;
			odbc_stmt_sptr->Tables();
			SQLSMALLINT col_count;
			odbc_stmt_sptr->NumResultCols(&col_count);
			std::cout << "   Total of " << col_count << " columns located in "
				"tables." << std::endl;
			unsigned int row_count = 0;
			SQLRETURN    row_return_code;
			while (SQL_SUCCEEDED(row_return_code = odbc_stmt_sptr->Fetch())) {
				std::cout << "   " << std::setw(10) << ++row_count << ": " <<
					std::flush;
				SQLRETURN   col_return_code;
				SQLINTEGER  indicator;
				char        buffer[4095 + 1];
				SQLSMALLINT col_index;
				for (col_index = 1; col_index <= col_count; ++col_index) {
					col_return_code = odbc_stmt_sptr->GetData(col_index, SQL_C_CHAR,
						buffer, static_cast<SQLUSMALLINT>(sizeof(buffer)),
						&indicator);
					if ((col_index == 3) && (indicator != SQL_NULL_DATA))
						table_list.push_back(buffer);
					std::cout <<
						((col_index > 1) ? "|" : "") << 
						((indicator == SQL_NULL_DATA) ? "*NULL*" : buffer) <<
						std::flush;
				}
				std::cout << std::endl;

			}
			std::cout << "   ---> " << row_count << " rows found." << std::endl;
			std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
		}
		// ////////////////////////////////////////////////////////////////
		{
			std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
			std::cout << "Test of '::SQLBindCol()'..." << std::endl;
			std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
			OdbcHandleStmtSPtr odbc_stmt_sptr(odbc_dbc_sptr->CreateStatement());
			std::cout << "   Statement created." << std::endl;
			odbc_stmt_sptr->Tables();
			SQLSMALLINT col_count;
			odbc_stmt_sptr->NumResultCols(&col_count);
			std::cout << "   Total of " << col_count << " columns located in "
				"tables." << std::endl;
			typedef char BufferType[1023 + 1];
			boost::shared_array<BufferType> buffer_list(new BufferType[col_count]);
			boost::shared_array<SQLLEN>     ind_list(new SQLLEN[col_count]);
			SQLSMALLINT                     col_index;
			for (col_index = 0; col_index < col_count; ++col_index)
				odbc_stmt_sptr->BindCol(col_index + 1, SQL_C_CHAR,
					buffer_list[col_index],
					static_cast<SQLUSMALLINT>(sizeof(buffer_list[col_index])),
					&(ind_list[col_index]));
			unsigned int row_count = 0;
			SQLRETURN    row_return_code;
			while (SQL_SUCCEEDED(row_return_code = odbc_stmt_sptr->Fetch())) {
				std::cout << "   " << std::setw(10) << ++row_count << ": " <<
					std::flush;
				for (col_index = 0; col_index < col_count; ++col_index) {
					std::cout <<
						((col_index > 0) ? "|" : "") << 
						((ind_list[col_index] == SQL_NULL_DATA) ? "*NULL*" :
						buffer_list[col_index]) << std::flush;
				}
				std::cout << std::endl;
			}
			std::cout << "   ---> " << row_count << " rows found." << std::endl;
			std::cout << MLB::Utility::PadLeft("", 79, '-') << std::endl;
		}
	}
	catch (const std::exception &except) {
		std::cout << "ERROR in regression test " << this_name << ": " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cout << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cout << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
TEST_IncrementalBuffer();
		OdbcHandleEnv odbc_env;
		odbc_env.SetAttr(SQL_ATTR_ODBC_VERSION, SQL_OV_ODBC3);
SQLINTEGER attr_value = 666;
odbc_env.GetAttr(SQL_ATTR_ODBC_VERSION, attr_value);
		TEST_Drivers___Basic(odbc_env.GetNativeHandle(), return_code);
		TEST_Drivers___AttrInfoBasic(odbc_env, return_code);
		TEST_Drivers___DataSourceBasic(odbc_env, return_code);
//TEST_OdbcHandleDbc(return_code);
TEST_OdbcHandleStmt(return_code);
	}
	catch (const std::exception &except) {
		std::cout << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

