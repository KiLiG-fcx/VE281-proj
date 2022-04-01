//
// Created by lmyannie60 on 2020/11/24.
//

#include <tuple>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <stdexcept>
using namespace std;

/**
 * An abstract template base of the KDTree class
 */
template<typename...>
class KDTree;

/**
 * A partial template specialization of the KDTree class
 * The time complexity of functions are based on n and k
 * n is the size of the KDTree
 * k is the number of dimensions
 * @typedef Key         key type
 * @typedef Value       value type
 * @typedef Data        key-value pair
 * @static  KeySize     k (number of dimensions)
 */
template<typename ValueType, typename... KeyTypes>
class KDTree<std::tuple<KeyTypes...>, ValueType> {
public:
    typedef std::tuple<KeyTypes...> Key;
    typedef ValueType Value;
    typedef std::pair<const Key, Value> Data;
    static inline constexpr size_t KeySize = std::tuple_size<Key>::value;
    static_assert(KeySize > 0, "Can not construct KDTree with zero dimension");
protected:
    struct Node {
        Data data;
        Node *parent;
        Node *left = nullptr;
        Node *right = nullptr;

        Node(const Key &key, const Value &value, Node *parent) : data(key, value), parent(parent) {}

        const Key &key() { return data.first; }

        Value &value() { return data.second; }
    };

public:
    /**
     * A bi-directional iterator for the KDTree
     * ! ONLY NEED TO MODIFY increment and decrement !
     */
    class Iterator {
    private:
        KDTree *tree;
        Node *node;

        Iterator(KDTree *tree, Node *node) : tree(tree), node(node) {}

        /**
         * Increment the iterator
         * Time complexity: O(log n)
         */
        void increment() {
            // TODO: implement this function
            //cout<<"calling increment"<<endl;
//            if(node->right){
//                Node *tempnode=node->right;
//                while(tempnode->left){
//                    tempnode=tempnode->left;
//                }
//                node=tempnode;
//            }
//            else{
//                Node *tempnode2=node;
//                Node *tempnode1=node->parent;
//                while(tempnode1){
//                    if(tempnode1->left==tempnode2){
//                        node=tempnode1;
//                        return;
//                    }
//                    tempnode2=tempnode1;
//                    tempnode1=tempnode1->parent;
//                }
//                this->node= nullptr;
//            }
            if(node->right){
                node=node->right;
                while(node->left)
                    node=node->left;
            }
            else{
                while(node->parent && node->parent->left!=node)
                    node=node->parent;
                if(node->parent== nullptr)
                    node= nullptr;
                else
                    node=node->parent;
            }

        }

        /**
         * Decrement the iterator
         * Time complexity: O(log n)
         */
        void decrement() {
            // TODO: implement this function
            //cout<<"calling decrement"<<endl;
            if(node->left){
                Node *tempnode=node->left;
                while(tempnode->right){
                    tempnode=tempnode->right;
                }
                node=tempnode;
            }
            else{
                Node *tempnode2=node;
                Node *tempnode1=node->parent;
                while(tempnode1){
                    if(tempnode1->right==tempnode2){
                        node=tempnode1;
                        return;
                    }
                    tempnode2=tempnode1;
                    tempnode1=tempnode1->parent;
                }
                node= nullptr;
            }
        }

    public:
        friend class KDTree;

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

        Iterator &operator--() {
            decrement();
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            decrement();
            return temp;
        }

        bool operator==(const Iterator &that) const {
            return node == that.node;
        }

        bool operator!=(const Iterator &that) const {
            return node != that.node;
        }

        Data *operator->() {
            return &(node->data);
        }

        Data &operator*() {
            return node->data;
        }
    };

protected:                      // DO NOT USE private HERE!
    Node *root = nullptr;       // root of the tree
    size_t treeSize = 0;        // size of the tree

    /**
     * Find the node with key
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param node
     * @return the node with key, or nullptr if not found
     */
    template<size_t DIM>
    Node *find(const Key &key, Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if(node== nullptr){
            return nullptr;
        }
        if(node->key()==key){
            return node;
        }
        if(compareKey<DIM>(key,node->key(),std::less<>())){
            //DIM=DIM_NEXT;
            return find<DIM_NEXT>(key,node->left);
        }
        else{
            return find<DIM_NEXT>(key,node->right);
        }
    }

    /**
     * Insert the key-value pair, if the key already exists, replace the value only
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param value
     * @param node
     * @param parent
     * @return whether insertion took place (return false if the key already exists)
     */
    template<size_t DIM>
    bool insert(const Key &key, const Value &value, Node *&node, Node *parent) {
    /*    constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        //cout<<"calling insert"<<get<0>(key)<<" "<<get<1>(key)<<" "<<value<<endl;
        if(node== nullptr){
//            cout<<"case 1"<<endl;
            Node *newnode=new Node(key,value,parent);
            node=newnode;
            treeSize++;

            return true;
        }
        if(node->key()==key){
            node->value()=value;
            return false;
        }
        //cout<<"compare: "<<get<DIM>(key)<<"&"<<get<DIM>(node->key())<<endl;
        if(compareKey<DIM>(key, node->key(), std::less<>()) && get<DIM>(key)!=get<DIM>(node->key())){
            //cout<<"insert to left"<<endl;
            return insert<DIM_NEXT>(key,value,node->left,node);
        }
        else{
            //cout<<"insert to right"<<endl;
            return insert<DIM_NEXT>(key,value,node->right,node);
        }*/
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if(node== nullptr){
            node=new Node(key,value,parent); //inserted
            treeSize++;
            //std::cout<<node->value()<<"\n";
            return true;
        }
        if(key==node->key()){
            node->data.second=value; //only update the value
            return false;
        }
        if(compareKey<DIM>(key,node->key(),std::less<>())&& std::get<DIM>(key)!=std::get<DIM>(node->key())){
            return insert<DIM_NEXT>(key,value,node->left,node);
        }
        else{
            return insert<DIM_NEXT>(key,value,node->right,node);
        }
    }

    /**
     * Compare two keys on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return relationship of two keys on a dimension with the compare function
     */
    template<size_t DIM, typename Compare>
    static bool compareKey(const Key &a, const Key &b, Compare compare = Compare()) {
        if (std::get<DIM>(a) != std::get<DIM>(b)) {
            return compare(std::get<DIM>(a), std::get<DIM>(b));
        }
        return compare(a, b);
    }

    /**
     * Compare two nodes on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return the minimum / maximum of two nodes
     */
    template<size_t DIM, typename Compare>
    static Node *compareNode(Node *a, Node *b, Compare compare = Compare()) {
        if (!a) return b;
        if (!b) return a;
        return compareKey<DIM, Compare>(a->key(), b->key(), compare) ? a : b;
    }

    /**
     * Find the minimum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the minimum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node *findMin(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if(!node) return nullptr;
        Node *min=findMin<DIM_CMP,DIM_NEXT>(node->left); //find the minimum in the left subtree
        if(DIM_CMP!=DIM){
            Node *right_min=findMin<DIM_CMP,DIM_NEXT>(node->right); //find the minimum in the right
            min=compareNode<DIM_CMP,std::less<>>(min,right_min);
        }
        return compareNode<DIM_CMP,std::less<>>(min,node);
    }

    /**
     * Find the maximum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the maximum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node *findMax(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if(!node) return nullptr;
        Node *max=findMax<DIM_CMP,DIM_NEXT>(node->right);
        if(DIM_CMP!=DIM){
            Node *left_max=findMax<DIM_CMP,DIM_NEXT>(node->left);
            max=compareNode<DIM_CMP,std::greater<>>(max,left_max);
        }
        return compareNode<DIM_CMP,std::greater<>>(max,node);
    }

    template<size_t DIM>
    Node *findMinDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMin<DIM, 0>(root);
        return findMinDynamic<DIM_NEXT>(dim);
    }

    template<size_t DIM>
    Node *findMaxDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMax<DIM, 0>(root);
        return findMaxDynamic<DIM_NEXT>(dim);
    }

    /**
     * Erase a node with key (check the pseudocode in project description)
     * Time Complexity: max{O(k log n), O(findMin)}
     * @tparam DIM current dimension of node
     * @param node
     * @param key
     * @return nullptr if node is erased, else the (probably) replaced node
     */
    template<size_t DIM>
    Node *erase(Node *node, const Key &key) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if(!node)
            return nullptr;
        if(key==node->key()){
            if((!node->left)&&(!node->right)){
                if(node->parent->left==node)
                    node->parent->left= nullptr;
                else
                    node->parent->right= nullptr;
                delete node;
                treeSize--;
                return nullptr;
            }
            else if(node->right){
//                cout<<"right"<<endl;
                Node* minnode=findMin<DIM,DIM_NEXT>(node->right);
                Key &temp1= const_cast<Key &>(node->data.first);
                temp1=minnode->key();
//                cout<<get<0>(node->data.first)<<" "<<get<1>(node->data.first)<<endl;
                node->data.second=minnode->value();
                node->right=erase<DIM_NEXT>(node->right,minnode->key());
            }
            else if(node->left){
                Node *maxnode=findMax<DIM,DIM_NEXT>(node->left);
                Key &temp2= const_cast<Key &>(node->data.first);
                temp2=maxnode->key();
                node->data.second=maxnode->value();
                node->left=erase<DIM_NEXT>(node->left,maxnode->key());
            }
        }
        else{
            if(compareKey<DIM>(key, node->key(), std::less<>()))
                node->left=erase<DIM_NEXT>(node->left,key);
            else
                node->right=erase<DIM_NEXT>(node->right,key);
        }
        return node;
    }

    template<size_t DIM>
    Node *eraseDynamic(Node *node, size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return erase<DIM>(node, node->key());
        return eraseDynamic<DIM_NEXT>(node, dim);
    }

    // TODO: define your helper functions here if necessary

    template<size_t DIM>
    static bool datacmp(const std::pair<Key,Value> &dt1, const std::pair<Key,Value> &dt2){
        return compareKey<DIM,std::less<>>(dt1.first,dt2.first);
    }

    static bool equalkey(Data data1,Data data2){
        if(data1.first==data2.first)
            return true;
        else
            return false;
    }

    template<size_t DIM>
    void initialhelper(std::vector<std::pair<Key, Value>> data,Node* &node, Node* parent){
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        std::nth_element(data.begin(),data.begin()+(data.size()-1)/2,data.end(),datacmp<DIM>);
        if(data.empty())
            return;
        insert<DIM>(data[(data.size()-1)/2].first,data[(0+data.size()-1)/2].second,node,parent);
        if(data.size()>1){
            std::vector<std::pair<Key, Value>> left(data.begin(),data.begin()+(data.size()-1)/2);
            if(!left.empty()){
                initialhelper<DIM_NEXT>(left,node->left,node);
            }
            std::vector<std::pair<Key, Value>> right(data.begin()+(data.size()-1)/2+1,data.end());
            if(!right.empty()){
                initialhelper<DIM_NEXT>(right,node->right,node);
            }
        }
    }


    template<size_t DIM>
    void copyconstructhelper(Node* copynode,Node* &node,Node *parent){

        insert<DIM>(copynode->key(),copynode->value(),node,parent);
        if(copynode->left)
            copyconstructhelper<(DIM+1)%KeySize>(copynode->left,node->left,node);
        if(copynode->right)
            copyconstructhelper<(DIM+1)%KeySize>(copynode->right,node->right,node);



    }

    template<size_t DIM>
    void destroy(Node *&node){
        if(!node) return;
        if(node->left){
            return destroy<(DIM+1)%KeySize>(node->left);
        }
        if(node->right){
            return destroy<(DIM+1)%KeySize>(node->right);
        }
        erase<DIM>(root,node->key());
    }




public:
    KDTree() = default;
    /**
     * Time complexity: O(kn log n)
     * @param v we pass by value here because v need to be modified
     */
    explicit KDTree(std::vector<std::pair<Key, Value>> v) {
        // TODO: implement this function
        std::stable_sort(v.begin(),v.end(),datacmp<0>);

        auto it=std::unique(v.rbegin(),v.rend(),equalkey);

        v.erase(v.begin(),it.base());

        initialhelper<0>(v,root, nullptr);
    }

    /**
     * Time complexity: O(n)
     */
    KDTree(const KDTree &that) {
        // TODO: implement this function
        //root= nullptr;
        //std::cout<<"copy tree";
        copyconstructhelper<0>(that.root,root, nullptr);
    }

    /**
     * Time complexity: O(n)
     */
    KDTree &operator=(const KDTree &that) {
        // TODO: implement this function
        //root= nullptr;
        if(this==&that){
            return *this;
        }
        if(root)
            destroy<0>(this->root);
        copyconstructhelper<0>(that.root,root, nullptr);
        return *this;
    }

    /**
     * Time complexity: O(n)
     */
    ~KDTree() {
        // TODO: implement this function
        if(root){
            destroy<0>(this->root);
        }
    }

    Iterator begin() {
        if (!root) return end();
        auto node = root;
        while (node->left) node = node->left;
        return Iterator(this, node);
    }

    Iterator end() {
        return Iterator(this, nullptr);
    }

    Iterator find(const Key &key) {
        return Iterator(this, find<0>(key, root));
    }

    void insert(const Key &key, const Value &value) {
        insert<0>(key, value, root, nullptr);
    }

    template<size_t DIM>
    Iterator findMin() {
        return Iterator(this, findMin<DIM, 0>(root));
    }

    Iterator findMin(size_t dim) {
        return Iterator(this, findMinDynamic<0>(dim));
    }

    template<size_t DIM>
    Iterator findMax() {
        return Iterator(this, findMax<DIM, 0>(root));
    }

    Iterator findMax(size_t dim) {
        return Iterator(this, findMaxDynamic<0>(dim));
    }

    bool erase(const Key &key) {
        auto prevSize = treeSize;
        erase<0>(root, key);
        return prevSize > treeSize;
    }

    Iterator erase(Iterator it) {
        if (it == end()) return it;
        auto node = it.node;
        if (!it.node->left && !it.node->right) {
            it.node = it.node->parent;
        }
        size_t depth = 0;
        auto temp = node->parent;
        while (temp) {
            temp = temp->parent;
            ++depth;
        }
        eraseDynamic<0>(node, depth % KeySize);
        return it;
    }

    size_t size() const { return treeSize; }
};
