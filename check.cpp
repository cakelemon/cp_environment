#include <bits/stdc++.h>
//#include <atcoder/fenwicktree>
#define ll long long
#define pll std::pair<ll, ll>
#define pii std::pair<int, int>
#define INF 0x7f7f7f7f;
#define MOD 998244353 // (ll)(1e9 + 7)
#define all(x) (x).begin(), (x).end()
#define FIO cin.tie(0); ios::sync_with_stdio(0)
#define MAXN 1000100
#define _USE_MATH_DEFINES 
using namespace std;
//using namespace atcoder;
//using mint = modint998244353;

/* ----------------- */

/* ----------------- */


void solve() {
    ifstream fin("input_file", ifstream::in);
    ifstream ans("my_answer", ifstream::in);

    int a, b; fin >>a>>b;
    int c; ans>>c;

    cout << (c == a + b ? "OK" : "Wrong sum");
}
    
int main() {
    FIO;
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
#endif
    // int t; cin>>t;
    // while(t--)
    solve();
}