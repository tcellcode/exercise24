// https://cses.fi/problemset/task/1191/
#include <bits/stdc++.h>
using namespace std;
#define int long long 
#define N (int)(1e6+5)
#define fou(i, a, b, s) for(int i = a; i <= b; i += s)
#define MOD (int)(1e9+7)
#define oo (int)(1e9)
#define el '\n'
#define fastio ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
 
int n, k, LOG2N = 25, res = oo;
vector<int> a;
vector<vector<int>> up;
 
void build()
{
    int p1 = 1, p2 = 1, sum = 0;
    while (p1 <= 2*n)
    {
        while (p2 <= 2*n && sum + a[p2] <= k)
        {
            sum += a[p2];
            p2++;
        }
        up[p1][0] = p2;
        sum -= a[p1];
        p1++;
        
    }
    
    up[2*n + 1][0] = 2*n + 1;
    for (int j = 1; j <= LOG2N; j++)
    {
        for (int i = 1; i <= 2*n + 1; i++)
        {
            up[i][j] = up[ up[i][j - 1] ][j - 1];
        }
    }
}
 
void solve()
{
    cin >> n >> k;
    a.resize(2*n + 5);
    up.resize(2*n + 5, vector<int>(LOG2N + 5, 0));
 
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
        a[i + n] = a[i];
    }
 
    build();
 
    for (int i = 1; i <= n; i++)
    {
        int step = 0, curr = i;
        for (int j = LOG2N; j >= 0; j--)
        {
            if (up[curr][j] < i + n)
            {
                curr = up[curr][j];
                step += (1 << j);
            }
            
        }
        res = min(res, step + 1);
 
    }
 
    cout << res;
    
}
 
signed main()
{
    fastio;
    solve();
    
    return 0;
}
