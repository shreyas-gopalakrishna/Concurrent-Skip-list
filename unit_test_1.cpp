/**
	Unit test 1 for the concurrent skip list data structure
*/
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <iterator>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "skip_list.h"

using namespace std;

size_t num_threads = 4;
SkipList skiplist;

/**
    Integers to be used for operations
*/
vector<int> numbers_insert;
vector<int> numbers_delete;
vector<int> numbers_get;
vector<pair<int,int>> numbers_range;

/*
    Generates input to test the skip list
*/
void generate_input(int max_number){
    // generating insert data
    for(int i = 1; i <= max_number; i++){
        numbers_insert.push_back(i);
    }

    // generating delete data
    for(size_t i = 0; i < numbers_insert.size(); i++){
        if( rand() % 3 == 0 ){
            numbers_delete.push_back(numbers_insert[i]);
        }
    }

    // generating search data
    for(size_t i = 0; i < numbers_insert.size(); i++){
        if( rand() % 5 == 0 ){
            numbers_get.push_back(numbers_insert[i]);
        }
    }

    // generating range data
    for(size_t i = 0; i < num_threads; i++){
        int a = (rand() % static_cast<int>(max_number + 1));
        int b = a + (rand() % static_cast<int>(max_number - a + 1));
        numbers_range.push_back(make_pair(a,b));
    }
}

/**

*/
void skiplist_add(size_t start, size_t end){
    if(end >= numbers_insert.size()) end = numbers_insert.size();
    if(start == end) skiplist.add(numbers_insert[start], to_string(numbers_insert[start]));
    for(size_t i = start; i < end; i++){
        skiplist.add(numbers_insert[i], to_string(numbers_insert[i]));
    }
}

void skiplist_remove(size_t start, size_t end){
    if(end >= numbers_delete.size()) end = numbers_delete.size();
    if(start == end) skiplist.remove(numbers_delete[start]);
    for(size_t i = start; i < end; i++){
        skiplist.remove(numbers_delete[i]);
    }
}

void skiplist_search(size_t start, size_t end){
    if(end >= numbers_get.size()) end = numbers_get.size();
    if(start == end) end++;
    for(size_t i = start; i < end; i++){
        string s = skiplist.search(numbers_get[i]);
        if(s.empty()) s = "Not Found";
        cout << "Searching for " << numbers_get[i] << " Search value: " << s << endl;
    }
}


void skiplist_range(int start, int end){
    map<int, string> range_output = skiplist.range(start, end);

    string s = "";
    for (auto const& x : range_output){
        s += x.second + " ";
    }

    cout << "Range (" << start << ", " << end << ") = " << s << endl;
}

/**
    Performs the insert, delete, get and range opetations on the skiplist
*/
int main(int argc, char *argv[]){

    cout << "\n---------- Unit Test - 1 ----------" << endl;

    cout << "\nThis Unit test uses 4 Threads. Numbers (1-30) are inserted into the skip list parallelly." << endl;
    cout << "Then a few numbers at random are removed from the skip list parallelly. And next a few numbers are searched in the skip list." << endl;
    cout << "Finally, few Range operations are done in the skip list parallelly. The output is dislayed after each step for verification." << endl;
    cout << "This Unit test displays the output. Output may be interleaved since multiple threads tend to print together." << endl;
    

	generate_input(30);

    skiplist = SkipList(numbers_insert.size(), 0.5);

    vector<thread> threads;

    cout << "\n---------- Numbers inserted parallelly ----------" << endl;

    for (auto i = numbers_insert.begin(); i != numbers_insert.end(); ++i)
    std::cout << *i << ' ';
    cout << endl;

    // insert
    int chunk_size = ceil(float(numbers_insert.size()) / num_threads);
    for(size_t i = 0; i < numbers_insert.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_add, i, i+chunk_size));
    }
    for (auto &th : threads) {
        th.join();
    }
    threads.clear();

    cout << "\n---------- Skip list after insert ----------" << endl;
    skiplist.display();


    cout << "\n---------- Numbers deleted parallelly ----------" << endl;

    for (auto i = numbers_delete.begin(); i != numbers_delete.end(); ++i)
    std::cout << *i << ' ';
    cout << endl;

    // delete
    chunk_size = ceil(float(numbers_delete.size()) / num_threads);
    for(size_t i = 0; i < numbers_delete.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_remove, i, i+chunk_size));
    }
    for (auto &th : threads) {
        th.join();
    }
    threads.clear();

    cout << "\n---------- Skip list after delete ----------" << endl;
    skiplist.display();

    cout << "\n---------- Numbers searched parallelly ----------" << endl;

    for (auto i = numbers_get.begin(); i != numbers_get.end(); ++i)
    std::cout << *i << ' ';
    cout << endl;

    // search
    chunk_size = ceil(float(numbers_get.size()) / num_threads);
    for(size_t i = 0; i < numbers_get.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_search, i, i+chunk_size));
    }
    for (auto &th : threads) {
        th.join();
    }
    threads.clear();

    cout << "\n---------- Range between random numbers in Skip list parallely ---------- " << endl;
    // range
    for(size_t i = 0; i < num_threads; i++){
        threads.push_back(thread(skiplist_range, numbers_range[i].first, numbers_range[i].second));
    }
    for (auto &th : threads) {
        th.join();
    }
	
    return 0;
}