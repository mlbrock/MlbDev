//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the configuration file management
								classes.

	Revision History	:	2005-08-13 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/ConfigFileSupport.hpp>
#include <Utility/StringSupport.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
ConfigItem::ConfigItem(const char *param_value, const char *source_name,
	unsigned int line_number)
	:param_value_(ThrowIfNull(param_value, "The parameter name"))
	,source_name_(NullToString(source_name))
	,line_number_(line_number)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ConfigItem::ConfigItem(const std::string &param_value,
	const std::string &source_name, unsigned int line_number)
	:param_value_(param_value)
	,source_name_(source_name)
	,line_number_(line_number)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ConfigItem::GetLocationString() const
{
	std::ostringstream o_str;

	if (source_name_.empty())
		o_str << "CommandLine" << ", ParameterIndex=" << line_number_;
	else
		o_str << "File=" << source_name_ << ", Line=" << line_number_;

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ConfigFileResolver::ConfigFileResolver()
	:inc_file_regex_(PosixCRegexWrapper(GetIncFilePatternDefault(),
		boost::REG_PERL | boost::REG_ICASE))
	,inc_file_text_(GetIncFileTextDefault())
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ConfigFileResolver::ConfigFileResolver(const std::string &inc_file_regex,
	const std::string &inc_file_text)
	:inc_file_regex_(PosixCRegexWrapper(inc_file_regex,
		boost::REG_PERL | boost::REG_ICASE))
	,inc_file_text_(inc_file_text)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ConfigFileResolver::~ConfigFileResolver()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ConfigFileResolver::IsCommentLine(const std::string &in_line) const
{
	return((!in_line.empty()) && (in_line[0] == '#'));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ConfigFileResolver::IsIncludeFileSpec(std::size_t current_index,
	const std::vector<std::string> &argv, std::string &param_name,
	std::string &inc_file_name) const
{
	if (current_index >= argv.size())
		return(false);

	if (!inc_file_regex_.RegexSearch(argv[current_index]))
		return(false);

	if ((current_index + 1) >= argv.size()) { 
		std::ostringstream o_str; 
		o_str << "A parameter indicating the presence of a following " 
			"configuration file name was encountered ('" << 
			argv[current_index] << "'), but there is no element after it in " 
			"the list of elements ([" << JoinString(argv, "] [") << "])."; 
		ThrowInvalidArgument(o_str.str()); 
	} 

	param_name    = argv[current_index];
	inc_file_name = argv[current_index + 1];

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ConfigFileResolver::IsIncludeFileSpec(std::size_t current_index, int argc,
	char **argv, std::string &param_name, std::string &inc_file_name) const
{
	return(IsIncludeFileSpec(current_index,
		std::vector<std::string>(argv, argv + argc), param_name, inc_file_name));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ConfigFileResolver::GetHelpTextShort() const
{
	std::ostringstream o_str;

	o_str << GetIncFileTextDefault() << " <parameter-include-file-name>";

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ConfigFileResolver::GetHelpTextLong() const
{
	std::ostringstream o_str;

	o_str <<
"\
   " << GetHelpTextShort() << "\n\
      Specifies the name of a file from which the program is to read program\n\
      parameters.";

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ConfigFile::ConfigFile(int argc, char **argv,
	const ConfigFileResolver &resolver)
	:original_argv_()
	,item_list_()
	,include_stack_()
{
	if (argc < 0)
		ThrowInvalidArgument("The 'argc' parameter is less than zero.");
	else if (argc) {
		ArgvVector(argv, argv + argc).swap(original_argv_);
		ConfigFile(original_argv_, resolver).swap(*this);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ConfigFile::ConfigFile(const ArgvVector &argv,
	const ConfigFileResolver &resolver)
	:original_argv_(argv)
	,item_list_()
	,include_stack_()
{
	AppendLines(original_argv_, "", 0, resolver);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ConfigFile::swap(ConfigFile &other)
{
	item_list_.swap(other.item_list_);
	include_stack_.swap(other.include_stack_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ConfigFile::AppendLines(const ArgvVector &argv,
	const std::string &source_name, unsigned int line_number,
	const ConfigFileResolver &resolver)
{
	AppendLines(false, argv, source_name, line_number, resolver);
}
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
struct ConfigFileIncFileRollback {
	ConfigFileIncFileRollback(ConfigItemDeque &include_stack)
		:include_stack_(include_stack)
	{
	}
	~ConfigFileIncFileRollback()
	{
		include_stack_.pop_back();
	}

	ConfigItemDeque &include_stack_;

private:
	ConfigFileIncFileRollback(const ConfigFileIncFileRollback &);
	ConfigFileIncFileRollback & operator = (const ConfigFileIncFileRollback &);
};
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
void ConfigFile::AppendIncludeFile(const ConfigItem inc_item,
	const std::string &inc_file_name, const ConfigFileResolver &resolver)
{
	ConfigItemDequeIterC iter_b(include_stack_.begin());
	ConfigItemDequeIterC iter_e(include_stack_.end());

	for ( ; iter_b != iter_e; ++iter_b) {
#if defined(_MSC_VER) || defined(WIN32) || defined(WIN64) || defined(VMS)
		if (!Utility_stricmp(iter_b->source_name_.c_str(), inc_file_name.c_str()))
#else
		if (!iter_b->source_name_.compare(inc_file_name))
#endif // #if defined(_MSC_VER) || defined(WIN32) || defined(WIN64) || defined(VMS)
		{
			std::ostringstream o_str;
			o_str << "Include file '" << inc_file_name << "' is included "
				"multiple times by this configuration:";
			ConfigItemDeque tmp_include_stack(include_stack_);
			std::streamsize depth = 1;
			while (!tmp_include_stack.empty()) {
				o_str << std::endl << std::setw(depth++) << "" << "From: " <<
					tmp_include_stack.front().GetLocationString();
				tmp_include_stack.pop_front();
			}
			ThrowInvalidArgument(o_str.str());
		}
	}

	include_stack_.push_back(inc_item);

	ConfigFileIncFileRollback rollback_stack(include_stack_);
	ArgvVector                argv;

	if (ReadFileLines(inc_file_name, argv))
		AppendLines(true, argv, inc_file_name, 1, resolver);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ConfigFile::ArgvVector &ConfigFile::GetArgvList(ArgvVector &out_argv)
{
	ConfigItemVectorIterC iter_b(item_list_.begin());
	ConfigItemVectorIterC iter_e(item_list_.end());
	ArgvVector            tmp_argv;

	tmp_argv.reserve(item_list_.size());

	for ( ; iter_b != iter_e; ++iter_b)
		tmp_argv.push_back(iter_b->param_value_);

	out_argv.swap(tmp_argv);

	return(out_argv);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ConfigFile::ArgvVector ConfigFile::GetArgvList()
{
	ArgvVector out_argv;

	return(GetArgvList(out_argv));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ConfigFile::ArgvListWrapper &ConfigFile::GetArgvListWrapper(
	ConfigFile::ArgvListWrapper &out_argv)
{
	ConfigFile::ArgvListWrapper tmp_argv;

	//	Construct the NULL pointer terminated char** array for argv...
	tmp_argv.FromContainer(GetArgvList(),
		"Attempt to construct an 'argv' list failed");

	out_argv.Swap(tmp_argv);

	return(out_argv);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ConfigFile::ArgvListWrapper ConfigFile::GetArgvListWrapper()
{
	ConfigFile::ArgvListWrapper out_argv;

	return(GetArgvListWrapper(out_argv));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t ConfigFile::GetMaxSourceNameLength() const
{
	std::size_t           max_src_width = 0;
	ConfigItemVectorIterC iter_b(item_list_.begin());
	ConfigItemVectorIterC iter_e(item_list_.end());

	for ( ; iter_b != iter_e; ++iter_b)
		max_src_width = std::max(max_src_width, iter_b->source_name_.size());

	return(max_src_width);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ConfigFile::FormatItems(std::size_t src_width) const
{
/*
	CODE NOTE: Obsoleted. To be removed.
	std::size_t max_src_width  = src_width;

	{
		ConfigItemVectorIterC iter_b(item_list_.begin());
		ConfigItemVectorIterC iter_e(item_list_.end());
		for ( ; iter_b != iter_e; ++iter_b)
			max_src_width  = std::max(max_src_width, iter_b->source_name_.size());
		max_src_width = (!src_width) ? max_src_width : src_width;
	}
*/

	std::size_t max_src_width = (src_width) ? src_width :
		GetMaxSourceNameLength();

	std::ostringstream    o_str;
	ConfigItemVectorIterC iter_b(item_list_.begin());
	ConfigItemVectorIterC iter_e(item_list_.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		o_str
			<< ((iter_b != item_list_.begin()) ? "\n" : "")
			<< std::setw(10)        << iter_b->line_number_ << ":"
			<< std::left
			<< std::setw(static_cast<std::streamsize>(max_src_width))  <<
				iter_b->source_name_ << ":"
			<< std::right
			<< iter_b->param_value_
				;
		
	}

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ConfigFile::AppendLines(bool src_is_file, const ArgvVector &argv,
	const std::string &source_name, unsigned int line_number,
	const ConfigFileResolver &resolver)
{
	std::size_t argc = argv.size();

	for (std::size_t count_1 = 0; count_1 < argc; ++count_1) {
		std::string this_line(TrimLeft(argv[count_1]));
		if ((!this_line.empty()) && (!resolver.IsCommentLine(this_line))) {
			std::string param_name;
			std::string inc_file_name;
			if (!src_is_file) {
				if (resolver.IsIncludeFileSpec(count_1, argv, param_name,
					inc_file_name)) {
					AppendIncludeFile(ConfigItem("", source_name, count_1),
						inc_file_name, resolver);
					++count_1;
				}
				else
					item_list_.push_back(
						ConfigItem(this_line, source_name, count_1));
			}
			else {
				std::string::size_type sep_point = this_line.find_first_of("=");
				if (!sep_point) {
					std::ostringstream o_str;
					o_str << "An empty parameter name was encountered in the "
						"line '" << argv[count_1] << "' loaded from line "
						"number " << line_number << " of configuration "
						"source '" << source_name << "'.";
					ThrowInvalidArgument(o_str.str());
				}
				else if (sep_point == std::string::npos) {
					std::ostringstream o_str;
					o_str << "Unable to locate the parameter name/value "
						"separator ('=') in the line '" << argv[count_1] <<
						"' loaded from line number " << line_number <<
						" of configuration source '" << source_name << "'.";
					ThrowInvalidArgument(o_str.str());
				}
				this_line[sep_point] = '\0';
				ArgvVector tmp_argv;
				tmp_argv.push_back(Trim(this_line.c_str()));
				tmp_argv.push_back(this_line.c_str() + sep_point + 1);
				if (resolver.IsIncludeFileSpec(0, tmp_argv, param_name,
					inc_file_name))
					AppendIncludeFile(ConfigItem("", source_name, line_number),
						inc_file_name, resolver);
				else {
					item_list_.push_back(ConfigItem(tmp_argv[0], source_name,
						line_number));
					item_list_.push_back(ConfigItem(tmp_argv[1], source_name,
						line_number));
				}
			}
		}
		if (line_number)
			++line_number;
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		using namespace MLB::Utility;
		ConfigFile cfg_file(argc, argv);
		std::cout << cfg_file.FormatItems(0) << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

