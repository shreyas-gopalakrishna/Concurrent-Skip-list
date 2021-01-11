/**
    One single Node in the Skip list and its properties
*/

#include "node.h"


/**
    Constructors
*/

Node::Node(){
}

Node::Node(int key, int level){
    key_value_pair = KeyValuePair(key, "");
    next.resize(level + 1);
    for (size_t i = 0; i < next.size(); i++){
        next[i] = NULL;
    }
    top_level = level;
}

Node::Node(int key, string value, int level){
    key_value_pair = KeyValuePair(key, value);
    next.resize(level + 1);
    for (size_t i = 0; i < next.size(); i++){
        next[i] = NULL;
    }
    top_level = level;
}

/**
    Returns the key in the node
*/
int Node::get_key(){
    return key_value_pair.get_key();
}

/**
    Returns the value in the node
*/
string Node::get_value(){
    return key_value_pair.get_value();
}

/**
    Locks the node
*/
void Node::lock(){
    node_lock.lock();
}

/**
    Unlocks the node
*/
void Node::unlock(){
    node_lock.unlock();
}

Node::~Node(){
}