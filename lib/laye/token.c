#include <choir/core.h>

CHOIR_API const char* ch_token_kind_name_get(ch_token_kind kind) {
    switch (kind) {
        default: return "[unknown Laye source token kind]";
#define CH_TOKEN(id) \
    case CH_TK_##id: return #id;
#include "choir/tokens.inc"
    }
}
