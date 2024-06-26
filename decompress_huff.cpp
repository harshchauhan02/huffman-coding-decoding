#include "common.h"
#include "bit_io.h"
#include <map>
#include <queue>
#include <iostream>
#include <sstream>
// don't stick to an approach 
// #include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <cassert>
#include <random>
#include <fstream>
#include <complex>
#include <list>
#include <chrono>
#include <cstring>

using namespace std;

#define gc getchar_unlocked
#define forn(i,n) for(int i = 0; i < int(n); i++)
#define ford(i,n) for(int i = int(n) - 1; i >= 0; i--)
#define fore(i,a,b) for(int i = int(a); i <= int(b); i++)
#define foreach(it,a) for(__typeof((a).begin()) it = (a).begin(); it != (a).end(); it++)

#define ll long long
#define deb(x) cout << #x << "=" << x << endl
#define deb2(x, y) cout << #x << "=" << x << "," << #y << "=" << y << endl
#define pb push_back
#define mp make_pair
#define F first
#define S second
#define sz size()
#define clr(x) memset(x, false, sizeof(x))
#define sortall(x) sort(all(x))
#define tr(it, a) for (auto it = a.begin(); it != a.end(); it++)

#define PI 3.1415926535897932384626
#define PYES cout << "YES\n"
#define PNO cout << "NO\n"
#define PYes cout << "Yes\n"
#define PNo cout << "No\n"
#define sq(x) (x * x)
typedef pair<int, int> pii;
typedef pair<string, int> psi;
typedef pair<ll, ll> pl;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<pii> vpii;
typedef vector<pl> vpl;
typedef vector<vi> vvi;
typedef vector<vl> vvl;

#define rev(v) reverse(v.begin(), v.end())
#define srt(v) sort(v.begin(), v.end())
#define grtsrt(v) sort(v.begin(), v.end(), greater<ll>())
#define mnv(v) *min_element(v.begin(), v.end())
#define mxv(v) *max_element(v.begin(), v.end())
#define all(v) v.begin(), v.end()
#define ACC(v) accumulate(v.begin(), v.end(), 0ll)

#define MOD 1000000007
#define EPS 1e-9
#define nl '\n'; 

#define DEFAULT_INFILE "test-files/hamlet-ascii.txt.huff"
#define DEFAULT_OUTFILE "test-files/hamlet-ascii.txt.out"
struct huffmannode;
using node_ptr = std::shared_ptr<huffmannode>;
using struct_node_ptr = std::shared_ptr<struct huffmannode>;

using namespace ipd;
std::map<char, std::string> bitcode;
std::map<char, size_t> freq;
struct huffmannode
{
    char c;
    size_t freq;
    node_ptr left, right;

    huffmannode(char c, size_t freq)
    {
        left = right = nullptr;
        this->c = c;
        this->freq = freq;
    }
};
struct compare
{
    bool operator()(node_ptr left, node_ptr right)
    {
        return (left->freq > right->freq);
    }
};
std::priority_queue<node_ptr, std::vector<node_ptr>, compare> q;
void storecodes(struct_node_ptr root, std::string str)
{
    if (root == nullptr)
    {
        return;
    }
    if (root->c != '#')
        bitcode[root->c] = str;
    storecodes(root->left, str + "0");
    storecodes(root->right, str + "1");
}

void createHuffmantree()
{
    struct_node_ptr left, right, top;
    for (auto v = freq.begin(); v != freq.end(); v++)
    {
        q.push(std::make_shared<huffmannode>(v->first, v->second));
    }
    while (q.size() != 1)
    {
        left = q.top();
        q.pop();
        right = q.top();
        q.pop();
        top = std::make_shared<huffmannode>('#', left->freq + right->freq);
        top->left = left;
        top->right = right;
        q.push(top);
    }
    storecodes(q.top(), "");
}

void getFreq(bifstream &in)
{
    char c;
    std::string str;
    while (in.read_bits(c, 8))
    {
        if (c == '#')
            break;
        str += c;
    }

    for (int i = 0; i < str.size(); i++)
    {

        if (i > 0 && str.at(i) == ':' && str.at(i + 1) != ':')
        {
            char ch = str.at(i - 1);
            std::string tmp;
            while (str.at(i + 1) != ';')
            {
                tmp += str.at(i + 1);
                i++;
            }
            std::stringstream tmps(tmp);
            size_t fre = 0;
            tmps >> fre;
            freq[ch] = fre;
        }
    }
}

void decode(struct_node_ptr root, bifstream &in, std::ostream &out)
{
    char c;
    struct_node_ptr curr = root;
    while (in.read_bits(c, 8))
    {

        if (c == '0')
        {
            curr = curr->left;
        }
        else
        {
            curr = curr->right;
        }
        if (curr->left == nullptr && curr->right == nullptr)
        {
            out << curr->c;
            curr = root;
        }
    }
}
int main(int argc, const char *argv[])
{
    const char *infile, *outfile;

    get_file_names(argc, argv, infile, outfile,
                   DEFAULT_INFILE, DEFAULT_OUTFILE);

    bifstream in(infile);
    assert_good(in, argv);

    std::ofstream out(outfile);
    assert_good(out, argv);

    getFreq(in);

    createHuffmantree();
    decode(q.top(), in, out);
    return 0;
}
