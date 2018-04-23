//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the configuration file management
								classes.

	Revision History	:	2005-08-13 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
#ifndef HH__MLB__Utility__ConfigFileSupport_hpp__HH

#define HH__MLB__Utility__ConfigFileSupport_hpp__HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/C_StringSupport.hpp>
#include <Utility/PosixCRegex.hpp>

#include <deque>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
struct ConfigItem {
	explicit ConfigItem(const char *param_value, const char *source_name = "",
		unsigned int line_number = 0);
	ConfigItem(const std::string &param_value, const std::string &source_name,
		unsigned int line_number = 0);

	std::string GetLocationString() const;

	std::string  param_value_;
	std::string  source_name_;
	unsigned int line_number_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<ConfigItem>          ConfigItemVector;
typedef ConfigItemVector::iterator       ConfigItemVectorIter;
typedef ConfigItemVector::const_iterator ConfigItemVectorIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::deque<ConfigItem>          ConfigItemDeque;
typedef ConfigItemDeque::iterator       ConfigItemDequeIter;
typedef ConfigItemDeque::const_iterator ConfigItemDequeIterC;
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct ConfigFileResolver {
	ConfigFileResolver();
	ConfigFileResolver(const std::string &inc_file_regex,
		const std::string &inc_file_text);

	virtual ~ConfigFileResolver();

	virtual bool IsCommentLine(const std::string &in_line) const;
	virtual bool IsIncludeFileSpec(std::size_t current_index,
		const std::vector<std::string> &argv, std::string &param_name,
		std::string &inc_file_name) const;
	virtual bool IsIncludeFileSpec(std::size_t current_index, int argc,
		char **argv, std::string &param_name, std::string &inc_file_name) const;

	virtual std::string GetHelpTextShort() const;
	virtual std::string GetHelpTextLong() const;

	PosixCRegexWrapper inc_file_regex_;
	std::string        inc_file_text_;

	inline static const char *GetIncFilePatternDefault()
	{
		return("^\\-\\-*PAR(M|(AM(ETER)*)*)*S*(_|\\-)*(INC(LUDE)*)S"
			"*(_|\\-)*(FILES*((_|\\-)*NAMES*)*)*$");
	}

	inline static const char *GetIncFileTextDefault()
	{
		return("-parameter_include_file");
	}
};
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class ConfigFile {
public:
	typedef std::vector<std::string> ArgvVector;
	typedef C_ListTypeString<int>    ArgvListWrapper;

	ConfigFile(int argc, char **argv,
		const ConfigFileResolver &resolver = ConfigFileResolver());
	ConfigFile(const ArgvVector &argv,
		const ConfigFileResolver &resolver = ConfigFileResolver());

	void swap(ConfigFile &other);

	void AppendLines(const ArgvVector &argv, const std::string &source_name,
		unsigned int line_number, const ConfigFileResolver &resolver);
	void AppendIncludeFile(const ConfigItem inc_item,
		const std::string &inc_file_name, const ConfigFileResolver &resolver);

	ArgvVector      &GetArgvList(std::vector<std::string> &out_argv);
	ArgvVector       GetArgvList();
	ArgvListWrapper &GetArgvListWrapper(ArgvListWrapper &out_argv);
	ArgvListWrapper  GetArgvListWrapper();

	std::size_t GetMaxSourceNameLength() const;
	std::string FormatItems(std::size_t src_width = 31) const;

	ArgvVector       original_argv_;
	ConfigItemVector item_list_;
	ConfigItemDeque  include_stack_;

private:
	void AppendLines(bool src_is_file, const ArgvVector &argv,
		const std::string &source_name, unsigned int line_number,
		const ConfigFileResolver &resolver);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__ConfigFileSupport_hpp__HH

