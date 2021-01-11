/**
	Implementation of the concurrent skip list data structure
	Sorted output written into another file
	Usage: ./skiplist [--name] -i <iterations> -t <num_threads> --operation=<combined, separate> [--help]
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
	cout << "./skiplist [--name] -i <max_number> -t <num_threads> --operation=<combined, separate> [--help] \n" ;
	cout << "--name                  Prints full name \n" ;
	cout << "-i <max_number>         Numbers from 1 to max_number are inserted into skip list, subset is chosen for get, delete, and range \n" ;
	cout << "-t <num_threads>        Max number of threads to use \n" ;
	cout << "--operation=<separate>  Performs multithreaded insert, delete, get and range operations one after the other \n" ;
	cout << "--operation=<combined>  Mixes up or combined get, insert, delete, range operations \n" ;
    cout << "--help                  Prints the usage of the program \n";
    cout << "\n[ Kindly use less input values since the output is displayed on console. Higher values provide an unreadable display ]\n"; 
    cout << "\n[ max_number must be between INT_MIN and INT_MAX and exclusive of INT_MIN and INT_MAX ]" << endl;
    cout << "[Output may be interleaved since multiple threads tend to print search value found together.] \n" << endl;

	exit(EXIT_FAILURE);
}

/**
    Generates input data in vectors to perform the operations on the skip list.
*/
void generate_input(int max_number){
    // generating insert data
    for(int i = 1; i <= max_number; i++){
        numbers_insert.push_back(i);
    }

    // generating delete data
    for(size_t i = 0; i < numbers_insert.size(); i++){
        if( rand() % 4 == 0 ){
            numbers_delete.push_back(numbers_insert[i]);
        }
    }

    // generating search data
    for(size_t i = 0; i < numbers_insert.size(); i++){
        if( rand() % 4 == 0 ){
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
    Performs parallel insert to skip list from start index to end index(not inclusive) of the insert vector
*/
void skiplist_add(size_t start, size_t end){
    if(end >= numbers_insert.size()) end = numbers_insert.size();
    if(start == end) skiplist.add(numbers_insert[start], to_string(numbers_insert[start]));
    for(size_t i = start; i < end; i++){
        skiplist.add(numbers_insert[i], to_string(numbers_insert[i]));
    }
}

/**
    Performs parallel delete to skip list from start index to end index(not inclusive) of the delete vector
*/
void skiplist_remove(size_t start, size_t end){
    if(end >= numbers_delete.size()) end = numbers_delete.size();
    if(start == end) skiplist.remove(numbers_delete[start]);
    for(size_t i = start; i < end; i++){
        skiplist.remove(numbers_delete[i]);
    }
}

/**
    Performs parallel search to skip list from start index to end index(not inclusive) of the search vector
*/
void skiplist_search(size_t start, size_t end){
    if(end >= numbers_get.size()) end = numbers_get.size();
    if(start == end) end++;
    for(size_t i = start; i < end; i++){
        string s = skiplist.search(numbers_get[i]);
        if(s.empty()) s = "Not Found";
        cout << "Searching for " << numbers_get[i] << " Search value: " << s << endl;
    }
}

/**
    Performs parallel range to skip list from start index to end index(not inclusive) of the range vector
*/
void skiplist_range(int start, int end){
    map<int, string> range_output = skiplist.range(start, end);

    string s = "";
    for (auto const& x : range_output){
        s += x.second + " ";
    }
    cout << "Range (" << start << ", " << end << ") = " << s << endl;
}

/**
    Performs operations on skip list one after the other. Each operation works concurrently
*/
void concurrent_skiplist_seperate(){
    vector<thread> threads;

    // insert
    int chunk_size = ceil(float(numbers_insert.size()) / num_threads);
    for(size_t i = 0; i < numbers_insert.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_add, i, i+chunk_size));
    }
    for (auto &th : threads) {
        th.join();
    }

    cout << "\n---------- Skip list after insert ----------" << endl;

    skiplist.display();

    threads.clear();

    cout << "\n---------- Deleting the following elements from the Skip list ----------" << endl;

    for (auto i = numbers_delete.begin(); i != numbers_delete.end(); ++i)
    std::cout << *i << ' ';

    // delete
    chunk_size = ceil(float(numbers_delete.size()) / num_threads);
    for(size_t i = 0; i < numbers_delete.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_remove, i, i+chunk_size));
    }
    for (auto &th : threads) {
        th.join();
    }

    cout << "\n\n---------- Skip list after delete ---------- " << endl;

    skiplist.display();

    threads.clear();

    cout << "\n---------- Searching random numbers in Skip list ---------- " << endl;
    // search
    chunk_size = ceil(float(numbers_get.size()) / num_threads);
    for(size_t i = 0; i < numbers_get.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_search, i, i+chunk_size));
    }
    for (auto &th : threads) {
        th.join();
    }

    threads.clear();

    cout << "\n---------- Range between random numbers in Skip list ---------- " << endl;
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
    cout << "Add: " << start << " " << end << endl;
    skiplist_add(start, end);

    start = (rand() % static_cast<int>(numbers_delete.size() + 1));
    end = start + (rand() % static_cast<int>(numbers_delete.size() - start + 1));
    cout << "Remove: " << start << " " << end << endl;
    skiplist_remove(start, end);

    start = (rand() % static_cast<int>(numbers_get.size() + 1));
    end = start + (rand() % static_cast<int>(numbers_get.size() - start + 1));
    cout << "Searching: " << start << " " << end << endl;
    skiplist_search(start, end);

    start = (rand() % static_cast<int>(numbers_insert.size() + 1));
    end = start + (rand() % static_cast<int>(numbers_insert.size() - start + 1));
    cout << "Range: " << start << " " << end << endl;
    skiplist_range(start, end);
}

/**
    Performs all operations paralelly on the skip list.
*/
void concurrent_skiplist_combined(){

    cout << "\n---------- Numbers being inserted ----------" << endl;

    for (auto i = numbers_insert.begin(); i != numbers_insert.end(); ++i)
    std::cout << *i << ' ';
    cout << endl;

    cout << "\n---------- Numbers being deleted ----------" << endl;

    for (auto i = numbers_delete.begin(); i != numbers_delete.end(); ++i)
    std::cout << *i << ' ';
    cout << endl;

    cout << "\n---------- Numbers being searched ----------" << endl;

    for (auto i = numbers_get.begin(); i != numbers_get.end(); ++i)
    std::cout << *i << ' ';
    cout << endl;

    vector<thread> threads;

    for(size_t i = 0; i < num_threads; i++){
        threads.push_back(thread(skiplist_combined_operations));
    }
    for (auto &th : threads) {
        th.join();
    }

    skiplist.display();

}


/**
    Performs the insert, delete, get and range opetations on the skiplist based on operation type
*/
int main(int argc, char *argv[]){

    static struct option long_options[] = {
        {"name", no_argument, NULL, 'n'},
        {"operation", required_argument, NULL, 'o'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };

    //Variables for options
    bool name = false;
    int max_number = -1;
    string operation = "";
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
            case 'o':
                operation = std::string(optarg);
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
	    if(operation == "" || max_number <= 0 || num_threads < 1 ){
	        show_usage();
	    }else{

            generate_input(max_number);
            skiplist = SkipList(numbers_insert.size(), 0.5);

	        if(operation == "separate"){
                concurrent_skiplist_seperate();
	        }
	        else if (operation == "combined"){
                concurrent_skiplist_combined();
	        }else{
	            cout << "Invalid operation type \n";
	            show_usage();
	        }
	    }
    }else{
        show_usage();
    }
    return 0;
}