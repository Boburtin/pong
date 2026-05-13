CXX := g++
CXXFLAGS := -std=c++23 -Wall -Werror -Wextra -DUNICODE -D_UNICODE
LDFLAGS = -static -municode -mwindows
LDLIBS = -ld2d1 -ldwrite

SRCDIR := src
BUILDDIR := build
TARGET := bin/GGPong
SRCEXT := cpp

SRCS := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRCS:.$(SRCEXT)=.o))

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	@./$(TARGET)

clean:
	rm -rf $(BUILDDIR) $(TARGET) bin

.PHONY: all clean run