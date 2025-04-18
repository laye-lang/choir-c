#define CC "gcc"
#define LIB "ar"
#define LD "gcc"

#define LCONFIG , "-DLAYE_USE_LINUX"
#define CFLAGS "-std=c23", "-Wall", "-Wextra", "-Wno-trigraphs", "-Wno-unused-parameter", "-Werror", "-Werror=return-type", "-pedantic", "-pedantic-errors", "-ggdb", "-fsanitize=address" LCONFIG
#define LDFLAGS "-ggdb", "-fsanitize=address"

#define EXE_EXT ""
#define LIB_EXT ".a"
#define LIB_PREFIX "lib"
