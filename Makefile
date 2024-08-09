# compiler and flags
CXX = g++
CXXFLAGS = -Wall -g -Iinclude -IC:/msys64/mingw64/include

# linker flags for raylib and its dependencies
LDFLAGS = -L"C:/msys64/mingw64/lib" -lraylib -lopengl32 -lgdi32 -lwinmm

# Define directories
SRCDIR = src
INCDIR = include
BUILDDIR = build
BINDIR = bin

# Define source files
SRCS = $(wildcard $(SRCDIR)/*.cpp) $(wildcard ./*.cpp)
# Define o files
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(filter-out ./main.cpp, $(SRCS))) $(patsubst ./%.cpp, $(BUILDDIR)/%.o, $(filter ./main.cpp, $(SRCS)))

# Define exe
EXEC = $(BINDIR)/main.exe

# Default rule
all: $(EXEC)

# Rule to link o files to create exe
$(EXEC): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile source files into o files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: ./%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(BUILDDIR) $(BINDIR)

.PHONY: all clean