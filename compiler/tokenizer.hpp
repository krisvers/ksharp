#ifndef KRISVERS_KSHARP_COMPILER_TOKENIZER_HPP
#define KRISVERS_KSHARP_COMPILER_TOKENIZER_HPP

#include <string>
#include <map>

#include "general.hpp"

namespace ksharp::compiler::tokenizer {

enum class TokenType {
	NONE,
	IDENTIFIER,
	LITERAL,
	TYPE,
	SCOPE,
	SCOPE_END,
	SEMICOLON,
	RETURN,
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
	static Keyword* getKeyword(const char* source, unsigned int index, MetaInfo& metaInfo);

	int tokenize(const char* source, Token& token, MetaInfo& metaInfo);

private:
	inline static std::map<std::string, Keyword> keywords;
};

} // namespace ksharp::compiler::tokenizer

#endif
