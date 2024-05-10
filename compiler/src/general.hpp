#ifndef KRISVERS_KSHARP_COMPILER_GENERAL_HPP
#define KRISVERS_KSHARP_COMPILER_GENERAL_HPP

namespace ksharp::compiler {

struct MetaInfo {
	/* constant */
	const char* originalSource;
	unsigned int originalLength;
	unsigned int length;

	/* based on original source (generated every call to updateMetaInfo) */
	unsigned int line;
	unsigned int column;
	unsigned int originalIndex;

	/* based on linted source (used by updateMetaInfo) */
	unsigned int index;
};

void updateMetaInfo(MetaInfo& metaInfo, const char* source, unsigned int index);

} // namespace ksharp::compiler

#endif
