#include "parser.hpp"
#include "general.hpp"
#include "tokenizer.hpp"

#include <iostream>
#include <vector>

using namespace ksharp::compiler::parser;

int Parser::parse(AST& ast, const char* source) {
	MetaInfo metaInfo;
	metaInfo.line = 1;
	metaInfo.column = 1;
	metaInfo.index = 0;
	metaInfo.length = strlen(source);

	tokenizer::Tokenizer tokenizer;
	tokenizer::Token token;

	ASNode** node = &ast.root;
	ASNode* prevNode = NULL;

	while (tokenizer.tokenize(source, token, metaInfo) == 0) {
		if (token.type == tokenizer::TokenType::NONE) {
			break;
		}

		*node = new ASNode();
		(*node)->sibling = nullptr;
		(*node)->child = nullptr;
		(*node)->parent = nullptr;
		(*node)->token = token;

		if (prevNode != NULL) {
			if (tokenizer::Tokenizer::isTokenFinal(prevNode->token.type)) {

			}
		}

		prevNode = *node;
	}

	return 0;
}
