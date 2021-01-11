#include <map>
#include "node.h"

class SkipList{
    private:
        // Head and Tail of the Skiplist
        Node *head;
        Node *tail;
    public:
        SkipList();
        SkipList(int max_elements, float probability);
        ~SkipList();
        int get_random_level();

        // Supported operations
        int find(int key, vector<Node*> &predecessors, vector<Node*> &successors);
        bool add(int key, string value);
        string search(int key);
        bool remove(int key);
        map<int, string> range(int start_key, int end_key);
        void display();
};