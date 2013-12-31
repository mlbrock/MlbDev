// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB BoostX Posix CRegex Include File
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


#ifndef HH__MLB__BoostX__PosixCRegex_hpp__HH

#define HH__MLB__BoostX__PosixCRegex_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility_Exception.hpp>
#include <BoostX.hpp>

#if _MSC_VER >= 1300
# pragma warning(disable:4668)
#endif // #ifdef _MSC_VER >= 1300

# include <boost/cregex.hpp>

#if _MSC_VER >= 1300
# pragma warning(default:4668)
#endif // #ifdef _MSC_VER >= 1300

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace BoostX {

//	////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_BOOSTX) PosixCRegexException :
	public BoostException {
public:
	PosixCRegexException(int regex_error_code,
		const boost::regex_t *the_regex, const char *except_string = NULL)
		:BoostException(FixupString(regex_error_code, the_regex, except_string))
		,regex_error_code_() { }

	int whatcode() const {
		return(regex_error_code_);
	}

	virtual void Rethrow(const char *except_string = NULL) const {
		PosixCRegexException tmp_except(*this);
		tmp_except.SetWhat(except_string);
		throw tmp_except;
	}

protected:
	int regex_error_code_;

	static std::string FixupString(int regex_error_code,
		const boost::regex_t *the_regex, const char *other_text = NULL) {
		char error_text[255 + 1];
		regerror(regex_error_code, the_regex, error_text, sizeof(error_text) - 1);
		std::ostringstream status_string;
		status_string <<
			"Error in boost::regex Posix compatibility function call: " <<
			(((other_text != NULL) && *other_text) ? other_text : "Error ") <<
			": Regex error code " << regex_error_code << ", '" << error_text <<
			"'.";
		return(status_string.str());
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_BOOSTX PosixCRegexWrapper {
public:
	explicit PosixCRegexWrapper(const char *regex_string = NULL,
		int regex_flags = boost::REG_PERL)
		:this_regex_()
		,ready_flag_(false)
	{
		if (regex_string != NULL)
			ConstructRegexThis(regex_string, regex_flags);
	}
	explicit PosixCRegexWrapper(const std::string &regex_string,
		int regex_flags = boost::REG_PERL)
		:this_regex_()
		,ready_flag_(false)
	{
		ConstructRegexThis(regex_string.c_str(), regex_flags);
	}
	~PosixCRegexWrapper()
	{
		DestroyRegex();
	}

	bool IsReady() const {
		return(ready_flag_);
	}
	void CheckReady() const {
		if (!IsReady())
			MLB::Utility::ThrowInvalidArgument("This 'MLB::BoostX::"
				"PosixCRegexWrapper' instance is not constructed.");
	}

	bool ClearRegex() {
		bool ready_flag = ready_flag_;
		DestroyRegex();
		return(ready_flag);
	}
	bool SetRegex(const char *regex_string, int regex_flags = boost::REG_PERL) {
		boost::regex_t tmp_regex;
		ConstructRegex(regex_string, tmp_regex, regex_flags);
		bool ready_flag = ClearRegex();
		this_regex_ = tmp_regex;
		ready_flag_ = true;
		return(ready_flag);
	}
	bool SetRegex(const std::string &regex_string,
		int regex_flags = boost::REG_PERL) {
		return(SetRegex(regex_string.c_str(), regex_flags));
	}

	bool RegexSearch(const char *target_string, int search_flags = 0) const {
		CheckReady();
		if (target_string == NULL)
			MLB::Utility::ThrowInvalidArgument("Call to 'MLB::BoostX::"
				"PosixCRegexWrapper::RegexSearch()' with a 'NULL' string.");
		int regex_error;
		if ((regex_error = boost::regexec(&this_regex_, target_string,
			0, NULL, search_flags)) == 0)
			return(true);
		else if (regex_error != boost::REG_NOMATCH)
			throw MLB::BoostX::PosixCRegexException(regex_error, &this_regex_,
				"Call to 'regexec()' failed");
		return(false);
	}
	bool RegexSearch(const std::string &target_string,
		int search_flags = 0) const {
		return(RegexSearch(target_string.c_str(), search_flags));
	}

private:
	boost::regex_t this_regex_;
	bool           ready_flag_;

	void ConstructRegexThis(const char *regex_string, int regex_flags) {
		ConstructRegex(regex_string, this_regex_, regex_flags);
		ready_flag_ = true;
	}
	void DestroyRegex() {
		if (ready_flag_)
			boost::regfree(&this_regex_);
		ready_flag_ = false;
	}
	void ConstructRegex(const char *regex_string, boost::regex_t &in_regex,
		int regex_flags) {
		if (regex_string == NULL)
			MLB::Utility::ThrowInvalidArgument(
				"Specified regex string is 'NULL'.");
		try {
			int regex_error;
			if ((regex_error = boost::regcomp(&in_regex, regex_string,
				regex_flags)) != 0)
				throw MLB::BoostX::PosixCRegexException(regex_error, &in_regex,
					"Call to 'regcomp()' failed");
		}
		catch (const std::exception &except) {
			MLB::Utility::Rethrow(except, "Unable to compile the regular "
				"expression '" + std::string(regex_string) + "': " +
				std::string(except.what()));
		}
	}
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace BoostX

} // namespace MLB

#endif // #ifndef HH__MLB__BoostX__PosixCRegex_hpp__HH

