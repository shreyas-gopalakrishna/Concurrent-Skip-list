# Concurrent-Skip-list

### Skip list
A skip list is probabilistic data structure with a logarithmic insertion, deletion and search in the average case with an ordered sequence of elements while maintaining a linked list like structure. The skip list is built on multiple layers. The bottom layer has all the elements connected and the topper layers behave as an express lane to skip few elements to transverse the skip list faster. Insert, search, delete all traverse the express lane to find the right position quickly and then perform the necessary action. A depiction of how a skip list is organized is shown in the image below.

### Concurrent Skip list
A concurrent skip list is a multithreaded implementation of the skip list data structure where the insert, delete, get and range operations can be performed together across multiple threads. The implementation uses hand over hand locking to access nodes for writing and atomic variables while reading (no locks needed while reading).

### Implementation Strategies

1.  Node 
  ``` Node
  class Node{
    public:
      KeyValuePair key_value_pair; vector<Node*> next;
      mutex node_lock;
      atomic<bool> marked = {false}; atomic<bool> fully_linked = {false}; int top_level;
  };
```
  The 𝐾𝑒𝑦𝑉𝑎𝑙𝑢𝑒𝑃𝑎𝑖𝑟 stores a key and value for every node. In my implementation, the key is an integer, and the value is a string. The 𝑛𝑒𝑥𝑡 member variable points to the next node at each level in the skip list. Each node uses a 𝑛𝑜𝑑𝑒_𝑙𝑜𝑐𝑘 to lock the node when it is being modified. An atomic variable 𝑚𝑎𝑟𝑘𝑒𝑑 is used to indicate if a node is being deleted and another atomic variable 𝑓𝑢𝑙𝑙𝑦_𝑙𝑖𝑛𝑘𝑒𝑑 is used to indicate if node is completely linked to its successors and predecessors. The member variable 𝑡𝑜𝑝_𝑙𝑒𝑣𝑒𝑙 has the max level until which the particular node is available.
  
2. Skip list – insert

Before inserting an element into the skip list, we check if the element is already present in the skip list and if the node is marked. If the element is already present and node is unmarked, we don’t insert the element since it is already present in the skip list. If the element is present and node is not fully linked, then we wait until it is completely linked before inserting. If the element is present and node is marked, it is being deleted, so we wait and try our whole insert algo again later.

To add the element after the above check, we find references to predecessors and successors of the position this element has to be inserted at each level. These references can be corrupted by the time we actually perform the insert. Since each node just has a pointer to the next node, we will only need to hold the lock of the predecessor and not the successor. But we need to be sure that both the predecessor and successor is not marked and the next of the predecessor is the successor at each level. In case these conditions are not met, we wait and try our whole insert algo again later.

To insert, we start holding lock of the predecessor node at each level simultaneously checking the above conditions, if conditions not met, we release the locks held and go for a fresh try to insert. Once the condition is met, we have the lock to all the predecessors, and we can make the insert. To insert, a new node is created by randomly choosing the top level until which it must be available. The successors of the newly created node are linked at every level and then the predecessors at each level are linked to the newly created node. Once all the links are complete the node is marked as fully linked and then we release all the locks of the predecessors held at each level. This completes the concurrent insert.

3. Skip list – delete

Before deleting an element from the skip list, we check if the element is present in the skip list and if the node is not present, we return. If the element is present, we check if is fully linked and unmarked if not, we try the delete algo again.

Once the above conditions are met, we find references to predecessors and successors of the position this element is present. These references can be corrupted by the time we actually perform the insert. We try to take the lock of the node to be deleted and then go ahead to try acquiring the locks of the predecessors to the node at each level. While acquiring the locks, we also check if the predecessor is not marked and also if the next element to the predecessor is the current element we are trying to delete. If the conditions are not met, we release lock of the predecessors we are holding, also release the lock of the element being deleted and try the delete algo again.

Once we have all the locks of the predecessors, the required conditions are met, so we now link the predecessors to the successors of the node to be deleted. Once the linking is done, the node is deleted from the skip list and memory for that particular node is freed. After deleting the node, the locks of all the predecessor nodes held are released. This completes the concurrent delete.


4. Skip list – search (wait-free)

The search for an element in the skip list is done by traversing the entire skip list at higher level and dropping to lower levels as the search gets closer to the search key. If a key is found, we check if the node is unmarked and fully linked. It yes, then our search is successful, and we return the value associated with the key. If the node is marked or not fully linked, we return false as the node is marked for deletion or not completely linked after other operations.

The atomic member variables of the node 𝑚𝑎𝑟𝑘𝑒𝑑 and 𝑓𝑢𝑙𝑙𝑦_𝑙𝑖𝑛𝑘𝑒𝑑 make sure that we don’t need to lock the node to read. Hence making the read or search operation lock free. This implementation allows multiple readers to execute in parallel.

5. Skip list – range

The range operation works similar to the search where we traverse the skip list at higher level and drop to lower level as we get closer to the start of the range. When we find key in between the range we need, we add the key value pair to a map. If we encounter a node which is marked, it is ignored. If we encounter a node which is not fully linked, we wait until completely linked and then continue the traversal until we exceed the end of range. The map now contains all the key value pairs within the range which is returned.


### Usage 

``` Skiplist s = SkipList(num_of_elements,fraction) ```

``` Skiplist s = SkipList(100, 0.5) ```

### Compilation instructions

``` g++ main.cpp key_value_pair.cpp node.cpp skip_list.cpp -o skiplist -pthread ```

``` g++ benchmark.cpp key_value_pair.cpp node.cpp skip_list.cpp -o skiplist -pthread ```

``` g++ unit_test_1.cpp key_value_pair.cpp node.cpp skip_list.cpp -o skiplist -pthread ```

### Execution instructions

``` ./skiplist [--name] -i <iterations> -t <num_threads> --operation=<combined, separate> [--help] ```

``` perf stat -d /benchmark [--name] -i <max_number> -t <num_threads> --benchmark=<insert, delete, search, range, all_operations, high_contention, low_contention> [--help] ```

