#ifndef KRISVERS_KSHARP_COMPILER_PARSER_HPP
#define KRISVERS_KSHARP_COMPILER_PARSER_HPP

#include <string>

#include "general.hpp"
#include "tokenizer.hpp"

namespace ksharp::compiler::parser {

enum class ASNodeType {
	DECLARATION,
		IDENTIFIER,
		TYPE,

	FUNCTION_DEFINITION,
	FUNCTION_DECLARATION,
		FUNCTION_ARGUMENT,
		RETURN_TYPE,

	SCOPE,
	SCOPE_END,

	LITERAL_INTEGER,
	LITERAL_FLOAT,
	LITERAL_STRING,
	LITERAL_BOOL,

	KEYWORD,
};

struct ASNode {
	ASNodeType type;
	std::string value;
	struct ASNode* sibling;
	struct ASNode* child;
	struct ASNode* parent;
};

struct AST {
	ASNode* root;
};

class Parser {
public:
	void freeNode(ASNode* node);
	std::ostream& printNode(std::ostream& os, ASNode* node, unsigned int depth);

	int parse(AST& ast, const char* source);
};

}

#endif
