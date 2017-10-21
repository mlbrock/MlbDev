//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	EmitRuledBuffer.cpp

	File Description	:	Logic to emit a ruled buffer.

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

#include <Utility.hpp>

#include <algorithm>
#include <cstdio>
#include <cstring>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
const int ErbFlag_None         = 0x00;
const int ErbFlag_NoCEscSeqs   = 0x01;
const int ErbFlag_UseHexNul    = 0x02;
const int ErbFlag_Use8BitAscii = 0x04;
const int ErbFlag_HexRule      = 0x08;
const int ErbFlag_RuleOnTop    = 0x10;
const int ErbFlag_Default      = ErbFlag_None;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: To be moved int MlbDev/include/Utility/EmitRuledBuffer.hpp .
std::vector<std::string> EmitRuledBuffer(std::size_t src_length,
	const char *src_ptr, std::size_t start_offset = 0,
	int flags = ErbFlag_Default);
std::vector<std::string> EmitRuledBuffer(const char *src_ptr,
	std::size_t start_offset = 0, int flags = ErbFlag_Default);
std::vector<std::string> EmitRuledBuffer(const char *begin_ptr,
	const char *end_ptr, std::size_t start_offset = 0,
	int flags = ErbFlag_Default);
std::vector<std::string> EmitRuledBuffer(const std::string &src,
	std::size_t start_offset = 0, int flags = ErbFlag_Default);
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

//	////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {

//	////////////////////////////////////////////////////////////////////////////
const char *MyCSequenceSrc = "\a\b\t\n\v\f\r";
const char *MyCSequenceDst = "abtnvfr";

const char *MyHexDigitList = "0123456789abcdef";
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void HandleRule(std::vector<std::string> &dst, std::size_t curr_index,
	std::size_t &next_rule, std::size_t start_offset, int flags)
{
	if (curr_index != next_rule)
		return;

	unsigned long long rule_fixed = start_offset + static_cast<unsigned long long>(next_rule);
	char               rule_buffer[1 + 8 + 1];

	if (!(flags & ErbFlag_HexRule))
		::sprintf(rule_buffer, "%s%llu",
			(rule_fixed < 100000000) ? "" : "?", rule_fixed % 100000000);
	else if (rule_fixed < 0x100000000)
		::sprintf(rule_buffer, "%llx", rule_fixed);
	else
		::sprintf(rule_buffer, "?%-08.08llx", rule_fixed % 0x100000000);

	std::size_t dst_1_pad = (dst[0].size() - dst[1].size()) - 1;
	std::size_t dst_2_pad = (dst[0].size() - dst[2].size()) -
		((::strlen(rule_buffer) / 2) + 1);

	dst[1]    += std::string(dst_1_pad, ' ') + '|';
	dst[2]    += std::string(dst_2_pad, ' ') + rule_buffer;
	next_rule += 10;
}
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
	const char *src_ptr, std::size_t start_offset, int flags)
{
	std::vector <std::string> dst(3);

	if (src_length < 1)
		return(dst);

	bool use_c_sequences = !(flags & ErbFlag_NoCEscSeqs);
	bool use_simple_nul  = !(flags & ErbFlag_UseHexNul);
	bool use_8bit_ascii  = ((flags & ErbFlag_Use8BitAscii) != 0);

	dst[0].reserve(src_length);
	dst[1].reserve(src_length);
	dst[2].reserve(src_length);

	std::size_t  curr_index = 0;
	std::size_t  rule_adj   = (start_offset % 10ULL);
	std::size_t  next_rule  = 10 + ((rule_adj) ? (10 - rule_adj) : 0);
	const char  *c_seq_ptr;

	while (curr_index < src_length) {
		if (curr_index == next_rule)
			HandleRule(dst, curr_index, next_rule, start_offset, flags);
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

	if (curr_index == next_rule)
		HandleRule(dst, curr_index, next_rule, start_offset, flags);

	if (flags & ErbFlag_RuleOnTop)
		std::reverse(dst.begin(), dst.end());

	return(dst);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::vector<std::string> EmitRuledBuffer(const char *src_ptr,
	std::size_t start_offset, int flags)
{
	return(EmitRuledBuffer(::strlen(src_ptr), src_ptr, start_offset, flags));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::vector<std::string> EmitRuledBuffer(const char *begin_ptr,
	const char *end_ptr, std::size_t start_offset, int flags)
{
	return(EmitRuledBuffer((end_ptr > begin_ptr) ?
		static_cast<std::size_t>(end_ptr - begin_ptr) : 0, begin_ptr,
		start_offset, flags));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::vector<std::string> EmitRuledBuffer(const std::string &src,
	std::size_t start_offset, int flags)
{
	return(EmitRuledBuffer(src.size(), src.c_str(), start_offset, flags));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

//	////////////////////////////////////////////////////////////////////////////
//	****************************************************************************
//	****************************************************************************
//	****************************************************************************
//	////////////////////////////////////////////////////////////////////////////

#ifdef TEST_MAIN

#include <Utility/Sleep.hpp>
#include <Utility/ParseCmdLineArg.hpp>

#include <iomanip>

using namespace MLB::Utility;

//	////////////////////////////////////////////////////////////////////////////
void TEST_EmitStringContents(const std::string &src,
	std::size_t start_offset = 0, int flags = ErbFlag_Default)
{
	std::vector<std::string> dst(EmitRuledBuffer(src, start_offset, flags));

	for (std::size_t count_1 = 0; count_1 < dst.size(); ++count_1) 
		std::cout << dst[count_1] << '\n';

	std::cout << std::flush;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_EmitFileContents(const std::string &file_name,
	std::size_t start_offset = 0, int flags = ErbFlag_Default)
{
	std::string file_data(ReadFileData(file_name));

	TEST_EmitStringContents(file_data, start_offset, flags);
}
//	////////////////////////////////////////////////////////////////////////////

namespace {

//	////////////////////////////////////////////////////////////////////////////
/*
A line\nFollowed by another line.
          |         |         |
         10        20        30
A line\nAnother line with embedded \x7f high-ASCII \xff characters.
          |         |         |            |            |         |
         10        20        30           40           50        60
12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
         |         |         |         |         |         |         |         |         |         |         |
        10        20        30        40        50        60        70        80        90        100       110
*/
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

	while (*list_ptr++) {
TEST_EmitStringContents(list_ptr[-1], 0xfffffffe);
TEST_EmitStringContents(list_ptr[-1], 0xfffffffe, ErbFlag_HexRule);
		TEST_EmitStringContents(list_ptr[-1],   0);
		TEST_EmitStringContents(list_ptr[-1],   0, ErbFlag_HexRule);
		TEST_EmitStringContents(list_ptr[-1],   0, ErbFlag_RuleOnTop);
//		TEST_EmitStringContents(list_ptr[-1],   1);
//		TEST_EmitStringContents(list_ptr[-1],   2);
//		TEST_EmitStringContents(list_ptr[-1],   3);
//		TEST_EmitStringContents(list_ptr[-1],   4);
		TEST_EmitStringContents(list_ptr[-1],   5);
		TEST_EmitStringContents(list_ptr[-1],   5, ErbFlag_HexRule);
		TEST_EmitStringContents(list_ptr[-1],   5, ErbFlag_RuleOnTop);
		TEST_EmitStringContents(list_ptr[-1], 100);
		TEST_EmitStringContents(list_ptr[-1], 100, ErbFlag_HexRule);
		TEST_EmitStringContents(list_ptr[-1], 100, ErbFlag_RuleOnTop);
		TEST_EmitStringContents(list_ptr[-1], 0xfffffffe);
		TEST_EmitStringContents(list_ptr[-1], 0xfffffffe, ErbFlag_HexRule);
//		TEST_EmitStringContents(list_ptr[-1], 0xfffffffe, ErbFlag_RuleOnTop);
//		TEST_EmitStringContents(list_ptr[-1], 101);
//		TEST_EmitStringContents(list_ptr[-1], 105);
	}
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

