//
// Created by lmyannie60 on 2020/10/31.
//

#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <functional>
using namespace std;

template<typename T>
void nswap(T &i, T &j) { //swap the elements i and j
    T temp = i;
    i = j;
    j = temp;
}

template<typename T, typename Compare=std::less<T>>
void bubble_sort(std::vector<T> &vector, Compare comp = Compare()) {
    int count = 0; //used for vector size
    for (auto i = vector.begin(); i < vector.end(); i++) {
        count++;
    }
    for (auto i = 0; i < count - 1; i++) {
        for (auto j = count - 1; j > i; j--) { //bubble from the last element
            if (comp(vector[j], vector[j - 1])) { // if the latter one is smaller
                nswap(vector[j], vector[j - 1]); //swap the elements
            }
        }
    }
}

template<typename T, typename Compare=std::less<T>>
void insertion_sort(std::vector<T> &vector, Compare comp = Compare()) {
    int count = 0;
    for (auto i = vector.begin(); i < vector.end(); i++) {
        count++; // count the vector size
    }
    for (int i = 0; i < count; i++) {
        for (int j = i; (j > 0 && comp(vector[j], vector[j - 1])); j--) { // find the insertion place
            nswap(vector[j], vector[j - 1]);
        }
    }
}

template<typename T, typename Compare=std::less<T>>
void selection_sort(std::vector<T> &vector, Compare comp = Compare()) {
    int count = 0;
    for (auto i = vector.begin(); i < vector.end(); i++) {
        count++; //count the vector size
    }
    for (auto i = 0; i < count - 1; i++) {
        auto lowest = i; //assume the first one to be minimum
        for (auto j = count - 1; j > i; j--) { // find the minimum in later parts
            if (comp(vector[j], vector[lowest])) {
                lowest = j;
            }
        }
        nswap(vector[i], vector[lowest]);//swap the current one with the minimum
    }
}

template<typename T, typename Compare=std::less<T>>
void merges(std::vector<T> &vec, int left, int mid, int right, Compare comp = Compare()) {
    std::vector<T> nmerge;
    int i = left;
    int j = mid + 1;
    while (i <= mid && j <= right) { // when both are not exhausted
        if (comp(vec[j], vec[i])) { // if vj<vi
            nmerge.push_back(vec[j++]);
        } else { // use the i element and go to next
            nmerge.push_back(vec[i++]);
        }
    }
    while (i <= mid) { // used up j? fill up with i
        nmerge.push_back(vec[i++]);
    }
    while (j <= right) { //used up i, fill up with j
        nmerge.push_back(vec[j++]);
    }
    for (int l = left, k = 0; l <= right; k++, l++) { // put them back into the vec
        vec[l] = nmerge[k];
    }
}

template<typename T, typename Compare=std::less<T>>
void merge_help(std::vector<T> &vector, int left, int right, Compare comp = Compare()) {
    if (left >= right) return; // the recursive call
    int mid = (left + right) / 2;
    merge_help(vector, left, mid, comp); // sort the left half
    merge_help(vector, mid + 1, right, comp); //sort the right half
    merges(vector, left, mid, right, comp); // merge the two halves
}

template<typename T, typename Compare=std::less<T>>
void merge_sort(std::vector<T> &vector, Compare comp = Compare()) {
    int right = 0;
    for (auto i = vector.begin(); i < vector.end(); i++) {
        right++; // count the vector size
    }
    merge_help(vector, 0, right - 1, comp);
}

template<typename T, typename Compare=std::less<T>>
int extra_part(std::vector<T> &vec, int left, int right, Compare comp = Compare()) {
    std::vector<T> small, large; // two vectors for smaller and larger than pivot
    for (int i = left + 1; i <= right; i++) {
        if (comp(vec[i], vec[left])) {
            small.push_back(vec[i]); //push the small one into small vector
        } else {
            large.push_back(vec[i]); //put into large vector
        }
    }
    T temp = vec[left];
    int j = left;
    int smallsize = 0;
    int bigsize = 0; //count the size of 2 vectors
    for (auto i = small.begin(); i < small.end(); i++) {
        smallsize++;
    }
    for (int i = 0; i < smallsize; i++) { //put back into vec (small part)
        vec[j] = small[i];
        j++;
    }
    vec[j] = temp; // put back the pivot
    j++;
    for (auto i = large.begin(); i < large.end(); i++) {
        bigsize++;
    }
    for (int i = 0; i < bigsize; i++) { // put back large part
        vec[j] = large[i];
        j++;
    }
    return left + smallsize; // the next pivot number
}


template<typename T, typename Compare=std::less<T>>
void extra_help(std::vector<T> &vector, int left, int right, Compare comp = Compare()) {
    // //recursive call of extra space qs
    if (left >= right) return;
    int pivot_index = extra_part(vector, left, right, comp);
    extra_help(vector, left, pivot_index - 1, comp);
    extra_help(vector, pivot_index + 1, right, comp);
}

template<typename T, typename Compare=std::less<T>>
void quick_sort_extra(std::vector<T> &vector, Compare comp = Compare()) {
    int count = 0;
    for (auto i = vector.begin(); i < vector.end(); i++) {
        count++;//count the element number in vector
    }
    extra_help(vector, 0, count - 1, comp);
}

template<typename T, typename Compare=std::less<T>>
int partition(std::vector<T> &vector, int left, int right, T &pn, Compare comp = Compare()) {
    do {
        while (comp(vector[++left], pn)); // find the first element larger in the left
        while ((left < right) && comp(pn, vector[--right])); // and the smaller on right
        nswap(vector[left], vector[right]);//exchange
    } while (left < right); // the l and r exchanging partition
    return left;
}

int find_pivot(int left, int right) {
    return (left + right) / 2; //choose the middle element
}

template<typename T, typename Compare=std::less<T>>
void in_place_qs(std::vector<T> &vector, int left, int right, Compare comp = Compare()) {
    if (right <= left) return; //recursive call of in place qs
    int pivot_index = find_pivot(left, right);
    nswap(vector[pivot_index], vector[right]);//put the pivot to right
    int i = partition(vector, left - 1, right, vector[right], comp);//swapping partition
    nswap(vector[i], vector[right]);// put back the pivot
    in_place_qs(vector, left, i - 1, comp);
    in_place_qs(vector, i + 1, right, comp);
}

template<typename T, typename Compare=std::less<T>>
void quick_sort_inplace(std::vector<T> &vector, Compare comp = Compare()) {
    int count = 0;
    for (auto i = vector.begin(); i < vector.end(); i++) {
        count++; //count the vector element number
    }
    in_place_qs(vector, 0, count - 1, comp);
}

int main(){
    vector<double> time;
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(1, 200033);
    vector<int> time_case;
   /* for(int i=1;i<=100;i=i+9){
        time_case.push_back(i);
        cout<<i<<endl;
    }
    for(int i=101;i<=1000;i=i+50){
        time_case.push_back(i);
        cout<<i<<endl;
    }
    for(int i=1001;i<=10000;i=i+300){
        time_case.push_back(i);
        cout<<i<<endl;
    }
    for(int i=10001;i<=100000;i=i+5000){
        time_case.push_back(i);
        cout<<i<<endl;
    }*/
    /*
    for(int i=0;i<time_case.size();i++){
        int n=time_case[i];
        vector<int> test;
        test.reserve(n);
        for(int j=0;j<n;j++){
            test.push_back(distrib(gen));
        }
        auto start = std::chrono::steady_clock::now();
        bubble_sort(test);
        auto end = std::chrono::steady_clock::now();
        double dr_ns=std::chrono::duration<double,std::pico>(end-start).count();
        cout << "elapsed time: " << dr_ns << "s\n";
    }*/
    for(int i=1;i<=50;i++){
        cout<<i<<endl;
    }
}