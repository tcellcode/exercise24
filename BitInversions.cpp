// https://cses.fi/problemset/task/1188/
#include <bits/stdc++.h>
using namespace std;
#define int long long 
#define N (int)(1e6+5)
#define fou(i, a, b, s) for(int i = a; i <= b; i += s)
#define MOD (int)(1e9+7)
#define oo (int)(1e18)
#define el '\n'
#define fastio ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);



template <typename ValueType, typename LazyType>
class LazySegmentTree {
private:
    int n;
    std::vector<ValueType> st;
    std::vector<LazyType> lazy;
    
    ValueType id_Value; // Identity element for the node (e.g., 0 for sum)
    LazyType id_Lazy;   // Identity element for the lazy tag (e.g., 0 for "no pending update")

    // Function to merge two child nodes
    std::function<ValueType(ValueType, ValueType)> combine;
    // Function to apply a lazy tag to a node
    std::function<void(ValueType&, LazyType, int)> apply_op;
    // Function to merge a new lazy tag into an existing one
    std::function<void(LazyType&, LazyType)> compose_op;

    void build(int id, int l, int r, const std::vector<ValueType>& arr) {
        if (l == r) {
            
            st[id] = arr[l]; 
            return;
        }
        int mid = (l + r) >> 1;
        build(2 * id, l, mid, arr);
        build(2 * id + 1, mid + 1, r, arr);
        st[id] = combine(st[2 * id], st[2 * id + 1]);
    }

    void push(int id, int l, int r) {
        if (lazy[id] != id_Lazy) {
            int mid = (l + r) >> 1;
            
            // Apply to left child
            apply_op(st[2 * id], lazy[id], mid - l + 1);
            compose_op(lazy[2 * id], lazy[id]);
            
            // Apply to right child
            apply_op(st[2 * id + 1], lazy[id], r - mid);
            compose_op(lazy[2 * id + 1], lazy[id]);
            
            // Clear current node's lazy tag
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
        update(2 * id, l, mid, u, v, val);
        update(2 * id + 1, mid + 1, r, u, v, val);
        st[id] = combine(st[2 * id], st[2 * id + 1]);
    }

    ValueType query(int id, int l, int r, int u, int v) {
        if (r < u || v < l) return id_Value;
        if (u <= l && r <= v) return st[id];
        push(id, l, r);
        int mid = (l + r) >> 1;
        return combine(
            query(2 * id, l, mid, u, v),
            query(2 * id + 1, mid + 1, r, u, v)
        );
    }

public:
    LazySegmentTree(const std::vector<ValueType>& arr, 
                    ValueType identity_Value, LazyType identity_Lazy,
                    std::function<ValueType(ValueType, ValueType)> combine_func,
                    std::function<void(ValueType&, LazyType, int)> apply_func,
                    std::function<void(LazyType&, LazyType)> compose_func) 
        : id_Value(identity_Value), id_Lazy(identity_Lazy), 
          combine(combine_func), apply_op(apply_func), compose_op(compose_func) {
        
        n = arr.size() - 1;
        // 1-based indexing for internal array, size 4*N is safe upper bound
        st.assign(4 * n + 1, id_Value);
        lazy.assign(4 * n + 1, id_Lazy);
        if (n > 0) {
            build(1, 1, n, arr); // Build using 1-indexed bounds [1, n]
        }
    }

    // 1-indexed public wrappers
    void update(int u, int v, LazyType val) {
        update(1, 1, n, u, v, val);
    }

    ValueType query(int u, int v) {
        return query(1, 1, n, u, v);
    }
};





struct Node
{
    int pref0;
    int pref1;
    int suff0;
    int suff1;
    int len;
    int max0;
    int max1;
};

int n, m;
string s;
void solve()
{
    cin >> s;
    n = s.size() + 1;

    vector<Node> a(s.size() + 1);
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '0')
        {
            a[i + 1] = {1, 0, 1, 0, 1, 1, 0};
        }
        else 
        {
            a[i + 1] = {0, 1, 0, 1, 1, 0, 1};
        }
        
    }

    

    auto combine = [](Node l, Node r) -> Node
    {
        if (l.len == 0) return r;
        if (r.len == 0) return l;
        Node node;
        
        node.pref0 = l.pref0 + (l.pref0 == l.len ? r.pref0 : 0);
        node.pref1 = l.pref1 + (l.pref1 == l.len ? r.pref1 : 0);

        node.suff0 = r.suff0 + (r.suff0 == r.len ? l.suff0 : 0);
        node.suff1 = r.suff1 + (r.suff1 == r.len ? l.suff1 : 0);

        node.len = l.len + r.len;
        
        node.max0 = max({l.max0, r.max0, l.suff0 + r.pref0});
        node.max1 = max({l.max1, r.max1, l.suff1 + r.pref1});
        
        return node;
    };
    
    auto apply = [](Node& node, int val, int len)
    {
        if (val == 1)
        {
            swap(node.pref0, node.pref1);
            swap(node.suff0, node.suff1);
            swap(node.max0, node.max1);
        }
    };

    auto compose = [](int& current_lazy, int new_lazy)
    {
        current_lazy = (current_lazy + new_lazy) % 2;
    };

    LazySegmentTree<Node, int> segtree(
        a, Node{0, 0, 0, 0, 0, 0}, oo, combine, apply, compose
    );

    cin >> m;
    for (int i = 1; i <= m; i++)
    {
        int x; cin >> x;
        segtree.update(x, x, 1);
        cout << max({
            segtree.query(1, n).max0,
            segtree.query(1, n).max1
        }) << " ";
    }

    


}


signed main()
{
    fastio;
    solve();
    return 0;
}
