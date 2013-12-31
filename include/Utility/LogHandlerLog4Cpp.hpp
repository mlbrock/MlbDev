//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of logging management using log4cpp.

	Revision History	:	2005-01-02 --- New ostream-based log model.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
#ifndef HH__MLB__LogHandlerLog4Cpp_hpp__HH

#define HH__MLB__LogHandlerLog4Cpp_hpp__HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/LogManager.hpp>

#define LOG4CPP_FIX_ERROR_COLLISION 1

#ifdef _Windows
# pragma warning(push)
# pragma warning(disable:4512)
#endif // #ifdef _Windows

#include <log4cpp/Portability.hh>
#ifdef LOG4CPP_HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/NDC.hh>
#include <log4cpp/PropertyConfigurator.hh>

#ifdef _Windows
# pragma warning(pop)
#endif // #ifdef _Windows

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
class LogHandlerLog4Cpp : public MLB::Utility::LogHandler {
public:
	LogHandlerLog4Cpp(const std::string &application_name,
		const std::string &properties_file_name = "",
		bool require_properties_file = true) :
		 application_name_(application_name)
		,properties_file_name_(properties_file_name)
		,require_properties_file_(require_properties_file) {
		try {
			if (properties_file_name_.empty())
				properties_file_name_ = application_name + ".properties";
			log4cpp::PropertyConfigurator::configure(properties_file_name_);
			return;
		}
		catch (const std::exception &except) {
			//	If the ctor parameter was empty, we'll try to use a default
			//	log4cpp configuration. Otherwise, we throw...
			if ((!properties_file_name.empty()) && require_properties_file)
				Rethrow(except, "Unable to parse log4cpp properties file '" +
					properties_file_name + std::string(": ") + except.what());
		}
		try {
			// Try a default configuration...
			log4cpp::Category &root_ref = log4cpp::Category::getRoot();
			root_ref.setPriority(log4cpp::Priority::DEBUG);
		}
		catch (const std::exception &except) {
			Rethrow(except, "Unable to configure log4cpp using the defaults: " +
				std::string(except.what()));
		}
	}

	void InstallHandler() {
	}
	void RemoveHandler() {
	}

	virtual void EmitLine(const LogEmitControl &emit_control) {
		std::ostringstream log_text;
		log_text << " [" << std::setw(10) << CurrentThreadId() << "] " <<
			emit_control.line_buffer_;
		//	IMPLEMENTATION NOTE: May be possible to cache this within this instance..
		log4cpp::Category &app_instance =
			log4cpp::Category::getInstance(application_name_);
		switch (emit_control.log_level_) {
			case LogLevel_Spam      :
				app_instance.debug(log_text.str());
				break;
			case LogLevel_Minutiae  :
				app_instance.debug(log_text.str());
				break;
			case LogLevel_Debug     :
				app_instance.debug(log_text.str());
				break;
			case LogLevel_Detail    :
				app_instance.debug(log_text.str());
				break;
			case LogLevel_Info      :
				app_instance.info(log_text.str());
				break;
			case LogLevel_Notice    :
				app_instance.notice(log_text.str());
				break;
			case LogLevel_Warning   :
				app_instance.warn(log_text.str());
				break;
			case LogLevel_Error     :
				app_instance.error(log_text.str());
				break;
			case LogLevel_Critical  :
				app_instance.crit(log_text.str());
				break;
			case LogLevel_Alert     :
				app_instance.alert(log_text.str());
				break;
			case LogLevel_Emergency :
				app_instance.emerg(log_text.str());
				break;
			case LogLevel_Fatal     :
				app_instance.fatal(log_text.str());
				break;
			default						:
				app_instance.error(log_text.str());
				break;
		}
	}

	std::string application_name_;
	std::string properties_file_name_;
	bool        require_properties_file_;
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__LogHandlerLog4Cpp_hpp__HH

