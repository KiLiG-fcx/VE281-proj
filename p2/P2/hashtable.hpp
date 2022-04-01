#include "hash_prime.hpp"

#include <exception>
#include <functional>
#include <vector>
#include <forward_list>
#include <math.h>

/**
 * The Hashtable class
 * The time complexity of functions are based on n and k
 * n is the size of the hashtable
 * k is the length of Key
 * @tparam Key          key type
 * @tparam Value        data type
 * @tparam Hash         function object, return the hash value of a key
 * @tparam KeyEqual     function object, return whether two keys are the same
 */
template<
        typename Key, typename Value,
        typename Hash = std::hash<Key>,
        typename KeyEqual = std::equal_to<Key>
>
class HashTable {
public:
    typedef std::pair<const Key, Value> HashNode;
    typedef std::forward_list<HashNode> HashNodeList;
    typedef std::vector<HashNodeList> HashTableData;

    /**
     * A single directional iterator for the hashtable
     * ! DO NOT NEED TO MODIFY THIS !
     */
    class Iterator {
    private:
        typedef typename HashTableData::iterator VectorIterator;
        typedef typename HashNodeList::iterator ListIterator;

        const HashTable *hashTable;
        VectorIterator bucketIt;    // an iterator of the buckets
        ListIterator listItBefore;  // a before iterator of the list, here we use "before" for quick erase and insert
        bool endFlag = false;       // whether it is an end iterator

        /**
         * Increment the iterator
         * Time complexity: Amortized O(1)
         */
        void increment() {
            if (bucketIt == hashTable->buckets.end()) {
                endFlag = true;
                return;
            }
            auto newListItBefore = listItBefore;
            ++newListItBefore;
            if (newListItBefore != bucketIt->end()) {
                if (++newListItBefore != bucketIt->end()) {
                    // use the next element in the current forward_list
                    ++listItBefore;
                    return;
                }
            }
            while (++bucketIt != hashTable->buckets.end()) {
                if (!bucketIt->empty()) {
                    // use the first element in a new forward_list
                    listItBefore = bucketIt->before_begin();
                    return;
                }
            }
            endFlag = true;
        }

        explicit Iterator(HashTable *hashTable) : hashTable(hashTable) {
            bucketIt = hashTable->buckets.begin();
            listItBefore = bucketIt->before_begin();
            endFlag = bucketIt == hashTable->buckets.end();
        }

        Iterator(HashTable *hashTable, VectorIterator vectorIt, ListIterator listItBefore) :
                hashTable(hashTable), bucketIt(vectorIt), listItBefore(listItBefore) {
            endFlag = bucketIt == hashTable->buckets.end();
        }

    public:
        friend class HashTable;

        Iterator() = delete;

        Iterator(const Iterator &) = default;

        Iterator &operator=(const Iterator &) = default;

        Iterator &operator++() {
            increment();
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            increment();
            return temp;
        }

        bool operator==(const Iterator &that) const {
            if (endFlag && that.endFlag) return true;
            if (bucketIt != that.bucketIt) return false;
            return listItBefore == that.listItBefore;
        }

        bool operator!=(const Iterator &that) const {
            if (endFlag && that.endFlag) return false;
            if (bucketIt != that.bucketIt) return true;
            return listItBefore != that.listItBefore;
        }

        HashNode *operator->() {
            auto listIt = listItBefore;
            ++listIt;
            return &(*listIt);
        }

        HashNode &operator*() {
            auto listIt = listItBefore;
            ++listIt;
            return *listIt;
        }
    };

protected:                                                                  // DO NOT USE private HERE!
    static constexpr double DEFAULT_LOAD_FACTOR = 0.5;                      // default maximum load factor is 0.5
    static constexpr size_t DEFAULT_BUCKET_SIZE = HashPrime::g_a_sizes[0];  // default number of buckets is 5

    HashTableData buckets;                                                  // buckets, of singly linked lists
    typename HashTableData::iterator firstBucketIt;                         // help get begin iterator in O(1) time

    size_t tableSize;                                                       // number of elements
    double maxLoadFactor;                                                   // maximum load factor
    Hash hash;                                                              // hash function instance
    KeyEqual keyEqual;                                                      // key equal function instance

    /**
     * Time Complexity: O(k)
     * @param key
     * @param bucketSize
     * @return the hash value of key with a new bucket size
     */
    inline size_t hashKey(const Key &key, size_t bucketSize) const {
        return hash(key) % bucketSize;
    }

    /**
     * Time Complexity: O(k)
     * @param key
     * @return the hash value of key with current bucket size
     */
    inline size_t hashKey(const Key &key) const {
        return hash(key) % buckets.size();
    }

    /**
     * Find the minimum bucket size for the hashtable
     * The minimum bucket size must satisfy all of the following requirements:
     * - It is not less than (i.e. greater or equal to) the parameter bucketSize
     * - It is greater than floor(tableSize / maxLoadFactor)
     * - It is a (prime) number defined in HashPrime (hash_prime.hpp)
     * - It is minimum if satisfying all other requirements
     * Time Complexity: O(1)
     * @throw std::range_error if no such bucket size can be found
     * @param bucketSize lower bound of the new number of buckets
     */
    size_t findMinimumBucketSize(size_t bucketSize) const {
        // TODO: implement this function
        //size_t size; // the final result
        size_t rq2=(size_t)(std::floor(double(tableSize)/maxLoadFactor));
     /*   for(int i=0;i<=61;i++){  //search from small to large (0-61) in gasize
            if(bucketSize==HashPrime::g_a_sizes[i]){
                return bucketSize;
            }
            else if(HashPrime::g_a_sizes[i]==rq2+1){
                return HashPrime::g_a_sizes[i];
            }
        }*/
        // not found bucket_size in the array
        for(int i=0;i<=61;i++){ //find the first one and return
            if(HashPrime::g_a_sizes[i]>rq2 && HashPrime::g_a_sizes[i]>=bucketSize){
                return HashPrime::g_a_sizes[i]; //find the minimum in the hash_prime
            }
        }
        return HashPrime::g_a_sizes[61]; //?
    }

    // TODO: define your helper functions here if necessary

public:
    HashTable() :
            buckets(DEFAULT_BUCKET_SIZE), tableSize(0), maxLoadFactor(DEFAULT_LOAD_FACTOR),
            hash(Hash()), keyEqual(KeyEqual()) {
        firstBucketIt = buckets.end();
    }

    explicit HashTable(size_t bucketSize) :
            tableSize(0), maxLoadFactor(DEFAULT_LOAD_FACTOR),
            hash(Hash()), keyEqual(KeyEqual()) {
        bucketSize = findMinimumBucketSize(bucketSize);
        buckets.resize(bucketSize);
        firstBucketIt = buckets.end();
    }

    HashTable(const HashTable &that) {
        // TODO: implement this function
        buckets = that.buckets;
        tableSize = that.tableSize;
        maxLoadFactor = that.maxLoadFactor;
        hash = that.hash;
        keyEqual = that.keyEqual;
        firstBucketIt = buckets.begin() + std::distance(that.buckets.begin(), that.firstBucketIt);
    }

    HashTable &operator=(const HashTable &that) {
        // TODO: implement this function
        if(this==&that){
            return *this;
        }
        buckets = that.buckets;
        tableSize = that.tableSize;
        maxLoadFactor = that.maxLoadFactor;
        hash = that.hash;
        keyEqual = that.keyEqual;
        firstBucketIt = buckets.begin() + std::distance(that.buckets.begin(), that.firstBucketIt);
        return *this;
    };

    ~HashTable() = default;

    Iterator begin() {
        if (firstBucketIt != buckets.end()) {
            return Iterator(this, firstBucketIt, firstBucketIt->before_begin());
        }
        return end();
    }

    Iterator end() {
        return Iterator(this, buckets.end(), buckets.begin()->before_begin());
    }

    /**
     * Find whether the key exists in the hashtable
     * Time Complexity: Amortized O(k)
     * @param key
     * @return whether the key exists in the hashtable
     */
    bool contains(const Key &key) {
        return find(key) != end();
    }

    /**
     * Find the value in hashtable by key
     * If the key exists, iterator points to the corresponding value, and it.endFlag = false
     * Otherwise, iterator points to the place that the key were to be inserted, and it.endFlag = true
     * Time Complexity: Amortized O(k)
     * @param key
     * @return a pair (success, iterator of the value)
     */
    Iterator find(const Key &key) {
        // TODO: implement this function
        size_t hk=hashKey(key); // get the key's bucket
        Iterator it(this);
        it.bucketIt=it.bucketIt+hk; // find its bucket
        it.listItBefore=it.bucketIt->before_begin(); //from the first in the list
        auto buck_first=it.listItBefore; // the before iterator of the key's bucket
        //buck_first++;
        for(buck_first++;buck_first!=it.bucketIt->end();++buck_first){ //iterate through the list
            if(key==buck_first->first){ // find the key in this bucket
                return it;
            }
            it.listItBefore++; // update the list iterator
        }
        it.endFlag=true; //reach the end
        return it;
    }

    /**
     * Insert value into the hashtable according to an iterator returned by find
     * the function can be only be called if no other write actions are done to the hashtable after the find
     * If the key already exists, overwrite its value
     * firstBucketIt should be updated
     * If load factor exceeds maximum value, rehash the hashtable
     * Time Complexity: O(k)
     * @param it an iterator returned by find
     * @param key
     * @param value
     * @return whether insertion took place (return false if the key already exists)
     */
    bool insert(const Iterator &it, const Key &key, const Value &value) {
        // TODO: implement this function
        auto itl=it.listItBefore;
        itl++; //iterator of the first list elem
        if(itl!=it.bucketIt->end() && key==itl->first){ //find the key in the table
            itl->second=value; //update the value
            return false; //already exists, return f
        }
        //does not exist
        this->tableSize++; //before insertion, add table size
        it.bucketIt->emplace_after(it.listItBefore,key,value); // insert into the forward_list
        firstBucketIt = min(firstBucketIt, it.bucketIt); //update the first_bucket_it
        if(loadFactor()>maxLoadFactor) { // after insertion, load factor exceed
            rehash(bucketSize()); // get the new bucketsize
        }
        return true;
    }

    /**
     * Insert <key, value> into the hashtable
     * If the key already exists, overwrite its value
     * firstBucketIt should be updated
     * If load factor exceeds maximum value, rehash the hashtable
     * Time Complexity: Amortized O(k)
     * @param key
     * @param value
     * @return whether insertion took place (return false if the key already exists)
     */
    bool insert(const Key &key, const Value &value) {
        // TODO: implement this function
        Iterator it=find(key); //find the iterator place
        if(!it.endFlag){ //already exists
            it->second=value; //find the key, update the value
            return false;
        }
        this->tableSize++; //before insertion, add table size
        it.bucketIt->emplace_after(it.listItBefore,key,value); // insert into the forward_list
        firstBucketIt = min(firstBucketIt, it.bucketIt); //update the first_bucket_it
        if(loadFactor()>maxLoadFactor) { // after insertion, load factor exceed
            rehash(bucketSize()); // get the new bucketsize
        }
        return true;
    }

    /**
     * Erase the key if it exists in the hashtable, otherwise, do nothing
     * DO NOT rehash in this function
     * firstBucketIt should be updated
     * Time Complexity: Amortized O(k)
     * @param key
     * @return whether the key exists
     */
    bool erase(const Key &key) {
        // TODO: implement this function
        Iterator it=find(key);
        if(it==this->end()){ //does not find
            return false;
        }
        //find the key
        this->tableSize--; //first reduce table_size
        it.bucketIt->erase_after(it.listItBefore);
        //update first_bucket_it
        Iterator itt=it;
        itt++;
        if(it.bucketIt->empty() && it.bucketIt==firstBucketIt){ //an empty first bucket
            this->firstBucketIt=itt.bucketIt; //update first bucket it
        }
        return true;
    }

    /**
     * Erase the key at the input iterator
     * If the input iterator is the end iterator, do nothing and return the input iterator directly
     * firstBucketIt should be updated
     * Time Complexity: O(1)
     * @param it
     * @return the iterator after the input iterator before the erase
     */
    Iterator erase(const Iterator &it) {
        // TODO: implement this function
        if(it==end()){
            return it;
        }
        //repeat the previous steps
        this->tableSize--;
        it.bucketIt->erase_after(it.listItBefore);
        Iterator itt=it;
        itt++;
        if(it.bucketIt->empty() && it.bucketIt==firstBucketIt){
            this->firstBucketIt=itt.bucketIt;
        }
        return itt;
    }

    /**
     * Get the reference of value by key in the hashtable
     * If the key doesn't exist, create it first (use default constructor of Value)
     * firstBucketIt should be updated
     * If load factor exceeds maximum value, rehash the hashtable
     * Time Complexity: Amortized O(k)
     * @param key
     * @return reference of value
     */
    Value &operator[](const Key &key) {
        // TODO: implement this function
        Iterator it=find(key);
        if(!it.endFlag){ //find it, return value=it.second
            return it->second;
        }
        //create the key-value, just like insertion
        this->tableSize++;
        it.bucketIt->emplace_after(it.listItBefore,key,Value()); // create with default
        firstBucketIt = min(firstBucketIt, it.bucketIt); //update the first_bucket_it
        Value &value=it->second; //get the default value
        if(loadFactor()>maxLoadFactor) { // after insertion, load factor exceed
            rehash(bucketSize()); // get the new bucketsize
        }
        return value;
    }

    /**
     * Rehash the hashtable according to the (hinted) number of buckets
     * The bucket size after rehash need not be same as the parameter bucketSize
     * Instead, findMinimumBucketSize is called to get the correct number
     * firstBucketIt should be updated
     * Do nothing if the bucketSize doesn't change
     * Time Complexity: O(nk)
     * @param bucketSize lower bound of the new number of buckets
     */
    void rehash(size_t bucketSize) {
        bucketSize = findMinimumBucketSize(bucketSize);
        if (bucketSize == buckets.size()) return;
        // TODO: implement this function
        HashTableData htd(bucketSize); // create the table with new size
        //begin rehashing the elements
        for(auto &item:buckets){ //iterate through the buckets
            for(auto &elem:item){ // iterate the list element
                auto nit=hashKey(elem.first,bucketSize); //get the new key place
                auto &nvalue=htd[nit];
                nvalue.emplace_front(move(elem)); //place the new element
            }
        }
        buckets.swap(htd); //exchange the buckets to new hash table
        firstBucketIt=buckets.end(); //update the first_bucket_it of the new hash table
    }

    /**
     * @return the number of elements in the hashtable
     */
    size_t size() const { return tableSize; }

    /**
     * @return the number of buckets in the hashtable
     */
    size_t bucketSize() const { return buckets.size(); }

    /**
     * @return the current load factor of the hashtable
     */
    double loadFactor() const { return (double) tableSize / (double) buckets.size(); }

    /**
     * @return the maximum load factor of the hashtable
     */
    double getMaxLoadFactor() const { return maxLoadFactor; }

    /**
     * Set the max load factor
     * @throw std::range_error if the load factor is too small
     * @param loadFactor
     */
    void setMaxLoadFactor(double loadFactor) {
        if (loadFactor <= 1e-9) {
            throw std::range_error("invalid load factor!");
        }
        maxLoadFactor = loadFactor;
        rehash(buckets.size());
    }

};

