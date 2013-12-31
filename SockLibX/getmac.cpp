/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	Network Programming Support Library (NPSL) Source Module						*/
/*	***********************************************************************	*/
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Determines the machine Ethernet Media Access Control
								(MAC) address.

	Revision History	:	1994-02-25 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1994 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*		Necessary include files . . .														*/
/*	***********************************************************************	*/

#include <Utility/C_StringSupport.hpp>
#include <Utility/MACAddress.hpp>

#include "npsli.h"

/*	***********************************************************************	*/

/*	***********************************************************************	*/
int NPSL_GetMAC(NPSL_ADDR_MAC *ethernet_address, char *error_text)
{
	int return_code;

	try {
		MLB::Utility::MACAddress tmp_mac(
			MLB::Utility::MACAddress::GetMyMACAddress());
		MLB::Utility::C_memcpy(ethernet_address, tmp_mac.octet,
			sizeof(*ethernet_address), "NPSL_GetMAC");
		return_code = NPSL_SUCCESS;
	}
	catch (const std::exception &except) {
		MLB::Utility::nstrcpy(error_text, except.what(),
			NPSL_MAX_ERROR_TEXT - 1);
		return_code = NPSL_SYSTEM_FAILURE;
	}

	return(return_code);
}
/*	***********************************************************************	*/

#ifdef TEST_MAIN

COMPAT_FN_DECL(int main, (void));

int main()
{
	int           return_code;
	NPSL_ADDR_MAC ethernet_address;
	char          error_text[NPSL_MAX_ERROR_TEXT];

	fprintf(stderr, "Test harness for function 'NPSL_GetMAC()'\n");
	fprintf(stderr, "---- ------- --- -------- ---------------\n\n");

	if ((return_code = NPSL_GetMAC(&ethernet_address, error_text)) ==
		NPSL_SUCCESS)
		printf("Ethernet Address: %02X-%02X-%02X-%02X-%02X-%02X\n",
			ethernet_address.octet[0],
			ethernet_address.octet[1],
			ethernet_address.octet[2],
			ethernet_address.octet[3],
			ethernet_address.octet[4],
			ethernet_address.octet[5]);
	else
		fprintf(stderr, "ERROR: %s\n", error_text);

	return(return_code);
}

#endif /* #ifdef TEST_MAIN */

