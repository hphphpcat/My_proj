#include <algorithm>
#include <cstdio>
#include <queue>
#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
using namespace std;
const long long MAXN = 5e9; // 双倍字符串长度
const int CHAR_NUM = 4;     // 字符集个数，注意修改下方的 (-'a')
int get(char c)             // mapping characters to integers
{
    if (c == 'A')
        return 0;
    if (c == 'T')
        return 1;
    if (c == 'C')
        return 2;
    return 3;
}
// 生成基础序列
std::string generateBaseSequence(int length)
{
    std::string sequence;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3); // 0:A, 1:T, 2:C, 3:G

    for (int i = 0; i < length; ++i)
    {
        char nucleotide;
        switch (dis(gen))
        {
        case 0:
            nucleotide = 'A';
            break;
        case 1:
            nucleotide = 'T';
            break;
        case 2:
            nucleotide = 'C';
            break;
        case 3:
            nucleotide = 'G';
            break;
        }
        sequence += nucleotide;
    }

    return sequence;
}

// 引入变异，使得相似度为90%
std::string introduceVariation(const std::string &baseSequence, double similarity)
{
    std::string mutatedSequence = baseSequence;
    int numMutations = static_cast<int>(baseSequence.size() * (1.0 - similarity));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, baseSequence.size() - 1);

    for (int i = 0; i < numMutations; ++i)
    {
        int mutationIndex = dis(gen);
        char newNucleotide;
        do
        {
            newNucleotide = static_cast<char>('A' + dis(gen) % 4);
        } while (newNucleotide == mutatedSequence[mutationIndex]);
        mutatedSequence[mutationIndex] = newNucleotide;
    }
    return mutatedSequence;
}

struct exSAM
{
    int *len;   // 节点长度
    int *link;  // 后缀链接，link
    int **next; // 转移
    int tot;    // 节点总数：[0, tot)

    void init()
    { // 初始化函数
        len = new int[MAXN];
        link = new int[MAXN];
        next = new int *[MAXN];
        for (int i = 0; i < MAXN; i++)
        {
            next[i] = new int[4];
            for (int j = 0; j < 4; j++)
                next[i][j] = 0;
        }
        tot = 1;
        link[0] = -1;
    }

    int insertSAM(int last, int c)
    { // last 为父 c 为子
        int cur = next[last][c];
        if (len[cur])
            return cur;
        len[cur] = len[last] + 1;
        int p = link[last];
        while (p != -1)
        {
            if (!next[p][c])
                next[p][c] = cur;
            else
                break;
            p = link[p];
        }
        if (p == -1)
        {
            link[cur] = 0;
            return cur;
        }
        int q = next[p][c];
        if (len[p] + 1 == len[q])
        {
            link[cur] = q;
            return cur;
        }
        int clone = tot++;
        for (int i = 0; i < CHAR_NUM; ++i)
            next[clone][i] = len[next[q][i]] != 0 ? next[q][i] : 0;
        len[clone] = len[p] + 1;
        while (p != -1 && next[p][c] == q)
        {
            next[p][c] = clone;
            p = link[p];
        }
        link[clone] = link[q];
        link[cur] = clone;
        link[q] = clone;
        return cur;
    }

    int insertTrie(int cur, int c)
    {

        if (next[cur][c])
            return next[cur][c];     // 已有该节点 直接返回
        return next[cur][c] = tot++; // 无该节点 建立节点
    }

    void insert(const string &s)
    {
        int root = 0;
        for (int i = 0; i < s.size(); ++i)
            root = insertTrie(root, get(s[i])); // 一边插入一边更改所插入新节点的父节点
    }

    void build()
    {
        queue<pair<int, int>> q;
        for (int i = 0; i < 4; ++i)
            if (next[0][i])
                q.push({i, 0});
        while (!q.empty())
        { // 广搜遍历
            auto item = q.front();
            q.pop();
            auto last = insertSAM(item.second, item.first);
            for (int i = 0; i < CHAR_NUM; ++i)
                if (next[last][i])
                    q.push({i, last});
        }
    }
} exSam;
int main()
{
    const int sequenceLength = 1e7;
    const int numSequences = 30;
    const double similarity = 0.9;

    std::vector<std::string> sequences;

    // 生成基础序列
    std::string baseSequence = generateBaseSequence(sequenceLength);
    exSam.init();
    // 生成相似序列
    for (int i = 0; i < numSequences; ++i)
    {
        std::string mutatedSequence = introduceVariation(baseSequence, similarity);
        exSam.insert(mutatedSequence);
    }
    exSam.build();
    cout << exSam.tot << endl;
    // int cnt[5] = {0};
    // int *c = new int[5300000];
    // int *dp = new int[5300000];
    // int sum = 0;
    // for (int i = 1; i <= SAM.O; i++)
    // {
    //     int t = 0;
    //     for (int j = 0; j < 4; j++)
    //         if (SAM.trans[i][j])
    //         {
    //             sum++;
    //             t++;
    //             c[SAM.trans[i][j]]++;
    //         }
    //     cnt[t]++;
    // }

    // cout << "点数" << SAM.O << endl;
    // cout << "边数" << sum << endl;
    // cout << "入度分布" << endl;
    // for (int i = 0; i <= 4; i++)
    //     cout << cnt[i] << "," << endl;
    // freopen("outputsam.txt", "w", stdout);
    // int *dep = new int[5300000];
    // for (int i = 1; i <= SAM.O; i++)
    //     cout << c[i] << ",";
    // fclose(stdout);
    // freopen("outputdep.txt", "w", stdout);
    // queue<int> q;
    // q.push(1);
    // while (q.size())
    // {
    //     auto tt = q.front();
    //     q.pop();
    //     for (int i = 0; i < 4; i++)
    //     {
    //         if (SAM.trans[tt][i])
    //         {
    //             dp[SAM.trans[tt][i]] = max(dp[SAM.trans[tt][i]], dp[tt] + 1);
    //             if (!(--c[SAM.trans[tt][i]]))
    //                 q.push(SAM.trans[tt][i]);
    //         }
    //     }
    // }
    // for (int i = 1; i <= SAM.O; i++)
    // {
    //     if (i % 100)
    //         cout << dp[i] << ",";
    //     else
    //         cout << dp[i] << endl;
    // }
    // fclose(stdout);
    return 0;
}