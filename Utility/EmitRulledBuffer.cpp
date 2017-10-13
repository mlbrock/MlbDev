//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	EmitRulledBuffer.cpp

	File Description	:	

	Revision History	:	2017-10-13 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2017 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/C_StringSupport.hpp>
#include <Utility/ExecProcess.hpp>
#include <Utility/Utility_Exception.hpp>

#include <algorithm>

#if defined(__unix__) && defined(__GNUC__)
# include <unistd.h>
#endif // #if defined(__unix__) && defined(__GNUC__)

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
std::vector<std::string> EmitRulledBuffer(std::size_t src_length,
	const char *src_ptr, std::size_t start_offset = 0,
	bool use_c_sequences = true, bool use_simple_nul = true,
	bool use_8bit_ascii = false);
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB


namespace MLB {

namespace Utility {

namespace {

//	////////////////////////////////////////////////////////////////////////////
const char *MyCSequenceSrc = "\a\b\t\n\v\f\r";
const char *MyCSequenceDst = "abtnvfr";

const char *MyHexDigitList = "0123456789abcdef";
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
std::vector<std::string> EmitRulledBuffer(std::size_t src_length,
	const char *src_ptr, std::size_t start_offset, bool use_c_sequences,
	bool use_simple_nul, bool use_8bit_ascii)
{
	std::vector<std::string> dst(3);

	if (src_length < 1)
		return(dst);

	dst[0].reserve(src_length);
	dst[1].reserve(src_length);
	dst[2].reserve(src_length);

	std::size_t curr_length = src_length;
	std::size_t curr_index  = 0;
	std::size_t next_rule   = 10 + (start_offset % 10);

	const char *c_seq_ptr;

	while (curr_index < src_length) {
		if (::isprint(*src_ptr)) {
			dst        += *src_ptr;
			rule_chars += 1;
		}
		else if (use_c_sequences &&
			((c_seq_ptr = ::strchr(MyCSequenceSrc, *src_ptr)) != NULL)) {
			dst        += '\\'
			dst        += MyCSequenceDst[c_seq_ptr - MyCSequenceSrc];
			rule_chars += 2;
		}
		else if ((!(*src_ptr)) && use_simple_nul) {
			dst        += "\\0"
			rule_chars += 2;
		}
		else if (use_8bit_ascii && (*src_ptr > '~')) {
			dst        += *src_ptr;
			rule_chars += 1;
		}
		else {
			dst        += '\\';
			dst        += '0';
			dst        += 'x';
			dst        += MyHexDigitList[(*src_ptr >> 4) & 0x0f];
			dst        += MyHexDigitList[*src_ptr & 0x0f];
			rule_chars += 5;
		}
	}
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/Sleep.hpp>
#include <Utility/ParseCmdLineArg.hpp>

#include <iomanip>

using namespace MLB::Utility;

//	////////////////////////////////////////////////////////////////////////////
void TEST_EmitFileContents(const std::string &file_name)
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	std::cout << "Test routine for 'EmitRulledBuffer()'" << std::endl;
	std::cout << "---- ------- --- --------------------" << std::endl;

	int          return_code   = EXIT_SUCCESS;
	unsigned int fork_depth    = 5;
	unsigned int sleep_seconds = 0;

	if (ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ <file-name> [ <file-name> ... ] ]" << std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}

	try {
		for (int count_1 = 1; count_1 < argc; ++count_1)
			TEST_EmitFileContents(argv[count_1]);
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

