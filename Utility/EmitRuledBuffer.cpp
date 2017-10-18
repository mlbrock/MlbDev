//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	EmitRuledBuffer.cpp

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
std::vector<std::string> EmitRuledBuffer(std::size_t src_length,
	const char *src_ptr, std::size_t start_offset = 0,
	bool use_c_sequences = true, bool use_simple_nul = true,
	bool use_8bit_ascii = false);
std::vector<std::string> EmitRuledBuffer(const std::string &src,
	std::size_t start_offset = 0, bool use_c_sequences = true,
	bool use_simple_nul = true, bool use_8bit_ascii = false);
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
/*

     123456789 123456789 123456789 123456789 123456789 123456789 123456789
         |         |         |         |         |         |         |
0123456789012345678901234567890123456789012345678901234567890123456789012345678
*/
//	////////////////////////////////////////////////////////////////////////////
std::vector<std::string> EmitRuledBuffer(std::size_t src_length,
	const char *src_ptr, std::size_t start_offset, bool use_c_sequences,
	bool use_simple_nul, bool use_8bit_ascii)
{
	std::vector<std::string> dst(3);

	if (src_length < 1)
		return(dst);

	dst[0].reserve(src_length);
	dst[1].reserve(src_length);
	dst[2].reserve(src_length);

	std::size_t  curr_index = 0;
	std::size_t  next_rule  = 10 + (start_offset % 10);
	const char  *c_seq_ptr;

	while (curr_index < src_length) {
		if (curr_index == next_rule) {
			std::ostringstream o_str;
			o_str << ((next_rule < 100000000) ? "" : "?") <<
				(next_rule % 100000000);
			std::size_t dst_1_pad = (dst[0].size() - dst[1].size()) - 1;
			std::size_t dst_2_pad = (dst[0].size() - dst[2].size()) -
				((o_str.str().size() / 2) + 1);
			dst[1]    += std::string(dst_1_pad, ' ') + '|';
			dst[2]    += std::string(dst_2_pad, ' ') + o_str.str();
			next_rule += 10;
		}
		if (::isprint(*src_ptr))
			dst[0] += *src_ptr;
		else if (use_c_sequences &&
			((c_seq_ptr = ::strchr(MyCSequenceSrc, *src_ptr)) != NULL)) {
			dst[0] += '\\';
			dst[0] += MyCSequenceDst[c_seq_ptr - MyCSequenceSrc];
		}
		else if ((!(*src_ptr)) && use_simple_nul)
			dst[0] += "\\0";
		else if (use_8bit_ascii && (*src_ptr > '~'))
			dst[0] += *src_ptr;
		else {
			dst[0] += "\\x";
			dst[0] += MyHexDigitList[(*src_ptr >> 4) & 0x0f];
			dst[0] += MyHexDigitList[*src_ptr & 0x0f];
		}
		++src_ptr;
		++curr_index;
	}

	return(dst);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::vector<std::string> EmitRuledBuffer(const std::string &src,
	std::size_t start_offset, bool use_c_sequences, bool use_simple_nul,
	bool use_8bit_ascii)
{
	return(EmitRuledBuffer(src.size(), src.c_str(), start_offset,
		use_c_sequences, use_simple_nul, use_8bit_ascii));
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
void TEST_EmitStringContents(const std::string &src)
{
	std::vector<std::string> dst(EmitRuledBuffer(src));

	for (std::size_t count_1 = 0; count_1 < dst.size(); ++count_1) 
		std::cout << dst[count_1] << '\n';

	std::cout << std::flush;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_EmitFileContents(const std::string &file_name)
{
	std::string file_data(ReadFileData(file_name));

	TEST_EmitStringContents(file_data);
}
//	////////////////////////////////////////////////////////////////////////////

namespace {

//	////////////////////////////////////////////////////////////////////////////
const char *TEST_DoStandAloneTestsList[] = {
	"A line\nFollowed by another line.",
	"A line\nAnother line with embedded \x7f"" high-ASCII \xff characters.",
	"12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890",
	NULL
};
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
void TEST_DoStandAloneTests()
{
	const char **list_ptr = TEST_DoStandAloneTestsList;

	while (*list_ptr)
		TEST_EmitStringContents(*list_ptr++);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	std::cout << "Test routine for 'EmitRuledBuffer()'" << std::endl;
	std::cout << "---- ------- --- -------------------" << std::endl;

	int return_code = EXIT_SUCCESS;

	if (ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ <file-name> [ <file-name> ... ] ]" << std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}

	try {
		if (argc > 1) {
			for (int count_1 = 1; count_1 < argc; ++count_1)
				TEST_EmitFileContents(argv[count_1]);
		}
		else
			TEST_DoStandAloneTests();
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

