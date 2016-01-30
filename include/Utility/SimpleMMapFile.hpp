//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for simple memory-map file use.

	Revision History	:	2004-10-02 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2004 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
#ifndef HH__MLB__SimpleMMapFile_hpp__HH

#define HH__MLB__SimpleMMapFile_hpp__HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
class API_UTILITY SimpleMMapFile {
public:
	SimpleMMapFile();
	SimpleMMapFile(const std::string &file_name, bool is_read_only = false,
		bool is_exclusive = false, unsigned int granularity = 0);
	virtual ~SimpleMMapFile();

	void Open(const std::string &file_name, bool is_read_only = false,
		bool is_exclusive = false, unsigned int granularity = 0);
	void Close();

	bool IsOpen() const;
	bool IsOpenFile() const;
	bool IsOpenMapping() const;

	bool IsReadOnly() const;
	bool IsNewFile() const;

	void CheckOpen() const;

	void IncreaseSize(unsigned int new_map_size,
		unsigned int reserve_at_start = 0);
	void AddToSize(unsigned int added_size);

	unsigned int GetOffset() const;
	void SetOffset(unsigned int new_offset);

	void Write(const std::string &out_data);
	void Write(const char *out_data);
	void Write(unsigned int out_length, const void *out_data);

	void *GetPtr(unsigned int file_offset) const;

	unsigned int  GetMMapSize() const;
	char         *GetMMapPtr() const;

	static unsigned int FixupGranularity(unsigned int granularity);

private:
	std::string   file_name_;
	bool          is_read_only_;
	bool          is_exclusive_;
	unsigned int  granularity_;
	bool          is_new_file_;
	unsigned int  current_offset_;
	unsigned int  mmap_size_;
	char         *mmap_ptr_;		//	'char *' For ease of address calculations...
	HANDLE        file_handle_;
	HANDLE        mmap_handle_;

	void CheckOffset(unsigned int file_offset) const;

	void OpenFileInternal();
	void MapFile(unsigned int new_map_size);
	void ReMapFile(unsigned int new_map_size, unsigned int reserve_at_start = 0);
	void ReMapFileInternal(const std::string &file_name, HANDLE file_handle,
		unsigned int granularity, HANDLE &mmap_handle, char *&mmap_ptr,
		bool is_read_only, unsigned int new_map_size);

	void CloseFile();
	void CloseMapping();
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__SimpleMMapFile_hpp__HH

