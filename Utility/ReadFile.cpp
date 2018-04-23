//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of logic for the reading of operating
								system file contents.

	Revision History	:	1993-10-02 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility_Exception.hpp>

#include <fstream>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
std::string &ReadFileData(const std::string &file_name, std::string &file_data)
{
	try {
		std::ifstream in_file(file_name.c_str(), std::ios_base::binary);
		if (in_file.fail())
			MLB::Utility::ThrowStdException("Open failed.");
		in_file.seekg(0, std::ios_base::end);
		if (in_file.fail())
			MLB::Utility::ThrowStdException("Attempt to seek to the end of the "
				"file failed.");
/*
		// CODE NOTE: Was using this with MSVC 2003...
		std::ifstream::streamoff file_size = in_file.tellg();
*/
		std::ifstream::pos_type file_size = in_file.tellg();
		if (in_file.fail())
			MLB::Utility::ThrowStdException("Attempt to determine the end of "
				"file position failed.");
		in_file.seekg(0, std::ios_base::beg);
		if (in_file.fail())
			MLB::Utility::ThrowStdException("Attempt to seek to the beginning "
				"of the file failed.");
		if (file_size > 0) {
			std::string tmp_file_data(static_cast<std::size_t>(file_size), '\0');
			//	Casting away of constness is safe because no changes to the string
			//	can occur over duration of the call...
			in_file.read(const_cast<char *>(tmp_file_data.c_str()), file_size);
			if (in_file.fail())
				MLB::Utility::ThrowStdException("Attempt to read " +
					MLB::Utility::AnyToString(file_size) + " bytes failed.");
			file_data.swap(tmp_file_data);
		}
		else
			file_data.clear();
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to read contents of file '" +
			file_name + "': " + std::string(except.what()));
	}

	return(file_data);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string ReadFileData(const std::string &file_name)
{
	std::string file_data;

	return(ReadFileData(file_name, file_data));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int ReadFileLines(const std::string &file_name,
	StringVector &file_lines)
{
	try {
		std::ifstream in_file(file_name.c_str());
		if (in_file.fail())
			MLB::Utility::ThrowStdException("Open failed.");
		StringVector tmp_file_lines;
		while (!in_file.eof()) {
			std::string this_line;
			std::getline(in_file, this_line);
			tmp_file_lines.push_back(this_line);
		}
		file_lines.swap(tmp_file_lines);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Unable to read contents of file '" +
			file_name + "': " + std::string(except.what()));
	}

	return(static_cast<unsigned int>(file_lines.size()));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

