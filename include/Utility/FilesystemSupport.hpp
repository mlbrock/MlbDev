// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Include file for Boost filesystem library support.

	Revision History	:	2009-08-31 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2009 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////


#ifndef HH__MLB__Utility__FilesystemSupport_hpp__HH

#define HH__MLB__Utility__FilesystemSupport_hpp__HH	1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
	\file FilesystemSupport.hpp

	\brief	The Boost filesystem library support header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(push)
# if _MSC_VER < 1400
#  pragma warning(disable:4347 4625 4626 4640)
# endif // # if _MSC_VER < 1400
# pragma warning(disable:4217 4668)
# if _MSC_VER >= 1500
#  pragma warning(disable: 4347 4365 4571 4625 4626 4640)
# endif // # if _MSC_VER >= 1500
#endif // #if defined(_Windows) && !defined(__MINGW32__)

#include <boost/filesystem/path.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
struct PathTraversalBase {
	virtual ~PathTraversalBase()
	{
	}

	/**
		This function will be called by \e TraverseDirectory for each constituent
		of a given directory.

		\param orig_dir_name is the original directory name given as an argument
		to \e TraverseDirectory .

		\param work_dir_name is the canonical version of \e orig_dir_name .

		\param this_path is a \c const reference to a \c boost::filesystem::path
		instance which represents the current directory constituent.

		\return Return values from this method have the following meanings:

			-	If \e 0 then \e TraverseDirectory is signalled that it is to
				stop traversing the directory. It will return with \e false .

			-	If greater than \e 0 then \e TraverseDirectory will continue to
				traverse the directory, but will not descend recursively into any
				constituent directories encountered.

			-	If less than \e 0 then \e TraverseDirectory will continue to
				traverse the directory, and will descend recursively into any
				constituent directories encountered.
	*/
	virtual int DoPath(const std::string & /* orig_dir_name */,
		const std::string & /*work_dir_name */,
		const boost::filesystem::path & /* this_path */)
	{
		return(0);
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY std::string GetFilesystemStem(
	const boost::filesystem::path &src_path);
API_UTILITY bool        TraverseDirectory(const std::string &orig_dir_name,
	PathTraversalBase &traversal_control);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY std::string              BoostFs_GetPathLeafString(
	const boost::filesystem::path &src_path);
API_UTILITY std::string              BoostFs_GetPathStemString(
	const boost::filesystem::path &src_path);
API_UTILITY std::string              BoostFs_GetNativeFileString(
	const boost::filesystem::path &src_path);
API_UTILITY boost::filesystem::path &BoostFs_ConstructNativePath(
	const char *src_path, boost::filesystem::path &dst_path);
API_UTILITY boost::filesystem::path &BoostFs_ConstructNativePath(
	const std::string &src_path, boost::filesystem::path &dst_path);
API_UTILITY boost::filesystem::path  BoostFs_ConstructNativePath(
	const char *src_path);
API_UTILITY boost::filesystem::path  BoostFs_ConstructNativePath(
	const std::string &src_path);
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__FilesystemSupport_hpp__HH

