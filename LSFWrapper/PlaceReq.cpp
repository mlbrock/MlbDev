// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB LSFWrapper Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Support for LSF functionality 'ls_placereq()'.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2016.
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
char **LSF_placereq(const char *resreq, unsigned int *num,
	int options, const char *fromhost)
{
	char **result;

	LSFWrapper_THROW_IF_NULL(result, ls_placereq,
		(const_cast<char *>(resreq), reinterpret_cast<int *>(num), options,
		const_cast<char *>(fromhost)));

	if ((*num && (result == NULL)) || ((!(*num)) && (result != NULL))){
		std::ostringstream number_text;
		if (num == NULL)
			number_text << "(NULL = 1 host)";
		else if (!(*num))
			number_text << "(0 = all available hosts)";
		else
			number_text << *num;
		std::ostringstream error_text;
		error_text << "Platform/LSF function 'ls_placereq()' indicates that " <<
			number_text << " hosts were returned, but the pointer to that array "
			"of structures is " << ((result == NULL) ? "" : "not ") << "'NULL'.";
		throw LSFException(error_text);
	}

	return(result);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void PlaceReq(HostList &out_list, const std::string &resource_requirements,
	unsigned int number_of_hosts, int place_req_options,
	const std::string &from_host_name)
{
	char **result_list  = NULL;

	result_list = LSF_placereq(
		(resource_requirements.empty()) ? NULL : resource_requirements.c_str(),
		&number_of_hosts,
		place_req_options,
		(from_host_name.empty()) ? NULL : from_host_name.c_str());

	HostList tmp_list;

	MLB::Utility::CharPtrArrayToStringContainer(number_of_hosts, result_list,
		tmp_list);

	out_list.swap(tmp_list);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
HostList PlaceReq(const std::string &resource_requirements,
	unsigned int number_of_hosts, int place_req_options,
	const std::string &from_host_name)
{
	HostList out_list;

	PlaceReq(out_list, resource_requirements, number_of_hosts, place_req_options,
		from_host_name);

	return(out_list);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace LSFWrapper

} // namespace MLB

#include <iomanip>

#ifdef TEST_MAIN

using namespace MLB::LSFWrapper;

//	////////////////////////////////////////////////////////////////////////////
static void TEST_PlaceReq()
{
	HostList host_list = PlaceReq("", 1);

	HostList::const_iterator iter_b(host_list.begin());
	HostList::const_iterator iter_e(host_list.end());

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

	std::cout << "Test of function 'PlaceReq()'" << std::endl;
	std::cout << "---- -- -------- ------------" << std::endl;

	try {
		TEST_PlaceReq();
	}
	catch (const std::exception &except) {
		std::cout << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

