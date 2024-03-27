#ifndef KRISVERS_KSHARP_COMPILER_PARSER_HPP
#define KRISVERS_KSHARP_COMPILER_PARSER_HPP

#include <string>

namespace ksharp::compiler::parser {

enum class TokenType {
	NONE,
	ANNOTATION,
	COMPOUND,
	FUNC_DECLARATION,
	DECLARATION,
	FUNC_DEFINITION,
	ASSIGMENT,
	IDENTIFIER,
	TYPE,
	SCOPE,
	RETURN,
	LITERAL,
};

struct Token {
	TokenType type;
	std::string value;
	const char* valuePtr;
};

struct ASTNodeBinary {
	Token token;
	struct ASTNodeGeneric* left;
	struct ASTNodeGeneric* right;
};

struct AST {
	ASTNodeBinary* root;
};

class Parser {
public:
	int parse(AST& ast, const char* source);
};

}

#endif
