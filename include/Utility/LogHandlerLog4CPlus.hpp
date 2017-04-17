//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of logging management using log4cplus.

	Revision History	:	2005-01-02 --- New ostream-based log model.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
#ifndef HH__MLB__LogHandlerLog4CPlus_hpp__HH

#define HH__MLB__LogHandlerLog4CPlus_hpp__HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility_Exception.hpp>
#include <Utility/LogManager.hpp>

namespace log4cplus {
	class PropertyConfigurator;
};

#include <log4cplus/logger.h>

#ifdef _Windows
# pragma warning(push)
# pragma warning(disable:4625 4626)
#endif // #ifdef _Windows

#include <log4cplus/fileappender.h>

#ifdef _Windows
# pragma warning(pop)
#endif // #ifdef _Windows

#include <log4cplus/configurator.h>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
class LogHandlerLog4CPlus : public MLB::Utility::LogHandler {
public:
	LogHandlerLog4CPlus(const std::string &application_name,
		const std::string &properties_file_name = "",
		bool require_properties_file = true) :
		 application_name_(application_name)
		,empty_file_name_(properties_file_name.empty())
		,properties_file_name_((!properties_file_name.empty()) ?
			properties_file_name : (application_name + ".properties"))
		,require_properties_file_(require_properties_file)
		,property_configuration_() {
		Initialize();
	}

	virtual void InstallHandler() {
	}
	virtual void RemoveHandler() {
	}

	virtual void EmitLine(const LogEmitControl &emit_control) {
		std::ostringstream log_text;
		log_text << " [" << std::setw(10) << CurrentThreadId() << "] " <<
			emit_control.line_buffer_;
		//	IMPLEMENTATION NOTE: May be possible to cache this within this instance..
		log4cplus::Logger app_instance =
			log4cplus::Logger::getInstance(application_name_);
		ForwardString(app_instance, emit_control.log_level_, log_text);
	}

	//	Functions forward the specified string for logging...
	static void ForwardString(log4cplus::Logger &app_instance,
		LogLevel log_level, const std::ostringstream &out_string) {
		ForwardString(app_instance, log_level, out_string.str());
	}
	static void ForwardString(log4cplus::Logger &app_instance,
		LogLevel log_level, const std::string &out_string) {
		ForwardString(app_instance, log_level, out_string.c_str());
	}
	static void ForwardString(log4cplus::Logger &app_instance,
		LogLevel log_level, const char *out_string) {
		if (out_string == NULL)
			out_string = "";
#ifdef _Windows
# pragma warning(push)
# pragma warning(disable:4127)
#endif // #ifdef _Windows
		switch (log_level) {
			case LogLevel_Literal   :
				LOG4CPLUS_TRACE(app_instance, out_string);
				break;
			case LogLevel_Spam      :
				LOG4CPLUS_TRACE(app_instance, out_string);
				break;
			case LogLevel_Minutiae  :
				LOG4CPLUS_TRACE(app_instance, out_string);
				break;
			case LogLevel_Debug     :
				LOG4CPLUS_DEBUG(app_instance, out_string);
				break;
			case LogLevel_Detail    :
				LOG4CPLUS_DEBUG(app_instance, out_string);
				break;
			case LogLevel_Info      :
				LOG4CPLUS_INFO(app_instance, out_string);
				break;
			case LogLevel_Notice    :
				LOG4CPLUS_WARN(app_instance, out_string);
				break;
			case LogLevel_Warning   :
				LOG4CPLUS_WARN(app_instance, out_string);
				break;
			case LogLevel_Error     :
				LOG4CPLUS_ERROR(app_instance, out_string);
				break;
			case LogLevel_Critical  :
				LOG4CPLUS_ERROR(app_instance, out_string);
				break;
			case LogLevel_Alert     :
				LOG4CPLUS_ERROR(app_instance, out_string);
				break;
			case LogLevel_Emergency :
				LOG4CPLUS_ERROR(app_instance, out_string);
				break;
			case LogLevel_Fatal     :
				LOG4CPLUS_FATAL(app_instance, out_string);
				break;
			default						:
				LOG4CPLUS_ERROR(app_instance, out_string);
				break;
#ifdef _Windows
# pragma warning(pop)
#endif // #ifdef _Windows
		}
	}

protected:
	std::string                                        application_name_;
	bool                                               empty_file_name_;
	std::string                                        properties_file_name_;
	bool                                               require_properties_file_;
	boost::shared_ptr<log4cplus::PropertyConfigurator> property_configuration_;

	virtual void Initialize() {
		try {
			log4cplus::helpers::Properties props(properties_file_name_.c_str());
			if (!props.size())
				MLB::Utility::ThrowException("No property name/value pairs "
					"loaded from property file --- does file exist?");
			property_configuration_.
				reset(new log4cplus::PropertyConfigurator(props));
			property_configuration_->configure();
			return;
		}
		catch (const std::exception &except) {
			//	If the ctor parameter was empty, we'll try to use a default
			//	log4cpp configuration. Otherwise, we throw...
			if ((!empty_file_name_) && require_properties_file_)
				Rethrow(except, "Unable to parse log4cplus properties file '" +
					properties_file_name_ + std::string(": ") + except.what());
		}
		try {
			// Try a default configuration...
			log4cplus::BasicConfigurator default_configuration;
			default_configuration.configure();
		}
		catch (const std::exception &except) {
			Rethrow(except, "Unable to configure log4cplus using the defaults: " +
				std::string(except.what()));
		}
	}
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__LogHandlerLog4CPlus_hpp__HH

