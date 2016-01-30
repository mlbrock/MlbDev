// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	29 West LBM API Wrapper Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the LbmSrc class.

	Revision History	:	2008-08-16 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <LbmWrapper/LbmAttrSupport.hpp>

#include <string.h>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LbmWrapper {

//	////////////////////////////////////////////////////////////////////////////
LbmSrc::LbmSrc(LbmContext &context, LbmTopicSrc &topic)
	:LbmObjectBase<lbm_src_t>()
{
	Initialize(context, topic, NULL, NULL, NULL);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmSrc::LbmSrc(LbmContext &context, const char *topic_name)
	:LbmObjectBase<lbm_src_t>()
{
	LbmTopicSrc topic(context, topic_name);

	Initialize(context, topic, NULL, NULL, NULL);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmSrc::LbmSrc(LbmContext &context, const std::string &topic_name)
	:LbmObjectBase<lbm_src_t>()
{
	LbmTopicSrc topic(context, topic_name);

	Initialize(context, topic, NULL, NULL, NULL);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LbmSrc::Initialize(LbmContext &context, LbmTopicSrc &topic,
	lbm_src_cb_proc call_back, void *user_data_ptr,
	lbm_event_queue_t *event_queue)
{
	lbm_src_t *tmp_ptr;

	WRAP29_LBM_THROW_IF_NE_ZERO(
		::lbm_src_create,
		(&tmp_ptr, context.GetObjectPtr(), topic.GetObjectPtr(), call_back,
			user_data_ptr, event_queue));

	object_sptr_.reset(tmp_ptr, ::lbm_src_delete);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LbmSrc::Send(const std::string &msg, int flags)
{
	Send(msg.data(), msg.size(), flags);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LbmSrc::Send(const void *msg_ptr, std::size_t msg_length, int flags)
{
	Send(static_cast<const char *>(msg_ptr), msg_length, flags);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LbmSrc::Send(const char *msg_ptr, std::size_t msg_length, int flags)
{
	WRAP29_LBM_THROW_IF_NE_ZERO(
		::lbm_src_send,
		(GetObjectPtr(), msg_ptr, msg_length, flags));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LbmSrc::SendAsciiZ(const char *msg_ptr, int flags)
{
	Send(msg_ptr, ::strlen(msg_ptr), flags);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void LbmSrc::Dump(int *size, lbm_config_option_t *opts)
{
	ConfigDumpHelper<lbm_src_t>::DumpConfig(::lbm_src_topic_dump,
		GetObjectPtr(), size, opts);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmConfigOptionList &LbmSrc::Dump(LbmConfigOptionList &out_list)
{
	return(ConfigDumpHelper<lbm_src_t>::DumpConfig(::lbm_src_topic_dump,
		GetObjectPtr(), out_list));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
LbmConfigOptionList LbmSrc::Dump()
{
	LbmConfigOptionList out_list;

	return(Dump(out_list));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace LbmWrapper

} // namespace MLB

#ifdef TEST_MAIN

#include <LbmWrapper/LbmMiscSupport.hpp>
#include <LbmWrapper/LbmLogSupport.hpp>

#include <Utility/Sleep.hpp>
#include <Utility/HRTimer.hpp>
#include <Utility/StringSupport.hpp>
#include <Utility/OSSocketLibraryContext.hpp>

using namespace MLB::LbmWrapper;

//	////////////////////////////////////////////////////////////////////////////
void TEST_GetContextOptionsDump_1(LbmContext &lbm_context)
{
/*
	int                  size = 0;
	lbm_config_option_t *opts = NULL;
*/
	lbm_config_option_t opts[10];
	int                 size = sizeof(opts) / sizeof(opts[0]);

	try {
		WRAP29_LBM_THROW_IF_NE_ZERO(
			::lbm_context_dump,
			(lbm_context.GetObjectPtr(), &size, opts));
std::cerr << "***** ::lbm_errnum() = " << ::lbm_errnum() << std::endl;
		for (int count_1 = 0; count_1 < size; ++count_1)
			std::cout
				<< opts[count_1].type  << "|"
				<< opts[count_1].oname << "|"
				<< opts[count_1].val
				<< std::endl
					;
		std::cout << std::endl;
		LbmConfigOptionList opt_list(lbm_context.Dump());
		std::cout << opt_list << std::endl << std::endl;
	}
	catch (const std::exception &except) {
std::cerr << "***** ::lbm_errnum() = " << ::lbm_errnum() << std::endl;
		std::cerr << "Attempt to get an attribute dump with size = " << size <<
			" failed: " << except.what() << std::endl;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_GetContextOptionsDump_2()
{
	try {
		LbmAttrContext      attr_context;
//		LbmConfigOptionList opt_list(attr_context.Dump());

LbmConfigOptionList opt_list;
opt_list = ConfigDumpHelper<lbm_context_attr_t>::DumpConfig(
	::lbm_context_attr_dump, attr_context.GetObjectPtr());
std::cout << opt_list << std::endl;

opt_list = ConfigDumpHelper<lbm_context_attr_t>::DumpConfig(
	LbmAttributeFuncs<lbm_context_attr_t>::AttrDump, attr_context.GetObjectPtr());
std::cout << opt_list << std::endl;

opt_list = attr_context.Dump();
std::cout << opt_list << std::endl;

	}
	catch (const std::exception &except) {
		std::cerr << "Attempt to get an attribute dump failed: " <<
			except.what() << std::endl;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_GetStrAttr(LbmAttrTopicSrc &tsrc_attr, char *get_buffer,
	std::size_t &get_length, std::size_t try_length)
{
	try {
		get_length = try_length;
		*get_buffer = '\0';
		std::cerr << "Getting with buffer length = " << get_length << std::endl;
		tsrc_attr.GetOpt("transport", get_buffer, &get_length);
		std::cerr << ">>> OK    : string=[" << get_buffer << "], length=" <<
			get_length << std::endl;
	}
	catch (const std::exception &except) {
		std::cerr << ">>> FAILED: " << except.what() << std::endl;
		std::cerr << ">>> FAILED: string=[" << get_buffer << "], length=" <<
			get_length << std::endl;
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_AttributeTest()
{
	std::cerr << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	try {
		const char      *t_name_ptr = "lbtrm";
		std::size_t      t_name_len = ::strlen(t_name_ptr);
		LbmAttrTopicSrc  tsrc_attr;
		std::cerr << "Setting the 'transport' attribute to '" << t_name_ptr <<
			"' (" << t_name_len << " bytes in length)." << std::endl;
		tsrc_attr.SetOpt("transport", t_name_ptr);
		char        get_buffer[255 + 1];
		std::size_t get_length;
		std::string smart_get(tsrc_attr.GetOpt("transport"));
		std::cerr << "Getting using the adaptive 'std::string' method: [" <<
			smart_get << "]" << std::endl;
		t_name_len = smart_get.size();
		TEST_GetStrAttr(tsrc_attr, get_buffer, get_length, sizeof(get_buffer));
		TEST_GetStrAttr(tsrc_attr, get_buffer, get_length, 80);
		TEST_GetStrAttr(tsrc_attr, get_buffer, get_length, 79);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "MLB 'LbmAttrTopicSrc' class test "
			"failed: " + std::string(except.what()));
	}

	std::cerr << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cerr << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_TransportMcAddressSingle(const std::string &mc_address)
{
	try {
		const char *transport_name = "lbt-rm";
		std::cout << "Set source MC address to " << mc_address << ": ";
		std::string topic_suffix(MLB::Utility::Trim(mc_address));
		{
			std::string::iterator iter_b(topic_suffix.begin());
			std::string::iterator iter_e(topic_suffix.end());
			for ( ; iter_b != iter_e; ++iter_b) {
				if (*iter_b == '.')
					*iter_b = '_';
			}
		}
		LbmAttrContext context_attr;
#ifdef _MSC_VER
		context_attr.SetOpt("fd_management_type", "wincompport");
#else
		context_attr.SetOpt("fd_management_type", "epoll");
#endif // #ifdef _MSC_VER
		LbmContext lbm_context(context_attr);
		LbmAttrTopicSrc topic_src_attr;
		topic_src_attr.SetOpt("transport", transport_name);
		topic_src_attr.SetOpt("transport_lbtrm_multicast_address", mc_address);
/*
		LbmTopicSrc topic_src(lbm_context, "TransportTest." + topic_suffix,
			topic_src_attr);
*/
LbmTopicSrc topic_src(lbm_context, "TransportTest", topic_src_attr);
		LbmSrc      src(lbm_context, topic_src);
		bool        mc_text_found = false;
		std::vector<std::string> attr_list(MLB::Utility::GetSplit(
			GetFormattedString(src.Dump()), "\n"));
		{
			std::vector<std::string>::const_iterator iter_b(attr_list.begin());
			std::vector<std::string>::const_iterator iter_e(attr_list.end());
			for ( ; iter_b != iter_e; ++iter_b) {
				if (::strstr(iter_b->c_str(),
					"transport_lbtrm_multicast_address            :") != NULL) {
					std::cout << *iter_b << std::endl;
					mc_text_found = true;
					break;
				}
			}
		}
		if (!mc_text_found)
			MLB::Utility::ThrowLogicError("Unable to locate the source MC "
				"address attribute value!!!");
		MLB::Utility::SleepSecs(2);
		src.Send("Sending LBM message on MC address '" + topic_suffix +
				"' --- Hello!", LBM_SRC_NONBLOCK);
		MLB::Utility::SleepSecs(2);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to construct a source on the "
			"specified multicast address '" + mc_address + "' failed: " +
			std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_TransportMcAddressMulti()
{
	std::cerr << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	for (int count_1 = 0; count_1 < 3; ++count_1) {
		try {
			if (count_1)
				std::cerr << MLB::Utility::PadLeft("", 79, '-') << std::endl;
			std::ostringstream o_str;
			o_str << "239.255.1." << (count_1 + 1);
			TEST_TransportMcAddressSingle(o_str.str());
		}
		catch (const std::exception &except) {
			std::cout << "ERROR: " << except.what() << std::endl;
		}
	}

	std::cerr << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cerr << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void TEST_STRESS_MinSrc(const char *transport_name)
{
	std::cerr << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	unsigned int iter_count = 1000000;
	unsigned int this_iter  = 0;

	char message[64];

	::memset(message, ' ', sizeof(message) - 1);
	::memcpy(message, "A 64-byte message.", 18);
	message[sizeof(message) - 1] = '\0';

	try {
		LbmAttrContext context_attr;
#ifdef _MSC_VER
		context_attr.SetOpt("fd_management_type", "wincompport");
#else
		context_attr.SetOpt("fd_management_type", "epoll");
#endif // #ifdef _MSC_VER
		LbmContext      lbm_context(context_attr);
		LbmAttrTopicSrc topic_src_attr;
		topic_src_attr.SetOpt("implicit_batching_minimum_length", "8192");
		topic_src_attr.SetOpt("transport", transport_name);
		LbmTopicSrc topic_src(lbm_context, "Greeting", topic_src_attr);
		LbmSrc      src(lbm_context, topic_src);
		std::cerr << "Sleeping 3 seconds before starting send..." << std::flush;
		MLB::Utility::SleepSecs(3);
		std::cerr << " done." << std::endl;
		std::cerr << "Sending " << iter_count << " messages with message "
			"size " << sizeof(message) << ":" << std::flush;
		MLB::Utility::HRTimer hr_timer;
		hr_timer.Start();
		for ( ; this_iter < iter_count; ++this_iter)
			src.Send(message, sizeof(message), LBM_SRC_BLOCK);
		hr_timer.End();
		std::cerr << " done in " << hr_timer.GetTickDiffInSeconds() <<
			" seconds." << std::endl;
		MLB::Utility::SleepSecs(2);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "MLB stress test analogue of the Minimal "
			"LBM Source Implementation minsrc.c failed on iteration " +
			MLB::Utility::AnyToString(this_iter) + " of " +
			MLB::Utility::AnyToString(iter_count) + " iterations: " +
			std::string(except.what()));
	}

	std::cerr << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cerr << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
/*
	This is the analogue of the Minimal LBM Source Implementation minsrc.c...
*/
void TEST_MinSrc(const char *transport_name)
{
	std::cerr << MLB::Utility::PadLeft("", 79, '=') << std::endl;

	try {
		LbmAttrContext context_attr;
#ifdef _MSC_VER
		context_attr.SetOpt("fd_management_type", "wincompport");
#else
		context_attr.SetOpt("fd_management_type", "epoll");
#endif // #ifdef _MSC_VER
		LbmContext lbm_context(context_attr);
		LbmAttrTopicSrc topic_src_attr;
		topic_src_attr.SetOpt("transport", transport_name);
		LbmTopicSrc topic_src(lbm_context, "Greeting", topic_src_attr);
		LbmSrc      src(lbm_context, topic_src);
/*
LbmConfigOptionList opt_list(src.Dump());
std::cout << opt_list << std::endl;
*/
		MLB::Utility::SleepSecs(3);
//		src.SendAsciiZ("Hello!", LBM_MSG_FLUSH | LBM_SRC_BLOCK);
		src.SendAsciiZ("Hello!", LBM_SRC_NONBLOCK);
		MLB::Utility::SleepSecs(2);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "MLB analogue of the Minimal LBM Source "
			"Implementation minsrc.c failed: " + std::string(except.what()));
	}

	std::cerr << MLB::Utility::PadLeft("", 79, '=') << std::endl;
	std::cerr << std::endl;
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		const char *transport_name = (argc > 1) ? argv[1] : "tcp";
		const char *config_name    = (argc > 2) ? argv[2] : NULL;
		MLB::Utility::OSSocketLibraryContext socket_library_context;
		std::cout << "LBM Version: " << ::lbm_version() << std::endl;
/*
		{
			LbmContext lbm_context;
			TEST_GetContextOptionsDump_1(lbm_context);
		}
		TEST_GetContextOptionsDump_2();
*/
		LbmLoggerCout my_logger;
		LbmLog(my_logger);
		if (config_name != NULL)
			LbmConfig(config_name);
		TEST_TransportMcAddressMulti();
		TEST_MinSrc(transport_name);
		TEST_STRESS_MinSrc(transport_name);
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << std::endl << "ERROR: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
//	////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

