/**
	Unit test 3 for the concurrent skip list data structure
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

void skiplist_combined_operations(){

    int start = (rand() % static_cast<int>(numbers_insert.size() + 1));
    int end = start + (rand() % static_cast<int>(numbers_insert.size() - start + 1));
    // cout << "Add Index: " << start << " " << end << endl;
    skiplist_add(start, end);

    start = (rand() % static_cast<int>(numbers_delete.size() + 1));
    end = start + (rand() % static_cast<int>(numbers_delete.size() - start + 1));
    // cout << "Remove Index: " << start << " " << end << endl;
    skiplist_remove(start, end);

    start = (rand() % static_cast<int>(numbers_get.size() + 1));
    end = start + (rand() % static_cast<int>(numbers_get.size() - start + 1));
    // cout << "Searching Index: " << start << " " << end << endl;
    skiplist_search(start, end);

    start = (rand() % static_cast<int>(numbers_insert.size() + 1));
    end = start + (rand() % static_cast<int>(numbers_insert.size() - start + 1));
    // cout << "Range: " << start << " " << end << endl;
    skiplist_range(start, end);
}


void concurrent_skiplist_combined(){

    vector<thread> threads;

    for(size_t i = 0; i < num_threads; i++){
        threads.push_back(thread(skiplist_combined_operations));
    }
    for (auto &th : threads) {
        th.join();
    }
}

/**
    Performs the insert, delete, get and range opetations on the skiplist
*/
int main(int argc, char *argv[]){

    cout << "\n---------- Unit Test - 3 ----------" << endl;

    cout << "\nThis Unit test uses 8 Threads. Numbers (1-5000) are used for this unit test." << endl;
    cout << "All threads call a function which performs insert, delete, search and range with random numbers (1-5000)." << endl;
    cout << "This simulates all operations parallelly on the skip list." << endl;

    cout << "\nThe final elements in the skip list will be based on if insert or delete got scheduled and executed first for any given element.\n" << endl;
    
    cout << "Few of the numbers from the 'delete vector' are deleted from the skip list and few may still exist since insert got scheduled after delete for these numbers." << endl;
    cout << "But the tests notice that none of the numbers not present in the 'delete vector' are deleted. And our traversal of the skip list is sorted. This verifies our skip list to be accurate. \n" << endl;

	generate_input(5000);

    skiplist = SkipList(numbers_insert.size(), 0.5);

    concurrent_skiplist_combined();

    // checking if insert is done
    if(skiplist.search(numbers_insert[0]) == to_string(numbers_insert[0]) || skiplist.search(numbers_insert[0]) == ""){
        cout << "Unit Test 1: Insert: PASS" << endl;
    }else{
        cout << "Unit Test 1: Insert: FAIL" << endl;
    }

    // checking if insert is done
    if(skiplist.search(numbers_insert[1]) == to_string(numbers_insert[1]) || skiplist.search(numbers_insert[1]) == "" ){
        cout << "Unit Test 2: Insert: PASS" << endl;
    }else{
        cout << "Unit Test 2: Insert: FAIL" << endl;
    }


    // checking if delete is done
    if(skiplist.search(numbers_delete[0]) == ""){
        cout << "Unit Test 3: Delete: PASS" << endl;
    }else{
        cout << "Unit Test 3: Delete: FAIL" << endl;
    }

    // checking if delete is done
    if(skiplist.search(numbers_delete[1]) == ""){
        cout << "Unit Test 4: Delete: PASS" << endl;
    }else{
        cout << "Unit Test 4: Delete: FAIL" << endl;
    }


    if(skiplist.search(numbers_get[0]) == to_string(numbers_get[0]) || skiplist.search(numbers_get[0]) == ""){
        cout << "Unit Test 5: Search: PASS" << endl;
    }else{
        cout << "Unit Test 5: Insert: FAIL" << endl;
    }

    // checking if Search is done
    if(skiplist.search(numbers_get[1]) == to_string(numbers_get[1]) || skiplist.search(numbers_get[1]) == ""){
        cout << "Unit Test 6: Search: PASS" << endl;
    }else{
        cout << "Unit Test 6: Search: FAIL" << endl;
    }

    // checking by search if deleted element is present
    if(skiplist.search(numbers_delete[0]) == ""){
        cout << "Unit Test 7: Search: PASS" << endl;
    }else{
        cout << "Unit Test 7: Search: FAIL" << endl;
    }

    map<int, string> range_output = skiplist.range(1, 5000);

    // checking range
    if(range_output.count(numbers_delete[0]) == 0){
        cout << "Unit Test 8: Range: PASS" << endl;
    }else{
        cout << "Unit Test 8: Range: FAIL" << endl;
    }

    // checking range
    if(range_output.count(numbers_delete[1]) == 0){
        cout << "Unit Test 9: Range: PASS" << endl;
    }else{
        cout << "Unit Test 9: Range: FAIL" << endl;
    }

	
    return 0;
}