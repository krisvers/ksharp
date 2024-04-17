#include "parser.hpp"
#include "general.hpp"
#include "tokenizer.hpp"

#include <iostream>

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
	for (unsigned int i = 0; i < depth; ++i) {
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
		case ASNodeType::FUNCTION_DEFINITION:
			os << "Function Definition: \"" << node->value << "\"" << std::endl;
			break;
		case ASNodeType::SCOPE:
			os << "Scope: \"" << node->value << "\"" << std::endl;
			break;
		case ASNodeType::SCOPE_END:
			os << "Scope End: \"" << node->value << "\"" << std::endl;
			break;
		case ASNodeType::LITERAL_INTEGER:
			os << "Literal Integer: \"" << node->value << "\"" << std::endl;
			break;
		case ASNodeType::LITERAL_FLOAT:
			os << "Literal Float: \"" << node->value << "\"" << std::endl;
			break;
		case ASNodeType::LITERAL_STRING:
			os << "Literal String: \"" << node->value << "\"" << std::endl;
			break;
		case ASNodeType::LITERAL_BOOL:
			os << "Literal Bool: \"" << node->value << "\"" << std::endl;
			break;
		case ASNodeType::KEYWORD:
			os << "Keyword: \"" << node->value << "\"" << std::endl;
			break;
		case ASNodeType::RETURN:
			os << "Return: \"" << node->value << "\"" << std::endl;
			break;
		default:
			os << "Unknown: \"" << node->value << "\"" << std::endl;
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

	for (tokenizer::Token& token : tokens) {
		std::string str;
		tokenizer.tokenToString(token, str);
		std::cout << str << ": \"" << token.value << "\"" << std::endl;
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
						ASNode** argNode = &(*node)->child->sibling->sibling;
						while (args.length() > 0) {
							auto next = args.find(",");
							if (next == std::string::npos) {
								next = args.length();
							}

							std::string arg = args.substr(0, next);
							args.replace(0, next + 1, "");

							*argNode = new ASNode();
							(*argNode)->type = ASNodeType::FUNCTION_ARGUMENT;
							(*argNode)->value = arg;
							(*argNode)->child = nullptr;
							(*argNode)->sibling = nullptr;
							(*argNode)->parent = *node;

							next = arg.find(":");
							(*argNode)->child = new ASNode();
							(*argNode)->child->type = ASNodeType::IDENTIFIER;
							if (next != std::string::npos) {
								(*argNode)->child->value = arg.substr(0, next);
							}

							(*argNode)->child->sibling = nullptr;
							(*argNode)->child->child = nullptr;
							(*argNode)->child->parent = (*node)->child->sibling;

							(*argNode)->child->sibling = new ASNode();
							(*argNode)->child->sibling->type = ASNodeType::TYPE;
							(*argNode)->child->sibling->value = arg.substr(arg.find(":") + 1);
							(*argNode)->child->sibling->sibling = nullptr;
							(*argNode)->child->sibling->child = nullptr;
							(*argNode)->child->sibling->parent = (*node)->child->sibling->sibling;

							argNode = &(*argNode)->sibling;
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

					tokenizer::TokenType prevType = nextToken->type;
					nextToken = req(i + 3);
					if (nextToken == nullptr) {
						std::cout << "Error: no token found; expected semicolon or assigment token" << std::endl;
						return 1;
					}

					if (nextToken->type != tokenizer::TokenType::SEMICOLON && nextToken->type != tokenizer::TokenType::ASSIGNMENT) {
						std::string str;
						tokenizer.tokenToString(*nextToken, str);
						std::cout << "Error: expected semicolon or assignment token; found " << str << std::endl;
						return 1;
					}

					if (nextToken->type == tokenizer::TokenType::ASSIGNMENT) {
						if (prevType == tokenizer::TokenType::TYPE) {
							nextToken = req(i + 4);
							if (nextToken == nullptr) {
								std::cout << "Error: no token found; expected literal token" << std::endl;
								return 1;
							}

							(*node)->child->sibling->sibling = new ASNode();
							(*node)->child->sibling->sibling->value = nextToken->value;
							(*node)->child->sibling->sibling->sibling = nullptr;
							(*node)->child->sibling->sibling->child = nullptr;
							(*node)->child->sibling->sibling->parent = *node;

							if (nextToken->type == tokenizer::TokenType::IDENTIFIER) {
								(*node)->child->sibling->sibling->type = ASNodeType::IDENTIFIER;
							} else if (nextToken->type == tokenizer::TokenType::LITERAL) {
								if (nextToken->value.find("\"") != std::string::npos) {
									(*node)->child->sibling->sibling->type = ASNodeType::LITERAL_STRING;
								} else if (nextToken->value.find(".") != std::string::npos) {
									(*node)->child->sibling->sibling->type = ASNodeType::LITERAL_FLOAT;
								} else if (nextToken->value == "true" || nextToken->value == "false") {
									(*node)->child->sibling->sibling->type = ASNodeType::LITERAL_BOOL;
								} else {
									(*node)->child->sibling->sibling->type = ASNodeType::LITERAL_INTEGER;
								}
							}
							i += 2;
						} else if (prevType == tokenizer::TokenType::FUNCTION_TYPE) {
							
						}
					}

					if (prevNode != nullptr) {
						prevNode->sibling = *node;
					}

					prevNode = *node;
					node = &(*node)->sibling;
					i += 3;
				}
			}
		} else if (token->type == tokenizer::TokenType::KEYWORD) {
			*node = new ASNode();
			(*node)->type = ASNodeType::KEYWORD;
			(*node)->value = token->value;
			(*node)->sibling = nullptr;
			(*node)->child = nullptr;
			(*node)->parent = parent;

			if (token->value == "return") {
				nextToken = req(i + 1);
				if (nextToken == nullptr) {
					std::cout << "Error: no token found; expected literal token" << std::endl;
					return 1;
				}

				(*node)->type = ASNodeType::RETURN;
				(*node)->value = token->value;
				(*node)->sibling = nullptr;
				(*node)->parent = parent;
				(*node)->child = new ASNode();

				if (nextToken->type == tokenizer::TokenType::IDENTIFIER) {
					(*node)->child->type = ASNodeType::IDENTIFIER;
				} else if (nextToken->type == tokenizer::TokenType::LITERAL) {
					if (nextToken->value.find("\"") != std::string::npos) {
						(*node)->child->type = ASNodeType::LITERAL_STRING;
					} else if (nextToken->value.find(".") != std::string::npos) {
						(*node)->child->type = ASNodeType::LITERAL_FLOAT;
					} else if (nextToken->value == "true" || nextToken->value == "false") {
						(*node)->child->type = ASNodeType::LITERAL_BOOL;
					} else {
						(*node)->child->type = ASNodeType::LITERAL_INTEGER;
					}
				}

				(*node)->child->value = nextToken->value;
				(*node)->child->sibling = nullptr;
				(*node)->child->child = nullptr;
				(*node)->child->parent = *node;

				i += 1;
			}
		}
	}

	return 0;
}
