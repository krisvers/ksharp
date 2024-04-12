#ifndef KRISVERS_KSHARP_COMPILER_PARSER_HPP
#define KRISVERS_KSHARP_COMPILER_PARSER_HPP

#include <string>

#include "general.hpp"
#include "tokenizer.hpp"

namespace ksharp::compiler::parser {

struct ASNode {
	tokenizer::Token token;
	struct ASNode* sibling;
	struct ASNode* child;
	struct ASNode* parent;
};

struct AST {
	ASNode* root;
};

class Parser {
public:
	int parse(AST& ast, const char* source);
};

}

#endif
