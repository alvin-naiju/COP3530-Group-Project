#include <limits>
#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <array>
#include <queue>
#include <chrono>
using namespace std;
bool sortbysec(const pair<int,int> &a, const pair<int,int> &b);

class Graph
{
   private:
    //Adjacency List to hold graph data
    map<int, vector<pair<int,int>>> graph;
    map<int, vector<pair<int,int>>>::iterator it;

   public:
    void insertEdge(int from, int to, int weight);
    void printGraph();
    float getSize();
    vector<int> dijkstras(int source, int numVertices);
    //Greedy BFS
    pair<int, int> lowestAdjacent(int source, int count);
    vector<int> greedyBFS(int source, int numVertices, int target, bool backtrack = false);
    
};
void Graph::insertEdge(int from, int to, int weight)
{
    graph[from].push_back(make_pair(to,weight));
    if (graph.find(to)==graph.end())
        graph[to] = {};
}
void Graph::printGraph()
{
    for(it = graph.begin(); it != graph.end(); ++it)
    {
        cout << it->first<<"        ";
        for(int j = 0; j < it->second.size(); j++)
            cout << " " << it->second[j].first;
        cout<<"\n";
    }
}
float Graph::getSize()
{
    int count=0;
    for(it = graph.begin(); it != graph.end(); ++it)
        count++;
    return count;
}

int main()
{
    Graph adjList;
    //set size of graph and load data from file
    cout<<"------Welcome to our algorithm solver------"<<endl;
    cout<<"Please choose a graph size to search through"<<endl;
    cout<<"1. Small"<<endl;
    cout<<"2. Medium"<<endl;
    cout<<"3. Large"<<endl;
    cout<<"4. Very large"<<endl;
    cout<<"Enter your selection here: ";
    int size;
    cin>>size;
    cout<<endl;
    ifstream file;
    int numVertices;
    switch(size)
    {
        case(1):
            file.open("small.txt");
            numVertices = 10;
            break;
        case(2):
            file.open("medium.txt");
            numVertices = 1000;
            break;
        case(3):
            file.open("large.txt");
            numVertices = 10000;
            break;
        case(4):
            file.open("veryLarge.txt");
            numVertices = 100000;
            break;
        default:
            cout<<"Invalid input"<<endl;
    }
    //---------Parse Input-----------
    //read in data points for graph representation
    if(file.is_open())
    {
        string firstLine;
        getline(file, firstLine);
        
        
        for(int i=0; i<numVertices; i++)
        {
            //the first number in each line is the from, the following numbers are its adjacent vertices,
            //the weight is the value of each adjacent vertex
            string data;
            getline(file, data);
            
            //pull first number (from)
            stringstream s_stream(data);
            string substr;
            getline(s_stream, substr, ' ');
            int from = stoi(substr);
            
            //pull remainding numbers (to), store to adjacency list
            while(s_stream.good())
            {
                getline(s_stream, substr, ' ');
                int to = stoi(substr);
                //store each from to pair into adjacency list
                adjList.insertEdge(from,to,to);
            }
        }
    }
    else
        cout << "File did not open, please restart program" << endl;
    
    bool quit = false;
    while(!quit)
    {
        //----------Menu Options------------
        cout<<"What would you like to do?"<< endl;
        cout<<"1. Print a graph"<<endl;
        cout<<"2. Find shortest distance between 2 points on a graph using dijkstras algorthim"<<endl;
        cout<<"3. List the shortest distances from one point to all other points on a graph using dijkstras algorthim"<<endl;
        cout<<"4. Find shortest distance between 2 points on a graph using Greedy BFS"<<endl;
        cout<<"5. Quit program"<<endl;
        cout<<"Enter your selection here: ";
        int menu;
        cin >> menu;
        int v1;
        int v2;
        vector<int> dist;
        switch(menu)
        {
            case(1):
            {
                cout<<endl;
                cout<<"---Printing graph as an adjacency list---"<<endl;
                cout<<"Vertex    Adjacent vertices"<<endl;
                adjList.printGraph();
                cout<<endl;
                break;
            }
            case(2):
            {    //get user input
                cout<<endl<<"Enter 2 numbers between 0-"<< numVertices-1 <<" to find the distance between them"<<endl;
                cout<<"From vertex: ";
                cin >> v1;
                cout<<"To vertex: ";
                cin >> v2;
                
                //track time of dijsktras
                auto start =std::chrono::high_resolution_clock::now();
                dist = adjList.dijkstras(v1,numVertices);
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = duration_cast<std::chrono::microseconds>(stop - start);
                cout<<endl<<"Dijsktra's Algoritm took: "<< duration.count() << " microseconds."<<endl;
                
                //print distance
                if(dist[v2]<numeric_limits<int>::max())
                    cout<<"The distance from vertex "<<v1<<" to vertex "<<v2<<" is: "<< dist[v2]<<endl;
                else
                    cout<<"There is no path from vertex "<<v1<<" to vertex "<<v2<<endl;
                cout<<endl;
                break;
            }
            case(3):
            {
                //get user input
                cout<<endl<<"Enter a number between 0-"<< numVertices-1<<endl;
                cout<<"From vertex: ";
                cin >> v1;
                
                //track time of dijsktras
                auto start =std::chrono::high_resolution_clock::now();
                dist = adjList.dijkstras(v1,numVertices);
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = duration_cast<std::chrono::microseconds>(stop - start);
                cout<<endl<<"Dijsktra's Algoritm took: "<< duration.count() << " microseconds."<<endl;
                
                //print all distances
                cout<<"Vertex   Distance from source "<<v1<<endl;
                for(int i=0; i<numVertices;i++)
                {
                    if(dist[i]<numeric_limits<int>::max())
                        cout<< i <<"        "<< dist[i]<<endl;
                    else
                        cout<< i <<"        "<< "Not connected to source"<<endl;;
                }
                cout<<endl;
                break;
            }
            case(4):
            {
                //get user input
                cout<<endl<<"Enter 2 numbers between 0-"<< numVertices-1 <<" to find the distance between them"<<endl;
                cout<<"From vertex: ";
                cin >> v1;
                cout<<"To vertex: ";
                cin >> v2;
                
                //track time of dijsktras
                auto start =std::chrono::high_resolution_clock::now();
                vector<int> distGreedy = adjList.greedyBFS(v1, numVertices, v2);
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = duration_cast<std::chrono::microseconds>(stop - start);
                
                cout<<endl<<"Greedy BFS took: "<< duration.count() << " microseconds."<<endl;
                
                //print all distances
                if(distGreedy.size() == 0)
                {
                    cout<<"There is no path from vertex "<<v1<<" to vertex "<<v2<<endl;
                }
                else
                {
                    cout<<"Path from source to target "<<v1<<endl;
                    cout<<v1<<" ";
                    for(int i=0; i<distGreedy.size()-1;i++)
                    {
                        cout<< distGreedy.at(i) << " ";
                    }
                    cout<<v2;
                    cout<<" Distance: "<<distGreedy.at(distGreedy.size()-1);
                    cout<<endl;
                    
                }
                break;
            }
            case(5):
            {
                quit = true;
                break;
            }
            default:
            {
                cout <<endl<< "Invalid menu option"<<endl;
            }
        }
    }
    
    
}

//Dijkstra Algoritm
vector<int> Graph::dijkstras(int source, int numVertices)
{
    //set all distance to INF
    vector<int> distance;
    for(int i = 0; i<graph.size(); i++)
        distance.push_back(numeric_limits<int>::max());
    //set source dist to 0
    distance[source] = 0;
    
    //create priority queue
    priority_queue<pair<int, int>, vector< pair<int, int> >, greater<pair<int, int> > > pq;
    //push source onto pq
    pq.push(make_pair(source,0));
    
    //this ensures every vertex gets checked
    while(!pq.empty())
    {
        //get min vertex
        int u = pq.top().first;
        pq.pop();
        
        //check each of u's adjacent nodes
        for(int i=0; i<graph[u].size(); i++)
        {
            //adjacent node
            int v = graph[u][i].first;
            //adjacent nodes weight
            int weight = graph[u][i].second;
            
            //check if dist to v is greater than going through u
            if(distance[v] > distance[u] + weight)
            {
                //reset distance to v to the path through u
                distance[v] = distance[u] + weight;
                pq.push(make_pair(v,distance[v]));
            }
        }
    }
    return distance;
}

bool sortbysec(const pair<int,int> &a, const pair<int,int> &b) {
    return (a.second < b.second);
}

pair<int, int> Graph::lowestAdjacent(int source, int count) {
    vector<pair<int,int>> subject = graph[source];
    sort(subject.begin(), subject.end(), sortbysec);
   //make a pair to represent the node and weight
//   pair<int, int> lowest = make_pair(INT_MAX, INT_MAX);
   //iterate through all adjacent nodes
//   for(int i = 0; i < graph[source].size(); i++) {
//       //if the weight of the current node is less than the weight of the "lowest node"
//       if(graph[source][i].second < lowest.second && count == 0) {
//           //update the values
//           graph[source][i].first = lowest.first;
//           graph[source][i].second = lowest.second;
//       }
//       else if (count > 0) {
//           count--;
//       }
//       else {
//           cout << "Something went wrong with the count variable: " << count << endl;
//       }
//   }
   return subject[count];
}

//greedy best first search
vector<int> Graph::greedyBFS(int source, int numVertices, int target, bool backtrack) {
//    //vector to keep track of visited vertices
//    vector<bool> visited(numVertices, false);
   //create a string to store the path
   string path = "";
   //create an int to keep track of the distance
   int distance = 0;
   int count = 0;
   //do not create another recursive call if we have already visited this node or if the selected node is
   //the target node (occurs when there are cycles in the graph)
   //for (int i = 0; i < graph[source].size(); i++) {
    pair<int, int> previous;
    pair<int, int> current = lowestAdjacent(source, count);
       while(current.first != target) {
           if(backtrack == true) {
               count = 0;
               backtrack = false;
           }
           path += " " + to_string(current.first);
           distance += (current.second);
           if(graph[current.first].size() == 0) {
               count++;
               current = previous;
           }
           previous = current;
           if(previous == current) {
               backtrack = true;
           }
           current = lowestAdjacent(current.first, count);
           
       }
    distance += target;
   //}
   //finish the path and append the path distance to the end of the string
   path += " " + to_string(distance);
   stringstream iss(path);

   //turn the string of numbers into a vector of numbers
   int number;
   vector<int> pathDistance;
   while ( iss >> number ) {
       pathDistance.push_back(number);
   }
   return pathDistance;
}

