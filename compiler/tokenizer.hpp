#ifndef KRISVERS_KSHARP_COMPILER_TOKENIZER_HPP
#define KRISVERS_KSHARP_COMPILER_TOKENIZER_HPP

#include <string>

#include "general.hpp"

namespace ksharp::compiler::tokenizer {

enum class TokenType {
	NONE,
	FUNC_DECLARATION,
	DECLARATION,
	FUNC_DEFINITION,
	ASSIGMENT,
	IDENTIFIER,
	TYPE,
	SCOPE,
	RETURN,
	LITERAL,
};

struct Token {
	TokenType type;
	std::string value;
	const char* valuePtr;
	bool hasValue;
};

class Tokenizer {
public:
	static bool isTokenFinal(TokenType type);

	int tokenize(const char* source, Token& token, MetaInfo& metaInfo);
};

} // namespace ksharp::compiler::tokenizer

#endif
