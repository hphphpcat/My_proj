#include <algorithm>
#include <cstdio>
#include <queue>
#include <iostream>
#include <bits/stdc++.h>
#define Re register int
#define LL long long
using namespace std;
const int N = 1e7 + 5, M = 1e7 + 3;
int n, t;
char ch[N];
inline void in(Re &x)
{
    int fu = 0;
    x = 0;
    char c = getchar();
    while (c < '0' || c > '9')
        fu |= c == '-', c = getchar();
    while (c >= '0' && c <= '9')
        x = (x << 1) + (x << 3) + (c ^ 48), c = getchar();
    x = fu ? -x : x;
}
std::string generateRandomDNA(int length)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);

    const char nucleotides[] = {'A', 'T', 'C', 'G'};
    std::string dnaSequence;

    for (int i = 0; i < length; ++i)
    {
        dnaSequence += nucleotides[dis(gen)];
    }

    return dnaSequence;
}

// Function to mutate a DNA sequence to ensure similarity
void mutateDNA(std::string &dnaSequence)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, dnaSequence.length() - 1);
    std::uniform_int_distribution<> nucleotideDis(0, 3);

    // Mutate 10% of the sequence
    int mutationCount = static_cast<int>(0.1 * dnaSequence.length());
    for (int i = 0; i < mutationCount; ++i)
    {
        int index = dis(gen);
        char mutatedNucleotide = dnaSequence[index];
        // Ensure mutation results in a different nucleotide
        while (mutatedNucleotide == dnaSequence[index])
        {
            mutatedNucleotide = "ATCG"[nucleotideDis(gen)];
        }

        dnaSequence[index] = mutatedNucleotide;
    }
}
struct Trie
{
    int get(char c)
    {
        if (c == 'A')
            return 0;
        if (c == 'T')
            return 1;
        if (c == 'C')
            return 2;
        return 3;
    }
    int O, c[M], fa[M], tr[M][4];
    // fa[x]: Trie树上x的父节点
    // c[x]: Trie树上x的颜色
    Trie() { O = 1; } // 根初始化为1
    inline void insert(string &ch)
    {

        Re p = 1;
        for (int i = 0; i < ch.size(); i++)
        {
            Re a = get(ch[i]);
            if (!tr[p][a])
                tr[p][a] = ++O, fa[O] = p, c[O] = a;
            p = tr[p][a];
        }
    }
} T1;
struct Suffix_Automaton
{
    int O, pos[N], link[N], maxlen[N], trans[N][4];
    queue<int> Q;
    // pos[x]:Trie上的x节点（路径1->x所表示的字符串）在SAM上的对应节点编号
    // link[i]: 后缀链接
    // trans[i]: 状态转移数组
    Suffix_Automaton() { O = 1; } // 根初始化为1
    inline int insert(Re ch, Re last)
    { // 和普通SAM一样
        Re x, y, z = ++O, p = last;
        maxlen[z] = maxlen[last] + 1;
        while (p && !trans[p][ch])
            trans[p][ch] = z, p = link[p];
        if (!p)
            link[z] = 1;
        else
        {
            x = trans[p][ch];
            if (maxlen[p] + 1 == maxlen[x])
                link[z] = x;
            else
            {
                y = ++O;
                maxlen[y] = maxlen[p] + 1;
                for (Re i = 0; i < 4; ++i)
                    trans[y][i] = trans[x][i];
                while (p && trans[p][ch] == x)
                    trans[p][ch] = y, p = link[p];
                link[y] = link[x], link[z] = link[x] = y;
            }
        }
        return z;
    }
    inline void build()
    { // bfs遍历Trie树构造广义SAM
        for (Re i = 0; i < 4; ++i)
            if (T1.tr[1][i])
                Q.push(T1.tr[1][i]); // 插入第一层字符
        pos[1] = 1;                  // Tire树上的根1在SAM上的位置为根1
        while (!Q.empty())
        {
            Re x = Q.front();
            Q.pop();

            pos[x] = insert(T1.c[x], pos[T1.fa[x]]); // 注意是pos[Trie->fa[x]]
            for (Re i = 0; i < 4; ++i)
                if (T1.tr[x][i])
                    Q.push(T1.tr[x][i]);
        }
    }
} SAM;
int main()
{
    const int sequenceLength = 200000;
    const int numSequences = 30;

    std::vector<std::string> dnaSequences;

    // Generate 30 random DNA sequences
    for (int i = 0; i < numSequences; ++i)
    {
        std::string sequence = generateRandomDNA(sequenceLength);
        dnaSequences.push_back(sequence);
    }

    // Mutate sequences to ensure similarity
    for (int i = 1; i < numSequences; ++i)
    {
        mutateDNA(dnaSequences[i]);
    }
    cout << dnaSequences[0].substr(0, 20) << " " << dnaSequences[1].substr(0, 20) << endl;

    for (Re i = 1; i <= 30; ++i)
    {
        T1.insert(dnaSequences[i - 1]);
    }
    SAM.build();
    cout<<T1.O<<" "<<SAM.O<<endl;
}