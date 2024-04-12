#ifndef KRISVERS_KSHARP_COMPILER_GENERAL_HPP
#define KRISVERS_KSHARP_COMPILER_GENERAL_HPP

namespace ksharp::compiler {

struct MetaInfo {
	unsigned int line;
	unsigned int column;
	unsigned int index;
	unsigned int length;
};

} // namespace ksharp::compiler

#endif
