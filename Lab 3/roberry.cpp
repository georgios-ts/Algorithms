//
//  main.cpp
//  robbery
//
//  Created by Giorgos Tsili on 13/12/18.
//  Copyright © 2018 Giorgos Tsili. All rights reserved.
//



#include<iostream>
#include <vector>
#include <stack>

#define MAXN 50000
#define MAXM 500000
#define BSIZE 1<<15

#define mp make_pair
#define dest first
#define id second
#define weight first
#define src second.first
#define dst second.second



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


using namespace std;

typedef pair< int, int > edge;  // edge = {dest, id} in multi-graphs (id to distinguish between parallel edges)

// A class that represent an undirected multi - graph with adjacency lists.

class Multi_Graph {
    
    int n, m;    // Number of vertices and edges
    vector<edge> *adj;
    bool *visited;
    int *disc, *low, *parent, *explored, timer;
    int dfs(int u, int p);
    
public:
    
    Multi_Graph(int V);
    void add_edge(int u, int v);
    int bridges();
    void clear();
    void show();
};

Multi_Graph::Multi_Graph(int V) {
    
    n = V;
    m = 0;
    timer = 0;
    adj = new vector<edge>[V];
    low = new int[V];
    disc = new int[V];
    visited = new bool[V];
    parent = new int[V];
    
}

void Multi_Graph::clear() {
    
    m = 0;
    for (int i = 0; i < n; i++)
        adj[i].clear();
}

void Multi_Graph::show() {
    
    for (int u = 0; u < n; u++) {
        cout << u << ": ";
        for(edge e: adj[u])
            cout << e.dest << " ";
        cout << "\n";
    }
    
}

void Multi_Graph::add_edge(int u, int v) {
    
    adj[u].push_back(mp(v, ++m));
    adj[v].push_back(mp(u, m));
    
}

int Multi_Graph::dfs(int u, int p = -1) {
    
    int bridge = 0, v, idx;
    
    vector<edge> tree_edges;
    
    stack< pair<int, int> > stack;
    
    stack.push(mp(u, -1));
    
    while (!stack.empty()) {
        u = stack.top().first;
        v = stack.top().second;
        
        stack.pop();
        
        if (visited[u])
            continue;
        
        visited[u] = true;
        disc[u] = low[u] = timer++;
        
        if (v != -1)
            tree_edges.push_back(mp(v, u));
        
        for (edge e: adj[u]) {
            
            v = e.dest, idx = e.id;
            
            if (!visited[v]) {
                stack.push(mp(v, u));
                parent[v] = idx;
            }
        }
    }
    
    vector<edge>:: reverse_iterator i;
    for (i = tree_edges.rbegin(); i != tree_edges.rend(); ++i) {
        
        edge e = *i;
        
        int u = e.first, v = e.second;
        
        for (edge x: adj[v]) {
            
            int w = x.dest, idx = x.id;
            
            if (parent[v] != idx)
                low[v] = min(low[v], disc[w]);
        }
        
        low[u] = min(low[u], low[v]);
        if (low[v] > disc[u])
            bridge++;
    }
    
    
    return bridge;
    
}

int Multi_Graph::bridges() {
    
    int bridge = 0;
    timer = 0;
    memset(visited, 0, n);
    memset(parent, -1, n);
    
    for (int i = 0; i < n; i++) {
        if(!visited[i])
            bridge += dfs(i);
    }
    
    return bridge;
}



typedef pair<int, pair<int, int> > w_edge;



int N, M, parent[ MAXN + 1], Rank[ MAXN + 1];
vector<w_edge> G;


int find(int u) {
    
    if (parent[u] == u)
        return u;
    
    return parent[u] = find( parent[u] );
}


void Union(int u, int v) {
    
    u = find(u);
    v = find(v);
    
    if (Rank[u] > Rank[v])
        parent[v] = u;
    
    else {
        parent[u] = v;
        if (Rank[u] == Rank[v])
            Rank[v]++;
    }
    
}



int main()
{
    int a, b, c, u, v, w, j, i, critical = 0, bad = 0;
    
    N = readInt();
    M = readInt();
    
    Multi_Graph C(N + 1);
    G.reserve(M + 1);
    
    for (i = 0; i < M; i++) {
        a = readInt();
        b = readInt();
        c = readInt();
        G.push_back(mp(c, mp(a - 1, b - 1)));
    }
    
    sort(G.begin(), G.end());
    
    for (i = 0; i < N; i++) {
        parent[i] = i;
        Rank[i] = 0;
    }
    
    i = 0;
    while (i < M) {
        
        w = G[i].weight;
        j = i;
        while (j < M && G[j].weight == w) {
            u = find(G[j].src);   v = find(G[j++].dst);
            if (u == v)
                bad++;
            else
                C.add_edge(u, v);
        }
        

        int x = C.bridges();
        
        critical += x;
        C.clear();
        
        while (i < j) {
            u = find(G[i].src);   v = find(G[i++].dst);
            if (u != v)
                Union(u, v);
        }
    }
    
    cout << critical << "\n";
    cout << bad << "\n";
    cout << M - critical - bad << "\n";
    
    return 0;
}
