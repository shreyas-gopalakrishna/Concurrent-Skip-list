/**
	Unit test 2 for the concurrent skip list data structure
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

size_t num_threads = 8;
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
        if( rand() % 6 == 0 ){
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
    }
}


void skiplist_range(int start, int end){
    map<int, string> range_output = skiplist.range(start, end);

    // string s = "";
    // for (auto const& x : range_output){
    //     s += x.second + " ";
    // }

    // cout << "Range (" << start << ", " << end << ") = " << s << endl;
}

/**
    Performs the insert, delete, get and range opetations on the skiplist
*/
int main(int argc, char *argv[]){

    cout << "\n---------- Unit Test - 2 ----------" << endl;

    cout << "\nThis Unit test uses 8 Threads. Numbers (1-2000) are inserted into the skip list parallelly." << endl;
    cout << "Then a few numbers at random are removed from the skip list parallelly. And next a few numbers are searched in the skip list." << endl;
    cout << "Finally, few Range operations are done in the skip list parallelly. " << endl;
    cout << "This is an automated test, and only the test results are displayed. " << endl;
    

	generate_input(2000);

    skiplist = SkipList(numbers_insert.size(), 0.5);

    vector<thread> threads;
    int chunk_size;

    // insert
    try{
        chunk_size = ceil(float(numbers_insert.size()) / num_threads);
        for(size_t i = 0; i < numbers_insert.size(); i = i + chunk_size){
            threads.push_back(thread(skiplist_add, i, i+chunk_size));
        }
        for (auto &th : threads) {
            th.join();
        }
        threads.clear();
        cout << "Unit Test 1: Insert: PASS" << endl;
    }catch(const std::exception& e){
        cout << "Unit Test 1: Insert: FAIL" << endl;
    }

    // checking if insert is done
    if(skiplist.search(numbers_insert[0]) == to_string(numbers_insert[0])){
        cout << "Unit Test 2: Insert: PASS" << endl;
    }else{
        cout << "Unit Test 2: Insert: FAIL" << endl;
    }

    // checking if insert is done
    if(skiplist.search(numbers_insert[1]) == to_string(numbers_insert[1])){
        cout << "Unit Test 3: Insert: PASS" << endl;
    }else{
        cout << "Unit Test 3: Insert: FAIL" << endl;
    }

    // delete
    try{
        chunk_size = ceil(float(numbers_delete.size()) / num_threads);
        for(size_t i = 0; i < numbers_delete.size(); i = i + chunk_size){
            threads.push_back(thread(skiplist_remove, i, i+chunk_size));
        }
        for (auto &th : threads) {
            th.join();
        }
        threads.clear();
        cout << "Unit Test 4: Delete: PASS" << endl;
    }catch(const std::exception& e){
        cout << "Unit Test 4: Delete: FAIL" << endl;
    }

    // checking if delete is done
    if(skiplist.search(numbers_delete[0]) == ""){
        cout << "Unit Test 5: Delete: PASS" << endl;
    }else{
        cout << "Unit Test 5: Delete: FAIL" << endl;
    }

    // checking if delete is done
    if(skiplist.search(numbers_delete[1]) == ""){
        cout << "Unit Test 6: Delete: PASS" << endl;
    }else{
        cout << "Unit Test 6: Delete: FAIL" << endl;
    }

    // search
    try{
        chunk_size = ceil(float(numbers_get.size()) / num_threads);
        for(size_t i = 0; i < numbers_get.size(); i = i + chunk_size){
            threads.push_back(thread(skiplist_search, i, i+chunk_size));
        }
        for (auto &th : threads) {
            th.join();
        }
        threads.clear();
        cout << "Unit Test 7: Search: PASS" << endl;
    }catch(const std::exception& e){
        cout << "Unit Test 7: Search: FAIL" << endl;
    }

    // checking if search is done
    if(skiplist.search(numbers_get[0]) == to_string(numbers_get[0]) || skiplist.search(numbers_get[0]) == ""){
        cout << "Unit Test 8: Search: PASS" << endl;
    }else{
        cout << "Unit Test 8: Insert: FAIL" << endl;
    }

    // checking if Search is done
    if(skiplist.search(numbers_delete[0]) == ""){
        cout << "Unit Test 9: Search: PASS" << endl;
    }else{
        cout << "Unit Test 9: Search: FAIL" << endl;
    }
    
    // ranges
    try{
        for(size_t i = 0; i < num_threads; i++){
            threads.push_back(thread(skiplist_range, numbers_range[i].first, numbers_range[i].second));
        }
        for (auto &th : threads) {
            th.join();
        }
        threads.clear();
        cout << "Unit Test 10: Range: PASS" << endl;
    }catch(const std::exception& e){
        cout << "Unit Test 10: Range: FAIL" << endl;
    }

    map<int, string> range_output = skiplist.range(1, 2000);

    // checking range
    if(range_output.count(numbers_delete[0]) == 0){
        cout << "Unit Test 11: Range: PASS" << endl;
    }else{
        cout << "Unit Test 11: Range: FAIL" << endl;
    }

    return 0;
}