#include <bits/stdc++.h>
using namespace std;

#define fastio ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
#define oo (int)(1e18)
#define N (int)(1e6 + 5)
#define el '\n'
#define MOD (int)(998244353)
#define int long long 

#define FREOPEN


struct Node
{
     int sz, preflen, sufflen, leftval, rightval, maxlen;
};



Node st[4 * N];
int a[N];

Node merge(const Node &a, const Node &b)
{
     if (a.sz == 0) return b;
     if (b.sz == 0) return a;


     Node res;
     res.sz = a.sz + b.sz;
     res.leftval = a.leftval;
     res.rightval = b.rightval;
     res.maxlen = max(a.maxlen, b.maxlen);
     res.preflen = a.preflen;
     res.sufflen = b.sufflen;
     if (a.rightval < b.leftval)
     {
          res.maxlen = max(res.maxlen, a.sufflen + b.preflen);
          if (a.preflen == a.sz)
          {
               res.preflen = a.sz + b.preflen;
          }
          if (b.sufflen == b.sz)
          {
               res.sufflen = b.sz + a.sufflen;
          }
     }
     return res;
}








void update(int id, int l, int r, int pos, int val)
{
     if (pos < l || r < pos) return;
     if (l == r)
     {
          st[id] = {1, 1, 1, val, val, 1};
          return;
     }
     int mid = (l + r) / 2;
     update(2 * id, l, mid, pos, val);
     update(2 * id + 1, mid + 1, r, pos, val);
     st[id] = merge(st[2 * id], st[2 * id + 1]);

}

Node get(int id, int l, int r, int u, int v)
{
     if (v < l || r < u) return {0, 0, 0, -oo, oo, 0};
     if (u <= l && r <= v) return st[id];
     int mid = (l + r) / 2;
     return merge(get(2 * id, l, mid, u, v), get(2 * id + 1, mid + 1, r, u, v));
}


signed main()
{     
     fastio;

     #ifdef FREOPEN
          freopen("input.txt", "r", stdin);
          freopen("output.txt", "w", stdout);
     #endif


     int t; cin >> t;
     while (t--)
     {
          int n, q; cin >> n >> q;

          for (int i = 1; i <= n; i++)
          {
               int a; cin >> a;
               update(1, 1, n, i, a);
          }
          
          
          for (int i = 1; i <= q; i++)
          {
               int type; cin >> type;
               if (type == 1)
               {
                    int i, x; cin >> i >> x;
                    update(1, 1, n, i, x);
               }
               else
               {
                    int l, r; cin >> l >> r;
                    cout << get(1, 1, n, l, r).maxlen << el;
               }
          }
     }
     
     

     
     

}
