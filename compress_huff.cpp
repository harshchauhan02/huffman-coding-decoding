// Huffman Coding DECODING
#include <cstdlib>
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
// This constant can be avoided by explicitly
// calculating height of Huffman Tree
#include "common.h"
#include "bit_io.h"
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <iostream>
#include <fstream>

#define DEFAULT_INFILE "test-files/hamlet-ascii.txt.huff"
#define DEFAULT_OUTFILE "test-files/hamlet-ascii.txt.puff"

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
    std::priority_queue<node_ptr, std::vector<node_ptr>, compare> q;

    for (auto v = freq.begin(); v != freq.end(); v++)
    {
        node_ptr newone = std::make_shared<huffmannode>(v->first, v->second);
        q.push(newone);
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
void buildfreqtable(std::istream &in)
{
    char c;
    while (in.read(&c, 1))
    {
        freq[c]++;
    }
}
void writecodes(std::istream &in, bostream &out)
{
    char c;
    in.clear();
    in.seekg(0, std::ios::beg);
    while (in.read(&c, 1))
    {
        std::string code = bitcode[c];
        for (int i = 0; i < code.size(); i++)
        {
            out.write_bits(code.at(i), 8);
        }
    }
}
void writefreqtabletofile(bostream &out)
{
    for (auto v = freq.begin(); v != freq.end(); v++)
    {
        char ch = v->first;
        std::string f = std::to_string(v->second);

        out.write_bits(ch, 8);
        out.write_bits(':', 8);
        for (int i = 0; i < f.size(); i++)
        {
            out.write_bits(f.at(i), 8);
        }
        out.write_bits(';', 8);
    }
    out.write_bits('#', 8);
}
int main(int argc, const char *argv[])
{
    const char *infile, *outfile;

    get_file_names(argc, argv, infile, outfile,
                   DEFAULT_INFILE, DEFAULT_OUTFILE);

    std::ifstream in(infile);
    assert_good(in, argv);

    bofstream out(outfile);
    assert_good(out, argv);

    buildfreqtable(in);
    writefreqtabletofile(out);

    createHuffmantree();
    writecodes(in, out);

    return 0;
}
