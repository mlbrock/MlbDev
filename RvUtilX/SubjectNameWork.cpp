// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the SubjectNameWork class.

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

#include <RvUtilX.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace {	// An anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Helper function for SubjectNameWork::SetSubjectName().
// ////////////////////////////////////////////////////////////////////////////
void CheckElementLength(unsigned int element_count, unsigned int element_length,
	const char *subject_name)
{
	if (!element_length)
		throw MLB::RvUtilX::RvException("The length of element number " +
			MLB::Utility::AnyToString(element_count) + " in subject name '" +
			std::string(subject_name) +
			"' is zero --- empty elements are not valid.");
	else if (element_length > MLB::RvUtilX::MaxElementSize)
		throw MLB::RvUtilX::RvException("The length of element number " +
			MLB::Utility::AnyToString(element_count) + " in subject name '" +
			std::string(subject_name) + "' has a length (" +
			MLB::Utility::AnyToString(element_length) +
			" which exceeds the maximum Rendezvous subject element length (" +
			MLB::Utility::AnyToString(MLB::RvUtilX::MaxElementSize) + ").");
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

namespace MLB {

namespace RvUtilX {

// ////////////////////////////////////////////////////////////////////////////
SubjectNameWork::SubjectNameWork() :
	 element_count_(0)
	,is_pattern_(false)
{
	subject_name_[0] = '\0';
	memset(element_list_, '\0', sizeof(element_list_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SubjectNameWork::SubjectNameWork(const std::string &subject_name) :
	 element_count_(0)
	,is_pattern_(false)
{
	SetSubjectName(subject_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SubjectNameWork::SubjectNameWork(const char *subject_name) :
	 element_count_(0)
	,is_pattern_(false)
{
	SetSubjectName(subject_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SubjectNameWork::SubjectNameWork(const SubjectNameWork &other) :
	 element_count_(0)
	,is_pattern_(false)
{
	SetSubjectName(other.ToString());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
SubjectNameWork & SubjectNameWork::operator = (const SubjectNameWork &other)
{
	SubjectNameWork tmp_datum(other);

	swap(tmp_datum);

	return(*this);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	A faster swap would calculate the pointer differences into temporary arrays,
//	but the 'SubjectNameWork' class is used only for creating instances of the
//	class 'SubjectNameFlat'...
void SubjectNameWork::swap(SubjectNameWork &other)
{
	char tmp_subject_name_1[sizeof(subject_name_)];
	ToString(tmp_subject_name_1);

	char tmp_subject_name_2[sizeof(subject_name_)];
	SetSubjectName(other.ToString(tmp_subject_name_2));

	other.SetSubjectName(tmp_subject_name_1);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SubjectNameWork::SetSubjectName(const char *subject_name)
{
	memset(subject_name_, '\0', sizeof(subject_name_));
	memset(element_list_, '\0', sizeof(element_list_));

	element_count_ = 0;
	is_pattern_    = false;

	if (subject_name == NULL)
		throw RvException("SubjectNameWork: subject name parameter is 'NULL'.");

	if (!(*subject_name))
		throw RvException("SubjectNameWork: subject name parameter is empty "
			"(has zero length).");

	if (strlen(subject_name) > MaxSubjectSize)
		throw RvException("Subject name '" + std::string(subject_name) +
			"' length (" + MLB::Utility::AnyToString(strlen(subject_name)) +
			") exceeds the maximum permissible length (" +
			MLB::Utility::AnyToString(MaxSubjectSize) + ").");

	strcpy(subject_name_, subject_name);

	char  *tmp_ptr     = subject_name_;
	char **element_ptr = element_list_;

	*element_ptr   = subject_name_;
	element_count_ = 1;

	//	////////////////////////////////////////////////////////////////
	//	Parse the elements...
	while (*tmp_ptr) {
		if (element_count_ == MaxElementCount)
			throw RvException("Subject name '" + std::string(subject_name) +
				"' contains more than the maximum permissible number of " +
				"elements (" + MLB::Utility::AnyToString(MaxElementCount) +  ").");
		if (*tmp_ptr == ElementSeparatorChar) {
			*tmp_ptr = '\0';
			CheckElementLength(element_count_,
				static_cast<unsigned int>(tmp_ptr - *element_ptr), subject_name);
			++element_count_;
			*(++element_ptr) = tmp_ptr + 1;
		}
		tmp_ptr++;
	}

	CheckElementLength(element_count_,
		static_cast<unsigned int>(strlen(*element_ptr)), subject_name);

	//	////////////////////////////////////////////////////////////////
	// Check each element to ensure that any Rendezvous meta-characters
	//	are appropriate in context...
	unsigned int count_1;
	for (count_1 = 0; count_1 < element_count_; ++count_1) {
		//	Single asterisk ('*') is OK.
		if (!strcmp(element_list_[count_1], ElementAnyCharStr)) {
			is_pattern_ = true;
			continue;
		}
		//	Single greater-than ('>') is OK...
		else if (!strcmp(element_list_[count_1], ElementLastCharStr)) {
			//	... so long as it's at the end of the subject name.
			if (count_1 < (element_count_ - 1))
				throw RvException("Subject name '" + std::string(subject_name) +
					"' contains a trailing element match pattern character ('" +
					ElementLastCharStr + "') inside of element number " +
					MLB::Utility::AnyToString(count_1 + 1) +
					", which is not the last element.");
			is_pattern_ = true;
			break;
		}
		//	Can't have meta-chars embedded within elements... Only stand-alone.
		else if (((tmp_ptr = strchr(element_list_[count_1],
			ElementAnyChar)) != NULL) ||
			((tmp_ptr = strchr(element_list_[count_1],
			ElementLastChar)) != NULL))
			throw RvException("Subject name '" + std::string(subject_name) +
				"' contains the pattern matching meta-character '" + *tmp_ptr +
				"' within element number " +
				MLB::Utility::AnyToString(count_1 + 1) + ".");
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void SubjectNameWork::SetSubjectName(const std::string &subject_name)
{
	SetSubjectName(subject_name.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string SubjectNameWork::ToString() const
{
	std::string out_string;

	return(ToString(out_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &SubjectNameWork::ToString(std::string &out_string) const
{
	char subject_name[sizeof(subject_name_)];

	return(out_string.assign(ToString(subject_name)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
char *SubjectNameWork::ToString(char *out_string) const
{
	unsigned int  count_1;
	char         *tmp_ptr = out_string;

	for (count_1 = 0; count_1 < element_count_; ++count_1) {
		if (count_1)
			*tmp_ptr++ = '.';
		strcpy(tmp_ptr, element_list_[count_1]);
		tmp_ptr += strlen(tmp_ptr);
	}

	return(out_string);
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string SubjectNameWork::ToStringFromElements(unsigned int element_start,
	unsigned int element_end) const
{
	std::string out_string;

	return(ToStringFromElements(out_string, element_start, element_end));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string &SubjectNameWork::ToStringFromElements(std::string &out_string,
	unsigned int element_start, unsigned int element_end) const
{
	char subject_name[MaxSubjectSize + 1];

	return(out_string.assign(ToStringFromElements(subject_name, element_start,
		element_end)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *SubjectNameWork::ToStringFromElements(char *out_string,
	unsigned int element_start, unsigned int element_end) const
{
	if (element_start > element_end)
		std::swap(element_start, element_end);

	char *out_ptr = out_string;

	while ((element_start < element_count_) &&
		(element_start <= element_end)) {
		size_t element_length = strlen(element_list_[element_start]);
		::memcpy(out_ptr, element_list_[element_start++], element_length);
		if (out_ptr != out_string) {
			out_ptr[element_length] = '.';
			++out_ptr;
		}
		out_ptr += element_length;
	}

	*out_ptr = '\0';

	return(out_string);
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool SubjectNameWork::operator < (const SubjectNameWork &other) const
{
	return(strcmp(subject_name_, other.subject_name_) < 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const SubjectNameWork &datum)
{
	char tmp_datum[sizeof(datum.subject_name_)];

	o_str <<
		datum.ToString(tmp_datum);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::RvUtilX;

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		SubjectNameWork subject_1("Name.1.Text");
		SubjectNameWork subject_2("Name.2.Text.Other.Stuff");
		//
		std::cout << "Start values                        :" << std::endl;
		std::cout << " ---> [" << subject_1 << "]" << std::endl;
		std::cout << " ---> [" << subject_2 << "]" << std::endl;
		//
		subject_1.swap(subject_2);
		std::cout << "Swap test (1, 2)                    :" << std::endl;
		std::cout << " ---> [" << subject_1 << "]" << std::endl;
		std::cout << " ---> [" << subject_2 << "]" << std::endl;
		//
		subject_1.swap(subject_2);
		std::cout << "Swap test (1, 2)                    :" << std::endl;
		std::cout << " ---> [" << subject_1 << "]" << std::endl;
		std::cout << " ---> [" << subject_2 << "]" << std::endl;
		//
		subject_1 = subject_2;
		std::cout << "Assignment constructor test (1 <- 2):" << std::endl;
		std::cout << " ---> [" << subject_1 << "]" << std::endl;
		std::cout << " ---> [" << subject_2 << "]" << std::endl;
		//
		SubjectNameWork subject_3(subject_2);
		std::cout << "Copy constructor test (3 <- 2)      :" << std::endl;
		std::cout << " ---> [" << subject_2 << "]" << std::endl;
		std::cout << " ---> [" << subject_3 << "]" << std::endl;
		std::cout << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "SubjectNameWork.cpp test fatal error "
			"encountered: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

