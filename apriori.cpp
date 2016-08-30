/* */
#include<bits/stdc++.h>

#define PB push_back
#define MP make_pair
#define F first
#define S second
#define UB(X,v) upper_bound(X.begin(), X.end(), v)
#define LB(X,v) lower_bound(X.begin(), X.end(), v)

#define RI(a) scanf("%d", &a)
#define RIL(a) scanf("%lld", &a)
#define PI(a) printf("%d\n", a)
#define PIL(a) printf("%lld\n", a)

#define SZ(a) (int)(a.size())
#define CLR(a) a.clear()

#define SET(a,b) memset(a, b, sizeof(a))
#define LET(a,b) __typeof(a.begin()) b;

#define TR(a,it) for(__typeof(a.begin()) it = a.begin(); it != a.end(); it++)

#define REP(i,l,h) for(int i=(l); i <= (h); i++)
#define DEP(i,h,l) for(int i=(h);i >= (l); i--)

#define ALL(a) a.begin(), a.end()

#define TC()  int tc; cin >> tc; for(int tc_ctr=1; tc_ctr<=tc; tc_ctr++)

#define PRSNT(a, e) (a.find(e) != a.end())

#define MINH priority_queue<int, vector<int>, greater<int> >

#define N 100001
#define MOD 1000000007

using namespace std;

#define DBG(...) __f(#__VA_ARGS__, __VA_ARGS__)
template <typename Arg1>
void __f(const char* name, Arg1&& arg1) {
    cerr << name << " : " << arg1 << std::endl;
}
template <typename Arg1, typename... Args>
void __f(const char* names, Arg1&& arg1, Args&&... args) {
    const char* comma = strchr(names + 1, ',');cerr.write(names, comma - names) << " : " << arg1<<" | ";__f(comma+1, args...);
}

typedef long long LL;
typedef pair<int, int> PII;
typedef vector< vector<int> > VVI;
typedef vector< pair<int, int> > VPII;
typedef vector<int> VI;

class setEnumerationTree {
	string val;
	int count;
	

};
int main()
{
    return 0;
}