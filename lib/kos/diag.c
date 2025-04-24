#include <kos/kos.h>

void k_diag_init(k_diag* diag, k_diag_callback callback, void* userdata) {
    *diag = (k_diag){
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

    if (diag_data.level == K_DIAG_ERROR && diag->error_count >= diag->error_limit) {
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
