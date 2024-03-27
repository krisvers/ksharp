#include "parser.hpp"

using namespace ksharp::compiler::parser;

static char requestChar(const char* source, unsigned int index, unsigned int len) {
	if (index < len) {
		return source[index];
	}

	return '\0';
}

#define req(i) requestChar(source, i, sourceLength)

int Parser::parse(AST& ast, const char* source) {
	ast.root = nullptr;
	unsigned int sourceLength = strlen(source);
	
	Token currentToken = Token();

	for (unsigned int i = 0; i < sourceLength; ++i) {
		if (currentToken.type == TokenType::NONE) {
			if (source[i] == ' ' || source[i] == '\n' || source[i] == '\t') {
				continue;
			}

			if (source[i] == '@') {
				currentToken.type = TokenType::ANNOTATION;
				currentToken.valuePtr = &source[i];
			} else if (source[i] == '{') {
				currentToken.type = TokenType::COMPOUND;
				currentToken.valuePtr = &source[i];
			} else if (source[i] == '(') {
				currentToken.type = TokenType::SCOPE;
				currentToken.valuePtr = &source[i];
			} else if (source[i] == ';') {
				currentToken.type = TokenType::NONE;
				currentToken.valuePtr = &source[i];
			} else if (source[i] == '=') {
				currentToken
			}
		}
	}

	return 0;
}