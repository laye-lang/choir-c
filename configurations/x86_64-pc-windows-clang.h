#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE 1

#define CC "clang"
#define LIB "lib"
#define LD "clang"

#define LIB_MSVC

#define LCONFIG
#define CFLAGS "-std=c23", "-Wall", "-Wextra", "-Wno-gnu-zero-variadic-macro-arguments", "-Wno-trigraphs", "-Wno-unused", "-Wno-unused-parameter", "-Wno-unused-function", "-Wno-unused-variable", "-Werror", "-Werror=return-type", "-pedantic", "-pedantic-errors", "-ggdb", "-fsanitize=address" LCONFIG
#define LDFLAGS "-fsanitize=address"

#define EXE_EXT ".exe"
#define LIB_EXT ".lib"
#define LIB_PREFIX ""
