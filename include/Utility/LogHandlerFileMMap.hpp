//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
//	////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the memory-mapped log handler file
								class LogHandlerFileMMap.

	Revision History	:	2005-01-02 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2005 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
//	////////////////////////////////////////////////////////////////////////////
 
#ifndef HH__MLB__Utility__Utility__LogHandlerFileMMap_hpp__HH

#define HH__MLB__Utility__Utility__LogHandlerFileMMap_hpp__HH	1

//	////////////////////////////////////////////////////////////////////////////
//	////////////////////////////////////////////////////////////////////////////
//	Required include files...
//	////////////////////////////////////////////////////////////////////////////

#include <Utility/LogHandlerFileBase.hpp>

#ifdef _Windows
# pragma warning(push)
# pragma warning(disable:4061 4365)
#endif // #ifdef _Windows

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#ifdef _Windows
# pragma warning(pop)
#endif // #ifdef _Windows

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

//	////////////////////////////////////////////////////////////////////////////
class API_UTILITY LogHandlerFileMMap : public LogHandlerFileBase {
	typedef unsigned long long                 MyMappingValue;
	typedef boost::interprocess::file_mapping  MyFileMapping;
	typedef boost::interprocess::mapped_region MyMappedRegion;
	typedef boost::shared_ptr<MyFileMapping>   MyFileMappingSPtr;
	typedef boost::shared_ptr<MyMappedRegion>  MyMappedRegionSPtr;
public:
	LogHandlerFileMMap();
	explicit LogHandlerFileMMap(const char *file_name,
		LogHandlerFileBaseFlag flags = Default);
	explicit LogHandlerFileMMap(const std::string &file_name,
		LogHandlerFileBaseFlag flags = Default);
	LogHandlerFileMMap(const char *base_name, const char *dir_name,
		LogHandlerFileBaseFlag flags = Default);
	LogHandlerFileMMap(const std::string &base_name, const std::string &dir_name,
		LogHandlerFileBaseFlag flags = Default);
	LogHandlerFileMMap(const char *base_name, const char *dir_name,
		const MLB::Utility::TimeT &start_time,
		LogHandlerFileBaseFlag flags = Default);
	LogHandlerFileMMap(const std::string &base_name, const std::string &dir_name,
		const MLB::Utility::TimeT &start_time,
		LogHandlerFileBaseFlag flags = Default);

	virtual ~LogHandlerFileMMap();

protected:
	virtual void InstallHandlerImpl();
	virtual void RemoveHandlerImpl();
	virtual void OpenFileImpl(const char *file_name);
	virtual void FlushImpl();
	virtual void EmitLineImpl(const LogEmitControl &emit_control);
	virtual void EmitLiteralImpl(unsigned int literal_length,
		const char *literal_string);

	std::string        eol_string_;
	std::size_t        eol_string_length_;
	unsigned int       page_alloc_size_;
	unsigned int       chunk_alloc_size_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4251)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
	MyFileMappingSPtr  mapping_sptr_;
	MyMappedRegionSPtr region_sptr_;
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
	unsigned int       mapping_size_;
	MyMappingValue     mapping_offset_;
	MyMappingValue     write_offset_;

private:
	void *GetCurrentPtr()
	{
		return(static_cast<void *>(static_cast<char *>(
			region_sptr_->get_address()) + write_offset_));
	}

	void AddToOffset(std::size_t added_offset)
	{
		if ((write_offset_ + added_offset) > mapping_size_)
			ThrowInvalidArgument("The value specified to 'LogHandlerFileMMap::"
				"AddToOffset()' (" + AnyToString(added_offset) + ") when added "
				"to the current 'write_offset_' value (" +
				AnyToString(write_offset_) + ") is greater than the "
				"'mapped_size_' member (" + AnyToString(mapping_size_) + ").");

		write_offset_ += added_offset;
	}

	void EnsureNeededSpace(std::size_t needed_length);

	MLB_Utility_NonCopyable_Macro(LogHandlerFileMMap);
};
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__Utility__LogHandlerFileMMap_hpp__HH

