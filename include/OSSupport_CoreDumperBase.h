// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the operating system support library.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH_MLB_OSSupport_CoreDumperBase_h_HH

#define HH_MLB_OSSupport_CoreDumperBase_h_HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file OSSupport_CoreDumperBase.h

	\brief	The MLB Operating System Support header file for managing core
	dumps.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OSSupport.h>

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4217 4275 4668 4625 4626)
# if (_MSC_VER >= 1500)
#  pragma warning(disable:4061 4365)
# endif // #if (_MSC_VER >= 1500)
#endif // #ifdef _MSC_VER

#include <boost/thread/mutex.hpp>

#ifdef _MSC_VER
# pragma warning(pop)
#endif // #ifdef _MSC_VER

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
class API_OSSUPPORT CoreDumperBase {
public:
	typedef boost::mutex                      LockType;
	typedef LockType::scoped_lock             ScopedLock;
	typedef boost::shared_ptr<CoreDumperBase> CoreDumperBaseSPtr;

	explicit CoreDumperBase(const char *file_name_base,
		bool is_full_name = false, int dump_flags = -1,
		void *user_data_ptr = NULL);
	explicit CoreDumperBase(const std::string &file_name_base,
		bool is_full_name = false, int dump_flags = -1,
		void *user_data_ptr = NULL);
	CoreDumperBase(const char *file_name_base,
		const char *dir_name, bool is_full_name = false,
		int dump_flags = -1, void *user_data_ptr = NULL);
	CoreDumperBase(const std::string &file_name_base,
		const std::string &dir_name, bool is_full_name = false,
		int dump_flags = -1, void *user_data_ptr = NULL);
	virtual ~CoreDumperBase();

	std::string   GetFileBaseName() const;
	int           GetDumpFlags() const;
	void         *GetUserDataPtr() const;
	std::string   GetNextFileName() const;
	std::string   SetFileBaseName(const std::string &file_name_base,
		bool is_full_name = false,
		MLB::Utility::ProcessId process_id = MLB::Utility::CurrentProcessId(),
		const std::string &host_name = MLB::Utility::GetHostNameCanonical(),
		const MLB::Utility::TimeT &time_stamp = MLB::Utility::TimeT());
	std::string   SetFileBaseName(const std::string &file_name_base,
		const std::string &dir_name, bool is_full_name = false,
		MLB::Utility::ProcessId process_id = MLB::Utility::CurrentProcessId(),
		const std::string &host_name = MLB::Utility::GetHostNameCanonical(),
		const MLB::Utility::TimeT &time_stamp = MLB::Utility::TimeT());
	void         *SetUserDataPtr(void *new_user_data_ptr);
	int           SetDumpFlags(int new_dump_flags);

	bool          CoreDumpIsSupported() const;
	std::string   PerformCoreDump(void *other_data_ptr, int dump_flags = -1);

	LockType     &GetLockRef() const;

	static std::string DecorateBaseName(const std::string &file_name_base,
		MLB::Utility::ProcessId process_id = MLB::Utility::CurrentProcessId(),
		const std::string &host_name = MLB::Utility::GetHostNameCanonical(),
		const MLB::Utility::TimeT &time_stamp = MLB::Utility::TimeT());
	static std::string DecorateBaseName(const char *file_name_base,
		MLB::Utility::ProcessId process_id = MLB::Utility::CurrentProcessId(),
		const std::string &host_name = MLB::Utility::GetHostNameCanonical(),
		const MLB::Utility::TimeT &time_stamp = MLB::Utility::TimeT());
	static std::string ResolveBaseName(const char *file_name_base,
		bool is_full_name = false,
		MLB::Utility::ProcessId process_id = MLB::Utility::CurrentProcessId(),
		const std::string &host_name = MLB::Utility::GetHostNameCanonical(),
		const MLB::Utility::TimeT &time_stamp = MLB::Utility::TimeT());
	static std::string ResolveBaseName(const char *file_name_base,
		const char *dir_name, bool is_full_name = false,
		MLB::Utility::ProcessId process_id = MLB::Utility::CurrentProcessId(),
		const std::string &host_name = MLB::Utility::GetHostNameCanonical(),
		const MLB::Utility::TimeT &time_stamp = MLB::Utility::TimeT());
	static std::string GetDefaultCoreDir();
	static std::string GetDefaultCoreExt();

protected:
	virtual std::string  GetFileBaseNameImpl() const;
	virtual int          GetDumpFlagsImpl() const;
	virtual void        *GetUserDataPtrImpl() const;
	virtual std::string  GetNextFileNameImpl() const;

	virtual std::string  SetFileBaseNameImpl(const std::string &file_name_base,
		bool is_full_name = false,
		MLB::Utility::ProcessId process_id = MLB::Utility::CurrentProcessId(),
		const std::string &host_name = MLB::Utility::GetHostNameCanonical(),
		const MLB::Utility::TimeT &time_stamp = MLB::Utility::TimeT());
	virtual std::string  SetFileBaseNameImpl(const std::string &file_name_base,
		const std::string &dir_name, bool is_full_name = false,
		MLB::Utility::ProcessId process_id = MLB::Utility::CurrentProcessId(),
		const std::string &host_name = MLB::Utility::GetHostNameCanonical(),
		const MLB::Utility::TimeT &time_stamp = MLB::Utility::TimeT());
	virtual int          SetDumpFlagsImpl(int new_dump_flags);
	virtual void        *SetUserDataPtrImpl(void *new_user_data_ptr);

	virtual bool         CoreDumpIsSupportedImpl() const;
	virtual std::string  PerformCoreDumpImpl(void *other_data_ptr,
		int dump_flags);

	std::string       file_name_base_;
	int               dump_flags_;
	void             *user_data_ptr_;
	unsigned int      mini_dump_instance_;
	mutable LockType  the_lock_;

private:
	MLB_Utility_NonCopyable_Macro(CoreDumperBase);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<CoreDumperBase> CoreDumperBaseSPtr;
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#endif // #ifndef HH_MLB_OSSupport_CoreDumperBase_h_HH

