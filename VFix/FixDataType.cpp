// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFix Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	PFixFieldType.cpp

	File Description	:	Implementation of the PFixFieldType class.

	Revision History	:	2015-06-27 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RapidXmlUtils/RapidXmlContext.hpp>
#include <RapidXmlUtils/XmlDomElement.hpp>

//	CODE NOTE: Needed by the .cpp only.
#include <Utility/C_StringSupport.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

//	Needed for Utility/ColReportHelper.hpp.
#include <Utility/InlineContainer.hpp>

//	Needed for Utility/ColReportHelper.cpp.
//	Needed for PFixDataType.hpp
#include <boost/io/ios_state.hpp>

/*
	CODE NOTE: To be removed.
namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
std::ostream &EmitFillLine(const std::vector<std::streamsize> &col_list,
	std::ostream &o_str, char fill_char, char col_sep_char, bool emit_new_line)
{
	if (!col_list.empty()) {
		boost::io::ios_all_saver                     io_state(o_str);
		std::vector<std::streamsize>::const_iterator iter_b(col_list.begin());
		std::vector<std::streamsize>::const_iterator iter_e(col_list.end());
		std::vector<std::streamsize>::const_iterator iter_this(iter_b);
		o_str << std::setfill(fill_char);
		for ( ; iter_this != iter_e; ++iter_this) {
			if (iter_this != iter_b)
				o_str << col_sep_char;
			o_str << std::setw(*iter_this) << "";
		}
	}

	if (emit_new_line)
		o_str << "\n";

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB
*/

// ////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to ColReportHelper.hpp.
/*
	Need to save the original widths as the minimum widths for a column.

	Also need an accessor for this new member and a way to change the values
	for the columns in it.
*/
class ColReportHelper {
public:
	explicit ColReportHelper(
		const std::vector<std::string> &name_list_1 = std::vector<std::string>(),
		const std::vector<std::string> &name_list_2 = std::vector<std::string>(),
		const std::vector<std::string> &name_list_3 = std::vector<std::string>());
	explicit ColReportHelper(const std::vector<std::size_t> &min_width_list,
		const std::vector<std::string> &name_list_1 = std::vector<std::string>(),
		const std::vector<std::string> &name_list_2 = std::vector<std::string>(),
		const std::vector<std::string> &name_list_3 = std::vector<std::string>());

	void swap(ColReportHelper &other);

	inline std::size_t ColReportHelper::GetRowCount() const
	{
		return(name_list_.size());
	}

	inline std::size_t ColReportHelper::GetColCount() const
	{
		return(min_width_list_.size());
	}

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
	std::ostream &EmitHeader(std::ostream &o_str = std::cout,
		char fill_char_1 = '=', char fill_char_2 = '=', char col_sep_char = ' ',
		bool with_new_line = true) const;
	std::ostream &EmitTrailer(std::ostream &o_str = std::cout,
		char fill_char = '=', char col_sep_char = ' ',
		bool with_new_line = true) const;

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

// ////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to ColReportHelper.cpp.
ColReportHelper::ColReportHelper(
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
//	CODE NOTE: Move to ColReportHelper.cpp.
ColReportHelper::ColReportHelper(
	const std::vector<std::string> &name_list_1,
	const std::vector<std::string> &name_list_2,
	const std::vector<std::string> &name_list_3)
	:min_width_list_()
	,max_width_list_()
	,name_list_()
{
	ColReportHelper::ColReportHelper(std::vector<std::size_t>(),
		name_list_1, name_list_2, name_list_3).swap(*this);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to ColReportHelper.cpp.
void ColReportHelper::swap(ColReportHelper &other)
{
	min_width_list_.swap(other.min_width_list_);
	max_width_list_.swap(other.max_width_list_);
	name_list_.swap(other.name_list_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to ColReportHelper.cpp.
std::size_t ColReportHelper::GetMinColWidth(std::size_t col_index) const
{
	CheckThisIndex(GetColCount(), col_index, "column");

	return(min_width_list_[col_index]);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to ColReportHelper.cpp.
std::size_t ColReportHelper::GetMaxColWidth(std::size_t col_index) const
{
	CheckThisIndex(GetColCount(), col_index, "column");

	return(max_width_list_[col_index]);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to ColReportHelper.cpp.
const std::string &ColReportHelper::GetColName(std::size_t row_index,
	std::size_t col_index) const
{
	CheckThisIndex(GetRowCount(), row_index, "row");
	CheckThisIndex(GetColCount(), col_index, "column");

	return(name_list_[row_index][col_index]);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to ColReportHelper.cpp.
void ColReportHelper::SetColWidth(std::size_t col_index, std::size_t value)
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
//	CODE NOTE: Move to ColReportHelper.cpp.
void ColReportHelper::SetColName(std::size_t row_index, std::size_t col_index,
	const std::string &value)
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
//	CODE NOTE: Move to ColReportHelper.cpp.
void ColReportHelper::SetRowCount(std::size_t row_count)
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
//	CODE NOTE: Move to ColReportHelper.cpp.
void ColReportHelper::SetColCount(std::size_t col_count)
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
//	CODE NOTE: Move to ColReportHelper.cpp.
std::vector<std::size_t> &ColReportHelper::GetMinWidthList()
{
	return(min_width_list_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to ColReportHelper.cpp.
std::vector<std::size_t> &ColReportHelper::GetMaxWidthList()
{
	return(max_width_list_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to ColReportHelper.cpp.
std::vector<std::vector<std::string> > &ColReportHelper::GetNameList()
{
	return(name_list_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to ColReportHelper.cpp.
std::size_t ColReportHelper::AssertColCountMinimum(
	std::size_t col_count_min) const
{
	if (GetColCount() < col_count_min) {
		std::ostringstream o_str;
		o_str << "The number of columns in an instance of ColReportHelper (" <<
			GetColCount() << ") is less than the minimum required by the "
			"application (" << col_count_min << ").";
		MLB::Utility::ThrowLogicError(o_str.str());
	}

	return(GetColCount());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to ColReportHelper.cpp.
const std::vector<std::size_t> &ColReportHelper::GetMinWidthList() const
{
	return(min_width_list_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to ColReportHelper.cpp.
const std::vector<std::size_t> &ColReportHelper::GetMaxWidthList() const
{
	return(max_width_list_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to ColReportHelper.cpp.
const std::vector<std::vector<std::string> > &
	ColReportHelper::GetNameList() const
{
	return(name_list_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to ColReportHelper.cpp.
void ColReportHelper::ForceRecalculatation()
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
//	CODE NOTE: Move to ColReportHelper.cpp.
void ColReportHelper::RecalculateColWidth(std::size_t col_index)
{
	std::size_t max_width = min_width_list_[col_index];

	for (std::size_t count_1 = 0; count_1 < GetRowCount(); ++count_1)
		max_width = std::max(max_width, name_list_[count_1][col_index].size());

	max_width_list_[col_index] = max_width;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to ColReportHelper.cpp.
void ColReportHelper::CheckThisIndex(std::size_t element_count,
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
//	CODE NOTE: Move to ColReportHelper.cpp.
void ColReportHelper::CheckIndex(std::size_t index_value,
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
//	CODE NOTE: Move to ColReportHelper.cpp.
std::ostream &ColReportHelper::EmitFillLine(std::ostream &o_str, char fill_char,
	char col_sep_char, bool with_new_line) const
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
//	CODE NOTE: Move to ColReportHelper.cpp.
std::ostream &ColReportHelper::EmitHeader(std::ostream &o_str,
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
//	CODE NOTE: Move to ColReportHelper.cpp.
std::ostream &ColReportHelper::EmitTrailer(std::ostream &o_str, char fill_char,
	char col_sep_char, bool with_new_line) const
{
	return(EmitFillLine(o_str, fill_char, col_sep_char, with_new_line));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to some .hpp file...
const std::size_t MaxTextLength_PFixFieldType     = 15;
const std::size_t MaxTextLength_PFixFieldTypeBase = 24;

const std::size_t MaxTextLength_FixVersion      = 15;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to VFixXPortType.hpp.
enum VFixXPortType {
	VFixXPortType_Int,
	VFixXPortType_IntNonNegative,		//	>= 0
	VFixXPortType_IntPositive,			//	>  0
	VFixXPortType_IntLength,
	VFixXPortType_IntTagNum,			//	>  0, no leading zeroes in string
	VFixXPortType_IntSeqNum,			//	>  0
	VFixXPortType_IntNumInGroup,		//	>= 0
	VFixXPortType_IntDayOfMonth,		//	>= 1 <= 12
	VFixXPortType_Float,
	VFixXPortType_FloatDecimal,
	VFixXPortType_FloatQty,
	VFixXPortType_FloatPrice,
	VFixXPortType_FloatPriceOffset,
	VFixXPortType_FloatAmt,
	VFixXPortType_FloatPercentage,	
	VFixXPortType_Char,
	VFixXPortType_CharBoolean,			//	'Y' or 'N'
	VFixXPortType_MultiValueChar,		//	Single-char fields seperated by space.
	VFixXPortType_MultiValueString,	//	Multi-char fields seperated by space.	
	VFixXPortType_String,
	VFixXPortType_StringLanguage,
	VFixXPortType_StringRawData,
	VFixXPortType_StringTenor,			//	{D|M|W|Y}x (x > 0)
	VFixXPortType_StringXmlData,
	VFixXPortType_StringIsoCountry,	//	ISO 3166 country code
	VFixXPortType_StringIsoCurrency,	//	ISO 4217 currecny code
	VFixXPortType_StringIsoExchange,	//	ISO 10383 market identifier code
	VFixXPortType_MonthYear,				//	yyyymm[dd|ww]
	VFixXPortType_UtcTimestamp,			//	yyyy-mm-ddThh:mm:ss[.xxx]
	VFixXPortType_UtcTimeOnly,			//	hh:mm:ss[.xxx]
	VFixXPortType_UtcDateOnly,			//	yyyy-mm-dd
	VFixXPortType_LocalMktDate,			//	ISO 8601 (yyyy-mm-dd local-time)
	VFixXPortType_TzTimeOnly,			//	hh:mm[:ss][Z|OFF[+|-hh[:mm]]]
	VFixXPortType_TzTimestamp,			//	yyyymmdd-hh:mm:ss[Z|OFF[+|-hh[:mm]]]
	VFixXPortType_Date,
	VFixXPortType_DateTime,
	VFixXPortType_Time,
	VFixXPortType_ReservedPlus100,	//	Values > 100 are reserved.
	VFixXPortType_ReservedPlus1000,	//	Values > 1000 are reserved.
	VFixXPortType_ReservedPlus4000,	//	Values > 4000 are reserved.
	VFixXPortType_None,
	VFixXPortType_Minimum =  VFixXPortType_Int,
	VFixXPortType_Maximum =  VFixXPortType_None,
	VFixXPortType_Count   = (VFixXPortType_Maximum - VFixXPortType_Minimum) + 1
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to VFixXPortType.hpp.
VFixXPortType StringToVFixXPortType(const char *src,
	bool throw_on_error = true);
VFixXPortType StringToVFixXPortType(const std::string &src,
	bool throw_on_error = true);
VFixXPortType StringToVFixXPortType(const char *src_1, const char *src_2,
	bool throw_on_error = true);
VFixXPortType StringToVFixXPortType(const std::string &src_1,
	const std::string &src_2, bool throw_on_error = true);

const char *VFixXPortTypeToString(VFixXPortType src,
	bool throw_on_error = true);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to some .hpp file...
const char *CopyToString(std::size_t src_len, const char *src_str,
	std::size_t dst_len, char *dst_str, bool can_truncate_flag = false);
const char *CopyToString(const char *src_str, std::size_t dst_len,
	char *dst_str, bool can_truncate_flag = false);
const char *CopyToString(const std::string &src_str, std::size_t dst_len,
	char *dst_ptr, bool can_truncate_flag = false);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to PFixFieldType.hpp.
class PFixFieldType {
public:
	typedef std::set<PFixFieldType> PFixFieldTypeSet_I;

	//	Constructor used for searches...
	explicit PFixFieldType(const char *name = "");

	explicit PFixFieldType(const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	bool operator < (const PFixFieldType &other) const
	{
		return(name_ < other.name_);
	}

	void swap(PFixFieldType &other);

	friend std::ostream & operator << (std::ostream &o_str,
		const PFixFieldType &datum);

	static bool ShouldApplyXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element);

	static PFixFieldTypeSet_I &LoadFromXmlElement(
		const MLB::RapidXmlUtils::XmlDomElement &xml_element,
		PFixFieldTypeSet_I &out_set);

	static PFixFieldTypeSet_I &LoadFromXmlFile(const std::string &file_name,
		PFixFieldTypeSet_I &out_set);
	static PFixFieldTypeSet_I  LoadFromXmlFile(const std::string &file_name);

	//	CODE NOTE: For testing. May be removed at some later date.
	static std::set<int> &CheckForUnusedTypes(const PFixFieldTypeSet_I &in_set,
		std::set<int> &out_set);
	static std::size_t    EmitUnusedTypes(const PFixFieldTypeSet_I &in_set,
		std::ostream &o_str = std::cout);

	std::string   name_;
	std::string   base_name_1_;
	std::string   base_name_2_;
	std::string   fix_version_added_;
	std::string   description_;
	VFixXPortType vfix_xport_type_;

private:
	PFixFieldType(const std::string &name, const std::string &base_name_1,
		const std::string &base_name_2, const std::string &fix_version_added,
		const std::string &description, VFixXPortType vfix_xport_type);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<PFixFieldType>          PFixFieldTypeVector;
typedef PFixFieldTypeVector::iterator       PFixFieldTypeVectorIter;
typedef PFixFieldTypeVector::const_iterator PFixFieldTypeVectorIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef PFixFieldType::PFixFieldTypeSet_I     PFixFieldTypeSet;
typedef PFixFieldTypeSet::iterator            PFixFieldTypeSetIter;
typedef PFixFieldTypeSet::const_iterator      PFixFieldTypeSetIterC;
typedef std::pair<PFixFieldTypeSetIter, bool> PFixFieldTypeSetInsertPair;
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to some .cpp file...
const char *CopyToString(std::size_t src_len, const char *src_str,
	std::size_t dst_len, char *dst_str, bool can_truncate_flag)
{
	if (src_len <= dst_len)
		::strcpy(dst_str, src_str);
	else if (!can_truncate_flag){
		std::ostringstream o_str;
		o_str << "Unable to copy a source string of length " << src_len <<
			" because it exceeds the maximum destination string length of " <<
			dst_len << ".";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}
	else
		MLB::Utility::nstrcpy(dst_str, src_str, dst_len);

	return(dst_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *CopyToString(const char *src_str, std::size_t dst_len,
	char *dst_str, bool can_truncate_flag)
{
	return(CopyToString(::strlen(src_str), src_str, dst_len, dst_str,
		can_truncate_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *CopyToString(const std::string &src_str, std::size_t dst_len,
	char *dst_str, bool can_truncate_flag)
{
	return(CopyToString(src_str.size(), src_str.c_str(), dst_len, dst_str,
		can_truncate_flag));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

namespace {

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to VFixXPortType.cpp.
const char *DataTypeStringFixToVFixList[VFixXPortType_Count][2] = {
	{ "int"                   , "Int"               },
	{ "xs:nonNegativeInteger" , "IntNonNegative"    },
	{ "xs:positiveInteger"    , "IntPositive"       },
	{ "Length"                , "IntLength"         },
	{ "TagNum"                , "IntTagNum"         },
	{ "SeqNum"                , "IntSeqNum"         },
	{ "NumInGroup"            , "IntNumInGroup"     },
	{ "DayOfMonth"            , "IntDayOfMonth"     },
	{ "float"                 , "Float"             },
	{ "xs:decimal"            , "FloatDecimal"      },
	{ "Qty"                   , "FloatQty"          },
	{ "Price"                 , "FloatPrice"        },
	{ "PriceOffset"           , "FloatPriceOffset"  },
	{ "Amt"                   , "FloatAmt"          },
	{ "Percentage"            , "FloatPercentage"   },
	{ "Char"                  , "Char"              },
	{ "Boolean"               , "CharBoolean"       },
	{ "MultipleCharValue"     , "MultiValueChar"    },
	{ "MultipleStringValue"   , "MultiValueString"  },
	{ "String"                , "String"            },
	{ "Language"              , "StringLanguage"    },
	{ "data"                  , "StringRawData"     },
	{ "Tenor"                 , "StringTenor"       },
	{ "XMLData"               , "StringXmlData"     },
	{ "Country"               , "StringIsoCountry"  },
	{ "Currency"              , "StringIsoCurrency" },
	{ "Exchange"              , "StringIsoExchange" },
	{ "MonthYear"             , "MonthYear"         },
	{ "UTCTimestamp"          , "UtcTimestamp"      },
	{ "UTCTimeOnly"           , "UtcTimeOnly"       },
	{ "UTCDateOnly"           , "UtcDateOnly"       },
	{ "LocalMktDate"          , "LocalMktDate"      },
	{ "TZTimeOnly"            , "TzTimeOnly"        },
	{ "TZTimestamp"           , "TzTimestamp"       },
	{ "Date"                  , "Date"              },
	{ "DateTime"              , "DateTime"          },
	{ "Time"                  , "Time"              },
	{ "Reserved100Plus"       , "ReservedPlus100"   },
	{ "Reserved1000Plus"      , "ReservedPlus1000"  },
	{ "Reserved4000Plus"      , "ReservedPlus4000"  },
	{ "None"                  , "None"              }
};
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to VFixXPortType.cpp.
VFixXPortType StringToVFixXPortType(const char *src, bool throw_on_error)
{
	if (src && *src) {
		for (int count_1 = 0; count_1 < VFixXPortType_Count; ++count_1) {
			if (!MLB::Utility::Utility_stricmp(src,
				DataTypeStringFixToVFixList[count_1][0]))
				return(static_cast<VFixXPortType>(count_1));
		}
		//	Look-aside...
		if (!MLB::Utility::Utility_stricmp(src, "xs:integer"))
			return(VFixXPortType_Int);
		else if (!MLB::Utility::Utility_stricmp(src, "xs:string"))
			return(VFixXPortType_String);
/*
	Can't resolve these uniquely as they're actually semantic types.
		else if (!MLB::Utility::Utility_stricmp(src, "xs:date"))
		else if (!MLB::Utility::Utility_stricmp(src, "xs:time"))
		else if (!MLB::Utility::Utility_stricmp(src, "xs:dateTime"))
*/
		else if (!MLB::Utility::Utility_stricmp(src, "xs:language"))
			return(VFixXPortType_StringLanguage);
		else if (throw_on_error) {
			std::ostringstream o_str;
			o_str << "Unable to map the string '" << src << "' to a VFix "
				"data type enumeration.";
			MLB::Utility::ThrowInvalidArgument(o_str.str());
		}
	}
	else if (throw_on_error)
		MLB::Utility::ThrowInvalidArgument("No data type name specified.");

	return(VFixXPortType_None);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to VFixXPortType.cpp.
VFixXPortType StringToVFixXPortType(const std::string &src,
	bool throw_on_error)
{
	return(StringToVFixXPortType(src.c_str(), throw_on_error));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to VFixXPortType.cpp.
VFixXPortType StringToVFixXPortType(const char *src_1, const char *src_2,
	bool throw_on_error)
{
	if (((!src_1) || (!(*src_1))) && ((!src_2) || (!(*src_2))))
		return(StringToVFixXPortType(NULL, throw_on_error));
	else if ((src_1 && *src_1) && ((!src_2) || (!(*src_2))))
		return(StringToVFixXPortType(src_1, throw_on_error));
	else if (((!src_1) || (!(*src_1))) && (src_2 || (*src_2)))
		return(StringToVFixXPortType(src_2, throw_on_error));

	VFixXPortType return_code = StringToVFixXPortType(src_1, false);

	return((return_code != VFixXPortType_None) ? return_code :
		StringToVFixXPortType(src_2, throw_on_error));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to VFixXPortType.cpp.
VFixXPortType StringToVFixXPortType(const std::string &src_1,
	const std::string &src_2, bool throw_on_error)
{
	return(StringToVFixXPortType(src_1.c_str(), src_2.c_str(),
		throw_on_error));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: Move to VFixXPortType.cpp.
const char *VFixXPortTypeToString(VFixXPortType src, bool throw_on_error)
{
	if ((src < VFixXPortType_Minimum) || (src > VFixXPortType_Maximum)) {
		if (throw_on_error) {
			std::ostringstream o_str;
			o_str << "Invalid VFixXPortType enumeration value (" << src << ").";
			MLB::Utility::ThrowInvalidArgument(o_str.str());
		}
		return("*INVALID*");
	}

	return(DataTypeStringFixToVFixList[src][1]);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFix {

// ////////////////////////////////////////////////////////////////////////////
PFixFieldType::PFixFieldType(const char *name)
	:name_(name)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixFieldType::PFixFieldType(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element)
try
	:name_()
	,base_name_1_()
	,base_name_2_()
	,fix_version_added_()
	,description_()
	,vfix_xport_type_(VFixXPortType_None)
{
	using namespace MLB::RapidXmlUtils;

	if ((xml_element.node_type_ != XmlDomElement::NodeType_Element) ||
		(xml_element.element_name_ != "Datatype")) {
		std::ostringstream o_str;
		o_str << "Unexpected XML combination of element name ('" <<
			xml_element.element_name_ << "') and node type (" <<
			xml_element.node_type_ << "), expected element name 'Datatype' and "
			"node type " << XmlDomElement::NodeType_Element << ".";
		MLB::Utility::ThrowInvalidArgument(o_str.str());
	}		
		
	const std::string   &added(xml_element.GetAttributeValue("added", true));
	const std::string   &name(xml_element.GetChildRef("Name").
		GetNodeTextFromChildRef());
	const std::string   &description(xml_element.GetChildRef("Description").
		GetNodeTextFromChildRef());
	const XmlDomElement *base_info_ptr_1(xml_element.GetChildPtr("BaseType"));
	const std::string   *base_name_ptr_1 = NULL;
	const std::string   *base_name_ptr_2 = NULL;

	if (base_info_ptr_1) {
		base_name_ptr_1 = &base_info_ptr_1->GetNodeTextFromChildRef();
		if (xml_element.GetChildPtr("XML") &&
			xml_element.GetChildRef("XML").GetChildPtr("Base"))
			base_name_ptr_2 = &xml_element.GetChildRef("XML").GetChildRef("Base").
				GetNodeTextFromChildRef();
	}
	else {
		try {
			base_name_ptr_2 = &xml_element.GetChildRef("XML").GetChildRef("Base").
				GetNodeTextFromChildRef();
		}
		catch (const std::exception &except) {
			MLB::Utility::ThrowInvalidArgument("Able to locate the FIX "
				"datatype base name in neither <Datatype><Base> nor "
				"<Datatype><XML><Base>: " + std::string(except.what()));
		}
	}

	PFixFieldType(name, (base_name_ptr_1) ? *base_name_ptr_1 : "",
		(base_name_ptr_2) ? *base_name_ptr_2 : "", added, description,
/*
		StringToVFixXPortType(
			(base_name_ptr_1) ? base_name_ptr_1->c_str() : NULL,
			(base_name_ptr_2) ? base_name_ptr_2->c_str() : NULL, true)).
*/
		StringToVFixXPortType(name.c_str(), true)).
		swap(*this);
}
catch (const std::exception &except) {
	MLB::Utility::Rethrow(except, "Unable to construct an PFixFieldType "
		"instance: " + std::string(except.what()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixFieldType::PFixFieldType(const std::string &name,
	const std::string &base_name_1, const std::string &base_name_2,
	const std::string &fix_version_added, const std::string &description,
	VFixXPortType vfix_xport_type)
	:name_(name)
	,base_name_1_(base_name_1)
	,base_name_2_(base_name_2)
	,fix_version_added_(fix_version_added)
	,description_(description)
	,vfix_xport_type_(vfix_xport_type)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void PFixFieldType::swap(PFixFieldType &other)
{
	std::swap(name_, other.name_);
	std::swap(base_name_1_, other.base_name_1_);
	std::swap(base_name_2_, other.base_name_2_);
	std::swap(fix_version_added_, other.fix_version_added_);
	std::swap(description_, other.description_);
	std::swap(vfix_xport_type_, other.vfix_xport_type_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool PFixFieldType::ShouldApplyXmlElement(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element)
{
	using namespace MLB::RapidXmlUtils;

	return((xml_element.node_type_ == XmlDomElement::NodeType_Element) &&
		(xml_element.element_name_ == "Datatype") &&
		(xml_element.GetChildRef("Name").GetNodeTextFromChildRef() !=
		"Pattern"));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixFieldTypeSet &PFixFieldType::LoadFromXmlElement(
	const MLB::RapidXmlUtils::XmlDomElement &xml_element,
	PFixFieldTypeSet &out_set)
{
	using namespace MLB::RapidXmlUtils;

	PFixFieldTypeSet tmp_set;

	try {
		if ((xml_element.node_type_ != XmlDomElement::NodeType_Unknown) ||
			(xml_element.element_name_ != "Datatypes")) {
			std::ostringstream o_str;
			o_str << "Expected the major node in the XML for a FIX datatype to "
				"have the name 'Datatypes' and the node type " <<
				XmlDomElement::NodeType_Unknown << " , but its actual name is '" << 
				xml_element.element_name_ << "' and the node type is " <<
				xml_element.node_type_ << ".";
			MLB::Utility::ThrowLogicError(o_str.str());
		}
		for (std::size_t count_1 = 0; count_1 < xml_element.child_list_.size();
			++count_1) {
			const XmlDomElement &this_element(xml_element.child_list_[count_1]);
			if (ShouldApplyXmlElement(this_element)) {
				PFixFieldType tmp_datum(this_element);
				PFixFieldTypeSetInsertPair insert_pair(tmp_set.insert(tmp_datum));
				if (!insert_pair.second) {
					std::ostringstream o_str;
					o_str << "Attempt to insert datatype element for datatype "
						"name '" << tmp_datum.name_ << "' failed because it is "
						"already in the set of datatypes.";
					MLB::Utility::ThrowInvalidArgument(o_str.str());
				}
			}
		}
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Attempt to parse the FIX datatypes XML data failed: " <<
			except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	out_set.swap(tmp_set);

	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixFieldTypeSet &PFixFieldType::LoadFromXmlFile(const std::string &file_name,
	PFixFieldTypeSet &out_set)
{
	MLB::RapidXmlUtils::RapidXmlContext xml_context;
	MLB::RapidXmlUtils::XmlDomElement   xml_element;

	try {
		MLB::RapidXmlUtils::XmlDomElement::ParseXmlFile(file_name,
			xml_element);
		LoadFromXmlElement(xml_element, out_set);
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Attempt to parse the FIX datatypes XML file '" <<
			file_name << "' failed: " << except.what();
		MLB::Utility::Rethrow(except, o_str.str());
	}

	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
PFixFieldTypeSet PFixFieldType::LoadFromXmlFile(const std::string &file_name)
{
	PFixFieldTypeSet out_set;

	return(LoadFromXmlFile(file_name, out_set));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::set<int> &PFixFieldType::CheckForUnusedTypes(
	const PFixFieldTypeSet &in_set, std::set<int> &out_set)
{
	std::set<int> tmp_set;

	for (int count_1 = VFixXPortType_Minimum; count_1 <= VFixXPortType_Maximum;
		++count_1) {
		if (count_1 != VFixXPortType_None)
			tmp_set.insert(count_1);
	}

	PFixFieldTypeSetIterC iter_b(in_set.begin());
	PFixFieldTypeSetIterC iter_e(in_set.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		std::set<int>::iterator iter_f(tmp_set.find(iter_b->vfix_xport_type_));
		if (iter_f != tmp_set.end())
			tmp_set.erase(iter_f);
	}

	out_set.swap(tmp_set);

	return(out_set);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &EmitFillLine(const std::vector<std::streamsize> &col_list,
	std::ostream &o_str = std::cout, char fill_char = '=',
	char col_sep_char = ' ', bool emit_new_line = true);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t PFixFieldType::EmitUnusedTypes(const PFixFieldTypeSet &in_set,
	std::ostream &o_str)
{
	std::set<int> out_set;

	CheckForUnusedTypes(in_set, out_set);

	std::set<int>::const_iterator iter_b(out_set.begin());
	std::set<int>::const_iterator iter_e(out_set.end());

	for ( ; iter_b != iter_e; ++iter_b)
		o_str << DataTypeStringFixToVFixList[*iter_b][1] << std::endl;

	return(out_set.size());
}
// ////////////////////////////////////////////////////////////////////////////

/*
	CODE NOTE: To be removed.
// ////////////////////////////////////////////////////////////////////////////
const MLB::Utility::InlineContainer<std::streamsize> &ResolveColWidths(
	MLB::Utility::InlineContainer<std::streamsize> &start_widths,
	const std::vector<std::string> &name_list_1 = std::vector<std::string>(),
	const std::vector<std::string> &name_list_2 = std::vector<std::string>(),
	const std::vector<std::string> &name_list_3 = std::vector<std::string>())
{
	std::size_t size_0 = start_widths.size();
	std::size_t size_1 = name_list_1.size();
	std::size_t size_2 = name_list_2.size();
	std::size_t size_3 = name_list_3.size();
	std::size_t max_name_list_size = std::max(size_1, std::max(size_2, size_3));

	if (max_name_list_size > size_0)
		start_widths.resize(max_name_list_size);

	for (std::size_t count_1 = 0; count_1 < max_name_list_size; ++count_1)
		std::size_t this_size = std::max(start_widths[count_1],
			std::max(((count_1 < size_1) ? name_list_1[count_1].size() : 0),
				std::max(((count_1 < size_2) ? name_list_2[count_1].size() : 0),
					((count_1 < size_3) ? name_list_3[count_1].size() : 0))));

	return(start_widths);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const MLB::Utility::InlineContainer<std::streamsize> ResolveColWidths(
	const std::vector<std::string> &name_list_1 = std::vector<std::string>(),
	const std::vector<std::string> &name_list_2 = std::vector<std::string>(),
	const std::vector<std::string> &name_list_3 = std::vector<std::string>())
{
	MLB::Utility::InlineContainer<std::streamsize> start_widths;

	return(ResolveColWidths(start_widths, name_list_1, name_list_2,
		name_list_3));
}
// ////////////////////////////////////////////////////////////////////////////
*/

namespace {

// ////////////////////////////////////////////////////////////////////////////
/*
const MLB::Utility::InlineContainer<std::string>     ColNames_PFixFieldType_1(
	MLB::Utility::MakeInlineVector<std::string>
		("Type")("Base")("Base")("Fix Version")("XPort")("XPort"));
const MLB::Utility::InlineContainer<std::string>     ColNames_PFixFieldType_2(
	MLB::Utility::MakeInlineVector<std::string>
		("Name")("Name 1")("Name 2")("Added")("Value")("Name"));
const MLB::Utility::InlineContainer<std::streamsize> ColWidths_PFixFieldType(
	ResolveColWidths(
		MLB::Utility::MakeInlineVector<std::streamsize>(23)(7)(23)(10)(2)(10),
		ColNames_PFixFieldType_1, ColNames_PFixFieldType_2);
*/
const MLB::Utility::ColReportHelper MyColReportHelper(
	MLB::Utility::MakeInlineVector<std::size_t>(23)(7)(23)(10)(2)(19),
	MLB::Utility::MakeInlineVector<std::string>
		("Type")("Base")("Base")("Fix Version")("XPort")("XPort"),
	MLB::Utility::MakeInlineVector<std::string>
		("Name")("Name 1")("Name 2")("Added")("Value")("Name"));
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const PFixFieldType &datum)
{
	MyColReportHelper.AssertColCountMinimum(6);

	boost::io::ios_all_saver io_state(o_str);

	o_str
		<< std::left
		<< std::setw(MyColReportHelper[0]) << datum.name_              << " "
		<< std::setw(MyColReportHelper[1]) << datum.base_name_1_       << " "
		<< std::setw(MyColReportHelper[2]) << datum.base_name_2_       << " "
		<< std::setw(MyColReportHelper[3]) << datum.fix_version_added_ << " "
		<< std::right
		<< std::setw(MyColReportHelper[4]) << datum.vfix_xport_type_   << " "
		<< VFixXPortTypeToString(datum.vfix_xport_type_)
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str,
	const PFixFieldTypeSet &datum)
{
	MyColReportHelper.EmitHeader(o_str, '=', '-', ' ', true);

	PFixFieldTypeSetIterC iter_b(datum.begin());
	PFixFieldTypeSetIterC iter_e(datum.end());

	for ( ; iter_b != iter_e; ++iter_b)
		o_str << *iter_b << std::endl;

	MyColReportHelper.EmitTrailer(o_str, '=', ' ', true);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFix

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/StringSupport.hpp>
#include <Utility/ParseCmdLineArg.hpp>

using namespace MLB::VFix;

namespace {

// ////////////////////////////////////////////////////////////////////////////
void TEST_RunTest(const char *file_name)
{
/*
std::cout << "GetFillLineString(42)                        : " <<
	MLB::Utility::GetFillLineString(42, 10)    << std::endl;
std::cout << "GetFillLineString(static_cast<double>(42.5)) : " <<
	MLB::Utility::GetFillLineString(static_cast<double>(42.5), 10)  << std::endl;
std::cout << "GetFillLineString(static_cast<float>(42.6))  : " <<
	MLB::Utility::GetFillLineString(static_cast<float>(42.6), 10)  << std::endl;
std::cout << "GetFillLineString(\"Why?\")                    : " <<
	MLB::Utility::GetFillLineString("Why", 10) << std::endl;

std::cout << "GetFillLineStringX(42)                       : " <<
	MLB::Utility::GetFillLineStringX(42, 10)    << std::endl;
std::cout << "GetFillLineString(static_cast<double>(42.5)) : " <<
	MLB::Utility::GetFillLineStringX(static_cast<double>(42.5), 10)  << std::endl;
std::cout << "GetFillLineStringX(static_cast<float>(42.6)) : " <<
	MLB::Utility::GetFillLineStringX(static_cast<float>(42.6), 10)  << std::endl;
std::cout << "GetFillLineStringX(\"Why?\")                   : " <<
	MLB::Utility::GetFillLineStringX("Why", 10) << std::endl;
*/

/*
	PFixFieldTypeSet      element_set(PFixFieldType::LoadFromXmlFile(file_name));
	PFixFieldTypeSetIterC iter_b(element_set.begin());
	PFixFieldTypeSetIterC iter_e(element_set.end());

	for ( ; iter_b != iter_e; ++iter_b)
		std::cout << *iter_b << std::endl;
*/

	PFixFieldTypeSet element_set(PFixFieldType::LoadFromXmlFile(file_name));

	std::cout << element_set << std::endl;

	PFixFieldType::EmitUnusedTypes(element_set);
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	using namespace MLB::Utility;

	int return_code = EXIT_SUCCESS;

	try {
		if (ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
			std::cout << "USAGE: " << std::endl <<
				"   " << argv[0] << " <xml-file-name> [ <xml-file-name> ... ]" <<
				std::endl << std::endl;
			return(EXIT_SUCCESS);
		}
		else if (argc < 2)
			ThrowInvalidArgument("No XML files were specified.");
		for (int count_1 = 1; count_1 < argc; ++count_1) {
			TEST_RunTest(argv[count_1]);
		}
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

/*
	Testing command line for Linux:
	------- ------- ---- --- ------
	..\..\..\..\..\VFix\XML\FIX.5.0SP2\Base\Datatypes.xml > TestResults.InitDatatypesXml.txt

	Testing command line for Windows:
	------- ------- ---- --- --------
	XML/FIX.5.0SP2/Base/Datatypes.xml > TestResults.InitDatatypesXml.txt
*/

#endif // #ifdef TEST_MAIN





























