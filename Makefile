frameworks = -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon
compile_flags = -std=c++11 -stdlib=libc++
linker_flags = -lglfw3
cpp_files = $(wildcard ./*.cpp)
object_files = $(patsubst %.cpp,build/%.o,$(cpp_files))

all: build/game

build/%.o: %.cpp
	clang++ -c $(compile_flags) $< -o $@

build/game: $(object_files)
	clang++ $(linker_flags) $(frameworks) $(object_files) -o $@

clean:
	-rm build/*.o
	-rm build/game