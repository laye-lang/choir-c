#include <kos/kos.h>

#undef max
#define max(A, B) ((A) > (B) ? (A) : (B))

static const char* level_names[] = {
    "Ignored",
    "Note",
    "Remark",
    "Warning",
    "Error",
    "Fatal",
    NULL,
};

static const char* level_colors[] = {
    "",
    "\x1b[92m",
    "\x1b[93m",
    "\x1b[95m",
    "\x1b[91m",
    "\x1b[96m",
    NULL,
};

void k_diag_init(k_diag* diag, k_arena* string_arena, k_diag_callback callback, void* userdata) {
    *diag = (k_diag){
        .string_arena = string_arena,
        .callback = callback,
        .callback_userdata = userdata,
    };
}

void k_diag_deinit(k_diag* diag) {
    k_diag_flush(diag);
    k_da_free(&diag->diag_group);
    memset(diag, 0, sizeof *diag);
}

void k_diag_flush(k_diag* diag) {
    if (diag == nullptr) return;
    if (diag->diag_group.count == 0) return;

    if (diag->callback) {
        diag->callback(diag->callback_userdata, diag->diag_group);
    }

    diag->diag_group.count = 0;
}

void k_diag_emit(k_diag* diag, k_diag_data diag_data) {
    assert(diag != nullptr);

    if (diag_data.level != K_DIAG_NOTE) {
        k_diag_flush(diag);
    }

    if (diag_data.level == K_DIAG_ERROR && diag->error_count >= diag->error_limit && diag->error_limit != 0) {
        if (!diag->has_reported_error_limit_reached) {
            diag->has_reported_error_limit_reached = true;
            if (diag->callback) {
                assert(diag->diag_group.count == 0);
                k_da_push(&diag->diag_group, ((k_diag_data){ .level = K_DIAG_ERROR, .message = K_SV_CONST("") }));
                diag->callback(diag->callback_userdata, diag->diag_group);
                diag->diag_group.count = 0;
            }
        }

        return;
    }

    if (diag_data.level == K_DIAG_IGNORE) {
        diag->last_diag_was_ignored = true;
        return;
    }

    if (diag_data.level == K_DIAG_NOTE && diag->last_diag_was_ignored) {
        // we don't want to report notes which are attached to an ignored diagnostic.
        return;
    }

    if (diag_data.level >= K_DIAG_ERROR) {
        diag->error_limit++;
    }

    diag->last_diag_was_ignored = false;
    k_da_push(&diag->diag_group, diag_data);

    if (diag_data.level == K_DIAG_FATAL) {
        k_diag_flush(diag);
        abort();
    }
}

void k_diag_emitf(k_diag* diag, k_diag_level level, const char* format, ...) {
    va_list v;
    va_start(v, format);
    k_string msg = { .arena = diag->string_arena };
    k_vsprintf(&msg, format, v);
    va_end(v);

    k_diag_emit(diag, (k_diag_data){
        .level = level,
        .message = k_sv(msg.data, msg.count),
    });
}

void k_diag_emitsf(k_diag* diag, k_diag_level level, k_diag_source source, const char* format, ...) {
    va_list v;
    va_start(v, format);
    k_string msg = { .arena = diag->string_arena };
    k_vsprintf(&msg, format, v);
    va_end(v);

    k_diag_emit(diag, (k_diag_data){
        .level = level,
        .source = source,
        .message = k_sv(msg.data, msg.count),
    });
}

void k_diag_formatted(void* userdata, k_diag_data_group group) {
    k_diag_formatted_state state = *(k_diag_formatted_state*)userdata;

    int well_edge_width = 2;
    int well_number_width_min = 3;

    int well_inner_width = well_number_width_min;

    // TODO(local): calculate the actual inner width when we have line numbers & source rendering

    int well_inner_left_padding = well_edge_width + max(0, well_inner_width - 3);
    bool render_well_bottom = true;

    for (isize_t gi = 0; gi < group.count; gi++) {
        k_diag_data diag = group.data[gi];

        if (gi == 0)
            fprintf(stderr, "╭");
        else fprintf(stderr, "├");

        for (int i = 0; i < well_inner_left_padding - 1; i++)
            fprintf(stderr, "─");

        fprintf(stderr, "[");
        const char* level_name = level_names[diag.level];
        fprintf(stderr, "%s%s\x1b[0m", level_colors[diag.level], level_name);
        fprintf(stderr, "]");

        if (diag.source.name.count > 0) {
            fprintf(stderr, "@"K_STR_FMT, K_STR_EXPAND(diag.source.name));
            if (diag.source.use_byte_offset) {
                fprintf(stderr, "[%lld]", diag.source.byte_offset);
            } else {
                fprintf(stderr, "(%lld,%lld)", diag.source.line, diag.source.column);
            }

            fprintf(stderr, "\n│ ");
            for (int i = 0; i < well_inner_width; i++)
                fprintf(stderr, " ");
            fprintf(stderr, " ├─ ");
        } else fprintf(stderr, ": ");

        k_string_view message = diag.message;
        k_string_view message_line = k_sv_take_until(&message, '\n');
        fprintf(stderr, "\x1b[1m"K_STR_FMT"\x1b[0m\n", K_STR_EXPAND(message_line));

        while (message.count > 0) {
            fprintf(stderr, "│ ");
            for (int i = 0; i < well_inner_width; i++)
                fprintf(stderr, " ");
            fprintf(stderr, " │");
            for (isize_t i = 0; i < 6 + k_cast(isize_t)strlen(level_name) - (4 + well_inner_width); i++)
                fprintf(stderr, " ");

            message_line = k_sv_take_until(&message, '\n');
            fprintf(stderr, K_STR_FMT"\n", K_STR_EXPAND(message_line));
        }
    }

    if (render_well_bottom) {
        fprintf(stderr, "╰");
        for (int i = 0; i < well_inner_width + (2 * (well_edge_width - 1)); i++)
            fprintf(stderr, "─");
        fprintf(stderr, "╯");
    }

    fprintf(stderr, "\n");
}
