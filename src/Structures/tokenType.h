#ifndef TOKENTYPE_H_INCLUDED
#define TOKENTYPE_H_INCLUDED
//I thought I should put some documentation in
enum Type
{
IF,
IFHEADER,
IMPORTN,
RETURN,
ARGLIST,
FUNCTIONEND,
FUNCTION,
FUNCTIONHEAD,
CMD,
VARDEC,
ROOT,
FUNCNAME,
CMDSEQ,
VARSET
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
    ENDKEYWORD,
    /*
    A end keyword is unique,because it erases the TERM's around it and is considered part of a segment on the previous line.
    The keyword is used to close a loop,function, or class.
    Can you finish the rest of these?
    */
    LOOPTERM,
    VAR,
    OPAREN,
    CPAREN,
    COMMA,
    COLON,
    EQUALS,
    TERM,
    IFKEYWORD,
    QUOTE,
    TOKENEND
};
#endif // TOKENTYPE_H_INCLUDED
