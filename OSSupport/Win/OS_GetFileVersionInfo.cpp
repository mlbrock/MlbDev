// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of wrappers to the Windows function
								GetFileVersionInfo().

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OSSupport_Win.h>

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/Utility_Exception.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

namespace {
// ////////////////////////////////////////////////////////////////////////////
const char         *OS_PredefinedVersionInfoNameList[] = {
	"Comments",
	"InternalName",
	"ProductName ",
	"CompanyName",
	"LegalCopyright",
	"ProductVersion",
	"FileDescription",
	"LegalTrademarks",
	"PrivateBuild",
	"FileVersion",
	"OriginalFilename",
	"SpecialBuild"
};
const unsigned int  OS_PredefinedVersionInfoNameCount  =
	sizeof(OS_PredefinedVersionInfoNameList) /
	sizeof(OS_PredefinedVersionInfoNameList[0]);
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
const DWORD VS_FIXEDFILEINFO_Signature = 0xFEEF04BD;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void Extract_VS_FIXEDFILEINFO(DWORD buffer_length,
	char *buffer_ptr, VS_FIXEDFILEINFO &version_info)
{
	struct OS_VS_VERSIONINFO {
		WORD             wLength;
		WORD             wValueLength;
		WORD             wType;
		WCHAR            szKey[1];
		WORD             Padding1[1];
		VS_FIXEDFILEINFO Value;
		WORD             Padding2[1];
		WORD             Children[1];
	};

	try {
		OS_VS_VERSIONINFO *vs_info_ptr =
			reinterpret_cast<OS_VS_VERSIONINFO *>(buffer_ptr);
		if (vs_info_ptr->wValueLength != sizeof(VS_FIXEDFILEINFO))
			MLB::Utility::ThrowException("The virtual structure 'VS_VERSIONINFO' "
				"has a 'wValueLength' member (" +
				MLB::Utility::AnyToString(vs_info_ptr->wValueLength) + ") which "
				"is equal to the size of the 'VS_FIXEDFILEINFO' structure (" +
				MLB::Utility::AnyToString(sizeof(VS_FIXEDFILEINFO)) + ").");
		if (wcscmp(vs_info_ptr->szKey, L"VS_VERSION_INFO"))
			MLB::Utility::ThrowException("The virtual structure 'VS_VERSIONINFO' "
				"has an invalid string in member 'szKey'.");
		char *tmp_ptr           = reinterpret_cast<char *>(vs_info_ptr->szKey) +
			sizeof(L"VS_VERSION_INFO");
		unsigned int tmp_length = buffer_length - (buffer_ptr - tmp_ptr);
		bool         found_flag = false;
		while (tmp_length > 3) {
			char *try_ptr = static_cast<char *>(memchr(tmp_ptr,
				reinterpret_cast<const char *>(&VS_FIXEDFILEINFO_Signature)[0],
				tmp_length));
			if (try_ptr == NULL)
				break;
			if (!memcmp(try_ptr, &VS_FIXEDFILEINFO_Signature,
				sizeof(VS_FIXEDFILEINFO_Signature))) {
				tmp_ptr    = try_ptr;
				found_flag = true;
				break;
			}
			tmp_length += static_cast<unsigned int>(try_ptr - tmp_ptr) - 1;
			tmp_ptr     = try_ptr + 1;
		}
		if (!found_flag)
			MLB::Utility::ThrowException("The virtual structure 'VS_VERSIONINFO' "
				"does not seem to contain a 'VS_FIXEDFILEINFO' 'Value' member "
				"with a valid 'dwSignature' signature value (" +
				MLB::Utility::ValueToStringHex(VS_FIXEDFILEINFO_Signature) + ").");
		version_info = *reinterpret_cast<VS_FIXEDFILEINFO *>(tmp_ptr);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to extract the 'VS_FIXEDFILEINFO' "
			"structure: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OS_GetFileVersionInfo(const std::string &file_name, DWORD buffer_length,
	char *buffer_ptr, WORD lang_code, WORD code_page,
	VS_FIXEDFILEINFO &version_info, OS_VersionInfoSet &string_map)
{
	DWORD unknown_handle = NULL;

	typedef COMPAT_FN_TYPE(BOOL (WINAPI *OS_FPtr_GetFileVersionInfo),
		(LPTSTR, DWORD, DWORD, LPVOID));
	typedef COMPAT_FN_TYPE(BOOL (WINAPI *OS_FPtr_VerQueryValue),
		(const LPVOID, LPTSTR, LPVOID, PUINT));

	HMODULE module_handle = EnsureLoadedLibrary("version", true);

#pragma warning(disable:4191)
	OS_FPtr_GetFileVersionInfo get_info_proc_addr   =
		reinterpret_cast<OS_FPtr_GetFileVersionInfo>(OS_GetProcAddress(
		module_handle, "GetFileVersionInfoA", true));
	OS_FPtr_VerQueryValue      query_value_proc_addr =
		reinterpret_cast<OS_FPtr_VerQueryValue>(OS_GetProcAddress(
		module_handle, "VerQueryValueA", true));
#pragma warning(default:4191)

	if ((*get_info_proc_addr)(const_cast<char *>(file_name.c_str()),
		unknown_handle, buffer_length, buffer_ptr) == 0)
		MLB::Utility::ThrowSystemError("Call to 'GetFileVersionInfoA()' for "
			"file name '" + file_name + "' with buffer a length of " +
			MLB::Utility::AnyToString(buffer_length) + " failed");

	VS_FIXEDFILEINFO tmp_version_info;
	Extract_VS_FIXEDFILEINFO(buffer_length, buffer_ptr, tmp_version_info);

	struct LANG_AND_CODEPAGE_INFO {
		WORD lang_;
		WORD code_;
	};

	UINT                    xlate_length;
	LANG_AND_CODEPAGE_INFO *xlate_info;


	if ((*query_value_proc_addr)(buffer_ptr, "\\VarFileInfo\\Translation",
		reinterpret_cast<void **>(&xlate_info), &xlate_length) == 0)
		MLB::Utility::ThrowException("Unable to extract the langauge/code-page "
			"translation table from file name '" + file_name + "'.");

	unsigned int      count_1;
	OS_VersionInfoSet tmp_string_map;

	for (count_1 = 0; count_1 < (xlate_length / sizeof(LANG_AND_CODEPAGE_INFO));
		++count_1) {
		if ((lang_code && (xlate_info[count_1].lang_ != lang_code)) ||
			(code_page && (xlate_info[count_1].code_ != code_page)))
			continue;
		std::string sub_block_base("\\StringFileInfo\\" +
			MLB::Utility::ValueToStringHex(xlate_info[count_1].lang_, false) +
			MLB::Utility::ValueToStringHex(xlate_info[count_1].code_, false) +
			"\\");
		unsigned int count_2;
		for (count_2 = 0; count_2 < OS_PredefinedVersionInfoNameCount;
			++count_2) {
			void        *datum_ptr;
			UINT         datum_length;
			std::string  sub_block_spec(sub_block_base +
				std::string(OS_PredefinedVersionInfoNameList[count_2]));
			if ((*query_value_proc_addr)(buffer_ptr,
				const_cast<char *>(sub_block_spec.c_str()), &datum_ptr,
				&datum_length) == 0)
				datum_ptr = "";
			tmp_string_map.insert(OS_VersionInfoKey(
				OS_PredefinedVersionInfoNameList[count_2],
				xlate_info[count_1].lang_, xlate_info[count_1].code_,
				static_cast<char *>(datum_ptr)));
		}
	}

	version_info = tmp_version_info;
	string_map.swap(tmp_string_map);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OS_GetFileVersionInfo(const std::string &file_name, WORD lang_code,
	WORD code_page, VS_FIXEDFILEINFO &version_info,
	OS_VersionInfoSet &string_map)
{
	DWORD  info_size = OS_GetFileVersionInfoSize(file_name);
	double working_buffer[4096];

	//	Not sure whether the length includes any termination...
	if (sizeof(working_buffer) > info_size)
		OS_GetFileVersionInfo(file_name.c_str(), info_size,
			reinterpret_cast<char *>(working_buffer), lang_code, code_page,
			version_info, string_map);
	else {
		boost::shared_array<char> tmp_buffer(new char[info_size + 1]);
		OS_GetFileVersionInfo(file_name.c_str(), info_size, tmp_buffer.get(),
			lang_code, code_page, version_info, string_map);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OS_GetFileVersionInfo(const std::string &file_name,
	VS_FIXEDFILEINFO &version_info, OS_VersionInfoSet &string_map)
{
	OS_GetFileVersionInfo(file_name, 0, 0, version_info, string_map);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/ParseCmdLineArg.hpp>

using namespace MLB::OSSupport;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (MLB::Utility::ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ <file-name> [ <file-name> ... ] ]" <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		std::vector<std::string> file_list;
		if (argc == 1)
			file_list.push_back(argv[0]);
		else
			std::vector<std::string>(argv + 1, argv + argc).swap(file_list);
		std::cout << "Test OS_GetFileVersionInfo():" << std::endl;
		std::vector<std::string>::const_iterator iter_b(file_list.begin());
		std::vector<std::string>::const_iterator iter_e(file_list.end());
		for ( ; iter_b != iter_e; ++iter_b) {
			VS_FIXEDFILEINFO        version_info;
			OS_VersionInfoSet string_map;
			OS_GetFileVersionInfo(*iter_b, version_info, string_map);
			std::cout << "   " << *iter_b << ": " << std::endl;
			OS_VersionInfoSetIterC iter_m_b(string_map.begin());
			OS_VersionInfoSetIterC iter_m_e(string_map.end());
			for ( ; iter_m_b != iter_m_e; ++iter_m_b)
				std::cout << "   >>> " << std::left << std::setw(20) <<
					iter_m_b->info_name_ << std::right << " " <<
					std::setw(5) << iter_m_b->lang_code_ << " " <<
					std::setw(5) << iter_m_b->code_page_ << " " <<
					iter_m_b->info_value_ << std::endl;
		}
		std::cout << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << "REGRESSION TEST FAILURE: " <<
			"OSSupport/Win/OS_GetFileVersionInfo.cpp: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

