#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

struct point{ // a point (x,y)
    int x;
    int y;
};
/*
bool angle_comp(point &p0, point &p1, point &p2){
    int ccw=(p1.x-p0.x)*(p2.y-p0.y)-(p1.y-p0.y)*(p2.x-p0.x);
    return (ccw>0);
}*/

int distance(point &p1, point &p2){ // the distance between p1 and p2
    int diss=(p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y);
    return diss;
}

int ccwc(point &p0, point &p1, point &p2){ // calculate the ccw of p1 and p2
    int ccw=(p1.x-p0.x)*(p2.y-p0.y)-(p1.y-p0.y)*(p2.x-p0.x);
    return ccw;
}

void read_points(vector<point> &vec, int n){ // read the points from cin
    int x,y;
    for(int i=0;i<n;i++){
        point npt={};
        cin>>x>>y;
        npt.x=x;
        npt.y=y;
        vec.push_back(npt);// and push into vector
    }
}

bool cmp(const point &p1, const point &p2){ //compare to get P0
    if(p1.y!=p2.y){
        return (p1.y<p2.y);
    }
    return p1.x<p2.x; //then return the one with smaller x
}

point &choose_y(vector<point> &vec){
    vector<point> &sorted=vec; //choose the P0
    sort(sorted.begin(),sorted.end(),cmp);
    return sorted[0];
}

void swap(vector<point> &vec, int i, int j){
    point temp=vec[i]; //swap vec[i] and vec[j]
    vec[i]=vec[j];
    vec[j]=temp;
}

void pt_sort(vector<point> &vec, point &p0,int n){
   // int n=vec.size();
    for(int i=1;i<n;i++){
        for(int j=i;(j>0)&&ccwc(p0,vec[j],vec[j-1])>=0;j--){
            if(ccwc(p0,vec[j],vec[j-1])>0){// if they have different angle
                swap(vec,j,j-1);
            }
            else if(ccwc(p0,vec[j],vec[j-1])==0){//if they have same angle
                int dis1=distance(vec[j],p0);
                int dis2=distance(vec[j-1],p0);//compare the distance of two points
                if(dis2>dis1){ // put the one with larger distance at the back
                    swap(vec,j,j-1);
                }
            }
        }
    }
}

int del(vector<point> &vec,int n, point &p0){ //a bad function
   for(int i=1;i<n-1;i++){ //must keep the first element p0, so starting from vec[1]
       if(ccwc(p0,vec[i],vec[i+1])==0 ){ //i and i+1 are colinear
           int x=i; // should go over i again instead of the going to next
           for(int k=i;k<n-1;k++){
               vec[k]=vec[k+1];
           }
           n--; //substituted
           i=x-1;
       }
   }
    for(int i=1;i<n;i++){ //check for same elements
        if(vec[i].x==p0.x && vec[i].y==p0.y){
            int x=i;
            for(int k=i;k<n-1;k++){
                vec[k]=vec[k+1];
            }
            n--;
            i=x-1;//also should go over vec[i] again
        }
    }
   return n;
}

void convex(vector<point> &x, vector<point> &s, int n){
    for(int i=0;i<n;i++){ // for each element in x
        while(s.size()>1 && ccwc(*(s.end()-2),*(s.end()-1),x[i])<=0){
            s.pop_back();
        }
        s.push_back(x[i]);
    }
}

void out_print(vector<point> &vec){//print the result
    for(auto i=vec.begin();i<vec.end();i++) {
        cout << i->x << " " << i->y << endl;
    }
}

int main(){
    int num;
    cin>>num;
    if(num<=0){ //no point, return
        return 0;
    }
    vector<point> vec_p;
    read_points(vec_p,num);
    point p0=choose_y(vec_p);
    pt_sort(vec_p,p0,num);
    int n=del(vec_p,num,p0); // the actual number of points we need to deal with
    vector<point> s;
    convex(vec_p,s,n);
    out_print(s);
    return 0;
}