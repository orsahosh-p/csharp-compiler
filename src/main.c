#include <stdio.h>
#include "lexer.h"

int main(void)
{
    const char *source =
    "int age = 21;\n"
    "/* This comment is not closed";

    Lexer lexer;
    lexer_init(&lexer, source);

    for (;;) {
    Token token = lexer_next_token(&lexer);

    if (token.type == TOKEN_INVALID) {
        fprintf(
            stderr,
            "Program.cs:%zu:%zu: error: %s\n",
            token.line,
            token.column,
            token.error_message
                ? token.error_message
                : "Invalid token"
        );

        return 1;
    }

    if (token.type == TOKEN_EOF) {
        break;
    }

    printf(
        "Type: %-2d | Text: %.*s\n",
        token.type,
        (int)token.length,
        token.start
    );
}

return 0;
}