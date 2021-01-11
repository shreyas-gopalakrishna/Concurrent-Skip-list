/**
    Stores a Key and value pair.
    This file can be modified if the types of key and value change.
*/

#include "key_value_pair.h"


/**
    Constructors
*/
KeyValuePair::KeyValuePair(){
}

KeyValuePair::KeyValuePair(int k, string v){
    key = k;
    value = v;
}

/**
    Returns the key
*/
int KeyValuePair::get_key(){
    return key;
}

/**
    Returns the value
*/
string KeyValuePair::get_value(){
    return value;
}

KeyValuePair::~KeyValuePair(){
}