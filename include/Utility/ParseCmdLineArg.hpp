// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	The ParseCmdLineArg class include file.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility_ParseCmdLineArg_hpp__HH

#define HH__MLB__Utility_ParseCmdLineArg_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file ParseCmdLineArg.hpp

	\brief	The header for the MLB::Utility::ParseCmdLineArg class.

	Contains the ParseCmdLineArg class definition.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/C_StringSupport.hpp>
#include <Utility/TimeSupport.hpp>
#include <Utility/UniqueId.hpp>
#include <Utility/ParseNumericString.hpp>
#include <Utility/Utility_Exception.hpp>

#if _MSC_VER >= 1500
# pragma warning(disable:4242 4347 4571)
#endif // # if _MSC_VER >= 1500

#ifdef __GNUC__
# include <stdlib.h>
# include <string.h>
#endif // #ifdef __GNUC__

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
struct API_UTILITY ParseCmdLineArg {
	static bool HasCmdLineHelp(int argc, char **argv, int first_index = 1);
	static bool HasCmdLineArg(const char *arg_name, int argc, char **argv,
		int first_index = 1);
	static int  FindCmdLineArg(const char *arg_name, int argc, char **argv,
		int first_index = 1);
	static bool IsCmdLineArg(const char *param_name, unsigned int current_index,
		int argc, char **argv);
	static bool IsCmdLineArg(const std::string &param_name,
		unsigned int current_index, int argc, char **argv);
	static bool ParseDirectory(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv,
		std::string &out_datum, const std::string &base_path = "",
		bool require_existence = true);
	static bool ParseWorkingDir(unsigned int &current_index, int argc,
		char **argv, std::string &out_datum, const std::string &base_path = "",
		bool require_existence = true);
	static bool ParseLoggingDir(unsigned int &current_index, int argc,
		char **argv, std::string &out_datum, const std::string &base_path = "",
		bool require_existence = true);
	static bool ParseLoggingFlag(unsigned int &current_index, int argc,
		char **argv, bool &out_datum);
	template <typename ParseType>
		static bool ParseCmdLineNumeric(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv, ParseType &out_datum,
		ParseType min_value = NumericBounds<ParseType>::min(),
		ParseType max_value = NumericBounds<ParseType>::max()) {
		std::string tmp_string;
		if (!ParseCmdLineFollowingNotEmpty(param_name.c_str(), current_index,
			argc, argv, tmp_string))
			return(false);
		try {
			ParseCmdLineNumeric(argv[current_index], out_datum, min_value,
				max_value);
		}
		catch (const std::exception &except) {
			std::ostringstream error_text;
			error_text << "The specified value for parameter '" <<
				argv[current_index - 1] << "' is in error: " << except.what();
			Rethrow(except, error_text);
		}
		return(true);
	}
	template <typename ParseType>
		static bool ParseCmdLineNumeric(const char *param_value,
		ParseType &out_datum,
		ParseType min_value = NumericBounds<ParseType>::min(),
		ParseType max_value = NumericBounds<ParseType>::max()) {
		return(ParseNumericString(param_value, out_datum, true,
			min_value, max_value));
	}
	template <typename ParseType>
		static bool ParseCmdLineNumeric(const std::string &param_value,
		ParseType &out_datum,
		ParseType min_value = NumericBounds<ParseType>::min(),
		ParseType max_value = NumericBounds<ParseType>::max()) {
		return(ParseCmdLineNumeric(param_value.c_str(), out_datum, min_value,
			max_value));
	}
	template <typename  ContainerType, typename ParseType>
		static bool ParseCmdLineNumeric(const ContainerType &spec_cont,
		unsigned int &current_index, int argc, char **argv, ParseType &out_datum,
		ParseType min_value = NumericBounds<ParseType>::min(),
		ParseType max_value = NumericBounds<ParseType>::max()) {
		typename ContainerType::const_iterator iter_b(spec_cont.begin());
		typename ContainerType::const_iterator iter_e(spec_cont.end());
		while (iter_b != iter_e) {
			if (ParseCmdLineNumeric(*iter_b, current_index, argc, argv, out_datum,
				min_value, max_value))
				return(true);
			++iter_b;
		}
		return(false);
	}
	template <typename ParseType>
		static bool ParseCmdLineDatum(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv, ParseType &out_datum){
		std::string tmp_string;
		if (!ParseCmdLineFollowingNotEmpty(param_name.c_str(), current_index,
			argc, argv, tmp_string))
			return(false);
		try {
			ParseCmdLineDatum(argv[current_index], out_datum);
		}
		catch (const std::exception &except) {
			std::ostringstream error_text;
			error_text << "The specified value for parameter '" <<
				argv[current_index - 1] << "' is in error: " << except.what();
			Rethrow(except, error_text);
		}
		return(true);
	}

	//	Assumes a container of values convertible to std::string...
	template <typename  ContainerType, typename ParseType>
		static bool ParseCmdLineDatumSpec(const ContainerType &spec_cont,
		unsigned int &current_index, int argc, char **argv,
		ParseType &out_datum) {
		typename ContainerType::const_iterator iter_b(spec_cont.begin());
		typename ContainerType::const_iterator iter_e(spec_cont.end());
		while (iter_b != iter_e) {
			if (ParseCmdLineDatum(*iter_b, current_index, argc, argv, out_datum))
				return(true);
			++iter_b;
		}
		return(false);
	}

	//	Assumes a container of values convertible to std::string...
	template <class ContainerType>
		static bool ParseCmdLineFollowingSpec(const ContainerType &spec_cont,
		unsigned int &current_index, int argc, char **argv,
		std::string &out_datum) {
		typename ContainerType::const_iterator iter_b(spec_cont.begin());
		typename ContainerType::const_iterator iter_e(spec_cont.end());
		while (iter_b != iter_e) {
			if (ParseCmdLineFollowing(*iter_b, current_index, argc, argv,
				out_datum))
				return(true);
			++iter_b;
		}
		return(false);
	}

   static bool ParseCmdLineDatum(const std::string &param_value,
		bool &out_datum);
	static bool ParseCmdLineDatum(const std::string &param_value,
		signed char &out_datum);
	static bool ParseCmdLineDatum(const std::string &param_value,
		unsigned char &out_datum);
	static bool ParseCmdLineDatum(const std::string &param_value,
		signed short &out_datum);
	static bool ParseCmdLineDatum(const std::string &param_value,
		unsigned short &out_datum);
	static bool ParseCmdLineDatum(const std::string &param_value,
		signed int &out_datum);
	static bool ParseCmdLineDatum(const std::string &param_value,
		unsigned int &out_datum);
	static bool ParseCmdLineDatum(const std::string &param_value,
		signed long &out_datum);
	static bool ParseCmdLineDatum(const std::string &param_value,
		unsigned long &out_datum);
	static bool ParseCmdLineDatum(const std::string &param_value,
		signed long long &out_datum);
	static bool ParseCmdLineDatum(const std::string &param_value,
		unsigned long long &out_datum);
	static bool ParseCmdLineDatum(const std::string &param_value,
		float &out_datum);
	static bool ParseCmdLineDatum(const std::string &param_value,
		double &out_datum);
   static bool ParseCmdLineDatum(const std::string &param_value,
		std::string &out_datum);
   static bool ParseCmdLineDatum(const std::string &param_value,
		UniqueId &out_datum);
   static bool ParseCmdLineDatum(const std::string &param_value,
		TimeT &out_datum);
   static bool ParseCmdLineDatum(const std::string &param_value,
		TimeVal &out_datum);
   static bool ParseCmdLineDatum(const std::string &param_value,
		TimeSpec &out_datum);

	static bool ParseCmdLineFollowing(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv,
		std::string &out_datum);
	static bool ParseCmdLineFollowing(const char *param_name,
		unsigned int &current_index, int argc, char **argv,
		std::string &out_datum);
	static bool ParseCmdLineFollowingNotEmpty(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv,
		std::string &out_datum);
	static bool ParseCmdLineFollowingNotEmpty(const char *param_name,
		unsigned int &current_index, int argc, char **argv,
		std::string &out_datum);
	static void CheckCmdLineSource(unsigned int current_index,
		int argc, char **argv);
	static void CheckCmdLineArgList(int argc, char **argv);
	static void RequireFollowingParameter(unsigned int current_index,
		int argc, char **argv);
	static void RequireFollowingParameterNotEmpty(unsigned int current_index,
		int argc, char **argv);
	template <typename ContainerType>
		static bool ParseCmdLineAndRemainder(const std::string &param_name,
		unsigned int &current_index, int argc, char **argv,
		ContainerType &out_datum) {
		CheckCmdLineSource(current_index, argc, argv);
		if (Utility_stricmp(param_name.c_str(), argv[current_index]))
			return(false);
		while (++current_index < static_cast<unsigned int>(argc))
			out_datum.push_back(argv[current_index]);
		return(true);
	}
	static void InvalidArgument(const char *parameter);
	static void CheckDirectoryPath(const std::string &param_name,
		std::string &in_path, const std::string &base_path = "",
		bool require_existence = false);
	static void CheckFilePath(const std::string &param_name,
		std::string &in_path, const std::string &base_path = "",
		bool require_existence = false);
	static void CheckFilePathGeneral(const std::string &param_name,
		std::string &in_path, const std::string &base_path = "",
		bool require_existence = false, bool is_directory = false,
		bool is_file = false);

	//	Support for a delay in start-up by sleeping in main()...
	static std::string  GetMainWaitMSecsHelpShort();
	static std::string  GetMainWaitMSecsHelpLong();
	static bool         ParseMainWaitMSecs(unsigned int &current_index,
		int argc, char **argv, unsigned int &out_datum);
	static bool         ParseMainWaitMSecs(unsigned int &current_index,
		int argc, char **argv);
	static unsigned int GetMainWaitMSecs(int argc, char **argv);
	static unsigned int PerformMainWaitMSecs(int argc, char **argv,
		std::ostream &o_str = std::cout);
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility_ParseCmdLineArg_hpp__HH

