// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	29 West LBM API Wrapper Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Miscellaneous LBM wrapper logic.

	Revision History	:	2008-08-16 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2008 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <LbmWrapper/LbmMiscSupport.hpp>

#include <Utility/ParseCmdLineArg.hpp>
#include <Utility/InlineContainer.hpp>

//	////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace LbmWrapper {

// ////////////////////////////////////////////////////////////////////////////
void LbmLicenseFile(const char *licfile)
{
	WRAP29_LBM_THROW_IF_NE_ZERO(
		::lbm_license_file,
		(licfile));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LbmLicenseFile(const std::string &licfile)
{

	LbmLicenseFile(licfile.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LbmLicenseStr(const char *licstr)
{
	WRAP29_LBM_THROW_IF_NE_ZERO(
		::lbm_license_str,
		(licstr));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LbmLicenseStr(const std::string &licstr)
{
	LbmLicenseStr(licstr.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LbmConfig(const char *fname)
{
	WRAP29_LBM_THROW_IF_NE_ZERO(
		::lbm_config,
		(fname));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LbmConfig(const std::string &fname)
{
	LbmConfig(fname.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int LbmScheduleTimer(lbm_context_t *ctx, lbm_timer_cb_proc proc, void *clientd,
	lbm_event_queue_t *evq, lbm_ulong_t delay)
{
	int lbm_code = ::lbm_schedule_timer(ctx, proc, clientd, evq, delay);

	if (lbm_code == -1)
		WRAP29_LBM_THROW(::lbm_schedule_timer);

	return(lbm_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int LbmScheduleTimerRecurring(lbm_context_t *ctx, lbm_timer_cb_proc proc,
	void *clientd, lbm_event_queue_t *evq, lbm_ulong_t delay)
{
	int lbm_code =
		::lbm_schedule_timer_recurring(ctx, proc, clientd, evq, delay);

	if (lbm_code == -1)
		WRAP29_LBM_THROW(::lbm_schedule_timer);

	return(lbm_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int LbmEventDispatch(lbm_event_queue_t *evq, lbm_ulong_t tmo)
{
	int lbm_code = ::lbm_event_dispatch(evq, tmo);

	if (lbm_code < 0)
		WRAP29_LBM_THROW(::lbm_event_dispatch);

	if (!lbm_code)
		throw LbmExceptionStatus("Invocation of '::lbm_event_dispatch()' "
			"unexpectedly returned zero");

	return(lbm_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LbmEventDispatchUnblock(lbm_event_queue_t *evq)
{
	WRAP29_LBM_THROW_IF_NE_ZERO(
		::lbm_event_dispatch_unblock,
		(evq));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int LbmEventQueueSize(lbm_event_queue_t *evq)
{
	int lbm_code = ::lbm_event_queue_size(evq);

	if (lbm_code < 0)
		WRAP29_LBM_THROW(::lbm_event_queue_size);

	return(lbm_code);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LbmEventQueueShutdown(lbm_event_queue_t *evq)
{
	WRAP29_LBM_THROW_IF_NE_ZERO(
		::lbm_event_queue_shutdown,
		(evq));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *GetOptimalLbmFdManagementType()
{
#ifdef _MSC_VER

	OSVERSIONINFOEX win_ver;

	::memset(&win_ver, '\0', sizeof(win_ver));
	win_ver.dwOSVersionInfoSize = sizeof(win_ver);

	if (!::GetVersionEx(reinterpret_cast<OSVERSIONINFO *>(&win_ver)))
		MLB::Utility::ThrowSystemError("Call to 'GetVersionEx()' failed");

	//	'wincompport' for Windows XP and later only.
	return(((win_ver.dwMajorVersion > 5) ||
		((win_ver.dwMajorVersion == 5) && (win_ver.dwMinorVersion >= 1))) ?
		"wincompport" : "wsaeventselect");

#elif defined(__linux__)
	return("epoll");			//	Linux 2.6 and later only.
#elif defined (__SVR4) && defined (__sun)
	return("devpoll");		//	/dev/poll driver. Solaris 8 and later only.
#elif defined(MACOSX)
	return("kqueue");
#else
	return("select");
#endif // #ifdef _MSC_VER
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool GetLbmConfigFileFromCmdLine(unsigned int &current_index, int argc,
	char **argv, std::string &out_datum, bool require_existence)
{
	std::string tmp_datum;

	if (!MLB::Utility::ParseCmdLineArg::ParseCmdLineFollowingSpec(
		MLB::Utility::MakeInlineVector<std::string>
			("-LBM_CONFIGURATION_FILE")
			("-LBM_CONFIGURATIONFILE")
			("-LBMCONFIGURATION_FILE")
			("-LBMCONFIGURATIONFILE")
			("-LBM_CONFIG_FILE")
			("-LBM_CONFIGFILE")
			("-LBMCONFIG_FILE")
			("-LBMCONFIGFILE")
			("-LBM_CFG_FILE")
			("-LBM_CFGFILE")
			("-LBMCFG_FILE")
			("-LBMCFGFILE")
			("-LBM_CONFIGURATION")
			("-LBMCONFIGURATION")
			("-LBM_CONFIG")
			("-LBMCONFIG")
			("-LBM_CFG")
			("-LBMCFG"),
		current_index, argc, argv, tmp_datum))
		return(false);

	MLB::Utility::ParseCmdLineArg::CheckFilePath("LBM configuration file name",
		tmp_datum, "", require_existence);

	out_datum.swap(tmp_datum);

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# pragma warning(disable:4127)
#endif // #if defined(_Windows) && !defined(__MINGW32__)
//	////////////////////////////////////////////////////////////////////////////
std::string LbmGetIncludeFileVersion(bool tnw_format, bool use_tag)
{
	std::ostringstream o_str;

	o_str
		<< LBM_VERS_MAJOR << "."
		<< LBM_VERS_MINOR << "."
			;

	if (tnw_format && (!LBM_VERS_MAINT) && use_tag)
		o_str
			<< LBM_VERS_TAG;
	else
		o_str
			<< LBM_VERS_MAINT
			<< ((use_tag) ? LBM_VERS_TAG : "")
				;

	return(o_str.str());
}
//	////////////////////////////////////////////////////////////////////////////
#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

//	////////////////////////////////////////////////////////////////////////////
unsigned int LbmGetIncludeFileVersionCombined()
{
	return(LBM_VERS);
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace LbmWrapper

} // namespace MLB

