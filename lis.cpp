#include <bits/stdc++.h>
using namespace std;
#define int long long
#define fastio ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
#define oo (int)(1e18)
#define N (int)(1e6 + 5)
#define el '\n'


template <typename ValueType, typename LazyType,         
          typename CombineFunc, typename ApplyFunc, typename ComposeFunc>
class LazySegmentTree {
private:
    int n;
    std::vector<ValueType> st;
    std::vector<LazyType> lazy;
    
    ValueType id_Value; 
    LazyType id_Lazy;    

    CombineFunc combine;
    ApplyFunc apply_op;
    ComposeFunc compose_op;

    void build(int id, int l, int r, const std::vector<ValueType>& arr) {
        lazy[id] = id_Lazy; // Clear lazy state for multiple testcases
        if (l == r) {
            st[id] = arr[l]; 
            return;
        }
        int mid = (l + r) >> 1;
        build(id << 1, l, mid, arr);
        build((id << 1) | 1, mid + 1, r, arr);
        st[id] = combine(st[id << 1], st[(id << 1) | 1]);
    }

    void push(int id, int l, int r) {
        if (lazy[id] != id_Lazy) {
            int mid = (l + r) >> 1;
            
            apply_op(st[id << 1], lazy[id], mid - l + 1);
            compose_op(lazy[id << 1], lazy[id]);
            
            apply_op(st[(id << 1) | 1], lazy[id], r - mid);
            compose_op(lazy[(id << 1) | 1], lazy[id]);
            
            lazy[id] = id_Lazy;
        }
    }

    void update(int id, int l, int r, int u, int v, LazyType val) {
        if (r < u || v < l) return;
        if (u <= l && r <= v) {
            apply_op(st[id], val, r - l + 1);
            compose_op(lazy[id], val);
            return;
        }
        push(id, l, r);
        int mid = (l + r) >> 1;
        update(id << 1, l, mid, u, v, val);
        update((id << 1) | 1, mid + 1, r, u, v, val);
        st[id] = combine(st[id << 1], st[(id << 1) | 1]);
    }

    ValueType query(int id, int l, int r, int u, int v) {
        if (r < u || v < l) return id_Value;
        if (u <= l && r <= v) return st[id];
        push(id, l, r);
        int mid = (l + r) >> 1;
        return combine(
            query(id << 1, l, mid, u, v),
            query((id << 1) | 1, mid + 1, r, u, v)
        );
    }

public:
    LazySegmentTree(ValueType identity_Value, LazyType identity_Lazy,
                    CombineFunc combine_func, ApplyFunc apply_func, ComposeFunc compose_func) 
        : n(0), id_Value(identity_Value), id_Lazy(identity_Lazy), 
          combine(combine_func), apply_op(apply_func), compose_op(compose_func) {
        // Expanded to 500005 to meet the maximum N limit of 5 * 10^5
        st.resize(4 * 500005 + 1, id_Value); 
        lazy.resize(4 * 500005 + 1, id_Lazy);
    }

    void init_once(int new_n) {
        n = new_n;
    }

    void build(const std::vector<ValueType>& arr) {
        build(1, 1, n, arr);
    }

    void update(int u, int v, LazyType val) {
        update(1, 1, n, u, v, val);
    }

    ValueType query(int u, int v) {
        return query(1, 1, n, u, v);
    }
};


int n, lds[N], lis[N], LIS[N], LDS[N];
vector<int> a(N), val;

/*
lis + a[i] + lds
i -> n       i -> n

*/

void solve()
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
        val.push_back(a[i]);
    }

    sort(val.begin(), val.end());
    val.erase(unique(val.begin(), val.end()), val.end());

    int m = val.size();

    auto get_rank = [&](int x) { return lower_bound(val.begin(), val.end(), x) - val.begin() + 1; };
    auto combine = [](int a, int b) {return max(a, b); };
    auto apply = [](int& val, int lazy, int len) {val = max(val, lazy); };
    auto compose = [](int& curr, int lazy) {curr = max(curr, lazy); };

    LazySegmentTree<int, int, decltype(combine), decltype(apply), decltype(compose)> stlds(0, 0, combine, apply, compose);
    stlds.init_once(m + 5);

    LazySegmentTree<int, int, decltype(combine), decltype(apply), decltype(compose)> stlis(0, 0, combine, apply, compose);
    stlis.init_once(m + 5);

    for (int i = n; i >= 1; i--)
    {
        int r = get_rank(a[i]);
        
        int mxlds = stlds.query(1, r - 1);
        int currlds = mxlds + 1;
        stlds.update(r, r, currlds);
        lds[r] = max(lds[r], currlds);

        int mxlis = stlis.query(r + 1, m);
        int currlis = mxlis + 1;
        stlis.update(r, r, currlis);
        lis[r] = max(lis[r], currlis);
    }

    for (int i = 1; i <= m; i++)
    {
        LDS[i] = max(LDS[i - 1], lds[i]);
    }
    
    for(int i = m; i >= 1; i--)
    {
        LIS[i] = max(LIS[i + 1], lis[i]);
    }

    int ans = 0;
    for (int i = 1; i <= m; i++)
    {
        ans = max(ans, LDS[i] + LIS[i + 1]);
    }

    cout << ans;

}

signed main() 
{
    fastio;
    solve();
    return 0;
}
