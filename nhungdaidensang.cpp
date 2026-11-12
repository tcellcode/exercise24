/*
Đề bài

Có n bóng đèn xếp thành một hàng. Đèn i sáng với xác suất a[i]/100 và tắt với xác suất còn lại. Trạng thái của các đèn độc lập với nhau.

Trong một đoạn liên tiếp [l, r], một dải sáng là một nhóm bóng đèn sáng liên tiếp cực đại trong chính đoạn đó: không thể mở rộng nhóm thêm sang trái hay phải mà vẫn thuộc [l, r] và đều sáng.

Ví dụ, giả sử trong một lần bật thử, sáu đèn có trạng thái 1 1 0 1 1 1 (1 là sáng, 0 là tắt). Có đúng 2 dải sáng: đèn 1–2 và đèn 4–6. Đoạn chỉ gồm đèn 4–5 tuy đều sáng nhưng không được tính là một dải riêng, vì còn có thể mở rộng sang đèn 6 đang sáng.

Dải sáng luôn được xác định trong phạm vi đoạn đang hỏi. Chẳng hạn, nếu chỉ xét [2, 5], trạng thái trong đoạn là 1 0 1 1, nên có 2 dải sáng: đèn 2 và đèn 4–5. Không xét đèn 1 hay đèn 6 nằm ngoài đoạn khi xác định biên của các dải. Nếu tất cả đèn trong đoạn đều tắt thì số dải sáng bằng 0.

Các ví dụ trạng thái trên chỉ để giải thích cách đếm. Trong bài thực tế, đèn sáng hay tắt là ngẫu nhiên, nên kết quả yêu cầu là kỳ vọng (giá trị trung bình về mặt xác suất) của số dải sáng, không phải số dải sáng của một lần bật cụ thể.

Ban quản lý thực hiện các thao tác sau trên dãy xác suất:

1 x y: Hoán đổi xác suất sáng của hai đèn tại vị trí x và y. Nếu x = y thì không có thay đổi.
2 l r: Tính kỳ vọng số dải sáng trong đoạn [l, r] ở lần bật thử tiếp theo.
Mỗi thao tác loại 2 được tính với trạng thái xác suất hiện tại. Hãy in kỳ vọng modulo 998244353.

Một phân số P/Q được biểu diễn bằng P * Q^(-1) mod 998244353.

Dữ liệu vào

Dòng đầu chứa số nguyên T (1 <= T <= 20), số bộ test.
Với mỗi test:
Dòng đầu chứa n, q (1 <= n, q <= 200000).
Dòng tiếp theo chứa n số nguyên a[i] (0 <= a[i] <= 100).
q dòng tiếp theo chứa ba số nguyên type, x, y.
Nếu type = 1 thì 1 <= x, y <= n.
Nếu type = 2 thì 1 <= x <= y <= n, tương ứng với [l, r].
Dữ liệu ra

Với mỗi test, in một dòng cho mỗi thao tác loại 2, là kỳ vọng số dải sáng cực đại của đoạn được hỏi modulo 998244353.
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


template <typename Node, Node (*merge)(Node, Node)>
class SegmentTree
{
private:
     int n;
     const Node neutral;

public:
     vector<Node> st;
     
     SegmentTree(int n, Node neutral) : n(n), neutral(neutral)
     {
          st.assign(4 * n + 5, neutral);
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
          if (v < l || r < u) return neutral;
          if (u <= l && r <= v) return st[id];
          int mid = (l + r) / 2;
          return merge(get(2 * id, l, mid, u, v), get(2 * id + 1, mid + 1, r, u, v));
     }
};


int p[N];



int binpower(int a, int b, int mod) 
{
     int res = 1;
     a %= mod;
     while (b > 0)
     {
          if (b & 1) res = (res * a) % mod;
          a = (a * a) % mod;
          b /= 2;
     }
     return res;
}

int merge(int a, int b)
{
     return (a + b) % MOD;
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

          const int INV100 = binpower(100, MOD - 2, MOD);
          for (int i = 1; i <= n; i++)
          {
               cin >> p[i];
               p[i] = (p[i] * INV100) % MOD;
          }

          
          SegmentTree<int, merge> segtree(n, 0);
          for (int i = 1; i <= n; i++)
          {
               segtree.update(1, 1, n, i, ( (1 - p[i - 1] + MOD) % MOD * p[i] ) % MOD );
          }
          
          
          
          while (q--)
          {
               int type, x, y; cin >> type >> x >> y;
               if (type == 1)
               {
                    if (x == y) continue;
                    swap(p[x], p[y]);
                    int affected[] = {x, x + 1, y, y + 1};
                    for (int pos : affected)
                    {
                         if (1 <= pos && pos <= n ) segtree.update(1, 1, n, pos, ( (1 - p[pos - 1] + MOD) % MOD * p[pos] ) % MOD );
                    }
               }
               else
               {
                    cout << (p[x] + segtree.get(1, 1, n, x + 1, y) ) % MOD << el;
               }
          }
          

     }

     

}
