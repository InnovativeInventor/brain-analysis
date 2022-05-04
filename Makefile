CXX=clang++
LD=clang++

W_FLAGS=-pedantic -Wall -Wextra

CXX_FLAGS=-g -std=c++17 -stdlib=libc++ -Ofast -Iinclude $(W_FLAGS)

L_FLAGS=-stdlib=libc++ -lc++abi

main: build/main.o build/brain_serde.o
	$(LD) -o $@ $^ $(L_FLAGS)
build/main.o: src/main.cc include/graph.h include/brain_serde.h include/rank.h include/girvan_newman.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<
build/brain_serde.o: src/brain_serde.cc include/brain_serde.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<


test: build/catch.o build/serde.o build/pagerank-manual.o build/pagerank-cycle.o build/brain_serde.o build/graph.o build/girvan_newman_tests.o
	$(LD) -o $@ $^ $(L_FLAGS)
build/catch.o: tests/tests-main.cc
	$(CXX) $(CXX_FLAGS) -c -o $@ $<
build/serde.o: tests/serde.cc include/brain_serde.h tests/utils.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<
build/graph.o: tests/graph.cc include/graph.h tests/utils.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<
build/pagerank-manual.o: tests/pagerank-manual.cc include/graph.h include/rank.h tests/utils.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<
build/pagerank-cycle.o: tests/pagerank-cycle.cc include/graph.h include/rank.h tests/utils.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<
build/girvan_newman_tests.o: tests/girvan_newman_tests.cc include/graph.h include/girvan_newman.h tests/utils.h include/brain_serde.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

exe: main
	./main data/coords.txt data/img_edge_list.txt data/results/img_orig.txt data/results/img_ranks.txt data/results/img_girvan_newman.txt 0.3
	./main data/coords.txt data/pcp_edge_list.txt data/results/pcp_orig.txt data/results/pcp_ranks.txt data/results/pcp_girvan_newman.txt 0.3
exe_test: test
	./test

clean:
	rm -rf build/*.o
	rm -rf main
	rm -rf test

.PHONY: clean, exe, exe_test
