// https://cses.fi/problemset/task/1742/
#include <bits/stdc++.h>
using namespace std;
#define int long long
#define fastio ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
#define oo (int)(1e18) 
 
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
        st.resize(4 * 200005 + 1, id_Value);
        lazy.resize(4 * 200005 + 1, id_Lazy);
    }
 
    void init_once(int new_n) {
        n = new_n;
    }
 
    void update(int u, int v, LazyType val) {
        update(1, 1, n, u, v, val);
    }
 
    ValueType query(int u, int v) {
        return query(1, 1, n, u, v);
    }
};
 
struct Node {
    int mn, mx;
    bool operator!=(const Node& other) const {
        return mn != other.mn || mx != other.mx;
    }
};
 
struct CombineNode {
    Node operator()(const Node& a, const Node& b) const {
        return {min(a.mn, b.mn), max(a.mx, b.mx)};
    }
};
struct ApplyNode {
    void operator()(Node& val, const Node& lazy, int len) const {
        val = lazy; 
    }
};
struct ComposeNode {
    void operator()(Node& curr, const Node& lazy) const {
        curr = lazy;
    }
};
 
const Node id_val = {oo, -oo};
const Node id_lazy = {oo, -oo};
static LazySegmentTree<Node, Node, CombineNode, ApplyNode, ComposeNode> segtree(id_val, id_lazy, CombineNode{}, ApplyNode{}, ComposeNode{});
 
struct Segment {
    int id;
    int x1, y1, x2, y2;
    bool is_horizontal;
    int dir; 
    int cy1, cy2; 
};
 
struct Event {
    int x;
    int type; 
    int y1, y2;
    int id;
    int seg_idx; // Maps back to original segment index to filter by k
    bool operator < (const Event& other) const {
        if (x != other.x) return x < other.x;
        if (type != other.type) return type < other.type;
        return y1 < other.y1;
    }
};
 
const int MAXN = 200005;
int active_horiz[MAXN]; 
int touch_stamp[MAXN];
int sweep_counter = 0;
vector<int> touched_y;
vector<Event> global_events;
 
bool check(int k, const vector<Segment>& all_segments) {
    for (int i = 1; i < k; i++) {
        int d1 = all_segments[i - 1].dir;
        int d2 = all_segments[i].dir;
        if ((d1 == 0 && d2 == 1) || (d1 == 1 && d2 == 0) ||
            (d1 == 2 && d2 == 3) || (d1 == 3 && d2 == 2)) return true;
    }
 
    touched_y.clear();
    sweep_counter++;
    int prev_x = -oo, prev_y2 = -oo, prev_id = -1;
 
    auto cleanup = [&]() {
        for (int y : touched_y) {
            active_horiz[y] = -1;
            segtree.update(y, y, id_val);
        }
        touched_y.clear();
    };
 
    // Iterate through pre-sorted global events, filtering only those belonging to segments < k
    for (const auto& ev : global_events) {
        if (ev.seg_idx >= k) continue; 
 
        int y_idx = ev.y1; 
        
        if (ev.type == 0) {
            if (active_horiz[y_idx] != -1) {
                int existing_id = active_horiz[y_idx];
                if (abs(existing_id - ev.id) >= 2) {
                    cleanup();
                    return true;
                }
            } else {
                if (touch_stamp[y_idx] != sweep_counter) {
                    touch_stamp[y_idx] = sweep_counter;
                    touched_y.push_back(y_idx);
                }
            }
            active_horiz[y_idx] = ev.id;
            segtree.update(y_idx, y_idx, {ev.id, ev.id});
            
        } else if (ev.type == 1) {
            if (ev.x == prev_x) {
                if (ev.y1 <= prev_y2) {
                    if (abs(prev_id - ev.id) >= 2) {
                        cleanup();
                        return true;
                    }
                }
                if (ev.y2 > prev_y2) {
                    prev_y2 = ev.y2;
                    prev_id = ev.id;
                }
            } else {
                prev_x = ev.x;
                prev_y2 = ev.y2;
                prev_id = ev.id;
            }
            
            Node res = segtree.query(ev.y1, ev.y2);
            if (res.mn <= ev.id - 2 || res.mx >= ev.id + 2) {
                cleanup();
                return true;
            }
            
        } else if (ev.type == 2) {
            if (active_horiz[y_idx] == ev.id) {
                active_horiz[y_idx] = -1;
                segtree.update(y_idx, y_idx, id_val);
            }
        }
    }
    cleanup();
    return false;
}
 
bool check_last(int k, const vector<Segment>& segs) {
    if (k <= 1) return false;
    Segment last = segs[k - 1];
    
    int d1 = segs[k - 2].dir;
    int d2 = last.dir;
    if ((d1 == 0 && d2 == 1) || (d1 == 1 && d2 == 0) ||
        (d1 == 2 && d2 == 3) || (d1 == 3 && d2 == 2)) return true;
        
    int lx_min = min(last.x1, last.x2);
    int lx_max = max(last.x1, last.x2);
    int ly_min = min(last.y1, last.y2);
    int ly_max = max(last.y1, last.y2);
 
    for (int i = 0; i < k - 2; i++) {
        Segment p = segs[i];
        int px_min = min(p.x1, p.x2);
        int px_max = max(p.x1, p.x2);
        int py_min = min(p.y1, p.y2);
        int py_max = max(p.y1, p.y2);
        
        if (lx_min > px_max || lx_max < px_min) continue;
        if (ly_min > py_max || ly_max < py_min) continue;
        
        return true;
    }
    return false;
}
 
void solve() {
    int n; 
    if (!(cin >> n)) return;
 
    int cx = 0, cy = 0;
    vector<Segment> segment(n);
    vector<int> all_y;
    all_y.reserve(2 * n);
 
    for (int i = 0; i < n; i++) {
        char d; int x; cin >> d >> x;
        int nx = cx, ny = cy;
        int dir = 0;
        switch (d) {
            case 'U': ny += x; dir = 0; break;
            case 'D': ny -= x; dir = 1; break;
            case 'L': nx -= x; dir = 2; break;
            case 'R': nx += x; dir = 3; break;
        }
        segment[i] = {i, cx, cy, nx, ny, (ny == cy), dir, 0, 0};
        all_y.push_back(cy); all_y.push_back(ny);
        cx = nx; cy = ny;
    }
 
    sort(all_y.begin(), all_y.end());
    all_y.erase(unique(all_y.begin(), all_y.end()), all_y.end());
 
    for (int i = 0; i < n; i++) {
        segment[i].cy1 = lower_bound(all_y.begin(), all_y.end(), segment[i].y1) - all_y.begin() + 1;
        segment[i].cy2 = lower_bound(all_y.begin(), all_y.end(), segment[i].y2) - all_y.begin() + 1;
    }
 
    global_events.clear();
    for (int i = 0; i < n; i++) {
        const Segment& s = segment[i];
        if (s.is_horizontal) {
            global_events.push_back({min(s.x1, s.x2), 0, s.cy1, s.cy1, s.id, i});
            global_events.push_back({max(s.x1, s.x2), 2, s.cy1, s.cy1, s.id, i});
        } else {
            global_events.push_back({s.x1, 1, min(s.cy1, s.cy2), max(s.cy1, s.cy2), s.id, i});
        }
    }
    sort(global_events.begin(), global_events.end());
 
    segtree.init_once(all_y.size());
    memset(active_horiz, -1, sizeof(active_horiz));
    memset(touch_stamp, 0, sizeof(touch_stamp));
 
    int l = 1, r = n, valid_k = 0;
    while (l <= r) {
        int mid = (l + r) >> 1;
        if (check(mid, segment)) {
            r = mid - 1;
        } else {
            valid_k = mid;
            l = mid + 1;
        }
    }
    
    int dist = 0;
    for (int i = 0; i < valid_k; i++) {
        dist += abs(segment[i].x2 - segment[i].x1) + abs(segment[i].y2 - segment[i].y1);
    }
 
    if (valid_k == n) {
        cout << dist << "\n";
        return;
    }
    
    Segment bad = segment[valid_k];
    int ld = 0, rd = abs(bad.x2 - bad.x1) + abs(bad.y2 - bad.y1), add_d = 0;
    while (ld <= rd) {
        int md = (ld + rd) >> 1;
        Segment partial = bad;
        
        if (bad.is_horizontal) partial.x2 = bad.x1 + (bad.x1 < bad.x2 ? md : -md);
        else partial.y2 = bad.y1 + (bad.y1 < bad.y2 ? md : -md);
 
        Segment original = segment[valid_k];
        segment[valid_k] = partial;
 
        if (check_last(valid_k + 1, segment)) {
            add_d = md;
            rd = md - 1;
        } else {
            ld = md + 1;
        }
        segment[valid_k] = original; 
    }
 
    dist += add_d;
    cout << dist << "\n";
}
 
signed main() {
    fastio;
    solve();
    return 0;
}
