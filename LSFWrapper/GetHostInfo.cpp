// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB LSFWrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Support for LSF functionality 'ls_gethostinfo()'.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/C_StringSupport.hpp>

#include <LSFWrapper/LSFWrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LSFWrapper {

//	////////////////////////////////////////////////////////////////////////////
struct hostInfo *LSF_gethostinfo(const char *resreq, unsigned int *numhosts,
	char **hostlist, unsigned int listsize, int options)
{
	struct hostInfo *result;

	LSFWrapper_THROW_IF_NULL(result, ls_gethostinfo,
		(const_cast<char *>(resreq), reinterpret_cast<int *>(numhosts),
		hostlist, listsize, options));

	if ((*numhosts && (result == NULL)) || ((!(*numhosts)) && (result != NULL))){
		std::ostringstream error_text;
		error_text << "Platform/LSF function 'ls_gethostinfo()' indicates " <<
			"that " << *numhosts << " 'hostInfo' structures were returned, but " <<
			"the pointer to that array of structures is " <<
			((result == NULL) ? "" : "not ") << "'NULL'.";
		throw LSFException(error_text);
	}

	return(result);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void GetHostInfo(LSF_HostInfoVector &out_list, int host_info_options,
	const std::string &resource_requirements,
	const HostList &candidate_host_list)
{
	struct hostInfo *result_list;
	unsigned int     result_count;

	if (candidate_host_list.empty())
		result_list = LSF_gethostinfo(
			(resource_requirements.empty()) ? NULL : resource_requirements.c_str(),
			&result_count,
			NULL,
			0,
			host_info_options);
	else {
		// Allocate a temporary list of candidate hosts.
		MLB::Utility::C_ListTypeString<int> tmp_candidate_host_list;
		tmp_candidate_host_list.FromContainer(candidate_host_list,
			"Creation of temporary candidate host name list for the invocation "
			"of 'LSFWrapper::LSF_gethostinfo()'");
		result_list = LSF_gethostinfo(
			(resource_requirements.empty()) ? NULL : resource_requirements.c_str(),
			&result_count,
			tmp_candidate_host_list.GetList(),
			tmp_candidate_host_list.GetCount(),
			host_info_options);
	}

	LSF_HostInfoVector tmp_list;

	tmp_list.reserve(result_count);

	while (result_count--)
		tmp_list.push_back(*result_list++);

	out_list.swap(tmp_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LSF_HostInfoVector GetHostInfo(int host_info_options,
	const std::string &resource_requirements,
	const HostList &candidate_host_list)
{
	LSF_HostInfoVector out_list;

	GetHostInfo(out_list, host_info_options, resource_requirements,
		candidate_host_list);

	return(out_list);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace LSFWrapper

} // namespace MLB

#include <iomanip>

#ifdef TEST_MAIN

using namespace MLB::LSFWrapper;

//	////////////////////////////////////////////////////////////////////////////
static void TEST_GetHostInfo()
{
	LSF_HostInfoVector host_info_list = GetHostInfo();

	LSF_HostInfoVector::const_iterator iter_b(host_info_list.begin());
	LSF_HostInfoVector::const_iterator iter_e(host_info_list.end());

	while (iter_b != iter_e) {
		std::cout << *iter_b << std::endl;
		++iter_b;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	std::cout << "Test of function 'GetHostInfo()'" << std::endl;
	std::cout << "---- -- -------- ---------------" << std::endl;

	try {
		TEST_GetHostInfo();
	}
	catch (const std::exception &except) {
		std::cout << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

