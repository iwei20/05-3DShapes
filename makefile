sourcedir = src
objdir = obj
targetdir = bin
CC = g++
FLAGS = -g -rdynamic -Wall

list = $(subst $(sourcedir),$(objdir),$(subst .cpp,.o,$(wildcard $(sourcedir)/*.cpp)))

h ?= 500
w ?= 500
script ?= script

run: build
	@echo "Running executable..."
	@./$(targetdir)/main.out $(script) $(w) $(h)

build: $(targetdir)/main.out
	@echo "Build finished"

$(targetdir)/main.out: $(list) | $(targetdir)
	@echo "Linking to executable..."
	@$(CC) $(FLAGS) -o $@ $(list)
	@echo "Executable $@ linked"

$(list): $(objdir)/%.o : $(sourcedir)/%.cpp | $(objdir)
	@echo "Compiling $(<F)"
	@$(CC) $(FLAGS) -c $< -o $@

$(objdir):
	@echo "Making object directory..."
	@mkdir -p $@

$(targetdir):
	@echo "Making binary directory..."
	@mkdir -p $@

clean:
	@echo "Removing object directory..."
	-@rm -r $(objdir)

remove: clean
	@echo "Removing target directory..."
	-@rm -r $(targetdir)
	@echo "Removing picture..."
	-@rm *.ppm
	
.PHONY: run build clean remove