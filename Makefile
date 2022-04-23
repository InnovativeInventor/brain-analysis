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
test: build/catch.o build/serde.o build/pagerank-cycle.o build/brain_serde.o
	$(LD) -o $@ $^ $(L_FLAGS)
build/catch.o: tests/tests-main.cc
	$(CXX) $(CXX_FLAGS) -c -o $@ $<
build/serde.o: tests/serde.cc include/brain_serde.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<
build/pagerank-cycle.o: tests/pagerank-cycle.cc
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

exe: main
	./main
exe_test: test
	./test

clean:
	rm -rf build/*.o
	rm -rf main
	rm -rf test

.PHONY: clean, exe, exe_test
