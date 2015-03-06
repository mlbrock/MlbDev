// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Program
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Writes a batch file to stdout which contains path names
								resolve to their UNC equivalents. For use under the
								Microsoft Windows environment.

	Revision History	:	2001-10-17 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2015.
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
#include <Utility/PathName.hpp>
#include <Utility/ParseCmdLineArg.hpp>
#include <Utility/InlineContainer.hpp>

#include <iomanip>

// ////////////////////////////////////////////////////////////////////////////

using namespace MLB::Utility;

// ////////////////////////////////////////////////////////////////////////////
const char DefaultEnvName[] = "NO_ENV_SPECIFIED";
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int          return_code       = EXIT_SUCCESS;
	std::string  my_name           = MLB::Utility::GetFileNamePortion(argv[0]);
	unsigned int env_count         = 0;
	bool         must_exist_flag   = false;
	bool         must_be_unc_flag  = false;
	bool         is_dir_flag       = false;
	bool         is_file_flag      = false;
	bool         continue_flag     = false;
	bool         cmnt_wrapper_flag = true;
	bool         cmnt_value_flag   = true;
	std::string  base_dir("./");
	std::string  this_name(DefaultEnvName);
	std::string  this_value;
	

	// //////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////
	//	Help may have been requested on the command line...
	// //////////////////////////////////////////////////////////////////////
	if (ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			" -name <environment-variable-name>  ... " <<
			" -value <environment-variable-value>  ... " <<
			"[ -working_dir <path-to-use-as-the-working-directory> ( = ./ ) ] " <<
			"[ -exists <boolean> ( = false ) ] " <<
			"[ -unc <boolean> ( = false ) ] " <<
			"[ -dir <boolean> ( = false ) ] " <<
			"[ -file <boolean> ( = false ) ] " <<
			"[ -continue <boolean> ( = false ) ] " <<
			"[ -comment_wrapper <boolean> ( = true ) ] " <<
			"[ -comment_value <boolean> ( = true ) ]\n\n" <<
"\
   -help\n\
      Produces this output.\n\n\
   -name <environment-variable-name>\n\
      The environment variable name which will be emitted.\n\n\
   -value <environment-variable-value> ...\n\
      The path name to be used as the environment variable value.\n\n\
   -working_dir <path-to-use-as-the-working-directory> ( = ./ )\n\
      The directory to use as the current working directory.\n\n\
   -exists <boolean> ( = false )\n\
      Used to indicate whether the environment variable value specified by\n\
      the '-value' parameter must exist.\n\n\
   -unc <boolean> ( = false )\n\
      Used to indicate whether the environment variable value specified by\n\
      the '-value' parameter must resolve to a UNC name.\n\n\
   -dir <boolean> ( = false )\n\
      Used to indicate whether the environment variable value specified by\n\
      the '-value' parameter must be a directory.\n\n\
   -file <boolean> ( = false )\n\
      Used to indicate whether the environment variable value specified by\n\
      the '-value' parameter must be a file.\n\n\
   -continue <boolean> ( = false )\n\
      Specifies whether program operation is to continue after an error is\n\
      encountered.\n\n\
   -comment_wrapper <boolean> ( = true )\n\
      Specifies whether program output is to be wrapped in comment lines.\n\
      Note that this parameter must be the first on the command line in order\n\
      to be effective.\n\n\
   -comment_value <boolean> ( = true )\n\
      Specifies whether each resolved name/value action should be commented." <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}
	// //////////////////////////////////////////////////////////////////////

	try {
		base_dir = PathNameToGeneric(base_dir, "", false, true, false);
		// ////////////////////////////////////////////////////////////////
		// ////////////////////////////////////////////////////////////////
		//	Look to see if the comment wrapper parameter is specified as
		//	the first parameter...
		// ////////////////////////////////////////////////////////////////
		if (argc > 2) {
			unsigned int this_index = 1;
			ParseCmdLineArg::ParseCmdLineDatumSpec(
				MLB::Utility::MakeInlineVector<std::string>
					("-COMMENT_WRAPPERS")
					("-COMMENTWRAPPERS")
					("-COMMENT_WRAPPER")
					("-COMMENTWRAPPER"),
				this_index, argc, argv, cmnt_wrapper_flag);
		}
		// ////////////////////////////////////////////////////////////////
		// ////////////////////////////////////////////////////////////////
		//	Emit a little info...
		// ////////////////////////////////////////////////////////////////
		if (cmnt_wrapper_flag) {
			std::cout << ":: " << std::setfill(':') << std::setw(76) << "" <<
				std::setfill(' ') << std::endl;
			std::cout << ":: " << std::setfill(':') << std::setw(76) << "" <<
				std::setfill(' ') << std::endl;
			std::cout << ":: CATS Batch File for UNC Environment Variables" <<
				std::endl;
			std::cout << std::endl;
			std::cout << ":: Created     : " << TimeVal() << " (UTC)" << std::endl;
			MLB::Utility::StringVector argv_list;
			MLB::Utility::CharPtrArrayToStringContainer(
				static_cast<unsigned int>(argc), argv, argv_list,
				"Failure in constructiong 'argv' list");
			std::cout << ":: Command Line: ";
			std::copy(argv_list.begin(), argv_list.end(),
				std::ostream_iterator<MLB::Utility::StringVector::value_type>
				(std::cout, " "));
			std::cout << std::endl;
			std::cout << ":: " << std::setfill(':') << std::setw(76) << "" <<
				std::setfill(' ') << std::endl;
		}
		// ////////////////////////////////////////////////////////////////
		// ////////////////////////////////////////////////////////////////
		//	Process the command line parameters...
		// ////////////////////////////////////////////////////////////////
		unsigned int count_1;
		std::string  unc_name;
		std::string  tmp_base_dir;
		for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
			try {
				if (ParseCmdLineArg::ParseCmdLineDatum("-NAME", count_1, argc,
					argv, this_name)) {
					if (this_name.empty()) {
						this_name = DefaultEnvName;
						ThrowInvalidArgument("Environment variable name is empty.");
					}
				}
				else if (ParseCmdLineArg::ParseCmdLineDatum("-VALUE", count_1,
					argc, argv, this_value)) {
					if (this_value.empty())
						ThrowInvalidArgument("Environment variable value is empty.");
					std::string result_path(PathNameToGeneric(this_value, base_dir,
						must_exist_flag, is_dir_flag, is_file_flag));
					if (must_be_unc_flag && ((result_path.size() < 3) ||
						(result_path.substr(0, 2) != "\\\\")))
						ThrowInvalidArgument("Resolution of the command line "
							"path '" + std::string(argv[count_1]) +
							"' resulted in the generic path '" + result_path +
							"', which is not a valid UNC path as was specified by "
							"a previously-encountered '-UNC' parameter.");
					std::cout << std::endl;
					if (cmnt_value_flag) {
						std::cout << ":: Conversion From  : " << this_value <<
							std::endl;
						std::cout << ":: Must Exists Flag : " <<
							AnyToString(must_exist_flag) << std::endl;
						std::cout << ":: Must Be UNC Flag : " <<
							AnyToString(must_be_unc_flag) << std::endl;
						std::cout << ":: Is Directory Flag: " <<
							AnyToString(is_dir_flag) << std::endl;
						std::cout << ":: Is File Flag     : " <<
							AnyToString(is_file_flag) << std::endl;
					}
					std::cout << "set " << this_name << "=" << result_path <<
						std::endl;
					++env_count;
				}
				else if (ParseCmdLineArg::ParseCmdLineFollowingSpec(
					MLB::Utility::MakeInlineVector<std::string>
						("-WORKING_DIRECTORY")
						("-WORKING_DIR")
						("-WORK_DIRECTORY")
						("-WORK_DIR"), count_1, argc, argv, tmp_base_dir)) {
					if (tmp_base_dir.empty())
						ThrowInvalidArgument("The working directory parameter is "
							"empty.");
					std::string result_path(PathNameToGeneric(tmp_base_dir, "",
						must_exist_flag, true, false));
					if (cmnt_value_flag) {
						std::cout << std::endl;
						std::cout << ":: Changed base directory from: " <<
							base_dir << std::endl;
						std::cout << ":: Changed base directory to  : " <<
							result_path << std::endl;
					}
					base_dir = result_path;
				}
				else if (ParseCmdLineArg::ParseCmdLineDatum("-EXISTS", count_1,
					argc, argv, must_exist_flag))
					;
				else if (ParseCmdLineArg::ParseCmdLineDatum("-UNC", count_1,
					argc, argv, must_be_unc_flag))
					;
				else if (ParseCmdLineArg::ParseCmdLineDatum("-DIR", count_1,
					argc, argv, is_dir_flag))
					;
				else if (ParseCmdLineArg::ParseCmdLineDatum("-FILE", count_1,
					argc, argv, is_file_flag))
					;
				else if (ParseCmdLineArg::ParseCmdLineDatum("-CONTINUE", count_1,
					argc, argv, continue_flag))
					;
				else if (ParseCmdLineArg::ParseCmdLineDatumSpec(
					MLB::Utility::MakeInlineVector<std::string>
					("-COMMENT_WRAPPERS")
					("-COMMENTWRAPPERS")
					("-COMMENT_WRAPPER")
					("-COMMENTWRAPPER"),
					count_1, argc, argv, cmnt_wrapper_flag))
					;
				else if (ParseCmdLineArg::ParseCmdLineDatumSpec(
					MLB::Utility::MakeInlineVector<std::string>
					("-COMMENT_VALUES")
					("-COMMENTVALUES")
					("-COMMENT_VALUE")
					("-COMMENTVALUE"),
					count_1, argc, argv, cmnt_value_flag))
					;
				else
					ParseCmdLineArg::InvalidArgument(argv[count_1]);
			}
			catch (const std::exception &except) {
				if (!continue_flag)
					throw;
				std::cerr << std::endl << std::endl << my_name <<
					": INTERMEDIATE ERROR ENCOUNTERED: " << except.what() <<
					std::endl;
				return_code = EXIT_FAILURE;
			}
		}
		std::cout << std::endl;
		if (cmnt_wrapper_flag) {
			std::cout << ":: " << std::setfill(':') << std::setw(76) << "" <<
				std::setfill(' ') << std::endl;
			std::cout << ":: End of Batch File" << std::endl;
			std::cout << ":: " << std::setfill(':') << std::setw(76) << "" <<
				std::setfill(' ') << std::endl;
			std::cout << ":: " << std::setfill(':') << std::setw(76) << "" <<
				std::setfill(' ') << std::endl;
		}
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << std::endl << my_name <<
			": FATAL ERROR ENCOUNTERED: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

