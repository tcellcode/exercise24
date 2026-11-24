/*
Có một tập đa tập các xâu trên bảng chữ cái a, b, c, ban đầu rỗng. Thực hiện + s (thêm), - s (xóa một bản sao, thao tác bảo đảm luôn có ít nhất s trong tập), hoặc ? k (in xâu đứng thứ 
k
k nếu sắp xếp tất cả các xâu hiện có theo thứ tự từ điển, tính cả bản sao). Nếu kích thước tập < k, in ra NONE.
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
          int child[3];
          int startCnt = 0;
          int endCnt = 0;
          Node()
          {
               child[0] = child[1] = child[2] = -1;
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
          int curr = 0;
          trie[curr].startCnt += val;

          for (char ch : s)
          {
               int c = ch - 'a';
               if (trie[curr].child[c] == -1)
               {
                    trie[curr].child[c] = trie.size();
                    trie.emplace_back();
               }
               curr = trie[curr].child[c];
               trie[curr].startCnt += val;
          }

          trie[curr].endCnt += val;
     }


     string findKth(int k)
     {
          if (trie[0].startCnt < k) return "NONE";

          int curr = 0;
          string res = "";
          while (true)
          {
               if (k <= trie[curr].endCnt)
               {
                    return res;
               }

               k -= trie[curr].endCnt;

               for (int c = 0; c < 3; c++)
               {
                    int nxt = trie[curr].child[c];
                    if (nxt != -1)
                    {
                         if (k <= trie[nxt].startCnt)
                         {
                              res += char('a' + c);
                              curr = nxt;
                              break;
                         }
                         else k -= trie[nxt].startCnt;
                         
                    }
               }
          }
          
     }
};

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
          int q; cin >> q;

          Trie trie;

          while (q--)
          {
               char type; cin >> type;
               if (type == '+')
               {
                    string s; cin >> s;
                    trie.add(s, 1);
               }
               else if (type == '-')
               {
                    string s; cin >> s;
                    trie.add(s, -1);
               }
               else if (type == '?')
               {
                    int k; cin >> k;
                    cout << trie.findKth(k) << el; 
               }
          }
          
     }
     
     
}
