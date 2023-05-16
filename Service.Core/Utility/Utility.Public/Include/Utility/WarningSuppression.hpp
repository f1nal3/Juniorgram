#pragma once

#ifdef _MSC_VER
#	define suppressWarning(MSVC_CODE, GCC_CLANG_TEXT) __pragma(warning(push)) \
													  __pragma(warning(disable: MSVC_CODE))
#	define restoreWarning __pragma(warning(pop))
#elif __GNUC__ 
#	define _doPragma(x) _Pragma(#x)
#	define suppressWarning(MSVC_CODE, GCC_CLANG_TEXT) _doPragma(GCC diagnostic push) \
													  _doPragma(GCC diagnostic ignored GCC_CLANG_TEXT)
#	define restoreWarning _doPragma(GCC diagnostic pop)
#elif __clang__
#	define _doPragma(x) _Pragma(#x)
#	define suppressWarning(MSVC_CODE, GCC_CLANG_TEXT) _doPragma(clang diagnostic push) \
													  _doPragma(clang diagnostic ignored GCC_CLANG_TEXT)
#	define restoreWarning _doPragma(clang diagnostic pop)
#endif
