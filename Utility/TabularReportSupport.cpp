//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	TabularReportSupport.cpp

	File Description	:	Implementation of the TabularReportSupport class.

	Revision History	:	2015-06-29 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/TabularReportSupport.hpp>
#include <Utility/Utility_Exception.hpp>

#include <iosfwd>

#include <boost/io/ios_state.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
TabularReportSupport::TabularReportSupport(
	const std::vector<std::size_t> &min_width_list,
	const std::vector<std::string> &name_list_1,
	const std::vector<std::string> &name_list_2,
	const std::vector<std::string> &name_list_3)
	:min_width_list_(min_width_list)
	,max_width_list_(min_width_list)
	,name_list_()
{
	if (!name_list_1.empty())
		name_list_.push_back(name_list_1);

	if (!name_list_2.empty()) {
		name_list_.resize(2);
		name_list_[1] = name_list_2;
	}

	if (!name_list_3.empty()) {
		name_list_.resize(3);
		name_list_[2] = name_list_3;
	}

	//	Complete construction
	ForceRecalculatation();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TabularReportSupport::TabularReportSupport(
	const std::vector<std::string> &name_list_1,
	const std::vector<std::string> &name_list_2,
	const std::vector<std::string> &name_list_3)
	:min_width_list_()
	,max_width_list_()
	,name_list_()
{
	TabularReportSupport(std::vector<std::size_t>(),
		name_list_1, name_list_2, name_list_3).swap(*this);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TabularReportSupport::swap(TabularReportSupport &other)
{
	min_width_list_.swap(other.min_width_list_);
	max_width_list_.swap(other.max_width_list_);
	name_list_.swap(other.name_list_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t TabularReportSupport::GetTotalWidth() const
{
	std::size_t total_width = 0;

	for (std::size_t count_1 = 0; count_1 < GetColCount(); ++count_1)
		total_width += ((count_1) ? 1 : 0) + GetMaxColWidth(count_1);

	return(total_width);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t TabularReportSupport::GetMinColWidth(std::size_t col_index) const
{
	CheckThisIndex(GetColCount(), col_index, "column");

	return(min_width_list_[col_index]);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t TabularReportSupport::GetMaxColWidth(std::size_t col_index) const
{
	CheckThisIndex(GetColCount(), col_index, "column");

	return(max_width_list_[col_index]);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const std::string &TabularReportSupport::GetColName(std::size_t row_index,
	std::size_t col_index) const
{
	CheckThisIndex(GetRowCount(), row_index, "row");
	CheckThisIndex(GetColCount(), col_index, "column");

	return(name_list_[row_index][col_index]);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TabularReportSupport::SetColWidth(std::size_t col_index,
	std::size_t value)
{
	CheckIndex(col_index, "column");

	if (col_index >= GetColCount())
		SetColCount(col_index + 1);

	if (value != min_width_list_[col_index]) {
		min_width_list_[col_index] = value;
		RecalculateColWidth(col_index);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TabularReportSupport::SetColName(std::size_t row_index,
	std::size_t col_index, const std::string &value)
{
	CheckIndex(row_index, "row");
	CheckIndex(col_index, "column");

	if (row_index >= GetRowCount())
		SetRowCount(row_index + 1);

	if (col_index >= GetColCount())
		SetColCount(col_index + 1);

	std::size_t old_width = name_list_[row_index][col_index].size();

	name_list_[row_index][col_index] = value;

	if (value.size() != old_width)
		RecalculateColWidth(col_index);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TabularReportSupport::SetRowCount(std::size_t row_count)
{
	std::size_t old_row_count = GetRowCount();

	if (row_count < old_row_count)
		name_list_.resize(row_count);
	else if (row_count != old_row_count) {
		name_list_.resize(row_count);
		for (std::size_t count_1 = old_row_count; count_1 < row_count; ++count_1)
			name_list_[count_1].resize(GetColCount());
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TabularReportSupport::SetColCount(std::size_t col_count)
{
	if (col_count == GetColCount())
		return;

	min_width_list_.resize(col_count);
	max_width_list_.resize(col_count);

	for (std::size_t count_1 = 0; count_1 < GetRowCount(); ++count_1)
		name_list_[count_1].resize(col_count);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::vector<std::size_t> &TabularReportSupport::GetMinWidthList()
{
	return(min_width_list_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::vector<std::size_t> &TabularReportSupport::GetMaxWidthList()
{
	return(max_width_list_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::vector<std::vector<std::string> > &TabularReportSupport::GetNameList()
{
	return(name_list_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t TabularReportSupport::AssertColCountMinimum(
	std::size_t col_count_min) const
{
	if (GetColCount() < col_count_min) {
		std::ostringstream o_str;
		o_str << "The number of columns in an instance of TabularReportSupport"
			" (" << GetColCount() << ") is less than the minimum required by the "
			"application (" << col_count_min << ").";
		MLB::Utility::ThrowLogicError(o_str.str());
	}

	return(GetColCount());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const std::vector<std::size_t> &TabularReportSupport::GetMinWidthList() const
{
	return(min_width_list_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const std::vector<std::size_t> &TabularReportSupport::GetMaxWidthList() const
{
	return(max_width_list_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const std::vector<std::vector<std::string> > &
	TabularReportSupport::GetNameList() const
{
	return(name_list_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TabularReportSupport::ForceRecalculatation()
{
	std::size_t col_count =
		std::max(min_width_list_.size(), max_width_list_.size());

	for (std::size_t count_1 = 0; count_1 < GetRowCount(); ++count_1)
		col_count = std::max(col_count, name_list_[count_1].size());

	min_width_list_.resize(col_count);
	max_width_list_.resize(col_count);

	for (std::size_t count_1 = 0; count_1 < GetRowCount(); ++count_1)
		name_list_[count_1].resize(col_count);

	for (std::size_t count_1 = 0; count_1 < col_count; ++count_1)
		RecalculateColWidth(count_1);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TabularReportSupport::RecalculateColWidth(std::size_t col_index)
{
	std::size_t max_width = min_width_list_[col_index];

	for (std::size_t count_1 = 0; count_1 < GetRowCount(); ++count_1)
		max_width = std::max(max_width, name_list_[count_1][col_index].size());

	max_width_list_[col_index] = max_width;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TabularReportSupport::CheckThisIndex(std::size_t element_count,
	std::size_t index_value, const char *index_name) const
{
	if (index_value >= element_count) {
		std::ostringstream o_str;
		o_str << "The specified " << index_name << " index value (" <<
			index_value << ") is not less than the number of " << index_name <<
			"s (" << element_count << ").";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TabularReportSupport::CheckIndex(std::size_t index_value,
	const char *index_name)
{
	if (index_value == std::numeric_limits<std::size_t>::max()) {
		std::ostringstream o_str;
		o_str << "The specified " << index_name << " index value (" <<
			index_value << ") is not less than the maximum representable (" <<
			index_value << ").";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &TabularReportSupport::EmitFillLine(std::ostream &o_str,
	char fill_char, char col_sep_char, bool with_new_line) const
{
	if (!GetColCount())
		return(o_str);

	boost::io::ios_all_saver io_state(o_str);

	o_str << std::setfill(fill_char);

	for (std::size_t count_1 = 0; count_1 < GetColCount(); ++count_1) {
		if (count_1)
			o_str << col_sep_char;
		o_str << std::setw(GetMaxColWidthSS(count_1)) << "";
	}

	if (with_new_line)
		o_str << "\n";

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &TabularReportSupport::EmitFillLineUnbroken(std::ostream &o_str,
	char fill_char, bool with_new_line) const
{
	return(EmitFillLine(o_str, fill_char, fill_char, with_new_line));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &TabularReportSupport::EmitFillLinesUnbroken(std::ostream &o_str,
	char fill_char, std::size_t line_count, bool with_new_line) const
{
	if (!GetColCount())
		return(o_str);

	if (line_count == 1)
		EmitFillLine(o_str, fill_char, fill_char, with_new_line);
	else if (line_count > 1) {
		std::ostringstream tmp_o_str;
		EmitFillLine(tmp_o_str, fill_char, fill_char, false);
		while (line_count--) {
			o_str << tmp_o_str.str() <<
				((with_new_line || line_count) ? "\n" : "");
		}
	}

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &TabularReportSupport::EmitHeader(std::ostream &o_str,
	char fill_char_1, char fill_char_2, char col_sep_char,
	bool with_new_line) const
{
	if (!GetColCount())
		return(o_str);

	EmitFillLine(o_str, fill_char_1, col_sep_char, true);

	{
		boost::io::ios_all_saver io_state(o_str);
		o_str << std::left;
		for (std::size_t count_1 = 0; count_1 < GetRowCount(); ++count_1) {
			for (std::size_t count_2 = 0; count_2 < GetColCount(); ++count_2) {
				if (count_2)
					o_str << col_sep_char;
				o_str << std::setw(GetMaxColWidthSS(count_2)) <<
					GetColName(count_1, count_2);
			}
			o_str << "\n";
		}
	}

	return(EmitFillLine(o_str, fill_char_2, col_sep_char, with_new_line));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &TabularReportSupport::EmitTrailer(std::ostream &o_str,
	char fill_char, char col_sep_char, bool with_new_line) const
{
	return(EmitFillLine(o_str, fill_char, col_sep_char, with_new_line));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &TabularReportSupport::EmitCentered(const std::string &center_text,
	std::ostream &o_str, char fill_char, bool with_new_line) const
{
	std::size_t total_width = GetTotalWidth();

	if (center_text.size() >= total_width)
		o_str << center_text.substr(0, total_width);
	else {
		std::size_t              pad_length = total_width - center_text.size();
		boost::io::ios_all_saver io_state(o_str);
		o_str << std::setfill(fill_char)
			<< std::setw(static_cast<std::streamsize>(pad_length / 2)) << ""
			<< center_text
			<< std::setw(static_cast<std::streamsize>(
				(pad_length / 2) + (pad_length % 2))) << ""
				;
	}

	if (with_new_line)
		o_str << "\n";

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/InlineContainer.hpp>

namespace {

// ////////////////////////////////////////////////////////////////////////////
const MLB::Utility::TabularReportSupport MyTabularReportSupport(
	MLB::Utility::MakeInlineVector<std::size_t>(23)(7)(23)(10)(2)(19),
	MLB::Utility::MakeInlineVector<std::string>
		("Line")("Base")("Base")("Fix Version")("Numeric")("More"),
	MLB::Utility::MakeInlineVector<std::string>
		("Info")("Name 1")("Name 2")("Added")("Value")("Data"));
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_RunTest()
{
	MyTabularReportSupport.EmitHeader();

	std::cout
		<< std::left
		<< std::setw(MyTabularReportSupport[0]) << "Test Line"         << " "
		<< std::setw(MyTabularReportSupport[1]) << "A Base Name"       << " "
		<< std::setw(MyTabularReportSupport[2]) << "Another Base Name" << " "
		<< std::setw(MyTabularReportSupport[3]) << "FIX Version"       << " "
		<< std::right
		<< std::setw(MyTabularReportSupport[4]) << 666 << " "
		<< "Whatever... Some more data..."
			;

	MyTabularReportSupport.EmitHeader();
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_RunTest();
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

