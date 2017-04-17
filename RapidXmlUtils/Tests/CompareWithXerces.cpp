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

		Copyright Michael L. Brock 2015 - 2017.
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
void RunInternalStringTests(const char *xml_string)
{
	EmitSep('=');
	std::cout << "Internal Test String:" << std::endl;
	std::cout << "-------- ---- -------" << std::endl;
	std::cout << xml_string << std::endl;
	EmitSep('-');

	MLB::XercesUtils::XmlDomElement   xerces_element(
		MLB::XercesUtils::XmlDomElement::ParseXmlString(xml_string));
	MLB::RapidXmlUtils::XmlDomElement rapidxml_element(
		MLB::RapidXmlUtils::XmlDomElement::ParseXmlString(xml_string));

	std::ostringstream o_str_xerces;
	std::ostringstream o_str_rapidxml;

	xerces_element.ConcatElementTree(o_str_xerces);
	rapidxml_element.ConcatElementTree(o_str_rapidxml);

	std::cout << "Xerces and RapidXml parses are ";
	if (o_str_xerces.str() == o_str_rapidxml.str())
		std::cout << "identical." << std::endl;
	else {
		std::cout << "NOT identical." << std::endl;
		EmitSep('-');
		std::cout << o_str_xerces.str();
	}

	EmitSep('-');
	std::cout << o_str_rapidxml.str();

	EmitSep('=');
	std::cout << std::endl;
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

	//	Internal testing...
	{
		const char *xml_string = "<top><!-- Comment 1 -->"
			"   <middle a=\"1\" b=\"2\" c=\"3\"></middle>"
			"\t<!--\n\t\tComment 2\n\t-->\n</top>";
		RunInternalStringTests(xml_string);
	}
	{
		const char *xml_string = "<!-- Comment 1 --><top><!-- Comment 2 -->"
			"   <middle a=\"1\" b=\"2\" c=\"3\"></middle>"
			"\t<!--\n\t\tComment 3\n\t-->\n</top>";
		RunInternalStringTests(xml_string);
	}
	{
		const char *xml_string = "<!-- Comment 1 --><!-- Comment 2 -->"
			"<top><!-- Comment 3 -->"
			"   <middle a=\"1\" b=\"2\" c=\"3\"></middle>"
			"\t<!--\r\n\t\tComment 4\r\n\t-->\n</top><!-- Comment 5 -->"
			"<!-- Comment 6 -->";
		RunInternalStringTests(xml_string);
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
			std::cout << "Attempt to parse a file using the MLB::" <<
				parse_type << " library failed: " << except.what() << std::endl;
			EmitSep('=');
			std::cout << std::endl;
			continue;
		}
		std::ostringstream o_str_xerces;
		std::ostringstream o_str_rapidxml;
		xerces_element.ConcatElementTree(o_str_xerces, 0);
		rapidxml_element.ConcatElementTree(o_str_rapidxml, 0);
		if (o_str_xerces.str() == o_str_rapidxml.str()) {
			std::cout << "Processing of XML is identical for file '" <<
				argv[count_1] << "'." << std::endl;
			EmitSep('=');
			std::cout << std::endl;
			continue;
		}
		std::cout << "Processing of XML differs for file '" << argv[count_1] <<
			"'." << std::endl;
		EmitSep('-');
		StringVector string_list_xerces(GetSplit(o_str_xerces.str(), "\n"));
		StringVector string_list_rapidxml(GetSplit(o_str_rapidxml.str(), "\n"));
		//	Need C++14 in order to use the new std::mismatch() overloads...
		StringVectorIterC iter_x_b(string_list_xerces.begin());
		StringVectorIterC iter_x_e(string_list_xerces.end());
		StringVectorIterC iter_r_b(string_list_rapidxml.begin());
		StringVectorIterC iter_r_e(string_list_rapidxml.end());
		std::size_t       line_index = 0;
		for ( ; (iter_x_b != iter_x_e) && (iter_r_b != iter_r_e);
			++line_index, ++iter_x_b, ++iter_r_b) {
			if (*iter_x_b != *iter_r_b)
				std::cout << std::setw(8) << line_index << ": [" <<
					*iter_x_b << "] ---> [" << *iter_r_b << "]" << std::endl;
		}
		if ((iter_x_b != iter_x_e) || (iter_r_b != iter_r_e)) {
			StringVectorIterC iter_b;
			StringVectorIterC iter_e;
			if (iter_x_b != iter_x_e) {
				iter_b = iter_x_b;
				iter_e = iter_x_e;
			}
			else {
				iter_b = iter_r_b;
				iter_e = iter_r_e;
			}
			for ( ; iter_b != iter_e; ++line_index, ++iter_b) {
				std::cout << std::setw(8) << line_index << ": ";
				if (iter_x_b != iter_x_e)
					std::cout << "[" << *iter_b << "] ---> ***NON-EXISTENT***" <<
						std::endl;
				else
					std::cout << "***NON-EXISTENT*** ---> [" << *iter_b << "]" <<
						std::endl;
			}
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

