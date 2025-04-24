#include "config.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ODIR "out"

#define LIBCHOIR_STATIC_LIBRARY_FILE "libchoir"

#define CHOIR_EXECUTABLE_FILE "choir"
#define LAYEC_EXECUTABLE_FILE "layec"
#define CCLY_EXECUTABLE_FILE  "ccly"

#if defined(NOBCONFIG_MISSING)
#    error No nob configuration has been specified. Please copy the relevant config file from the config directory for your platform and toolchain into the appropriate '<PLATFORM>.h' file.
#endif

#define NOB_IMPLEMENTATION
#include "nob.h"

typedef struct {
    const char* source_file;
    const char* object_file;
} source_paths;

static source_paths libchoir_files[] = {
    {"lib/kos/arena.c", ODIR "/kos-arena.o"},
    {"lib/kos/diag.c", ODIR "/kos-diag.o"},
    {"lib/kos/string.c", ODIR "/kos-string.o"},
    {"lib/kos/unicode.c", ODIR "/kos-unicode.o"},

    {"lib/choir/context.c", ODIR "/choir-context.o"},
    {"lib/choir/size_align.c", ODIR "/choir-size_align.o"},

    {"lib/laye/lex.c", ODIR "/laye-lex.o"},
    {"lib/laye/pp.core.c", ODIR "/laye-pp-core.o"},
    {"lib/laye/token.c", ODIR "/laye-token.o"},

    {0},
};

static source_paths layec_files[] = {
    {"src/layec.c", ODIR "/layec.o"},
    {0},
};

static source_paths ccly_files[] = {
    {"src/ccly.c", ODIR "/ccly.o"},
    {0},
};

static source_paths choir_files[] = {
    {"src/choir.c", ODIR "/choir.o"},
    {0},
};

static Nob_File_Paths all_header_files = {0};

static bool compile_object(const char* source_path, const char* object_path, const char* source_root) {
    bool result = true;

    Nob_Cmd cmd = {0};
    if (0 == nob_needs_rebuild1(object_path, source_path) && 0 == nob_needs_rebuild(object_path, all_header_files.items, all_header_files.count)) {
        nob_return_defer(true);
    }

    nob_cmd_append(&cmd, CC);
#if defined(CC_MSVC)
    nob_cmd_append(&cmd, "/nologo");
    nob_cmd_append(&cmd, "/c", source_path);
    nob_cmd_append(&cmd, nob_temp_sprintf("/Fo%s", object_path));
    nob_cmd_append(&cmd, nob_temp_sprintf("/I%s/include", source_root));
#else // !CC_MSVC
    nob_cmd_append(&cmd, "-c", source_path);
    nob_cmd_append(&cmd, "-o", object_path);
    nob_cmd_append(&cmd, nob_temp_sprintf("-I%s/include", source_root));
#endif
    nob_cmd_append(&cmd, "" CFLAGS "");

    if (!nob_cmd_run_sync(cmd)) {
        nob_return_defer(false);
    }

defer:;
    nob_cmd_free(cmd);
    return result;
}

static bool package_library(Nob_File_Paths object_files, const char* library_path) {
    bool result = true;

    Nob_Cmd cmd = {0};
    if (0 == nob_needs_rebuild(library_path, object_files.items, object_files.count)) {
        nob_return_defer(true);
    }

    nob_cmd_append(&cmd, LIB);
#if defined(LIB_MSVC)
    nob_cmd_append(&cmd, "/nologo");
    nob_cmd_append(&cmd, nob_temp_sprintf("/out:%s", library_path));
#else // !LIB_MSVC
    nob_cmd_append(&cmd, "rcs", library_path);
#endif
    nob_da_append_many(&cmd, object_files.items, object_files.count);

    if (!nob_cmd_run_sync(cmd)) {
        nob_return_defer(false);
    }

defer:;
    nob_cmd_free(cmd);
    return result;
}

static bool link_executable(Nob_File_Paths input_paths, const char* executable_path) {
    bool result = true;

    Nob_Cmd cmd = {0};
    if (0 == nob_needs_rebuild(executable_path, input_paths.items, input_paths.count)) {
        nob_return_defer(true);
    }

    nob_cmd_append(&cmd, LD);
#if defined(LD_MSVC)
    nob_cmd_append(&cmd, "/nologo", "/subsystem:console");
    nob_cmd_append(&cmd, nob_temp_sprintf("/out:%s", executable_path));
    nob_cmd_append(&cmd, nob_temp_sprintf("/pdb:%s.pdb", executable_path));
#else // !LD_MSVC
    nob_cmd_append(&cmd, "-o", executable_path);
#endif
    nob_cmd_append(&cmd, "" LDFLAGS "");
    nob_da_append_many(&cmd, input_paths.items, input_paths.count);

    if (!nob_cmd_run_sync(cmd)) {
        nob_return_defer(false);
    }

defer:;
    nob_cmd_free(cmd);
    return result;
}

static void clean(void) {
    if (nob_file_exists("./choir")) remove("./choir");
    if (nob_file_exists("./choir.exe")) remove("./choir.exe");

    if (nob_file_exists("./layec")) remove("./layec");
    if (nob_file_exists("./layec.exe")) remove("./layec.exe");

    if (nob_file_exists("./ccly")) remove("./ccly");
    if (nob_file_exists("./ccly.exe")) remove("./ccly.exe");

    Nob_File_Paths outs = {0};
    nob_read_entire_dir(ODIR, &outs);
    for (size_t i = 2; i < outs.count; i++) {
        remove(nob_temp_sprintf(ODIR "/%s", outs.items[i]));
    }

    remove(ODIR);
}

static bool build_object_files(const char* source_root, source_paths* sources, Nob_File_Paths* object_paths) {
    bool result = true;

    for (int64_t i = 0; sources[i].source_file != 0; i++) {
        const char* source_file = nob_temp_sprintf("%s/%s", source_root, sources[i].source_file);
        if (!compile_object(source_file, sources[i].object_file, source_root)) {
            nob_return_defer(false);
        }

        nob_da_append(object_paths, sources[i].object_file);
    }

defer:
    return result;
}

// The implementation idea is stolen from https://github.com/zhiayang/nabs
static void _go_rebuild_urself(const char* source_path, int argc, char** argv) {
    const char* binary_path = nob_shift(argv, argc);
#ifdef _WIN32
    // On Windows executables almost always invoked without extension, so
    // it's ./nob, not ./nob.exe. For renaming the extension is a must.
    if (!nob_sv_end_with(nob_sv_from_cstr(binary_path), ".exe")) {
        binary_path = nob_temp_sprintf("%s.exe", binary_path);
    }
#endif

    Nob_File_Paths file_paths = {0};
    nob_da_append(&file_paths, source_path);
    nob_da_append(&file_paths, "config.h");

    int rebuild_is_needed = nob_needs_rebuild(binary_path, file_paths.items, file_paths.count);
    nob_da_free(file_paths);

    if (rebuild_is_needed < 0) exit(1); // error
    if (!rebuild_is_needed) return;     // no rebuild is needed

    Nob_Cmd cmd = {0};

    const char* old_binary_path = nob_temp_sprintf("%s.old", binary_path);

    if (!nob_rename(binary_path, old_binary_path)) exit(1);
    nob_cmd_append(&cmd, NOB_REBUILD_URSELF(binary_path, source_path));
    if (!nob_cmd_run_sync_and_reset(&cmd)) {
        nob_rename(old_binary_path, binary_path);
        exit(1);
    }

    nob_cmd_append(&cmd, binary_path);
    nob_da_append_many(&cmd, argv, argc);
    if (!nob_cmd_run_sync_and_reset(&cmd)) exit(1);
    exit(0);
}

int main(int argc, char** argv) {
    _go_rebuild_urself(__FILE__, argc, argv);

    bool result = 0;

    const char* program_name = nob_shift_args(&argc, &argv);

    if (argc > 0) {
        const char* arg = nob_shift_args(&argc, &argv);
        if (0 == strcmp(arg, "clean")) {
            clean();
            nob_return_defer(0);
        }
    }

    if (!nob_mkdir_if_not_exists(ODIR)) {
        nob_return_defer(1);
    }

    const char* source_root = ".";
    while (!nob_file_exists(nob_temp_sprintf("%s/include", source_root))) {
        source_root = nob_temp_sprintf("%s/..", source_root);
    }

    Nob_File_Paths include_file_paths = {0};
    if (!nob_read_entire_dir(nob_temp_sprintf("%s/include/choir", source_root), &include_file_paths)) {
        nob_return_defer(1);
    }

    for (size_t i = 2; i < include_file_paths.count; i++) {
        nob_da_append(&all_header_files, nob_temp_sprintf("%s/include/choir/%s", source_root, include_file_paths.items[i]));
    }

    include_file_paths.count = 0;
    if (!nob_read_entire_dir(nob_temp_sprintf("%s/include/kos", source_root), &include_file_paths)) {
        nob_return_defer(1);
    }

    for (size_t i = 2; i < include_file_paths.count; i++) {
        nob_da_append(&all_header_files, nob_temp_sprintf("%s/include/kos/%s", source_root, include_file_paths.items[i]));
    }

    nob_da_free(include_file_paths);

    Nob_File_Paths libchoir_object_paths = {0};
    if (!build_object_files(source_root, libchoir_files, &libchoir_object_paths)) {
        nob_return_defer(1);
    }

    const char* libfile = ODIR "/" LIB_PREFIX LIBCHOIR_STATIC_LIBRARY_FILE LIB_EXT;
    if (!package_library(libchoir_object_paths, libfile)) {
        nob_return_defer(1);
    }

    Nob_File_Paths layec_input_paths = {0};
    if (!build_object_files(source_root, layec_files, &layec_input_paths)) {
        nob_return_defer(1);
    }

    nob_da_append(&layec_input_paths, libfile);
    const char* layecfile = ODIR "/" LAYEC_EXECUTABLE_FILE EXE_EXT;
    if (!link_executable(layec_input_paths, layecfile)) {
        nob_return_defer(1);
    }

    Nob_File_Paths ccly_input_paths = {0};
    if (!build_object_files(source_root, ccly_files, &ccly_input_paths)) {
        nob_return_defer(1);
    }

    nob_da_append(&ccly_input_paths, libfile);
    const char* cclyfile = ODIR "/" CCLY_EXECUTABLE_FILE EXE_EXT;
    if (!link_executable(ccly_input_paths, cclyfile)) {
        nob_return_defer(1);
    }

    Nob_File_Paths choir_input_paths = {0};
    if (!build_object_files(source_root, choir_files, &choir_input_paths)) {
        nob_return_defer(1);
    }

    nob_da_append(&choir_input_paths, libfile);
    const char* choirfile = ODIR "/" CHOIR_EXECUTABLE_FILE EXE_EXT;
    if (!link_executable(choir_input_paths, choirfile)) {
        nob_return_defer(1);
    }

    if (1 == nob_needs_rebuild1(LAYEC_EXECUTABLE_FILE EXE_EXT, layecfile)) {
        if (!nob_copy_file(layecfile, LAYEC_EXECUTABLE_FILE EXE_EXT)) {
            nob_return_defer(1);
        }
    }

    if (1 == nob_needs_rebuild1(CCLY_EXECUTABLE_FILE EXE_EXT, cclyfile)) {
        if (!nob_copy_file(cclyfile, CCLY_EXECUTABLE_FILE EXE_EXT)) {
            nob_return_defer(1);
        }
    }

    if (1 == nob_needs_rebuild1(CHOIR_EXECUTABLE_FILE EXE_EXT, choirfile)) {
        if (!nob_copy_file(choirfile, CHOIR_EXECUTABLE_FILE EXE_EXT)) {
            nob_return_defer(1);
        }
    }

defer:;
    return result;
}
