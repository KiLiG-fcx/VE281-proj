//
// Created by lmyannie60 on 2020/12/4.
//
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int main(){
    vector <int> v;
    v.push_back(1);
    v.push_back(34);
    //v.push_back(0);

    v.push_back(999);
    v.push_back(27);
    v.push_back(93);
    v.push_back(34);
    v.push_back(8);
    v.push_back(8);
    v.push_back(57);
    stable_sort(v.begin(),v.end(),std::less<>());
    auto it=unique(v.begin(),v.end());
    int count=-1;
    for(auto i=v.begin();i<it;i++){
        count++;
    }
    for(auto i=v.begin();i<it;i++){
        cout<<*i<<endl;
    }
    std::vector<int> vec;
    for(auto i=v.begin();i<it;i++){
        vec.push_back(*i);
    }
    cout<<vec.size()<<endl;
    nth_element(v.begin(),v.begin()+(v.size()-1)/2,v.end());
    cout<<vec[(vec.size()-1)/2];
    //cout<<v.size();
    //cout<<count;
}
