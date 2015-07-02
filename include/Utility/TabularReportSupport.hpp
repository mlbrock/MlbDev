//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	TabularReportSupport.hpp

	File Description	:	Definition of the TabularReportSupport class.

	Revision History	:	2015-06-29 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
#ifndef HH__MLB__Utility__TabularReportSupport_hpp__HH

#define HH__MLB__Utility__TabularReportSupport_hpp__HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

#include <ostream>
#include <string>
#include <vector>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
class TabularReportSupport {
public:
	explicit TabularReportSupport(
		const std::vector<std::string> &name_list_1 = std::vector<std::string>(),
		const std::vector<std::string> &name_list_2 = std::vector<std::string>(),
		const std::vector<std::string> &name_list_3 = std::vector<std::string>());
	explicit TabularReportSupport(const std::vector<std::size_t> &min_width_list,
		const std::vector<std::string> &name_list_1 = std::vector<std::string>(),
		const std::vector<std::string> &name_list_2 = std::vector<std::string>(),
		const std::vector<std::string> &name_list_3 = std::vector<std::string>());

	void swap(TabularReportSupport &other);

	inline std::size_t GetRowCount() const
	{
		return(name_list_.size());
	}

	inline std::size_t GetColCount() const
	{
		return(min_width_list_.size());
	}

	std::size_t GetTotalWidth() const;

	std::size_t GetMinColWidth(std::size_t col_index) const;
	std::size_t GetMaxColWidth(std::size_t col_index) const;

	inline std::streamsize GetMinColWidthSS(std::size_t col_index) const
	{
		return(static_cast<std::streamsize>(GetMinColWidth(col_index)));
	}

	inline std::streamsize GetMaxColWidthSS(std::size_t col_index) const
	{
		return(static_cast<std::streamsize>(GetMaxColWidth(col_index)));
	}

	const std::string &GetColName(std::size_t row_index,
		std::size_t col_index) const;

	void SetColWidth(std::size_t col_index, std::size_t value);
	void SetColName(std::size_t row_index, std::size_t col_index,
		const std::string &value);

	void SetRowCount(std::size_t row_count);
	void SetColCount(std::size_t col_count);

	std::vector<std::size_t>               &GetMinWidthList();
	std::vector<std::size_t>               &GetMaxWidthList();
	std::vector<std::vector<std::string> > &GetNameList();

	const std::vector<std::size_t>               &GetMinWidthList() const;
	const std::vector<std::size_t>               &GetMaxWidthList() const;
	const std::vector<std::vector<std::string> > &GetNameList() const;

	std::size_t AssertColCountMinimum(std::size_t col_count_min) const;

	std::streamsize operator [] (std::size_t col_index) const
	{
		return(GetMaxColWidthSS(col_index));
	}

	void ForceRecalculatation();

	std::ostream &EmitFillLine(std::ostream &o_str = std::cout,
		char fill_char = '=', char col_sep_char = ' ',
		bool with_new_line = true) const;
	std::ostream &EmitFillLineUnbroken(std::ostream &o_str = std::cout,
		char fill_char = '=', bool with_new_line = true) const;
	std::ostream &EmitFillLinesUnbroken(std::ostream &o_str = std::cout,
		char fill_char = '=', std::size_t line_count = 1,
		bool with_new_line = true) const;
	std::ostream &EmitHeader(std::ostream &o_str = std::cout,
		char fill_char_1 = '=', char fill_char_2 = '=', char col_sep_char = ' ',
		bool with_new_line = true) const;
	std::ostream &EmitTrailer(std::ostream &o_str = std::cout,
		char fill_char = '=', char col_sep_char = ' ',
		bool with_new_line = true) const;
	std::ostream &EmitCentered(const std::string &center_text,
		std::ostream &o_str = std::cout,
		char fill_char = '=', bool with_new_line = true) const;


private:
	std::vector<std::size_t>               min_width_list_;
	std::vector<std::size_t>               max_width_list_;
	std::vector<std::vector<std::string> > name_list_;

	void RecalculateColWidth(std::size_t col_index);

	void CheckThisIndex(std::size_t element_count, std::size_t index_value,
		const char *index_name) const;

	static void CheckIndex(std::size_t index_value, const char *index_name);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__TabularReportSupport_hpp__HH

