/**
	Program to perform benchmark testing on the skip list.
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

size_t num_threads = 1;
SkipList skiplist;
size_t max_number = 100;
struct timespec start_time, end_time;

/**
    Integers to be used for operations
*/
vector<int> numbers_insert;
vector<int> numbers_delete;
vector<int> numbers_get;
vector<pair<int,int>> numbers_range;

/**
    Display the usage of the program
*/
void show_usage(){
	cout << "Usage: \n\n" ;
	cout << "./benchmark [--name] -i <max_number> -t <num_threads> --benchmark=<insert, delete, search, range, all_operations, high_contention, low_contention> [--help] \n" ;
	cout << "--name                         Prints full name \n" ;
	cout << "-i <max_number>                Numbers from 0 to max_number are inserted into skip list, subset is chosen for get, delete, and range \n" ;
	cout << "-t <num_threads>               Max number of threads to use \n" ;
	cout << "--benchmark=<insert>           Performs multithreaded insert based on input \n" ;
	cout << "--benchmark=<delete>           Performs multithreaded delete based on input \n" ;
	cout << "--benchmark=<search>           Performs multithreaded search based on input \n" ;
	cout << "--benchmark=<range>            Performs multithreaded range based on input \n" ;
	cout << "--benchmark=<all_operations>   Performs multithreaded all operations \n" ;
	cout << "--benchmark=<high_contention>  Simulates high contention \n" ;
	cout << "--benchmark=<low_contention>   Simulates low contention \n" ;
    cout << "--help                         Prints the usage of the program \n"; 
    cout << "\n[ max_number must be between INT_MIN and INT_MAX and exclusive of INT_MIN and INT_MAX ]\n";
	exit(EXIT_FAILURE);
}

/**
    Display elapsed time
*/
void show_elapsed_time(){
    unsigned long long elapsed_ns;
	elapsed_ns = (end_time.tv_sec-start_time.tv_sec)*1000000000 + (end_time.tv_nsec-start_time.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
}

void generate_input(int max_number){
    // generating insert data
    for(int i = 1; i <= max_number; i++){
        numbers_insert.push_back(i);
    }

    // generating delete data
    for(size_t i = 0; i < numbers_insert.size(); i++){
        numbers_delete.push_back(numbers_insert[i]);
    }

    // generating search data
    for(size_t i = 0; i < numbers_insert.size(); i++){
        numbers_get.push_back(numbers_insert[i]);
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
}

void insert_benchmark(){
    vector<thread> threads;

    // insert
    int chunk_size = ceil(float(numbers_insert.size()) / num_threads);
    for(size_t i = 0; i < numbers_insert.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_add, i, i+chunk_size));
    }
    for (auto &th : threads) {
        th.join();
    }

    threads.clear();
}

void delete_benchmark(){
    vector<thread> threads;

    // delete
    int chunk_size = ceil(float(numbers_delete.size()) / num_threads);
    for(size_t i = 0; i < numbers_delete.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_remove, i, i+chunk_size));
    }
    for (auto &th : threads) {
        th.join();
    }

    threads.clear();
}


void search_benchmark(){
    vector<thread> threads;

    // search
    int chunk_size = ceil(float(numbers_get.size()) / num_threads);
    for(size_t i = 0; i < numbers_get.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_search, i, i+chunk_size));
    }
    for (auto &th : threads) {
        th.join();
    }

    threads.clear();

}
void range_benchmark(){
    vector<thread> threads;

    // range
    for(size_t i = 0; i < num_threads; i++){
        threads.push_back(thread(skiplist_range, numbers_range[i].first, numbers_range[i].second));
    }
    for (auto &th : threads) {
        th.join();
    }

    threads.clear();

}

void skiplist_combined_operations(){

    int start = (rand() % static_cast<int>(numbers_insert.size() + 1));
    int end = start + (rand() % static_cast<int>(numbers_insert.size() - start + 1));
    skiplist_add(start, end);

    start = (rand() % static_cast<int>(numbers_delete.size() + 1));
    end = start + (rand() % static_cast<int>(numbers_delete.size() - start + 1));
    skiplist_remove(start, end);

    start = (rand() % static_cast<int>(numbers_get.size() + 1));
    end = start + (rand() % static_cast<int>(numbers_get.size() - start + 1));
    skiplist_search(start, end);

    start = (rand() % static_cast<int>(numbers_insert.size() + 1));
    end = start + (rand() % static_cast<int>(numbers_insert.size() - start + 1));
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

void all_operations_benchmark(){
    concurrent_skiplist_combined();
}

void high_contention_benchmark_thread(){

    for(size_t i = 0; i < max_number; i++){
        skiplist.add(3 , "3");
        skiplist.remove(3);
    }

}

void high_contention_benchmark(){   
    skiplist = SkipList(3, 0.5);

    skiplist.add(1, "1");
    skiplist.add(2, "2");

    vector<thread> threads;

    for(size_t i = 0; i < num_threads; i++){
        threads.push_back(thread(high_contention_benchmark_thread));
    }
    for (auto &th : threads) {
        th.join();
    }
}

void low_contention_benchmark(){

    vector<thread> threads;

    // generating insert data
    for(size_t i = 1; i <= max_number; i++){
        numbers_insert.push_back(i);
    }

    skiplist = SkipList(numbers_insert.size(), 0.5);

    // insert
    int chunk_size = ceil(float(numbers_insert.size()) / num_threads);
    for(size_t i = 0; i < numbers_insert.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_add, i, i+chunk_size));
    }
    for (auto &th : threads) {
        th.join();
    }

    threads.clear();
}


/**
    Performs the insert, delete, get and range opetations on the skiplist to benchmark test it.
*/
int main(int argc, char *argv[]){

    static struct option long_options[] = {
        {"name", no_argument, NULL, 'n'},
        {"benchmark", required_argument, NULL, 'b'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };

    //Variables for options
    bool name = false;
    string benchmark = "";
    bool help = false;

    while (true) {
        int option_index = 0;
        int flag_char = getopt_long(argc, argv, "i:t:", long_options, &option_index);
        if (flag_char == -1) {
          break;
        }

        switch (flag_char) {
            case 'n':
                name = true;
                break;
            case 'b':
                benchmark = std::string(optarg);
                break;
            case 'h':
                help = true;
                break;
            case 't':
                num_threads = stoi(optarg);
                break;
            case 'i':
                max_number = stoi(optarg);
                break;
            case '?':
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    if(name){
        cout << "\nShreyas Gopalakrishna\n";
    }

    if(help){
        show_usage();
    }

	if(argc > 2){
	    if(benchmark == "" || max_number <= 0 || num_threads < 1 ){
	        show_usage();
	    }else{

	        if(benchmark == "insert"){
                generate_input(max_number);
                skiplist = SkipList(numbers_insert.size(), 0.5);
                clock_gettime(CLOCK_MONOTONIC,&start_time);
                insert_benchmark();
                clock_gettime(CLOCK_MONOTONIC,&end_time);
	        }
	        else if (benchmark == "delete"){
                generate_input(max_number);
                skiplist = SkipList(numbers_insert.size(), 0.5);
                insert_benchmark();
                clock_gettime(CLOCK_MONOTONIC,&start_time);
                delete_benchmark();
                clock_gettime(CLOCK_MONOTONIC,&end_time);
	        }else if (benchmark == "search"){
                generate_input(max_number);
                skiplist = SkipList(numbers_insert.size(), 0.5);
                insert_benchmark();
                clock_gettime(CLOCK_MONOTONIC,&start_time);
                search_benchmark();
                clock_gettime(CLOCK_MONOTONIC,&end_time);
	        }else if (benchmark == "range"){
                generate_input(max_number);
                skiplist = SkipList(numbers_insert.size(), 0.5);
                insert_benchmark();
                clock_gettime(CLOCK_MONOTONIC,&start_time);
                range_benchmark();
                clock_gettime(CLOCK_MONOTONIC,&end_time);
	        }
            else if (benchmark == "all_operations"){
                generate_input(max_number);
                skiplist = SkipList(numbers_insert.size(), 0.5);
                clock_gettime(CLOCK_MONOTONIC,&start_time);
                all_operations_benchmark();
                clock_gettime(CLOCK_MONOTONIC,&end_time);
	        }else if (benchmark == "high_contention"){
                clock_gettime(CLOCK_MONOTONIC,&start_time);
                high_contention_benchmark();
                clock_gettime(CLOCK_MONOTONIC,&end_time);
	        }else if (benchmark == "low_contention"){
                clock_gettime(CLOCK_MONOTONIC,&start_time);
                low_contention_benchmark();
                clock_gettime(CLOCK_MONOTONIC,&end_time);
	        }else{
	            cout << "Invalid benchmark type \n";
	            show_usage();
	        }
            show_elapsed_time();
	    }
    }else{
        show_usage();
    }
    return 0;
}