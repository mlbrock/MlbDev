//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the log miscellaneous support
								functionality.

	Revision History	:	2010-10-23 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2010 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
#ifndef HH__MLB__Utility__LogFileSupport_hpp__HH

#define HH__MLB__Utility__LogFileSupport_hpp__HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/LogManager.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
void        CheckFileNameBaseSegment(const std::string &string_value,
	const char *string_name, bool is_required);
std::string ConstructFileNameBase(const std::string &dir_name,
	const std::string &env_name, const std::string &domain_name,
	const std::string &base_name, const std::string &ext_name,
	const TimeSpec &start_time = TimeSpec(),
	const std::string &host_name = GetHostNameCanonical(),
	const std::string &user_name = GetCurrentUserName(),
	ProcessId process_id = CurrentProcessId());
std::string AppendToFileNameBase(const std::string &in_file_name_base,
	const std::string &new_segment);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class LogFilePreConfigureParse {
public:
	LogFilePreConfigureParse(int argc, char **argv, const char *log_dir_regex =
		"^\\-\\-*LOG(GING)*_*((DIR(ECTORY)*)|(PATH))(_*NAME)*$",
		const char *env_regex = "^\\-\\-*ENV(IRON(MENT)*)*_*(NAME)*$",
		const char *domain_regex = NULL, const char *ext_regex = NULL);
	virtual ~LogFilePreConfigureParse();

	std::string log_dir_;
	std::string env_name_;
	std::string domain_name_;
	std::string ext_name_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class LogFilePreConfigure {
public:
	LogFilePreConfigure(LogManager &log_manager, int argc, char **argv,
		const std::string &base_name, const std::string &ext_name,
		const char *log_dir_regex =
			"^\\-\\-*LOG(GING)*_*((DIR(ECTORY)*)|(PATH))(_*NAME)*$",
		const char *env_regex = "^\\-\\-*ENV(IRON(MENT)*)*_*(NAME)*$",
		const char *domain_regex = NULL, const char *ext_regex = NULL);
	virtual ~LogFilePreConfigure();

	void swap(LogFilePreConfigure &other);

	TimeSpec                 start_time_;
	std::string              file_name_base_;
	std::vector<std::string> argv_;
	std::string              start_dir_;
	std::string              log_dir_;
	std::string              env_name_;
	std::string              domain_name_;
	std::string              base_name_;
	std::string              ext_name_;
	std::string              log_file_name_;
	bool                     log_file_is_open_;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__LogFileSupport_hpp__HH

