#ifndef LCONFIG_H_
#define LCONFIG_H_

/*
@@ CLAYE_USE_C11 controlls the use of non-C11 features.
** Define it if you want 'claye' to avoid the use of C23 features,
** or Windows-specific features on Windows.
*/
/* #define CLAYE_USE_C11 */

#if !defined(CLAYE_USE_C11) && defined(_WIN32)
#    define CLAYE_USE_WINDOWS
#endif

#if defined(CLAYE_USE_WINDOWS)
#    define CLAYE_USE_DLL
#    if defined(_MSC_VER)
#        define CLAYE_USE_C11
#    endif
#endif

#if defined(CLAYE_USE_LINUX)
#    define CLAYE_USE_POSIX
#    define CLAYE_USE_DLOPEN
#endif

#if defined(CLAYE_BUILD_AS_DLL)
#    if defined(CLAYE_LIB)
#        define CLAYE_API __declspec(dllexport)
#    else
#        define CLAYE_API __declspec(dllimport)
#    endif
#else /* CLAYE_BUILD_AS_DLL */
#    define CLAYE_API extern
#endif

#endif /* LCONFIG_H_ */
