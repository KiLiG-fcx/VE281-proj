#include <iostream>
#include <queue>
#include <sstream>

using namespace std;

class Node {
public:
    unsigned int index;
    unsigned int distance;
    //int predecessor;
    bool visited;
    Node *next;
    pair<unsigned int, unsigned int> edge;

    bool operator==(Node node) {
        if (index == node.index && distance == node.distance && next == node.next && edge.first == node.edge.first &&
            edge.second == node.edge.second)
            return true;
    };


};

struct cmp1 {
    bool operator()(Node a, Node b) {
        return a.distance >= b.distance;
    }
};

//struct cmp2 {
//    bool operator()(Node a, Node b) {
//        return a.index >= b.index;
//    }
//};

class Graph {

public:
    Node **nodearray;
    unsigned int nodenumber;

public:
    Graph() = default;

    Graph(unsigned int nodenum, vector<vector<int>> array, unsigned int infinite, unsigned int edgenum) {
        nodenumber = nodenum;
        Node **Nodearray = new Node *[nodenum];
        for (unsigned int i = 0; i < nodenum; i++) {
            Nodearray[i] = nullptr;
        }
        nodearray = Nodearray;
        for (unsigned int i = 0; i < edgenum; i++) {
            //cout << i << endl;
            if (Nodearray[array[i][0]] == nullptr) {
                //cout<<"case 1"<<endl;
                Nodearray[array[i][0]] = new Node;
                Nodearray[array[i][0]]->index = array[i][1];
                Nodearray[array[i][0]]->edge = make_pair(array[i][0], array[i][2]);
                Nodearray[array[i][0]]->next = nullptr;
                //Nodearray[array[i][0]]->predecessor = -1;
                Nodearray[array[i][0]]->visited = false;
                Nodearray[array[i][0]]->distance = infinite;
            } else {
                //cout<<"case 2"<<endl;
                Node *currentnode = Nodearray[array[i][0]];
                while (currentnode->next != nullptr) {
                    currentnode = currentnode->next;
                }

                currentnode->next = new Node;
                currentnode->next->index = array[i][1];
                currentnode->next->edge = make_pair(array[i][0], array[i][2]);
                currentnode->next->next = nullptr;
                //currentnode->predecessor = -1;
                currentnode->next->visited = false;
                currentnode->next->distance = infinite;

            }
        }
    }

    void Shortestpath(unsigned int infinite, unsigned int start, unsigned int end) {
        //int totaldistance = 0;
        priority_queue<Node, vector<Node>, cmp1> unexplored;
        //Node explored[nodenumber];
        vector<Node> explored(nodenumber);
        for (unsigned int i = 0; i < nodenumber; i++) {
            explored[i].visited = false;
            explored[i].distance = infinite;
        }

        //cout<<"start: "<<start<<endl;
        for (unsigned int i = 0; i < nodenumber; i++) {
            if(nodearray[i]){
                Node *currentnode = nodearray[i];
                while (currentnode != nullptr) {
                    if (currentnode->index == start) {
                        //cout<<"set zero: "<<currentnode->index<<endl;
                        currentnode->distance = 0;
                    }
                    unexplored.push(*currentnode);
                    currentnode = currentnode->next;
                }
            }
        }
        Node sourcenode;
        sourcenode.distance=0;
        sourcenode.index=start;
        unexplored.push(sourcenode);


//        if(unexplored.empty())
//            cout<<"empty"<<endl;
//        else
//            cout<<unexplored.top().index<<endl;


        while (!unexplored.empty()) {
            if (!explored[unexplored.top().index].visited) {
                for (unsigned int i = 0; i < nodenumber; i++) {
                    Node *currentnode = nodearray[i];
                    if (i == unexplored.top().index) {
                        while (currentnode != nullptr) {
                            //cout << "currentnode " << currentnode->index << " old distance: " << currentnode->distance
                                 //<< " new distance: " << (currentnode->edge.second + unexplored.top().distance) << endl;
                            if (currentnode->distance > (currentnode->edge.second + unexplored.top().distance)) {
                                currentnode->distance = (currentnode->edge.second + unexplored.top().distance);
                                //currentnode->predecessor = unexplored.top().index;
                                unexplored.push(*currentnode);
                            }
                            currentnode = currentnode->next;
                        }
                    }

                }
                explored[unexplored.top().index].distance = unexplored.top().distance;
                explored[unexplored.top().index].visited = true;

            }
            unexplored.pop();
        }

        if (explored[end].distance == infinite) {
            cout << "No path exists!" << endl;
        } else {
            cout << "Shortest path length is " << explored[end].distance << endl;
        }


    }


    void DAG() {
        //int indegreetable[nodenumber];
        vector<int> indegreetable(nodenumber);
        for(unsigned int i=0;i<nodenumber;i++){
            indegreetable[i]=0;
        }
        for (unsigned int i = 0; i < nodenumber; i++) {
            Node *current = nodearray[i];
            while (current) {
                indegreetable[current->index]++;
                current = current->next;
            }
        }
//        cout<<"print indegreetable: ";
//        for(unsigned int i=0;i<nodenumber;i++){
//            cout<<indegreetable[i]<<" ";
//        }
//        cout<<endl;
        queue<int> unvisited;
        //int visited[nodenumber];
        vector<int> visited(nodenumber);
        for(unsigned int i=0;i<nodenumber;i++)
            visited[i]=0;
        for (unsigned int i = 0; i < nodenumber; i++) {
            if (indegreetable[i] == 0) {
                unvisited.push(i);
            }
        }
        while (!unvisited.empty()) {
            Node *current = nodearray[unvisited.front()];
            while (current) {
                //cout<<"updata: "<<current->index<<" ";
                //cout<<endl;
                indegreetable[current->index]--;
                current = current->next;
            }
//            if (visited[unvisited.front()] == 1) {
//                cout<<"unvisited.froint "<<unvisited.front()<<endl;
//                cout << "The graph is not a DAG" << endl;
//                return;
//            } else
                visited[unvisited.front()] = 1;
            unvisited.pop();
            for (unsigned int i = 0; i < nodenumber; i++) {
                if (indegreetable[i] == 0 && visited[i]==0) {
                    //cout<<"push: "<<i<<endl;
                    unvisited.push(i);
                }
            }
        }
        for(unsigned int i=0;i<nodenumber;i++){
            if(visited[i]==0){
                cout<<"The graph is not a DAG"<<endl;
                return;
            }
        }
        cout << "The graph is a DAG" << endl;

    }

    void transfer(unsigned int infinite){
        //cout<<"nodenumber: "<<nodenumber<<endl;
        for(unsigned int i=0;i<nodenumber ;i++){
            Node* current=nodearray[i];
            //cout<<"current "<<current->index<<endl;
            while(current){
                if(!nodearray[current->index]){
                    //cout<<"case: not exist"<<endl;
                    nodearray[current->index]=new Node;
                    nodearray[current->index]->index=i;
                    nodearray[current->index]->next= nullptr;
                    nodearray[current->index]->edge=make_pair(current->index,current->edge.second);
                    nodearray[current->index]->visited= false;
                    nodearray[current->index]->distance=infinite;
                }
                else{
                    Node* temp=nodearray[current->index];
                    //cout<<"temp "<<temp->index<<endl;
                    while(temp->next){
                        if(temp->index!=i)
                            temp=temp->next;
                        else{
                            if(temp->edge.second!=current->edge.second){
                                temp->edge.second=(temp->edge.second<current->edge.second)? temp->edge.second:current->edge.second;
                            }
                            break;
                        }
                    }
                    if(!temp->next && temp->index!=i){
                        //cout<<"case: not exist"<<endl;
                        temp->next=new Node;
                        temp->next->index=i;
                        temp->next->next= nullptr;
                        temp->next->edge=make_pair(current->index,current->edge.second);
                        temp->next->visited= false;
                        temp->next->distance=infinite;
                    }
                }
//                cout<<current->index<<endl;
//                if(current->next){
//                    cout<<"have next"<<endl;
//                    cout<<current->next->index<<endl;
//                }
                current=current->next;
            }
        }
        //cout<<"-----------finish transfer-------------"<<endl;
//        for (int i = 0; i < nodenumber; i++) {
//            if (nodearray[i] != nullptr) {
//                Node *current = nodearray[i];
//                while (current != nullptr) {
//                    cout << i << ": " << current->index << " ";
//                    current = current->next;
//                }
//            }
//            cout << endl;
//        }
        //cout << "--------------------------" << endl;
    }

    void MST(unsigned int infinite) {
        transfer(infinite);
        int totalweight = 0;
        priority_queue<Node, vector<Node>, cmp1> unexplored;
        //int visited[nodenumber];
        vector<int> visited(nodenumber);
        for(unsigned int i=0;i<nodenumber;i++)
            visited[i]=0;
        for(unsigned int i=0;i<nodenumber;i++){
            if(nodearray[i]){
                nodearray[i]->distance = 0;
                break;
            }
        }
        for (unsigned int i = 0; i < nodenumber; i++) {
            Node *currentnode = nodearray[i];
            while (currentnode) {
                unexplored.push(*currentnode);
                currentnode=currentnode->next;
            }
        }
        while (!unexplored.empty()) {
            if (visited[unexplored.top().index] == 0) {
                //cout<<"unexplored.top: "<<unexplored.top().index<<endl;
                visited[unexplored.top().index] = 1;
                //cout<<"totalweight add "<<totalweight<<"+"<<unexplored.top().distance<<endl;
                totalweight+=unexplored.top().distance;
                Node *current = nodearray[unexplored.top().index];
                unexplored.pop();
                while (current) {
                    //cout<<"current: "<<current->index<<" distance "<<current->distance<<" edge "<<current->edge.second<<endl;
                    if (current->distance > current->edge.second) {
                        current->distance = current->edge.second;
                        unexplored.push(*current);
                        //cout<<"unexplored top "<<unexplored.top().index<<" whether visit "<<visited[unexplored.top().index]<<endl;

                    }
                    current = current->next;
                }
            } else
                unexplored.pop();

        }
        for(unsigned int i=0;i<nodenumber;i++){
            if(visited[i]==0){
                cout<<"No MST exists!"<<endl;
                return;
            }
        }
        cout<<"The total weight of MST is "<<totalweight<<endl;

    }


};

int calculinf(vector<vector<int>> nodearray, int nodenum) {
    int inf = 0;
    for (int i = 0; i < nodenum; i++) {
        inf += nodearray[i][2];
    }
    return inf;
}

int main() {

    unsigned int nodenum, infinite;
    unsigned int start, end;
    cin >> nodenum >> start >> end;
    unsigned int edgenum=0;
    string line;
    istringstream iStream;

//    string a="123 45 6";
//    string b="78 9 10";
//    string c=a+b;
//    cout<<c<<endl;
//    iStream.str(c);
//    int d;
//    while(iStream){
//        iStream>>d;
//        cout<<d<<" ";
//    }
//    cout<<"---------ex----------"<<endl;


    string blank=" ";
    string totalline;
    while(getline(cin, line)){
        //cout<<"line: "<<line<<endl;
        edgenum++;
        //iStream.str(blank);
        totalline+=line+blank;
    }
    edgenum--;
//    cout<<"total line: "<<totalline;
//    cout<<"edgenum: "<<edgenum<<endl;


    //int nodearray[edgenum][3];
    vector<vector<int>> nodearray(edgenum,vector<int>(3));
    unsigned int i=0;
    iStream.str(totalline);
    while(iStream){
        for(int j=0;j<3;j++){
            iStream>>nodearray[i][j];
        }
        i++;
    }

//    cout<<"-----------print nodearray------------"<<endl;
//    for (i = 0; i < edgenum; i++) {
//        for (int j = 0; j < 3; j++) {
//            cout << nodearray[i][j]<<" ";
//        }
//        cout<<endl;
//    }
    //cout<<"-------------------------------------"<<endl;
    infinite = calculinf(nodearray, nodenum);
//    cout<<"infinite: "<<infinite<<endl;

    Graph graph(nodenum, nodearray, infinite,edgenum);
//    if(graph.nodearray[0]!= nullptr){
//        cout<<"0 not empty"<<endl;
//        cout<<graph.nodearray[0]->index<<endl;
//    }
//    if(graph.nodearray[1]!= nullptr){
//        cout<<"1 not empty"<<endl;
//        cout<<graph.nodearray[1]->index<<endl;
//    }
//    if(graph.nodearray[1]->next!= nullptr){
//        cout<<"1's next not empty"<<endl;
//        cout<<graph.nodearray[1]->next->index<<endl;
//    }
//    if(graph.nodearray[2]!= nullptr){
//        cout<<"2 not empty"<<endl;
//        cout<<graph.nodearray[2]->index<<endl;
//    }
//    if(graph.nodearray[3]!= nullptr){
//        cout<<"3 not empty"<<endl;
//        cout<<graph.nodearray[3]->index<<endl;
//    }




//    cout << "-----------print graph----------" << endl;
//    for (unsigned int i = 0; i < nodenum; i++) {
//        if (graph.nodearray[i] != nullptr) {
//            Node *current = graph.nodearray[i];
//            while (current != nullptr) {
//                cout << i << ": " << current->index << " ";
//                current = current->next;
//            }
//        }
//        cout << endl;
//    }
//    cout << "--------------------------" << endl;

    graph.Shortestpath(infinite, start, end);
    graph.DAG();
    //graph.transfer(infinite);
    graph.MST(infinite);



//    cout<<nodenum<<" "<<start<<" "<<end<<endl;
//    for(int i=0;i<nodenum;i++){
//        for(int j=0;j<3;j++){
//            cout<<nodearray[i][j]<<" ";
//        }
//        cout<<endl;
//    }


    return 0;
}