#ifndef LAYE_H_
#define LAYE_H_

#if defined(__cplusplus)
extern "C" {
#endif // defined(__cplusplus)

#include "lconfig.h"
#include "macros.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float float32;
typedef double float64;

typedef size_t usize;
typedef ptrdiff_t isize;

/// TODO(local): We need to consider unsigned, wrapping and saturating operators *or* their keyword/expression syntax otherwise.
/// They are not included in the Laye source token kind enum until it is better known what to do.

/// @brief Describes the kind of a Laye source text token.
/// @ref ly_token
typedef enum ly_token_kind {
    /// @brief Represents the end of a list of tokens, the end of a source file.
    /// @details This is the default kind for a token when zero initialized.
    LY_TK_EOF = 0,

    /// @brief The single tilde character '~'.
    /// @details Used as the prefix operator 'bitwise complement' and the infix 'bitwise xor'.
    LY_TK_TILDE = '~',

    /// @brief The single bang/exclamation point character '!'.
    /// @details Not used for prefix negation. Instead, used as a suffix to assert that an expression of a nilable type cannot be nil.
    LY_TK_BANG = '!',

    /// @brief The single percent character '%'.
    /// @details Used as the infix operator 'numeric remainder', the one which allows negative result values.
    LY_TK_PERCENT = '%',

    /// @brief The single ampersand character '&'.
    /// @details Used as the prefix operator 'lvalue address' and the infix operators 'bitwise and' and 'boolean and'.
    LY_TK_AMPERSAND = '&',

    /// @brief The single star character '*'.
    /// @details Used as the prefix operator 'address dereference' and the infix operator 'numeric multiply'.
    LY_TK_STAR = '*',

    /// @brief The single open parenthesis character '('.
    /// @details Must always be matched with a following paired close parenthesis character ')'.
    /// Used to delimit various syntax constructs.
    LY_TK_OPEN_PAREN = '(',

    /// @brief The single close parenthesis character ')'.
    /// @details Must always be matched with a paired preceding open parenthesis character '('.
    /// Used to delimit various syntax constructs.
    LY_TK_CLOSE_PAREN = ')',

    /// @brief The single minus character '-'.
    /// @details Used as the prefix operator 'numeric negate' and the infix operator 'numeric subtract'.
    LY_TK_MINUS = '-',

    /// @brief The single equal character '='.
    /// @details Used in initializations and assignments.
    LY_TK_EQUAL = '=',

    /// @brief The single plus character '+'.
    /// @details Used as the prefix operator 'numeric identity' and the infix operator 'numeric add'.
    LY_TK_PLUS = '+',

    /// @brief The single open square bracket character '['.
    /// @details Must always be matched with a following paired close square bracket character ']'.
    /// Used to delimit various syntax constructs.
    LY_TK_OPEN_SQUARE = '[',

    /// @brief The single close square bracket character ']'.
    /// @details Must always be matched with a preceding paired open square bracket character '['.
    /// Used to delimit various syntax constructs.
    LY_TK_CLOSE_SQUARE = ']',

    /// @brief The single open curly bracket character '{'.
    /// @details Must always be matched with a following paired close curly bracket character '}'.
    /// Used to delimit various syntax constructs.
    LY_TK_OPEN_CURLY = '{',

    /// @brief The single close curly bracket character '}'.
    /// @details Must always be matched with a preceding paired open curly bracket character '{'.
    /// Used to delimit various syntax constructs.
    LY_TK_CLOSE_CURLY = '}',

    /// @brief The single pipe character '|'.
    /// @details Used as the infix operators 'bitwise or' and 'boolean or'.
    LY_TK_PIPE = '|',

    /// @brief The single semi-colon character ';'.
    /// @details Used as a delimiter at the end of some statements, those which are not instead terminated by closing curly brackets due to a contained compound syntax.
    LY_TK_SEMI_COLON = ';',

    /// @brief The single colon character ':'.
    /// @details Used after an identifier to define named labels within function bodies.
    LY_TK_COLON = ':',

    /// @brief The single comma character ','.
    /// @details Used to delimit expressions or other syntax constructs in contexts where potentially many of them are expected.
    LY_TK_COMMA = ',',

    /// @brief The single less-than character '<'.
    /// @details Used as the binary operator 'numeric less than' and as the open bracket character for template declarations and instantiations.
    /// As this character is only treated as an open angle bracket sometimes, it is not required to be paired with a following close angle bracket '>'.
    LY_TK_LESS = '<',

    /// @brief The single greater-than character '>'.
    /// @details Used as the binary operator 'numeric greater than' and as the close bracket character for template declarations and instantiations.
    /// As this character is only treated as a close angle bracket sometimes, it is not required to be paired with a preceding open angle bracket '<'.
    LY_TK_GREATER = '>',

    /// @brief The single dot character '.'.
    /// @details Used to perform name lookup into an expression of a structured type, usually a field within a struct, and to denote a field designator in constructor initializer syntax.
    LY_TK_DOT = '.',

    /// @brief The single forward slash character '/'.
    /// @details Used as the infix operator 'numeric divide'.
    LY_TK_SLASH = '/',

    /// @brief The single question mark character '?'.
    /// @details Used to denote that a type may also be nil.
    LY_TK_QUESTION = '?',

    /// @brief A token which was determined to be missing from the source text, and was inserted by the compiler in an attempt to aid with parser error recovery.
    /// @details This is the first multi-byte token kind.
    LY_TK_MISSING = 256,

    /// @brief An identifier, a 'symbol' used to name declarations in source text.
    LY_TK_IDENTIFIER,

    /// @brief The 'global' keyword.
    /// @details Used as the optional first part of a 'path' syntax, a series of identifiers resolving names through entities with namespaces.
    /// When used, explicitly specifiles that the rest of the path is to be resolved from the 'global' scope, bypassing any locally-declared names.
    LY_TK_GLOBAL,

    /// @brief A token representing a literal integer value.
    LY_TK_LITERAL_INTEGER,

    /// @brief A token representing a literal floating-point real value.
    LY_TK_LITERAL_FLOAT,

    /// @brief A token representing a literal string value.
    LY_TK_LITERAL_STRING,

    /// @brief A token representing a literal 'rune' value, a single Unicode codepoint.
    LY_TK_LITERAL_RUNE,

    /// @brief The compound tilde-equal token '~='.
    /// Used as the compound-assignment operator for the infix operator 'bitwise xor'.
    LY_TK_TILDE_EQUAL,

    /// @brief The compound bang-equal token '!='.
    /// @details Used as the comparison operator 'not equal to'.
    LY_TK_BANG_EQUAL,

    /// @brief The compound percent-equal token '%='.
    /// @details Used as the compound-assignment operator for the infix operator 'numeric remainder'.
    LY_TK_PERCENT_EQUAL,

    /// @brief The compound ampersand-equal token '&='.
    /// @details Used as the compound-assignment operator for the infix operators 'bitwise and' and 'boolean and'.
    LY_TK_AMPERSAND_EQUAL,

    /// @brief The compound star-equal token '*='.
    /// @details Used as the compound-assignment operator for the infix operator 'numeric multiply'.
    LY_TK_STAR_EQUAL,

    /// @brief The compound minus-equal token '-='.
    /// @details Used as the compound-assignment operator for the infix operator 'numeric subtract'.
    LY_TK_MINUS_EQUAL,

    /// @brief The double-minus token '--'.
    /// @details Used as an lvalue suffix to form a decrement statement.
    /// This form is not an expression, as in C, but may be parsed as one to aid in error handling and reporting to the user.
    /// Likewise, a form as an expression prefix operator may also be parsed for the same reasons.
    LY_TK_MINUS_MINUS,

    /// @brief The double-equal token '=='.
    /// @details Used as the general-purpose equality comparison infix operator.
    LY_TK_EQUAL_EQUAL,

    /// @brief The equal-greater token '=>'.
    /// @details Used as a delimiter between a function prototype and a single-expression body, an implicit return body.
    LY_TK_EQUAL_GREATER,

    /// @brief The compound plus-equal token '+='.
    /// @details Used as the compound-assignment operator for the infix operator 'numeric add'.
    LY_TK_PLUS_EQUAL,

    /// @brief The double-plus token '++'.
    /// @details Used as an lvalue suffix to form an increment statement.
    /// This form is not an expression, as in C, but may be parsed as one to aid in error handling and reporting to the user.
    /// Likewise, a form as an expression prefix operator may also be parsed for the same reasons.
    LY_TK_PLUS_PLUS,

    /// @brief The compound pipe-equal token '|='.
    /// @details Used as the compound-assignment operator for the infix operators 'bitwise or' and 'boolean or'.
    LY_TK_PIPE_EQUAL,

    /// @brief The colon-colon token '::'.
    /// @details Used as the namespace resolution operator, placed between two identifiers (and in some cases, the 'global' and 'operator' keyword tokens).
    LY_TK_COLON_COLON,

    /// @brief The less-equal token '<='.
    /// @details Used as the infix operator 'numeric less than or equal to'.
    LY_TK_LESS_EQUAL,

    /// @brief The less-equal-greater token '<=>'.
    /// @details Used as the infix operator 'numeric three-way compare'.
    /// The three-way comparison is an ordering operation, yielding a negative value if `left < right`, zero if `left == right`, or a positive value if `left > right`.
    /// Implementing this operator allows the compiler to rewrite the four other comparison operators in terms of this one, reducing boilerplate work expected of the programmer.
    LY_TK_LESS_EQUAL_GREATER,

    /// @brief The less-less token '<<'.
    /// @details Used as the infix operator 'arithmetic shift left'.
    LY_TK_LESS_LESS,

    /// @brief The compound less-less-equal token '<<='.
    /// @details Used as the compound-assignment operator for the infix operator 'arithmetic shift left'.
    LY_TK_LESS_LESS_EQUAL,

    /// @brief The greater-equal token '>='.
    /// @details Used as the infix operator 'numeric greater than or equal to'.
    LY_TK_GREATER_EQUAL,

    /// @brief The greater-greater token '>>'.
    /// @details Used as the infix operator 'arithmetic shift right'.
    LY_TK_GREATER_GREATER,

    /// @brief The compound greater-greater-equal token '>>='.
    /// @details Used as the compound-assignment operator for the infix operator 'arithmetic shift right'.
    LY_TK_GREATER_GREATER_EQUAL,

    /// @brief The compound slash-equal token '/='.
    /// @details Used as the compound-assignment operator for the infix operator 'numeric divide'.
    LY_TK_SLASH_EQUAL,

    /// @brief The question-question token '??'.
    /// @details Used as the infix operator 'nil coalesce'.
    /// This operator is short-circuiting; iff the left value is nil, then the right expression is evaluated and returned in its place.
    LY_TK_QUESTION_QUESTION,

    /// @brief The compound question-question-equal token '??='.
    /// @details Used as the compound-assignment operator for the infix operator 'nil coalesce'.
    LY_TK_QUESTION_QUESTION_EQUAL,

    /// @brief The keyword token 'var'.
    /// @details Used as a replacement for the type in a variable declaration (a 'binding') to infer its type from the right-hand initialier expression, or as a replacement for the type in a compound initializer expression (a 'constructor') to infer its type from the left-hand assignment target.
    LY_TK_VAR,

    /// @brief The keyword token 'void'.
    /// @details The spelling of the pseudo-type 'void', indicating a function does not return a value.
    /// The 'void' type is invalid anywhere other than as a function return type.
    LY_TK_VOID,

    /// @brief The keyword token 'noreturn'.
    /// @details The spelling of the pseudo-type 'noreturn', indicating a function does not ever return; it will either continue forever, or terminate the program.
    /// The 'noreturn' type is invalid anywhere other than as a function return type.
    /// As we cannot solve the halting problem, this is only a valid return type for a function where the inability to return is trivially knowable, or is otherwise asserted by the programmer through syntax such as the 'unreachable' statement or through calls to other 'noreturn' functions.
    /// Function declarations may use this as their return type without needing these guarantees, as the function body cannot be known.
    LY_TK_NORETURN,

    /// @brief The keyword token 'bool'.
    /// @details The spelling of the type 'bool', the default boolean type.
    /// This type has an unspecified, but well defined size on the target. This is usually one byte, a 'bool8'.
    LY_TK_BOOL,

    /// @brief The family of keyword tokens 'boolN', where 'N' is a number represented by a sequence of one or more digits.
    /// @details The spelling a boolean type with the specified bit width.
    LY_TK_BOOL_SIZED,

    /// @brief The keyword token 'int'.
    /// @details The spelling of the type 'int', the default signed integer type.
    /// This type has an unspecified, but well defined size on the target. For example, on a 64-bit target this is usually eight bytes, an 'int64'.
    LY_TK_INT,

    /// @brief The family of keyword tokens 'intN', where 'N' is a number represented by a sequence of one or more digits.
    /// @details The spelling a signed integer type with the specified bit width.
    LY_TK_INT_SIZED,

    /// @brief The keyword token 'uint'.
    /// @details The spelling of the type 'uint', the default unsigned integer type.
    /// This type has an unspecified, but well defined size on the target. For example, on a 64-bit target this is usually eight bytes, a 'uint64'.
    LY_TK_UINT,

    /// @brief The family of keyword tokens 'uintN', where 'N' is a number represented by a sequence of one or more digits.
    /// @details The spelling an unsigned integer type with the specified bit width.
    LY_TK_UINT_SIZED,

    /// @brief The family of keyword tokens 'floatN', where 'N' is a number represented by a sequence of one or more digits.
    /// @details The spelling a floating-point real number type with the specified bit width.
    /// This type differs from the sized boolean and integer types, as only a small number of values for N are supported, where an IEEE float of that size is supported on the hardware or a software implementation exists.
    LY_TK_FLOAT_SIZED,

    /// @brief The keyword token '__builtin_ffi_bool'.
    /// @details The spelling of the type '__builtin_ffi_bool', the boolean type analogus to the platform's boolean type in C.
    LY_TK_BUILTIN_FFI_BOOL,

    /// @brief The keyword token '__builtin_ffi_char'.
    /// @details The spelling of the type '__builtin_ffi_char', the integer type analogus to the platform's 'char' type in C.
    LY_TK_BUILTIN_FFI_CHAR,

    /// @brief The keyword token '__builtin_ffi_schar'.
    /// @details The spelling of the type '__builtin_ffi_schar', the integer type analogus to the platform's 'signed char' type in C.
    LY_TK_BUILTIN_FFI_SCHAR,

    /// @brief The keyword token '__builtin_ffi_uchar'.
    /// @details The spelling of the type '__builtin_ffi_uchar', the integer type analogus to the platform's 'unsigned char' type in C.
    LY_TK_BUILTIN_FFI_UCHAR,

    /// @brief The keyword token 'short'.
    /// @details The spelling of the type 'short', the integer type analogus to the platform's 'short' type in C.
    LY_TK_BUILTIN_FFI_SHORT,

    /// @brief The keyword token 'ushort'.
    /// @details The spelling of the type 'ushort', the integer type analogus to the platform's 'unsigned short' type in C.
    LY_TK_BUILTIN_FFI_USHORT,

    /// @brief The keyword token '__builtin_ffi_int'.
    /// @details The spelling of the type '__builtin_ffi_int', the integer type analogus to the platform's 'int' type in C.
    LY_TK_BUILTIN_FFI_INT,

    /// @brief The keyword token '__builtin_ffi_uint'.
    /// @details The spelling of the type '__builtin_ffi_uint', the integer type analogus to the platform's 'unsigned int' type in C.
    LY_TK_BUILTIN_FFI_UINT,

    /// @brief The keyword token '__builtin_ffi_long'.
    /// @details The spelling of the type '__builtin_ffi_long', the integer type analogus to the platform's 'long' type in C.
    LY_TK_BUILTIN_FFI_LONG,

    /// @brief The keyword token '__builtin_ffi_ulong'.
    /// @details The spelling of the type '__builtin_ffi_ulong', the integer type analogus to the platform's 'unsigned long' type in C.
    LY_TK_BUILTIN_FFI_ULONG,

    /// @brief The keyword token '__builtin_ffi_longlong'.
    /// @details The spelling of the type '__builtin_ffi_longlong', the integer type analogus to the platform's 'long long' type in C.
    LY_TK_BUILTIN_FFI_LONGLONG,

    /// @brief The keyword token '__builtin_ffi_ulonglong'.
    /// @details The spelling of the type '__builtin_ffi_ulonglong', the integer type analogus to the platform's 'unsigned long long' type in C.
    LY_TK_BUILTIN_FFI_ULONGLONG,

    /// @brief The keyword token '__builtin_ffi_float'.
    /// @details The spelling of the type '__builtin_ffi_float', the integer type analogus to the platform's 'float' type in C.
    LY_TK_BUILTIN_FFI_FLOAT,

    /// @brief The keyword token '__builtin_ffi_double'.
    /// @details The spelling of the type '__builtin_ffi_double', the integer type analogus to the platform's 'double' type in C.
    LY_TK_BUILTIN_FFI_DOUBLE,

    /// @brief The keyword token '__builtin_ffi_long_double'.
    /// @details The spelling of the type '__builtin_ffi_long_double', the integer type analogus to the platform's 'long double' type in C.
    LY_TK_BUILTIN_FFI_LONG_DOUBLE,

    LY_TK_TRUE,
    LY_TK_FALSE,
    LY_TK_NIL,

    LY_TK_IF,
    LY_TK_ELSE,
    LY_TK_FOR,
    LY_TK_WHILE,
    LY_TK_DO,
    LY_TK_SWITCH,
    LY_TK_CASE,
    LY_TK_DEFAULT,
    LY_TK_RETURN,
    LY_TK_BREAK,
    LY_TK_CONTINUE,
    LY_TK_FALLTHROUGH,
    LY_TK_YIELD,
    LY_TK_UNREACHABLE,

    LY_TK_DEFER,
    LY_TK_DISCARD,
    LY_TK_GOTO,
    LY_TK_XYZZY,
    LY_TK_ASSERT,
    LY_TK_TRY,
    LY_TK_CATCH,

    LY_TK_STRUCT,
    LY_TK_VARIANT,
    LY_TK_ENUM,
    LY_TK_TEMPLATE,
    LY_TK_MODULE,
    LY_TK_ALIAS,
    LY_TK_DELEGATE,
    LY_TK_REGISTER,
    LY_TK_TEST,
    LY_TK_IMPORT,
    LY_TK_EXPORT,
    LY_TK_FROM,
    LY_TK_AS,
    LY_TK_STATIC,
    LY_TK_STRICT,
    LY_TK_OPERATOR,

    LY_TK_MUT,
    LY_TK_NEW,
    LY_TK_DELETE,
    LY_TK_CAST,
    LY_TK_EVAL,
    LY_TK_IS,

    LY_TK_SIZEOF,
    LY_TK_ALIGNOF,
    LY_TK_OFFSETOF,
    LY_TK_COUNTOF,
    LY_TK_RANKOF,
    LY_TK_TYPEOF,

    LY_TK_NOT,
    LY_TK_AND,
    LY_TK_OR,
    LY_TK_XOR,

    LY_TK_VARARGS,
    LY_TK_CONST,
    LY_TK_FOREIGN,
    LY_TK_INLINE,
    LY_TK_CALLCONV,
    LY_TK_PURE,
    LY_TK_DISCARDABLE,
} ly_token_kind;

/// @brief Returns the name of the enum constant associated with this Laye token kind.
CLAYE_API const char* ly_token_kind_name_get(ly_token_kind kind);

typedef struct ly_source {
    const char* text;
    int64_t length;
} ly_source;

typedef struct ly_location {
    ly_source* source;
    int64_t offset : 48;
    int64_t length : 16;
};

/// @brief
typedef struct ly_token {
    /// @brief The beginning of this token's text, its "lexeme", in the Laye source.
    /// @details This is a pointer within the loaded source text of the file, wherever that may have come from.
    /// This means that the lifetime of source text is expected to be as long as the lifetime of tokens.
    /// Usually, we should not need source information (or tokens) after a full semantic analysis pass has completed, since by the time we're generating code for the program all user errors should be gone (and we should be past any place an LSP should care about).
    /// With that said, I have found it useful to emit diagnostics with source location information even past semantic analysis, since that can help pinpoint what context triggers an ICE diagnostic.
    /// Maybe source text, and by extension tokens and their lexemes, should live for the entire duration of the program.
    const char* lexeme_begin;
    /// @brief The length of this token's text, its "lexeme".
    /// @details Hypothetically, this length *could* be specified as an int64_t, but we may already see issues with tooling if a single token is 2GB long.
    /// Technically my only "worry" is calling `printf("%.*s", ...)` on lexeme data, since the count of that format needs to evaluate to an `int`.
    int lexeme_length;
    /// @brief The distinct kind of this token.
    /// @details The kind of a token determines what (sometimes broad) purpose it serves to the syntactic and semantic meaning of the source text.
    /// Some kinds, such as specific operators and keywords, are very consistent visually while others, like identifiers or literals, are more of a class of similar-looking inputs.
    ly_token_kind kind;
} ly_token;

typedef struct ly_syntax {
    int dummy;
} ly_syntax;

typedef struct ly_ast_header {
    int dummy;
} ly_ast_header;

typedef struct ly_ast {
    int dummy;
} ly_ast;

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

#endif // LAYE_H_
