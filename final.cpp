#ifndef DSU_H
#define DSU_H

#include<vector>
using namespace std;

class DSU
{
public:

    vector<int> parent;
    vector<int> rank;

    DSU(int n)
    {
        parent.resize(n);
        rank.resize(n,0);

        for(int i=0;i<n;i++)
            parent[i]=i;
    }

    int find(int x)
    {
        if(parent[x]==x)
            return x;

        return parent[x]=find(parent[x]);
    }

    void unite(int a,int b)
    {
        a=find(a);
        b=find(b);

        if(a!=b)
        {
            if(rank[a]<rank[b])
                swap(a,b);

            parent[b]=a;

            if(rank[a]==rank[b])
                rank[a]++;
        }
    }
};

#endif

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
using namespace std;

struct Edge
{
    int to;
    int weight;
};

struct Road
{
    int u;
    int v;
    int weight;
};

class Graph
{
private:
    int V;

public:

    vector<vector<Edge>> adj;
    vector<Road> roads;

    Graph(int vertices);

    void addEdge(int u,int v,int w);

    void generateRandomGraph(int edges);

    void printGraph();

    int getVertices();
};

#endif

#ifndef ALGORITHMS_H
#define ALGORITHMS_H


void kruskalMST(Graph &g);

void strategicCities(Graph &g);

void disasterRouting(Graph &g);

void trafficRouting(Graph &g);

void articulationPoints(Graph &g);

void disasterRouting(Graph &g);

void trafficRouting(Graph &g);

void articulationPoints(Graph &g);

#endif

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

Graph::Graph(int vertices)
{
    V=vertices;
    adj.resize(V);
}

void Graph::addEdge(int u,int v,int w)
{
    adj[u].push_back({v,w});
    adj[v].push_back({u,w});

    roads.push_back({u,v,w});
}

void Graph::generateRandomGraph(int edges)
{
    srand(time(0));

    for(int i=0;i<edges;i++)
    {
        int u=rand()%V;
        int v=rand()%V;

        if(u==v)
        {
            i--;
            continue;
        }

        int w=1+rand()%100;

        addEdge(u,v,w);
    }
}

void Graph::printGraph()
{
    for(int i=0;i<V;i++)
    {
        cout<<"City "<<i<<" : ";

        for(auto edge:adj[i])
        {
            cout<<"("<<edge.to<<","<<edge.weight<<") ";
        }

        cout<<endl;
    }
}

int Graph::getVertices()
{
    return V;
}

#include<iostream>
#include<algorithm>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

bool compareRoad(Road a,Road b)
{
    return a.weight<b.weight;
}
void kruskalMST(Graph &g)
{
    vector<Road> edges=g.roads;

    sort(edges.begin(),edges.end(),compareRoad);

    DSU dsu(g.getVertices());

    int totalWeight=0;

    cout<<"\n===== Minimum Spanning Tree =====\n";

    int edgeCount=0;

    for(auto road:edges)
    {
        if(dsu.find(road.u)!=dsu.find(road.v))
        {
            dsu.unite(road.u,road.v);

            cout<<road.u<<" ---- "
                <<road.v
                <<"  Weight = "
                <<road.weight
                <<endl;

            totalWeight+=road.weight;

            edgeCount++;

            if(edgeCount==g.getVertices()-1)
                break;
        }
    }

    cout<<"\nTotal Roads : "<<edgeCount<<endl;

    cout<<"Total Cost : "<<totalWeight<<endl;
}

void strategicCities(Graph &g)
{
    vector<pair<int,int>> degree;

    for(int i = 0; i < g.getVertices(); i++)
    {
        degree.push_back({(int)g.adj[i].size(), i});
    }

    sort(degree.begin(), degree.end(), greater<pair<int,int>>());

    cout << "\n===== Top 10 Strategic Cities =====\n\n";

    cout << "Rank\tCity\tConnections\n";

    for(int i = 0; i < 10; i++)
    {
        cout << i + 1 << "\t"
             << degree[i].second << "\t"
             << degree[i].first << endl;
    }
}
int shortestDistance(Graph &g,int source,int destination)
{
    int V=g.getVertices();

    vector<int> dist(V,INT_MAX);

    priority_queue<
        pair<int,int>,
        vector<pair<int,int>>,
        greater<pair<int,int>>
    > pq;

    dist[source]=0;

    pq.push({0,source});

    while(!pq.empty())
    {
        int current=pq.top().second;
        pq.pop();

        for(auto edge:g.adj[current])
        {
            if(dist[current]+edge.weight<dist[edge.to])
            {
                dist[edge.to]=dist[current]+edge.weight;

                pq.push({dist[edge.to],edge.to});
            }
        }
    }

    return dist[destination];
}
void disasterRouting(Graph &g)
{
    int source,destination;

    cout<<"\n===== Disaster Recovery Routing =====\n";

    cout<<"Enter Source City : ";
    cin>>source;

    cout<<"Enter Destination City : ";
    cin>>destination;

    int V=g.getVertices();

    vector<int> distance(V,INT_MAX);
    vector<int> parent(V,-1);

    priority_queue<pair<int,int>,
    vector<pair<int,int>>,
    greater<pair<int,int>>> pq;

    distance[source]=0;

    pq.push({0,source});

    while(!pq.empty())
    {
        int current=pq.top().second;
        pq.pop();

        for(auto edge:g.adj[current])
        {
            int next=edge.to;
            int weight=edge.weight;

            if(distance[current]+weight<distance[next])
            {
                distance[next]=distance[current]+weight;

                parent[next]=current;

                pq.push({distance[next],next});
            }
        }
    }

    if(distance[destination]==INT_MAX)
    {
        cout<<"\nNo Route Exists!\n";
        return;
    }

    vector<int> path;

    for(int v=destination;v!=-1;v=parent[v])
        path.push_back(v);

    reverse(path.begin(),path.end());

    cout<<"\nShortest Route\n";

    for(int city:path)
        cout<<city<<" ";

    cout<<"\n\nDistance = "<<distance[destination]<<endl;
}

void trafficRouting(Graph &g)
{
    Graph trafficGraph = g;

    srand(time(0));

    for(int i=0;i<100;i++)
    {
        int city = rand()%trafficGraph.getVertices();

        if(trafficGraph.adj[city].empty())
            continue;

        int road = rand()%trafficGraph.adj[city].size();

        trafficGraph.adj[city][road].weight += 50;
    }

    int source,destination;

    cout<<"\n===== Traffic Aware Routing =====\n";

    cout<<"Source : ";
    cin>>source;

    cout<<"Destination : ";
    cin>>destination;

    int normal = shortestDistance(g,source,destination);

    int traffic = shortestDistance(trafficGraph,source,destination);

    cout<<"\nNormal Distance : "<<normal<<endl;

    cout<<"Traffic Distance : "<<traffic<<endl;

    cout<<"Extra Delay : "<<traffic-normal<<endl;
}

void APDFS(Graph &g,
           int u,
           vector<bool> &visited,
           vector<int> &disc,
           vector<int> &low,
           vector<int> &parent,
           vector<bool> &ap,
           int &timer)
{
    visited[u]=true;

    disc[u]=low[u]=++timer;

    int children=0;

    for(auto edge:g.adj[u])
    {
        int v=edge.to;

        if(!visited[v])
        {
            children++;

            parent[v]=u;

            APDFS(g,v,visited,disc,low,parent,ap,timer);

            low[u]=min(low[u],low[v]);

            if(parent[u]==-1 && children>1)
                ap[u]=true;

            if(parent[u]!=-1 && low[v]>=disc[u])
                ap[u]=true;
        }

        else if(v!=parent[u])
        {
            low[u]=min(low[u],disc[v]);
        }
    }
}

void articulationPoints(Graph &g)
{
    int V=g.getVertices();

    vector<bool> visited(V,false);

    vector<int> disc(V);

    vector<int> low(V);

    vector<int> parent(V,-1);

    vector<bool> ap(V,false);

    int timer=0;

    for(int i=0;i<V;i++)
    {
        if(!visited[i])
        {
            APDFS(g,
                  i,
                  visited,
                  disc,
                  low,
                  parent,
                  ap,
                  timer);
        }
    }

    cout<<"\n===== Critical Cities =====\n\n";

    int count=0;

    for(int i=0;i<V;i++)
    {
        if(ap[i])
        {
            cout<<"City "<<i<<endl;
            count++;
        }
    }

    cout<<"\nTotal Critical Cities : "<<count<<endl;
}

#include<iostream>

#include"Graph.h"
#include"Algorithms.h"

using namespace std;

int main()
{
    Graph g(500);

    g.generateRandomGraph(1200);

    cout << "Graph Generated!\n";

    kruskalMST(g);

    strategicCities(g);

    disasterRouting(g);

    trafficRouting(g);

    articulationPoints(g);

    return 0;
}

