using namespace std;

#include <string>

class KeyValuePair{
    private:
        int key;
        string value;
    public:
        KeyValuePair();
        KeyValuePair(int key, string value);
        ~KeyValuePair();
        int get_key();
        string get_value();
};