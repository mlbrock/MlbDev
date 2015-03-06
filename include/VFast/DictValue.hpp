// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Definition of the DictValue class.

	Revision History	:	2008-11-18 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__DictValue_hpp__HH

#define HH__MLB__VFast__DictValue_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/*
	\file DictValue.hpp

	\brief	The container type for initial and dictionary values.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/DataType.hpp>

#include <Utility/ParseCmdLineArg.hpp>

#include <vector>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
/**
	The container type for initial and dictionary values.

	\note For reasons of performance, this structure \e must contain POD types
	(and aggregates of POD types) only. Also, virtual functions are not
	permitted.
*/
struct DictValue {
	/**
		\enum DictValueFlag Dictionary value content flags.
	*/
	enum DictValueFlag {
		/** Indicates no flag is set on the dictionary value. */
		DictValueFlag_None     = 0x00000000,
		/** Indicates that a dictionary value is \e NULL . */
		DictValueFlag_IsNull   = 0x00000001,
		/** Indicates that a dictionary value is absent. */
		DictValueFlag_IsAbsent = 0x00000002,
		/** The minimum value of a \c DictValueFlag enumeration . */
		DictValueFlag_Min      = DictValueFlag_None,
		/** The maximum value of a \c DictValueFlag enumeration . */
		DictValueFlag_Max      = DictValueFlag_IsAbsent,
		/** The total number of enumeration values in \c DictValueFlag . */
		DictValueFlag_Count    = (DictValueFlag_Max - DictValueFlag_Min) + 1
	};

	/**
		\enum DictValueState Dictionary value state flags.
	*/
	enum DictValueState {
		/** The initial state for a dictionary entry. */
		DictValueState_Undefined = 0,
		/** Indicates for optional fields that no dictionary value exists. */
		DictValueState_Empty     = 1,
		/** Indicates that a value is present for the dictionary entry. */
		DictValueState_Assigned  = 2,
		/** The minimum value of a \c DictValueState enumeration . */
		DictValueState_Min       = DictValueState_Undefined,
		/** The maximum value of a \c DictValueState enumeration . */
		DictValueState_Max       = DictValueState_Assigned,
		/** The total number of enumeration values in \c DictValueState . */
		DictValueState_Count     = (DictValueState_Max - DictValueState_Min) + 1
	};

	explicit DictValue(DataType data_type = DataType_Invalid,
		bool is_null = true);
	explicit DictValue(const char *datum, DataType data_type = DataType_Ascii);
	         DictValue(unsigned int datum_length, const char *datum,
		DataType data_type = DataType_Ascii);
	explicit DictValue(const std::string &datum,
		DataType data_type = DataType_Ascii);
	         DictValue(Type_SInt32 datum);
	         DictValue(Type_UInt32 datum);
	         DictValue(Type_SInt64 datum);
	         DictValue(Type_UInt64 datum);

	bool operator < (const DictValue &other) const;

	/**
		\return True if the \c DictValue is NULL.
	*/
	inline bool IsNull() const
	{
		return((data_flag_ & DictValueFlag_IsNull) != 0);
	}

	/**
		\return True if the \c DictValue is an absent as defined by the FAST
		protocol.
	*/
	inline bool IsAbsent() const
	{
		return((data_flag_ & DictValueFlag_IsAbsent) != 0);
	}

	/**
		Sets the \c DictValue to NULL.
	*/
	inline void SetNull()
	{
		data_flag_ =
			static_cast<DictValueFlag>(data_flag_ | DictValueFlag_IsNull);
	}

	/**
		Sets the \c DictValue to absent as defined by the FAST protocol.
	*/
	inline void SetAbsent()
	{
		data_flag_ =
			static_cast<DictValueFlag>(data_flag_ | DictValueFlag_IsAbsent);
	}

	/**
		Clears the \c DictValue NULL flag.
	*/
	inline void ClearNull()
	{
		data_flag_ =
			static_cast<DictValueFlag>(data_flag_ & ~DictValueFlag_IsNull);
	}

	/**
		Clears the \c DictValue absent flag.
	*/
	inline void ClearAbsent()
	{
		data_flag_ =
			static_cast<DictValueFlag>(data_flag_ & ~DictValueFlag_IsAbsent);
	}

	/**
		\return True if the \c DictValue is \e undefined as specified by the FAST
		protocol.
	*/
	bool IsUndefined() const
	{
		return(data_state_ == DictValueState_Undefined);
	}

	/**
		\return True if the \c DictValue is \e empty as specified by the FAST
		protocol.
	*/
	bool IsEmpty() const
	{
		return(data_state_ == DictValueState_Empty);
	}

	/**
		\return True if the \c DictValue is \e assigned as specified by the FAST
		protocol.
	*/
	bool IsAssigned() const
	{
		return(data_state_ == DictValueState_Assigned);
	}

	/**
		\return Sets the \c DictValue to \e undefined as specified by the FAST
		protocol.
	*/
	void SetIsUndefined()
	{
		data_state_ = DictValueState_Undefined;
	}

	/**
		\return Sets the \c DictValue to \e empty as specified by the FAST
		protocol.
	*/
	void SetIsEmpty()
	{
		data_state_ = DictValueState_Empty;
	}

	/**
		\return Sets the \c DictValue to \e assigned as specified by the FAST
		protocol.
	*/
	void SetIsAssigned()
	{
		data_state_ = DictValueState_Assigned;
	}

	bool IsTypeInt() const;
	bool IsTypeSInt() const;
	bool IsTypeUInt() const;
	bool IsTypeInt32() const;
	bool IsTypeInt64() const;
	bool IsTypeNumeric() const;
	bool IsTypeCharArray() const;
	bool IsTypePrimitive() const;

	bool IsTypeComposite() const;

	void ClearValue();

	void SetValueFromString(const std::string &string_value,
		DataType data_type = DataType_Invalid);

	template <typename ThisType> void GetTypeValue(ThisType &) const;

	void SetValue_Primitive(const DictValue &value_src);
	void SetValue_String(unsigned int string_length, const char *string_ptr);
	void SetValue_String(const std::string &in_datum);
	void SetValue(const char *datum);
	void SetValue(unsigned int datum_length, const char *datum_ptr);
	void SetValue(const std::string &datum);
	void SetValue(Type_SInt32 datum);
	void SetValue(Type_UInt32 datum);
	void SetValue(Type_SInt64 datum);
	void SetValue(Type_UInt64 datum);
	void SetValue(Type_Decimal datum);

	unsigned int GetRawDatumSize() const;

	const void *GetPtrToDatumRaw() const;
	      void *GetPtrToDatumRaw();

	template <typename BasicType>
		const BasicType *GetDatumPtr() const
	{
		return(string_);
	}
	template <typename BasicType>
		BasicType *GetDatumPtr()
	{
		return(string_);
	}

	int  Compare(const DictValue &other) const;
	bool IsEquivalent(const DictValue &other) const;
	bool CheckIsEquivalent(const DictValue &other) const;

	void IncrementInteger();

	DataType CheckDataType(DataType data_type) const;

	std::string ValueToString() const;
	std::string ValueToStringWidth(unsigned int width) const;

	Type_Decimal   decimal_;
	Type_SInt64    sint_64_;
	Type_UInt64    uint_64_;
	Type_SInt32    sint_32_;
	Type_UInt32    uint_32_;
	DataType       data_type_;
	DictValueFlag  data_flag_;
	DictValueState data_state_;
	unsigned int   string_length_;
	Type_String    string_[VFastMaxStringLength + 1];

	static DataType CheckStringCtorType(DataType data_type);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <>
	inline const Type_SInt32 *DictValue::GetDatumPtr<Type_SInt32>() const
{
	return(&sint_32_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <>
	inline const Type_UInt32 *DictValue::GetDatumPtr<Type_UInt32>() const
{
	return(&uint_32_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <>
	inline const Type_SInt64 *DictValue::GetDatumPtr<Type_SInt64>() const
{
	return(&sint_64_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <>
	inline const Type_UInt64 *DictValue::GetDatumPtr<Type_UInt64>() const
{
	return(&uint_64_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <>
	inline const Type_Decimal *DictValue::GetDatumPtr<Type_Decimal>() const
{
	return(&decimal_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <>
	inline Type_SInt32 *DictValue::GetDatumPtr<Type_SInt32>()
{
	return(const_cast<Type_SInt32 *>(const_cast<const DictValue * const>(
		this)->GetDatumPtr<Type_SInt32>()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <>
	inline Type_UInt32 *DictValue::GetDatumPtr<Type_UInt32>()
{
	return(const_cast<Type_UInt32 *>(const_cast<const DictValue * const>(
		this)->GetDatumPtr<Type_UInt32>()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <>
	inline Type_SInt64 *DictValue::GetDatumPtr<Type_SInt64>()
{
	return(const_cast<Type_SInt64 *>(const_cast<const DictValue * const>(
		this)->GetDatumPtr<Type_SInt64>()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <>
	inline Type_UInt64 *DictValue::GetDatumPtr<Type_UInt64>()
{
	return(const_cast<Type_UInt64 *>(const_cast<const DictValue * const>(
		this)->GetDatumPtr<Type_UInt64>()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <>
	inline Type_Decimal *DictValue::GetDatumPtr<Type_Decimal>()
{
	return(const_cast<Type_Decimal *>(const_cast<const DictValue * const>(
		this)->GetDatumPtr<Type_Decimal>()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <> inline void DictValue::GetTypeValue(Type_String &out_datum) const
{
	::memcpy(&out_datum, string_, string_length_);

	(&out_datum)[string_length_] = '\0';
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <> inline void DictValue::GetTypeValue(Type_SInt32 &out_datum) const
{
	out_datum = sint_32_;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <> inline void DictValue::GetTypeValue(Type_UInt32 &out_datum) const
{
	out_datum = uint_32_;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <> inline void DictValue::GetTypeValue(Type_SInt64 &out_datum) const
{
	out_datum = sint_64_;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <> inline void DictValue::GetTypeValue(Type_UInt64 &out_datum) const
{
	out_datum = uint_64_;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <> inline void DictValue::GetTypeValue(Type_Decimal &out_datum) const
{
	out_datum = decimal_;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<DictValue>        DictValueList;
typedef DictValueList::iterator       DictValueListIter;
typedef DictValueList::const_iterator DictValueListIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const DictValue &datum);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/*
	\brief Used to map types to pointers to the member data used to stores
	values of that type.
*/
template <typename Type> struct TypeToTypeMember {
};
template <> struct TypeToTypeMember<Type_SInt32> {
	Type_SInt32 DictValue::*GetMemberPtr() { return(&DictValue::sint_32_); }
};
template <> struct TypeToTypeMember<Type_UInt32> {
	Type_UInt32 DictValue::*GetMemberPtr() { return(&DictValue::uint_32_); }
};
template <> struct TypeToTypeMember<Type_SInt64> {
	Type_SInt64 DictValue::*GetMemberPtr() { return(&DictValue::sint_64_); }
};
template <> struct TypeToTypeMember<Type_UInt64> {
	Type_UInt64 DictValue::*GetMemberPtr() { return(&DictValue::uint_64_); }
};
template <> struct TypeToTypeMember<Type_Decimal> {
	Type_Decimal DictValue::*GetMemberPtr() { return(&DictValue::decimal_); }
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifndef HH__MLB__VFast__DictValue_hpp__HH

