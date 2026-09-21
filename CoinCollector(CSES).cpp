// https://cses.fi/problemset/task/1686/

#include <bits/stdc++.h>
using namespace std;
#define int long long 
#define N (int)(1e6+5)
#define fou(i, a, b, s) for(int i = a; i <= b; i += s)
#define MOD (int)(1e9+7)
#define oo (int)(1e9)
#define el '\n'
#define fastio ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);

int n, m, dfstime, scc, res;
vector<vector<int>> adj, adj_scc;
vector<int> dp;
int low[N], id[N], scc_coin[N], component[N], k[N];
bool vis[N];
stack<int> s;

void tarjan(int u)
{
    low[u] = id[u] = ++dfstime;
    s.push(u);

    for (auto v : adj[u])
    {
        if (vis[v]) continue;
        
        if (id[v]) low[u] = min(low[u], id[v]);
        else
        {
            tarjan(v);
            low[u] = min(low[u], low[v]);
        }
    }

    if (low[u] == id[u])
    {
        ++scc;
        int v;
        do
        {
            v = s.top();
            s.pop();  
            vis[v] = 1;

            component[v] = scc;
            scc_coin[scc] += k[v];
        } while (v != u);
    }
}

int getMaxCoin(int u)
{
    if (dp[u] != -1) return dp[u];
    int max_future = 0;
    for (auto v : adj_scc[u])
    {
        max_future = max(max_future, getMaxCoin(v));
    }
    return dp[u] = scc_coin[u] + max_future;
}

void solve()
{
    cin >> n >> m;
    adj.resize(n + 5);
    
    for (int i = 1; i <= n; i++)
    {
        cin >> k[i];
    }
    for (int i = 1; i <= m; i++)
    {
        int a, b; cin >> a >> b;
        adj[a].push_back(b);
    }
    
    for (int i = 1; i <= n; i++)
    {
        if (!id[i]) tarjan(i);
    }

    adj_scc.resize(scc + 5);
    for (int u = 1; u <= n; u++)
    {
        for (auto v : adj[u])
        {
            if (component[u] != component[v])
            {
                adj_scc[component[u]].push_back(component[v]);
            }
        }
    }

    dp.assign(scc + 5, -1);
    for (int i = 1; i <= scc; i++) // reverse toposort
    {
        res = max(res, getMaxCoin(i));
    }
    cout << res;
}

signed main()
{
    fastio;
    solve();
    return 0;
}
