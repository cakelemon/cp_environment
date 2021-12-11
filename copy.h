#pragma once
#include <vector>
#include <algorithm>
#include <cassert>

#define _USE_MATH_DEFINES 
#define ll long long
#define MOD 1000000007

using namespace std;

ll pos_m(ll a) { return a >= 0 ? a : a + MOD; }
ll add_m(ll a, ll b) { return pos_m((a + b) % MOD); }
ll mul_m(ll a, ll b) { return pos_m((a * b) % MOD); }

ll pow_m(ll a, ll n) {
	ll r = 1;
	for (ll b = n; b > 0; b >>= 1, a = mul_m(a, a))
		if (b & 1) r = mul_m(r, a);
	return r;
}

ll inv_m(ll a) { return pow_m(a, MOD - 2); }
ll Frac(ll a, ll b) { return mul_m(a, inv_m(b)); }
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }

ll m_comb[510][510];
ll comb(ll n, ll r) {
	if (r == 0 || r == n) return 1;
	if (m_comb[n][r] != -1)
		return m_comb[n][r];
	m_comb[n][r] = add_m(comb(n - 1, r - 1), comb(n - 1, r));
	return m_comb[n][r];
}

void phi_1_to_n(int n) {
	vector<int> phi(n + 1);
	phi[0] = 0;
	phi[1] = 1;
	for (int i = 2; i <= n; i++)
		phi[i] = i;

	for (int i = 2; i <= n; i++) {
		if (phi[i] == i) {
			for (int j = i; j <= n; j += i)
				phi[j] -= phi[j] / i;
		}
	}
}

// fenw tree
ll sum(vector<long long>& treeM, int i) {
	ll ans = 0;
	while (i > 0) {
		ans += treeM[i];
		i -= (i & -i);
	}
	return ans;
}

void update(vector<long long>& treeM, int i, int num) {
	while (i < treeM.size()) {
		treeM[i] += num;
		i += (i & -i);
	}
}

/* -- segment tree -- */
template <class T>
struct segtree {
	int _segR;
	vector<T> _segtree;
	function<T(T, T)> _combine;
	T _defaultValue;

	// initArr: 1 based array
public:
	segtree(vector<T>& initArr, function<T(T, T)> combine, T defaultValue) {
		assert(initArr.size() > 1 && initArr[0] == 0);
		_segR = initArr.size() - 1;
		_segtree = vector<T>(initArr.size() * 4);
		_combine = combine;
		_defaultValue = defaultValue;
		init(1, 1, _segR, initArr);
	}

	T query(int start, int end, int node = 1, int l = 0, int r = 0) {
		if (node == 1) {
			l = 1;
			r = _segR;
		}

		if (start <= l && r <= end)
			return _segtree[node];
		if (r < start || end < l)
			return _defaultValue;
		int m = (l + r) / 2;
		T lVal = query(start, end, 2 * node, l, m);
		T rVal = query(start, end, 2 * node + 1, m + 1, r);
		return _combine(lVal, rVal);
	}

	void update(int index, T delta, int node = 1, int l = 0, int r = 0) {
		if (node == 1) {
			l = 1;
			r = _segR;
		}

		if (l <= index && index <= r) {
			_segtree[node] = _combine(_segtree[node], delta);
		}
		if (index < l || r < index || l == r)
			return;
		int m = (l + r) / 2;
		update(index, delta, 2 * node, l, m);
		update(index, delta, 2 * node + 1, m + 1, r);
	}

	void fixedUpdate(int index, T newValue, int node = 1, int l = 0, int r = 0) {
		if (node == 1) {
			l = 1;
			r = _segR;
		}
		if (index < l || r < index)
			return;
		if (l == r) {
			_segtree[node] = newValue;
			return;
		}
		int m = (l + r) / 2;
		fixedUpdate(index, newValue, 2 * node, l, m);
		fixedUpdate(index, newValue, 2 * node + 1, m + 1, r);
		if (l <= index && index <= r) {
			T lVal = query(l, r, 2 * node, l, m);
			T rVal = query(l, r, 2 * node + 1, m + 1, r);
			_segtree[node] = _combine(lVal, rVal);
		}
	}

private:
	T init(int node, int l, int r, vector<T>& initArr) {
		if (l == r) {
			return _segtree[node] = initArr[l];
		}
		int m = (l + r) / 2;
		T lVal = init(2 * node, l, m, initArr);
		T rVal = init(2 * node + 1, m + 1, r, initArr);
		return _segtree[node] = _combine(lVal, rVal);
	}
};

/* -- segment tree w. lazy propagation -- */
template <class T>
struct lazysegtree {
	int _segR;
	vector<T> _segtree;
	vector<T> _lazy;
	function<T(T, T)> _combine;
	// delta 가 int 번 적용된 값 계산
	function<T(T, int)> _deltaMult;
	T _defaultValue;

public:
	// initArr: 1 based array
	lazysegtree(vector<T>& initArr, function<T(T, T)> combine, function<T(T, int)> lazyDelta, T defaultValue) {
		assert(initArr.size() > 1 && initArr[0] == 0);
		_segR = initArr.size() - 1;
		_combine = combine;
		_deltaMult = lazyDelta;
		_defaultValue = defaultValue;
		_segtree = vector<T>(initArr.size() * 4);
		_lazy = vector<T>(initArr.size() * 4, defaultValue);
		init(1, 1, _segR, initArr);
	}

	T query(int start, int end, int node = 1, int l = 0, int r = 0) {
		if (node == 1) {
			l = 1;
			r = _segR;
		}
		int m = (l + r) / 2;
		if (_lazy[node] != _defaultValue) {
			if (l != r) {
				updateRange(l, m, _lazy[node], node * 2, l, m);
				updateRange(m + 1, r, _lazy[node], node * 2 + 1, m + 1, r);
			}
			_segtree[node] = _combine(_segtree[node], _deltaMult(_lazy[node], r - l + 1));
			_lazy[node] = _defaultValue;
		}

		if (start <= l && r <= end)
			return _segtree[node];
		if (r < start || end < l)
			return _defaultValue;
		T lVal = query(start, end, 2 * node, l, m);
		T rVal = query(start, end, 2 * node + 1, m + 1, r);
		return _combine(lVal, rVal);
	}

	void updateRange(int s, int e, T delta, int node = 1, int l = 0, int r = 0) {
		if (node == 1) {
			l = 1;
			r = _segR;
		}

		if (s <= l && r <= e) {
			_lazy[node] = _combine(_lazy[node], delta);
			return;
		}
		if (e < l || r < s)
			return;

		int len = min(r, e) - max(l, s) + 1;
		_segtree[node] = _combine(_segtree[node], _deltaMult(delta, len));

		int m = (l + r) / 2;
		updateRange(s, e, delta, 2 * node, l, m);
		updateRange(s, e, delta, 2 * node + 1, m + 1, r);
	}

private:
	T init(int node, int l, int r, vector<T>& initArr) {
		if (l == r) {
			return _segtree[node] = initArr[l];
		}
		int m = (l + r) / 2;
		T lVal = init(2 * node, l, m, initArr);
		T rVal = init(2 * node + 1, m + 1, r, initArr);
		return _segtree[node] = _combine(lVal, rVal);
	}
};

// tree that supports LCA
struct tree {
	vector<vector<int>> _adj;
	vector<vector<int>> _pa;
	vector<int> _depth;
	int _n;
	bool _canLCA = false;

public:
	tree(int n) {
		_n = n;
		_adj = vector<vector<int>>(n + 1);
	}

	void connect(int a, int b) {
		_adj[a].push_back(b);
		_adj[b].push_back(a);
	}

	void init_lca(int root) {
		_canLCA = true;
		_depth = vector<int>(_n + 1);

		int lg = 0;
		while ((1 << lg) <= _n) lg++;
		_pa = vector<vector<int>>(_n + 1, vector<int>(lg + 1));

		dfs_lca(root, 0, 0);
		for (int i = 1; i <= lg; i++) {
			for (int node = 1; node <= _n; node++) {
				_pa[node][i] = _pa[_pa[node][i - 1]][i - 1];
			}
		}
	}

	int LCA(int a, int b) {
		assert(_canLCA);
		if (_depth[a] > _depth[b]) swap(a, b); // depth[b] > depth[a]

		for (int j = _pa[0].size() - 1; j >= 0; j--) {
			if (_depth[b] - _depth[a] >= (1 << j)) b = _pa[b][j];
		}

		if (a == b) return a;
		for (int j = _pa[0].size() - 1; j >= 0; j--) {
			if (_pa[a][j] != _pa[b][j]) {
				a = _pa[a][j];
				b = _pa[b][j];
			}
		}
		return _pa[a][0];
	}

private:
	void dfs_lca(int here, int pa, int depth) {
		_pa[here][0] = pa;
		_depth[here] = depth;

		for (int there : _adj[here]) {
			if (there == pa) continue;

			dfs_lca(there, here, depth + 1);
		}
	}
};

/* --- binary matching --- */
const int CUR_SIZE = 300;
vector<int> adj[CUR_SIZE];

bool vis[CUR_SIZE];
int match[CUR_SIZE];
bool dfs(int here) {
	if (vis[here])
		return false;
	vis[here] = true;
	for (int there : adj[here]) {
		if (match[there] == 0 || dfs(match[there])) {
			match[there] = here;
			return true;
		}
	}
	return false;
}

