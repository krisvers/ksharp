#include "parser.hpp"

#include <iostream>
#include <cstring>
#include <map>

using namespace ksharp::compiler;

const char* test_string = R"(
#test_type u32;
#test_struct {
	x: u32;
	y: u32;
}

string: string = "Hello, World!\n\n@#$%^&*()_+{}|:<>?~`";
var: test_type = 2;
struct: test_struct;
fib: (n: u32) u32 = {

}

main: () s32;
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

void codeGenIndent(std::ostream& ostream, unsigned int depth) {
	if (depth == 0) return;

	while (depth--) {
		ostream << "  ";
	}
}

bool codeGenForC(std::ostream& ostream, parser::ASNode* node, unsigned int depth, bool prefix) {
	if (node == nullptr) {
		return false;
	}

	switch (node->type) {
		case parser::ASNodeType::DECLARATION: {
			codeGenIndent(ostream, depth);
			codeGenForC(ostream, node->child->sibling, 0, prefix);
			ostream << " ";
			codeGenForC(ostream, node->child, 0, prefix);

			/* assignment */
			if (node->child->sibling->sibling != nullptr) {
				ostream << " = ";
				codeGenForC(ostream, node->child->sibling->sibling, 0, prefix);
			}
			ostream << ";\n";
			break;
		}
		case parser::ASNodeType::IDENTIFIER: {
			if (prefix) {
				ostream << "ksharp_" << node->value;
			} else {
				ostream << node->value;
			}
			break;
		}
		case parser::ASNodeType::RETURN_TYPE:
		case parser::ASNodeType::TYPE: {
			if (typeMap.find(node->value) == typeMap.end()) {
				ostream << "ksharp_" << node->value;
			} else {
				ostream << typeMap[node->value];
			}
			break;
		}
		case parser::ASNodeType::FUNCTION_DEFINITION: {
			codeGenIndent(ostream, depth);
			codeGenForC(ostream, node->child, 0, prefix);
			ostream << ": (";
			parser::ASNode* arg = node->child->sibling->sibling;
			while (arg != nullptr) {
				codeGenForC(ostream, arg, 0, prefix);
				arg = arg->sibling;
				if (arg == nullptr) {
					break;
				}

				ostream << ", ";
			}
			ostream << ") ";
			codeGenForC(ostream, arg->sibling, 0, prefix);
			break;
		}
		case parser::ASNodeType::FUNCTION_DECLARATION: {
			codeGenIndent(ostream, depth);
			codeGenForC(ostream, node->child->sibling, 0, prefix);
			ostream << " ";
			codeGenForC(ostream, node->child, 0, prefix);
			ostream << "(";

			parser::ASNode* arg = node->child->sibling->sibling;

			while (arg != nullptr) {
				if (arg->type != parser::ASNodeType::FUNCTION_ARGUMENT) {
					break;
				}

				codeGenForC(ostream, arg, 0, prefix);
				arg = arg->sibling;
				if (arg != nullptr) {
					ostream << ", ";
				}
			}
			ostream << ")";

			if (arg != nullptr) {
				parser::ASNode* code = arg;
				while (code != nullptr) {
					codeGenForC(ostream, code, depth + 1, prefix);
					code = code->sibling;
				}
			} else {
				ostream << ";";
			}

			ostream << "\n";
			break;
		}
		case parser::ASNodeType::FUNCTION_ARGUMENT: {
			codeGenForC(ostream, node->child->sibling, 0, prefix);
			ostream << " ";
			codeGenForC(ostream, node->child, 0, prefix);
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
			codeGenIndent(ostream, depth);
			ostream << "return ";
			codeGenForC(ostream, node->child, 0, prefix);
			ostream << ";\n";
			break;
		}
		case parser::ASNodeType::TYPEDEF: {
			codeGenIndent(ostream, depth);
			ostream << "typedef ";
			codeGenForC(ostream, node->child, 0, prefix);
			ostream << " ";
			codeGenForC(ostream, node->child->sibling, 0, prefix);
			ostream << ";\n";
			break;
		}
		case parser::ASNodeType::STRUCTDEF: {
			codeGenIndent(ostream, depth);
			ostream << "typedef struct ";
			codeGenForC(ostream, node->child, 0, prefix);
			ostream << " {\n";

			parser::ASNode* scopeNode = node->child->sibling;
			if (scopeNode == nullptr) {
				break;
			}

			parser::ASNode* memberNode = scopeNode->child;
			while (memberNode != nullptr && memberNode->type != parser::ASNodeType::SCOPE_END) {
				codeGenForC(ostream, memberNode, depth + 1, false);
				memberNode = memberNode->sibling;
			}

			codeGenIndent(ostream, depth);
			ostream << "};\n";
			break;
		}
		default:
			break;
	}

	return true;
}

int main(int argc, char** argv) {
	bool noPrint = false;
	if (argc > 1) {
		if (strcmp(argv[1], "-c") == 0) {
			noPrint = true;
		}
	}

	if (!noPrint) {
		std::cout << "Source:\n| -------- |" << test_string << "| -------- |\n\n";
	}

	parser::Parser parser;
	parser::AST ast;
	MetaInfo metaInfo;

	if (parser.parse(ast, metaInfo, test_string) != 0) {
		std::cout << "Error parsing source\n";
		return 1;
	}
	if (!noPrint) {
		std::cout << "\nParser AST:\n| -------- |\n";
		parser.printNode(std::cout, ast.root, 0);

		std::cout << "| -------- |\n\nC Code Gen:\n";
	}

	std::cout << "#include <stdint.h>\n";

	/* c code gen */
	parser::ASNode* node = ast.root;
	while (node != nullptr) {
		codeGenForC(std::cout, node, 0, true);
		node = node->sibling;
	}

	std::cout <<
R"(
int main(int argc, char** argv) {
	return ksharp_main();
}
)";

	parser.freeNode(ast.root);

	return 0;
}
