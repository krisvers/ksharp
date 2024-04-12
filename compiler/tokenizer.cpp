#include "tokenizer.hpp"

#include <iostream>

namespace ksharp::compiler::tokenizer {

bool Tokenizer::isTokenFinal(TokenType type) {
	switch (type) {
		case TokenType::RETURN:
		case TokenType::LITERAL:
			return true;
		default: return false;
	}
}

static char requestChar(const char* source, unsigned int index, unsigned int len) {
	if (index < len) {
		return source[index];
	}

	return '\0';
}

#define req(i) requestChar(source, i, metaInfo.length)

int Tokenizer::tokenize(const char* source, Token& token, MetaInfo& metaInfo) {
	token.type = TokenType::IDENTIFIER;
	token.value = "";
	token.valuePtr = nullptr;
	token.hasValue = false;

	unsigned int i = metaInfo.index;
	for (; i < metaInfo.length; ++i) {
		++metaInfo.column;
		if (source[i] == '\n') {
			metaInfo.column = 1;
			++metaInfo.line;
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
					++metaInfo.column;
					c = req(i + 1);
					if (c == '\0') {
						std::cout << "Error: Unexpected end of file at (" << metaInfo.line << ":" << metaInfo.column << ")" << std::endl;
						metaInfo.index = i;
						return 1;
					}
				}
			} else if (c == '*') {
				while (source[i] != '*' || c != '/') {
					++i;
					++metaInfo.column;
					c = req(i + 1);
					if (source[i] == '\n') {
						metaInfo.column = 1;
						++metaInfo.line;
						continue;
					}
				}
			}
		}
	}

	metaInfo.index = i;
	return 1;
}

} // namespace ksharp::compiler::tokenizer
