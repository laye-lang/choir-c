#include <laye/core.h>

CHOIR_API const char* ly_token_kind_get_name(ly_token_kind kind) {
    switch (kind) {
        default: return "[unknown Laye source token kind]";
#define LY_TOKEN(id) \
    case LY_TK_##id: return #id;
#include <laye/tokens.h>
    }
}

CHOIR_API const char* ly_token_kind_get_spelling(ly_token_kind kind) {
    switch (kind) {
        default: return NULL;
#define LY_TOKEN(id) \
    case LY_TK_##id: return NULL;
#define CH_PUNCT(id, spelling) \
    case LY_TK_##id: return spelling;
#define CH_KEYWORD(id, spelling, flags) \
    case LY_TK_KW_##id: return spelling;
#define CH_PPKEYWORD(id, spelling) \
    case LY_TK_PP_##id: return spelling;
#define CH_PPKEYWORD2(id0, id1, spelling) \
    case LY_TK_PP_##id0##id1: return spelling;
#include <laye/tokens.h>
    }
}

CHOIR_API ly_token_key ly_token_kind_get_key(ly_token_kind kind) {
    switch (kind) {
        default: return LY_TKKEY_NOT_KW;
#define LY_TOKEN(id) \
    case LY_TK_##id: return LY_TKKEY_NOT_KW;
#define CH_KEYWORD(id, spelling, flags) \
    case LY_TK_KW_##id: return flags;
#include <laye/tokens.h>
    }
}
