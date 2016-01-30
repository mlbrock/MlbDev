// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for the operating system environment
								support.

	Revision History	:	1993-10-02 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 1993 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__GetEnvironment_hpp__HH

#define HH__MLB__Utility__GetEnvironment_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file GetEnvironment.hpp

	\brief	The MLB environment variable support header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

#include <set>
#include <vector>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
struct API_UTILITY EnvElement {
	         EnvElement();
	explicit EnvElement(const std::string &env_string);
	         EnvElement(const std::string &env_name,
		const std::string &env_value);

	bool operator < (const EnvElement &other) const;

	std::string &ToString(std::string &out_string) const;
	std::string  ToString() const;

	std::string env_name_;
	std::string env_value_;
};
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const EnvElement &datum);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef std::vector<EnvElement>        EnvElementList;
typedef EnvElementList::iterator       EnvElementListIter;
typedef EnvElementList::const_iterator EnvElementListIterC;

typedef std::set<EnvElement>           EnvElementSet;
typedef EnvElementSet::iterator        EnvElementSetIter;
typedef EnvElementSet::const_iterator  EnvElementSetIterC;

typedef std::vector<std::string>       EnvpList;
typedef EnvpList::iterator             EnvpListIter;
typedef EnvpList::const_iterator       EnvpListIterC;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY std::string::size_type
	GetEnvStringSeparatorIndex(const std::string &env_string);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY void           GetEnvironment(EnvpList &env_list);
API_UTILITY EnvpList       GetEnvironment();

API_UTILITY void           EnvpListToEnvElementList(const EnvpList &envp_list,
	EnvElementList &env_element_list);
API_UTILITY EnvElementList EnvpListToEnvElementList(
	const EnvpList &envp_list = GetEnvironment());
API_UTILITY void           EnvElementListToEnvpList(
	const EnvElementList &env_element_list, EnvpList &envp_list);
API_UTILITY EnvpList       EnvElementListToEnvpList(
	const EnvElementList &env_element_list);

API_UTILITY void           EnvpListToEnvElementSet(const EnvpList &envp_list,
	EnvElementSet &env_element_list);
API_UTILITY EnvElementSet  EnvpListToEnvElementSet(const EnvpList &envp_list);
API_UTILITY void           EnvElementSetToEnvpList(
	const EnvElementSet &env_element_list, EnvpList &envp_list);
API_UTILITY EnvpList       EnvElementSetToEnvpList(
	const EnvElementSet &env_element_list);

API_UTILITY void           MergeEnvironment(const EnvpList &base_env,
	const EnvpList &added_env, EnvpList &result_env,
	bool merge_smart_flag = false, bool merge_path_flag = false);
API_UTILITY EnvpList       MergeEnvironment(const EnvpList &override_env,
	const EnvpList &added_env = GetEnvironment(), bool merge_smart_flag = false,
	bool merge_path_flag = false);

API_UTILITY void           CheckEnvironment(
	const EnvpList &envp_list = GetEnvironment());
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__GetEnvironment_hpp__HH

