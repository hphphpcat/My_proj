#include <bits/stdc++.h>
using namespace std;

// 序列结构
struct Sequence
{
    std::string id;
    std::string sequence;
    // 可以根据需要添加其他字段
};
map<string, string> id2s;
map<string, string> s2id;
map<string, vector<pair<char, string>>> mp[2];
// 连接结构
struct Link
{
    std::string from;
    char fromDir;
    std::string to;
    char toDir;
    // 可以根据需要添加其他字段
};

// 解析一行序列数据
Sequence parseSequence(const std::string &line)
{
    std::stringstream ss(line);
    Sequence seq;
    std::string segment;
    std::vector<std::string> seglist;
    while (std::getline(ss, segment, '\t'))
    {
        seglist.push_back(segment);
    }
    if (seglist.size() > 2)
    {
        seq.id = seglist[1];
        seq.sequence = seglist[2];
        id2s[seq.id] = seq.sequence;
        s2id[seq.sequence] = seq.id;
        // 提取其他信息
    }
    return seq;
}

// 解析一行连接数据
Link parseLink(const std::string &line)
{
    std::stringstream ss(line);
    Link link;
    std::string segment;
    std::vector<std::string> seglist;

    while (std::getline(ss, segment, '\t'))
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
string t[5];
// 精确的graph 和 精确的序列 找出精确匹配的位置
// 精确的graph 和 突变的序列 找出最大相似度 以及 位置
// 精确的graph 和 突变的序列 找出所有>=base 所有位置
string query;

double res;
string res_str;
double base = 0.8;
vector<pair<string, double>> vec;
int dp[100][100];
string last;
void calcScore(const string &s)
{
    if (s.substr(0, last.size()) == last && last.size() + 1 == s.size())
    {
        int i = s.size();
        // cout << last << " " << s << endl;
        for (int j = 1; j <= query.size(); j++)
        {
            if (i >= 1)
                dp[i][j] = min(dp[i - 1][j] + 1, dp[i][j]);
            if (j >= 1)
                dp[i][j] = min(dp[i][j - 1] + 1, dp[i][j]);
            if (s[i - 1] == query[j - 1])
                dp[i][j] = min(dp[i][j], dp[i - 1][j - 1]);
        }
        if (1 - 1.0 * dp[s.size()][query.size()] / (int)max(s.size(), query.size()) > res)
        {
            res = max(res, 1 - 1.0 * dp[s.size()][query.size()] / (int)max(s.size(), query.size()));
            res_str = s;
        }
        if (1 - 1.0 * dp[s.size()][query.size()] / (int)max(s.size(), query.size()) >= base)
        {
            vec.push_back({s, 1 - 1.0 * dp[s.size()][query.size()] / (int)max(s.size(), query.size())});
        }
        last = s;
        return;
    }
    last = s;
    memset(dp, 0x3f, sizeof dp);
    for (int i = 0; i <= 99; i++)
        dp[i][0] = i;
    for (int i = 0; i <= query.size(); i++)
        dp[0][i] = i;
    for (int i = 1; i <= s.size(); i++)
        for (int j = 1; j <= query.size(); j++)
        {
            if (i >= 1)
                dp[i][j] = min(dp[i - 1][j] + 1, dp[i][j]);
            if (j >= 1)
                dp[i][j] = min(dp[i][j - 1] + 1, dp[i][j]);
            if (s[i - 1] == query[j - 1])
                dp[i][j] = min(dp[i][j], dp[i - 1][j - 1]);
        }
    if (1 - 1.0 * dp[s.size()][query.size()] / (int)max(s.size(), query.size()) > res)
    {
        res = max(res, 1 - 1.0 * dp[s.size()][query.size()] / (int)max(s.size(), query.size()));
        res_str = s;
    }
}
bool check_on_fa(const string &path)
{
    for (int i = 0; i < 5; i++)
        if (t[i].find(path) != string::npos)
            return true;
    return false;
}
void dfs(string s, string path, int pre)
{
    for (auto ne : mp[pre][s])
    {
        string t = id2s[ne.second];
        if (ne.first == '-')
            reverse(t.begin(), t.end());
        if (res > 0.9)
            return;
        for (int i = path.size() - 1; i >= 0; i--)
        {
            if (path.size() - i > query.size() + 5)
                break;
            string tmp = path.substr(i);
            for (int j = 0; j < t.size(); j++)
            {
                tmp += t[j];
                if (tmp.size() < query.size() - 5)
                    continue;
                if (tmp.size() > query.size() + 5)
                    break;
                if (res > 0.9)
                    return;

                calcScore(tmp);
            }
        }
        dfs(ne.second, path + t, ne.first == '-' ? 1 : 0);
    }
}
std::string string_variation(std::string query)
{
    int n = query.size();
    int num_mutations = n * 0.1; // 10% of the string length

    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, n - 1);

    std::set<int> mutated_indices;
    while (mutated_indices.size() < num_mutations)
    {
        mutated_indices.insert(distr(gen));
    }

    for (auto idx : mutated_indices)
    {
        char current_char = query[idx];
        char new_char;

        do
        {
            std::uniform_int_distribution<> char_distr(0, 3);
            new_char = "ATCG"[char_distr(gen)];
        } while (new_char == current_char);

        query[idx] = new_char;
    }

    return query;
}
// 主函数
int main()
{
    int result = system("python3 ./aa.py");
    assert(result == 0);
    result = system("./minigraph1/minigraph -cxggs -L 0 -d 0 -l 0 ./minigraph1/1.fa ./minigraph1/2.fa ./minigraph1/3.fa ./minigraph1/4.fa ./minigraph1/5.fa   > ./minigraph1/o.gfa");
    assert(result == 0);
    std::string line;
    std::ifstream fa_file1("./minigraph1/1.fa");
    std::ifstream fa_file2("./minigraph1/2.fa");
    std::ifstream fa_file3("./minigraph1/3.fa");
    std::ifstream fa_file4("./minigraph1/4.fa");
    std::ifstream fa_file5("./minigraph1/5.fa");

    if (fa_file1.is_open())
    {
        getline(fa_file1, line);
        getline(fa_file1, line);
        t[0] = line;
    }
    else
    {
        cout << "open fa1 error" << endl;
    }
    if (fa_file2.is_open())
    {
        getline(fa_file2, line);
        getline(fa_file2, line);
        t[1] = line;
    }
    else
    {
        cout << "open fa2 error" << endl;
    }
    if (fa_file3.is_open())
    {
        getline(fa_file3, line);
        getline(fa_file3, line);
        t[2] = line;
    }
    else
    {
        cout << "open fa3 error" << endl;
    }
    if (fa_file4.is_open())
    {
        getline(fa_file4, line);
        getline(fa_file4, line);
        t[3] = line;
    }
    else
    {
        cout << "open fa4 error" << endl;
    }
    if (fa_file5.is_open())
    {
        getline(fa_file5, line);
        getline(fa_file5, line);
        t[4] = line;
    }
    else
    {
        cout << "open fa5 error" << endl;
    }
    cout << "successs!!!" << endl;
    query = t[1].substr(100, 76);
    query = string_variation(query);
    cout << t[1].substr(100, 76) << endl;
    cout << query << endl;
    std::ifstream gfa_file("./minigraph1/o.gfa"); // 替换为您的GFA文件路径
    vector<string> strs;

    if (gfa_file.is_open())
    {
        while (getline(gfa_file, line))
        {
            if (line[0] == 'S')
            {
                Sequence seq = parseSequence(line);
                strs.push_back(seq.id);
                std::cout << "Sequence: " << seq.id << " - " << seq.sequence << std::endl;
                // 处理序列
            }
            else if (line[0] == 'L')
            {
                Link link = parseLink(line);
                if (link.fromDir == '+')
                    mp[0][link.from].push_back({link.toDir, link.to});
                else
                    mp[1][link.from].push_back({link.toDir, link.to});

                std::cout << "Link: " << link.from << " (" << link.fromDir << ") -> "
                          << link.to << " (" << link.toDir << ")" << std::endl;
                // 处理连接
            }
        }
        gfa_file.close();
    }
    else
    {
        std::cout << "无法打开文件" << std::endl;
    }

    for (auto x : strs)
    {
        // cout << "x is " << x << endl;
        cout << res << endl;
        string t1 = id2s[x];

        for (int i = 0; i < t1.size(); i++)
        {
            string tmp = "";
            for (int j = i; j < t1.size(); j++)
            {
                tmp += t1[j];
                if (tmp.size() < query.size() - 5)
                    continue;
                if (tmp.size() > query.size() + 10)
                    break;
                calcScore(tmp);
            }
        }
        dfs(x, t1, 0);
        string t = t1;
        reverse(t.begin(), t.end());
        for (int i = 0; i < t.size(); i++)
        {
            string tmp = "";
            for (int j = i; j < t.size(); j++)
            {
                tmp += t[j];
                if (tmp.size() < query.size() - 5)
                    continue;
                if (tmp.size() > query.size() + 10)
                    break;
                calcScore(tmp);
            }
        }
        dfs(x, t, 1);
    }
    cout << "max score is " << res << endl;
    cout << "str is " << res_str << endl;
    sort(vec.begin(), vec.end(), [&](pair<string, double> a, pair<string, double> b)
         { return a.second > b.second; });
    for (auto j : vec)
        cout
            << j.first << " " << j.second << endl;
    return 0;
}
