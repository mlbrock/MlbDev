//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of operating system file support.

	Revision History	:	2004-10-02 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2004 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/OSFileSupport.hpp>
#include <Utility/AlignmentSupport.hpp>
#include <Utility/Utility_Exception.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
OSFileHandle::OSFileHandle(FileHandleNative native_handle)
	:native_handle_(native_handle)
	,native_handle_owned_((native_handle == FileHandleNativeValueNull))
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
OSFileHandle::~OSFileHandle()
{
	CloseIfOpenSafe();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFileHandle::IsOpen() const
{
	return(IsOpenOSHandle());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFileHandle::IsOpenOSHandle() const
{
	return(native_handle_ != FileHandleNativeValueNull);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFileHandle::CheckOpen() const
{
	CheckOpenOSHandle();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFileHandle::CheckClosed() const
{
	CheckClosedOSHandle();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFileHandle::CheckOpenOSHandle() const
{
	if (!IsOpen())
		ThrowInvalidArgument("Specified file handle is not open.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFileHandle::CheckClosedOSHandle() const
{
	if (IsOpen())
		ThrowInvalidArgument("Specified file handle is not closed.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFileHandle::Close()
{
	CloseOSHandle();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFileHandle::CloseIfOpen()
{
	return(CloseOSHandleIfOpen());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFileHandle::CloseIfOpenSafe()
{
	return(CloseOSHandleIfOpenSafe());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFileHandle::CloseOSHandle()
{
	CheckOpen();

#ifdef _Windows
	::CloseHandle(native_handle_);
#else
	close(native_handle_);
#endif // #ifdef _Windows

	native_handle_ = FileHandleNativeValueNull;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFileHandle::CloseOSHandleIfOpen()
{
	bool was_open;

	if ((was_open = IsOpen()) == true)
		Close();

	return(was_open);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFileHandle::CloseOSHandleIfOpenSafe()
{
	try {
		return(CloseOSHandleIfOpen());
	}
	catch (const std::exception &) {
	}

	return(false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFileHandle::SetOSHandle(FileHandleNative native_handle)
{
	if (IsOpenOSHandle())
		CloseOSHandle();

	 native_handle_       = native_handle;
	 native_handle_owned_ = (native_handle == FileHandleNativeValueNull);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
OSFile::OSFile(const std::string &file_name)
	:OSFileHandle()
	,file_name_(file_name)
	,file_size_(0)
	,file_offset_(0)
	,is_read_only_(false)
	,is_new_file_(false)
	,truncate_to_offset_on_close_(false)
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
OSFile::~OSFile()
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFile::IsReadOnly() const
{
	return(is_read_only_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFile::IsReadable() const
{
	return(IsOpen());
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFile::IsWritable() const
{
	return(IsOpen() && (!IsReadOnly()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFile::IsNewFile() const
{
	return(is_new_file_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFile::IsOffsetFile(FileOffsetType file_offset) const
{
	return(IsOpen() && (file_offset < file_size_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFile::IsExtentFile(FileOffsetType extent_offset,
	FileSizeType extent_length) const
{
	return(IsOpen() && IsOffsetFile(extent_offset) &&
		IsOffsetFile(GetExtentOffsetEnd(extent_offset, extent_length)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFile::IsTruncateToOffsetOnClose() const
{
	return(truncate_to_offset_on_close_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFile::SetTruncateToOffsetOnClose(bool new_truncate_to_offset_on_close)
{
	bool old_truncate_to_offset_on_close = truncate_to_offset_on_close_;

	truncate_to_offset_on_close_ = new_truncate_to_offset_on_close;

	return(old_truncate_to_offset_on_close);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFile::CheckNotReadOnly() const
{
	if (!IsWritable())
		ThrowException("Specified file is not writable.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFile::CheckWritable() const
{
	CheckNotReadOnly();
	CheckOpenOSHandle();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFile::CheckOffsetFile(FileOffsetType file_offset) const
{
	CheckOpenOSHandle();

	if (file_offset >= file_size_)
		ThrowInvalidArgument("Specified file offset " +
			AnyToString(file_offset) + " within file '" + file_name_ +
			"' is invalid because the size of the file is only " +
			AnyToString(file_size_) + " bytes.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFile::CheckExtentFile(FileOffsetType extent_offset,
	FileSizeType extent_length) const
{
	try {
		CheckOpenOSHandle();
		CheckOffsetFile(extent_offset);
		CheckOffsetFile(GetExtentOffsetEnd(extent_offset, extent_length));
	}
	catch (const std::exception &except) {
		Rethrow(except, "Specified file extent (extent starting offset " +
			AnyToString(extent_offset) + " +  extent length " +
			AnyToString(extent_length) + " = extent ending offset " +
			AnyToString(GetExtentOffsetEnd(extent_offset, extent_length)) +
			") within file '" + file_name_ + "' is invalid: " +
			std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFile::Close()
{
	if (IsTruncateToOffsetOnClose() && IsOpen()) {
		//	Want to ensure file is closed even in the truncate throws...
		try {
			TruncateToSize(file_offset_);
		}
		catch (const std::exception &) {
			OSFileHandle::CloseOSHandleIfOpenSafe();
			throw;
		}
	}

	OSFileHandle::Close();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
const std::string &OSFile::GetFileNameRef() const
{
	return(file_name_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::string OSFile::GetFileName() const
{
	return(file_name_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
FileOffsetType OSFile::GetExtentOffsetEnd(FileOffsetType extent_offset,
	FileSizeType extent_length) const
{
	return(extent_offset + ((extent_length) ? (extent_length - 1) : 0));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFile::Open(const std::string &file_name, bool read_only_flag,
	bool do_not_create)
{
	CheckClosedOSHandle();

	//	NOTE: In the case where the file doesn't already, there exists a
	//			race condition between the call to GetFileAttributes() and
	//			the call to CreateFile().

	bool new_file_flag = false;

	if (::GetFileAttributes(file_name.c_str()) == INVALID_FILE_ATTRIBUTES) {
		if ((GetLastError() != ERROR_FILE_NOT_FOUND) || read_only_flag)
			ThrowSystemError("Attempt to open file '" + file_name +
				"' failed --- GetFileAttributes() failed");
		new_file_flag = true;
	}

	if (do_not_create && new_file_flag)
		ThrowException("File '" + file_name + "' does not already exist.");

	if ((native_handle_ = ::CreateFile(file_name.c_str(),
		static_cast<DWORD>((read_only_flag) ? FILE_READ_DATA :
			(FILE_READ_DATA | FILE_WRITE_DATA)),
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		static_cast<DWORD>((read_only_flag || do_not_create) ?
			OPEN_EXISTING : OPEN_ALWAYS),
		FILE_ATTRIBUTE_NORMAL,
		NULL)) == INVALID_HANDLE_VALUE)
		ThrowSystemError("Attempt to open file '" + file_name +
			"' with 'CreateFile()' failed");

	file_name_    = file_name;
	file_size_    = GetFileSize(file_name);
	is_read_only_ = read_only_flag;
	is_new_file_  = new_file_flag;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFile::IncreaseFileSize(FileSizeType new_file_size)
{
	OSFile::IncreaseFileSize(new_file_size, 1); 
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFile::AddFileSize(FileSizeType added_file_size)
{
	OSFile::AddFileSize(added_file_size, 1);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFile::IncreaseFileSize(FileSizeType new_file_size,
	FileSizeType granularity)
{
	GetFileSize();

	IncreaseFileSizeInternal(new_file_size, granularity);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFile::AddFileSize(FileSizeType added_file_size, FileSizeType granularity)
{
	IncreaseFileSizeInternal(GetFileSize() + added_file_size, granularity);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
FileSizeType OSFile::GetFileSize()
{
	file_size_ = GetFileSize(file_name_);

	return(file_size_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
FileOffsetType OSFile::GetOffset()
{
	return(SetOffset(0, FileSeekMethod_Current));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
FileOffsetType OSFile::SetOffset(FileSeekOffsetType file_offset,
	FileSeekMethod seek_method)
{
#ifdef _Windows
	LARGE_INTEGER tmp_offset;
	LARGE_INTEGER tmp_return_value;

	tmp_offset.QuadPart = file_offset;

	if (::SetFilePointerEx(native_handle_, tmp_offset, &tmp_return_value,
		seek_method) != 0)
		return(static_cast<FileOffsetType>(tmp_return_value.QuadPart));
#else
	off_t tmp_return_value;
	if ((tmp_return_value = ::lseek(native_handle_, file_offset,
		seek_method)) != static_cast<off_t>(-1))
		return(static_cast<FileOffsetType>(tmp_return_value));
#endif // #ifdef _Windows

	ThrowSystemError("Attempt to seek within file '" + GetFileName() +
		"' to offset " + AnyToString(file_offset) + " using seek method " +
		AnyToString(seek_method) + " failed");

	return(0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int OSFile::ReadBlock(void *block_ptr,
	unsigned int block_length)
{
	DWORD return_value = 0;

	if (::ReadFile(native_handle_, block_ptr, block_length,
		&return_value, NULL) == 0)
		ThrowSystemError("Attempt to read file '" + GetFileName() + "' for " +
			AnyToString(block_length) + " bytes failed");

	return(return_value);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int OSFile::ReadBlockStrict(void *block_ptr,
	unsigned int block_length)
{
	unsigned int return_value = ReadBlock(block_ptr, block_length);

	if (block_length != return_value)
		ThrowException("Attempt to read file '" + GetFileName() + "' for " +
			AnyToString(block_length) + " bytes failed because the actual number "
			"of bytes read was " + AnyToString(return_value) + ".");

	return(return_value);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int OSFile::WriteBlock(const void *block_ptr,
	unsigned int block_length)
{
	CheckWritable();

	DWORD return_value = 0;

	if (::WriteFile(native_handle_, block_ptr, block_length,
		&return_value, NULL) == 0)
		ThrowSystemError("Attempt to write file '" + GetFileName() + "' for " +
			AnyToString(block_length) + " bytes failed");

	return(return_value);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
unsigned int OSFile::WriteBlockStrict(const void *block_ptr,
	unsigned int block_length)
{
	unsigned int return_value = WriteBlock(block_ptr, block_length);

	if (block_length != return_value)
		ThrowException("Attempt to write file '" + GetFileName() + "' for " +
			AnyToString(block_length) + " bytes failed because the actual number "
			"of bytes written was " + AnyToString(return_value) + ".");

	return(return_value);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
FileOffsetType OSFile::GetInternalOffset() const
{
	return(file_offset_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
FileOffsetType OSFile::SetInternalOffset(FileOffsetType new_file_offset)
{
	FileOffsetType old_file_offset = file_offset_;

	file_offset_ = new_file_offset;

	return(old_file_offset);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	NOTE: Under Windows, if the size to which the file is to be truncated is
//			less than the size of an exisiting 'CreateFileMapping()', the
//			mapping must be dropped before this call can succeed.
void OSFile::TruncateToSize(FileSizeType new_file_size)
{
	FileSizeType current_file_size = GetFileSize();

	try {
		CheckWritable();
		if (new_file_size > current_file_size) {
			HANDLE          mmap_handle = INVALID_HANDLE_VALUE;
			ULARGE_INTEGER  tmp_length;
			tmp_length.QuadPart = new_file_size;
			if ((mmap_handle = ::CreateFileMapping(
				native_handle_,
				NULL,
				static_cast<DWORD>((is_read_only_) ?
					PAGE_READONLY : PAGE_READWRITE),
				tmp_length.HighPart,
				tmp_length.LowPart,
				NULL)) == NULL)
				ThrowSystemError("Attempt to increase the file size with "
					"'CreateFileMapping()' failed");
			::CloseHandle(mmap_handle);
		}
		else if (new_file_size < current_file_size) {
			LARGE_INTEGER tmp_length;
			tmp_length.QuadPart = static_cast<LONGLONG>(new_file_size);
			if (::SetFilePointerEx(
				native_handle_,
				tmp_length,
				NULL,
				FILE_BEGIN) == 0)
				ThrowSystemError("Attempt to prepare for a decrease in the file "
					"size with by setting the file pointer using "
					"'SetFilePointerEx()' failed (distance to move = " +
					AnyToString(new_file_size) + ").");
			if (::SetEndOfFile(native_handle_) == 0)
				ThrowSystemError("Attempt to decrease the file size with "
					"'SetEndOfFile()' failed");
		}
	}
	catch (const std::exception &except) {
		Rethrow(except, "Unable to change the size of file '" + file_name_ +
			"' from " + AnyToString(current_file_size) + " bytes to the "
			"requested size of " + AnyToString(new_file_size) + " bytes: " +
			std::string(except.what()));
	}

	GetFileSize();
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OSFile::Flush()
{
	CheckWritable();

	if (::FlushFileBuffers(native_handle_) == 0)
		ThrowSystemError("Attempt to flush buffer contents for file '" +
			file_name_ + "' failed.");
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
FileSizeType OSFile::GetFileSize(const std::string &file_name)
{
	DWORD size_low;
	DWORD size_high;

	if ((size_low = ::GetFileSize(native_handle_, &size_high)) ==
		INVALID_FILE_SIZE)
		ThrowSystemError("Attempt to determine the file size for '" +
			file_name + "' failed --- GetFileSize() did not succeed");

	return((static_cast<FileSizeType>(size_high) <<
		static_cast<FileSizeType>(sizeof(DWORD) * 8)) |
		static_cast<FileSizeType>(size_low));		
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFile::IncreaseFileSizeInternal(FileSizeType new_file_size,
	FileSizeType granularity)
{
	if (new_file_size == file_size_)
		return;

	if (!new_file_size)
		ThrowInvalidArgument("The specified new file size "
			"(" + AnyToString(new_file_size) + ") is zero.");

	if (new_file_size < file_size_)
		ThrowInvalidArgument("The specified new file size (" +
			AnyToString(new_file_size) +
			") is less than the current file size (" +
			AnyToString(file_size_) + ").");

	TruncateToSize(GranularRoundUp(new_file_size, granularity));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
OSFileMMap::OSFileMMap(const std::string &file_name)
	:OSFile(file_name)
	,os_granularity_(GetOSPageGranularity())
	,mmap_offset_(0)
	,mmap_length_(0)
	,mmap_ptr_(NULL)
	,mmap_ptr_adjust_(0)
	,mmap_handle_(INVALID_HANDLE_VALUE)
{
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
OSFileMMap::~OSFileMMap()
{
	OSFileMMap::CloseIfOpenSafe();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFileMMap::IsMapped() const
{
	return(IsOpen() && (mmap_ptr_ != NULL));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFileMMap::IsOffsetMMap(FileOffsetType file_offset) const
{
	return(IsMapped() && (file_offset >= mmap_offset_) &&
		((file_offset - mmap_offset_) < mmap_length_));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFileMMap::IsExtentMMap(FileOffsetType extent_offset,
	FileSizeType extent_length) const
{
	return(IsMapped() && IsOffsetMMap(extent_offset) &&
		IsOffsetMMap(GetExtentOffsetEnd(extent_offset, extent_length)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFileMMap::CheckMapped() const
{
	OSFileMMap::CheckOpen();

	if (!IsMapped())
		ThrowException("Specified file is not memory-mapped.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFileMMap::CheckUnmapped() const
{
	if (IsMapped())
		ThrowException("Specified file is memory-mapped.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFileMMap::CheckOffsetMMap(FileOffsetType file_offset) const
{
	CheckMapped();

	if (file_offset < mmap_offset_)
		ThrowInvalidArgument("Specified memory-mapped offset " +
			AnyToString(file_offset) + " within file '" + file_name_ +
			"' is invalid because the memory-mapped area begins at file "
			"offset " + AnyToString(mmap_offset_) + ".");
	else if ((file_offset - mmap_offset_) >= mmap_length_)
		ThrowInvalidArgument("Specified memory-mapped offset " +
			AnyToString(file_offset) + " within file '" + file_name_ +
			"' is invalid because the memory-mapped area extends from file "
			"offset " + AnyToString(mmap_offset_) + " through file offset " +
			 AnyToString((mmap_offset_ + mmap_length_) - 1) + " inclusive.");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFileMMap::CheckExtentMMap(FileOffsetType extent_offset,
	FileSizeType extent_length) const
{
	try {
		CheckMapped();
		CheckOffsetMMap(extent_offset);
		CheckOffsetMMap(GetExtentOffsetEnd(extent_offset, extent_length));
	}
	catch (const std::exception &except) {
		Rethrow(except, "Specified memory-mapped extent (extent starting "
			"offset " + AnyToString(extent_offset) + " +  extent length " +
			AnyToString(extent_length) + " = extent ending offset " +
			AnyToString(GetExtentOffsetEnd(extent_offset, extent_length)) +
			") within file '" + file_name_ + "' memory-mapped area of length " +
			AnyToString(mmap_length_) + " is invalid: " +
			std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFileMMap::CheckGranularity(FileSizeType granularity)
{
	if (granularity % os_granularity_)
		ThrowException("Specified memory-mapping granularity (" +
			AnyToString(granularity) + ") is not an integral multiple of "
			"the operating system memory-mapping granularity (" +
			AnyToString(os_granularity_) + ").");
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void *OSFileMMap::GetPtr(FileOffsetType map_offset) const
{
	CheckOffsetMMap(map_offset);

	return(static_cast<void *>(GetMMapPtr() + (map_offset - mmap_offset_)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void *OSFileMMap::GetPtr_void(FileOffsetType map_offset) const
{
	return(GetPtr(map_offset));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *OSFileMMap::GetPtr_char(FileOffsetType map_offset) const
{
	return(static_cast<char *>(GetPtr(map_offset)));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
char *OSFileMMap::GetMMapPtr() const
{
	return(mmap_ptr_);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFileMMap::Close()
{
	UnMap();

	if (IsOpen())
		OSFile::Close();
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFileMMap::CloseIfOpen()
{
	bool was_open;

	if ((was_open = IsOpen()) == true)
		OSFileMMap::Close();
	return(was_open);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
bool OSFileMMap::CloseIfOpenSafe()
{
	try {
		return(OSFileMMap::CloseIfOpen());
	}
	catch (const std::exception &) {
	}

	return(false);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFileMMap::MapFile()
{
	MapFile(GetFileSize(), 0);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
//	CODE NOTE: The efficiency of this method can be improved under Windows by
//	calling CreateFileMapping() only when there is no previous mapping or
//	the previous mapping is for a different (smaller) file size.
void OSFileMMap::MapFile(FileSizeType map_length, FileOffsetType map_offset)
{
	try {
		GetFileSize();
		if (!file_size_)
			ThrowInvalidArgument("The size of the file to be mapped (" +
				AnyToString(file_size_) + ") is zero.");
		if (map_offset >= file_size_)
			ThrowInvalidArgument("The specified mapping offset (" +
				AnyToString(map_offset) + ") is not less than the file size (" +
				AnyToString(file_size_) + ").");
		if (map_length > GetMaxMapLength())
			ThrowInvalidArgument("The specified mapping length (" +
				AnyToString(map_length) +
				") is greater than the maximum permissible value (" +
				AnyToString(GetMaxMapLength()) + ").");
		FileSizeType   new_map_length = map_length;
		FileOffsetType new_map_offset = map_offset;
		FileOffsetType new_ptr_adjust = 0;
		FixupLengthAndOffset(new_map_length, new_map_offset, new_ptr_adjust);
		//	Having passed the checks above, we remove any extent mapping...
		UnMap();
		ULARGE_INTEGER  tmp_length;
		tmp_length.QuadPart = file_size_;
		if ((mmap_handle_ = ::CreateFileMapping(
			native_handle_,
			NULL,
			static_cast<DWORD>((is_read_only_) ? PAGE_READONLY : PAGE_READWRITE),
			tmp_length.HighPart,
			tmp_length.LowPart,
			NULL)) == NULL)
			ThrowSystemError("Attempt to create a file mapping of size " +
				AnyToString(new_map_length) + " for file '" + file_name_ +
				"' with 'CreateFileMapping()' failed");
//		file_size_ = adj_map_length;		//	True under Windows, not Unix.
		ULARGE_INTEGER  tmp_offset;
		tmp_offset.QuadPart = new_map_offset;
		if ((mmap_ptr_ = static_cast<char *>(::MapViewOfFile(
			mmap_handle_,
			static_cast<DWORD>((is_read_only_) ? FILE_MAP_READ : FILE_MAP_WRITE),
			tmp_offset.HighPart,
			tmp_offset.LowPart,
			static_cast<unsigned int>(new_map_length)))) == NULL) {
			::CloseHandle(mmap_handle_);
			mmap_handle_ = INVALID_HANDLE_VALUE;
			std::ostringstream error_text;
			error_text << "Attempt to create a file mapping of size " <<
				new_map_length << " for file '" << file_name_ <<
				"' with 'MapViewOfFile()' failed";
			if (GetLastSystemErrorCode() == ERROR_NOT_ENOUGH_MEMORY)
				throw ExceptionMMapVMFailure(error_text.str() + ": " +
					GetSystemErrorString());
			else
				ThrowSystemError(error_text);
		}
		mmap_offset_     = new_map_offset;
		mmap_length_     = new_map_length;
		mmap_ptr_adjust_ = new_ptr_adjust;
	}
	catch (const std::exception &except) {
		Rethrow(except, "Unable to map file '" + file_name_ + "': " +
			std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OSFileMMap::ReMapFile()
{
	ReMapFile(GetFileSize(), 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OSFileMMap::ReMapFile(FileSizeType map_length, FileOffsetType map_offset)
{
	UnMap();
	MapFile(map_length, map_offset);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OSFileMMap::UnMap()
{
	if (mmap_ptr_ != NULL) {
		::UnmapViewOfFile(mmap_ptr_);
		mmap_ptr_ = NULL;
	}

	if (mmap_handle_ != INVALID_HANDLE_VALUE) {
		::CloseHandle(mmap_handle_);
		mmap_handle_ = INVALID_HANDLE_VALUE;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OSFileMMap::IncreaseFileSize(FileSizeType new_file_size)
{
	OSFile::IncreaseFileSize(new_file_size, os_granularity_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OSFileMMap::AddFileSize(FileSizeType added_file_size)
{
	OSFile::AddFileSize(added_file_size, os_granularity_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OSFileMMap::IncreaseFileSize(FileSizeType new_file_size,
	FileSizeType granularity)
{
	CheckGranularity(granularity);

	OSFile::IncreaseFileSize(new_file_size, granularity);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OSFileMMap::AddFileSize(FileSizeType added_file_size,
	FileSizeType granularity)
{
	CheckGranularity(granularity);

	OSFile::IncreaseFileSize(GetFileSize() + added_file_size, granularity);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OSFileMMap::EnsureSpaceForLengthFromOffset(FileSizeType extent_length,
	FileOffsetType extent_offset)
{
	//	The desired length must not be zero...
	if (!extent_length)
		MLB::Utility::ThrowInvalidArgument("Request to ensure there is "
			"sufficient space in memory-mapped file '" + file_name_ +
			"' specified a length of zero bytes.");

	//	Gotta be open...
	if (IsMapped()) {
		//	If the specified area is already mapped, then we're done...
		if (IsExtentMMap(extent_offset, extent_length))
			return;
	}

	//	If the specified area extends beyond the end of the file, we'll need to
	//	extend the file...
	if (!IsExtentFile(extent_offset, extent_length))
		IncreaseFileSize(GetExtentOffsetEnd(extent_offset, extent_length) + 1);

	//	Now re-map the file from the specified offset for the length...
	ReMapFile(extent_length, extent_offset);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OSFileMMap::EnsureExtentIsMapped(FileSizeType extent_length,
	FileOffsetType extent_offset)
{
	//	The desired length must not be zero...
	if (!extent_length)
		MLB::Utility::ThrowInvalidArgument("Request to ensure there is "
			"sufficient space in the memory-mapped file '" + file_name_ +
			"' specified a length of zero bytes.");

	//	If the specified area extends beyond the end of the file, the desired
	//	mapping can't exist...
	CheckExtentFile(extent_offset, extent_length);

	//	Gotta be open...
	if (IsMapped()) {
		//	If the specified area is already mapped, then we're done...
		if (IsExtentMMap(extent_offset, extent_length))
			return;
	}

	//	Now re-map the file from the specified offset for the length...
	ReMapFile(extent_length, extent_offset);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void OSFileMMap::MFlush()
{
	CheckWritable();

	if (mmap_ptr_ != NULL) {
		if (::FlushViewOfFile(mmap_ptr_, static_cast<SIZE_T>(mmap_length_)) == 0)
			ThrowSystemError("Attempt to flush memory-mapped contents for "
				"file '" + file_name_ + "' failed.");
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int OSFileMMap::FixupGranularity(unsigned int granularity)
{
	return(GranularRoundUp(granularity,
		static_cast<unsigned int>(GetOSPageGranularity())));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
FileSizeType OSFileMMap::FixupGranularity(FileSizeType granularity)
{
	return(GranularRoundUp(granularity, GetOSPageGranularity()));
}
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void OSFileMMap::FixupLengthAndOffset(FileSizeType &map_length,
	FileOffsetType &map_offset, FileOffsetType &ptr_adjust)
{
	if (!map_length)
		ThrowInvalidArgument("The specified mapping length (" +
			AnyToString(map_length) + ") is zero.");

	if (map_length > GetMaxMapLength())
		ThrowInvalidArgument("The specified mapping length (" +
			AnyToString(map_length) +
			") is greater than the maximum permissible value (" +
			AnyToString(GetMaxMapLength()) + ").");

	FileOffsetType   end_map_offset = FixupGranularity(map_offset + map_length) - 1;
	FileSizeType     new_map_offset = (map_offset / GetOSPageGranularity()) *
		GetOSPageGranularity();

	ptr_adjust = map_offset - new_map_offset;
	map_length = (end_map_offset - new_map_offset) + 1;
	map_offset = new_map_offset;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
FileSizeType OSFileMMap::GetOSPageGranularity()
{
	return(static_cast<FileSizeType>(GetPageAllocGranularitySize()));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
FileSizeType OSFileMMap::GetMaxMapLength()
{
	return(static_cast<FileSizeType>(0x7FFFFFFF -
		GetPageAllocGranularitySize()));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <Utility/PathName.hpp>
#include <Utility/UniqueId.hpp>

using namespace MLB::Utility;

// ////////////////////////////////////////////////////////////////////////////
int TEST_OSFile(int return_code)
{
	bool        file_created_flag = false;
	std::string tmp_file_name("./OSFile.TestFile." +
		UniqueId().ToString() + ".txt");

	try {
		FileSizeType alloc_gran = GetPageAllocGranularitySize();
		OSFile tmp_file_1;
		tmp_file_1.Open(tmp_file_name, false, false);
		std::cout << "Opened test file '" << tmp_file_name << "' for writing." <<
			std::endl;
		tmp_file_1.IncreaseFileSize(alloc_gran);
		OSFile tmp_file_2;
		tmp_file_2.Open(tmp_file_name, true, true);
		std::cout << "Opened test file '" << tmp_file_name << "' for reading." <<
			std::endl;
		tmp_file_1.Close();
		tmp_file_2.Close();
		RemoveFile(tmp_file_name);
	}
	catch (const std::exception &except) {
		if (file_created_flag) {
			try {
				RemoveFile(tmp_file_name);
			}
			catch (...) {
			}
		}
		std::cerr << std::endl << "Error in TEST_OSFile    : " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int TEST_OSFileMMap(int return_code)
{
	bool        file_created_flag = false;
	std::string tmp_file_name("./OSFileMMap.TestFile." +
		UniqueId().ToString() + ".txt");

	try {
		FileSizeType alloc_gran = GetPageAllocGranularitySize();
		OSFileMMap tmp_file;
		tmp_file.Open(tmp_file_name, false, false);
		std::cout << "Opened test file '" << tmp_file_name << "'." << std::endl;
		tmp_file.IncreaseFileSize(alloc_gran * 2);
		std::cout << "Increased size of test file '" << tmp_file_name <<
			"' from 0 bytes to " << static_cast<unsigned int>(alloc_gran * 2) <<
			" bytes." << std::endl;
		tmp_file.TruncateToSize(alloc_gran);
		std::cout << "Truncated size of test file '" << tmp_file_name <<
			"' from " << static_cast<unsigned int>(alloc_gran * 2) <<
			" bytes to " << static_cast<unsigned int>(alloc_gran) << " bytes." <<
			std::endl;
		tmp_file.Close();
		RemoveFile(tmp_file_name);
	}
	catch (const std::exception &except) {
		if (file_created_flag) {
			try {
				RemoveFile(tmp_file_name);
			}
			catch (...) {
			}
		}
		std::cerr << std::endl << "Error in TEST_OSFileMMap: " <<
			except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		return_code = TEST_OSFile(return_code);
		return_code = TEST_OSFileMMap(return_code);
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << "ERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

