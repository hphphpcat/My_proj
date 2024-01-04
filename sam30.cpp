#include <bits/stdc++.h>
using namespace std;
const int M = 30;
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
struct SuffixAutomaton
{

    struct node
    {
        int ch[4], fa, len, cnt;
        void clean()
        {
            memset(ch, 0, sizeof(ch));
            fa = len = cnt = 0;
        }
    } S[1 + maxn << 1];
    int root, last, siz;

    void init()
    {
        for (int i = 1; i <= maxn << 1; i++)
            S[i].clean();
        root = last = siz = 1;
    }

    void insert(int c)
    {
        int p = last, now = ++siz;
        S[now].cnt = 1;
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
                S[q_new].len = S[p].len + 1;
                S[now].fa = S[q].fa = q_new;
                for (; p && S[p].ch[c] == q; p = S[p].fa)
                    S[p].ch[c] = q_new;
            }
        }
        last = now;
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
        // 关闭文件
        outFile.close();
    }
}

void desearialize_sam(int id)
{
    std::ifstream inFile("./sams/example" + to_string(id) + "._sam");
    SAM.init();
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
            cout << line << endl;
        }
    }
}
int main()
{
    // for (int i = 0; i < 5; i++)
    //     searialize_sam(i);
    for (int i = 0; i < 1; i++)
    {
        string s = "ATCG";
        // read s
        SAM.init();
        for (auto j : s)
            SAM.insert(get(j));
        searialize_sam(i);
    }
    desearialize_sam(0);
    cout << "---" << endl;
    searialize_sam(1);
}
