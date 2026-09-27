// https://oj.vnoi.info/problem/area (Cách 2)
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

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
            
            // Apply to left child
            apply_op(st[id << 1], lazy[id], mid - l + 1);
            compose_op(lazy[id << 1], lazy[id]);
            
            // Apply to right child
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
    LazySegmentTree(const std::vector<ValueType>& arr, 
                    ValueType identity_Value, LazyType identity_Lazy,
                    CombineFunc combine_func, ApplyFunc apply_func, ComposeFunc compose_func) 
        : id_Value(identity_Value), id_Lazy(identity_Lazy), 
          combine(combine_func), apply_op(apply_func), compose_op(compose_func) {
        
        n = arr.size() - 1;
        st.assign(4 * n + 1, id_Value);
        lazy.assign(4 * n + 1, id_Lazy);
        if (n > 0) {
            build(1, 1, n, arr);
        }
    }

    void update(int u, int v, LazyType val) {
        update(1, 1, n, u, v, val);
    }

    ValueType query(int u, int v) {
        return query(1, 1, n, u, v);
    }
};

struct Event {
    long long x, y1, y2;
    int val;
    bool operator < (const Event& other) const {
        return x < other.x;
    }
};

struct Node {
    int min_val;
    int min_cnt;
};

const int MAX_Y = 30000;

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0);
    
    int n;
    if (!(cin >> n)) return 0;

    vector<Event> events;
    for (int i = 0; i < n; i++) {
        long long x1, y1, x2, y2; 
        cin >> x1 >> y1 >> x2 >> y2;
        events.push_back({x1, y1, y2, 1});
        events.push_back({x2, y1, y2, -1});
    }
    sort(events.begin(), events.end());

    auto combine_func = [](Node left, Node right) -> Node {
        if (left.min_val < right.min_val) return left;
        if (right.min_val < left.min_val) return right;
        return {left.min_val, left.min_cnt + right.min_cnt};
    };

    auto apply_func = [](Node& node, int lazy_val, int len) {
        node.min_val += lazy_val; 
    };

    auto compose_func = [](int& current_lazy, int new_lazy) {
        current_lazy += new_lazy;
    };

    // Khởi tạo trục Y: 30000 đoạn đơn vị, ban đầu đều có lớp phủ = 0, số lượng = 1
    vector<Node> arr(MAX_Y + 1, {0, 1});
    Node id_val = {(int)1e9, 0}; 
    int id_lazy = 0;             

    LazySegmentTree<Node, int, decltype(combine_func), decltype(apply_func), decltype(compose_func)> 
        st(arr, id_val, id_lazy, combine_func, apply_func, compose_func);

    long long total_area = 0;
    for (size_t i = 0; i < events.size() - 1; i++) {
        // Cập nhật lớp phủ cho đoạn [y1, y2]
        // Vì làm việc với đoạn đơn vị, y1 -> y2 tương ứng với các index từ y1 + 1 đến y2
        if (events[i].y1 < events[i].y2) {
            st.update(events[i].y1 + 1, events[i].y2, events[i].val);
        }

        // Đọc trạng thái toàn cục của trục Y
        Node root = st.query(1, MAX_Y);
        
        // Tính chiều cao bị che khuất
        long long active_len = MAX_Y;
        if (root.min_val == 0) {
            active_len -= root.min_cnt;
        }

        // Tính chiều rộng vệt quét và cộng dồn diện tích
        long long dx = events[i + 1].x - events[i].x;
        total_area += dx * active_len;
    }

    cout << total_area << "\n";
    return 0;
}
