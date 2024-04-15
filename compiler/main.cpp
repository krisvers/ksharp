#include "parser.hpp"

using namespace ksharp::compiler;

const char* test_string = R"(variable: u32;)";

int main(int argc, char** argv) {
	parser::Parser parser;
	parser::AST ast;
	if (parser.parse(ast, test_string) != 0) {
		return 1;
	}

	return 0;
}
