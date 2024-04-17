#include "parser.hpp"

#include <iostream>

using namespace ksharp::compiler;

const char* test_string = R"(
var: u32 = 2;
function: (n: u32, n2: s32) u32;
)";

int main(int argc, char** argv) {
	parser::Parser parser;
	parser::AST ast;

	parser.parse(ast, test_string);
	parser.printNode(std::cout, ast.root, 0);
	parser.freeNode(ast.root);

	return 0;
}
