CFLAGS = -Wall -g -std=c++11
CXX = g++

all: skiplist

skiplist:
	$(CXX) main.cpp key_value_pair.cpp node.cpp skip_list.cpp -o skiplist -pthread $(CFLAGS)
	$(CXX) benchmark.cpp key_value_pair.cpp node.cpp skip_list.cpp -o benchmark -pthread  $(CFLAGS)
	$(CXX) unit_test_1.cpp key_value_pair.cpp node.cpp skip_list.cpp -o unit_test_1 -pthread  $(CFLAGS)
	$(CXX) unit_test_2.cpp key_value_pair.cpp node.cpp skip_list.cpp -o unit_test_2 -pthread  $(CFLAGS)
	$(CXX) unit_test_3.cpp key_value_pair.cpp node.cpp skip_list.cpp -o unit_test_3 -pthread  $(CFLAGS)

clean:
	rm skiplist benchmark unit_test_1 unit_test_2 unit_test_3