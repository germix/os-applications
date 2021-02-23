#include "Lexer.h"

Lexer::Lexer(const QString& s) : pos(0), string(s)
{
}
Lexer::~Lexer()
{
}
Token Lexer::nextToken()
{
	QChar c;

	while(1)
	{
		c = string.at(pos);
		if(c == 0)
		{
			return Token(TOK_EOF);
		}
		else if(c.isSpace())
		{
			pos++;
		}
		else if(c.isLetter())
		{
			QString s;

			s += c;
			pos++;
			while(string.at(pos).isLetterOrNumber() || string.at(pos) == '_')
			{
				s += string.at(pos++);
			}
			return Token(TOK_IDENT, s);
		}
		else if(c.isDigit())
		{
			QString s;
			int msize;

			s += c;
			pos++;
			while(string.at(pos).isDigit())
			{
				s += string.at(pos++);
			}
			msize = s.toInt();
			if(string.at(pos).toUpper() == 'K')
			{
				s += string.at(pos++);
				msize *= 1024;
			}
			else if(string.at(pos).toUpper() == 'M')
			{
				s += string.at(pos++);
				msize *= 1024*1024;
			}
			return Token(TOK_MEMORY, s, msize);
		}
		else
		{
			pos++;
			return Token(c.toLatin1(), c);
		}
	}
}
