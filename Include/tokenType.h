#ifndef TOKENTYPE_H_INCLUDED
#define TOKENTYPE_H_INCLUDED
//I thought I should put some documentation in
enum Type
{
IF, //0
FOR,
FORHEADER,
IFHEADER,
IMPORTN,
RETURNCMD, //5
ARGLIST,
FUNCTIONEND,
FUNCTION,
FUNCTIONHEAD,
CMD, //10
VARDEC,
ROOT,
FUNCNAME,
CMDSEQ,
VARSET, //15
VARINIT,
EXPR,
ENDPOINT,
WHILE
};

enum TokenType
{
    TEXT,
    /*
    A text token is a temporary replacement for a keyword, variable name, or other things.
    Text tokens are replaced in the lexer with their more specific token that describes them. Variable names are automatically assigned to
    text tokens that do not have another definition.
    */
    NUMBER,
    /*
    Self-explanatory. A number token that represent a string of digits.
    */
    ELSEKEYWORD, /* < -- This is not yet implemented */
    TAB,
    /*
    The keyword is used to close a loop,function, or class.
    Can you finish the rest of these?
    */
    RETURNKEYWORD,
    /*
    The return keyword is the start of a command that prompts the program to exit once it reaches that scope and line.
    */
    VAR,
    FORKEYWORD,
    OPAREN,
    CPAREN,
    COMMA,
    COLON,
    TERM,
    IFKEYWORD,
    QUOTE,
    AT,
    ADD,
    SUBTRACT,
    DIVIDE,
    MULTIPLY,
    MODULO,
    POINTER,
    EQUALS,
    NOTEQUALS,
    ASYNCKEYWORD,
    CONSTKEYWORD,
    PTRKEYWORD,
    REFKEYWORD,
    SAFEKEYWORD,
    WHILEKEYWORD,
    FOREACHKEYWORD,
    SWITCHKEYWORD,
    SEMICOLON,
    CASEKEYWORD,
    ASSIGNMENT,
    FUNCTIONKEYWORD,
    INCLUDEKEYWORD,
    IMPORTKEYWORD
};
#endif // TOKENTYPE_H_INCLUDED
