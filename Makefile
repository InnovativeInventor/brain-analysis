CXX=clang++
LD=clang++

W_FLAGS=-pedantic -Wall -Wextra

CXX_FLAGS=-g -std=c++17 -stdlib=libc++ -Ofast -Iinclude $(W_FLAGS)

L_FLAGS=-stdlib=libc++ -lc++abi

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
