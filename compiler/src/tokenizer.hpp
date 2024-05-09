#ifndef KRISVERS_KSHARP_COMPILER_TOKENIZER_HPP
#define KRISVERS_KSHARP_COMPILER_TOKENIZER_HPP

#include <string>
#include <map>
#include <vector>

#include "general.hpp"

namespace ksharp::compiler::tokenizer {

enum class TokenType {
	NONE,
	IDENTIFIER,
	LITERAL,
	TYPE,
	FUNCTION_TYPE,
	SCOPE,
	SCOPE_END,
	TYPE_SEPARATOR,
	ASSIGNMENT,
	SEMICOLON,
	KEYWORD,
	TYPEDEF,
};

struct Token {
	TokenType type;
	std::string value;
	const char* valuePtr;
	bool hasValue;
};

class Tokenizer {
public:
	static void tokenToString(Token& token, std::string& str);

	int tokenize(const char* source, std::vector<Token>& tokens, MetaInfo& metaInfo);
};

} // namespace ksharp::compiler::tokenizer

#endif
