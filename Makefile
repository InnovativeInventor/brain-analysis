CXX=g++-11
LD=g++-11

W_FLAGS=-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wswitch-default -Wundef -Werror -Wno-unused -Wconversion

CXX_FLAGS=-g -std=c++20 -Ofast -flto -fno-signed-zeros -fno-trapping-math -frename-registers -funroll-loops -D_GLIBCXX_PARALLEL -march=native -Iinclude $(W_FLAGS)

L_FLAGS=-L/usr/lib/x86_64-linux-gnu -flto

main: build/main.o build/brain_serde.o
	$(LD) -o $@ $^ $(L_FLAGS)
build/main.o: src/main.cc include/graph.h include/brain_serde.h include/rank.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<
build/brain_serde.o: src/brain_serde.cc include/brain_serde.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

exe: main
	./main

clean:
	rm -rf build/*.o
	rm -rf main

.PHONY: clean, exe
