// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Program
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of a Tib/Rv program for sending XML
								data within the field 'xmlData'.

	Revision History	:	2005-07-04 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX/XRvSendControl.hpp>

#include <Utility/PathName.hpp>

// ////////////////////////////////////////////////////////////////////////////

using namespace MLB::RvUtilX;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		XRvSendControl send_control;
		if (!send_control.CheckCmdLineForHelpRequest(argc, argv)) {
			//	Set up defaults for Polaris Xml sends...
			send_control.field_name_ = "xmlData";
			send_control.field_type_ = TIBRVMSG_XML;
			send_control.ParseCmdLine(argc, argv);
			send_control.SendMessage();
		}
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << MLB::Utility::GetFileNamePortion(argv[0]) <<
			": fatal error encountered: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

