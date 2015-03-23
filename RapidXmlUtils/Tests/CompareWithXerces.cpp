// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB RapidXmlUtils Test Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	CompareWithXerces.cpp

	File Description	:	Compare parsing of XML files with the RapidXmlUtils
								library with the XercesUtils library.

	Revision History	:	2015-03-22 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2015 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RapidXmlUtils/RapidXmlContext.hpp>
#include <RapidXmlUtils/XmlDomElement.hpp>

#include <XercesUtils/XercesContext.hpp>
#include <XercesUtils/XmlDomElement.hpp>

#include <Utility/ParseCmdLineArg.hpp>
#include <Utility/InlineContainer.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/Utility_Exception.hpp>

#include <algorithm>
#include <iomanip>
#include <iostream>

// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
void EmitSep(char sep_char = '-')
{
	std::cout << std::setfill(sep_char) << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void RunComparisonVisAVisXercesUtils(int argc, char **argv)
{
	using namespace MLB::Utility;

	MLB::XercesUtils::XercesContext     xerces_context;
	MLB::RapidXmlUtils::RapidXmlContext rapidxml_context;

	if (ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " <xml-file-name> [ <xml-file-name> ... ]" <<
			std::endl << std::endl;
		return;
	}

	if (argc < 2)
		ThrowInvalidArgument("No XML files were specified.");

	for (int count_1 = 1; count_1 < argc; ++count_1) {
		EmitSep('=');
		std::cout << "File: " << argv[count_1] << std::endl;
		EmitSep('-');
		const char *parse_type = "XercesUtils";
		MLB::XercesUtils::XmlDomElement   xerces_element;
		MLB::RapidXmlUtils::XmlDomElement rapidxml_element;
		try {
			MLB::XercesUtils::XmlDomElement::ParseXmlFile(argv[count_1],
				xerces_element);
			parse_type = "RapidXmlUtils";
			MLB::RapidXmlUtils::XmlDomElement::ParseXmlFile(argv[count_1],
				rapidxml_element);
		}
		catch (const std::exception &except) {
			std::cout << "Attempt to parse file '" << argv[count_1] <<
				"' using the MLB::" << parse_type << " library failed: " <<
				except.what() << std::endl;
			continue;
		}
		std::ostringstream o_str_xerces;
		std::ostringstream o_str_rapidxml;
		xerces_element.ConcatElementTree(o_str_xerces, 0);
		rapidxml_element.ConcatElementTree(o_str_rapidxml, 0);
		if (o_str_xerces.str() == o_str_rapidxml.str()) {
			std::cout << "Processing of XML is identical for file '" <<
				argv[count_1] << "'." << std::endl;
			continue;
		}
		std::cout << "Processing of XML differs for file '" << argv[count_1] <<
			"'." << std::endl;
		EmitSep('-');
		StringVector string_list_xerces(GetSplit(o_str_xerces.str(), "\n"));
		StringVector string_list_rapidxml(GetSplit(o_str_rapidxml.str(), "\n"));
		std::pair<StringVectorIter, StringVectorIter> mismatch_location(
			std::mismatch(string_list_xerces.begin(), string_list_xerces.end(),
			string_list_rapidxml.begin()));
		if (mismatch_location.first == string_list_xerces.end()) {
			std::ostringstream o_str;
			o_str << "Comparison of strings indicates a difference between the "
				"generated output for file '" << argv[count_1] << "' when using "
				"the MLB::XercesUtils library and the MLB::RapidXmlUtils library, "
				"but std::mismatch() shows equivalent std::vectors of "
				"std::strings to be identical.";
			throw std::logic_error(o_str.str());
		}
		std::size_t line_index = static_cast<std::size_t>(
			mismatch_location.first - string_list_xerces.begin());
		for ( ; (mismatch_location.first != string_list_xerces.end()) &&
			(mismatch_location.second != string_list_rapidxml.end());
			++line_index, ++mismatch_location.first, ++mismatch_location.second) {
			if (*mismatch_location.first !=  *mismatch_location.second)
				std::cout << std::setw(8) << line_index << ": [" <<
					*mismatch_location.first << "] ---> [" <<
					*mismatch_location.second << "]" << std::endl;
		}
		EmitSep('=');
		std::cout << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		RunComparisonVisAVisXercesUtils(argc, argv);
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

