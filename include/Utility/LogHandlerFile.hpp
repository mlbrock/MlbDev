//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the log handler file class.

	Revision History	:	2005-01-02 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
#ifndef HH__MLB__Utility__Utility__LogHandlerFile_hpp__HH

#define HH__MLB__Utility__Utility__LogHandlerFile_hpp__HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/LogHandlerFileBase.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
class API_UTILITY LogHandlerXFile : public LogHandlerFileBase {
public:
	LogHandlerXFile();
	explicit LogHandlerXFile(const char *file_name,
		LogHandlerFileBaseFlag flags = Default);
	explicit LogHandlerXFile(const std::string &file_name,
		LogHandlerFileBaseFlag flags = Default);
	LogHandlerXFile(const char *base_name, const char *dir_name,
		LogHandlerFileBaseFlag flags = Default);
	LogHandlerXFile(const std::string &base_name, const std::string &dir_name,
		LogHandlerFileBaseFlag flags = Default);
	LogHandlerXFile(const char *base_name, const char *dir_name,
		const MLB::Utility::TimeT &start_time,
		LogHandlerFileBaseFlag flags = Default);
	LogHandlerXFile(const std::string &base_name, const std::string &dir_name,
		const MLB::Utility::TimeT &start_time,
		LogHandlerFileBaseFlag flags = Default);

	virtual ~LogHandlerXFile();

protected:
	virtual void InstallHandlerImpl();
	virtual void RemoveHandlerImpl();
	virtual void OpenFileImpl(const char *file_name);
	virtual void FlushImpl();
	virtual void EmitLineImpl(const LogEmitControl &emit_control);
	virtual void EmitLiteralImpl(unsigned int literal_length,
		const char *literal_string);

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4251)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
	boost::shared_ptr<std::ofstream> out_file_ptr_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

private:
	MLB_Utility_NonCopyable_Macro(LogHandlerXFile);
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__Utility__LogHandlerFile_hpp__HH

