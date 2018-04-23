//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for enumeration flags support.

	Revision History	:	2005-09-03 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
#ifndef HH__MLB__Utility__EnumFlags_hpp__HH

#define HH__MLB__Utility__EnumFlags_hpp__HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/EnumFlagOps.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
const unsigned int EnumFlagNameFullIdx  = 0;
const unsigned int EnumFlagNameShortIdx = 1;
const unsigned int EnumFlagNameCount    = 2;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ThisEnumType>
	struct EnumFlagDatum {
	typedef typename ThisEnumType EnumType;
	EnumFlagDatum(ThisEnumType enum_value, const std::string &enum_name,
		const std::string &short_name)
		:enum_value_(enum_value)
		,name_list_()
	{
		name_list_[0] = enum_name;
		name_list_[1] = short_name;
	}
	//	Constructor used for std::set<EnumFlagDatum>::find()...
	EnumFlagDatum(ThisEnumType enum_value)
		:enum_value_(enum_value)
		,name_list_()
	{
	}

	bool operator < (const EnumFlagDatum &other) const {
		return(enum_value_ < other.enum_value_);
	}

	ThisEnumType enum_value_;
	std::string  name_list_[EnumFlagNameCount];
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ThisEnumType>
	struct EnumFlags {
	typedef std::set<EnumFlagDatum<ThisEnumType> > MyEnumList;
	typedef typename MyEnumList::iterator          MyEnumListIter;
	typedef typename MyEnumList::const_iterator    MyEnumListIterC;
	EnumFlags(const std::string &enum_name, unsigned int enum_count,
		const EnumFlagDatum<ThisEnumType> enum_list[],
		const std::string &enum_none_name = "",
		const std::string &enum_mask_name = "")
		:enum_name_(enum_name)
		,enum_list_(enum_list, enum_list + enum_count)
		,enum_none_(static_cast<ThisEnumType>(0))
		,enum_mask_(static_cast<ThisEnumType>(0))
		,enum_none_name_(enum_none_name)
		,enum_mask_name_(enum_mask_name)
		,permit_none_flag_((enum_none_name.empty()) ? false : true)
		,permit_mask_flag_((enum_mask_name.empty()) ? false : true)
	{
		//	If an insert failed, we have duplicates...
		if (enum_count != static_cast<unsigned int>(enum_list_.size()))
			ThrowInvalidArgument("Unable to insert all enumeration value data: "
				"could there be a duplicate value in the list?");
		std::string     tmp_enum_none_name(UpperCase(enum_none_name_));
		std::string     tmp_enum_mask_name(UpperCase(enum_mask_name_));
		MyEnumListIterC iter_b(enum_list_.begin());
		MyEnumListIterC iter_e(enum_list_.end());
		while (iter_b != iter_e) {
			enum_mask_ = EnumFlagOr(enum_mask_, iter_b->enum_value_);
			if (!iter_b->enum_value_)
				enum_none_ = *iter_b;
			++iter_b;
		}
	}

	bool IsValid(ThisEnumType enum_value) const {
		return(enum_list_.find(enum_value) != enum_list_.end());
	}
	bool IsValidAny(ThisEnumType enum_value) const {
		return(IsValid(enum_value) ||
			(permit_none_flag_ && (enum_value == enum_none_.enum_value_)));
	}
	bool IsValidMask(ThisEnumType enum_value) const {
		return((enum_value & (~enum_mask_)) ? false : true);
	}
	bool IsValidMaskAny(ThisEnumType enum_value) const {
		return(IsValidAny(enum_value) || IsValidMask(enum_value));
	}
	ThisEnumType CheckValid(ThisEnumType enum_value) const {
		if (!IsValid(enum_value))
			ThrowIt(enum_value);
		return(enum_value);
	}
	ThisEnumType CheckValidAny(ThisEnumType enum_value) const {
		if (!IsValidAny(enum_value))
			ThrowIt(enum_value);
		return(enum_value);
	}
	ThisEnumType CheckValidMask(ThisEnumType enum_value) const {
		if (!IsValidMask(enum_value))
			ThrowIt(enum_value);
		return(enum_value);
	}
	ThisEnumType CheckValidMaskAny(ThisEnumType enum_value) const {
		if (!IsValidMaskAny(enum_value))
			ThrowIt(enum_value);
		return(enum_value);
	}
	ThisEnumType FlagAnd(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		return(EnumFlagAnd(enum_value_1, enum_value_2));
	}
	ThisEnumType FlagOr(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		return(EnumFlagOr(enum_value_1, enum_value_2));
	}
	ThisEnumType FlagXor(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		return(EnumFlagXor(enum_value_1, enum_value_2));
	}
	ThisEnumType FlagSet(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		return(EnumFlagSet(enum_value_1, enum_value_2));
	}
	ThisEnumType FlagClear(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		return(EnumFlagClear(enum_value_1, enum_value_2));
	}
	ThisEnumType ValueAdd(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		return(ValueAdd(enum_value_1, enum_value_2));
	}
	ThisEnumType ValueSubtract(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		return(ValueSubtract(enum_value_1, enum_value_2));
	}
	ThisEnumType ValueIncrement(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		return(ValueIncrement(enum_value_1, enum_value_2));
	}
	ThisEnumType ValueDecrement(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		return(ValueDecrement(enum_value_1, enum_value_2));
	}
	ThisEnumType FlagAndChecked(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		CheckValidMaskAny(enum_value_1);
		CheckValidMaskAny(enum_value_2);
		return(EnumFlagAnd(enum_value_1, enum_value_2));
	}
	ThisEnumType FlagOrChecked(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		CheckValidMaskAny(enum_value_1);
		CheckValidMaskAny(enum_value_2);
		return(EnumFlagOr(enum_value_1, enum_value_2));
	}
	ThisEnumType FlagXorChecked(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		CheckValidMaskAny(enum_value_1);
		CheckValidMaskAny(enum_value_2);
		return(EnumFlagXor(enum_value_1, enum_value_2));
	}
	ThisEnumType FlagSetChecked(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		CheckValidMaskAny(enum_value_1);
		CheckValidMaskAny(enum_value_2);
		return(EnumFlagSet(enum_value_1, enum_value_2));
	}
	ThisEnumType FlagClearChecked(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		CheckValidMaskAny(enum_value_1);
		CheckValidMaskAny(enum_value_2);
		return(EnumFlagClear(enum_value_1, enum_value_2));
	}
	ThisEnumType ValueAddChecked(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		CheckValidMaskAny(enum_value_1);
		CheckValidMaskAny(enum_value_2);
		return(ValueAdd(enum_value_1, enum_value_2));
	}
	ThisEnumType ValueSubtractChecked(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		CheckValidMaskAny(enum_value_1);
		CheckValidMaskAny(enum_value_2);
		return(ValueSubtract(enum_value_1, enum_value_2));
	}
	ThisEnumType ValueIncrementChecked(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		CheckValidMaskAny(enum_value_1);
		CheckValidMaskAny(enum_value_2);
		return(ValueIncrement(enum_value_1, enum_value_2));
	}
	ThisEnumType ValueDecrementChecked(ThisEnumType enum_value_1,
		ThisEnumType enum_value_2) const {
		CheckValidMaskAny(enum_value_1);
		CheckValidMaskAny(enum_value_2);
		return(ValueDecrement(enum_value_1, enum_value_2));
	}
	std::string ToString(ThisEnumType enum_value) const {
		try {
			return(ToStringChecked(enum_value));
		}
		catch (...) {
		}
		return(GetInvalidString(enum_value));
	}
	std::string ToStringFull(ThisEnumType enum_value) const {
		return("'" + ToString(enum_value) + "' = " +
			ValueToStringHex(enum_value));
	}
	std::string ToStringChecked(ThisEnumType enum_value) const {
		CheckValidAny(enum_value);
		return(enum_list_.find(static_cast<ThisEnumType>(enum_value))->
			name_list_[EnumFlagNameShortIdx]);
	}
	std::string MaskToStringFull(ThisEnumType enum_value) const {
		return("'" + MaskToString(enum_value) + "' = " +
			ValueToStringHex(enum_value));
	}
	std::string MaskToString(ThisEnumType enum_value) const {
		try {
			return(MaskToStringChecked(enum_value));
		}
		catch (...) {
		}
		return(GetInvalidString(enum_value));
	}
	std::string MaskToStringChecked(ThisEnumType enum_value) const {
		if (enum_value & (~enum_mask_))
			MLB::Utility::ThrowInvalidArgument("Unable to convert " + enum_name_ +
				" mask " + ValueToStringBin(static_cast<int>(enum_value)) + " = " +
				AnyToString(static_cast<int>(enum_value)) + " to a " + enum_name_ +
				" enumeration mask string.");
		if (enum_value == enum_none_.enum_value_)
			return(enum_none_.name_list_[EnumFlagNameShortIdx]);
		std::string out_string;
		MyEnumListIterC iter_b(enum_list_.begin());
		MyEnumListIterC iter_e(enum_list_.end());
		while (iter_b != iter_e) {
			if (EnumFlagAnd(enum_value, iter_b->enum_value_))
				out_string += ((out_string.empty()) ? "" : "|") +
					iter_b->name_list_[EnumFlagNameShortIdx];
			++iter_b;
		}
		return(out_string);
	}
	ThisEnumType FromString(const std::string &enum_value) const {
		try {
			return(FromStringChecked(enum_value));
		}
		catch (...) {
		}
		return(static_cast<ThisEnumType>(static_cast<unsigned int>(~enum_mask_)));
	}
	ThisEnumType FromStringChecked(const std::string &enum_value) const {
		std::string tmp_enum_value(UpperCase(enum_value));
		if (permit_none_flag_ && (!enum_none_name_.empty()) &&
			(UpperCase(enum_none_name_) == tmp_enum_value))
			return(enum_none_.enum_value_);
		MyEnumListIterC iter_b(enum_list_.begin());
		MyEnumListIterC iter_e(enum_list_.end());
		while (iter_b != iter_e) {
			unsigned int count_1;
			for (count_1 = 0; count_1 < EnumFlagNameCount; ++count_1) {
				if (tmp_enum_value == UpperCase(iter_b->name_list_[count_1]))
					return(iter_b->enum_value_);
			}
			++iter_b;
		}
		MLB::Utility::ThrowInvalidArgument("Unable to convert the string '" +
			enum_value + "' to a " + enum_name_ + " enumeration.");
		//	For compilers without really smart global data-flow analysis...
		//	That is, all compilers...
		return(static_cast<ThisEnumType>
			(static_cast<unsigned int>(~enum_mask_)));
	}
	ThisEnumType MaskFromString(const std::string &enum_value) const {
		try {
			return(MaskFromStringChecked(enum_value));
		}
		catch (...) {
		}
		return(static_cast<ThisEnumType>(static_cast<unsigned int>(~enum_mask_)));
	}
	ThisEnumType MaskFromStringChecked(const std::string &enum_value) const {
		unsigned int ored_enum_value =
			static_cast<unsigned int>(enum_none_.enum_value_);
		try {
			StringVector flag_list;
			SplitString(enum_value, "|", flag_list);
			StringVector::size_type count_1;
			for (count_1 = 0; count_1 < flag_list.size(); ++count_1) {
				try {
					ThisEnumType tmp_value;
					if (permit_mask_flag_ && (!enum_mask_name_.empty()) &&
						(UpperCase(enum_mask_name_) == UpperCase(flag_list[count_1])))
						tmp_value = enum_mask_;
					else
						tmp_value = FromStringChecked(flag_list[count_1]);
					ored_enum_value |= static_cast<unsigned int>(tmp_value);
				}
				catch (const std::exception &except) {
					MLB::Utility::Rethrow(except, "Failed to process flag number " +
						AnyToString(count_1 + 1) + " of " +
						AnyToString(flag_list.size()) + " flags: " +
						std::string(except.what()));
				}
			}
		}
		catch (const std::exception &except) {
			MLB::Utility::Rethrow(except, "Unable to convert the list of " +
				enum_name_ + " flags in the string '" + enum_value + "': " +
				std::string(except.what()));
		}
		return(static_cast<ThisEnumType>(ored_enum_value));
	}
	ThisEnumType Parse(const std::string &in_string) const {
		ThisEnumType enum_value = enum_none_.enum_value_;
		if (in_string.empty())
			ThrowInvalidArgument("The " + enum_name_ + " is empty.");
		if (std::isdigit(in_string[0])) {
			unsigned int tmp_enum_value = 0;
			ParseCmdLineArg::ParseCmdLineNumeric(in_string, tmp_enum_value);
			enum_value = static_cast<ThisEnumType>(tmp_enum_value);
			CheckValid(enum_value);
		}
		else
			enum_value = FromStringChecked(in_string);
		return(enum_value);
	}
	ThisEnumType ParseMask(const std::string &in_string) const {
		ThisEnumType enum_value = enum_none_.enum_value_;
		if (in_string.empty())
			ThrowInvalidArgument("The " + enum_name_ + " is empty.");
		if (std::isdigit(in_string[0])) {
			unsigned int tmp_enum_value = 0;
			ParseCmdLineArg::ParseCmdLineNumeric(in_string, tmp_enum_value);
			enum_value = static_cast<ThisEnumType>(tmp_enum_value);
			CheckValidMask(enum_value);
		}
		else
			enum_value = MaskFromStringChecked(in_string);
		return(enum_value);
	}

	std::string                 enum_name_;
	MyEnumList                  enum_list_;
	EnumFlagDatum<ThisEnumType> enum_none_;
	ThisEnumType                enum_mask_;
	std::string                 enum_none_name_;
	std::string                 enum_mask_name_;
	bool                        permit_none_flag_;
	bool                        permit_mask_flag_;

private:
	void ThrowIt(ThisEnumType enum_value) const {
		ThrowInvalidArgument("Invalid " + enum_name_ + " encountered (" +
			ValueToStringBin(static_cast<int>(enum_value)) + " = " +
			AnyToString(static_cast<int>(enum_value)) + ").");
	}
	std::string GetInvalidString(ThisEnumType enum_value) const {
		return("***INVALID:[" + AnyToString(static_cast<int>(enum_value)) +
			"=" + ValueToStringBin(static_cast<int>(enum_value)) + "]***");
			
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#define MLB_UTILITY_ENUM_FLAG_DATUM(enum_type, enum_value, short_name)	\
	MLB::Utility::EnumFlagDatum<enum_type>(enum_value, #enum_value, short_name)
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__EnumFlags_hpp__HH

