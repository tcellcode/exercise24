#include <bits/stdc++.h>
using namespace std;

#define fastio ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
#define oo (int)(1e18)
#define N (int)(1e6 + 5)
#define el '\n'
#define MOD (int)(998244353)
#define int long long 

#define FREOPEN

/*
Đề bài
Có n học viên đứng thành hàng; học viên ở vị trí i có mức năng lượng a[i]. Hai học viên đứng cạnh nhau có độ chênh năng lượng bằng trị tuyệt đối hiệu hai mức năng lượng. Có hai loại sự kiện: hai học viên đổi chỗ; hoặc yêu cầu tìm độ chênh nhỏ nhất giữa hai vị trí kề nhau trong một đoạn của hàng hiện tại.

Dữ liệu vào

Dòng đầu chứa số nguyên T (1 ≤ T ≤ 20), là số bộ test.
Với mỗi test:
Dòng đầu gồm n, q (2 ≤ n, q ≤ 200000).
Dòng kế tiếp gồm n số a[i] (|a[i]| ≤ 10^9).
q dòng tiếp theo có dạng:
1 x y: Hai học viên ở vị trí x, y đổi chỗ.
2 l r: Tìm chênh lệch nhỏ nhất của hai học viên kề nhau, với cả hai vị trí nằm trong [l, r] (l < r).
Các vị trí được đánh số từ 1.
Dữ liệu ra

Với mỗi test, xuất kết quả theo yêu cầu dưới đây:
Với mỗi yêu cầu loại 2, in độ chênh nhỏ nhất trên một dòng.

*/





int a[N];

template <typename Node, typename MergeFunc>
class SegmentTree
{
private:
     int n;
     const Node neutral;     

     MergeFunc merge;

public:
     vector<Node> st;
     
     SegmentTree(int n, Node neutral, MergeFunc merge) : n(n), neutral(neutral), merge(merge)
     {
          st.assign(4 * n, neutral);
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
               cin >> a[i];
          }

          auto merge = [](int a, int b){return min(a, b);};
          SegmentTree<int, decltype(merge)> segtree(n, oo, merge);
          
          for (int i = 1; i <= n - 1; i++)
          {
               segtree.update(1, 1, n - 1, i, abs(a[i + 1] - a[i]));
          }

          for (int i = 1; i <= q; i++)
          {
               int type; cin >> type;
               if (type == 1)
               {
                    int x, y; cin >> x >> y;
                    if (x == y) continue;
                    swap(a[x], a[y]);

                    int affected[] = {x - 1, x, y - 1, y};
                    for (int pos : affected)
                    {
                         if (1 <= pos && pos <= n - 1)
                         {
                              segtree.update(1, 1, n - 1, pos, abs(a[pos + 1] - a[pos]));
                         }
                    }

               }
               else
               {
                    int l, r; cin >> l >> r;
                    cout << segtree.get(1, 1, n - 1, l, r - 1) << el;
               }
          }

     }

     

}
