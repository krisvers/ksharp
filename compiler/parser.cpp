#include "parser.hpp"
#include "general.hpp"
#include "tokenizer.hpp"

#include <iostream>s

using namespace ksharp::compiler::parser;

static ksharp::compiler::tokenizer::Token* requestToken(std::vector<ksharp::compiler::tokenizer::Token>& tokens, unsigned int index) {
	if (index < tokens.size()) {
		return &tokens[index];
	}

	return nullptr;
}

#define req(i) requestToken(tokens, i)

void Parser::freeNode(ASNode* node) {
	if (node == nullptr) {
		return;
	}

	if (node->sibling != nullptr) {
		freeNode(node->sibling);
	}

	if (node->child != nullptr) {
		freeNode(node->child);
	}

	delete node;
}

std::ostream& Parser::printNode(std::ostream& os, ASNode* node, unsigned int depth) {
	for (int i = 0; i < depth; ++i) {
		os << "  ";
	}

	switch (node->type) {
		case ASNodeType::DECLARATION:
			os << "Declaration: \"" << node->child->value << "\"" << std::endl;
			break;
		case ASNodeType::IDENTIFIER:
			os << "Identifier: \"" << node->value << "\"" << std::endl;
			break;
		case ASNodeType::TYPE:
			os << "Type: \"" << node->value << "\"" << std::endl;
			break;
		case ASNodeType::FUNCTION_DECLARATION:
			os << "Function Declaration: \"" << node->value << "\"" << std::endl;
			break;
		case ASNodeType::FUNCTION_ARGUMENT:
			os << "Function Argument: \"" << node->value << "\"" << std::endl;
			break;
		case ASNodeType::RETURN_TYPE:
			os << "Return Type: \"" << node->value << "\"" << std::endl;
			break;
		default:
			break;
	}

	if (node->child != nullptr) {
		printNode(os, node->child, depth + 1);
	}

	if (node->sibling != nullptr) {
		printNode(os, node->sibling, depth);
	}

	return os;
}

int Parser::parse(AST& ast, const char* source) {
	MetaInfo metaInfo;
	metaInfo.line = 1;
	metaInfo.column = 0;
	metaInfo.index = 0;
	metaInfo.length = strlen(source);

	tokenizer::Tokenizer tokenizer;
	std::vector<tokenizer::Token> tokens;
	tokenizer::Token* token;

	while (tokenizer.tokenize(source, tokens, metaInfo) == 0) {
		token = &tokens[tokens.size() - 1];
		if (token->type == tokenizer::TokenType::NONE) {
			break;
		}
	}

	ASNode** node = &ast.root;
	ASNode* prevNode = nullptr;
	ASNode* parent = nullptr;

	tokenizer::Token* nextToken;
	for (unsigned int i = 0; i < tokens.size(); ++i) {
		token = &tokens[i];
		nextToken = req(i + 1);

		if (token->type == tokenizer::TokenType::IDENTIFIER) {
			if (nextToken != nullptr) {
				if (nextToken->type == tokenizer::TokenType::TYPE_SEPARATOR) {
					nextToken = req(i + 2);
					if (nextToken == nullptr) {
						std::cout << "Error: expected type token" << std::endl;
						return 1;
					}

					if (nextToken->type != tokenizer::TokenType::TYPE && nextToken->type != tokenizer::TokenType::FUNCTION_TYPE) {
						std::cout << "Error: expected type token" << std::endl;
						return 1;
					}

					if (nextToken->type == tokenizer::TokenType::FUNCTION_TYPE) {
						*node = new ASNode();
						(*node)->type = ASNodeType::FUNCTION_DECLARATION;
						(*node)->sibling = nullptr;
						(*node)->child = nullptr;
						(*node)->parent = parent;

						(*node)->child = new ASNode();
						(*node)->child->type = ASNodeType::IDENTIFIER;
						(*node)->child->value = token->value;
						(*node)->child->sibling = nullptr;
						(*node)->child->child = nullptr;
						(*node)->child->parent = *node;

						std::string functionType = nextToken->value;
						(*node)->child->sibling = new ASNode();
						(*node)->child->sibling->type = ASNodeType::RETURN_TYPE;
						(*node)->child->sibling->value = functionType;
						(*node)->child->sibling->value.replace((*node)->child->sibling->value.find("("), (*node)->child->sibling->value.find(")") + 1, "");
						(*node)->child->sibling->child = nullptr;
						(*node)->child->sibling->sibling = nullptr;
						(*node)->child->sibling->parent = (*node)->child->sibling;

						std::string arguments = functionType;
						arguments.replace(0, arguments.find("(") + 1, "");
						arguments.replace(arguments.find(")"), arguments.length(), "");

						std::string args = arguments;
						if (args.length() > 0) {
							auto next = args.find(",");
							if (next == std::string::npos) {
								next = args.length();
							}

							std::string arg = args.substr(0, next);
							args.replace(0, next + 1, "");

							(*node)->child->sibling->sibling = new ASNode();
							(*node)->child->sibling->sibling->type = ASNodeType::FUNCTION_ARGUMENT;
							(*node)->child->sibling->sibling->value = arg;
							(*node)->child->sibling->sibling->child = nullptr;
							(*node)->child->sibling->sibling->sibling = nullptr;
							(*node)->child->sibling->sibling->parent = *node;

							next = arg.find(":");
							(*node)->child->sibling->sibling->child = new ASNode();
							(*node)->child->sibling->sibling->child->type = ASNodeType::IDENTIFIER;
							if (next != std::string::npos) {
								(*node)->child->sibling->sibling->child->value = arg.substr(0, next);
							}

							(*node)->child->sibling->sibling->child->sibling = nullptr;
							(*node)->child->sibling->sibling->child->child = nullptr;
							(*node)->child->sibling->sibling->child->parent = (*node)->child->sibling;

							(*node)->child->sibling->sibling->child->sibling = new ASNode();
							(*node)->child->sibling->sibling->child->sibling->type = ASNodeType::TYPE;
							(*node)->child->sibling->sibling->child->sibling->value = arg.substr(arg.find(":") + 1);
							(*node)->child->sibling->sibling->child->sibling->sibling = nullptr;
							(*node)->child->sibling->sibling->child->sibling->child = nullptr;
							(*node)->child->sibling->sibling->child->sibling->parent = (*node)->child->sibling->sibling;
						}
					} else {
						*node = new ASNode();
						(*node)->type = ASNodeType::DECLARATION;
						(*node)->sibling = nullptr;
						(*node)->child = new ASNode();
						(*node)->parent = parent;

						(*node)->child->type = ASNodeType::IDENTIFIER;
						(*node)->child->value = token->value;
						(*node)->child->sibling = nullptr;
						(*node)->child->child = nullptr;
						(*node)->child->parent = *node;

						(*node)->child->sibling = new ASNode();
						(*node)->child->sibling->type = ASNodeType::TYPE;
						(*node)->child->sibling->value = nextToken->value;
						(*node)->child->sibling->sibling = nullptr;
						(*node)->child->sibling->child = nullptr;
						(*node)->child->sibling->parent = *node;
					}

					nextToken = req(i + 3);
					if (nextToken == nullptr) {
						std::cout << "Error: no token; expected semicolon token" << std::endl;
						return 1;
					}

					if (nextToken->type != tokenizer::TokenType::SEMICOLON) {
						std::cout << "Error: expected semicolon token" << std::endl;
						return 1;
					}

					if (prevNode != nullptr) {
						prevNode->sibling = *node;
					}

					prevNode = *node;
					node = &(*node)->sibling;

					i += 3;
				}
			}
		}
	}

	return 0;
}
