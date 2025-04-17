#include <choir/core.h>

CHOIR_API const char* ch_token_kind_name_get(ch_token_kind kind) {
    switch (kind) {
        default: return "[unknown Laye source token kind]";
#define CH_TOKEN(id) \
    case CH_TK_##id: return #id;
#include "choir/tokens.inc"
    }
}

CHOIR_API const char* ch_token_kind_spelling_get(ch_token_kind kind) {
    switch (kind) {
        default: return NULL;
#define CH_TOKEN(id) \
    case CH_TK_##id: return NULL;
#define CH_PUNCT(id, spelling) \
    case CH_TK_##id: return spelling;
#define CH_KEYWORD(id, spelling, flags) \
    case CH_TK_KW_##id: return spelling;
#define CH_PPKEYWORD(id, spelling) \
    case CH_TK_PP_##id: return spelling;
#define CH_PPKEYWORD2(id0, id1, spelling) \
    case CH_TK_PP_##id0##id1: return spelling;
#include "choir/tokens.inc"
    }
}

CHOIR_API ch_token_key ch_token_kind_key_get(ch_token_kind kind) {
    switch (kind) {
        default: return CH_TKKEY_NOT_KW;
#define CH_TOKEN(id) \
    case CH_TK_##id: return CH_TKKEY_NOT_KW;
#define CH_KEYWORD(id, spelling, flags) \
    case CH_TK_KW_##id: return flags;
#include "choir/tokens.inc"
    }
}
