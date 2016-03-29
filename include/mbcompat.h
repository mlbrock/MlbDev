/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	Compatibity Include File																*/
/*	***********************************************************************	*/
/*
	File Name  			:	%M%

	File Version      :	%I%

	Last Extracted		:	%D%	%T%

	Last Updated		:	%E%	%U%

	File Description	:	Basic compatibility include file.

	Revision History	:	1985-01-01 --- Creation.
									Michael L. Brock

		Copyright Michael L. Brock 1985 - 2016.
		Distributed under the Boost Software License, Version 1.0.
		(See accompanying file LICENSE_1_0.txt or copy at
		http://www.boost.org/LICENSE_1_0.txt)

*/
/*	***********************************************************************	*/

#ifndef HH__mbcompat_h__HH

#define HH__mbcompat_h__HH					1

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	***********************************************************************	*/
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)
# ifndef _Windows
#  define _Windows 1	/* Compatibility with the old Borland C++ compiler. */
# endif /* # ifndef _Windows */
#endif /* #if defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER) */

/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*
	Get rid of spurious warnings from more recent VC++ compilers...
*/
/*	***********************************************************************	*/
#ifdef _Windows
# if !defined(__MINGW32__)
#  if defined(_MSC_VER) && (_MSC_VER >= 1400)
#   pragma warning(disable:4548 4571)
#  endif /* #if defined(_MSC_VER) && (_MSC_VER >= 1400) */
# endif /* #if !defined(__MINGW32__) */
#endif /* #ifdef _Windows */
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/**
	Gets rid of the warnings resulting from Microsoft's non-standard
	"deprecation" of standard libaray functions.
*/
/*	***********************************************************************	*/
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
# ifndef _CRT_SECURE_NO_DEPRECATE
#  define _CRT_SECURE_NO_DEPRECATE	1
# endif /* # ifndef _CRT_SECURE_NO_DEPRECATE */
# ifndef _CRT_NONSTDC_NO_DEPRECATE
#  define _CRT_NONSTDC_NO_DEPRECATE	1
# endif /* # ifndef _CRT_NONSTDC_NO_DEPRECATE */
#endif /* #if defined(_MSC_VER) && (_MSC_VER >= 1400) */
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* Some pragmas to disable the more stupid MSVC warnings...						*/
/*	***********************************************************************	*/
#ifdef _Windows
# ifndef __MINGW32__
#  if _MSC_VER <= 1700
#   if _MSC_VER >= 1310
#    pragma warning(disable:4820)
#   endif /* if _MSC_VER >= 1310 */
#  endif /* if _MSC_VER <= 1700 */
# endif /* # ifndef __MINGW32__ */
#endif /* #ifdef _Windows */
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	Attempt to make pre-ANSI standard compilers compatible . . .				*/
/*	***********************************************************************	*/
#ifdef NO_CONST
# ifndef const
#  define const
# endif /* #ifndef const */
#endif /* #ifdef NO_CONST */

#ifdef NO_VOLATILE
# ifndef volatile
#  define volatile
# endif /* #ifndef volatile */
#endif /* #ifndef NO_VOLATILE */

#ifdef NO_VOID
# ifndef void
#  define void char
# endif /* #ifndef void */
#endif /* #ifdef NO_VOID */

#ifdef NO_SIZE_T
# ifndef size_t
#  define size_t unsigned int
# endif /* #ifndef size_t */
#endif /* #ifdef NO_SIZE_T */

#ifdef __cplusplus
# define MBCOMPAT_INLINE					inline
#else
# ifndef NO_INLINE
#  define NO_INLINE
# endif /* # ifndef NO_INLINE */
# ifndef inline
#  define inline
# endif /* # ifndef NO_INLINE */
#endif /* #ifndef __cplusplus */
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	Macros for function prototype, typedefs, and casting . . .					*/
/*	***********************************************************************	*/
#if defined(__cplusplus)
# define COMPAT_FN_DECL(frtn, fargs)				extern "C" { frtn fargs; }
# define COMPAT_FN_DECL_STATIC(frtn, fargs)		static frtn fargs
# define COMPAT_FN_TYPE(frtn, fargs)				frtn fargs
# define COMPAT_FN_CAST(frtn, fargs, fptr)		((frtn (*) fargs) (fptr))
# define COMPAT_FN_PARA(frtn, fname, fargs)		frtn (fname) fargs
#else
# if defined(__STDC__) || defined(_STDC_PROTO_)
#  define COMPAT_FN_DECL(frtn, fargs)				frtn fargs
#  define COMPAT_FN_DECL_STATIC(frtn, fargs)		static frtn fargs
#  define COMPAT_FN_TYPE(frtn, fargs)				frtn fargs
#  define COMPAT_FN_CAST(frtn, fargs, fptr)		((frtn (*) fargs) (fptr))
#  define COMPAT_FN_PARA(frtn, fname, fargs)		frtn (fname) fargs
# else
#  ifndef NARGS
#   define COMPAT_FN_DECL(frtn, fargs)				frtn fargs
#   define COMPAT_FN_DECL_STATIC(frtn, fargs)	static frtn fargs
#   define COMPAT_FN_TYPE(frtn, fargs)				frtn fargs
#   define COMPAT_FN_CAST(frtn, fargs, fptr)		((frtn (*) fargs) (fptr))
#   define COMPAT_FN_PARA(frtn, fname, fargs)	frtn (fname) fargs
#  else
#   define COMPAT_FN_DECL(frtn, fargs)				frtn()
#   define COMPAT_FN_DECL_STATIC(frtn, fargs)	static frtn()
#   define COMPAT_FN_TYPE(frtn, fargs)				frtn()
#   define COMPAT_FN_CAST(frtn, fargs, fptr)		((frtn (*) ()) (fptr))
#   define COMPAT_FN_PARA(frtn, fname, fargs)	frtn (fname)()
#  endif /* #  ifndef NARGS */
# endif
#endif /* #if defined(__cplusplus) */
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* Macros for miscellaneous casts . . .												*/
/*	***********************************************************************	*/
#ifdef __cplusplus
# define COMPAT_CAST_const(the_type, the_expression)			\
	(const_cast<the_type>(the_expression))
# define COMPAT_CAST_dynamic(the_type, the_expression)		\
	(dynamic_cast<the_type>(the_expression))
# define COMPAT_CAST_reinterpret(the_type, the_expression)	\
	(reinterpret_cast<the_type>(the_expression))
# define COMPAT_CAST_static(the_type, the_expression)			\
	(static_cast<the_type>(the_expression))
#else
# define COMPAT_CAST_const(the_type, the_expression)			\
	((the_type) (the_expression))
# define COMPAT_CAST_dynamic(the_type, the_expression)		\
	((the_type) (the_expression))
# define COMPAT_CAST_reinterpret(the_type, the_expression)	\
	((the_type) (the_expression))
# define COMPAT_CAST_static(the_type, the_expression)			\
	((the_type) (the_expression))
#endif /* #ifdef __cplusplus */
# define COMPAT_CAST_const_void(the_expression)					\
	COMPAT_CAST_static(const void *, (the_expression))
# define COMPAT_CAST_const_char(the_expression)					\
	COMPAT_CAST_static(const char *, (the_expression))
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* Macros for external data . . .														*/
/*	***********************************************************************	*/
#ifdef __cplusplus
# define COMPAT_EXTERN_DATA_DEF	extern "C"
# define COMPAT_EXTERN_DATA_DECL	extern "C"
#else
# define COMPAT_EXTERN_DATA_DEF	extern
# define COMPAT_EXTERN_DATA_DECL	extern
#endif /* #ifdef __cplusplus */
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	Macros for exception-handling compatibility...									*/
/*	***********************************************************************	*/
#ifdef __cplusplus
# ifndef MBCOMPAT_EXCEPT_NOT_SUPPORTED
#  define MBCOMPAT_EXCEPT_TRY								try
#  define MBCOMPAT_EXCEPT_CATCH(except)				catch(except)
#  define MBCOMPAT_EXCEPT_CATCH_ALL						catch(...)
#  define MBCOMPAT_EXCEPT_THROW(except)				throw(except)
#  define MBCOMPAT_EXCEPT_THROW_CTOR(except, data)	throw(except(data))
#  define MBCOMPAT_EXCEPT_RETHROW						throw
#  if _Windows
#   if _MSC_VER <= 1300
#    ifndef __GNUC__
#     ifndef MBCOMPAT_EXCEPT_NO_SPEC
#      define MBCOMPAT_EXCEPT_NO_SPEC		1
#      define MBCOMPAT_EXCEPT_NOTHROW					__declspec(nothrow)
#     endif /* #   ifndef MBCOMPAT_EXCEPT_NO_SPEC */
#    endif /* #ifndef __GNUC */
#   endif /* #  if _MSC_VER <= 1300 */
#  endif /* # if _Windows */
#  ifndef MBCOMPAT_EXCEPT_NO_SPEC
#   define MBCOMPAT_EXCEPT_NOTHROW						throw()
#   define MBCOMPAT_EXCEPT_CANTHROW(except)			throw(except)
#  else
#   ifndef MBCOMPAT_EXCEPT_NOTHROW
#    define MBCOMPAT_EXCEPT_NOTHROW
#   endif /* ifndef MBCOMPAT_EXCEPT_NOTHROW */
#   define MBCOMPAT_EXCEPT_CANTHROW(except)
#  endif /* # ifndef MBCOMPAT_EXCEPT_NO_SPEC */
# else
#  define MBCOMPAT_EXCEPT_TRY
#  define MBCOMPAT_EXCEPT_CATCH(except)
#  define MBCOMPAT_EXCEPT_CATCH_ALL
#  define MBCOMPAT_EXCEPT_THROW(except)
#  define MBCOMPAT_EXCEPT_THROW_CTOR(except, data)
#  define MBCOMPAT_EXCEPT_RETHROW
#  define MBCOMPAT_EXCEPT_NOTHROW
#  define MBCOMPAT_EXCEPT_CANTHROW(except)
# endif /* #ifndef MBCOMPAT_EXCEPT_NOT_SUPPORTED */
#endif /* #ifdef __cplusplus */
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*
	Used to determine the size of a structure, union, or class member when
	one hasn't got an instance handy...
*/
/*	***********************************************************************	*/
#ifndef MBCOMPAT_MemberSizeof
# define MBCOMPAT_MemberSizeof(structure_name, member_name) \
	sizeof(((structure_name *) 0)->member_name)
#endif /* #ifndef MBCOMPAT_MemberSizeof */
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*
	The macro 'offsetof' is defined by ANSI C. The following is defined for
	those systems which don't support it...
*/
/*	***********************************************************************	*/
#ifndef MBCOMPAT_Offsetof
# define MBCOMPAT_Offsetof(structure_name, member_name)	\
	((size_t) &(((structure_name *) 0)->member_name))
#endif /* #ifndef MBCOMPAT_Offsetof */
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/* To prevent the problems caused by the macro definitions 'min' and 'max'.*/
/*	***********************************************************************	*/
#ifdef __cplusplus
# ifdef _Windows
#  define HH__mbcompat_h__HH___Windows_MinMax_Guard	1
# elif WINDOWS
#  define HH__mbcompat_h__HH___Windows_MinMax_Guard	1
# elif WIN32
#  define HH__mbcompat_h__HH___Windows_MinMax_Guard	1
# endif /* #ifdef _Windows */

	/* If compiling under Windows... */
# ifdef HH__mbcompat_h__HH___Windows_MinMax_Guard
#  ifndef __MINGW32__
#   pragma warning(disable:4503 4710 4786)
#  endif /* # ifndef __MINGW32__ */
	/* ...And using a version of of MS VC++ less than or equal MSVC 2010... */
#  if _MSC_VER <= 1700
#   if _MSC_VER >= 1310
#    pragma warning(disable:4619 4820)
#    pragma warning(disable:4217)
#   endif /* if _MSC_VER >= 1310 */
#   if defined(_MSC_VER) && (_MSC_VER >= 1500)
#    pragma warning(disable:4548)
#   endif /* #if defined(_MSC_VER) && (_MSC_VER >= 1500) */
#   define NOMINMAX
#   ifndef __MINGW32__
#    pragma warning(disable:4018 4100 4146 4244 4245 4511 4512 4514 4663)
#   endif /* #   ifndef __MINGW32__ */
    /*
		Following occur in MSVC 2010 with /Wall. They've promised to fix in 2011.
		C:\Program Files\Microsoft Visual Studio 10.0\VC\include\crtdbg.h(1078): warning C4986: 'operator new[]': exception specification does not match previous declaration
		          C:\Program Files\Microsoft Visual Studio 10.0\VC\include\new(79) : see declaration of 'operator new[]'
		C:\Program Files\Microsoft Visual Studio 10.0\VC\include\crtdbg.h(1095): warning C4986: 'operator delete[]': exception specification does not match previous declaration
		          C:\Program Files\Microsoft Visual Studio 10.0\VC\include\new(77) : see declaration of 'operator delete[]'
    */
#   if _MSC_VER == 1600
#    pragma warning(push)
#    pragma warning(disable:4986)
#   endif /* #   if _MSC_VER == 1600 */
#   include <algorithm>
#   if _MSC_VER == 1600
#    pragma warning(pop)
#   endif /* #   if _MSC_VER == 1600 */
#   ifndef __MINGW32__
#    pragma warning(default:4018 4100 4146 4244 4245 4511 4512 4663)
#   endif /* #   ifndef __MINGW32__ */
#   ifdef min
#    undef min
#   endif /* #  ifdef min */
#   ifdef max
#    undef max
#   endif /* #  ifdef max */
#   ifdef _XUTILITY_
	/*
		... And including algorithm results in the include of xutility, this
		must be the Dinkumware STL implementation. If Visual C++ 6.0 (only),
		we define the min and max templates.
	*/
#    if _MSC_VER < 1300
		namespace std {
			template<class _Ty> inline
				const _Ty& max(const _Ty& _X, const _Ty& _Y)
					{return (_X < _Y ? _Y : _X); }
			template<class _Ty> inline
				const _Ty& min(const _Ty& _X, const _Ty& _Y)
				{return (_Y < _X ? _Y : _X); }
		} /* namespace std */
#    endif /* #    if _MSC_VER < 1300 */
#   endif /* # ifdef _XUTILITY_ */
#   if defined(_MSC_VER) && (_MSC_VER < 1500)
#    pragma warning(default:4548)
#   endif /* #if defined(_MSC_VER) && (_MSC_VER >= 1500) */
#   if _MSC_VER >= 1310
#    pragma warning(default:4217)
#   endif /* if _MSC_VER >= 1310 */
#  endif /* if _MSC_VER <= 1700 */
# endif /* #ifdef HH__mbcompat_h__HH___Windows_MinMax_Guard */
#endif /* #ifdef __cplusplus */
/*	***********************************************************************	*/

/*	***********************************************************************	*/
#ifdef _Windows
# define MBCOMPAT_HAS_THREADS				1
# define MBCOMPAT_THREAD_MODEL_WINDOWS	1
#elif _POSIX_C_SOURCE
# define MBCOMPAT_HAS_THREADS				1
# define MBCOMPAT_THREAD_MODEL_POSIX	1
#elif _POSIX_PTHREAD_SEMANTICS
# define MBCOMPAT_HAS_THREADS				1
# define MBCOMPAT_THREAD_MODEL_POSIX	1
#elif _POSIX_THREADS
# define MBCOMPAT_HAS_THREADS				1
# define MBCOMPAT_THREAD_MODEL_POSIX	1
#elif _REENTRANT
# define MBCOMPAT_HAS_THREADS				1
# define MBCOMPAT_THREAD_MODEL_UI		1
#endif /* #ifdef _Windows */
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	Generalized support for dynamic shared objects...								*/
/*	***********************************************************************	*/
#ifdef MB_LIB_STATIC
# define MB_LIB_IMPORT
# define MB_LIB_EXPORT
# define MB_LIB_LOCAL
# define MB_LIB_PUBLIC
#else
# ifdef _Windows
#  define MB_LIB_IMPORT 		__declspec(dllimport)
#  define MB_LIB_EXPORT 		__declspec(dllexport)
#  define MB_LIB_LOCAL
#  define MB_LIB_PUBLIC
# else
#  ifdef GCC_HASCLASSVISIBILITY
#   define MB_LIB_IMPORT
#   define MB_LIB_EXPORT 		__attribute__ ((visibility("default")))
#   define MB_LIB_LOCAL			__attribute__ ((visibility("hidden")))
#   define MB_LIB_PUBLIC		__attribute__ ((visibility("default")))
#  else
#   define MB_LIB_IMPORT
#   define MB_LIB_EXPORT
#   define MB_LIB_LOCAL
#   define MB_LIB_PUBLIC
#  endif /* ifdef GCC_HASCLASSVISIBILITY */
# endif /* #ifdef _Windows */
#endif /* #ifdef MB_LIB_STATIC */
/*	***********************************************************************	*/

/*	***********************************************************************	*/
/*	***********************************************************************	*/
/*	Support for C++ exceptions used across dynamic shared objects...			*/
/*	***********************************************************************	*/
#ifdef _Windows
# define MB_LIB_EXCEPTION_CLASS(some_api)		some_api
#elif defined(GCC_HASCLASSVISIBILITY)
# define MB_LIB_EXCEPTION_CLASS(some_api)		MB_LIB_EXPORT
#else
# define MB_LIB_EXCEPTION_CLASS(some_api)
#endif /* #ifdef _Windows */
/*	***********************************************************************	*/

#endif /* #ifndef HH__mbcompat_h__HH */

