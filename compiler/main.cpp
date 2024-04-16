#include "parser.hpp"

#include <iostream>

using namespace ksharp::compiler;

const char* test_string = R"(
fibonacci: (n: u32) u32;
)";

int main(int argc, char** argv) {
	parser::Parser parser;
	parser::AST ast;
	if (parser.parse(ast, test_string) != 0) {
		return 1;
	}

	parser.printNode(std::cout, ast.root, 0);
	parser.freeNode(ast.root);

	return 0;
}
