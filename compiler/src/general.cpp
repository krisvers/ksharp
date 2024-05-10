#include "general.hpp"
#include <cstring>
#include <iostream>

namespace ksharp::compiler {

void updateMetaInfo(MetaInfo& metaInfo, const char* source, unsigned int index) {
	metaInfo.index = index;
	metaInfo.column = 0;
	metaInfo.line = 1;

	unsigned int lintedIndex = 0;
	for (metaInfo.originalIndex = 0; metaInfo.originalIndex < metaInfo.originalLength; ++metaInfo.originalIndex) {
		char ch = metaInfo.originalSource[metaInfo.originalIndex];
		if (ch == '\n') {
			++metaInfo.line;
			metaInfo.column = 0;
		} else if (ch == '\r') {
			continue;
		} else if (ch == ' ' || ch == '\t') {
			++metaInfo.column;
		} else {
			++metaInfo.column;
			++lintedIndex;
			
			if (lintedIndex > index) {
				break;
			}
		}
	}
}

} // namespace ksharp::compiler