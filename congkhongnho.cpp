/*
Đề bài

Cho dãy 
a
a gồm 
n
n số nguyên không âm. Chọn hai chỉ số phân biệt 
i
,
j
i,j và tính 
a
i
⊞
a
j
a 
i
​
 ⊞a 
j
​
  theo phép cộng không nhớ: tại mỗi hàng thập phân, cộng hai chữ số tương ứng rồi chỉ giữ chữ số hàng đơn vị, không chuyển phần nhớ sang hàng kế tiếp.

Ví dụ, 
9
⊞
5
=
4
9⊞5=4 và 
19
⊞
95
=
4
19⊞95=4 (Lưu ý viết 
4
4 chứ không phải 
04
04 vì ta không xét chữ số 
0
0 vô nghĩa ở đầu, dù 
9
+
5
=
4
9+5=4 và 
1
+
9
=
0
1+9=0).

Hãy tìm giá trị lớn nhất có thể của 
a
i
⊞
a
j
a 
i
​
 ⊞a 
j
​
  với 
i
≠
j
i

=j.
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
          int child[10];
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

     void getDigit(int x, int d[9])
     {
          for (int i = 8; i >= 0; i--)
          {
               d[i] = x % 10;
               x /= 10;
          }
     }
     
     void add(int x)
     {
          int cur = 0;
          
          int d[9];
          getDigit(x, d);

          for (int i = 0; i < 9; i++)
          {
               int c = d[i];
               if (trie[cur].child[c] == -1)
               {
                    trie[cur].child[c] = trie.size();
                    trie.emplace_back();
               }
               cur = trie[cur].child[c];
          }
     }

     int query(int x)
     {
          int d[9];
          getDigit(x, d);
          int cur = 0, res = 0;
          for (int i = 0; i < 9; i++)
          {
               for (int target = 9; target >= 0; target--)
               {
                    int need = (target - d[i] + 10) % 10;
                    if (trie[cur].child[need] != -1)
                    {
                         res = res * 10 + target;
                         cur = trie[cur].child[need];
                         break;
                    }
               }
          }
          return res;
     }
};

void solve()
{
     Trie trie;
     int n; cin >> n;
     int ans = 0;
     for (int i = 0; i < n; i++)
     {
          int x; cin >> x;
          if (i > 0) ans = max(ans, trie.query(x));
          trie.add(x);
     }
     cout << ans << el;
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

