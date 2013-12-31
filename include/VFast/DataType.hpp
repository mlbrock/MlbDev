// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	VFast Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for DataType enumeration support.

	Revision History	:	2008-11-15 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__VFast__DataType_hpp__HH

#define HH__MLB__VFast__DataType_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
/**
	\file DataType.hpp

	\brief	The VFast header file containing data type support.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <VFast/VFast.hpp>

#include <string>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace VFast {

// ////////////////////////////////////////////////////////////////////////////
/**
	\enum DataType The supported Fix Fast types data types.

	\todo Add support for new types Type_{U|S}Int{8|16}.
*/
enum DataType {
	/** Representation of an invalid or unassigned data type. */
	DataType_Invalid            = -1,
	/** ASCII string data type. */
	DataType_Ascii              =  0,
	/** Unicode string data type. */
	DataType_Unicode            =  1,
	/** Opaque vector of bytes data type. */
	DataType_ByteVector         =  2,
	/** Signed 32-bit integral data type. */
	DataType_SInt32             =  3,
	/** Unsigned 32-bit integral data type. */
	DataType_UInt32             =  4,
	/** Signed 64-bit integral data type. */
	DataType_SInt64             =  5,
	/** Unsigned 64-bit integral data type. */
	DataType_UInt64             =  6,
	/** The decimal data type. */
	DataType_Decimal            =  7,
	/** Data type of one or more sequences of fields. */
	DataType_Sequence           =  8,
	/** Data tyoe of a group of related fields. */
	DataType_Group              =  9,
	/** The template data type. */
	DataType_Template           = 10,
	/** The static template reference data type. */
	DataType_TemplateRefStatic  = 11,
	/** The dynamic template reference data type. */
	DataType_TemplateRefDynamic = 12,
	/** The minimum value of a \c DataType enumeration . */
	DataType_Min                = DataType_Ascii,
	/** The maximum value of a \c DataType enumeration . */
	DataType_Max                = DataType_TemplateRefDynamic,
	/** The total number of enumerations in \c DataType . */
	DataType_Count              = (DataType_Max - DataType_Min) + 1
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/*
	\brief Used to map machine representation types to their equivalent DataType
	enums.
*/
template <typename Type>
	struct TypeToTypeEnum {
	static const DataType value = DataType_Invalid;
};
template <> struct TypeToTypeEnum<Type_String> {
	static const DataType value = DataType_Ascii;
};
template <> struct TypeToTypeEnum<Type_SInt32> {
	static const DataType value = DataType_SInt32;
};
template <> struct TypeToTypeEnum<Type_UInt32> {
	static const DataType value = DataType_UInt32;
};
template <> struct TypeToTypeEnum<Type_SInt64> {
	static const DataType value = DataType_SInt64;
};
template <> struct TypeToTypeEnum<Type_UInt64> {
	static const DataType value = DataType_UInt64;
};
template <> struct TypeToTypeEnum<Type_Decimal> {
	static const DataType value = DataType_Decimal;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/*
	\brief Used to map DataType enums which have direct machine representation
	types to those types.
*/
template <DataType>
	struct TypeEnumToType {
};
template <> struct TypeEnumToType<DataType_Ascii> {
	typedef Type_String value_type;
};
template <> struct TypeEnumToType<DataType_Unicode> {
	typedef Type_String value_type;
};
template <> struct TypeEnumToType<DataType_ByteVector> {
	typedef Type_String value_type;
};
template <> struct TypeEnumToType<DataType_SInt32> {
	typedef Type_SInt32 value_type;
};
template <> struct TypeEnumToType<DataType_UInt32> {
	typedef Type_UInt32 value_type;
};
template <> struct TypeEnumToType<DataType_SInt64> {
	typedef Type_SInt64 value_type;
};
template <> struct TypeEnumToType<DataType_UInt64> {
	typedef Type_UInt64 value_type;
};
template <> struct TypeEnumToType<DataType_Decimal> {
	typedef Type_Decimal value_type;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	Determines if the specified type is integral.

	\param data_type is the type to be tested.

	\return \e true if \e data_type is an integral type, \e false otherwise.
	That is, \e true if \e data_type is one of:

	- #DataType_SInt32
	- #DataType_UInt32
	- #DataType_SInt64
	- #DataType_UInt64
*/
bool        IsTypeInt(DataType data_type);

/**
	Determines if the specified type is signed integral.

	\param data_type is the type to be tested.

	\return \e true if \e data_type is a signed integral type, \e false
	otherwise. That is, \e true if \e data_type is one of:

	- #DataType_SInt32
	- #DataType_SInt64
*/
bool        IsTypeSInt(DataType data_type);

/**
	Determines if the specified type is unsigned integral.

	\param data_type is the type to be tested.

	\return \e true if \e data_type is an unsigned integral type, \e false
	otherwise. That is, \e true if \e data_type is one of:

	- #DataType_UInt32
	- #DataType_UInt64
*/
bool        IsTypeUInt(DataType data_type);

/**
	Determines if the specified type is an 32-bit integral.

	\param data_type is the type to be tested.

	\return \e true if \e data_type is an 32-bit integral type, \e false
	otherwise. That is, \e true if \e data_type is one of:

	- #DataType_SInt32
	- #DataType_UInt32
*/
bool        IsTypeInt32(DataType data_type);

/**
	Determines if the specified type is an 64-bit integral.

	\param data_type is the type to be tested.

	\return \e true if \e data_type is an 64-bit integral type, \e false
	otherwise. That is, \e true if \e data_type is one of:

	- #DataType_SInt64
	- #DataType_UInt64
*/
bool        IsTypeInt64(DataType data_type);

/**
	Determines if the specified type is a numeric type.

	\param data_type is the type to be tested.

	\return \e true if \e data_type is a numeric type, \e false
	otherwise. That is, \e true if \e data_type is one of:

	- #DataType_SInt32
	- #DataType_UInt32
	- #DataType_SInt64
	- #DataType_UInt64
	- #DataType_Decimal
*/
bool        IsTypeNumeric(DataType data_type);

/**
	Determines if the specified type is a character array type.

	\param data_type is the type to be tested.

	\return \e true if \e data_type is a character array type, \e false
	otherwise. That is, \e true if \e data_type is one of:

	- #DataType_Ascii
	- #DataType_Unicode
	- #DataType_ByteVector
*/
bool        IsTypeCharArray(DataType data_type);

/**
	Determines if the specified type is a primitive non-composite type.

	\param data_type is the type to be tested.

	\return \e true if \e data_type is a primitive non-composite type, \e false
	otherwise. That is, \e true if \e data_type is one of:

	- #DataType_Ascii
	- #DataType_Unicode
	- #DataType_ByteVector
	- #DataType_SInt32
	- #DataType_UInt32
	- #DataType_SInt64
	- #DataType_UInt64
	- #DataType_Decimal
*/
bool        IsTypePrimitive(DataType data_type);

/**
	Determines if the specified type is a composite type.

	\param data_type is the type to be tested.

	\return \e true if \e data_type is a composite type, \e false
	otherwise. That is, \e true if \e data_type is one of:

	- #DataType_Decimal
	- #DataType_Sequence
	- #DataType_Group
	- #DataType_Template
*/
bool        IsTypeComposite(DataType data_type);

/**
	Determines if the specified type is valid.

	\param data_type is the type to be tested.

	\return \e true if \e data_type is within the valid domain of the
	\c DataType enumeration, otherwise \e false .
*/
bool        DataTypeIsValid(DataType data_type);

/**
	Determines if the specified type is valid. Throws a \c std::invalid_argument
	exception if it is not.

	\param data_type is the type to be tested.

	\return The value of the \e data_type parameter if it is valid.
*/
DataType    CheckDataType(DataType data_type);

/**
	Determines if the specified string is the name of a value in the valid
	domain of the \c DataType enumeration.

	\param in_data_type is the string to be tested.

	\param out_data_type is the reference to a \c DataType . If the \e data_type
	parameter is valid, this function will place the enumeration value
	equivalent of \e in_data_type into the area to which \e out_data_type refers.

	\return \e true if the \e in_data_type string represents a valid \c DataType
	enumeration, otherwise \e false .
*/
bool        IsDataTypeString(const std::string &in_data_type,
	DataType &out_data_type);

/**
	Determines if the specified string is the name of a value in the valid
	domain of the \c DataType enumeration.

	\param in_data_type is the string to be tested.

	\return \e true if the \e in_data_type string represents a valid \c DataType
	enumeration, otherwise \e false .
*/
bool        IsDataTypeString(const std::string &in_data_type);

/**
	Maps the specified \c DataType to an equivalent \c std::string .

	\param data_type is the type to be converted.

	\return A \c std::string equivalent of the \e data_type parameter. If the
	\e data_type was not a valid \c DataType value, the hexadecimal string
	of that value is returned instead.
*/
std::string DataTypeToString(DataType data_type);

/**
	Maps the specified \c std::string to an equivalent \c DataType value.

	\param data_type is the string to be converted.

	\return If the \e data_type string was valid, the \c DataType equivalent of
	that string is returned. Otherwise, a std::invalid_argument exception is
	thrown.
*/
DataType    StringToDataType(const std::string &data_type);

/**
	Converts a datum to its \c std::string equivalent.

	\param data_type is the \c DataType of the datum to be converted. Note that
	if \e data_type is not valid, or if it is not a primitive type, the string
	"*NON-PRIMITIVE-TYPE*" will be returned by this function.

	\param datum_length is the length of the datum to be converted. Note that
	the \e datum_length parameter is used only when \e data_type is one of:

	- #DataType_Ascii
	- #DataType_Unicode
	- #DataType_ByteVector

	\param datum_ptr points to the datum to be converted.

	\return The \c std::string resulting from the conversion.
*/
std::string DatumToString(DataType data_type, unsigned int datum_length,
	const void *datum_ptr);

/**
	The \c std::ostream operator for instances of type \c DataType .
*/
std::ostream & operator << (std::ostream &o_str, const DataType &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace VFast

} // namespace MLB

#endif // #ifdef HH__MLB__VFast__DataType_hpp__HH

