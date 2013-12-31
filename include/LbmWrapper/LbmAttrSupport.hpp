// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	29 West LBM API Wrapper Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for support 29 West LBM attribute and
								configuration logic.

	Revision History	:	2008-08-16 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__LbmWrapper__LbmAttrSupport_hpp__HH

#define HH__MLB__LbmWrapper__LbmAttrSupport_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <LbmWrapper/LbmWrapper.hpp>

#include <Utility/ValueToStringRadix.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LbmWrapper {

//	////////////////////////////////////////////////////////////////////////////
template <typename LbmApiType>
	struct LbmAttributeFuncs {
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <> struct LbmAttributeFuncs<lbm_context_attr_t> {
	typedef lbm_context_attr_t LbmApiType;
	static int AttrOptionSize()
	{
		return(::lbm_context_attr_option_size());
	}
#if LBM_VERS_MAJOR < 4
	static void AttrInit(LbmApiType *attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_context_attr_init,
			(attr));
	}
#endif // #if LBM_VERS_MAJOR < 4
	static int AttrDump(LbmApiType *attr, int *size, lbm_config_option_t *opts)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_context_attr_dump,
			(attr, size, opts));

		return(0);
	}
	static void AttrCreate(LbmApiType **attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_context_attr_create,
			(attr));
	}
	static void AttrCreateDefault(LbmApiType **attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_context_attr_create_default,
			(attr));
	}
#if LBM_VERS_MAJOR < 4
	static void AttrCleanup(LbmApiType *attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_context_attr_cleanup,
			(attr));
	}
#endif // #if LBM_VERS_MAJOR < 4
	static void AttrDelete(LbmApiType *attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_context_attr_delete,
			(attr));
	}
	static void AttrDup(LbmApiType **dst_attr, const LbmApiType *src_attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_context_attr_dup,
			(dst_attr, src_attr));
	}
	static void AttrSetOpt(LbmApiType *attr, const char *optname, void *optval,
		std::size_t optlen)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_context_attr_setopt,
			(attr, optname, optval, optlen));
	}
	static void AttrStrSetOpt(LbmApiType *attr, const char *optname,
		const char *optval)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_context_attr_str_setopt,
			(attr, optname, optval));
	}
	static void AttrGetOpt(LbmApiType *attr, const char *optname,
		void *optval, std::size_t *optlen)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_context_attr_getopt,
			(attr, optname, optval, optlen));
	}
	static void AttrStrGetOpt(LbmApiType *attr, const char *optname,
		char *optval, std::size_t *optlen)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_context_attr_str_getopt,
			(attr, optname, optval, optlen));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <> struct LbmAttributeFuncs<lbm_rcv_topic_attr_t> {
	typedef lbm_rcv_topic_attr_t LbmApiType;
	static int AttrOptionSize()
	{
		return(::lbm_rcv_topic_attr_option_size());
	}
#if LBM_VERS_MAJOR < 4
	static void AttrInit(LbmApiType *attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_rcv_topic_attr_init,
			(attr));
	}
#endif // #if LBM_VERS_MAJOR < 4
	static int AttrDump(LbmApiType *attr, int *size, lbm_config_option_t *opts)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_rcv_topic_attr_dump,
			(attr, size, opts));

		return(0);
	}
	static void AttrCreate(LbmApiType **attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_rcv_topic_attr_create,
			(attr));
	}
	static void AttrCreateDefault(LbmApiType **attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_rcv_topic_attr_create_default,
			(attr));
	}
#if LBM_VERS_MAJOR < 4
	static void AttrCleanup(LbmApiType *attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_rcv_topic_attr_cleanup,
			(attr));
	}
#endif // #if LBM_VERS_MAJOR < 4
	static void AttrDelete(LbmApiType *attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_rcv_topic_attr_delete,
			(attr));
	}
	static void AttrDup(LbmApiType **dst_attr, const LbmApiType *src_attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_rcv_topic_attr_dup,
			(dst_attr, src_attr));
	}
	static void AttrSetOpt(LbmApiType *attr, const char *optname, void *optval,
		std::size_t optlen)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_rcv_topic_attr_setopt,
			(attr, optname, optval, optlen));
	}
	static void AttrStrSetOpt(LbmApiType *attr, const char *optname,
		const char *optval)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_rcv_topic_attr_str_setopt,
			(attr, optname, optval));
	}
	static void AttrGetOpt(LbmApiType *attr, const char *optname,
		void *optval, std::size_t *optlen)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_rcv_topic_attr_getopt,
			(attr, optname, optval, optlen));
	}
	static void AttrStrGetOpt(LbmApiType *attr, const char *optname,
		char *optval, std::size_t *optlen)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_rcv_topic_attr_str_getopt,
			(attr, optname, optval, optlen));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <> struct LbmAttributeFuncs<lbm_src_topic_attr_t> {
	typedef lbm_src_topic_attr_t LbmApiType;
	static int AttrOptionSize()
	{
		return(::lbm_src_topic_attr_option_size());
	}
#if LBM_VERS_MAJOR < 4
	static void AttrInit(LbmApiType *attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_src_topic_attr_init,
			(attr));
	}
#endif // #if LBM_VERS_MAJOR < 4
	static int AttrDump(LbmApiType *attr, int *size, lbm_config_option_t *opts)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_src_topic_attr_dump,
			(attr, size, opts));

		return(0);
	}
	static void AttrCreate(LbmApiType **attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_src_topic_attr_create,
			(attr));
	}
	static void AttrCreateDefault(LbmApiType **attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_src_topic_attr_create_default,
			(attr));
	}
#if LBM_VERS_MAJOR < 4
	static void AttrCleanup(LbmApiType *attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_src_topic_attr_cleanup,
			(attr));
	}
#endif // #if LBM_VERS_MAJOR < 4
	static void AttrDelete(LbmApiType *attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_src_topic_attr_delete,
			(attr));
	}
	static void AttrDup(LbmApiType **dst_attr, const LbmApiType *src_attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_src_topic_attr_dup,
			(dst_attr, src_attr));
	}
	static void AttrSetOpt(LbmApiType *attr, const char *optname, void *optval,
		std::size_t optlen)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_src_topic_attr_setopt,
			(attr, optname, optval, optlen));
	}
	static void AttrStrSetOpt(LbmApiType *attr, const char *optname,
		const char *optval)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_src_topic_attr_str_setopt,
			(attr, optname, optval));
	}
	static void AttrGetOpt(LbmApiType *attr, const char *optname,
		void *optval, std::size_t *optlen)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_src_topic_attr_getopt,
			(attr, optname, optval, optlen));
	}
	static void AttrStrGetOpt(LbmApiType *attr, const char *optname,
		char *optval, std::size_t *optlen)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_src_topic_attr_str_getopt,
			(attr, optname, optval, optlen));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <> struct LbmAttributeFuncs<lbm_event_queue_attr_t> {
	typedef lbm_event_queue_attr_t LbmApiType;
	static int AttrOptionSize()
	{
		return(::lbm_event_queue_attr_option_size());
	}
#if LBM_VERS_MAJOR < 4
	static void AttrInit(LbmApiType *attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_event_queue_attr_init,
			(attr));
	}
#endif // #if LBM_VERS_MAJOR < 4
	static int AttrDump(LbmApiType *attr, int *size, lbm_config_option_t *opts)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_event_queue_attr_dump,
			(attr, size, opts));

		return(0);
	}
	static void AttrCreate(LbmApiType **attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_event_queue_attr_create,
			(attr));
	}
	static void AttrCreateDefault(LbmApiType **attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_event_queue_attr_create_default,
			(attr));
	}
#if LBM_VERS_MAJOR < 4
	static void AttrCleanup(LbmApiType *attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_event_queue_attr_cleanup,
			(attr));
	}
#endif // #if LBM_VERS_MAJOR < 4
	static void AttrDelete(LbmApiType *attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_event_queue_attr_delete,
			(attr));
	}
	static void AttrDup(LbmApiType **dst_attr, const LbmApiType *src_attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_event_queue_attr_dup,
			(dst_attr, src_attr));
	}
	static void AttrSetOpt(LbmApiType *attr, const char *optname, void *optval,
		std::size_t optlen)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_event_queue_attr_setopt,
			(attr, optname, optval, optlen));
	}
	static void AttrStrSetOpt(LbmApiType *attr, const char *optname,
		const char *optval)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_event_queue_attr_str_setopt,
			(attr, optname, optval));
	}
	static void AttrGetOpt(LbmApiType *attr, const char *optname,
		void *optval, std::size_t *optlen)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_event_queue_attr_getopt,
			(attr, optname, optval, optlen));
	}
	static void AttrStrGetOpt(LbmApiType *attr, const char *optname,
		char *optval, std::size_t *optlen)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_event_queue_attr_str_getopt,
			(attr, optname, optval, optlen));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <> struct LbmAttributeFuncs<lbm_wildcard_rcv_attr_t> {
	typedef lbm_wildcard_rcv_attr_t LbmApiType;
	static int AttrOptionSize()
	{
		return(::lbm_wildcard_rcv_attr_option_size());
	}
#if LBM_VERS_MAJOR < 4
	static void AttrInit(LbmApiType *attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_wildcard_rcv_attr_init,
			(attr));
	}
#endif // #if LBM_VERS_MAJOR < 4
	static int AttrDump(LbmApiType *attr, int *size, lbm_config_option_t *opts)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_wildcard_rcv_attr_dump,
			(attr, size, opts));

		return(0);
	}
	static void AttrCreate(LbmApiType **attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_wildcard_rcv_attr_create,
			(attr));
	}
	static void AttrCreateDefault(LbmApiType **attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_wildcard_rcv_attr_create_default,
			(attr));
	}
#if LBM_VERS_MAJOR < 4
	static void AttrCleanup(LbmApiType *attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_wildcard_rcv_attr_cleanup,
			(attr));
	}
#endif // #if LBM_VERS_MAJOR < 4
	static void AttrDelete(LbmApiType *attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_wildcard_rcv_attr_delete,
			(attr));
	}
	static void AttrDup(LbmApiType **dst_attr, const LbmApiType *src_attr)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_wildcard_rcv_attr_dup,
			(dst_attr, src_attr));
	}
	static void AttrSetOpt(LbmApiType *attr, const char *optname, void *optval,
		std::size_t optlen)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_wildcard_rcv_attr_setopt,
			(attr, optname, optval, optlen));
	}
	static void AttrStrSetOpt(LbmApiType *attr, const char *optname,
		const char *optval)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_wildcard_rcv_attr_str_setopt,
			(attr, optname, optval));
	}
	static void AttrGetOpt(LbmApiType *attr, const char *optname,
		void *optval, std::size_t *optlen)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_wildcard_rcv_attr_getopt,
			(attr, optname, optval, optlen));
	}
	static void AttrStrGetOpt(LbmApiType *attr, const char *optname,
		char *optval, std::size_t *optlen)
	{
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_wildcard_rcv_attr_str_getopt,
			(attr, optname, optval, optlen));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef LbmConfigOptionList::iterator       LbmConfigOptionListIter;
typedef LbmConfigOptionList::const_iterator LbmConfigOptionListIterC;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
struct LbmConfigOptionLessThan {
	bool operator () (const lbm_config_option_t &lhs,
		const lbm_config_option_t &rhs) const;
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename ApiType> class ConfigDumpHelper {
	typedef int (*ActualDumperFunc)(ApiType *, int *, lbm_config_option_t *);
public:
	static void DumpConfig(ActualDumperFunc dumper_func, ApiType *api_type_ptr,
		int *size, lbm_config_option_t *opts)
	{
		(*dumper_func)(api_type_ptr, size, opts);
	}

	static LbmConfigOptionList &DumpConfig(ActualDumperFunc dumper_func,
		ApiType *api_type_ptr, LbmConfigOptionList &out_list)
	{
		LbmConfigOptionList tmp_list(1024);

		/*
			The funky 29 West API doesn't provide a way to tell us how large
			the array needs to be in order to get all of the configuration
			options. And this despite taking a pointer to the size argument!

			So we loop until the updated 'size' is less than the allocated size.
			We thus know that we've got all of the option value array elements.
		*/
		for ( ; ; ) {
			int tmp_size = static_cast<int>(tmp_list.size());
			DumpConfig(dumper_func, api_type_ptr, &tmp_size, &(tmp_list[0]));
			if (static_cast<std::size_t>(tmp_size) < tmp_list.size()) {
				out_list.swap(tmp_list);
				out_list.resize(static_cast<std::size_t>(tmp_size));
				break;
			}
			tmp_list.resize(tmp_list.size() + 1024);
		}

		return(out_list);
	}

	static LbmConfigOptionList DumpConfig(ActualDumperFunc dumper_func,
		ApiType *api_type_ptr)
	{
		LbmConfigOptionList out_list;

		return(DumpConfig(dumper_func, api_type_ptr, out_list));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
template <typename LbmApiType>
	class LbmAttributeBase : public LbmObjectBase<LbmApiType> {
	void Initialize()
	{
		LbmApiType *tmp_ptr;

		LbmAttributeFuncs<LbmApiType>::AttrCreate(
			(&tmp_ptr));

		this->object_sptr_.
			reset(tmp_ptr, LbmAttributeFuncs<LbmApiType>::AttrDelete);
	}

	void RethrowGetOptException(const char *optname,
		const std::exception &except)
	{
		std::ostringstream o_str;

		o_str << "Attempt to get option '" << optname << "' value failed: " <<
			except.what();
		MLB::Utility::Rethrow(except, o_str);
	}

public:
	LbmAttributeBase()
		:LbmObjectBase<LbmApiType>()
	{
		Initialize();
	}
	explicit LbmAttributeBase(LbmApiType *api_type_ptr)
		:LbmObjectBase<LbmApiType>(api_type_ptr)
	{
	}

	static int OptionSize()
	{
		return(LbmAttributeFuncs<LbmApiType>::AttrOptionSize());
	}

	void SetOpt(const char *optname, void *optval, std::size_t optlen)
	{
		try {
			LbmAttributeFuncs<LbmApiType>::AttrSetOpt(
				this->object_sptr_.get(), optname, optval, optlen);
		}
		catch (const std::exception &except) {
			std::ostringstream o_str;
			o_str << "Attempt to set option '" << optname << "' to the option "
				"value of length " << optlen << " at address " <<
				MLB::Utility::ValueToStringHex(optval) << " failed: " <<
				except.what();
			MLB::Utility::Rethrow(except, o_str);
		}
	}
	void SetOpt(const std::string &optname, void *optval, std::size_t optlen)
	{
		SetOpt(optname.c_str(), optval, optlen);
	}
	void SetOpt(const char *optname, const char *optval)
	{
		try {
			LbmAttributeFuncs<LbmApiType>::AttrStrSetOpt(
				this->object_sptr_.get(), optname, optval);
		}
		catch (const std::exception &except) {
			std::ostringstream o_str;
			o_str << "Attempt to set option '" << optname << "' to the string "
				"option value ";
			if (optval != NULL)
				o_str << "'" << optval << "' failed: " << except.what();
			else
				o_str << "NULL failed: " << except.what();
			MLB::Utility::Rethrow(except, o_str);
		}
	}
	void SetOpt(const std::string &optname, const char *optval)
	{
		SetOpt(optname.c_str(), optval);
	}
	void SetOpt(const char *optname, const std::string &optval)
	{
		SetOpt(optname, optval.c_str());
	}
	void SetOpt(const std::string &optname, const std::string &optval)
	{
		SetOpt(optname.c_str(), optval.c_str());
	}

	//	Added to provide orthgonality to the underlying LBM API.
	void StrSetOpt(const char *optname, const char *optval)
	{
		SetOpt(optname, optval);
	}

	void GetOpt(const char *optname, void *optval, std::size_t *optlen)
	{
		try {
			LbmAttributeFuncs<LbmApiType>::AttrGetOpt(
				this->object_sptr_.get(), optname, optval, optlen);
		}
		catch (const std::exception &except) {
			RethrowGetOptException(optname, except);
		}
	}
	void GetOpt(const std::string &optname, void *optval, std::size_t *optlen)
	{
		GetOpt(optname.c_str(), optval, optlen);
	}
	void GetOpt(const char *optname, char *optval, std::size_t *optlen)
	{
		try {
			LbmAttributeFuncs<LbmApiType>::AttrStrGetOpt(
				this->object_sptr_.get(), optname, optval, optlen);
		}
		catch (const std::exception &except) {
			RethrowGetOptException(optname, except);
		}
	}
	void GetOpt(const std::string &optname, char *optval, std::size_t *optlen)
	{
		GetOpt(optname.c_str(), optval, optlen);
	}

	std::string &GetOpt(const char *optname, std::string &optval)
	{
		char                    get_buffer[8192];
		std::size_t             get_length = sizeof(get_buffer);
		boost::shared_ptr<char> get_sptr(get_buffer,
											LbmObjectBase<LbmApiType>::MyNullDeleter());

		for ( ; ; ) {
			try {
				std::size_t tmp_length = get_length;
				GetOpt(optname, get_sptr.get(), &tmp_length);
				if (!tmp_length)
					optval.clear();
				else
					optval.assign(get_sptr.get(), tmp_length);
				break;
			}
			catch (const LbmExceptionStatus &except) {
				if (except.GetStatus() != 1)
					throw;
				get_length += 65536;
				boost::shared_ptr<char>(new char[get_length]).swap(get_sptr);
			}
		}

		return(optval);
	}

	std::string &GetOpt(const std::string &optname, std::string &optval)
	{
		return(GetOpt(optname.c_str(), optval));
	}

	std::string GetOpt(const char *optname)
	{
		std::string optval;

		return(GetOpt(optname, optval));
	}
	std::string GetOpt(const std::string &optname)
	{
		return(GetOpt(optname.c_str()));
	}

	//	Added to provide orthgonality to the underlying LBM API.
	void StrGetOpt(const char *optname, char *optval, std::size_t *optlen)
	{
		GetOpt(optname, optval, optlen);
	}

	void Dump(int *size, lbm_config_option_t *opts)
	{
		LbmAttributeFuncs<LbmApiType>::AttrDump(
			this->object_sptr_.get(), size, opts);
	}

	LbmConfigOptionList &Dump(LbmConfigOptionList &out_list)
	{
		return(ConfigDumpHelper<LbmApiType>::DumpConfig(
			LbmAttributeFuncs<LbmApiType>::AttrDump, this->object_sptr_.get(),
			out_list));
	}

	LbmConfigOptionList Dump()
	{
		LbmConfigOptionList out_list;

		return(Dump(out_list));
	}
};
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
typedef LbmAttributeBase<lbm_context_attr_t>      LbmAttrContext;
typedef LbmAttributeBase<lbm_rcv_topic_attr_t>    LbmAttrTopicRcv;
typedef LbmAttributeBase<lbm_src_topic_attr_t>    LbmAttrTopicSrc;
typedef LbmAttributeBase<lbm_wildcard_rcv_attr_t> LbmAttrWildcardRcv;
typedef LbmAttributeBase<lbm_event_queue_attr_t>  LbmAttrEventQueue;
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
std::size_t GetMaxOptionNameLength(const LbmConfigOptionList &opt_list);

std::string GetFormattedString(const LbmConfigOptionList &opt_list);

std::ostream & operator << (std::ostream &o_str,
	const LbmConfigOptionList &datum);
//	////////////////////////////////////////////////////////////////////////////

} // namespace LbmWrapper

} // namespace MLB

#endif // #ifndef HH__MLB__LbmWrapper__LbmAttrSupport_hpp__HH

