#ifndef CH_TOKEN
#    define CH_TOKEN(id)
#endif

#ifndef CH_PUNCT
#    define CH_PUNCT(id, spelling) CH_TOKEN(id)
#endif

#ifndef CH_KEYWORD
#    define CH_KEYWORD(id, spelling, flags) CH_TOKEN(KW_##id)
#endif

#ifndef CH_PP
#    define CH_PP(id) CH_TOKEN(PP_##id)
#endif

#ifndef CH_PPKEYWORD
#    define CH_PPKEYWORD(id, spelling) CH_TOKEN(PP_##id)
#endif

#ifndef CH_PPKEYWORD2
#    define CH_PPKEYWORD2(id0, id1, spelling) CH_TOKEN(PP_##id0##id1)
#endif

///===--------------------------------------===///
/// Special tokens.
///===--------------------------------------===///

CH_TOKEN(INVALID)          // not a token
CH_TOKEN(END_OF_FILE)      // end of file
CH_TOKEN(CODE_COMPLETION) // code completion marker

///===--------------------------------------===///
/// Preprocessor tokens.
///===--------------------------------------===///

CH_PP(NOT_KEYWORD)       // A preprocessor identifier, none of the defined keywords.
CH_PP(END_OF_DIRECTIVE)  // End of preprocessing directive.
CH_PP(NUMBER)            // A C preprocessor number.
CH_PP(MACRO_PARAM)       // An identifier determined to be a macro parameter name.
/// Laye special identifiers.
CH_PP(LAYE_TOKEN_MACRO)  // A Laye identifier prefixed with '#', opts into token-based C macro expansions.
CH_PP(LAYE_EXPR_MACRO)   // A Laye identifier prefixed with '@', opts into Laye's expression-based C function-like macro expansions.
CH_PP(LAYE_NUMBER)       // A C preprocessor number, but with Laye number syntax.

/// === C23 6.10.1 - Conditional inclusion.
CH_PPKEYWORD(IF, "if")
CH_PPKEYWORD(ELIF, "elif")
CH_PPKEYWORD(ELSE, "else")
CH_PPKEYWORD(ENDIF, "endif")
CH_PPKEYWORD(IFDEF, "ifdef")
CH_PPKEYWORD(IFNDEF, "ifndef")
CH_PPKEYWORD(ELIFDEF, "elifdef")
CH_PPKEYWORD(ELIFNDEF, "elifndef")
CH_PPKEYWORD(DEFINED, "defined")
CH_PPKEYWORD(__HAS_INCLUDE, "__has_include")
CH_PPKEYWORD(__HAS_EMBED, "__has_embed")
CH_PPKEYWORD(__HAS_C_ATTRIBUTE, "__has_c_attribute")
/// GNU extensions.
CH_PPKEYWORD(__HAS_INCLUDE_NEXT, "__has_include_next")
CH_PPKEYWORD(__HAS_BUILTIN, "__has_builtin")
CH_PPKEYWORD(__HAS_ATTRIBUTE, "__has_attribute")
/// Clang extensions.
CH_PPKEYWORD(__HAS_FEATURE, "__has_feature")
CH_PPKEYWORD(__HAS_EXTENSION, "__has_extension")
CH_PPKEYWORD(__IS_IDENTIFIER, "__is_identifier")

/// === C23 6.10.2 - Source file inclusion.
CH_PPKEYWORD(INCLUDE, "include")
/// GNU extensions.
CH_PPKEYWORD(INCLUDE_NEXT, "include_next")

/// === C23 6.10.3 Binary resource inclusion.
CH_PPKEYWORD(EMBED, "embed")

/// === C23 6.10.4 - Macro replacement.
CH_PPKEYWORD(DEFINE, "define")
CH_PPKEYWORD(UNDEF, "undef")
CH_PPKEYWORD2(__VA, _OPT__, "__VA_OPT__")
CH_PPKEYWORD2(__VA, _ARGS__, "__VA_ARGS__")

/// === C23 6.10.5 - Line control.
CH_PPKEYWORD(LINE, "line")

/// === C23 6.10.6 - Diagnostic directives.
CH_PPKEYWORD(ERROR, "error")
CH_PPKEYWORD(WARNING, "warning")

/// === C23 6.10.7 - Pragma directive.
CH_PPKEYWORD(PRAGMA, "pragma")

/// === C23 6.10.9 - Predefined macro names.
CH_PPKEYWORD2(__LI, NE__, "__LINE__")
CH_PPKEYWORD2(__FI, LE__, "__FILE__")

/// === C23 6.10.10 - Pragma directive.
CH_PPKEYWORD(_PRAGMA, "_Pragma")

///===--------------------------------------===///
/// Language tokens.
///===--------------------------------------===///

/// === C23 6.4.2 - Identifiers.
CH_TOKEN(IDENTIFIER)
CH_TOKEN(RAW_IDENTIFIER)

/// === C23 6.4.4.1 - Integer constants.
CH_TOKEN(INTEGER_CONSTANT)

/// === C23 6.4.4.2 - Floating constants.
CH_TOKEN(FLOATING_CONSTANT)

/// === C23 6.4.4.4 - Character constants.
CH_TOKEN(CHARACTER_CONSTANT)        //   'a'
CH_TOKEN(WIDE_CHARACTER_CONSTANT)   //  L'a'
CH_TOKEN(UTF8_CHARACTER_CONSTANT)   // u8'a'
CH_TOKEN(UTF16_CHARACTER_CONSTANT)  //  u'a'
CH_TOKEN(UTF32_CHARACTER_CONSTANT)  //  U'a'

/// === C23 6.4.5 - String literals.
CH_TOKEN(STRING_LITERAL)        //   "foo"
CH_TOKEN(WIDE_STRING_LITERAL)   //  L"foo"
CH_TOKEN(UTF8_STRING_LITERAL)   // u8"foo"
CH_TOKEN(UTF16_STRING_LITERAL)  //  u"foo"
CH_TOKEN(UTF32_STRING_LITERAL)  //  U"foo"

/// === C23 6.4.7 - Header names.
CH_TOKEN(HEADER_NAME)

/// === C23 6.4.6 - Punctuators.
CH_PUNCT(HASH, "#")
CH_PUNCT(HASH_HASH, "##")
CH_PUNCT(OPEN_PAREN, "(")
CH_PUNCT(CLOSE_PAREN, ")")
CH_PUNCT(OPEN_SQUARE, "[")
CH_PUNCT(CLOSE_SQUARE, "]")
CH_PUNCT(OPEN_CURLY, "{")
CH_PUNCT(CLOSE_CURLY, "}")
CH_PUNCT(COMMA, ",")
CH_PUNCT(SEMI_COLON, ";")
CH_PUNCT(DOT, ".")
CH_PUNCT(DOT_DOT_DOT, "...")
CH_PUNCT(COLON, ":")
CH_PUNCT(COLON_COLON, "::")
CH_PUNCT(EQUAL, "=")
CH_PUNCT(EQUAL_EQUAL, "==")
CH_PUNCT(EQUAL_GREATER, "=>")
CH_PUNCT(BANG, "!")
CH_PUNCT(BANG_EQUAL, "!=")
CH_PUNCT(LESS, "<")
CH_PUNCT(LESS_EQUAL, "<=")
CH_PUNCT(LESS_LESS, "<<")
CH_PUNCT(LESS_LESS_EQUAL, "<<=")
CH_PUNCT(GREATER, ">")
CH_PUNCT(GREATER_EQUAL, ">=")
CH_PUNCT(GREATER_GREATER, ">>")
CH_PUNCT(GREATER_GREATER_EQUAL, ">>=")
CH_PUNCT(PLUS, "+")
CH_PUNCT(PLUS_EQUAL, "+=")
CH_PUNCT(PLUS_PLUS, "++")
CH_PUNCT(MINUS, "-")
CH_PUNCT(MINUS_EQUAL, "-=")
CH_PUNCT(MINUS_MINUS, "--")
CH_PUNCT(MINUS_GREATER, "->")
CH_PUNCT(STAR, "*")
CH_PUNCT(STAR_EQUAL, "*=")
CH_PUNCT(SLASH, "/")
CH_PUNCT(SLASH_EQUAL, "/=")
CH_PUNCT(PERCENT, "%")
CH_PUNCT(PERCENT_EQUAL, "%=")
CH_PUNCT(CARET, "^")
CH_PUNCT(CARET_EQUAL, "^=")
CH_PUNCT(TILDE, "~")
CH_PUNCT(AMPERSAND, "&")
CH_PUNCT(AMPERSAND_EQUAL, "&=")
CH_PUNCT(AMPERSAND_AMPERSAND, "&&")
CH_PUNCT(PIPE, "|")
CH_PUNCT(PIPE_EQUAL, "|=")
CH_PUNCT(PIPE_PIPE, "||")
CH_PUNCT(QUESTION, "?")

/// === Laye - (Non-C) Punctuators.
CH_PUNCT(HASH_SQUARE, "#[")
CH_PUNCT(DOT_DOT, "..")
CH_PUNCT(DOT_DOT_EQUAL, "..=")
CH_PUNCT(LESS_EQUAL_GREATER, "<=>")
CH_PUNCT(TILDE_EQUAL, "~=")
CH_PUNCT(QUESTION_QUESTION, "??")
CH_PUNCT(QUESTION_QUESTION_EQUAL, "??=")

/// === C23 6.4.1 - Keywords.
///
/// These turn into CH_TKKW_* tokens.
/// Flags allowed:
///   CH_KWFLAG_ALL        - This is a keyword in all variants of C and Laye.
///   CH_KWFLAG_C          - This is a keyword in all variants of C.
///   CH_KWFLAG_LAYE       - This is a keyword in all variants of Laye.
///   CH_KWFLAG_C99        - This is a keyword introduced to C in C99.
///   CH_KWFLAG_C23        - This is a keyword introduced to C in C23.
///
///   CH_KWFLAG_GNU        - This is a keyword if GNU extensions are enabled.
///   CH_KWFLAG_MS         - This is a keyword if Microsoft extensions are enabled.
///   CH_KWFLAG_MS_COMPAT  - This is a keyword if Microsoft compatibility mode is enabled.
///   CH_KWFLAG_CLANG      - This is a keyword if Clang extensions are enabled.
///   CH_KWFLAG_BORLAND    - This is a keyword if Borland extensions are enabled.
///   CH_KWFLAG_CHOIR      - This is a keyword if Choir extensions are enabled.
///
///   CH_KWFLAG_BOOL       - This is a keyword if 'bool' is a built-in type in C.
///   CH_KWFLAG_WCHAR      - This is a keyword if 'wchar_t' is a built-in type in C.
CH_KEYWORD(ALIGNAS, "alignas", CH_TKKEY_C)
CH_KEYWORD(ALIGNOF, "alignof", CH_TKKEY_C23 | CH_TKKEY_LAYE)
CH_KEYWORD(AUTO, "auto", CH_TKKEY_C)
CH_KEYWORD(BOOL, "bool", CH_TKKEY_BOOL | CH_TKKEY_C23 | CH_TKKEY_LAYE)
CH_KEYWORD(BREAK, "break", CH_TKKEY_ALL)
CH_KEYWORD(CASE, "case", CH_TKKEY_ALL)
CH_KEYWORD(CHAR, "char", CH_TKKEY_C)
CH_KEYWORD(CONST, "const", CH_TKKEY_ALL)
CH_KEYWORD(CONSTEXPR, "constexpr", CH_TKKEY_ALL)
CH_KEYWORD(CONTINUE, "continue", CH_TKKEY_ALL)
CH_KEYWORD(DEFAULT, "default", CH_TKKEY_ALL)
CH_KEYWORD(DO, "do", CH_TKKEY_ALL)
CH_KEYWORD(DOUBLE, "double", CH_TKKEY_C)
CH_KEYWORD(ELSE, "else", CH_TKKEY_ALL)
CH_KEYWORD(ENUM, "enum", CH_TKKEY_ALL)
CH_KEYWORD(EXTERN, "extern", CH_TKKEY_C)
CH_KEYWORD(FALSE, "false", CH_TKKEY_C23 | CH_TKKEY_LAYE)
CH_KEYWORD(FLOAT, "float", CH_TKKEY_C)
CH_KEYWORD(FOR, "for", CH_TKKEY_ALL)
CH_KEYWORD(GOTO, "goto", CH_TKKEY_ALL)
CH_KEYWORD(IF, "if", CH_TKKEY_ALL)
CH_KEYWORD(INLINE, "inline", CH_TKKEY_ALL)
CH_KEYWORD(INT, "int", CH_TKKEY_ALL)
CH_KEYWORD(LONG, "long", CH_TKKEY_C)
CH_KEYWORD(NULLPTR, "nullptr", CH_TKKEY_C23)
CH_KEYWORD(REGISTER, "register", CH_TKKEY_ALL)
CH_KEYWORD(RESTRICT, "restrict", CH_TKKEY_C)
CH_KEYWORD(RETURN, "return", CH_TKKEY_ALL)
CH_KEYWORD(SHORT, "short", CH_TKKEY_C)
CH_KEYWORD(SIGNED, "signed", CH_TKKEY_C)
CH_KEYWORD(SIZEOF, "sizeof", CH_TKKEY_ALL)
CH_KEYWORD(STATIC, "static", CH_TKKEY_ALL)
CH_KEYWORD(STATIC_ASSERT, "static_assert", CH_TKKEY_C)
CH_KEYWORD(STRUCT, "struct", CH_TKKEY_ALL)
CH_KEYWORD(SWITCH, "switch", CH_TKKEY_ALL)
CH_KEYWORD(THREAD_LOCAL, "thread_local", CH_TKKEY_ALL)
CH_KEYWORD(TRUE, "true", CH_TKKEY_C23 | CH_TKKEY_LAYE)
CH_KEYWORD(TYPEDEF, "typedef", CH_TKKEY_C)
CH_KEYWORD(TYPEOF, "typeof", CH_TKKEY_C23 | CH_TKKEY_LAYE)
CH_KEYWORD(TYPEOF_UNQUAL, "typeof_unqual", CH_TKKEY_C23 | CH_TKKEY_LAYE)
CH_KEYWORD(UNION, "union", CH_TKKEY_C)
CH_KEYWORD(UNSIGNED, "unsigned", CH_TKKEY_C)
CH_KEYWORD(VOID, "void", CH_TKKEY_ALL)
CH_KEYWORD(VOLATILE, "volatile", CH_TKKEY_C)
CH_KEYWORD(WHILE, "while", CH_TKKEY_ALL)
CH_KEYWORD(_BITINT, "_BitInt", CH_TKKEY_C)
CH_KEYWORD(_ATOMIC, "_Atomic", CH_TKKEY_C)
CH_KEYWORD(_COMPLEX, "_Complex", CH_TKKEY_C)
CH_KEYWORD(_DECIMAL128, "_Decimal128", CH_TKKEY_C)
CH_KEYWORD(_DECIMAL32, "_Decimal32", CH_TKKEY_C)
CH_KEYWORD(_DECIMAL64, "_Decimal64", CH_TKKEY_C)
CH_KEYWORD(_GENERIC, "_Generic", CH_TKKEY_C)
CH_KEYWORD(_IMAGINARY, "_Imaginary", CH_TKKEY_C)
CH_KEYWORD(_NORETURN, "_Noreturn", CH_TKKEY_C)
/// C Standard keyword aliases.
CH_KEYWORD(_ALIGNAS, "_Alignas", CH_TKKEY_C)
CH_KEYWORD(_ALIGNOF, "_Alignof", CH_TKKEY_C)
CH_KEYWORD(_BOOL, "_Bool", CH_TKKEY_C)
CH_KEYWORD(_STATIC_ASSERT, "_Static_assert", CH_TKKEY_C)
CH_KEYWORD(_THREAD_LOCAL, "_Thread_local", CH_TKKEY_C)
/// GNU extensions.
CH_KEYWORD(ASM, "asm", CH_TKKEY_GNU)
CH_KEYWORD(__ASM__, "__asm__", CH_TKKEY_GNU)
CH_KEYWORD(__ATTRIBUTE__, "__attribute__", CH_TKKEY_GNU)
CH_KEYWORD(__AUTO_TYPE, "__auto_type", CH_TKKEY_GNU)
CH_KEYWORD(__THREAD, "__thread", CH_TKKEY_GNU)
/// Clang extensions.
/// Microsoft extensions.
/// Microsoft compatibility mode.


/// === Laye - (Non-C) Keywords.
CH_KEYWORD(ALIAS, "alias", CH_TKKEY_LAYE)
CH_KEYWORD(AND, "and", CH_TKKEY_LAYE)
CH_KEYWORD(AS, "as", CH_TKKEY_LAYE)
CH_KEYWORD(ASSERT, "assert", CH_TKKEY_LAYE)
CH_KEYWORD(BOOL_SIZED, NULL, CH_TKKEY_LAYE)
CH_KEYWORD(__BUILTIN_FFI_BOOL, "__builtin_ffi_bool", CH_TKKEY_LAYE)
CH_KEYWORD(__BUILTIN_FFI_CHAR, "__builtin_ffi_char", CH_TKKEY_LAYE)
CH_KEYWORD(__BUILTIN_FFI_SHORT, "__builtin_ffi_short", CH_TKKEY_LAYE)
CH_KEYWORD(__BUILTIN_FFI_INT, "__builtin_ffi_int", CH_TKKEY_LAYE)
CH_KEYWORD(__BUILTIN_FFI_LONG, "__builtin_ffi_long", CH_TKKEY_LAYE)
CH_KEYWORD(__BUILTIN_FFI_LONG_LONG, "__builtin_ffi_long_long", CH_TKKEY_LAYE)
CH_KEYWORD(__BUILTIN_FFI_FLOAT, "__builtin_ffi_float", CH_TKKEY_LAYE)
CH_KEYWORD(__BUILTIN_FFI_DOUBLE, "__builtin_ffi_double", CH_TKKEY_LAYE)
CH_KEYWORD(__BUILTIN_FFI_LONG_DOUBLE, "__builtin_ffi_long_double", CH_TKKEY_LAYE)
CH_KEYWORD(CALLCONV, "callconv", CH_TKKEY_LAYE)
CH_KEYWORD(CAST, "cast", CH_TKKEY_LAYE)
CH_KEYWORD(COUNTOF, "countof", CH_TKKEY_LAYE)
CH_KEYWORD(DEFER, "defer", CH_TKKEY_LAYE)
CH_KEYWORD(DELEGATE, "delegate", CH_TKKEY_LAYE)
CH_KEYWORD(DELETE, "delete", CH_TKKEY_LAYE)
CH_KEYWORD(DISCARD, "discard", CH_TKKEY_LAYE)
CH_KEYWORD(DISCARDABLE, "discardable", CH_TKKEY_LAYE)
CH_KEYWORD(EVAL, "eval", CH_TKKEY_LAYE)
CH_KEYWORD(EXPORT, "export", CH_TKKEY_LAYE)
CH_KEYWORD(FALLTHROUGH, "fallthrough", CH_TKKEY_LAYE)
CH_KEYWORD(FLOAT32, "float32", CH_TKKEY_LAYE)
CH_KEYWORD(FLOAT64, "float64", CH_TKKEY_LAYE)
CH_KEYWORD(FOREIGN, "foreign", CH_TKKEY_LAYE)
CH_KEYWORD(FROM, "from", CH_TKKEY_LAYE)
CH_KEYWORD(GLOBAL, "global", CH_TKKEY_LAYE)
CH_KEYWORD(IMPORT, "import", CH_TKKEY_LAYE)
CH_KEYWORD(INT_SIZED, NULL, CH_TKKEY_LAYE)
CH_KEYWORD(IS, "is", CH_TKKEY_LAYE)
CH_KEYWORD(MODULE, "module", CH_TKKEY_LAYE)
CH_KEYWORD(MUT, "mut", CH_TKKEY_LAYE)
CH_KEYWORD(NEW, "new", CH_TKKEY_LAYE)
CH_KEYWORD(NIL, "nil", CH_TKKEY_LAYE)
CH_KEYWORD(NORETURN, "noreturn", CH_TKKEY_LAYE)
CH_KEYWORD(NOT, "not", CH_TKKEY_LAYE)
CH_KEYWORD(OFFSETOF, "offsetof", CH_TKKEY_LAYE)
CH_KEYWORD(OPERATOR, "opperator", CH_TKKEY_LAYE)
CH_KEYWORD(OR, "or", CH_TKKEY_LAYE)
CH_KEYWORD(PRAGMA, "pragma", CH_TKKEY_LAYE)
CH_KEYWORD(RANKOF, "rankof", CH_TKKEY_LAYE)
CH_KEYWORD(REF, "ref", CH_TKKEY_LAYE)
CH_KEYWORD(STRICT, "strict", CH_TKKEY_LAYE)
CH_KEYWORD(TEMPLATE, "template", CH_TKKEY_LAYE)
CH_KEYWORD(TEST, "test", CH_TKKEY_LAYE)
CH_KEYWORD(UNREACHABLE, "unreachable", CH_TKKEY_LAYE)
CH_KEYWORD(VAR, "var", CH_TKKEY_LAYE)
CH_KEYWORD(VARARGS, "varargs", CH_TKKEY_LAYE)
CH_KEYWORD(VARIANT, "variant", CH_TKKEY_LAYE)
CH_KEYWORD(XOR, "xor", CH_TKKEY_LAYE)
CH_KEYWORD(XYZZY, "xyzzy", CH_TKKEY_LAYE)
CH_KEYWORD(YIELD, "yield", CH_TKKEY_LAYE)

#undef CH_TOKEN
#undef CH_PUNCT
#undef CH_KEYWORD
#undef CH_PP
#undef CH_PPKEYWORD
#undef CH_PPKEYWORD2
