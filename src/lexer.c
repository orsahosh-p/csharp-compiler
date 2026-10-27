#include "lexer.h"

#include <ctype.h>
#include <string.h>

static int is_at_end(const Lexer *lexer)
{
    return *lexer->current == '\0';
}

static char advance(Lexer *lexer)
{
    char c = *lexer->current++;

    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }

    return c;
}

static int match(Lexer *lexer, char expected)
{
    if (is_at_end(lexer) ||

        *lexer->current != expected) {
        return 0;
    }
    advance(lexer);
    return 1;
}

static Token make_token(
    const Lexer *lexer,
    TokenType type
)
{
    return (Token) {
        .type = type,
        .start = lexer->start,
        .length = (size_t)(
            lexer->current - lexer->start
        ),
        .line = lexer->token_line,
        .column = lexer->token_column,
        .error_message = NULL
    };
}

static Token error_token(const Lexer *lexer, const char *message)
{
    Token token = make_token(lexer, TOKEN_INVALID);
    token.error_message = message;
    return token;
}


void lexer_init(Lexer *lexer, const char *source)
{
    lexer->source = source;
    lexer->start = source;
    lexer->current = source;

    lexer->line = 1;
    lexer->column = 1;

    lexer->token_line = 1;
    lexer->token_column = 1;
}

static TokenType identifier_type(const Lexer *lexer)
{
    size_t length =
        (size_t)(lexer->current - lexer->start);

    struct Keyword {
        const char *text;
        TokenType type;
    };

    static const struct Keyword keywords[] = {
        {"int", TOKEN_INT},
        {"bool", TOKEN_BOOL},
        {"string", TOKEN_STRING},
        {"true", TOKEN_TRUE},
        {"false", TOKEN_FALSE},
        {"if", TOKEN_IF},
        {"else", TOKEN_ELSE},
        {"while", TOKEN_WHILE},
        {"return", TOKEN_RETURN}
    };

    size_t count = sizeof(keywords) / sizeof(keywords[0]);

    for (size_t i = 0; i < count; i++) {
        if (strlen(keywords[i].text) == length &&
            strncmp(lexer->start, keywords[i].text, length) == 0) {
            return keywords[i].type;
        }
    }

    return TOKEN_IDENTIFIER;
}

static Token scan_string(Lexer *lexer)
{
    while (!is_at_end(lexer)) {
        char c = *lexer->current;

        if (c == '"') {
            advance(lexer);
            return make_token(lexer, TOKEN_STRING_LITERAL);
        }

        if (c == '\n' || c == '\r') {
            return error_token(lexer, "Unterminated string literal");
        }

        if (c == '\\') {
            advance(lexer);

            if (is_at_end(lexer)) {
                return error_token(lexer, "Unterminated string literal");
            }

            if (*lexer->current == '\n' ||
                *lexer->current == '\r') {
                return error_token(lexer, "Unterminated string literal");
            }

            advance(lexer);
            continue;
        }

        advance(lexer);
    }

    return error_token(lexer, "Unterminated string literal");
}

static int skip_ignored(Lexer *lexer)
{
    for (;;) {
        char c = *lexer->current;

        switch (c) {
            case ' ':
            case '\t':
            case '\r':
                advance(lexer);
                break;

            case '\n':
                advance(lexer);
                break;

            case '/':
                if (lexer->current[1] == '/') {
                    while (!is_at_end(lexer) &&
                           *lexer->current != '\n') {
                        advance(lexer);
                    }
                }
                else if (lexer->current[1] == '*') {
                    
                    lexer->start = lexer->current;
                    lexer->token_line = lexer->line;
                    lexer->token_column = lexer->column;
                    
                    advance(lexer);
                    advance(lexer);

                    int closed = 0;

                    while (!is_at_end(lexer)) {
                        if (*lexer->current == '*' &&
                            lexer->current[1] == '/') {

                            advance(lexer);
                            advance(lexer);

                            closed = 1;
                            break;
                        }

                        advance(lexer);
                    }

                    if (!closed) {
                        return 0;
                    }
                }
                else {
                    return 1;
                }
                break;

            default:
                return 1;
        }
    }
}

Token lexer_next_token(Lexer *lexer)
{

    lexer->start = lexer->current;
    lexer->token_line = lexer->line;
    lexer->token_column = lexer->column;

    if (!skip_ignored(lexer)) {
    return error_token(
        lexer,
        "Unterminated block comment"
    );
    }

    lexer->start = lexer->current;
    lexer->token_line = lexer->line;
    lexer->token_column = lexer->column;

    lexer->start = lexer->current;

    if (is_at_end(lexer)) {
        return make_token(lexer, TOKEN_EOF);
    }

    char c = advance(lexer);

    if (isalpha((unsigned char)c) || c == '_') {
        while (isalnum((unsigned char)*lexer->current) ||
               *lexer->current == '_') {
            advance(lexer);
        }

        return make_token(lexer, identifier_type(lexer));
    }

    if (isdigit((unsigned char)c)) {
        while (isdigit((unsigned char)*lexer->current)) {
            advance(lexer);
        }

        return make_token(lexer, TOKEN_NUMBER);
    }

    switch (c) {

        case '"': return scan_string(lexer);

        case '+': return make_token(lexer, TOKEN_PLUS);
        case '-': return make_token(lexer, TOKEN_MINUS);
        case '*': return make_token(lexer, TOKEN_STAR);
        case '/': return make_token(lexer, TOKEN_SLASH);

        case '=':
            return make_token(
                lexer,
                match(lexer, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL
            );

        case '!':
            return make_token(
                lexer,
                match(lexer, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG
            );

        case '<':
            return make_token(
                lexer,
                match(lexer, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS
            );

        case '>':
            return make_token(
                lexer,
                match(lexer, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER
            );

        case ';': return make_token(lexer, TOKEN_SEMICOLON);
        case '(': return make_token(lexer, TOKEN_LEFT_PAREN);
        case ')': return make_token(lexer, TOKEN_RIGHT_PAREN);
        case '{': return make_token(lexer, TOKEN_LEFT_BRACE);
        case '}': return make_token(lexer, TOKEN_RIGHT_BRACE);

        default:
            return error_token(lexer, "Unexpected character");
    }
}