#ifndef KRISVERS_KSHARP_COMPILER_GENERAL_HPP
#define KRISVERS_KSHARP_COMPILER_GENERAL_HPP

namespace ksharp::compiler {

struct MetaInfo {
	unsigned int line;
	unsigned int column;
	unsigned int index;
	unsigned int length;
};

void updateMetaInfo(MetaInfo& metaInfo, const char* source, unsigned int index);

} // namespace ksharp::compiler

#endif
