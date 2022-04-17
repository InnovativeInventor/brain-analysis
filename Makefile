CXX=g++
LD=g++

W_FLAGS=-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wswitch-default -Wundef -Werror -Wno-unused -Wconversion

CXX_FLAGS=-g -std=c++20 -Ofast -flto -fno-signed-zeros -fno-trapping-math -frename-registers -funroll-loops -D_GLIBCXX_PARALLEL -march=native -Iinclude $(W_FLAGS)

L_FLAGS=-L/usr/lib/x86_64-linux-gnu -flto

main: build/main.o build/graph.o
	$(LD) -o $@ $^ $(L_FLAGS)
build/main.o: src/main.cc
	$(CXX) $(CXX_FLAGS) -c -o $@ $<
build/graph.o: src/graph.cc include/graph.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

exe: main
	./main

clean:
	rm -rf build/*.o
	rm -rf main

.PHONY: clean, exe
