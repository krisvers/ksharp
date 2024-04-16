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
	SEMICOLON,
	KEYWORD,
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

	int tokenize(const char* source, std::vector<Token>& tokens, MetaInfo& metaInfo);

private:
	inline static std::map<std::string, Keyword> keywords;
};

} // namespace ksharp::compiler::tokenizer

#endif
