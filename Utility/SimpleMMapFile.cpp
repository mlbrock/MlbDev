// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the SimpleMMapFile class.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility_Exception.hpp>
#include <Utility/SimpleMMapFile.hpp>

#ifdef _Windows
# if !defined(__MINGW32__)
#  pragma warning(push)
#  pragma warning(disable:4217 4668)
# endif // # if !defined(__MINGW32__)
# include <boost/shared_array.hpp>
# if !defined(__MINGW32__)
#  pragma warning(pop)
# endif // # if !defined(__MINGW32__)
#else
# include <boost/shared_array.hpp>
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
SimpleMMapFile::SimpleMMapFile()
	:file_name_()
	,is_read_only_(true)
	,is_exclusive_(false)
	,granularity_(0)
	,is_new_file_(true)
	,current_offset_(0)
	,mmap_size_(0)
	,mmap_ptr_(NULL)
	,file_handle_(INVALID_HANDLE_VALUE)
	,mmap_handle_(INVALID_HANDLE_VALUE)
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SimpleMMapFile::SimpleMMapFile(const std::string &file_name, bool is_read_only,
	bool is_exclusive, unsigned int granularity)
	:file_name_(file_name)
	,is_read_only_(is_read_only)
	,is_exclusive_(is_exclusive)
	,granularity_(FixupGranularity(granularity))
	,is_new_file_(true)
	,current_offset_(0)
	,mmap_size_(0)
	,mmap_ptr_(NULL)
	,file_handle_(INVALID_HANDLE_VALUE)
	,mmap_handle_(INVALID_HANDLE_VALUE)
{
	OpenFileInternal();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SimpleMMapFile::Open(const std::string &file_name, bool is_read_only,
	bool is_exclusive, unsigned int granularity)
{
	if (IsOpenFile() || IsOpenMapping())
		ThrowStdException("File mapping is already open.");

	file_name_    = file_name;
	is_read_only_ = is_read_only;
	is_exclusive_ = is_exclusive;
	granularity_  = FixupGranularity(granularity);
	is_new_file_  = true;
	mmap_size_    = 0;
	mmap_ptr_     = NULL;
	file_handle_  = INVALID_HANDLE_VALUE;
	mmap_handle_  = INVALID_HANDLE_VALUE;

	OpenFileInternal();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
SimpleMMapFile::~SimpleMMapFile()
{
	Close();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SimpleMMapFile::Close()
{
	CloseMapping();
	CloseFile();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SimpleMMapFile::IsOpen() const
{
	return(IsOpenFile() && IsOpenMapping());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SimpleMMapFile::IsOpenFile() const
{
	return(file_handle_ != INVALID_HANDLE_VALUE);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SimpleMMapFile::IsOpenMapping() const
{
	return((mmap_ptr_ != NULL) && (mmap_handle_ != INVALID_HANDLE_VALUE));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SimpleMMapFile::IsReadOnly() const
{
	return(is_read_only_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool SimpleMMapFile::IsNewFile() const
{
	return(is_new_file_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SimpleMMapFile::CheckOpen() const
{
	if (!IsOpen())
		ThrowStdException("File mapping is not open.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SimpleMMapFile::IncreaseSize(unsigned int new_map_size,
	unsigned int reserve_at_start)
{
	CheckOpen();

	if (FixupGranularity(new_map_size) > mmap_size_)
		ReMapFile(new_map_size, reserve_at_start);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SimpleMMapFile::AddToSize(unsigned int added_size)
{
	IncreaseSize(current_offset_ + added_size);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SimpleMMapFile::GetOffset() const
{
	return(current_offset_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SimpleMMapFile::SetOffset(unsigned int new_offset)
{
	CheckOffset(new_offset);

	current_offset_ = new_offset;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SimpleMMapFile::Write(const std::string &out_data)
{
	Write(static_cast<unsigned int>(out_data.size()), out_data.c_str());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SimpleMMapFile::Write(const char *out_data)
{
	if (out_data == NULL)
		ThrowInvalidArgument("Data to write into the file is "
			"specified with a NULL pointer.");

	Write(static_cast<unsigned int>(strlen(out_data)), out_data);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SimpleMMapFile::Write(unsigned int out_length, const void *out_data)
{
	if (out_length) {
		if (out_data == NULL)
			ThrowInvalidArgument("Data to write into the file "
				"is specified with a NULL pointer.");
		if ((current_offset_ + out_length) >= mmap_size_)
			AddToSize(out_length);
		memcpy(mmap_ptr_ + current_offset_, out_data, out_length);
		current_offset_ += out_length;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void *SimpleMMapFile::GetPtr(unsigned int file_offset) const
{
	CheckOffset(file_offset);

	return(static_cast<void *>(mmap_ptr_ + file_offset));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SimpleMMapFile::GetMMapSize() const
{
	return(mmap_size_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *SimpleMMapFile::GetMMapPtr() const
{
	return(mmap_ptr_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int SimpleMMapFile::FixupGranularity(unsigned int granularity)
{
	unsigned int alloc_size = GetPageAllocGranularitySize();

	return((granularity && (!(granularity % alloc_size))) ? granularity :
		(((granularity / alloc_size) + 1) * alloc_size));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SimpleMMapFile::CheckOffset(unsigned int file_offset) const
{
	CheckOpen();

	if (file_offset >= mmap_size_) {
		std::ostringstream error_text;
		error_text << "Attempt to access file offset " << file_offset <<
			" within memory-mapped file '" << file_name_ <<
			"' failed because the size of the file is only " << mmap_size_ <<
			".";
		ThrowInvalidArgument(error_text);
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SimpleMMapFile::CloseFile()
{
	if (file_handle_ != INVALID_HANDLE_VALUE) {
		CloseHandle(file_handle_);
		file_handle_ = INVALID_HANDLE_VALUE;
	}

	mmap_size_ = 0;
	mmap_ptr_  = NULL;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SimpleMMapFile::CloseMapping()
{
	if (mmap_ptr_ != NULL) {
		UnmapViewOfFile(mmap_ptr_);
		mmap_size_ = 0;
		mmap_ptr_  = NULL;
	}

	if (mmap_handle_ != INVALID_HANDLE_VALUE) {
		CloseHandle(mmap_handle_);
		mmap_handle_ = INVALID_HANDLE_VALUE;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SimpleMMapFile::OpenFileInternal()
{
	if (file_handle_ != INVALID_HANDLE_VALUE)
		ThrowStdException("File handle is already in use.");

	//	NOTE: In the case where the file doesn't already, there exists a
	//			race condition between the call to GetFileAttributes() and
	//			the call to CreateFile().
	if (::GetFileAttributes(file_name_.c_str()) == INVALID_FILE_ATTRIBUTES) {
		if ((::GetLastError() != ERROR_FILE_NOT_FOUND) || IsReadOnly()) {
			std::ostringstream error_text;
			error_text << "Attempt to open file '" << file_name_ <<
				"' for memory-mapping failed --- GetFileAttributes() failed";
			ThrowSystemError(error_text);
		}
		is_new_file_ = true;
	}
	else
		is_new_file_ = false;

	if ((file_handle_ = ::CreateFile(file_name_.c_str(),
		static_cast<DWORD>((is_read_only_) ? FILE_READ_DATA :
			(FILE_READ_DATA | FILE_WRITE_DATA)),
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL)) == INVALID_HANDLE_VALUE) {
		std::ostringstream error_text;
		error_text << "Attempt to open file '" << file_name_ <<
			"' for memory-mapping use with 'CreateFile()' failed";
		ThrowSystemError(error_text);
	}

	DWORD size_low;
	DWORD size_high;

	if ((size_low = ::GetFileSize(file_handle_, &size_high)) ==
		INVALID_FILE_SIZE) {
		CloseFile();
		std::ostringstream error_text;
		error_text << "Attempt to open file '" << file_name_ <<
			"' for memory-mapping failed --- GetFileSize() did not succeed";
		ThrowSystemError(error_text);
	}

	if (size_high) {
		CloseFile();
		std::ostringstream error_text;
		error_text << "Attempt to open file '" << file_name_ <<
			"' for memory-mapping failed --- size too large (" <<
			((static_cast<double>(size_high) *
			static_cast<double>(std::numeric_limits<unsigned int>::max())) +
			static_cast<double>(size_low)) << ")";
		ThrowSystemError(error_text);
	}

	if (size_low % GetPageAllocGranularitySize()) {
		if (is_read_only_) {
			CloseFile();
			std::ostringstream error_text;
			error_text << "Attempt to open file '" << file_name_ <<
				"' for memory-mapping failed --- file is open for read-only " <<
				"and the current file size (" << size_low <<
				" is not an integral multiple of the allocation granularity (" <<
				GetPageAllocGranularitySize() << ")";
			ThrowStdException(error_text);
		}
	}

	try {
		MapFile(size_low);
	}
	catch (const std::exception &except) {
		CloseFile();
		std::ostringstream error_text;
		error_text << "Attempt to open file '" << file_name_ <<
			"' for memory-mapping failed --- attempt to map file contents " <<
			"failed: " << except.what();
		Rethrow(except, error_text);
	}

	current_offset_ = size_low;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SimpleMMapFile::MapFile(unsigned int new_map_size)
{
	try {
		new_map_size = FixupGranularity(std::max(new_map_size, granularity_));
		ReMapFileInternal(file_name_, file_handle_, granularity_, mmap_handle_,
			mmap_ptr_, is_read_only_, new_map_size);
		mmap_size_ = new_map_size;
	}
	catch (const std::exception &) {
		CloseMapping();
		throw;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SimpleMMapFile::ReMapFile(unsigned int new_map_size,
	unsigned int reserve_at_start)
{
	HANDLE  tmp_mmap_handle = INVALID_HANDLE_VALUE;
	char   *tmp_mmap_ptr    = NULL;

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	First, try to create a mapping for the new size. If this succeeds,
	//	we just replace the old mapping with the new.
	//	//////////////////////////////////////////////////////////////////////
	try {
		ReMapFileInternal(file_name_, file_handle_, granularity_, tmp_mmap_handle,
			tmp_mmap_ptr, is_read_only_, new_map_size);
		//	Worked, so unmap the old mapping and replace with the new...
		CloseMapping();
		mmap_size_   = new_map_size;
		mmap_ptr_    = tmp_mmap_ptr;
		mmap_handle_ = tmp_mmap_handle;
		return;
	}
	catch (const ExceptionMMapVMFailure &) {
		//	If no previous old mapping existed, we just re-throw...
		if (!mmap_size_)
			throw;
	}
	//	//////////////////////////////////////////////////////////////////////

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	If the code above resulted in an 'ExceptionMMapVMFailure' exception,
	//	we'll try to unmap the old mapping and create the new mapping...
	//	//////////////////////////////////////////////////////////////////////
	unsigned int           saved_mmap_size = mmap_size_;
	ExceptionMMapVMFailure saved_vm_exception;
//++vm_avail_failure_count_;
	CloseMapping();
	try {
		boost::shared_array<char> reserved_ptr;
		if (reserve_at_start) {
			//	Reserve space at the start of the address if specified...
			reserve_at_start =
				MLB::Utility::SimpleMMapFile::FixupGranularity(reserve_at_start);
			reserved_ptr.reset(new char[reserve_at_start]);
		}
		ReMapFileInternal(file_name_, file_handle_, granularity_, mmap_handle_,
			mmap_ptr_, is_read_only_, new_map_size);
		//	Worked, so we're done...
		mmap_size_ = new_map_size;
		return;
	}
	catch (const ExceptionMMapVMFailure &except) {
		// Save the exception for re-throwing below...
		saved_vm_exception = except;
	}

	//	//////////////////////////////////////////////////////////////////////
	//	//////////////////////////////////////////////////////////////////////
	//	If we got an 'ExceptionMMapVMFailure' exception, we try to restore the
	//	old original mapping before re-throwing the exception...
	//	//////////////////////////////////////////////////////////////////////
	try {
		ReMapFileInternal(file_name_, file_handle_, granularity_, mmap_handle_,
			mmap_ptr_, is_read_only_, saved_mmap_size);
		mmap_size_ = saved_mmap_size;
		throw saved_vm_exception;
	}	
	//	Our final fall-through position: entirely invalidate the mapping.
	catch (const std::exception &) {
		Close();
		throw;
	}
	//	//////////////////////////////////////////////////////////////////////

}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void SimpleMMapFile::ReMapFileInternal(const std::string &file_name,
	HANDLE file_handle, unsigned int granularity, HANDLE &mmap_handle,
	char *&mmap_ptr, bool is_read_only, unsigned int new_map_size)
{
	new_map_size = FixupGranularity(std::max(new_map_size, granularity));

	if (mmap_handle != INVALID_HANDLE_VALUE)
		ThrowStdException("Mmap handle is already in use.");

	if (mmap_ptr != NULL)
		ThrowStdException("Mmap pointer is already in use.");

	if ((mmap_handle = ::CreateFileMapping(file_handle,
		NULL,
		static_cast<DWORD>((is_read_only) ? PAGE_READONLY : PAGE_READWRITE),
		0,
		new_map_size,
		NULL)) == NULL) {
		std::ostringstream error_text;
		error_text << "Attempt to create a file mapping of size " <<
			new_map_size << " for file '" << file_name <<
			"' with 'CreateFileMapping()' failed";
		ThrowSystemError(error_text);
	}

	if ((mmap_ptr = static_cast<char *>(::MapViewOfFile(mmap_handle,
		static_cast<DWORD>((is_read_only) ? FILE_MAP_READ : FILE_MAP_WRITE),
		0,
		0,
		new_map_size))) == NULL) {
		CloseHandle(mmap_handle);
		mmap_handle = INVALID_HANDLE_VALUE;
		std::ostringstream error_text;
		error_text << "Attempt to create a file mapping of size " <<
			new_map_size << " for file '" << file_name <<
			"' with 'MapViewOfFile()' failed";
		if (GetLastSystemErrorCode() == ERROR_NOT_ENOUGH_MEMORY)
			throw ExceptionMMapVMFailure(error_text.str() + ": " +
				GetSystemErrorString());
		else
			ThrowSystemError(error_text);
	}
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <iostream>

#include <Utility/ParseCmdLineArg.hpp>

using namespace MLB::Utility;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	std::cout << "Test routine for 'SimpleMMapFile()'" << std::endl;
	std::cout << "---- ------- --- ------------------" << std::endl;

	int           return_code   = EXIT_SUCCESS;
	unsigned int  record_count  =    1000;
	unsigned int  record_size   =    1000;
	unsigned int  alloc_gran    = 0x10000;
	bool          fragmentation = false;
	std::string   file_name("RegressionTest.SimpleMMapFile.smf");

	if (ParseCmdLineArg::HasCmdLineHelp(argc, argv, 1)) {
		std::cout << "USAGE: " << std::endl <<
			"   " << argv[0] << " " <<
			"[ -record_count <number-of-records-to-write> ( = " <<
				record_count << " ) ] " <<
			"[ -record_size <record-size> ( = " << record_size << " ) ] " <<
			"[ -allocation_granularity <file-allocation-granularity> ( = " <<
				alloc_gran << " ) ] " <<
			"[ -fragmentation <boolean> ( = false ) ]" <<
			std::endl << std::endl;
		exit(EXIT_SUCCESS);
	}

	try {
		unsigned int count_1;
		for (count_1 = 1; count_1 < static_cast<unsigned int>(argc); ++count_1) {
			if (ParseCmdLineArg::ParseCmdLineNumeric<unsigned int>("-RECORD_COUNT",
				count_1, argc, argv, record_count, 1))
				;
			else if (ParseCmdLineArg::ParseCmdLineNumeric<unsigned int>(
				"-RECORD_SIZE", count_1, argc, argv, record_size, 1))
				;
			else if (ParseCmdLineArg::ParseCmdLineDatum("-FRAGMENTATION", count_1,
				argc, argv, fragmentation))
				;
			else if (ParseCmdLineArg::ParseCmdLineNumeric<unsigned int>(
				"-ALLOCATION_GRANULARITY", count_1, argc, argv, alloc_gran, 0))
				;
			else
				ParseCmdLineArg::InvalidArgument(argv[count_1]);
		}
		std::cout << "File Name             : " << file_name << std::endl;
		std::cout << "Record Count          : " << record_count << std::endl;
		std::cout << "Record Size           : " << record_size << std::endl;
		std::cout << "Fragmentation         : " << fragmentation << std::endl;
		std::cout << "Allocation Granularity: " << alloc_gran << std::endl;
		std::cout << std::endl;
		std::set<unsigned int> fragment_set;
		std::string            buffer(std::string(record_size, 'A') + "\n");
		SimpleMMapFile         mmap_file(file_name, false, false, alloc_gran);
		std::cout << ">>> Opened file '" << file_name << "'... " << std::flush;
		try {
			for (count_1 = 0; count_1 < record_count; ++count_1) {
				mmap_file.Write(buffer);
				if (fragmentation)
					fragment_set.insert(count_1);
			}
			std::cout << "done." << std::endl;
		}
		catch (...) {
			std::cout << "FAILURE ON INTERATION INDEX " << count_1 << std::endl;
			throw;
		}
	}
	catch (const std::exception &except) {
		std::cout << std::endl;
		std::cout << "Regression test error: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

