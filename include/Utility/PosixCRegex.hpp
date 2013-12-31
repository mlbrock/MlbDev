// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for a C++ wrapper for the boost::regex 
								Posix C language regex support.

	Revision History	:	2006-10-25 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2006 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__PosixCRegex_hpp__HH

#define HH__MLB__Utility__PosixCRegex_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility_Exception.hpp>

#if _MSC_VER >= 1300
# pragma warning(push)
# pragma warning(disable:4668)
#endif // #ifdef _MSC_VER >= 1300

# include <boost/cregex.hpp>

#if _MSC_VER >= 1300
# pragma warning(pop)
#endif // #ifdef _MSC_VER >= 1300

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_UTILITY) PosixCRegexException :
	public ExceptionGeneral {
public:
	PosixCRegexException(int regex_error_code,
		const boost::regex_t *the_regex, const char *except_string = NULL);

	int whatcode() const;

	virtual void Rethrow(const char *except_string = NULL) const;

protected:
	int regex_error_code_;

	static std::string FixupString(int regex_error_code,
		const boost::regex_t *the_regex, const char *other_text = NULL);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_UTILITY PosixCRegexWrapper {
public:
	explicit PosixCRegexWrapper(const char *regex_string = NULL,
		int regex_flags = boost::REG_PERL);
	explicit PosixCRegexWrapper(const std::string &regex_string,
		int regex_flags = boost::REG_PERL);

	~PosixCRegexWrapper();

	bool IsReady() const;
	void CheckReady() const;
	bool ClearRegex();
	bool SetRegex(const char *regex_string, int regex_flags = boost::REG_PERL);
	bool SetRegex(const std::string &regex_string,
		int regex_flags = boost::REG_PERL);

	bool RegexSearch(const char *target_string, int search_flags = 0) const;
	bool RegexSearch(const std::string &target_string,
		int search_flags = 0) const;

private:
	boost::regex_t this_regex_;
	bool           ready_flag_;

	void ConstructRegexThis(const char *regex_string, int regex_flags);
	void DestroyRegex();
	void ConstructRegex(const char *regex_string, boost::regex_t &in_regex,
		int regex_flags);
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__PosixCRegex_hpp__HH

