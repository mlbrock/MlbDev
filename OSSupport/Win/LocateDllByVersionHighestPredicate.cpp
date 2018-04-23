// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Operating System Support (OSSupport) Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the LocateDllByVersionHighestPredicate
								class.

	Revision History	:	1998-04-08 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1998 - 2018.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <OSSupport/Win/OSSupport_Win.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace OSSupport {

// ////////////////////////////////////////////////////////////////////////////
LocateDllByVersionHighestPredicate::LocateDllByVersionHighestPredicate(
	const MLB::Utility::VersionNumber &min_version,
	const MLB::Utility::VersionNumber &max_version)
	:LocateDllByVersionPredicate(min_version, max_version)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string LocateDllByVersionHighestPredicate::DescribePredicate() const
{
	std::string out_string("matching DLL with the highest version");

	if ((min_version_ == MLB::Utility::VersionNumber::GetMinimumValue()) &&
		 (max_version_ == MLB::Utility::VersionNumber::GetMaximumValue()))
		;
	else if ((min_version_ > MLB::Utility::VersionNumber::GetMinimumValue()) &&
				(max_version_ < MLB::Utility::VersionNumber::GetMaximumValue()))
		out_string += " between " + min_version_.ToString() + " and " +
			max_version_.ToString() + " inclusive";
	else if (min_version_ > MLB::Utility::VersionNumber::GetMinimumValue())
		out_string += " greater than or equal to " + min_version_.ToString();
	else if (max_version_ < MLB::Utility::VersionNumber::GetMaximumValue())
		out_string += " less than or equal to " + max_version_.ToString();

	return(out_string);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool LocateDllByVersionHighestPredicate::IsGlobalPredicateInternal() const
{
	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool LocateDllByVersionHighestPredicate::IsMatchingDllInternal(
	const std::string &full_name)
{
	MLB::Utility::VersionNumber this_version;

	if (!IsVersionMatch(full_name, this_version)) {
		candidate_list_.push_back(DllMatchItem(full_name, this_version));
		return(false);
	}

	candidate_list_.push_back(DllMatchItem(full_name, this_version));
	matched_list_.push_back(DllMatchItem(full_name, this_version));

	if (best_match_.version_number_ < this_version)
		best_match_ = matched_list_.back();

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace OSSupport

} // namespace MLB

#ifdef TEST_MAIN

using namespace MLB::OSSupport;

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif /* #ifdef TEST_MAIN */

