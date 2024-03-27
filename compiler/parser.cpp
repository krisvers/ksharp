#include "parser.hpp"
#include <iostream>

using namespace ksharp::compiler::parser;

static char requestChar(const char* source, unsigned int index, unsigned int len) {
	if (index < len) {
		return source[index];
	}

	return '\0';
}

#define req(i) requestChar(source, i, sourceLength)

int Parser::parse(AST& ast, const char* source) {
	ast.root = new ASTNodeBinary();
	unsigned int sourceLength = strlen(source);
	
	Token currentToken = Token();
	currentToken.type = TokenType::IDENTIFIER;
	currentToken.value = "";
	currentToken.valuePtr = nullptr;

	unsigned int column = 1;
	unsigned int line = 1;

	for (unsigned int i = 0; i < sourceLength; ++i) {
		++column;
		if (source[i] == '\n') {
			column = 1;
			++line;
			continue;
		}

		if (source[i] == ' ' || source[i] == '\t' || source[i] == '\r') {
			continue;
		}

		if (source[i] == '/') {
			char c = req(i + 1);
			if (c == '/') {
				while (c != '\n') {
					++i;
					++column;
					c = req(i + 1);
					if (c == '\0') {
						std::cout << "Error: Unexpected end of file at (" << line << ":" << column << ")" << std::endl;
						return 1;
					}
				}
				std::cout << "Line comment: (" << line << ":" << column << ")" << std::endl;
			} else if (c == '*') {
				while (source[i] != '*' || c != '/') {
					++i;
					++column;
					c = req(i + 1);
					if (source[i] == '\n') {
						column = 1;
						++line;
						continue;
					} else if (c == '\0') {
						std::cout << "Error: Unexpected end of file (" << line << ":" << column << ")" << std::endl;
						return 1;
					}
				}
				std::cout << "Section comment: (" << line << ":" << column << ")" << std::endl;
			}
		}
	}

	return 0;
}
