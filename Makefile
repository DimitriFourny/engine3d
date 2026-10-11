# macOS setup: brew install glfw glm; make; ./build/game
# Resolve Homebrew paths for both Apple Silicon and Intel installations.
glfw_prefix = $(shell brew --prefix glfw)
glm_prefix = $(shell brew --prefix glm)
frameworks = -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon
compile_flags = -std=c++11 -stdlib=libc++ -I$(glfw_prefix)/include -I$(glm_prefix)/include
linker_flags = -L$(glfw_prefix)/lib -lglfw
cpp_files = $(wildcard ./*.cpp)
object_files = $(patsubst %.cpp,build/%.o,$(cpp_files))

all: build/game

build/%.o: %.cpp
	mkdir -p $(@D)
	clang++ -c $(compile_flags) $< -o $@

build/game: $(object_files)
	clang++ $(object_files) $(linker_flags) $(frameworks) -o $@

clean:
	-rm build/*.o
	-rm build/game