CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-but-set-variable
CXXSRC = $(shell find . -type f -name "*.cpp")

OBJDIR = obj
BUILDDIR = build
BINARY = ksharpvm
CXXOBJ = $(subst -_=,$(OBJDIR)/,$(subst /,_,$(subst ./,-_=,$(subst .cpp,.o,$(CXXSRC)))))

.PHONY: build obj link clean build-run

build: obj link

obj: $(CXXOBJ)

link:
	$(CXX) $(shell find $(OBJDIR) -type f -name "*.o") -o $(BUILDDIR)/$(BINARY)

clean:
	rm -rf $(OBJDIR) $(BUILDDIR)
	mkdir -p $(OBJDIR) $(BUILDDIR)

build-run: build
	$(BUILDDIR)/$(BINARY)

debug-make-vars:
	@echo CXX: $(CXX)
	@echo CXXFLAGS: $(CXXFLAGS)
	@echo OBJDIR: $(OBJDIR)
	@echo BUILDDIR: $(BUILDDIR)
	@echo BINARY: $(BINARY)
	@echo CXXSRC: $(CXXSRC)
	@echo CXXOBJ: $(CXXOBJ)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
