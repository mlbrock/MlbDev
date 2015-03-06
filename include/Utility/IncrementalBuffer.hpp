// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for incremental buffer support.

	Revision History	:	2004-06-19 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2004 - 2015.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility_IncrementalBuffer_hpp__HH

#define HH__MLB__Utility_IncrementalBuffer_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/AlignmentSupport.hpp>
#include <Utility/Utility_Exception.hpp>

#include <BoostX.hpp>

#ifdef _Windows
# pragma warning(push)
# pragma warning(disable:4217 4668)
#endif // #ifdef _Windows

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#ifdef _Windows
# pragma warning(pop)
#endif // #ifdef _Windows

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
template <
	typename DatumType = char, std::size_t InitialCount = 1>
	class IncrementalBuffer {
public:
	typedef typename boost::shared_ptr<DatumType> DatumTypeSPtr;
	IncrementalBuffer(std::size_t alloc_granularity = 1)
		:buffer_sptr_(buffer_default_, MLB::BoostX::NullDeleter())
		,allocation_count_((InitialCount < 1) ? 1 : InitialCount)
		,buffer_count_(0)
		,alloc_granularity_((alloc_granularity < 1) ? 1 : alloc_granularity)
	{
		memset(buffer_default_, '\0', sizeof(buffer_default_));
	}

	bool         IsHeapPtr() const
	{
		return(buffer_default_ != buffer_sptr_.get());
	}
	std::size_t  GetAllocationCount() const
	{
		return(allocation_count_);
	}
	std::size_t  GetAllocationGranularity() const
	{
		return(alloc_granularity_);
	}
	std::size_t  GetCount() const
	{
		return(buffer_count_);
	}
	template <typename CountAsType>
		CountAsType  GetAllocationCountAsType() const
	{
		return(CheckCountInAsTypeRange<CountAsType>(GetAllocationCount(),
			"allocation_count_"));
	}
	template <typename CountAsType>
		CountAsType  GetAllocationGranularityAsType() const
	{
		return(CheckCountInAsTypeRange<CountAsType>(GetAllocationGranularity(),
			"alloc_granularity_"));
	}
	template <typename CountAsType>
		CountAsType  GetCountAsType() const
	{
		return(CheckCountInAsTypeRange<CountAsType>(GetCount(),
			"buffer_count_"));
	}
	DatumType   *GetPtr()
	{
		return(buffer_sptr_.get());
	}

	bool SetValue(const DatumType *src_ptr, std::size_t src_count)
	{
		return(SetValue(static_cast<const void *>(src_ptr), src_count));
	}
	bool SetValue(const void *src_ptr, std::size_t src_count)
	{
		bool ptr_changed_flag = SetCount(src_count, false);
	
		memcpy(buffer_sptr_.get(), src_ptr, src_count);

		return(ptr_changed_flag);
	}

	bool SetAllocationCount(std::size_t new_count, bool copy_data_flag = true)
	{
		if (new_count <= allocation_count_)
			return(false);

		std::size_t   new_allocation_count =
			MLB::Utility::GranularRoundUp(new_count, alloc_granularity_);
		DatumTypeSPtr tmp_buffer_sptr(new DatumType[new_allocation_count]);

		if (copy_data_flag)
			memcpy(tmp_buffer_sptr.get(), buffer_sptr_.get(), buffer_count_);

		buffer_sptr_.swap(tmp_buffer_sptr);

		allocation_count_ = new_allocation_count;

		return(true);
	}

	bool SetCount(std::size_t new_count, bool copy_data_flag = true)
	{
		bool ptr_changed_flag = SetAllocationCount(new_count, copy_data_flag);

		buffer_count_ = new_count;

		return(ptr_changed_flag);
	}

private:
	//** Compilation fails if \e InitialCount is 0. This is a feature.
	DatumType     buffer_default_[InitialCount];
	DatumTypeSPtr buffer_sptr_;
	std::size_t   allocation_count_;
	std::size_t   buffer_count_;
	std::size_t   alloc_granularity_;

	template <typename CountAsType>
		CountAsType  CheckCountInAsTypeRange(std::size_t count_value,
		const char *count_name) const
	{
		if (static_cast<long long>(count_value) >
			static_cast<long long>(std::numeric_limits<CountAsType>::max()))
			MLB::Utility::ThrowInvalidArgument("The size of the specified "
				"count-as-type source value for the " + std::string(count_name) +
				" member (" + MLB::Utility::AnyToString(count_value) + ") "
				"exceeds the maximum possible for the destination value (" +
				MLB::Utility::AnyToString(
				std::numeric_limits<CountAsType>::max()) + ").");
		return(static_cast<CountAsType>(count_value));
	}

	MLB_Utility_NonCopyable_Macro(IncrementalBuffer);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility_IncrementalBuffer_hpp__HH

