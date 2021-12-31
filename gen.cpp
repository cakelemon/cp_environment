#include <bits/stdc++.h>
#define ll long long
#define pll std::pair<ll, ll>
#define pii std::pair<int, int>
#define uid(a, b) uniform_int_distribution<int>(a, b)(rng)

using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

void solve() {
    int n = uid(1, 5);
    int m = uid(1, 5);
    printf("%d %d\n", n, m);
}

int main() {
    int t = 1;
    while(t--)
        solve();
}