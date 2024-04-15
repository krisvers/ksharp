#include "parser.hpp"
#include "general.hpp"
#include "tokenizer.hpp"

#include <iostream>
#include <vector>

using namespace ksharp::compiler::parser;

int Parser::parse(AST& ast, const char* source) {
	MetaInfo metaInfo;
	metaInfo.line = 1;
	metaInfo.column = 0;
	metaInfo.index = 0;
	metaInfo.length = strlen(source);

	tokenizer::Tokenizer tokenizer;
	tokenizer::Token token;

	ASNode** node = &ast.root;
	ASNode* prevNode = nullptr;

	while (tokenizer.tokenize(source, token, metaInfo) == 0) {
		if (token.type == tokenizer::TokenType::NONE) {
			break;
		}

		std::string str;
		tokenizer::Tokenizer::tokenToString(token, str);

		std::cout << "Token: " << str << " \"" << token.value << "\"" << std::endl;

		*node = new ASNode();
		(*node)->sibling = nullptr;
		(*node)->child = nullptr;
		(*node)->parent = prevNode;
		(*node)->token = token;

		prevNode = *node;
	}

	return 0;
}
