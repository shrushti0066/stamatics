#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <iomanip>

using namespace std;

const int NUM_CITIES = 500;
const int AVG_DEGREE = 3;

vector<vector<pair<int,int>>> graph;

void generateGraph()
{
    graph.resize(NUM_CITIES);

    int targetEdges = (NUM_CITIES * AVG_DEGREE) / 2;

    set<pair<int,int>> edges;

    srand(time(0));

    while ((int)edges.size() < targetEdges)
    {
        int u = rand() % NUM_CITIES;
        int v = rand() % NUM_CITIES;

        if (u == v)
            continue;

        if (u > v)
            swap(u, v);

        if (edges.count({u, v}))
            continue;

        edges.insert({u, v});

        int weight = 5 + rand() % 46; // 5-50

        graph[u].push_back({v, weight});
        graph[v].push_back({u, weight});
    }

    cout << "\n========== TASK A ==========\n";
    cout << "Network Generated Successfully\n";
    cout << "Total Cities (Nodes): " << NUM_CITIES << endl;
    cout << "Total Roads (Edges): " << edges.size() << endl;

    double avgDeg =
        (2.0 * edges.size()) / NUM_CITIES;

    cout << fixed << setprecision(2);
    cout << "Average Degree: " << avgDeg << endl;

    int maxDegree = 0;
    int minDegree = INT_MAX;

    for(int i = 0; i < NUM_CITIES; i++)
    {
        int deg = graph[i].size();

        maxDegree = max(maxDegree, deg);
        minDegree = min(minDegree, deg);
    }

    cout << "Minimum Degree: " << minDegree << endl;
    cout << "Maximum Degree: " << maxDegree << endl;
}

void saveAdjacencyList()
{
    ofstream fout("adjacency_list.txt");

    for(int i = 0; i < NUM_CITIES; i++)
    {
        fout << "City_" << i << " = [";

        for(int j = 0; j < graph[i].size(); j++)
        {
            fout << "("
                 << graph[i][j].first
                 << ","
                 << graph[i][j].second
                 << ")";

            if(j != graph[i].size()-1)
                fout << ", ";
        }

        fout << "]\n";
    }

    fout.close();

    cout << "\nAdjacency list saved to adjacency_list.txt\n";
}

vector<int> BFS(int start,
                vector<bool>& visited)
{
    queue<int> q;
    vector<int> component;

    q.push(start);
    visited[start] = true;

    while(!q.empty())
    {
        int u = q.front();
        q.pop();

        component.push_back(u);

        for(auto &edge : graph[u])
        {
            int v = edge.first;

            if(!visited[v])
            {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    return component;
}

void analyzeComponents()
{
    vector<bool> visited(NUM_CITIES, false);

    vector<vector<int>> components;

    for(int i = 0; i < NUM_CITIES; i++)
    {
        if(!visited[i])
        {
            components.push_back(
                BFS(i, visited)
            );
        }
    }


    cout << "\n========== TASK B ==========\n";

    int largestIndex = 0;

    for(int i = 1; i < components.size(); i++)
    {
        if(components[i].size() >
           components[largestIndex].size())
        {
            largestIndex = i;
        }
    }

    cout << "Largest Connected Component Size: "
         << components[largestIndex].size()
         << endl;

    cout << "Cities in Largest Component:\n";

    for(int city : components[largestIndex])
    {
        cout << city << " ";
    }

    cout << "\n";


    cout << "\n========== TASK C ==========\n";

    cout << "Total Connected Components: "
         << components.size()
         << endl;

    int largestCluster = 0;
    int smallestCluster = INT_MAX;

    for(int i = 0; i < components.size(); i++)
    {
        int size = components[i].size();

        largestCluster =
            max(largestCluster, size);

        smallestCluster =
            min(smallestCluster, size);

        cout << "Component "
             << i + 1
             << " Size = "
             << size
             << endl;
    }

    cout << "\nLargest Cluster Size: "
         << largestCluster
         << endl;

    cout << "Smallest Cluster Size: "
         << smallestCluster
         << endl;
}

void dijkstra(int source)
{
    vector<int> dist(NUM_CITIES, INT_MAX);

    priority_queue<
        pair<int,int>,
        vector<pair<int,int>>,
        greater<pair<int,int>>
    > pq;

    dist[source] = 0;

    pq.push({0, source});

    while(!pq.empty())
    {
        auto current = pq.top();
        pq.pop();

        int d = current.first;
        int u = current.second;

        if(d > dist[u])
            continue;

        for(auto &edge : graph[u])
        {
            int v = edge.first;
            int w = edge.second;

            if(dist[u] != INT_MAX &&
               dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;

                pq.push({dist[v], v});
            }
        }
    }

    cout << "\n========== TASK D ==========\n";

    cout << "Source City: "
         << source
         << endl;

    int farthestCity = -1;
    int maxDistance = -1;

    vector<int> unreachable;

    cout << "\nShortest Distance Table:\n";

    for(int i = 0; i < NUM_CITIES; i++)
    {
        if(dist[i] == INT_MAX)
        {
            unreachable.push_back(i);
        }
        else
        {
            cout << "City "
                 << i
                 << " -> "
                 << dist[i]
                 << " km\n";

            if(dist[i] > maxDistance)
            {
                maxDistance = dist[i];
                farthestCity = i;
            }
        }
    }

    cout << "\nUnreachable Cities:\n";

    if(unreachable.empty())
    {
        cout << "None\n";
    }
    else
    {
        for(int city : unreachable)
        {
            cout << city << " ";
        }
        cout << endl;
    }

    cout << "\nCity with Maximum Shortest Path Distance: "
         << farthestCity
         << endl;

    cout << "Distance: "
         << maxDistance
         << " km\n";
}

int main()
{
    generateGraph();

    saveAdjacencyList();

    analyzeComponents();

    int source;

    cout << "\nEnter Source City (0-499): ";
    cin >> source;

    while(source < 0 || source >= NUM_CITIES)
    {
        cout << "Invalid City. Enter Again: ";
        cin >> source;
    }

    dijkstra(source);

    return 0;
}