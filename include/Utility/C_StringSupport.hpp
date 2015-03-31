// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File for C Language Support
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for utility library.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__C_StringSupport_hpp__HH

#define HH__MLB__C_StringSupport_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file 	C_StringSupport.hpp

	\brief	The MLB Utility C library interface header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/ValueTraits.hpp>
#include <Utility/Utility_Exception.hpp>

#ifdef __GNUC__
# include <memory.h>
# include <stdlib.h>
#endif // #ifdef __GNUC__

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Defined in 'C_String.cpp'...
//	////////////////////////////////////////////////////////////////////////////
API_UTILITY char *C_strcat(char *dst_string, const char *src_string,
	const char *name_string = NULL);
API_UTILITY char *C_strcpy(char *dst_string, const char *src_string,
	const char *name_string = NULL);
API_UTILITY char *C_strdup(const char *src_string,
	const char *name_string = NULL);
API_UTILITY int   C_stricmp(char *ptr_1, const char *ptr_2,
	const char *name_string = NULL);
API_UTILITY int   Utility_stricmp(const char *ptr_1, const char *ptr_2);
API_UTILITY int   Utility_strnicmp(const char *ptr_1, const char *ptr_2,
	size_t data_length);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Defined in 'C_StdLib.cpp'...
//	////////////////////////////////////////////////////////////////////////////
API_UTILITY void *C_calloc(size_t number_of_elements, size_t size_of_element,
	const char *name_string = NULL);
API_UTILITY void  C_free(void *mem_ptr, const char *name_string = NULL);
API_UTILITY void *C_malloc(size_t alloc_size, const char *name_string = NULL);
API_UTILITY void *C_realloc(void *mem_ptr, size_t alloc_size,
	const char *name_string = NULL);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Defined in C_Memory.cpp...
//	////////////////////////////////////////////////////////////////////////////
API_UTILITY const void *C_memchr(const void *buffer_ptr, int src_char,
	size_t data_length, const char *name_string = NULL);
API_UTILITY int         C_memcmp(const void *ptr_1, const void *ptr_2,
	size_t data_length, const char *name_string = NULL);
API_UTILITY void       *C_memcpy(void *dst_ptr, const void *src_ptr,
	size_t data_length, const char *name_string = NULL);
API_UTILITY void       *C_memmove(void *dst_ptr, const void *src_ptr,
	size_t data_length, const char *name_string = NULL);
API_UTILITY void       *C_memset(void *dst_ptr, int src_char,
	size_t data_length, const char *name_string = NULL);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Defined in C_MemoryX.cpp...
//	////////////////////////////////////////////////////////////////////////////
API_UTILITY void *C_memccpy(void *dst_ptr, const void *src_ptr, int src_char,
	size_t data_length, const char *name_string = NULL);
API_UTILITY void *C_memdup(const void *src_ptr, size_t src_length,
	const char *name_string = NULL);
API_UTILITY int   C_memicmp(const void *ptr_1, const void *ptr_2,
	size_t data_length, const char *name_string = NULL);
API_UTILITY int   Utility_memicmp(const void *ptr_1, const void *ptr_2,
	size_t data_length);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Defined in trim.cpp...
//	////////////////////////////////////////////////////////////////////////////
char *ltrim(char *in_text);
char *rtrim(char *in_text);
char *trim(char *in_text);
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_UTILITY bool IsNull(const char *test_string)
{
	return(test_string == NULL);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_UTILITY bool IsEmpty(const char *test_string)
{
	return((test_string == NULL) || (!(*test_string)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_UTILITY const char *NullToString(const char *test_string,
	const char *other_string = "")
{
	return((test_string != NULL) ? test_string :
		((other_string != NULL) ? other_string : ""));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_UTILITY const char *NullOrEmptyToString(const char *test_string,
	const char *other_string = "")
{
	return(((test_string != NULL) && *test_string) ? test_string :
		((other_string != NULL) ? other_string : ""));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_UTILITY std::string NullOrEmptyToString(
	const std::string &test_string, const std::string &other_string = "")
{
	return(NullOrEmptyToString(test_string.c_str(), other_string.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_UTILITY std::string NullOrEmptyToStringWithColon(
	const std::string &test_string, const std::string &other_string = "")
{
	const char *tmp_ptr = NullOrEmptyToString(test_string.c_str(),
		other_string.c_str());

	return((*tmp_ptr) ? (std::string(tmp_ptr) + ": ") : "");
}
//	////////////////////////////////////////////////////////////////////////////

namespace {
//	////////////////////////////////////////////////////////////////////////////
inline API_UTILITY void ThrowIfSupport(const char *name_string,
	const char *error_string)
{
	if ((name_string == NULL) || (!(*name_string)))
		name_string = "Value";

	MLB::Utility::ThrowInvalidArgument(std::string(name_string) +
		std::string(error_string));
}
//	////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
inline API_UTILITY const char *ThrowIfNull(const char *test_string,
	const char *name_string = NULL)
{
	if (test_string == NULL)
		ThrowIfSupport(name_string, " is a 'NULL' pointer.");

	return(test_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_UTILITY const char *ThrowIfEmpty(const char *test_string,
	const char *name_string = NULL)
{
	if (!(*test_string))
		ThrowIfSupport(name_string, " is an empty string.");

	return(test_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_UTILITY const std::string &ThrowIfEmpty(const std::string &test_string,
	const char *name_string = NULL)
{
	ThrowIfEmpty(test_string.c_str(), name_string);

	return(test_string);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
inline API_UTILITY const char *ThrowIfNullOrEmpty(const char *test_string,
	const char *name_string = NULL)
{
	return(ThrowIfEmpty(ThrowIfNull(test_string, name_string), name_string));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename CountType> void CheckCountTypeAndValue(CountType in_count,
	const char *name_string = NULL)
{
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(disable:4127)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

	if (!std::numeric_limits<CountType>::is_integer) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"The count type specified is not an integral type.";
		ThrowInvalidArgument(error_text.str().c_str());
	}

	if (IsValueNegative(in_count)) {
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"The count value specified (" << static_cast<int>(in_count) <<
			") is negative.";
		ThrowInvalidArgument(error_text.str().c_str());
	}

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(default:4127)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename CountType, typename ListType>
	void CheckCongruenceCountAndList(CountType in_count, ListType in_list,
	const char *name_string = NULL)
{
	CheckCountTypeAndValue(in_count, name_string);

	if ((in_count && (in_list == NULL)) || ((!(in_count)) && (in_list != NULL))){
		std::ostringstream error_text;
		error_text << NullOrEmptyToStringWithColon(name_string, "") <<
			"The count indicates the presence of " << in_count <<
			" elements in the list, but the list pointer is " <<
			((in_list == NULL) ? "" : "not ") << "'NULL'.";
		ThrowInvalidArgument(error_text);
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ListType, typename CountType = unsigned int>
	class C_ListType {
public:
	C_ListType() :
		the_count_(0),
		the_list_(NULL) {
	}
	virtual ~C_ListType() {
		Free();
	}

	C_ListType(const C_ListType &other) :
		the_count_(0), the_list_(NULL) {
		CountType count_1;
		for (count_1 = 0; count_1 < other.the_count_; ++count_1)
			this->Append(other.the_list_[count_1]);
	}
	C_ListType & operator = (const C_ListType &other) {
		if (this != &other) {
			C_ListType tmp_list(other);
			Swap(tmp_list);
		}
		return(*this);
	}

	virtual void Swap(C_ListType &other) {
		std::swap(the_count_, other.the_count_);
		std::swap(the_list_, other.the_list_);
	}

	//	Allocate a list. Frees any previously allocated memory.
	virtual ListType *Calloc(CountType in_count,
		const char *name_string = NULL) {
		MLB::Utility::CheckCountTypeAndValue(in_count, name_string);
		if (!in_count)
			Free();
		else {
			ListType *tmp_list;
			tmp_list = reinterpret_cast<ListType *>(
				C_calloc(in_count, sizeof(ListType), name_string));
			the_count_ = in_count;
			the_list_  = tmp_list;
		}
		return(the_list_);
	}

	//	Allocate a list (malloc analogue). Cheapo implementation.
	virtual ListType *Malloc(CountType in_count,
		const char *name_string = NULL) {
		return(Calloc(in_count, name_string));
	}

	// Frees a list.
	virtual void Free() {
		if (the_list_ != NULL)
			free(the_list_);
		Reset();
	}

	//	Re-allocates a list. Count may be zero.
	virtual ListType *Realloc(CountType in_count,
		const char *name_string = NULL) {
		MLB::Utility::CheckCountTypeAndValue(in_count, name_string);
		if (!in_count)
			Free();
		else if (!the_count_)
			Calloc(in_count, name_string);
		else if (in_count > the_count_) {
			ListType *tmp_list;
			tmp_list = reinterpret_cast<ListType *>(
				C_realloc(the_list_, in_count * sizeof(ListType),
				name_string));
			if (in_count > the_count_)
				memset(reinterpret_cast<char *>(tmp_list) +
					(sizeof(ListType) * the_count_), '\0',
					(in_count - the_count_) * sizeof(ListType));
			the_count_ = in_count;
			the_list_  = tmp_list;
		}
		return(the_list_);
	}

	//	Clears without de-allocating. Assumes you've gotten a copy...
	virtual void Reset() {
		the_count_ = 0;
		the_list_  = NULL;
	}

	virtual void Append(const ListType &in_datum,
		const char *name_string = NULL) {
		Realloc(the_count_ + 1, name_string);
		the_list_[the_count_ - 1] = in_datum;
	}
	virtual void AppendEmpty(const char *name_string = NULL) {
		Realloc(the_count_ + 1, name_string);
	}


	virtual CountType  GetCount() const {
		return(the_count_);
	}
	virtual ListType  *GetList() const {
		return(the_list_);
	}

	virtual void SetCount(CountType the_count) {
		the_count_ = the_count;
	}
	virtual void SetList(const ListType *the_list) {
		the_list_ = const_cast<ListType *>(the_list);
	}

	template <typename ContainerType>
		void FromContainer(const ContainerType &in_container,
		const char *name_string = NULL) {
		typename ContainerType::const_iterator iter_b(in_container.begin());
		typename ContainerType::const_iterator iter_e(in_container.end());
		while (iter_b != iter_e) {
			Append(*iter_b, name_string);
			++iter_b;
		}
	}

	template <typename ContainerType>
		void ToContainer(ContainerType &out_container,
		const char *name_string = NULL) const {
		try {
			CountType     tmp_count = 0;
			ContainerType tmp_container;
			while (tmp_count < the_count_)
				tmp_container.push_back(the_list_[tmp_count++]);
			std::swap(out_container, tmp_container);
		}
		catch (const std::exception &except) {
			std::ostringstream error_text;
			error_text <<
				MLB::Utility::NullOrEmptyToStringWithColon(name_string, "") <<
				except.what();
			MLB::Utility::Rethrow(except, error_text);
		}
	}

	CountType  the_count_;
	ListType  *the_list_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename CountType = unsigned int> class C_ListTypeString {
public:
	C_ListTypeString() :
		the_count_(0),
		the_list_(NULL) {
	}
	~C_ListTypeString() {
		Free();
	}

	C_ListTypeString(const C_ListTypeString &other) :
		the_count_(0), the_list_(NULL) {
		CountType count_1;
		for (count_1 = 0; count_1 < other.the_count_; ++count_1)
			this->Append(other.the_list_[count_1]);
	}
	C_ListTypeString & operator = (const C_ListTypeString &other) {
		C_ListTypeString tmp_list(other);
		Swap(tmp_list);
		return(*this);
	}

	void Swap(C_ListTypeString &other) {
		std::swap(the_count_, other.the_count_);
		std::swap(the_list_, other.the_list_);
	}

	//	Allocate a list. Frees any previously allocated memory.
	char **Calloc(CountType in_count, const char *name_string = NULL) {
		MLB::Utility::CheckCountTypeAndValue(in_count, name_string);
		if (!in_count)
			Free();
		else {
			char **tmp_list;
			tmp_list = reinterpret_cast<char **>(
				C_calloc(static_cast<std::size_t>(in_count), sizeof(char *),
				name_string));
			the_count_ = in_count;
			the_list_  = tmp_list;
		}
		return(the_list_);
	}

	//	Allocate a list (malloc analogue). Cheapo implementation.
	char **Malloc(CountType in_count,
		const char *name_string = NULL) {
		return(Calloc(in_count, name_string));
	}

	// Frees a list.
	void Free() {
		if (the_list_ != NULL) {
			while (the_count_ > 0) {
				if (the_list_[--the_count_] != NULL)
					free(the_list_[the_count_]);
			}
			::free(the_list_);
		}
		Reset();
	}

	//	Re-allocates a list. Count may be zero.
	char **Realloc(CountType in_count,
		const char *name_string = NULL) {
		MLB::Utility::CheckCountTypeAndValue(in_count, name_string);
		if (!in_count)
			Free();
		else if (!the_count_)
			Calloc(in_count, name_string);
		else if (in_count > the_count_) {
			char **tmp_list;
			tmp_list = reinterpret_cast<char **>(
				C_realloc(the_list_, in_count * sizeof(char *),
				name_string));
			if (in_count > the_count_)
				::memset(reinterpret_cast<char *>(tmp_list) +
					(sizeof(char *) * the_count_), '\0',
					(in_count - the_count_) * sizeof(char *));
			the_count_ = in_count;
			the_list_  = tmp_list;
		}
		return(the_list_);
	}

	//	Clears without de-allocating. Assumes you've gotten a copy...
	 void Reset() {
		the_count_ = 0;
		the_list_  = NULL;
	}

	void Append(const char *in_datum, const char *name_string = NULL) {
		if (in_datum != NULL) {
			Realloc(the_count_ + 1, name_string);
			the_list_[the_count_ - 1] = C_strdup(in_datum, name_string);
		}
		else
			AppendEmpty(name_string);
	}
	void AppendEmpty(const char *name_string = NULL) {
		Realloc(the_count_ + 1, name_string);
		the_list_[the_count_ - 1] = NULL;
	}

	CountType  GetCount() const {
		return(the_count_);
	}
	char **GetList() const {
		return(the_list_);
	}

	void SetCount(CountType the_count) {
		the_count_ = the_count;
	}
	void SetList(const char **the_list) {
		the_list_ = const_cast<char **>(the_list);
	}
	void SetList(char **the_list) {
		the_list_ = the_list;
	}

	template <typename ContainerType>
		void FromContainer(const ContainerType &in_container,
		const char *name_string = NULL) {
		typename ContainerType::const_iterator iter_b(in_container.begin());
		typename ContainerType::const_iterator iter_e(in_container.end());
		while (iter_b != iter_e) {
			Append(iter_b->c_str(), name_string);
			++iter_b;
		}
	}

	template <typename ContainerType>
		void ToContainer(ContainerType &out_container,
		const char *name_string = NULL) const {
		try {
			CountType     tmp_count = 0;
			ContainerType tmp_container;
			while (tmp_count < the_count_)
				tmp_container.push_back(std::string(the_list_[tmp_count++]));
			std::swap(out_container, tmp_container);
		}
		catch (const std::exception &except) {
			std::ostringstream error_text;
			error_text <<
				MLB::Utility::NullOrEmptyToStringWithColon(name_string, "") <<
				except.what();
			MLB::Utility::Rethrow(except, error_text);
		}
	}

	CountType   the_count_;
	char      **the_list_;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	ContainerType is assumed to contain type std::string.
template <typename ContainerType>
	void StringContainerToCharPtrArray(const ContainerType &container,
	unsigned int &out_count, char **&out_list, const char *name_string = NULL)
{
	out_count = 0;
	out_list  = NULL;

	if (!container.empty()) {
		C_ListTypeString<unsigned int> tmp_list;
		tmp_list.FromContainer(container, name_string);
		out_count = tmp_list.GetCount();
		out_list  = tmp_list.GetList();
		//	Mustn't try to free when we leave this scope...
		tmp_list.Reset();
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	Container type must be one of the non-associative containers with push_back()
template <typename ContainerType>
	void CharPtrArrayToStringContainer(unsigned int in_count,
	char **in_list, ContainerType &container, const char *name_string = NULL)
{
	container.clear();

	C_ListTypeString<unsigned int> tmp_list;
	//	Force it to look like a 'C_ListTypeString'...
	tmp_list.SetCount(in_count);
	tmp_list.SetList(in_list);
	//	Put the list into the container...
	tmp_list.ToContainer(container, name_string);
	//	Mustn't try to free when we leave this scope...
	tmp_list.Reset();
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__C_StringSupport_hpp__HH

