CXX = g++
CXXFLAGS = --std=c++11

FILES = node.cpp graph.cpp
SRC_PATH=./src
SOURCES=$(FILES:%.cpp=$(SRC_PATH)/%.cpp)
headers=$(patsubst %.cpp,%.hpp,$(SOURCES))
objects=$(patsubst %.cpp,%.o,$(SOURCES))

MKDIR_P:
	mkdir -p output/
	mkdir -p bin/

TEST_PATH=./tests
BIN_PATH=./bin

test-TDB: $(TEST_PATH)/Tarjan_and_DBSCAN.cpp ./src/node.o ./src/graph.o
	$(CXX) $(CXXFLAGS) -I$(SRC_PATH) $(objects) -o $(BIN_PATH)/test-TDB $(TEST_PATH)/Tarjan_and_DBSCAN.cpp

test-ER: $(TEST_PATH)/ERdigraph.cpp ./src/node.o ./src/graph.o
	$(CXX) $(CXXFLAGS) -I$(SRC_PATH) $(objects) -o $(BIN_PATH)/test-ER $(TEST_PATH)/ERdigraph.cpp

test-graph: $(TEST_PATH)/test_graph.cpp ./src/node.o ./src/graph.o
	$(CXX) $(CXXFLAGS) -I$(SRC_PATH) $(objects) -o $(BIN_PATH)/test-graph $(TEST_PATH)/test_graph.cpp

test-ER_p: $(TEST_PATH)/ER_find_p.cpp ./src/node.o ./src/graph.o
	$(CXX) $(CXXFLAGS) -I$(SRC_PATH) $(objects) -o $(BIN_PATH)/test-ER_p $(TEST_PATH)/ER_find_p.cpp

test-DBSCAN_rate: $(TEST_PATH)/DBSCAN_find_rate.cpp ./src/node.o ./src/graph.o ./src/ConfusionMatrix.o
	$(CXX) $(CXXFLAGS) -I$(SRC_PATH) $(objects) ./src/ConfusionMatrix.o -o $(BIN_PATH)/test-DBSCAN_rate $(TEST_PATH)/DBSCAN_find_rate.cpp

install: MKDIR_P ./src/node.o ./src/graph.o ./src/ConfusionMatrix.o test-TDB test-ER test-graph test-ER_p test-DBSCAN_rate

clean:
	rm ./src/*.o
	rm ./bin/*
