#include <bits/stdc++.h>
using namespace std;

#define fastio ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
#define oo (int)(1e18)
#define N (int)(1e6 + 5)
#define el '\n'
#define MOD (int)(998244353)
#define int long long 

//#define FREOPEN



struct Node
{
     int width, height, maxlen;
     bool operator<(const Node& other) const
     {
          if (width == other.width) return height > other.height;
          return width < other.width;
     }
     bool operator==(const Node& other) const
     {
          return (width == other.width && height == other.height);
     }
};



Node st[4 * N];
int n;
Node box[N];



Node merge(const Node &a, const Node &b)
{
     return (a.maxlen >= b.maxlen) ? a : b;
}


void update(int id, int l, int r, int pos, Node val)
{
     if (pos < l || r < pos) return;
     if (l == r)
     {
          st[id] = val;
          return;
     }
     int mid = (l + r ) / 2;
     update(2 * id, l, mid, pos, val);
     update(2 * id + 1, mid + 1, r, pos, val);
     st[id] = merge(st[2 * id], st[2 * id + 1]);
}

Node get(int id, int l, int r, int u, int v)
{
     if (v < l || r < u) return {0, 0, 0};
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
          cin >> n;

          vector<int> heights;
          for (int i = 1; i <= n; i++)
          {
               cin >> box[i].width >> box[i].height;
               heights.push_back(box[i].height);
          }

          sort(box + 1, box + 1 + n);
          sort(heights.begin(), heights.end());
          heights.erase(unique(heights.begin(), heights.end()), heights.end());

          Node best = {0, 0, 0};

          for (int i = 1; i <= n; i++)
          {
               int r = lower_bound(heights.begin(), heights.end(), box[i].height) - heights.begin() + 1;
               Node prevmax = (r > 1) ? get(1, 1, heights.size(), 1, r - 1) : Node{0, 0, 0};
               Node curr = {box[i].width, box[i].height, prevmax.maxlen + 1};
               update(1, 1, heights.size(), r, curr);
               best = max(best, curr);
          }
          
          cout << best.maxlen << el;

     }
     
     

     

}
