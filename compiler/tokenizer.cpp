#include "tokenizer.hpp"

#include <iostream>
#include <map>

namespace ksharp::compiler::tokenizer {

void Tokenizer::tokenToString(Token& token, std::string& str) {
	switch (token.type) {
		case TokenType::NONE:
			str = "NONE";
			break;
		case TokenType::IDENTIFIER:
			str = "IDENTIFIER";
			break;
		case TokenType::LITERAL:
			str = "LITERAL";
			break;
		case TokenType::TYPE:
			str = "TYPE";
			break;
		case TokenType::FUNCTION_TYPE:
			str = "FUNCTION_TYPE";
			break;
		case TokenType::SCOPE:
			str = "SCOPE";
			break;
		case TokenType::SCOPE_END:
			str = "SCOPE_END";
			break;
		case TokenType::TYPE_SEPARATOR:
			str = "TYPE_SEPARATOR";
			break;
		case TokenType::ASSIGNMENT:
			str = "ASSIGNMENT";
			break;
		case TokenType::SEMICOLON:
			str = "SEMICOLON";
			break;
		case TokenType::KEYWORD:
			str = "KEYWORD";
			break;
		default:
			str = "UNKNOWN";
			break;
	}
}

static char requestChar(const char* source, unsigned int index, unsigned int len) {
	if (index < len) {
		return source[index];
	}

	return '\0';
}

#define req(i) requestChar(source, i, metaInfo.length)

static bool isWhitespace(char ch) {
	return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}

static bool isAlpha(char ch) {
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

static bool isDigit(char ch) {
	return ch >= '0' && ch <= '9';
}

static bool isAlphaNumeric(char ch) {
	return isAlpha(ch) || isDigit(ch);
}

static bool isIdentifier(char ch) {
	return isAlphaNumeric(ch) || ch == '_';
}

static bool isStartIdentifier(char ch) {
	return isAlpha(ch) || ch == '_';
}

static bool isLiteral(char ch) {
	return isDigit(ch) || ch == '.';
}

static bool isTypeSeparator(char ch) {
	return ch == ':';
}

static bool isTypeChar(char ch) {
	return isAlphaNumeric(ch);
}

static bool isStartTypeChar(char ch) {
	return isAlpha(ch);
}

int Tokenizer::tokenize(const char* source, std::vector<Token>& tokens, MetaInfo& metaInfo) {
	Token token;
	token.type = TokenType::NONE;
	token.value = "";
	token.valuePtr = nullptr;
	token.hasValue = false;

	unsigned int i = metaInfo.index;
	for (; i < metaInfo.length; ++i) {
		++metaInfo.column;
		char ch = source[i];
		if (ch == '\n') {
			metaInfo.column = 0;
			++metaInfo.line;
			continue;
		}

		if (isWhitespace(ch)) {
			continue;
		}

		if (ch == '/') {
			char c = req(i + 1);
			if (c == '*') {
				while (ch != '*' || c != '/') {
					++i;
					++metaInfo.column;
					c = req(i + 1);
					ch = req(i);
					if (ch == '\n') {
						metaInfo.column = 0;
						++metaInfo.line;
						continue;
					}

					if (c == '\0' || ch == '\0') {
						std::cout << "Error: Unexpected end of file at (" << metaInfo.line << ":" << metaInfo.column << ")" << std::endl;
						metaInfo.index = i;
						return 1;
					}
				}
			}
		} else if (isStartIdentifier(ch)) {
			token.type = TokenType::IDENTIFIER;
			token.valuePtr = &source[i];

			unsigned int j = i;
			while (isIdentifier(ch)) {
				++j;
				ch = req(j);
				++metaInfo.column;
				if (ch == '\n') {
					++metaInfo.line;
					metaInfo.column = 0;
				}

				if (ch == '\0') {
					std::cout << "Error: Unexpected end of file at (" << metaInfo.line << ":" << metaInfo.column << ")" << std::endl;
					metaInfo.index = j;
					return 1;
				}
			}

			token.value = std::string(&source[i], j - i);
			token.hasValue = true;
			if (ch == '!') {
				token.type = TokenType::KEYWORD;
				++j;
			}

			i = j;
			break;
		} else if (isLiteral(ch)) {
			token.type = TokenType::LITERAL;
			token.valuePtr = &source[i];

			unsigned int j = i;
			while (isLiteral(ch)) {
				++j;
				ch = req(j);
				++metaInfo.column;
				if (ch == '\n') {
					++metaInfo.line;
					metaInfo.column = 0;
				}
				
				if (ch == '\0') {
					std::cout << "Error: Unexpected end of file at (" << metaInfo.line << ":" << metaInfo.column << ")" << std::endl;
					metaInfo.index = j - 1;
					return 1;
				}
			}

			token.value = std::string(&source[i], j - i);
			token.hasValue = true;
			i = j;
			break;
		} else if (ch == '"') {
			token.type = TokenType::LITERAL;
			token.valuePtr = &source[i];

			unsigned int j = i;
			++j;
			ch = req(j);
			++metaInfo.column;
			if (ch == '\n') {
				++metaInfo.line;
				metaInfo.column = 0;
			}

			while (ch != '"') {
				++j;
				ch = req(j);
				++metaInfo.column;
				if (ch == '\\') {
					++j;
					char c = req(j);
					if (c == '"') {
						++j;
					}
				}

				if (ch == '\n') {
					++metaInfo.line;
					metaInfo.column = 0;
				}

				if (ch == '\0') {
					std::cout << "Error: Unexpected end of file at (" << metaInfo.line << ":" << metaInfo.column << ")" << std::endl;
					metaInfo.index = j;
					return 1;
				}
			}

			token.value = std::string(&source[i], j - i + 1);
			token.hasValue = true;
			i = j + 1;
			break;
		} else if (isTypeSeparator(ch)) {
			token.type = TokenType::TYPE_SEPARATOR;
			token.value = ":";
			token.valuePtr = &source[i];
			token.hasValue = true;

			tokens.push_back(token);

			unsigned int j = i;
			++j;
			ch = req(j);
			while (isWhitespace(ch)) {
				++j;
				ch = req(j);
				++metaInfo.column;
				if (ch == '\n') {
					++metaInfo.line;
					metaInfo.column = 0;
				}

				if (ch == '\0') {
					std::cout << "Error: Unexpected end of file at (" << metaInfo.line << ":" << metaInfo.column << ")" << std::endl;
					metaInfo.index = j;
					return 1;
				}
			}

			i = j;
			if (ch == '(') {
				token.type = TokenType::FUNCTION_TYPE;

				while (ch != ')') {
					++j;
					ch = req(j);
					++metaInfo.column;
					if (ch == '\n') {
						++metaInfo.line;
						metaInfo.column = 0;
					}

					if (ch == '\0') {
						std::cout << "Error: Unexpected end of file at (" << metaInfo.line << ":" << metaInfo.column << ")" << std::endl;
						metaInfo.index = j;
						return 1;
					}
				}

				while (isWhitespace(ch)) {
					++j;
					ch = req(j);
					++metaInfo.column;
					if (ch == '\n') {
						++metaInfo.line;
						metaInfo.column = 0;
					}

					if (ch == '\0') {
						std::cout << "Error: Unexpected end of file at (" << metaInfo.line << ":" << metaInfo.column << ")" << std::endl;
						metaInfo.index = j;
						return 1;
					}
				}

				while (ch != ';' && ch != '=') {
					++j;
					ch = req(j);
					++metaInfo.column;
					if (ch == '\n') {
						++metaInfo.line;
						metaInfo.column = 0;
					}

					if (ch == '\0') {
						std::cout << "Error: Unexpected end of file at (" << metaInfo.line << ":" << metaInfo.column << ")" << std::endl;
						metaInfo.index = j;
						return 1;
					}
				}

				token.valuePtr = &source[i];
				token.value = std::string(&source[i], j - i);

				auto it = token.value.find(" ");
				while (it != std::string::npos) {
					token.value.replace(it, 1, "");
					it = token.value.find(" ");
				}
				token.hasValue = true;

				tokens.push_back(token);
				if (ch == ';') {
					token.type = TokenType::SEMICOLON;
					token.value = ";";
					token.valuePtr = &source[j];
					token.hasValue = true;

					++j;
					++metaInfo.column;
				}

				i = j;
				break;
			} else {
				if (!isStartTypeChar(ch)) {
					std::cout << "Error: Unexpected character '" << ch << "' (0x" << std::hex << static_cast<unsigned int>(ch) << std::dec << ") at (" << metaInfo.line << ":" << metaInfo.column << ")\nExpected type name" << std::endl;
					metaInfo.index = i;
					return 1;
				}

				token.type = TokenType::TYPE;

				while (isTypeChar(ch)) {
					++j;
					ch = req(j);
					++metaInfo.column;
					if (ch == '\n') {
						++metaInfo.line;
						metaInfo.column = 0;
					}

					if (ch == '\0') {
						std::cout << "Error: Unexpected end of file at (" << metaInfo.line << ":" << metaInfo.column << ")" << std::endl;
						metaInfo.index = j;
						return 1;
					}
				}
			}

			token.valuePtr = &source[i];
			token.value = std::string(&source[i], j - i);
			token.hasValue = true;
			i = j;
			break;
		} else if (isLiteral(ch)) {
			token.type = TokenType::LITERAL;
			token.valuePtr = &source[i];

			unsigned int j = i;
			while (isLiteral(ch)) {
				++j;
				ch = req(j);
				++metaInfo.column;
				if (ch == '\n') {
					++metaInfo.line;
					metaInfo.column = 0;
				}

				if (ch == '\0') {
					std::cout << "Error: Unexpected end of file at (" << metaInfo.line << ":" << metaInfo.column << ")" << std::endl;
					metaInfo.index = j - 1;
					return 1;
				}
			}

			token.value = std::string(&source[i], j - i);
			token.hasValue = true;
			i = j;
			break;
		} else if (ch == '{') {
			token.type = TokenType::SCOPE;
			token.value = "{";
			token.valuePtr = &source[i];
			token.hasValue = true;

			++i;
			++metaInfo.column;
			break;
		} else if (ch == '}') {
			token.type = TokenType::SCOPE_END;
			token.value = "}";
			token.valuePtr = &source[i];
			token.hasValue = true;

			++i;
			++metaInfo.column;
			break;
		} else if (ch == ';') {
			token.type = TokenType::SEMICOLON;
			token.value = ";";
			token.valuePtr = &source[i];
			token.hasValue = true;

			++i;
			++metaInfo.column;
			break;
		} else if (ch == '=') {
			token.type = TokenType::ASSIGNMENT;
			token.value = "=";
			token.valuePtr = &source[i];
			token.hasValue = true;

			tokens.push_back(token);

			++metaInfo.column;
			unsigned int j = ++i;
			ch = req(j);
			while (isWhitespace(ch)) {
				++j;
				ch = req(j);
				++metaInfo.column;
				if (ch == '\n') {
					++metaInfo.line;
					metaInfo.column = 0;
				}

				if (ch == '\0') {
					std::cout << "Error: Unexpected end of file at (" << metaInfo.line << ":" << metaInfo.column << ")" << std::endl;
					metaInfo.index = j;
					return 1;
				}
			}

			i = j;
			if (ch == ';') {
				std::cout << "Error: Unexpected character '" << ch << "' (0x" << std::hex << static_cast<unsigned int>(ch) << std::dec << ") at (" << metaInfo.line << ":" << metaInfo.column << ")\nExpected literal" << std::endl;
				metaInfo.index = i;
				return 1;
			}

			if (isLiteral(ch)) {
				token.type = TokenType::LITERAL;
				token.valuePtr = &source[i];

				while (isLiteral(ch)) {
					++j;
					ch = req(j);
					++metaInfo.column;
					if (ch == '\n') {
						++metaInfo.line;
						metaInfo.column = 0;
					}

					if (ch == '\0') {
						std::cout << "Error: Unexpected end of file at (" << metaInfo.line << ":" << metaInfo.column << ")" << std::endl;
						metaInfo.index = j - 1;
						return 1;
					}
				}

				token.value = std::string(&source[i], j - i);
				token.hasValue = true;
				i = j;
				break;
			}

			break;
		} else {
			std::cout << "Error: Unexpected character '" << ch << "' (0x" << std::hex << static_cast<unsigned int>(ch) << std::dec << ") at (" << metaInfo.line << ":" << metaInfo.column << ")" << std::endl;
			metaInfo.index = i;
			return 1;
		}
	}

	metaInfo.index = i;
	if (token.type == TokenType::NONE) {
		return 1;
	}

	tokens.push_back(token);
	return 0;
}

} // namespace ksharp::compiler::tokenizer
