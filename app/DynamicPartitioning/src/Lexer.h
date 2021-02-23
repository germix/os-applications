#ifndef LEXER_H
#define LEXER_H
#include <QString>

enum
{
	TOK_EOF = -1,
	TOK_IDENT = 256,
	TOK_MEMORY,
	TOK_NUMERIC = TOK_MEMORY,
};
class Token
{
public:
	int tok;
	int size;
	QString lexeme;
public:
	Token() : tok(TOK_EOF)
	{
	}
	Token(const Token& t) : tok(t.tok), size(t.size), lexeme(t.lexeme)
	{
	}
	Token(int _tok, const QString _lexeme = "", int _size = 0) : tok(_tok), size(_size), lexeme(_lexeme)
	{
	}
	~Token(){}
public:
	operator int()
	{
		return tok;
	}
	Token& operator = (const Token& t)
	{
		tok = t.tok;
		size = t.size;
		lexeme = t.lexeme;
		return *this;
	}
};

class Lexer
{
	int pos;
	QString string;
public:
	Lexer(const QString& s);
	~Lexer();
public:
	Token nextToken();
};


#endif // LEXER_H
