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

#if defined(_Windows) && !defined(__MINGW32__)
# pragma warning(pop)
#endif // #if defined(_Windows) && !defined(__MINGW32__)

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
std::string GetFilesystemStem(const boost::filesystem::path &src_path)
{
	/*
		MLB 2009-01-28:
		The leaf() method was inadvertantly removed in the Boost 1.36 to bring
		about compliance with changes resulting from the inclusion of
		boost::filesystem in C++0X TR2. They were restored (though deprecated)
		in Boost 1.37.
	*/

#if (BOOST_VERSION < 103600)
	return(src_path.leaf());
#else
	return(src_path.stem());
#endif // #if (BOOST_VERSION < 103600)
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
		MLB::Utility::Rethrow(except, "Error processing directory entry for '" +
			system_complete(dir_path).native_file_string() + "': " +
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
		path dir_path(work_dir_name, native);
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

} // namespace Utility

} // namespace MLB

