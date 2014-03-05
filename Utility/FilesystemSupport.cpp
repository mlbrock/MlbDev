// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
	File Name			:	%M%

	File Version		:	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Implementation of the Boost filesystem library support.

	Revision History	:	2009-08-31 --- Creation
									Michael L. Brock

		Copyright Michael L. Brock 2009 - 2014.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
//	Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/FilesystemSupport.hpp>
#include <Utility/PathName.hpp>
#include <Utility/Utility_Exception.hpp>

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

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/version.hpp>

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
std::string GetFilesystemStem(const boost::filesystem::path &src_path)
{
	return(BoostFs_GetPathLeafString(src_path));
}
// ////////////////////////////////////////////////////////////////////////////

namespace {
// ////////////////////////////////////////////////////////////////////////////
bool TraverseDirectoryInternal(const std::string &orig_dir_name,
	const std::string &work_dir_name, const boost::filesystem::path &dir_path,
	PathTraversalBase &traversal_control)
{
	using namespace boost::filesystem;

	try {
		directory_iterator iter_b(dir_path);
		directory_iterator iter_e;
		for ( ; iter_b != iter_e; ++iter_b) {
			int next_action = 0;
			try {
				next_action = traversal_control.DoPath(orig_dir_name,
					work_dir_name, *iter_b);
			}
			catch (const std::exception &except) {
				MLB::Utility::Rethrow(except, "The 'PathTraversalAction()' method "
					"of a traversal functor for 'TraverseDirectory()' has thrown "
					"an exception: " + std::string(except.what()));
			}
			if (!next_action)
				return(false);
			else if ((next_action < 0) && is_directory(*iter_b)) {
				if (!TraverseDirectoryInternal(orig_dir_name, work_dir_name,
					iter_b->path(), traversal_control))
					return(false);
			}
		}
	}
	catch (const std::exception &except) {
/*
	CODE NOTE: The method boost::filesystem::path::native_file_string() has
		been obsoleted by Boost Filesystem version 3. This code is therefore
		to be removed.
		MLB::Utility::Rethrow(except, "Error processing directory entry for '" +
			system_complete(dir_path).native_file_string() + "': " +
			std::string(except.what()));
*/
		MLB::Utility::Rethrow(except, "Error processing directory entry for '" +
			BoostFs_GetNativeFileString(system_complete(dir_path)) + "': " +
			std::string(except.what()));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////
} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
bool TraverseDirectory(const std::string &orig_dir_name,
	PathTraversalBase &traversal_control)
{
	using namespace boost::filesystem;

	try {
		std::string work_dir_name(orig_dir_name);
		MLB::Utility::ResolveDirectoryPath(work_dir_name, "", true);
/*
	CODE NOTE: The boost::filesystem::path constructor which accepted a
		second parameter to specify the native format of the path (usually
		boost::filesystem::native()) has been obsoleted by Boost Filesystem
		version 3. This code is therefore to be removed.
		path dir_path(work_dir_name, native);
*/
		path dir_path(BoostFs_ConstructNativePath(work_dir_name));
		if (!TraverseDirectoryInternal(orig_dir_name, work_dir_name,
			dir_path, traversal_control))
			return(false);
	}
	catch (const std::exception &except) {
		MLB::Utility::Rethrow(except, "Attempt to traverse the directory '" +
			orig_dir_name + "' failed: " + std::string(except.what()));
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string BoostFs_GetPathLeafString(const boost::filesystem::path &src_path)
{
#if (BOOST_VERSION < 103600)
#error "BOOST_VERSION="##BOOST_VERSION
	/*
		MLB 2009-01-28:
		The leaf() method was inadvertantly removed in the Boost 1.36 to bring
		about compliance with changes resulting from the inclusion of
		boost::filesystem in C++0X TR2. They were restored (though deprecated)
		in Boost 1.37.
	*/
	return(src_path.leaf());
#elif (BOOST_VERSION < 104400) || (defined(BOOST_FILESYSTEM_VERSION) && (BOOST_FILESYSTEM_VERSION < 3))
	/*
		Boost 1.44 supported both the old filesystem api as version 2 and the
		version 3.

		The old version 2 stem() method returned a string...
	*/
	return(src_path.stem());
#else
	/*
		But the new version 3 stem() method returns a path.
	*/
	return(BoostFs_GetNativeFileString(src_path.stem()));
#endif // #if (BOOST_VERSION < 103600)
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string BoostFs_GetPathStemString(const boost::filesystem::path &src_path)
{
	return(BoostFs_GetPathLeafString(src_path));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	To be moved into FilesystemSupport.cpp.
std::string BoostFs_GetNativeFileString(
	const boost::filesystem::path &src_path)
{
#if (!defined(BOOST_FILESYSTEM_VERSION)) || (BOOST_FILESYSTEM_VERSION < 3)
	return(src_path.native_file_string());
#else
	return(src_path.string());
#endif // #if (!defined(BOOST_FILESYSTEM_VERSION)) || (BOOST_FILESYSTEM_VERSION < 3)
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	To be moved into FilesystemSupport.cpp.
boost::filesystem::path &BoostFs_ConstructNativePath(const char *src_path,
	boost::filesystem::path &dst_path)
{
#if (BOOST_FILESYSTEM_VERSION < 3)
	boost::filesystem::path(src_path, boost::filesystem::native).swap(dst_path);
#else
	boost::filesystem::path(src_path).make_preferred().swap(dst_path);
#endif // #if (BOOST_FILESYSTEM_VERSION < 3)

	return(dst_path);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	To be moved into FilesystemSupport.cpp.
boost::filesystem::path &BoostFs_ConstructNativePath(
	const std::string &src_path, boost::filesystem::path &dst_path)
{
	return(BoostFs_ConstructNativePath(src_path.c_str(), dst_path));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	To be moved into FilesystemSupport.cpp.
boost::filesystem::path BoostFs_ConstructNativePath(const char *src_path)
{
	boost::filesystem::path dst_path;

	return(BoostFs_ConstructNativePath(src_path, dst_path));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
//	To be moved into FilesystemSupport.cpp.
boost::filesystem::path BoostFs_ConstructNativePath(const std::string &src_path)
{
	return(BoostFs_ConstructNativePath(src_path.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

