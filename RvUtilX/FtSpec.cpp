// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	TibCo/Rendezvous Support Library Module
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the FtSpec class.

	Revision History	:	2001-10-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 2001 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <RvUtilX/RvParseCmdLineArg.hpp>

#include <Utility/StringSupport.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace RvUtilX {

// ////////////////////////////////////////////////////////////////////////////
FtSpec::FtSpec() :
	 group_name_()
	,weight_(0)
	,active_goal_(0)
	,heartbeat_interval_(0.0)
	,preparation_interval_(0.0)
	,activation_interval_(0.0)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
FtSpec::FtSpec(const std::string &group_name, unsigned int weight,
	unsigned int active_goal, double heartbeat_interval,
	double preparation_interval, double activation_interval) :
	 group_name_(group_name)
	,weight_(weight)
	,active_goal_(active_goal)
	,heartbeat_interval_(heartbeat_interval)
	,preparation_interval_(preparation_interval)
	,activation_interval_(activation_interval)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
FtSpec::FtSpec(const MLB::Utility::StringVector &string_list) :
	 group_name_()
	,weight_(0)
	,active_goal_(0)
	,heartbeat_interval_(0.0)
	,preparation_interval_(0.0)
	,activation_interval_(0.0)
{
	if (string_list.size() > 6)
		MLB::Utility::ThrowInvalidArgument("The vector of strings passed to the "
			"'FtSpec' constructor exceeds the maximum length (6).");

	if (!string_list.empty()) {
		SetGroupName(string_list[0]);
		if (string_list.size() > 1) {
			SetWeight(string_list[1]);
			if (string_list.size() > 2) {
				SetActiveGoal(string_list[2]);
				if (string_list.size() > 3) {
					SetHeartbeatInterval(string_list[3]);
					if (string_list.size() > 4) {
						SetPreparationInterval(string_list[4]);
						if (string_list.size() > 5)
							SetActivationInterval(string_list[5]);
					}
				}
			}
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void FtSpec::swap(FtSpec &other)
{
	group_name_.swap(other.group_name_);
	std::swap(weight_, other.weight_);
	std::swap(active_goal_, other.active_goal_);
	std::swap(heartbeat_interval_, other.heartbeat_interval_);
	std::swap(preparation_interval_, other.preparation_interval_);
	std::swap(activation_interval_, other.activation_interval_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string FtSpec::GetGroupName() const
{
	return(group_name_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
tibrv_u16 FtSpec::GetWeight() const
{
	return(static_cast<tibrv_u16>(weight_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
tibrv_u16 FtSpec::GetActiveGoal() const
{
	return(static_cast<tibrv_u16>(active_goal_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
double FtSpec::GetHeartbeatInterval() const
{
	return(heartbeat_interval_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
double FtSpec::GetPreparationInterval() const
{
	return(preparation_interval_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
double FtSpec::GetActivationInterval() const
{
	return(activation_interval_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void FtSpec::SetGroupName(const std::string &group_name)
{
	group_name_ = group_name;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void FtSpec::SetWeight(unsigned int weight)
{
	weight_ = weight;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void FtSpec::SetActiveGoal(unsigned int active_goal)
{
	active_goal_ = active_goal;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void FtSpec::SetHeartbeatInterval(double heartbeat_interval)
{
	heartbeat_interval_ = heartbeat_interval;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void FtSpec::SetPreparationInterval(double preparation_interval)
{
	preparation_interval_ = preparation_interval;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void FtSpec::SetActivationInterval(double activation_interval)
{
	activation_interval_ = activation_interval;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void FtSpec::SetWeight(const std::string &weight)
{
	try {
		unsigned int value;
		RvParseCmdLineArg::ParseCmdLineNumeric<unsigned int>(weight, value, 1,
			65535);
		SetWeight(value);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Invalid Tib/Rendezvous fault "
			"tolerance weight (" + weight + ").");
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void FtSpec::SetActiveGoal(const std::string &active_goal)
{
	try {
		unsigned int value;
		RvParseCmdLineArg::ParseCmdLineNumeric<unsigned int>(active_goal,
			value, 1, 65535);
		SetActiveGoal(value);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Invalid Tib/Rendezvous fault "
			"tolerance active goal (" + active_goal + ").");
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void FtSpec::SetHeartbeatInterval(const std::string &heartbeat_interval)
{
	try {
		double value;
		RvParseCmdLineArg::ParseCmdLineNumeric(heartbeat_interval, value,
			0.001);
		SetHeartbeatInterval(value);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Invalid Tib/Rendezvous fault "
			"tolerance heartbeat interval (" + heartbeat_interval + ").");
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void FtSpec::SetPreparationInterval(const std::string &preparation_interval)
{
	try {
		double value;
		RvParseCmdLineArg::ParseCmdLineNumeric(preparation_interval, value,
			0.0);
		SetPreparationInterval(value);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Invalid Tib/Rendezvous fault "
			"tolerance preparation interval (" + preparation_interval + ").");
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void FtSpec::SetActivationInterval(const std::string &activation_interval)
{
	try {
		double value;
		RvParseCmdLineArg::ParseCmdLineNumeric(activation_interval, value,
			0.001);
		SetActivationInterval(value);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Invalid Tib/Rendezvous fault "
			"tolerance activation interval (" + activation_interval + ").");
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool FtSpec::operator < (const FtSpec &other) const
{
	return(Compare(other) < 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool FtSpec::operator == (const FtSpec &other) const
{
	return(Compare(other) == 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool FtSpec::operator != (const FtSpec &other) const
{
	return(!(*this == other));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
int FtSpec::Compare(const FtSpec &other) const
{
	int cmp;

	if ((cmp = MLB::Utility::BasicCompare(group_name_,
		other.group_name_)) == 0) {
		if ((cmp = MLB::Utility::BasicCompare(weight_,
			other.weight_)) == 0) {
			if ((cmp = MLB::Utility::BasicCompare(active_goal_,
				other.active_goal_)) == 0) {
				if ((cmp = MLB::Utility::BasicCompare(heartbeat_interval_,
					other.heartbeat_interval_)) == 0) {
					if ((cmp = MLB::Utility::BasicCompare(preparation_interval_,
						other.preparation_interval_)) == 0) {
						cmp = MLB::Utility::BasicCompare(activation_interval_,
							other.activation_interval_);
					}
				}
			}
		}
	}

	return(cmp);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void FtSpec::CreateFtMember(TibrvFtMember &ft_member, TibrvQueue &queue,
	TibrvFtMemberCallback &callback, TibrvTransport &transport) const
{
	TibrvStatus rv_status = ft_member.create(&queue, &callback, &transport,
		GetGroupName().c_str(), GetWeight(), GetActiveGoal(),
		GetHeartbeatInterval(), GetPreparationInterval(),
		GetActivationInterval());

	if (rv_status != TIBRV_OK) {
		std::ostringstream error_text;
		error_text << "Unable to create a fault tolerance member using " <<
			ToString() << ".";
		throw RvExceptionStatus(rv_status, error_text);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool FtSpec::ParseCmdLine(unsigned int &current_index, int argc,
	char **argv)
{
	return(RvParseCmdLineArg::ParseCmdLineFTParams(current_index,
		argc, argv, group_name_, weight_, active_goal_, heartbeat_interval_,
		preparation_interval_, activation_interval_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string FtSpec::ToString() const
{
	std::string out_string;

	return(ToString(out_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &FtSpec::ToString(std::string &out_string) const
{
	std::ostringstream out_text;

	out_text << "Group Name=" << group_name_ << ", Weight=" << weight_ <<
		", Active Goal=" << active_goal_ << ", Heartbeat Interval=" <<
		heartbeat_interval_ << ", Preparation Interval=" <<
		preparation_interval_ << ", Activation Interval=" <<
		activation_interval_;

	return(out_string.assign(out_text.str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string FtSpec::ToStringLines(unsigned int padding,
	const std::string &separator, const std::string &preface) const
{
	std::string out_string;

	return(ToStringLines(out_string, padding, separator, preface));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &FtSpec::ToStringLines(std::string &out_string,
  	unsigned int padding, const std::string &separator,
	const std::string &preface) const
{
	std::ostringstream o_str;

	o_str <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			(preface + "groupName")           <<
			std::right << separator << group_name_           << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			(preface + "weight")              <<
			std::right << separator << weight_               << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			(preface + "activeGoal")          <<
			std::right << separator << active_goal_          << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			(preface + "heartbeatInterval")   <<
			std::right << separator << heartbeat_interval_   << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			(preface + "preparationInterval") <<
			std::right << separator << preparation_interval_ << std::endl <<
		std::left << std::setw(static_cast<std::streamsize>(padding)) <<
			(preface + "activationInterval")  <<
			std::right << separator << activation_interval_;

	return(out_string.assign(o_str.str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MLB::Utility::StringVector FtSpec::ToArgvList(const std::string &quote_char,
	const std::string &prefix) const
{
	return(ToArgvList(*this, quote_char, prefix));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MLB::Utility::StringVector FtSpec::ToArgvList(const FtSpec &ft_spec,
	const std::string &quote_char, const std::string &prefix)
{
	MLB::Utility::StringVector argv_list;

	argv_list.push_back(prefix + "ft_group_name");
	argv_list.push_back((ft_spec.group_name_.empty()) ?
		(quote_char + quote_char)  :
		(quote_char + ft_spec.group_name_) + quote_char);
	argv_list.push_back(prefix + "ft_weight");
	argv_list.push_back(quote_char +
		MLB::Utility::AnyToString(ft_spec.weight_) + quote_char);
	argv_list.push_back(prefix + "ft_active_goal");
	argv_list.push_back(quote_char +
		MLB::Utility::AnyToString(ft_spec.active_goal_) + quote_char);
	argv_list.push_back(prefix + "ft_heartbeat_interval");
	argv_list.push_back(quote_char +
		MLB::Utility::AnyToString(ft_spec.heartbeat_interval_) + quote_char);
	argv_list.push_back(prefix + "ft_preparation_interval");
	argv_list.push_back(quote_char +
		MLB::Utility::AnyToString(ft_spec.preparation_interval_) + quote_char);
	argv_list.push_back(prefix + "ft_activation_interval");
	argv_list.push_back(quote_char +
		MLB::Utility::AnyToString(ft_spec.activation_interval_) + quote_char);

	return(argv_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
FtSpec FtSpec::FromDelimitedString(const std::string &in_string,
	const std::string &delimiter)
{
	MLB::Utility::StringVector element_list;

	MLB::Utility::SplitString(in_string, delimiter, element_list, 0, false);

	return(FtSpec(element_list));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const FtSpec &datum)
{
	o_str << datum.ToString();

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace RvUtilX

} // namespace MLB

