// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the portable time classes.

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

#include <Utility/TimeSupport.hpp>
#include <Utility/Utility_Exception.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# if _MSC_VER < 1400
#  pragma warning(disable:4625 4626 4640)
# endif // # if _MSC_VER < 1400
# pragma warning(disable:4061 4127 4217 4242 4244 4267 4511 4512 4701)
# if defined(_MSC_VER) && (_MSC_VER >= 1500)
#  pragma warning(disable:4365 4625 4626 4635 4640)
# endif // # if defined(_MSC_VER) && (_MSC_VER >= 1500)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

#include <boost/date_time/posix_time/posix_time.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

#include <iostream>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {
//	////////////////////////////////////////////////////////////////////////////
template <typename TimeType, typename SecondsType>
	TimeType &AddSecondsInternal(TimeType &time_datum, SecondsType &seconds_ref,
	int secs_to_add)
{
	if (secs_to_add) {
		double tmp_double = static_cast<double>(seconds_ref) +
			static_cast<double>(secs_to_add);
		if ((tmp_double <
				static_cast<double>(std::numeric_limits<SecondsType>::min())) ||
			(tmp_double >
				static_cast<double>(std::numeric_limits<SecondsType>::max())))
			ThrowInvalidArgument("The requested number of seconds to add (" +
				AnyToString(secs_to_add) + ") plus the current time (" +
				time_datum.ToString() + ") results in a total number of seconds ("+
				AnyToString(tmp_double) + ") which is outside of the permissible "
				"boundaries (" +
				AnyToString(std::numeric_limits<SecondsType>::min()) + " to " +
				AnyToString(std::numeric_limits<SecondsType>::max()) +
				", inclusive.");
		seconds_ref += secs_to_add;
	}

	return(time_datum);
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
void ParseFromString_Basic(char *in_date, time_t &out_secs,
	long &out_fractional, long fractional_places)
{
	in_date[19] = '\0';

	if (fractional_places < 9)
		in_date[20 + fractional_places] = '\0';

	in_date[ 4] = '-';
	in_date[ 7] = '-';
	in_date[10] = ' ';
	in_date[13] = ':';
	in_date[16] = ':';

	boost::posix_time::ptime tmp_ptime_3(
		boost::posix_time::time_from_string(in_date));
	boost::posix_time::ptime tmp_ptime_4(
		boost::posix_time::from_time_t(static_cast<time_t>(0)));
	boost::posix_time::time_duration this_duration = tmp_ptime_3 - tmp_ptime_4;

	out_secs       = this_duration.total_seconds();
	out_fractional = strtol(in_date + 20, NULL, 10);
}
//	////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

//	////////////////////////////////////////////////////////////////////////////
void ParseFromString(const char *in_date, time_t &out_secs,
	long &out_fractional, long fractional_places)
{
	if (in_date == NULL)
		ThrowInvalidArgument("Specified date string pointer is 'NULL'.");

	try {
		size_t date_length = strlen(in_date);
		char   date_buffer[Length_TimeSpec + 9 + 1];
		if (date_length < 8)
			ThrowInvalidArgument("Unknown date format.");
		if (date_length == 8) {
			char *end_ptr;
			strtoul(in_date, &end_ptr, 10);
			if (end_ptr != (in_date + 8))
				ThrowInvalidArgument("Invalid undelimited date string --- "
					"only numeric characters are valid.");
			strcat(nstrcat(strcat(nstrcat(strcat(nstrcpy(date_buffer, in_date, 4),
				"-"), in_date + 4, 2), "-"), in_date + 6, 2),
				" 00:00:00.000000000");
			ParseFromString_Basic(date_buffer, out_secs, out_fractional,
				fractional_places);
		}
		else {
			if ((!isdigit(in_date[0])) || (!isdigit(in_date[1])) ||
				(!isdigit(in_date[2])) || (!isdigit(in_date[3])) ||
				(!isdigit(in_date[5])) || (!isdigit(in_date[6])) ||
				(!isdigit(in_date[8])) || (!isdigit(in_date[9])))
				ThrowInvalidArgument("Invalid delimited date string --- "
					"expected format for date portion is 'yyyy-mm-dd'.");
			if (((in_date[4] != '-') && (in_date[4] != '/')) ||
				 ((in_date[7] != '-') && (in_date[7] != '/')))
				ThrowInvalidArgument("Invalid delimited date string --- "
					"expected format for date portion is 'yyyy-mm-dd'.");
			if (date_length == 10) {
				strcat(strcpy(date_buffer, in_date), " 00:00:00.000000000");
				ParseFromString_Basic(date_buffer, out_secs, out_fractional,
					fractional_places);
			}
			else if (date_length <= Length_TimeSpec) {
				if (((date_length < 20) && (date_length != 13) &&
					(date_length != 16) && (date_length != 19)))
					ThrowInvalidArgument("Unknown date format.");
				if (((in_date[10] != ' ') && (in_date[10] != '.')) ||
					(!isdigit(in_date[11])) || (!isdigit(in_date[12])))
					ThrowInvalidArgument("Invalid delimited date/time string.");
				if (date_length == 13)
					ParseFromString_Basic(strcat(strcpy(date_buffer, in_date),
						":00:00.000000000"), out_secs, out_fractional,
						fractional_places);
				else {
					if (((in_date[13] != ':') && (in_date[13] != '.')) ||
						(!isdigit(in_date[14])) || (!isdigit(in_date[15])))
						ThrowInvalidArgument("Invalid delimited date/time string.");
					if (date_length == 16)
						ParseFromString_Basic(strcat(strcpy(date_buffer, in_date),
							":00.000000000"), out_secs, out_fractional,
							fractional_places);
					else {
						if (((in_date[16] != ':') && (in_date[16] != '.')) ||
							(!isdigit(in_date[17])) || (!isdigit(in_date[18])))
							ThrowInvalidArgument("Invalid delimited date/time string.");
						else if (date_length == 19)
							ParseFromString_Basic(strcat(strcpy(date_buffer, in_date),
								".000000000"), out_secs, out_fractional,
								fractional_places);
						else if (in_date[19] != '.')
							ThrowInvalidArgument("Invalid delimited date/time string.");
						else
							ParseFromString_Basic(nstrcat(strcpy(date_buffer, in_date),
								"000000000", Length_TimeSpec - date_length), out_secs,
								out_fractional, fractional_places);
					}
				}
			}
			else
				ThrowInvalidArgument("Date/time string length exceeds maximum "
					"permissible (" + AnyToString(Length_TimeSpec) +
					" characters).");
		}
	}
	catch (const std::exception &except) {
		Rethrow(except, "Unable to parse date/time string '" +
			std::string(in_date) + "': " + std::string(except.what()));
	}
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
TimeT &TimeT::AddSeconds(int secs_to_add)
{
	return(AddSecondsInternal(*this, datum_, secs_to_add));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
TimeVal &TimeVal::AddSeconds(int secs_to_add)
{
	return(AddSecondsInternal(*this, tv_sec, secs_to_add));
}
//	////////////////////////////////////////////////////////////////////////////

//	////////////////////////////////////////////////////////////////////////////
TimeSpec &TimeSpec::AddSeconds(int secs_to_add)
{
	return(AddSecondsInternal(*this, tv_sec, secs_to_add));
}
//	////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <iostream>

#include <boost/shared_ptr.hpp>

/*
2005-06-23 12:34:56.123456
01234567890123456789012345
*/

// ////////////////////////////////////////////////////////////////////////////
const char         *TEST_FromStringList[] = {
	"19990101",
	"1999-01-01",
	"1999/01/01",
	"1999-01-01 12",
	"1999-01-01 12:34",
	"1999-01-01 12:34:56",
	"1999-01-01 12:34:56",
	"1999-01-01 12:34:56.1",
	"1999-01-01 12:34:56.12",
	"1999-01-01 12:34:56.123",
	"1999-01-01 12:34:56.1234",
	"1999-01-01 12:34:56.12345",
	"1999-01-01 12:34:56.123456",
	"1999-01-01 12:34:56.1234567",
	"1999-01-01 12:34:56.12345678",
	"1999-01-01 12:34:56.123456789",
	"1999-01-01 12.34.56.123456789"
};
const unsigned int  TEST_FromStringCount  =
	sizeof(TEST_FromStringList) / sizeof(TEST_FromStringList[0]);
// ////////////////////////////////////////////////////////////////////////////

using namespace MLB::Utility;

// ////////////////////////////////////////////////////////////////////////////
template <typename TimeType>
	void TEST_DoFromStringForType(const char *type_name)
{
	std::cout << std::setfill('=') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;
	std::cout << "FromString() Test for Type '" << type_name << ":'" <<
		std::endl;
	std::cout << std::setfill('-') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;

	unsigned int count_1;

	for (count_1 = 0; count_1 < TEST_FromStringCount; ++count_1) {
		std::cout << "FROM: " << std::left << std::setw(30) <<
			TEST_FromStringList[count_1] << std::right << " ---> TO: " <<
			TimeType::FromString(TEST_FromStringList[count_1]).ToString() <<
			std::endl;
	}

	std::cout << std::setfill('=') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_DoFromStringTests()
{
	TEST_DoFromStringForType<TimeT>("TimeT");
	TEST_DoFromStringForType<TimeVal>("TimeVal");
	TEST_DoFromStringForType<TimeSpec>("TimeSpec");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_DoFromStringTests();

{
	int      add_diff = -2;
	TimeSpec this_date(::time(NULL), 1);
	TimeSpec work_date;
	work_date = this_date;
	std::cout << std::setw(24) << this_date.ToString() <<
		" --- Add Secs : " << std::setw(11) << add_diff << std::endl;
	work_date.AddSeconds(add_diff);
	std::cout << std::setw(24) << work_date.ToString() << std::endl << std::endl;
	//	-------------------------------------------------------------------------
	work_date = this_date;
	std::cout << std::setw(24) << this_date.ToString() <<
		" --- Add MSecs: " << std::setw(11) << add_diff << std::endl;
	work_date.AddMilliseconds(add_diff);
	std::cout << std::setw(24) << work_date.ToString() << std::endl << std::endl;
	//	-------------------------------------------------------------------------
	work_date = this_date;
	std::cout << std::setw(24) << this_date.ToString() <<
		" --- Add USecs: " << std::setw(11) << add_diff << std::endl;
	work_date.AddMicroseconds(add_diff);
	std::cout << std::setw(24) << work_date.ToString() << std::endl << std::endl;
	//	-------------------------------------------------------------------------
	work_date = this_date;
	std::cout << std::setw(24) << this_date.ToString() <<
		" --- Add NSecs: " << std::setw(11) << add_diff << std::endl;
	work_date.AddNanoseconds(add_diff);
	std::cout << std::setw(24) << work_date.ToString() << std::endl << std::endl;
	//	-------------------------------------------------------------------------
	add_diff  =  1000000000;
	work_date = this_date;
	std::cout << std::setw(24) << this_date.ToString() <<
		" --- Add NSecs: " << std::setw(11) << add_diff << std::endl;
	work_date.AddNanoseconds(add_diff);
	std::cout << std::setw(24) << work_date.ToString() << std::endl << std::endl;
	//	-------------------------------------------------------------------------
	add_diff  = -1000000000;
	work_date = this_date;
	std::cout << std::setw(24) << this_date.ToString() <<
		" --- Add NSecs: " << std::setw(11) << add_diff << std::endl;
	work_date.AddNanoseconds(add_diff);
	std::cout << std::setw(24) << work_date.ToString() << std::endl << std::endl;
	//	-------------------------------------------------------------------------
	add_diff  =  1000000002;
	work_date = this_date;
	std::cout << std::setw(24) << this_date.ToString() <<
		" --- Add NSecs: " << std::setw(11) << add_diff << std::endl;
	work_date.AddNanoseconds(add_diff);
	std::cout << std::setw(24) << work_date.ToString() << std::endl << std::endl;
	//	-------------------------------------------------------------------------
	add_diff  = -1000000002;
	work_date = this_date;
	std::cout << std::setw(24) << this_date.ToString() <<
		" --- Add NSecs: " << std::setw(11) << add_diff << std::endl;
	work_date.AddNanoseconds(add_diff);
	std::cout << std::setw(24) << work_date.ToString() << std::endl << std::endl;
}
		std::cout << std::setfill('=') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
		std::cout << "String conversion tests:" << std::endl;
		std::cout << std::setfill('-') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
		TimeSpec    current_time_spec;
		TimeVal     current_time_val(current_time_spec.ToTimeVal());
		std::string string_time_spec(current_time_spec.ToString());
		std::string string_time_val(current_time_val.ToString());
		std::cout << "TimeSpec: [" << string_time_spec << "] ---> [\"" <<
			string_time_spec << "\"] ---> [" <<
			TimeSpec::FromString(string_time_spec) << "]" << std::endl;
		std::cout << "TimeVal : [" << string_time_val << "] ---> [\"" <<
			string_time_val << "\"] ---> [" <<
			TimeVal::FromString(string_time_val) << "]" << std::endl;
		std::cout << std::setfill('=') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
std::cout << "FROM STRING: " <<
	TimeVal::FromString("2004-11-11 12:34:56.123456789").ToString() <<
	std::endl;
		std::cout << "TimeTM Test:" << std::endl;
		std::cout << "------ -----" << std::endl;
		std::cout << "TimeTM::TimeUTC(0)         : " <<
			TimeTM::TimeUTC(0)           << std::endl;
		std::cout << "TimeTM::TimeLocal(0)       : " <<
			TimeTM::TimeLocal(0)         << std::endl;
		std::cout << "TimeTM::Now()              : " <<
			TimeTM::Now()                << std::endl;
		std::cout << "TimeTM::Now().ToString()   : " <<
			TimeTM::Now().ToString() << std::endl;
		std::cout << "TimeTM::Now().AscTimeSane(): " <<
			TimeTM::Now().AscTimeSane() << std::endl;
		std::cout << std::endl;

		std::cout << "TimeT Test:" << std::endl;
		std::cout << "----- -----" << std::endl;
		std::cout << "TimeT::Now()                   : " <<
			TimeT::Now()                 << std::endl;
		std::cout << "TimeT::Now().ToString()        : " <<
			TimeT::Now().ToString()      << std::endl;
		std::cout << "TimeT::Now().ToStringLocal()   : " <<
			TimeT::Now().ToStringLocal() << std::endl;
		std::cout << "TimeT::Now().ToStringInterval(): " <<
			TimeT::Now().ToStringInterval() << std::endl;
		std::cout << "TimeT::FromString()            : " <<
			TimeT::FromString(TimeT::Now().ToString()) << std::endl;
		std::cout << "TimeT::FromString(\"1999-11-11\"): " <<
			TimeT::FromString("1999-11-11") << std::endl;
		std::cout << std::endl;

		std::cout << "TimeVal Test:" << std::endl;
		std::cout << "------- -----" << std::endl;
		std::cout << "TimeVal::Now()                   : " <<
			TimeVal::Now()                 << std::endl;
		std::cout << "TimeVal::Now().ToString()        : " <<
			TimeVal::Now().ToString()      << std::endl;
		std::cout << "TimeVal::Now().ToStringLocal()   : " <<
			TimeVal::Now().ToStringLocal() << std::endl;
		std::cout << "TimeVal::Now().ToStringInterval(): " <<
			TimeVal::Now().ToStringInterval() << std::endl;
		std::cout << "TimeVal::FromString()            : " <<
			TimeVal::FromString(TimeVal::Now().ToString()) << std::endl;
		std::cout << "TimeVal::FromString(\"1999-11-11\"): " <<
			TimeVal::FromString("1999-11-11") << std::endl;
		std::cout << std::endl;

		std::cout << "TimeSpec Test:" << std::endl;
		std::cout << "-------- -----" << std::endl;
		std::cout << "TimeSpec::Now()                   : " <<
			TimeSpec::Now()                 << std::endl;
		std::cout << "TimeSpec::Now().ToString()        : " <<
			TimeSpec::Now().ToString()      << std::endl;
		std::cout << "TimeSpec::Now().ToStringLocal()   : " <<
			TimeSpec::Now().ToStringLocal() << std::endl;
		std::cout << "TimeSpec::Now().ToStringInterval(): " <<
			TimeSpec::Now().ToStringInterval() << std::endl;
		std::cout << "TimeSpec::FromString()            : " <<
			TimeSpec::FromString(TimeSpec::Now().ToString()) << std::endl;
		std::cout << "TimeSpec::FromString(\"1999-11-11\"): " <<
			TimeSpec::FromString("1999-11-11") << std::endl;
		std::cout << std::endl;
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

