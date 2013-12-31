//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Include File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for operating system file support.

	Revision History	:	2004-10-02 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2004 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
#ifndef HH_MLB_OSSupport_File_h_HH

#define HH_MLB_OSSupport_File_h_HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <OSSupport.h>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

//	////////////////////////////////////////////////////////////////////////////
typedef MLB::Utility::FileOffsetType FileOffsetType;
typedef MLB::Utility::FileSizeType   FileSizeType;
//	////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#ifdef _Windows
typedef HANDLE FileHandleNative;
const FileHandleNative FileHandleNativeValueNull = INVALID_HANDLE_VALUE;
#else
typedef int FileHandleNative;
const FileHandleNative FileHandleNativeValueNull = -1;
#endif // #ifdef _Windows
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef signed long long FileSeekOffsetType;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
enum FileSeekMethod {
#ifdef _Windows
	FileSeekMethod_Set     = FILE_BEGIN,
	FileSeekMethod_Begin   = FileSeekMethod_Set,
	FileSeekMethod_Current = FILE_CURRENT,
	FileSeekMethod_End     = FILE_END
#else
	FileSeekMethod_Set     = SEEK_SET,
	FileSeekMethod_Begin   = FileSeekMethod_Set,
	FileSeekMethod_Current = SEEK_CUR,
	FileSeekMethod_End     = SEEK_END
#endif // #ifdef _Windows
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class API_OSSUPPORT OSFileHandle {
public:
	OSFileHandle(FileHandleNative native_handle = FileHandleNativeValueNull);
	virtual ~OSFileHandle();

	virtual bool IsOpen() const;

	bool IsOpenOSHandle() const;

	virtual void CheckOpen() const;
	virtual void CheckClosed() const;

	void CheckOpenOSHandle() const;
	void CheckClosedOSHandle() const;

	virtual void Close();
	virtual bool CloseIfOpen();
	virtual bool CloseIfOpenSafe();

	void CloseOSHandle();
	bool CloseOSHandleIfOpen();
	bool CloseOSHandleIfOpenSafe();

	void SetOSHandle(FileHandleNative native_handle = FileHandleNativeValueNull);

	FileHandleNative native_handle_;
	bool             native_handle_owned_;

private:
	MLB_Utility_NonCopyable_Macro(OSFileHandle);
};
// ////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_OSSUPPORT OSFile : public OSFileHandle {
public:
	OSFile(const std::string &file_name = "");
	virtual ~OSFile();

	bool IsReadOnly() const;
	bool IsReadable() const;
	bool IsWritable() const;
	bool IsNewFile() const;
	bool IsOffsetFile(FileOffsetType file_offset) const;
	bool IsExtentFile(FileOffsetType extent_offset,
		FileSizeType extent_length) const;
	bool IsTruncateToOffsetOnClose() const;
	bool SetTruncateToOffsetOnClose(bool new_truncate_to_offset_on_close);

	void CheckNotReadOnly() const;
	void CheckWritable() const;
	void CheckOffsetFile(FileOffsetType file_offset) const;
	void CheckExtentFile(FileOffsetType extent_offset,
		FileSizeType extent_length) const;

	virtual void Close();

	const std::string &GetFileNameRef() const;
	      std::string  GetFileName() const;

	FileOffsetType GetExtentOffsetEnd(FileOffsetType extent_offset,
		FileSizeType extent_length) const;

	virtual void Open(const std::string &file_name, bool read_only_flag = true,
		bool do_not_create = false);

	virtual void IncreaseFileSize(FileSizeType new_file_size);
	virtual void AddFileSize(FileSizeType added_file_size);

	virtual void IncreaseFileSize(FileSizeType new_file_size,
		FileSizeType granularity);
	virtual void AddFileSize(FileSizeType added_file_size,
		FileSizeType granularity);

	FileSizeType GetFileSize();
	FileOffsetType GetOffset();
	FileOffsetType SetOffset(FileSeekOffsetType file_offset,
		FileSeekMethod seek_method);

	unsigned int ReadBlock(void *block_ptr, unsigned int block_length);
	unsigned int ReadBlockStrict(void *block_ptr, unsigned int block_length);
	unsigned int WriteBlock(const void *block_ptr, unsigned int block_length);
	unsigned int WriteBlockStrict(const void *block_ptr,
		unsigned int block_length);

	FileOffsetType GetInternalOffset() const;
	FileOffsetType SetInternalOffset(FileOffsetType new_file_offset);

	void TruncateToSize(FileSizeType new_file_size);
	virtual void Flush();

	std::string    file_name_;
	FileSizeType   file_size_;
	FileOffsetType file_offset_;				//	User-maintained.
	bool           is_read_only_;
	bool           is_new_file_;
	bool           truncate_to_offset_on_close_;

private:
	MLB_Utility_NonCopyable_Macro(OSFile);

	FileSizeType GetFileSize(const std::string &file_name);
	void IncreaseFileSizeInternal(FileSizeType new_file_size,
		FileSizeType granularity = 1);
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
class API_OSSUPPORT OSFileMMap : public OSFile {
public:
	OSFileMMap(const std::string &file_name = "");
	virtual ~OSFileMMap();

	bool IsMapped() const;
	bool IsOffsetMMap(FileOffsetType file_offset) const;
	bool IsExtentMMap(FileOffsetType extent_offset,
		FileSizeType extent_length) const;

	void CheckMapped() const;
	void CheckUnmapped() const;
	void CheckOffsetMMap(FileOffsetType file_offset) const;
	void CheckExtentMMap(FileOffsetType extent_offset,
		FileSizeType extent_length) const;
	virtual void CheckGranularity(FileSizeType granularity);

	void *GetPtr(FileOffsetType map_offset) const;
	void *GetPtr_void(FileOffsetType map_offset) const;
	char *GetPtr_char(FileOffsetType map_offset) const;

	char         *GetMMapPtr() const;

	void Close();
	bool CloseIfOpen();
	bool CloseIfOpenSafe();

	void MapFile();
	void MapFile(FileSizeType map_length, FileOffsetType map_offset);

	void ReMapFile();
	void ReMapFile(FileSizeType map_length, FileOffsetType map_offset);

	void UnMap();

	void IncreaseFileSize(FileSizeType new_file_size);
	void AddFileSize(FileSizeType added_file_size);

	void IncreaseFileSize(FileSizeType new_file_size,
		FileSizeType granularity);
	void AddFileSize(FileSizeType added_file_size,
		FileSizeType granularity);

	void EnsureSpaceForLengthFromOffset(FileSizeType extent_length,
		FileOffsetType extent_offset = 0);
	void EnsureExtentIsMapped(FileSizeType extent_length,
		FileOffsetType extent_offset = 0);

	virtual void MFlush();

	static unsigned int FixupGranularity(unsigned int granularity);
	static FileSizeType FixupGranularity(FileSizeType granularity);
	static void FixupLengthAndOffset(FileSizeType &map_length,
		FileOffsetType &map_offset, FileOffsetType &ptr_adjust);
	static FileSizeType GetOSPageGranularity();
	static FileSizeType GetMaxMapLength();

	FileSizeType    os_granularity_;
	FileOffsetType  mmap_offset_;
	FileSizeType    mmap_length_;
	char           *mmap_ptr_;
	FileSizeType    mmap_ptr_adjust_;
	HANDLE          mmap_handle_;

private:
	MLB_Utility_NonCopyable_Macro(OSFileMMap);
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#endif // #ifndef HH_MLB_OSSupport_File_h_HH

