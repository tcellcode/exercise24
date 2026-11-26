/*
Cho 
n
n chuỗi khác rỗng chỉ gồm chữ cái thường tiếng Anh. Với mỗi cặp chỉ số 
i
<
j
i<j, gọi 
c
(
i
,
j
)
c(i,j) là số ký tự đầu tiên liên tiếp giống nhau của hai chuỗi. Tính 
∑
i
<
j
c
(
i
,
j
)
∑ 
i<j
​
 c(i,j).
*/

#include <bits/stdc++.h>
using namespace std;

#define fastio ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
#define oo (int)(1e18)
#define N (int)(1e6 + 5)
#define el '\n'
#define MOD (int)(998244353)
#define int long long 

#define FREOPEN




struct Trie
{
     struct Node
     {
          int child[26];
          int cnt = 0;
          Node()
          {
               memset(child, -1, sizeof(child));
          }
     };

     vector<Node> trie;

     Trie()
     {
          trie.clear();
          trie.emplace_back();
     }

     
     void add(const string &s, int val)
     {
          int cur = 0;
          trie[cur].cnt += val;
          for (char ch : s)
          {
               int c = ch - 'a';
               if (trie[cur].child[c] == -1)
               {
                    trie[cur].child[c] = trie.size();
                    trie.emplace_back();
               }
               cur = trie[cur].child[c];
               trie[cur].cnt += val;
          }
     }
     
     void query()
     {
          int ans = 0;
          for (int i = 1; i < (int)trie.size(); i++)
          {
               ans += trie[i].cnt * (trie[i].cnt - 1) / 2;
          }
          cout << ans << el;
     }
};

void solve()
{
     int n; cin >> n;
     Trie trie;
     for (int i = 1; i <= n; i++)
     {
          string s; cin >> s;
          trie.add(s, 1);
     }
     trie.query();
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
          solve();
     }
     
     
}
