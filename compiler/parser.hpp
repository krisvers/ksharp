#ifndef KRISVERS_KSHARP_COMPILER_PARSER_HPP
#define KRISVERS_KSHARP_COMPILER_PARSER_HPP

#include <string>

namespace ksharp::compiler::parser {

enum class TokenType {
	NONE,
	/* meta types */
	COMPOUND,
	ASSIGMENT,
	COMPARISON,
	IF,
	ELSE,
	WHILE,
	FOR,

	IDENTIFIER,
	OPERATOR,
	LITERAL,
	DECLARATION,
	FUNC_DECLARATION,
	TYPE,

	FUNC_CALL,
	PARAMETER,
	RETURN,
};

struct Token {
	TokenType type;
	std::string value;
	const char* valuePtr;
};

struct ASTNodeGeneric {
	Token token;
	struct ASTNodeGeneric* next;
};

struct ASTNodeBinary {
	Token token;
	struct ASTNodeGeneric* left;
	struct ASTNodeGeneric* right;
};

struct ASTNodeMulti {
	Token token;
	unsigned int length;
	struct ASTNodeGeneric** children;
};

struct AST {
	ASTNodeGeneric* root;
};

class Parser {
public:
	int parse(AST& ast, const char* source);
};

}

#endif