#include "parser.hpp"

#include <iostream>
#include <map>

using namespace ksharp::compiler;

const char* test_string = R"(
var: u32 = 2;
main: (argc: u32, argv: string) u32;
)";

std::map<std::string, const char*> typeMap = {
	{ "u64", "uint64_t" },
	{ "s64", "int64_t" },
	{ "u32", "uint32_t" },
	{ "s32", "int32_t" },
	{ "u16", "uint16_t" },
	{ "s16", "int16_t" },
	{ "u8", "uint8_t" },
	{ "s8", "int8_t" },
	{ "f64", "double" },
	{ "f32", "float" },
	{ "bool", "bool" },
	{ "string", "char*" },
	{ "", "void" },
};

bool codeGenForC(std::ostream& ostream, parser::ASNode* node) {
	if (node == nullptr) {
		return false;
	}

	switch (node->type) {
		case parser::ASNodeType::DECLARATION: {
			codeGenForC(ostream, node->child->sibling);
			ostream << " ";
			codeGenForC(ostream, node->child);
			ostream << " = ";
			codeGenForC(ostream, node->child->sibling->sibling);
			ostream << ";\n";
			break;
		}
		case parser::ASNodeType::IDENTIFIER: {
			ostream << "ksharp_" << node->value;
			break;
		}
		case parser::ASNodeType::TYPE: {
			ostream << typeMap[node->value];
			break;
		}
		case parser::ASNodeType::FUNCTION_DEFINITION: {
			codeGenForC(ostream, node->child);
			ostream << ": (";
			parser::ASNode* arg = node->child->sibling->sibling;
			while (arg != nullptr) {
				codeGenForC(ostream, arg);
				arg = arg->sibling;
				if (arg == nullptr) {
					break;
				}

				ostream << ", ";
			}
			ostream << ") ";
			codeGenForC(ostream, arg->sibling);
			break;
		}
		case parser::ASNodeType::FUNCTION_DECLARATION: {
			codeGenForC(ostream, node->child);
			ostream << ": (";
			parser::ASNode* arg = node->child->sibling;
			while (arg != nullptr) {
				if (arg->type != parser::ASNodeType::FUNCTION_ARGUMENT) {
					break;
				}

				codeGenForC(ostream, arg);
				arg = arg->sibling;
				if (arg == nullptr) {
					break;
				}

				if (arg->type != parser::ASNodeType::FUNCTION_ARGUMENT) {
					break;
				}

				ostream << ", ";
			}
			ostream << ") ";
			codeGenForC(ostream, arg->sibling);
			ostream << ";\n";
			break;
		}
		case parser::ASNodeType::RETURN_TYPE: {
			codeGenForC(ostream, node->child);
			break;
		}
		case parser::ASNodeType::FUNCTION_ARGUMENT: {
			codeGenForC(ostream, node->child);
			ostream << ": ";
			codeGenForC(ostream, node->child->sibling);
			break;
		}
		case parser::ASNodeType::SCOPE: {
			ostream << "{\n";
			break;
		}
		case parser::ASNodeType::SCOPE_END: {
			ostream << "}\n";
			break;
		}
		case parser::ASNodeType::LITERAL_INTEGER: {
			ostream << node->value;
			break;
		}
		case parser::ASNodeType::LITERAL_FLOAT: {
			ostream << node->value;
			break;
		}
		case parser::ASNodeType::LITERAL_STRING: {
			ostream << node->value;
			break;
		}
		case parser::ASNodeType::LITERAL_BOOL: {
			ostream << node->value;
			break;
		}
		case parser::ASNodeType::KEYWORD: {
			ostream << node->value;
			break;
		}
		case parser::ASNodeType::RETURN: {
			ostream << "return ";
			codeGenForC(ostream, node->child);
			ostream << ";\n";
			break;
		}
	}

	return true;
}

int main(int argc, char** argv) {
	parser::Parser parser;
	parser::AST ast;

	parser.parse(ast, test_string);
	parser.printNode(std::cout, ast.root, 0);

	std::cout << "#include <stdint.h>\n";

	/* c code gen */
	parser::ASNode* node = ast.root;
	while (node != nullptr) {
		codeGenForC(std::cout, node);
		node = node->sibling;
	}

	parser.freeNode(ast.root);

	return 0;
}
