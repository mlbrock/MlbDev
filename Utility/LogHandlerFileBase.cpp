//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of logging management.

	Revision History	:	1993-10-02 --- Creation of predecessor 'mlog' facility.
									Michael L. Brock
								2005-01-02 --- New ostream-based log model.
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2017.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/LogHandlerFileBase.hpp>

#include <Utility/ValueToStringRadix.hpp>
#include <Utility/PathName.hpp>
#include <Utility/FilesystemSupport.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4217 4668)
# if _MSC_VER >= 1500
#  pragma warning(disable: 4347 4360 4571)
# endif // # if _MSC_VER >= 1500
#endif // #if defined(_Windows) && !defined(__MINGW32__)

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/exception.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFileBase::LogHandlerFileBase(LogHandlerFileBaseFlag flags)
	:LogHandler()
	,out_file_name_()
	,my_flags_(flags)
	,the_lock_()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFileBase::~LogHandlerFileBase()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileBase::InstallHandler()
{
	boost::mutex::scoped_lock my_lock(the_lock_);

	InstallHandlerImpl();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileBase::RemoveHandler()
{
	boost::mutex::scoped_lock my_lock(the_lock_);

	RemoveHandlerImpl();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileBase::EmitLine(const LogEmitControl &emit_control)
{
	if (emit_control.ShouldLogPersistent() || emit_control.ShouldLogScreen()) {
		boost::mutex::scoped_lock my_lock(the_lock_);
		emit_control.UpdateTime();
		if (emit_control.ShouldLogPersistent())
			EmitLineImpl(emit_control);
		if ((!(my_flags_ & NoConsoleOutput)) && emit_control.ShouldLogScreen()) {
			std::cout.write(emit_control.GetLeaderPtr(),
				static_cast<std::streamsize>(emit_control.GetLeaderLength()));
			std::cout.write(emit_control.line_buffer_.c_str(),
				static_cast<std::streamsize>(emit_control.line_buffer_.size()));
			std::cout << std::endl;
		}
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileBase::EmitLiteral(unsigned int literal_length,
	const char *literal_string)
{
	boost::mutex::scoped_lock my_lock(the_lock_);

	EmitLiteralImpl(literal_length, literal_string);

	if (!(my_flags_ & NoConsoleOutput)) {
		std::cout.write(literal_string,
			static_cast<std::streamsize>(literal_length));
		std::cout << std::endl;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileBase::EmitLiteral(const LogEmitControl &emit_control,
	unsigned int literal_length, const char *literal_string)
{
	if (emit_control.ShouldLogPersistent() || emit_control.ShouldLogScreen()) {
		boost::mutex::scoped_lock my_lock(the_lock_);
		if (emit_control.ShouldLogPersistent())
			EmitLiteralImpl(literal_length, literal_string);
		if ((!(my_flags_ & NoConsoleOutput)) && emit_control.ShouldLogScreen()) {
			std::cout.write(literal_string,
				static_cast<std::streamsize>(literal_length));
			std::cout << std::endl;
		}
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileBase::OpenFile(const char *file_name)
{
	if ((file_name == NULL) || (!(*file_name)))
		ThrowException("Specified log file name is NULL or an empty string.");

	try {
		OpenFileImpl(file_name);
	}
	catch (const std::exception &except) {
		Rethrow(except, "Unable to open log file '" + std::string(file_name) +
			std::string("': ") + except.what());
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileBase::OpenFile(const std::string &file_name)
{
	OpenFile(file_name.c_str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileBase::OpenFile(const char *base_name, const char *dir_name)
{
	OpenFile(base_name, dir_name, MLB::Utility::TimeT());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileBase::OpenFile(const std::string &base_name,
	const std::string &dir_name)
{
	OpenFile(base_name.c_str(), dir_name.c_str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileBase::OpenFile(const char *base_name, const char *dir_name,
	const MLB::Utility::TimeT &start_time)
{
	std::string file_name;

	if ((base_name == NULL) || (!(*base_name)))
		file_name = "LogFile.";
	else {
		file_name = base_name;
		if (base_name[strlen(base_name) - 1] != '.')
			file_name += '.';
	}

	std::string tmp_date_time(start_time.ToString());

	tmp_date_time[10]  = '.';
	tmp_date_time[13]  = '.';
	tmp_date_time[16]  = '.';
	file_name         += tmp_date_time + "." + GetHostNameCanonical() + "." +
		AnyToString(CurrentProcessId()) + ".log";

	boost::filesystem::path tmp_file(BoostFs_ConstructNativePath(file_name));

	if ((dir_name != NULL) && *dir_name) {
		std::string tmp_dir_name;
		ResolveFilePathGeneral(dir_name, tmp_dir_name, "", true, true, false);
		boost::filesystem::path tmp_path(
			BoostFs_ConstructNativePath(tmp_dir_name));
		boost::filesystem::path this_file;
		this_file        = tmp_path / tmp_file;
		file_name        = BoostFs_GetNativeFileString(this_file);
	}
	else {
		if (!tmp_file.has_root_path())
			tmp_file = boost::filesystem::system_complete(tmp_file);
		file_name = BoostFs_GetNativeFileString(tmp_file);
	}

	OpenFile(file_name);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileBase::OpenFile(const std::string &base_name,
	const std::string &dir_name, const MLB::Utility::TimeT &start_time)
{
	OpenFile(base_name.c_str(), dir_name.c_str(), start_time);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LogHandlerFileBase::Flush()
{
	boost::mutex::scoped_lock my_lock(the_lock_);

	FlushImpl();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFileBase::LogHandlerFileBaseFlag LogHandlerFileBase::GetFlags() const
{
	boost::mutex::scoped_lock my_lock(the_lock_);

	return(my_flags_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LogHandlerFileBase::LogHandlerFileBaseFlag LogHandlerFileBase::SetFlags(
	LogHandlerFileBaseFlag new_flags)
{
	boost::mutex::scoped_lock my_lock(the_lock_);

	LogHandlerFileBaseFlag old_flags = my_flags_;

	my_flags_ = new_flags;

	return(old_flags);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string LogHandlerFileBase::GetFileName() const
{
	return(out_file_name_);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

