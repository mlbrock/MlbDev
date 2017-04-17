// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the DictValue class.

	Revision History	:	2008-11-18 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/DictValue.hpp>
#include <VFast/VFastException.hpp>

#include <iomanip>
#include <sstream>
#include <stdexcept>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
DictValue::DictValue(DataType data_type, bool is_null)
	:decimal_(0.0)
	,sint_64_(0)
	,uint_64_(0)
	,sint_32_(0)
	,uint_32_(0)
	,data_type_(data_type)
	,data_flag_((is_null) ? DictValueFlag_IsNull : DictValueFlag_None)
	,data_state_(DictValueState_Undefined)
	,string_length_(0)
{
	string_[0] = '\0';
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
DictValue::DictValue(const char *datum, DataType data_type)
	:decimal_(0.0)
	,sint_64_(0)
	,uint_64_(0)
	,sint_32_(0)
	,uint_32_(0)
	,data_type_(CheckStringCtorType(data_type))
	,data_flag_(DictValueFlag_None)
	,data_state_(DictValueState_Undefined)
	,string_length_(0)
{
	SetValue(datum);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
DictValue::DictValue(unsigned int datum_length, const char *datum,
	DataType data_type)
	:decimal_(0.0)
	,sint_64_(0)
	,uint_64_(0)
	,sint_32_(0)
	,uint_32_(0)
	,data_type_(CheckStringCtorType(data_type))
	,data_flag_(DictValueFlag_None)
	,data_state_(DictValueState_Undefined)
	,string_length_(0)
{
	SetValue(datum_length, datum);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
DictValue::DictValue(const std::string &datum, DataType data_type)
	:decimal_(0.0)
	,sint_64_(0)
	,uint_64_(0)
	,sint_32_(0)
	,uint_32_(0)
	,data_type_(CheckStringCtorType(data_type))
	,data_flag_(DictValueFlag_None)
	,data_state_(DictValueState_Undefined)
	,string_length_(0)
{
	SetValue(datum);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
DictValue::DictValue(Type_SInt32 datum)
	:decimal_(0.0)
	,sint_64_(0)
	,uint_64_(0)
	,sint_32_(datum)
	,uint_32_(0)
	,data_type_(DataType_SInt32)
	,data_flag_(DictValueFlag_None)
	,data_state_(DictValueState_Undefined)
	,string_length_(0)
{
	string_[0] = '\0';
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
DictValue::DictValue(Type_UInt32 datum)
	:decimal_(0.0)
	,sint_64_(0)
	,uint_64_(0)
	,sint_32_(0)
	,uint_32_(datum)
	,data_type_(DataType_UInt32)
	,data_flag_(DictValueFlag_None)
	,data_state_(DictValueState_Undefined)
	,string_length_(0)
{
	string_[0] = '\0';
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
DictValue::DictValue(Type_SInt64 datum)
	:decimal_(0.0)
	,sint_64_(datum)
	,uint_64_(0)
	,sint_32_(0)
	,uint_32_(0)
	,data_type_(DataType_SInt64)
	,data_flag_(DictValueFlag_None)
	,data_state_(DictValueState_Undefined)
	,string_length_(0)
{
	string_[0] = '\0';
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
DictValue::DictValue(Type_UInt64 datum)
	:decimal_(0.0)
	,sint_64_(0)
	,uint_64_(datum)
	,sint_32_(0)
	,uint_32_(0)
	,data_type_(DataType_UInt64)
	,data_flag_(DictValueFlag_None)
	,data_state_(DictValueState_Undefined)
	,string_length_(0)
{
	string_[0] = '\0';
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool DictValue::operator < (const DictValue &other) const
{
	return(Compare(other) < 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool DictValue::IsTypeInt() const
{
	return(MLB::VFast::IsTypeInt(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool DictValue::IsTypeSInt() const
{
	return(MLB::VFast::IsTypeSInt(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool DictValue::IsTypeUInt() const
{
	return(MLB::VFast::IsTypeUInt(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool DictValue::IsTypeInt32() const
{
	return(MLB::VFast::IsTypeInt32(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool DictValue::IsTypeInt64() const
{
	return(MLB::VFast::IsTypeInt64(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool DictValue::IsTypeNumeric() const
{
	return(MLB::VFast::IsTypeNumeric(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool DictValue::IsTypeCharArray() const
{
	return(MLB::VFast::IsTypeCharArray(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool DictValue::IsTypePrimitive() const
{
	return(MLB::VFast::IsTypePrimitive(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool DictValue::IsTypeComposite() const
{
	return(MLB::VFast::IsTypeComposite(data_type_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void DictValue::ClearValue()
{
	data_flag_     = DictValueFlag_IsNull;
	sint_32_       = 0;
	uint_32_       = 0;
	sint_64_       = 0;
	uint_64_       = 0;
	decimal_       = 0.0;
	string_length_ = 0;
	string_[0]     = '\0';
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void DictValue::SetValueFromString(const std::string &string_value,
	DataType data_type)
{
	if (data_type != DataType_Invalid)
		data_type_ = data_type;

	switch (data_type_) {
		case DataType_Ascii					:
		case DataType_Unicode				:
		case DataType_ByteVector			:
			SetValue_String(string_value);
			break;
		case DataType_SInt32					:
			MLB::Utility::ParseCmdLineArg::ParseCmdLineNumeric(string_value,
				sint_32_);
			break;
		case DataType_UInt32					:
			MLB::Utility::ParseCmdLineArg::ParseCmdLineNumeric(string_value,
				uint_32_);
			break;
		case DataType_SInt64					:
			MLB::Utility::ParseCmdLineArg::ParseCmdLineNumeric(string_value,
				sint_64_);
			break;
		case DataType_UInt64					:
			MLB::Utility::ParseCmdLineArg::ParseCmdLineNumeric(string_value,
				uint_32_);
			break;
		case DataType_Decimal				:
			MLB::Utility::ParseCmdLineArg::ParseCmdLineNumeric(string_value,
				decimal_);
			break;
		case DataType_Invalid				:
		case DataType_Sequence				:
		case DataType_Group					:
		case DataType_Template				:
		case DataType_TemplateRefStatic	:
		case DataType_TemplateRefDynamic	:
		case DataType_Count					:
			MLB::Utility::ThrowLogicError("Attempt to set a dictionary value "
				"to a non-primitive type (" + DataTypeToString(data_type_) + ").");
			break;
		default							:
			MLB::Utility::ThrowLogicError("Attempt to set a dictionary value "
				"to an invalid type (" + DataTypeToString(data_type_) + ").");
			break;
	}

	ClearNull();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void DictValue::SetValue_String(unsigned int string_length,
	const char *string_ptr)
{
	if (!IsTypeCharArray())
		MLB::Utility::ThrowInvalidArgument("The data type of this value "
			"datum (" + DataTypeToString(data_type_) + ") can not be set with "
			"a char array value.");

	if (string_length > VFastMaxStringLength)
		MLB::Utility::ThrowInvalidArgument("The size of the string to be copied "
			"in the DictValue (" + MLB::Utility::AnyToString(string_length) +
			") exceeds the maximum permissible (" +
			MLB::Utility::AnyToString(VFastMaxStringLength) + ").");

	string_length_ = string_length;

	memcpy(string_, string_ptr, string_length);

	string_[string_length_] = '\0';

	ClearNull();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void DictValue::SetValue_String(const std::string &in_datum)
{
	SetValue_String(static_cast<unsigned int>(in_datum.size()), in_datum.data());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void DictValue::SetValue_Primitive(const DictValue &value_src)
{
	switch (value_src.data_type_) {
		case DataType_Ascii					:
		case DataType_Unicode				:
		case DataType_ByteVector			:
			string_length_ = value_src.string_length_;
			memcpy(string_, value_src.string_, string_length_);
			break;
		case DataType_SInt32					:
			sint_32_ = value_src.sint_32_;
			break;
		case DataType_UInt32					:
			uint_32_ = value_src.uint_32_;
			break;
		case DataType_SInt64					:
			sint_64_ = value_src.sint_64_;
			break;
		case DataType_UInt64					:
			uint_64_ = value_src.uint_64_;
			break;
		case DataType_Decimal				:
			decimal_ = value_src.decimal_;
			break;
		case DataType_Invalid				:
		case DataType_Sequence				:
		case DataType_Group					:
		case DataType_Template				:
		case DataType_TemplateRefStatic	:
		case DataType_TemplateRefDynamic	:
		case DataType_Count					:
			MLB::Utility::ThrowLogicError("Attempt to set a dictionary value "
				"to a non-primitive type (" + DataTypeToString(data_type_) + ").");
			break;
		default									:
			MLB::Utility::ThrowLogicError("Attempt to set a dictionary value "
				"to an invalid type (" + DataTypeToString(data_type_) + ").");
			break;
	}

	ClearNull();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void DictValue::SetValue(const char *datum)
{
	SetValue(static_cast<unsigned int>(::strlen(datum)), datum);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void DictValue::SetValue(unsigned int datum_length, const char *datum_ptr)
{
	SetValue_String(datum_length, datum_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void DictValue::SetValue(const std::string &datum)
{
	SetValue(static_cast<unsigned int>(datum.size()), datum.data());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void DictValue::SetValue(Type_SInt32 datum)
{
	data_type_ = DataType_SInt32;
	sint_32_   = datum;

	ClearNull();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void DictValue::SetValue(Type_UInt32 datum)
{
	data_type_ = DataType_UInt32;
	uint_32_   = datum;

	ClearNull();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void DictValue::SetValue(Type_SInt64 datum)
{
	data_type_ = DataType_SInt64;
	sint_64_   = datum;

	ClearNull();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void DictValue::SetValue(Type_UInt64 datum)
{
	data_type_ = DataType_UInt64;
	uint_64_   = datum;

	ClearNull();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void DictValue::SetValue(Type_Decimal datum)
{
	data_type_ = DataType_Decimal;
	decimal_   = datum;

	ClearNull();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int DictValue::GetRawDatumSize() const
{
	switch (data_type_) {
		case DataType_Ascii					:
		case DataType_Unicode				:
		case DataType_ByteVector			:
			return(string_length_ + 1);
			break;
		case DataType_SInt32					:
			return(sizeof(sint_32_));
			break;
		case DataType_UInt32					:
			return(sizeof(uint_32_));
			break;
		case DataType_SInt64					:
			return(sizeof(sint_64_));
			break;
		case DataType_UInt64					:
			return(sizeof(uint_64_));
			break;
		case DataType_Decimal				:
			return(sizeof(decimal_));
			break;
		case DataType_Invalid				:
		case DataType_Sequence				:
		case DataType_Group					:
		case DataType_Template				:
		case DataType_TemplateRefStatic	:
		case DataType_TemplateRefDynamic	:
		case DataType_Count					:
		default									:
			break;
	}

	return(0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const void *DictValue::GetPtrToDatumRaw() const
{
	switch (data_type_) {
		case DataType_Ascii					:
		case DataType_Unicode				:
		case DataType_ByteVector			:
			return(string_);
			break;
		case DataType_SInt32					:
			return(&sint_32_);
			break;
		case DataType_UInt32					:
			return(&uint_32_);
			break;
		case DataType_SInt64					:
			return(&sint_64_);
			break;
		case DataType_UInt64					:
			return(&uint_64_);
			break;
		case DataType_Decimal				:
			return(&decimal_);
			break;
		case DataType_Invalid				:
		case DataType_Sequence				:
		case DataType_Group					:
		case DataType_Template				:
		case DataType_TemplateRefStatic	:
		case DataType_TemplateRefDynamic	:
		case DataType_Count					:
		default									:
			break;
	}

	return(NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void *DictValue::GetPtrToDatumRaw()
{
	// Rather than duplicate the code in the const overload...
	return(const_cast<void *>(
		const_cast<const DictValue * const>(this)->GetPtrToDatumRaw()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int DictValue::Compare(const DictValue &other) const
{
	int cmp;

	//	Different types sort in order of the enumeration...
	if ((cmp = (data_type_ - other.data_type_)) != 0)
		return(cmp);

	//	We consider a NULL value to sort before a non-NULL value...
	if (IsNull() && (!other.IsNull()))
		return(-1);
	else if ((!IsNull()) && other.IsNull())
		return(1);

	switch (data_type_) {
		case DataType_Ascii					:
		case DataType_Unicode				:
		case DataType_ByteVector			:
			//	Length is limited in range, so no overflow is possible...
			if ((cmp = (static_cast<int>(string_length_) -
				static_cast<int>(other.string_length_))) != 0)
				return(cmp);
			return(::memcmp(string_, other.string_, string_length_));
			break;
		case DataType_SInt32					:
			return(sint_32_ - other.sint_32_);
			break;
		case DataType_UInt32					:
			if (uint_32_ < other.uint_32_)
				return(-1);
			else if (uint_32_ > other.uint_32_)
				return(1);
			return(0);
			break;
		case DataType_SInt64					:
			if (sint_64_ < other.sint_64_)
				return(-1);
			else if (sint_64_ > other.sint_64_)
				return(1);
			return(0);
			break;
		case DataType_UInt64					:
			if (uint_64_ < other.uint_64_)
				return(-1);
			else if (uint_64_ > other.uint_64_)
				return(1);
			return(0);
			break;
		case DataType_Decimal				:
			if (decimal_ < other.decimal_)
				return(-1);
			else if (decimal_ > other.decimal_)
				return(1);
			return(0);
			break;
		case DataType_Invalid				:
		case DataType_Sequence				:
		case DataType_Group					:
		case DataType_Template				:
		case DataType_TemplateRefStatic	:
		case DataType_TemplateRefDynamic	:
		case DataType_Count					:
		default									:
			break;
	}

	//	Not a primitive (or perhaps valid) type...
	return(0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/*
	This could be implemented as:

		return(Compare(other) != 0);

	but isn't for performance reasons.
*/
bool DictValue::IsEquivalent(const DictValue &other) const
{
	if ((data_type_ != other.data_type_) || (IsNull() != other.IsNull()))
		return(false);

	switch (data_type_) {
		case DataType_Ascii					:
		case DataType_Unicode				:
		case DataType_ByteVector			:
			if ((string_length_ != other.string_length_) ||
				::memcmp(string_, other.string_, string_length_))
				return(false);
			break;
		case DataType_SInt32					:
			if (sint_32_ != other.sint_32_)
				return(false);
			break;
		case DataType_UInt32					:
			if (uint_32_ != other.uint_32_)
				return(false);
			break;
		case DataType_SInt64					:
			if (sint_64_ != other.sint_64_)
				return(false);
			break;
		case DataType_UInt64					:
			if (uint_64_ != other.uint_64_)
				return(false);
			break;
		case DataType_Decimal				:
			//	CODE NOTE: Using a hard-coded epsilon here, should parameterize.
			if ((decimal_ != other.decimal_) &&
				(::fabs(decimal_ - other.decimal_) > 0.00000001))
				return(false);
			break;
		case DataType_Invalid				:
		case DataType_Sequence				:
		case DataType_Group					:
		case DataType_Template				:
		case DataType_TemplateRefStatic	:
		case DataType_TemplateRefDynamic	:
		case DataType_Count					:
		default									:
			break;
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool DictValue::CheckIsEquivalent(const DictValue &other) const
{
	if (!IsEquivalent(other)) {
		std::ostringstream o_str;
		o_str << "Dictionary value instances [" << *this << "] and [" <<
			other << "] are not equivalent: ";
		bool explained_flag = false;
		if (data_type_ != other.data_type_) {
			o_str << "first data type ('" << data_type_ << "') differs from "
				"second ('" << other.data_type_ << "')";
			explained_flag = true;
		}
		if (IsNull() != other.IsNull()) {
			if (explained_flag)
				o_str << " AND ";
			o_str << "first datum is " << ((IsNull()) ? "NULL" : "not NULL") <<
				" second is " << ((other.IsNull()) ? "NULL" : "not NULL");
			explained_flag = true;
		}
		if (!explained_flag)
			o_str << "first datum value (" << ValueToString() << ") differs from "
				"second (" << other.ValueToString() << ")";
		MLB::Utility::ThrowLogicError(o_str.str() + ".");
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void DictValue::IncrementInteger()
{
	switch (data_type_) {
		case DataType_SInt32					:
			++sint_32_;
			break;
		case DataType_UInt32					:
			++uint_32_;
			break;
		case DataType_SInt64					:
			++sint_64_;
			break;
		case DataType_UInt64					:
			++uint_64_;
			break;
		case DataType_Ascii					:
		case DataType_Unicode				:
		case DataType_ByteVector			:
		case DataType_Invalid				:
		case DataType_Decimal				:
		case DataType_Sequence				:
		case DataType_Group					:
		case DataType_Template				:
		case DataType_TemplateRefStatic	:
		case DataType_TemplateRefDynamic	:
		case DataType_Count					:
		default									:
			MLB::Utility::ThrowLogicError("Attempt to increment a non-integral "
				"type (" + DataTypeToString(data_type_) + ").");
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
DataType DictValue::CheckDataType(DataType data_type) const
{
	if (data_type_ != data_type)
		MLB::Utility::ThrowLogicError("The data type of this dictionary value " +
			DataTypeToString(data_type_) + ") is not the same as the specified "
			"dictionary value (" + DataTypeToString(data_type) + ").");

	return(data_type_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string DictValue::ValueToString() const
{
	if ((data_type_ != DataType_Ascii) && (data_type_ != DataType_Unicode) &&
		(data_type_ != DataType_ByteVector)) {
		std::stringstream o_str;
		switch (data_type_) {
			case DataType_SInt32					:
				o_str << sint_32_;
				break;
			case DataType_UInt32					:
				o_str << uint_32_;
				break;
			case DataType_SInt64					:
				o_str << sint_64_;
				break;
			case DataType_UInt64					:
				o_str << uint_64_;
				break;
			case DataType_Decimal				:
				o_str << decimal_;
				break;
			case DataType_Ascii					:
			case DataType_Unicode				:
			case DataType_ByteVector			:
			case DataType_Invalid				:
			case DataType_Sequence				:
			case DataType_Group					:
			case DataType_Template				:
			case DataType_TemplateRefStatic	:
			case DataType_TemplateRefDynamic	:
			case DataType_Count					:
			default									:
				return("*NON-PRIMITIVE-TYPE*");
		}
		return(o_str.str());
	}

	return(std::string(string_, string_length_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string DictValue::ValueToStringWidth(unsigned int width) const
{
	if (!width)
		return(ValueToString());

	std::ostringstream o_str;

	if (IsTypeNumeric())
		o_str << std::setw(static_cast<std::streamsize>(width)) <<
			ValueToString();
	else
		o_str << std::left << std::setw(static_cast<std::streamsize>(width)) <<
			ValueToString() << std::right;

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
DataType DictValue::CheckStringCtorType(DataType data_type)
{
	if ((data_type != DataType_Ascii) && (data_type != DataType_Unicode) &&
		(data_type != DataType_ByteVector))
		MLB::Utility::ThrowInvalidArgument("Attempt to construct a dictionary "
			"value using a string type as an argument, but the specified data "
			"type ('" + DataTypeToString(data_type) + "') is not one of the "
			"string types ('Ascii', 'Unicode' or 'ByteVector').");
	
	return(data_type);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const DictValue &datum)
{
	o_str
		<< std::left << std::setw(10) <<
			DataTypeToString(datum.data_type_) << std::right;

	if ((datum.data_type_ == DataType_Sequence) ||
		 (datum.data_type_ == DataType_Template))
		o_str << " = [] (N/A)";
	else if (datum.IsNull())
		o_str << " = [] (NULL)";
	else
		o_str << " = [" << datum.ValueToString() << "]";

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

