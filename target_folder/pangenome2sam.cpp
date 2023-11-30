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
const int N = 1.5e4 + 5, M = 6e4 + 3; // N -> SAM size  M -> Trie Size
struct Segment
{
    string id;
    string sequence;
    string chromosome_name;
    int offset;
    int length;
};
struct Link
{
    string from;
    char fromDir;
    string to;
    char toDir;
};
struct Walk
{
    vector<pair<string, int>> segs; // <name,dir>
    string chromosome_name;
};
map<string, Segment> s2allInfo;
vector<Walk> walks;
map<string, vector<pair<char, string>>> mp[2];
int get(char c) // mapping characters to integers
{
    if (c == 'A')
        return 0;
    if (c == 'T')
        return 1;
    if (c == 'C')
        return 2;
    return 3;
}
Segment parseSegment(const string &line)
{
    stringstream ss(line);
    Segment seg;
    string segment;
    vector<string> seglist;
    while (getline(ss, segment, '\t'))
    {
        seglist.push_back(segment);
    }
    if (seglist.size() >= 6)
    {
        seg.id = seglist[1];
        seg.sequence = seglist[2];
        seg.chromosome_name = seglist[3].substr(5);
        int offset = stoi(seglist[5].substr(5));
        int len = stoi(seglist[4].substr(5));
        seg.offset = offset;
        seg.length = len;
        s2allInfo[seg.id] = seg;
    }
    return seg;
}
Link parseLink(const string &line)
{
    stringstream ss(line);
    Link link;
    string segment;
    vector<string> seglist;
    while (getline(ss, segment, '\t'))
    {
        seglist.push_back(segment);
    }
    if (seglist.size() > 4)
    {
        link.from = seglist[1];
        link.fromDir = seglist[2][0]; // 获取方向信息
        link.to = seglist[3];
        link.toDir = seglist[4][0]; // 获取方向信息
    }

    return link;
}
Walk parseWalk(string &line)
{
    Walk w;
    string segment;
    vector<string> seglist;
    stringstream ss(line);
    while (getline(ss, segment, '\t'))
    {
        seglist.push_back(segment);
    }
    w.chromosome_name = seglist[3];
    segment = "";
    for (int i = 0; i < seglist.back().size(); i++)
    {
        int j = i + 1;
        int dir = 0;
        if (seglist.back()[i] == '<')
            dir = 1;
        while (j < seglist.back().size() && seglist.back()[j] != '<' && seglist.back()[j] != '>')
        {
            segment += seglist.back()[j++];
        }
        i = j - 1;
        w.segs.push_back({segment, dir}), segment = "";
    }
    return w;
}

struct Trie
{

    int O, c[M], fa[M], tr[M][4];
    vector<pair<int, int>> endpos[M];
    Trie() { O = 1; }                      // initialize root number
    inline void insert(string &ch, int id) // insert char to trie
    {
        int p = 1;
        for (int i = 0; i < ch.size(); i++)
        {
            int a = get(ch[i]);
            if (!tr[p][a])
                tr[p][a] = ++O, fa[O] = p, c[O] = a;
            p = tr[p][a];
            endpos[p].push_back({id, i});
        }
    }
} T1;

struct Suffix_Automaton
{
    int O, pos[N], link[N], maxlen[N], trans[N][4];
    vector<pair<int, int>> endpos[N];
    vector<string> paths[N];
    vector<int> fa[N];
    queue<int> Q;
    // pos[x]: The corresponding node number on SAM for node x on Trie (the string represented by path 1->x)
    // link[i]: Suffix link
    // trans[i]: Transition array for states
    Suffix_Automaton() { O = 1; }                                          // Initialize root as 1
    inline int insert(int ch, int last, const vector<pair<int, int>> &end) // insert character and record endpos
    {
        int x, y, z = ++O, p = last;
        maxlen[z] = maxlen[last] + 1;
        endpos[z] = end;
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
                for (int i = 0; i < 4; ++i)
                    trans[y][i] = trans[x][i];
                while (p && trans[p][ch] == x)
                    trans[p][ch] = y, p = link[p];
                link[y] = link[x], link[z] = link[x] = y;
            }
        }
        return z;
    }
    inline void build() // build trie for chromosomes
    {
        // Insert the characters of the first layer into the queue
        for (int i = 0; i < 4; ++i)
            if (T1.tr[1][i])
                Q.push(T1.tr[1][i]);

        // The position of root 1 in the SAM is set to 1
        pos[1] = 1;
        while (!Q.empty())
        {
            int x = Q.front();
            Q.pop();
            // Note that it is pos[Trie->fa[x]]
            pos[x] = insert(T1.c[x], pos[T1.fa[x]], T1.endpos[x]);

            // Insert the characters of the current layer into the queue
            for (int i = 0; i < 4; ++i)
                if (T1.tr[x][i])
                    Q.push(T1.tr[x][i]);
        }

        // Link nodes in the SAM
        for (int i = 2; i <= O; i++)
            fa[link[i]].push_back(i);

        // Perform DFS on the SAM
        dfs(1);

        // Get all strings in the SAM
        get_strs(1, "");
    }
    void dfs(int u) // merge endpositions
    {
        for (auto son : fa[u])
        {
            dfs(son);
            for (auto t : endpos[son])
                endpos[u].push_back(t);
        }
    }
    void get_strs(int u, string path) // get string set for each node
    {
        paths[u].push_back(path);
        for (int i = 0; i < 4; i++)
            if (trans[u][i])
            {
                char ch = 'A';
                if (i == 1)
                    ch = 'T';
                else if (i == 2)
                    ch = 'C';
                else if (i == 3)
                    ch = 'G';
                path += ch;
                get_strs(trans[u][i], path);
                path.pop_back();
            }
    }
    void output_all_positions(const string &query)
    {
        int now = 1;
        for (int i = 0; i < query.size(); i++)
        {
            if (!trans[now][get(query[i])])
            {
                cout << "error " << endl;
                return;
            }
            now = trans[now][get(query[i])];
        }
        cout << query << " has  " << endpos[now].size() << " exact matches" << endl;
        for (auto j : endpos[now])
            cout << walks[j.first].chromosome_name << " (0 based)" << j.second << endl;
    }
} SAM;
int main()
{
    freopen("sam.txt", "w", stdout);
    ifstream gfa_file("./o.gfa"); // open gfa file

    if (gfa_file.is_open())
    {
        string line;
        while (getline(gfa_file, line))
        {
            if (line[0] == 'S') // process segment
            {
                Segment seq = parseSegment(line);
                cout << "Segment: " << seq.id << " - " << seq.sequence << endl;
            }
            else if (line[0] == 'L') // process link
            {
                Link link = parseLink(line);
                if (link.fromDir == '+')
                    mp[0][link.from].push_back({link.toDir, link.to});
                else
                    mp[1][link.from].push_back({link.toDir, link.to});
                cout << "Link: " << link.from << " (" << link.fromDir << ") -> "
                     << link.to << " (" << link.toDir << ")" << endl;
            }
            else if (line[0] == 'W') // process walk
                walks.push_back(parseWalk(line));
        }
        gfa_file.close();
    }
    else
    {
        cout << "cannot open this file" << endl;
    }
    int i = 0;
    // get chromosomes  and insert them to trie
    for (auto w : walks)
    {
        cout << " - - -- -- - -- --  " << endl;
        string path = "";
        for (auto seg : w.segs)
        {
            string tmp = s2allInfo[seg.first].sequence;
            if (seg.second)
                reverse(tmp.begin(), tmp.end());
            path += tmp;
        }
        cout << path << ":chromosome_name  " << w.chromosome_name << endl;
        T1.insert(path, i);
        i++;
        cout << " - - -- -- - -- --  " << endl;
    }
    // bfs trie to build gsam and output gsam
    SAM.build();
    cout << " - - -- -- - -- -- " << endl;
    cout << "output gsam :" << endl;
    for (int i = 2; i <= SAM.O; i++)
    {
        cout << "sam node id:" << i << " "
             << "expressed suffix set is:";
        sort(SAM.paths[i].begin(), SAM.paths[i].end(), [&](string &a, string &b)
             { return a.length() < b.length(); });
        for (auto s : SAM.paths[i])
            cout << s << " ";
        cout << endl;
        cout << "endpos set : first chromosome_name second endposition" << endl;
        cout << "endpos size is " << SAM.endpos[i].size() << endl;
        for (auto j : SAM.endpos[i])
            cout << walks[j.first].chromosome_name << " endpos(base 0 index)" << j.second << endl;
        cout << " - - -- -- - -- -- " << endl;
    }
    cout << " - - -- -- - -- -- " << endl;
    string query = "CT";
    SAM.output_all_positions(query);
    return 0;
}