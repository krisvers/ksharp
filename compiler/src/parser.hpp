#ifndef KRISVERS_KSHARP_COMPILER_PARSER_HPP
#define KRISVERS_KSHARP_COMPILER_PARSER_HPP

#include <string>
#include <functional>

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
	RETURN,

	TYPEDEF,
	STRUCTDEF,
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

struct ASKeyword {
	std::string name;
};

class Parser {
public:
	void freeNode(ASNode* node);
	std::ostream& printNode(std::ostream& os, ASNode const* node, unsigned int depth);

	int parse(AST& ast, MetaInfo& metaInfo, const char* source);

private:
	std::map<std::string, ASKeyword> keywords;
};

}

#endif
