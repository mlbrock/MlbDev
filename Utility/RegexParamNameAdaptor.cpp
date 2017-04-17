//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of functions which provide regex-based
								adaptors for command line parameter parsing.

	Revision History	:	2006-10-25 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2006 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/RegexParamNameAdaptor.hpp>
#include <Utility/ParseCmdLineArg.hpp>
#include <Utility/PosixCRegex.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {
//	////////////////////////////////////////////////////////////////////////////
const char MyPrivateUnlikelyParameterName[] =
	"846B38B0-FCE6-472A-A636-74ED612C674C"
	"--"
	"7CD502BF-3D22-4ADF-9B2A-CD45D307D762"
	"--"
	"886DF695-DE04-4B06-96BC-1A5331B51D13"
	"--"
	"23539B89-76F1-4AFA-8888-636B85EFE7F4";
//	////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
bool RegexParamName(const PosixCRegexWrapper &param_pattern,
	const char *this_param)
{
	return(param_pattern.RegexSearch(this_param));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RegexParamName(const PosixCRegexWrapper &param_pattern,
	const std::string &this_param)
{
	return(RegexParamName(param_pattern, this_param.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RegexParamName(const char *param_pattern, const char *this_param)
{
	return(RegexParamName(PosixCRegexWrapper(param_pattern,
		boost::REG_PERL | boost::REG_ICASE), this_param));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RegexParamName(const std::string &param_pattern,
	const std::string &this_param)
{
	return(RegexParamName(param_pattern.c_str(), this_param.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RegexParamName(const char *param_pattern, unsigned int &current_index,
	int argc, char **argv)
{
	ParseCmdLineArg::CheckCmdLineSource(current_index, argc, argv);

	return(RegexParamName(param_pattern, argv[current_index]));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RegexParamName(const std::string &param_pattern,
	unsigned int &current_index, int argc, char **argv)
{
	return(RegexParamName(param_pattern.c_str(), current_index, argc, argv));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RegexParamName(const std::vector<std::string> &pattern_list,
	const char *this_param)
{
	std::vector<std::string>::const_iterator iter_b(pattern_list.begin());
	std::vector<std::string>::const_iterator iter_e(pattern_list.end());

	for ( ; iter_b != iter_e; ++iter_b) {
		if (RegexParamName(PosixCRegexWrapper(*iter_b,
			boost::REG_PERL | boost::REG_ICASE), this_param))
			return(true);
	}

	return(false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RegexParamName(const std::vector<std::string> &pattern_list,
	const std::string &this_param)
{
	return(RegexParamName(pattern_list, this_param.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool RegexParamName(const std::vector<std::string> &pattern_list,
	unsigned int &current_index, int argc, char **argv)
{
	ParseCmdLineArg::CheckCmdLineSource(current_index, argc, argv);

	return(RegexParamName(pattern_list, argv[current_index]));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *RegexParamNameAdaptor(const PosixCRegexWrapper &param_pattern,
	const char *this_param)
{
	return((RegexParamName(param_pattern, this_param)) ? this_param :
		MyPrivateUnlikelyParameterName);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *RegexParamNameAdaptor(const PosixCRegexWrapper &param_pattern,
	const std::string &this_param)
{
	return(RegexParamNameAdaptor(param_pattern, this_param.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////


//	////////////////////////////////////////////////////////////////////////////
const char *RegexParamNameAdaptor(const char *param_pattern,
	const char *this_param)
{
	return((RegexParamName(param_pattern, this_param)) ? this_param :
		MyPrivateUnlikelyParameterName);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *RegexParamNameAdaptor(const std::string &param_pattern,
	const std::string &this_param)
{
	return(RegexParamNameAdaptor(param_pattern.c_str(), this_param.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *RegexParamNameAdaptor(const char *param_pattern,
	unsigned int &current_index, int argc, char **argv)
{
	return((RegexParamName(param_pattern, current_index, argc, argv)) ?
		argv[current_index] : MyPrivateUnlikelyParameterName);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *RegexParamNameAdaptor(const std::string &param_pattern,
	unsigned int &current_index, int argc, char **argv)
{
	return(RegexParamNameAdaptor(param_pattern.c_str(), current_index, argc,
		argv));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *RegexParamNameAdaptor(const std::vector<std::string> &pattern_list,
	const char *this_param)
{
	return((RegexParamName(pattern_list, this_param)) ? this_param :
		MyPrivateUnlikelyParameterName);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *RegexParamNameAdaptor(const std::vector<std::string> &pattern_list,
	const std::string &this_param)
{
	return(RegexParamNameAdaptor(pattern_list, this_param.c_str()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const char *RegexParamNameAdaptor(const std::vector<std::string> &pattern_list,
	unsigned int &current_index, int argc, char **argv)
{
	return((RegexParamName(pattern_list, current_index, argc, argv)) ?
		argv[current_index] : MyPrivateUnlikelyParameterName);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#include <Utility/InlineContainer.hpp>
#include <Utility/StringSupport.hpp>

#ifdef TEST_MAIN

//	////////////////////////////////////////////////////////////////////////////
int TEST_ParseCmdLineRegex_1(int argc, char **argv)
{
	using namespace MLB::Utility;

	int return_code = EXIT_SUCCESS;

	try {
		PosixCRegexWrapper my_regex(
			"^\\-(CME(_)*)*PREAMBLE(_)*SEQ(UENCE)*((_)*NUM(B(ER)*)*)*$",
			boost::REG_PERL | boost::REG_ICASE);
		for (int count_1 = 1; count_1 < argc; ++count_1) {
			std::cout << std::setw(4) << count_1 << ": [" << argv[count_1] <<
				"] = " << std::flush;
			try {
				std:: cout << (RegexParamName(my_regex, argv[count_1]) ?
					"" : "NOT ") << "MATCHED" << std::endl;
			}
			catch (const std::exception &except) {
				std::cout << "ERROR: " << except.what() << std::endl;
			}
		}
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR IN REGEX PARSE TEST: " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cout << PadLeft("", 79, '-') << std::endl;

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int TEST_ParseCmdLineRegex_2(int argc, char **argv)
{
	using namespace MLB::Utility;

	int return_code = EXIT_SUCCESS;

	try {
		std::vector<std::string> pattern_list(
			MLB::Utility::MakeInlineVector<std::string>
				("^\\-(CME(_)*)*PREAMBLE(_)*SEQ(UENCE)*((_)*NUM(B(ER)*)*)*$")
				("^\\-(CME(_)*)*PREAMBLE(_)*SEQ(UENCE)*((_)*NUM(B(ER)*)*)*X$"));
		for (int count_1 = 1; count_1 < argc; ++count_1) {
			std::cout << std::setw(4) << count_1 << ": [" << argv[count_1] <<
				"] = " << std::flush;
			try {
				std:: cout << (RegexParamName(pattern_list, argv[count_1]) ?
					"" : "NOT ") << "MATCHED" << std::endl;
			}
			catch (const std::exception &except) {
				std::cout << "ERROR: " << except.what() << std::endl;
			}
		}
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR IN REGEX PARSE TEST: " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	std::cout << PadLeft("", 79, '-') << std::endl;

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		if (TEST_ParseCmdLineRegex_1(argc, argv) != EXIT_SUCCESS)
			return_code = EXIT_FAILURE;
		if (TEST_ParseCmdLineRegex_2(argc, argv) != EXIT_SUCCESS)
			return_code = EXIT_FAILURE;
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

