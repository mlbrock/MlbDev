//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the log handler file base class.

	Revision History	:	2005-01-02 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
#ifndef HH__MLB__Utility__Utility__LogHandlerFileBase_hpp__HH

#define HH__MLB__Utility__Utility__LogHandlerFileBase_hpp__HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/LogManager.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
class API_UTILITY LogHandlerFileBase : public LogHandler {
public:
	enum LogHandlerFileBaseFlag {
		None            = 0x0000,
		DoNotAppend     = 0x0001,
		NoConsoleOutput = 0x0002,
		Default         = None
	};
	explicit LogHandlerFileBase(LogHandlerFileBaseFlag flags = Default);

	virtual ~LogHandlerFileBase();

	void InstallHandler();
	void RemoveHandler();

	void EmitLine(const LogEmitControl &emit_control);
	void EmitLiteral(unsigned int literal_length, const char *literal_string);
	void EmitLiteral(const LogEmitControl &emit_control,
		unsigned int literal_length, const char *literal_string);

	void OpenFile(const char *file_name);
	void OpenFile(const std::string &file_name);
	void OpenFile(const char *base_name, const char *dir_name);
	void OpenFile(const std::string &base_name, const std::string &dir_name);
	void OpenFile(const char *base_name, const char *dir_name,
		const MLB::Utility::TimeT &start_time);
	void OpenFile(const std::string &base_name, const std::string &dir_name,
		const MLB::Utility::TimeT &start_time);

	void Flush();

	LogHandlerFileBaseFlag GetFlags() const;
	LogHandlerFileBaseFlag SetFlags(LogHandlerFileBaseFlag new_flags);

	std::string        GetFileName() const;

protected:
	virtual void InstallHandlerImpl() = 0;
	virtual void RemoveHandlerImpl() = 0;
	virtual void OpenFileImpl(const char *file_name) = 0;
	virtual void FlushImpl() = 0;
	virtual void EmitLineImpl(const LogEmitControl &emit_control) = 0;
	virtual void EmitLiteralImpl(unsigned int literal_length,
		const char *literal_string) = 0;

	std::string            out_file_name_;
	LogHandlerFileBaseFlag my_flags_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4251)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
	mutable boost::mutex   the_lock_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

private:
	MLB_Utility_NonCopyable_Macro(LogHandlerFileBase);
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__Utility__LogHandlerFileBase_hpp__HH

