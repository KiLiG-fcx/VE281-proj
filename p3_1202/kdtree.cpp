//
// Created by lmyannie60 on 2020/11/24.
//

#include <tuple>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include "kdtree.hpp"
using namespace std;

int main(){
    std::tuple<int,int,int> t1(11,22,33);std::tuple<int,int,int> t2(8,2,55);
    std::tuple<int,int,int> t3(7,4,16);std::tuple<int,int,int> t4(9,5,23);
    std::tuple<int,int,int> t5(13,89,35);std::tuple<int,int,int> t6(65,13,25);
    std::tuple<int,int,int> t7(44,55,17);std::tuple<int,int,int> t8(0,1,88);
    std::tuple<int,int,int> t9(13,89,35);
    //std::vector<std::tuple<int,int,int>> v;
    pair<tuple<int,int,int>,int> p1(t1,2);pair<tuple<int,int,int>,int> p2(t2,14);
    pair<tuple<int,int,int>,int> p3(t3,9);pair<tuple<int,int,int>,int> p4(t4,3);
    pair<tuple<int,int,int>,int> p5(t5,1);pair<tuple<int,int,int>,int> p6(t6,17);
    pair<tuple<int,int,int>,int> p7(t7,7);pair<tuple<int,int,int>,int> p8(t8,18);
    pair<tuple<int,int,int>,int> p9(t9,1);
    vector<pair<tuple<int,int,int>,int>> v;

    v.push_back(p1);v.push_back(p2);
    v.push_back(p3);v.push_back(p4);
    v.push_back(p5);v.push_back(p6);
    v.push_back(p7);v.push_back(p8);
    KDTree<tuple<int,int,int>,int> kd;
    kd.insert(p1.first,p1.second);
    kd.insert(p2.first,p2.second);
    kd.insert(p3.first,p3.second);
    kd.insert(p4.first,p4.second);
    //kd.erase(t3);
    kd.insert(p7.first,p7.second);
    //cout<<kd.findMax<0>()->second;
    KDTree<tuple<int,int,int>,int> kd2(kd);
    cout<<kd2.size();

    cout<<kd.size();

}

