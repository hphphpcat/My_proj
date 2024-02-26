
#include <bits/stdc++.h>
using namespace std;
const int M = 30;
const int SamNum = 2;
const int maxn = 1110 + 10;
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
// abct
// tabc
struct SuffixAutomaton
{
    string samName;
    void init(int id)
    {
        samName = "SAM" + to_string(id);
        for (int i = 1; i <= maxn << 1; i++)
            S[i].clean();
        root = last = siz = 1;
    }
    void init(int id, string s)
    {
        samName = "SAM" + to_string(id);
        for (int i = 1; i <= maxn << 1; i++)
            S[i].clean();
        root = last = siz = 1;
        int pos = 0;
        for (auto j : s)
            insert(get(j), pos), pos++;
        build();
    }
    struct node
    {
        int ch[4], fa, len, cnt;
        vector<int> endpos;
        vector<int> sons;
        void clean()
        {
            memset(ch, 0, sizeof(ch));
            fa = len = cnt = 0;
            endpos.clear();
            sons.clear();
        }
    } S[1 + maxn << 1];
    int root, last, siz;

    void insert(int c, int pos)
    {
        int p = last, now = ++siz;
        S[now].cnt = 1;
        S[now].endpos.push_back(pos);
        S[now].len = S[p].len + 1;
        for (; p && !S[p].ch[c]; p = S[p].fa)
            S[p].ch[c] = now;
        if (!p)
            S[now].fa = root;
        else
        {
            int q = S[p].ch[c];
            if (S[q].len == S[p].len + 1)
                S[now].fa = q;
            else
            {
                int q_new = ++siz;
                S[q_new] = S[q];
                S[q_new].cnt = 0;
                S[q_new].endpos.clear(); // 记得清空 不然 endpos 会重复
                S[q_new].len = S[p].len + 1;
                S[now].fa = S[q].fa = q_new;
                for (; p && S[p].ch[c] == q; p = S[p].fa)
                    S[p].ch[c] = q_new;
            }
        }
        last = now;
    }
    void build()
    {
        for (int i = 2; i <= siz; i++)
            S[S[i].fa].sons.push_back(i);
    }
} SAM;
void searialize_sam(int id)
{
    std::ofstream outFile("./sams/example" + to_string(id) + "._sam");
    if (outFile.is_open())
    {
        std::cout << "文件已成功打开。" << std::endl;
        outFile << SAM.siz << endl;
        outFile << "node fa-link" << endl;
        for (int i = 1; i <= SAM.siz; i++)
        {
            outFile << SAM.S[i].fa << endl;
        }
        outFile << "node link" << endl;
        for (int i = 1; i <= SAM.siz; i++)
        {
            for (int j = 0; j < 4; j++)
                outFile << SAM.S[i].ch[j] << " ";
            outFile << endl;
        }
        outFile << "node link" << endl;
        for (int i = 1; i <= SAM.siz; i++)
        {
            for (int j = 0; j < 4; j++)
                outFile << SAM.S[i].ch[j] << " ";
            outFile << endl;
        }
        outFile << "node endpos" << endl;
        for (int i = 1; i <= SAM.siz; i++)
        {
            for (int j = 0; j < SAM.S[i].endpos.size(); j++)
                outFile << SAM.S[i].endpos[j] << " ";
            outFile << endl;
        }
        outFile << "node sons" << endl;
        for (int i = 1; i <= SAM.siz; i++)
        {
            for (int j = 0; j < SAM.S[i].sons.size(); j++)
                outFile << SAM.S[i].sons[j] << " ";
            outFile << endl;
        }
        // 关闭文件
        outFile.close();
    }
}

void desearialize_sam(int id)
{
    std::ifstream inFile("./sams/example" + to_string(id) + "._sam");
    SAM.init(id);
    if (inFile.is_open())
    {
        std::cout << "文件已成功打开。" << std::endl;
        string line;
        int flag = 0;
        int id = 0;
        while (getline(inFile, line))
        {
            if (line.find("fa-link") != string::npos)
                flag = 1, id = 1;
            else if (line.find("node link") != string::npos)
                flag = 2, id = 1;
            else if (line == "node endpos")
            {
                flag = 3;
                id = 1;
            }
            else if (line == "node sons")
            {
                flag = 4;
                id = 1;
            }
            if (flag == 0)
                SAM.siz = stoi(line);
            else if (flag == 1 && line[0] != 'n')
            {
                SAM.S[id].fa = stoi(line);
                id++;
            }
            else if (flag == 2 && line[0] != 'n')
            {
                stringstream ssin(line);
                string num;
                int cnt = 0;
                while (ssin >> num)
                {
                    if (stoi(num) != 0)
                        SAM.S[id].ch[cnt] = stoi(num);
                    cnt++;
                }
                id++;
            }
            else if (flag == 3 && line[0] != 'n')
            {
                stringstream ssin(line);
                string num;
                while (ssin >> num)
                {
                    SAM.S[id].endpos.push_back(stoi(num));
                }
                id++;
            }
            else if (flag == 4 && line[0] != 'n')
            {
                stringstream ssin(line);
                string num;
                while (ssin >> num)
                {
                    SAM.S[id].sons.push_back(stoi(num));
                }
                id++;
            }
        }
        inFile.close();
    }
}

vector<string> queries;
vector<vector<string>> ans;
string allEndpos;
void findAllEndpos(int u)
{
    for (auto ps : SAM.S[u].endpos)
    {
        cout << u << "->" << ps << endl;
        allEndpos += to_string(ps) + " ";
    }
    for (auto son : SAM.S[u].sons)
        findAllEndpos(son);
}
void dfs_path(string path, int u)
{
    cout << u << " vvv " << path << endl;
    for (auto endpos : SAM.S[u].endpos)
        cout << endpos << " ";
    cout << endl;
    char idx[] = {'A', 'T', 'C', 'G'};
    for (int i = 0; i < 4; i++)
    {
        if (SAM.S[u].ch[i])
        {
            dfs_path(path + idx[i], SAM.S[u].ch[i]);
        }
    }
}
void findAns()
{
    for (int i = 0; i < SamNum; i++)
    {

        desearialize_sam(i);
        dfs_path("", 1);
        for (int queryId = 0; queryId < queries.size(); queryId++)
        {
            string query = queries[queryId];
            ans[queryId].push_back(SAM.samName);
            cout << query << endl;
            cout << SAM.siz << endl;
            bool flag = true;
            int now = 1;
            for (auto c : query)
            {
                now = SAM.S[now].ch[get(c)];
                if (!now)
                {
                    flag = false;
                    break;
                }
            }
            allEndpos.clear();
            if (flag)
            {
                findAllEndpos(now);
            }
            cout << "all ENdpos" << allEndpos << endl;
            ans[queryId].push_back(allEndpos);
        }
    }
}
void readQuires()
{
    std::ifstream inFile("./sams/quiries");
    string s;
    while (inFile >> s)
    {
        queries.push_back(s);
    }
    inFile.close();
}
int main()
{
    string s[] = {"ATCGATCGTACGTACG", "TCAGTACGATCGA"};
    for (int i = 0; i < 2; i++)
    {
        SAM.init(i, s[i]);
        searialize_sam(i);
    }
    readQuires();
    ans.resize(queries.size());
    findAns();
    for (int i = 0; i < ans.size(); i++)
    {
        cout << queries[i] << ":qaq";
        for (auto j : ans[i])
            cout << j << endl;
    }
    // vector<int>ans()
}
