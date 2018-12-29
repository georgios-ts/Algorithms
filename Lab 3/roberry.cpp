//
//  main.cpp
//  robbery
//
//  Created by Giorgos Tsili on 13/12/18.
//  Copyright © 2018 Giorgos Tsili. All rights reserved.
//



#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <cstring>

#define MAXN 50000
#define MAXM 500000
#define BSIZE 1<<15

#define mp make_pair

using namespace std;


// edge = {dest, id} in multi-graphs 
// (id to distinguish between parallel edges)
#define dest first
#define id second

typedef pair< int, int > edge;  

// weighted edge {weight, source, dest}
#define weight first
#define src second.first
#define dst second.second

typedef pair<int, pair<int, int> > w_edge;



int parent[ MAXN + 1], Rank[ MAXN + 1];


char buffer[BSIZE];
long long bpos = 0L, bsize = 0L;

int readInt()
{
    long long d = 0L;
    int x = 0;
    char c;
    
    while (1)  {
        if (bpos >= bsize) {
            bpos = 0;
            if (feof(stdin)) return x;
            bsize = fread(buffer, 1, BSIZE, stdin);
        }
        c = buffer[bpos++];
        if (c >= '0' && c <= '9') {  x = x*10 + (c-'0'); d = 1; }
        else if (d == 1) return x;
    }
    return -1;
}


// union - find implementation 
int find(int u) {
    
    if (parent[u] == u)
        return u;
    
    return parent[u] = find( parent[u] );
}


void Union(int u, int v) {
    
    u = find(u);  v = find(v);
    
    if (Rank[u] > Rank[v])
        parent[v] = u;
    
    else {
        parent[u] = v;
        if (Rank[u] == Rank[v])
            Rank[v]++;
    }
    
}



// A class that represent an undirected multi - graph with adjacency lists.

class MultiGraph {
    
    int V, E;                 // Number of vertices and edges
    vector<edge> *adj;
    bool *visited;
    int timer;
    int *disc, *low, *parent, *pedge;
   
    int dfs(int u);
    
public:
    
    MultiGraph(int n);
    void addEdge(int u, int v);
    int bridges();
    
};

MultiGraph::MultiGraph(int n) {
    
    timer = 0;
    V = n;  E = 0;
    adj = new vector<edge>[n];
    visited = new bool[n];
    low = new int[n]; disc = new int[n];  
    parent = new int[n]; pedge = new int[n];
    
}


void MultiGraph::addEdge(int u, int v) {
    
    adj[u].push_back(mp(v, ++E));
    adj[v].push_back(mp(u, E));
    
}

int MultiGraph::dfs(int u) {
    
    if (adj[u].empty())
        return 0;
    
    int bridge = 0, v, idx;
    
    vector<int> dfsOrder;
    stack<int> stack;
    stack.push(u);
    
    while (!stack.empty()) {
        
        u = stack.top();
            
        stack.pop();
        
        if (visited[u])
            continue;
        
        visited[u] = true;
        dfsOrder.push_back(u);
        disc[u] = low[u] = timer++;
       
        
        for (edge e: adj[u]) {
            
            v = e.dest, idx = e.id;
            
            if (!visited[v]) {
                stack.push(v);
                parent[v] = u;  pedge[v] = idx;
            }
        }
    }
    
    vector<int>:: reverse_iterator iter;
    for (iter = dfsOrder.rbegin(); iter != dfsOrder.rend() - 1; ++iter) {
        
        int u = *iter, p = parent[u];
        
        for (edge x: adj[u]) {
            
            int w = x.dest, idx = x.id;
            
            if (pedge[u] != idx)
                low[u] = min(low[u], disc[w]);
        }
        
        low[p] = min(low[p], low[u]);
        if (low[u] > disc[p])
            bridge++;
    }
    
    return bridge;    
}

int MultiGraph::bridges() {
    
    int bridge = 0;
    timer = 0;
    
    memset(visited, 0, V * sizeof(bool));
    memset(parent, -1, V * sizeof(int));
    memset(pedge,  -1, V * sizeof(int));
    
    for (int i = 0; i < V; i++) {
        if(!visited[i])
            bridge += dfs(i);
    }
    
    return bridge;
}




int N, M;
vector<w_edge> G;


int main()
{
    int a, b, c, u, v, w, j, i, critical = 0, bad = 0;
    
    N = readInt();   M = readInt();
    
    
    for (i = 0; i < M; i++) {
        a = readInt();  b = readInt();  c = readInt();
        G.push_back(mp(c, mp(a - 1, b - 1)));
    }
    
    sort(G.begin(), G.end());
    
    for (i = 0; i < N; i++) {
        parent[i] = i;  Rank[i] = 0;
    }
    
    i = 0;
    while (i < M) {
        
        MultiGraph C(N);
        
        w = G[i].weight;
        j = i;
        while (j < M && G[j].weight == w) {
            
            u = find(G[j].src);   v = find(G[j++].dst);
            
            if (u == v)
                bad++;
            else
                C.addEdge(u, v);
       }
       
        critical += C.bridges();

        
        while (i < j) {
            
            u = find(G[i].src);   v = find(G[i++].dst);
            
            if (u != v)
                Union(u, v);
        }
    }
    
    cout << critical << "\n" << bad << "\n" << M - critical - bad << "\n";
    
    
    return 0;
    
}
