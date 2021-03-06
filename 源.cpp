#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cctype>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <stack>
#define _KEY_WORDEND "waiting for your expanding"
#define MAX 507
#define DEBUG
using namespace std;

typedef struct //词的结构，二元组形式（单词种别，单词自身的值）
{
    int typenum; //单词种别
    char* word;
}WORD;
char input[255];
char token[255] = "";
int p_input; //指针
int p_token;
char ch;
char* rwtab[] = { "id","true","false","number","integer","decimal","bool",_KEY_WORDEND };//关键字
int var_list_symbol[50] = {};//变量类型表
string var_list_name[50] = {};//变量名字表
double var_list_value[50] = {};//变量值
int var_list_size = 0;
bool cout_type = false;
bool cout_induction = false;
bool cout_action = false;
bool cout_list = false;
bool cout_first = false;
bool cout_follow = false;
bool cout_itemset = false;
bool cout_ERROR = false;

WORD* scanner();//扫描

int words(string input_str)
{
    ofstream outfile("out.xml", ios::trunc);
    int over = 1;
    memset(input, 0, sizeof(input) / sizeof(char));
    WORD* oneword = new WORD;
    for (int i = 0; i < input_str.length(); i++) {
        input[i] = input_str[i];
    }
    p_input = 0;
    memset(token, 0, sizeof(token) / sizeof(char));
    p_token = 0;
    ch = 0;
    //printf("your words:  %s\n", input);
    while (over < 1000 && over != -1)
    {
        oneword = scanner();
        if (oneword->typenum < 1000)
        {
            if (oneword->typenum != 999)
                outfile << oneword->typenum << " " << oneword->word << endl;
        }
        over = oneword->typenum;
    }
    outfile.close();
    return 0;
}

//从输入缓冲区读取一个字符到ch中
char m_getch()
{
    ch = input[p_input];
    p_input++;
    return ch;
}

//去掉空白符号
void getbc()
{
    while (ch == ' ' || ch == 10)
    {
        ch = input[p_input];
        p_input++;
    }
}

//拼接单词
void concat()
{
    token[p_token] = ch;
    p_token++;
    token[p_token] = '\0';
}

//判断是否字母
int letter()
{
    if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')
        return 1;
    else
        return 0;
}

//判断是否数字
int digit()
{
    if (ch >= '0' && ch <= '9')
        return 1;
    else if (ch == '.')
        return 2;
    else
        return 0;
}

//检索关键字表格
int reserve()
{
    int i = 0;
    while (strcmp(rwtab[i], _KEY_WORDEND))
    {
        if (!strcmp(rwtab[i], token))
            return i + 1;
        i++;
    }
    return 8;//如果不是关键字，则返回种别码8
}

//回退一个字符
void retract()
{
    p_input--;
}

//词法扫描程序
WORD* scanner()
{
    WORD* myword = new WORD;
    myword->typenum = 10;  //初始值
    myword->word = "";
    p_token = 0;   //单词缓冲区指针
    m_getch();
    getbc();//去掉空白

    if (letter())//判断读取到的首字母是字母
    {
        //如int
        while (letter() || digit())
        {
            concat(); //连接
            m_getch();
        }
        retract(); //回退一个字符
        myword->typenum = reserve();//判断是否为关键字，返回种别码
        myword->word = token;
        return myword;
    }
    else if (digit())  //判断读取到的单词首字符是数字
    {
        while (digit()) //所有数字连接起来
        {
            concat();
            m_getch();
        }
        retract();
        //数字单词种别码统一为1，单词自身的值为数字本身
        myword->typenum = 1;
        myword->word = token;
        return(myword);
    }
    else switch (ch)
    {
    case '=':
        m_getch();//首字符为=,再读取下一个字符判断
        if (ch == '=')
        {
            myword->typenum = 61;
            myword->word = "==";
            return(myword);
        }
        retract();//读取到的下个字符不是=，则要回退，直接输出=
        myword->typenum = 91;
        myword->word = "=";
        return(myword);
        break;
    case '+':
        m_getch();//首字符为+,再读取下一个字符判断
        if (ch == '+')
        {
            myword->typenum = 22;
            myword->word = "++";
            return(myword);
        }
        retract();//读取到的下个字符不是+，则要回退，直接输出+
        myword->typenum = 41;
        myword->word = "+";
        return(myword);
        break;
    case '-':
        m_getch();//首字符为+,再读取下一个字符判断
        if (ch == '-')
        {
            myword->typenum = 23;
            myword->word = "--";
            return(myword);
        }
        retract();//读取到的下个字符不是+，则要回退，直接输出+
        myword->typenum = 42;
        myword->word = "-";
        return(myword);
        break;
    case '/'://读取到该符号之后，要判断下一个字符是什么符号，判断是否为注释
        m_getch();//首字符为/,再读取下一个字符判断
        if (ch == '*') // 说明读取到的是注释
        {
            m_getch();

            while (ch != '*')
            {
                m_getch();//注释没结束之前一直读取注释，但不输出
                if (ch == '*')
                {
                    m_getch();
                    if (ch == '/')//注释结束
                    {
                        myword->typenum = 999;
                        myword->word = "注释";
                        return (myword);
                        break;
                    }
                }
            }
        }
        else
        {
            retract();//读取到的下个字符不是*，即不是注释，则要回退，直接输出/

            myword->typenum = 32;
            myword->word = "/";
            return (myword);
            break;
        }
    case '*':
        myword->typenum = 31;
        myword->word = "*";
        return(myword);
        break;
    case '(':
        myword->typenum = 10;
        myword->word = "(";
        return(myword);
        break;
    case ')':
        myword->typenum = 11;
        myword->word = ")";
        return(myword);
        break;
    case '%':
        myword->typenum = 33;
        myword->word = "%";
        return(myword);
        break;
    case '>':
        m_getch();
        if (ch == '=')
        {
            myword->typenum = 54;
            myword->word = ">=";
            return(myword);
            break;
        }
        retract();
        myword->typenum = 53;
        myword->word = ">";
        return(myword);
        break;
    case '<':
        m_getch();
        if (ch == '=')
        {
            myword->typenum = 52;
            myword->word = "<=";
            return(myword);
            break;
        }
        else
        {
            retract();
            myword->typenum = 51;
            myword->word = "<";
            return (myword);
        }
    case '\0':
        myword->typenum = 1000;
        myword->word = "OVER";
        return(myword);
        break;
    case '&':
        m_getch();
        if (ch == '&')
        {
            myword->typenum = 71;
            myword->word = "&&";
            return(myword);
            break;
        }
        retract();
        myword->typenum = -1;
        myword->word = "ERROR";
        return(myword);
        break;
    case '|':
        m_getch();
        if (ch == '|')
        {
            myword->typenum = 81;
            myword->word = "||";
            return(myword);
            break;
        }
        retract();
        myword->typenum = -1;
        myword->word = "ERROR";
        return(myword);
        break;
    case '!':
        m_getch();
        if (ch == '=')
        {
            myword->typenum = 62;
            myword->word = "!=";
            return(myword);
            break;
        }
        else
        {
            retract();
            myword->typenum = 21;
            myword->word = "!";
            return (myword);
        }
    case '$':
        myword->typenum = 1000;
        myword->word = "OVER";
        return(myword);
        break;
    default:
        myword->typenum = -1;
        myword->word = "ERROR";
        return(myword);
        break;
    }
}

class WF
{
public:
    string left, right;
    int back;
    int id;
    WF(char s1[], char s2[], int x, int y)
    {
        left = s1;
        right = s2;
        back = x;
        id = y;
    }
    WF(const string& s1, const string& s2, int x, int y)
    {
        left = s1;
        right = s2;
        back = x;
        id = y;
    }
    bool operator < (const WF& a) const
    {
        if (left == a.left)
            return right < a.right;
        return left < a.left;
    }
    bool operator == (const WF& a) const
    {
        return (left == a.left) && (right == a.right);
    }
    void print()
    {
        if (cout_itemset)printf("%s->%s\n", left.c_str(), right.c_str());
    }
};

class Closure
{
public:
    vector<WF> element;
    void print(string str)
    {
        //项集族
        if (cout_itemset)
            printf("%-15s%-15s\n", "", str.c_str());
        for (int i = 0; i < element.size(); i++)
            element[i].print();
    }
    bool operator == (const Closure& a) const
    {
        if (a.element.size() != element.size()) return false;
        for (int i = 0; i < a.element.size(); i++)
            if (element[i] == a.element[i]) continue;
            else return false;
        return true;
    }
};

struct Content
{
    int type;
    int num;
    string out;
    Content() { type = -1; }
    Content(int a, int b)
        :type(a), num(b) {}
};

vector<WF> wf;
map<string, vector<int> > dic;
map<string, vector<int> > VN_set;
map<string, bool>  vis;
string start = "S";
vector<Closure> collection;
vector<WF> items;
char CH = '$';
int go[MAX][MAX];
int to[MAX];
vector<char> V;
bool used[MAX];
Content action[MAX][MAX];
int Goto[MAX][MAX];
map<string, set<char> > first;
map<string, set<char> > follow;

void make_item()
{
    memset(to, -1, sizeof(-1));
    for (int i = 0; i < wf.size(); i++)
        VN_set[wf[i].left].push_back(i);
    for (int i = 0; i < wf.size(); i++)
        for (int j = 0; j <= wf[i].right.length(); j++)
        {
            string temp = wf[i].right;
            temp.insert(temp.begin() + j, CH);
            dic[wf[i].left].push_back(items.size());
            if (j)
                to[items.size() - 1] = items.size();
            items.push_back(WF(wf[i].left, temp, i, items.size()));
        }
#ifdef DEBUG
    //puts("-------------------------项目表-------------------------");
    //for (int i = 0; i < items.size(); i++)
        //printf("%s->%s back:%d id:%d\n", items[i].left.c_str(), items[i].right.c_str(), items[i].back, items[i].id);
    //puts("--------------------------------------------------------");
#endif
}

void dfs(const string& x)
{
    if (vis[x]) return;
    vis[x] = 1;
    vector<int>& id = VN_set[x];
    for (int i = 0; i < id.size(); i++)
    {
        string& left = wf[id[i]].left;
        string& right = wf[id[i]].right;
        for (int j = 0; j < right.length(); j++)
            if (isupper(right[j]))
            {
                dfs(right.substr(j, 1));
                set<char>& temp = first[right.substr(j, 1)];
                set<char>::iterator it = temp.begin();
                bool flag = true;
                for (; it != temp.end(); it++)
                {
                    if (*it == '~') flag = false;
                    first[left].insert(*it);
                }
                if (flag) break;
            }
            else
            {
                first[left].insert(right[j]);
                break;
            }
    }
}

void make_first()
{
    vis.clear();
    map<string, vector<int> >::iterator it2 = dic.begin();
    for (; it2 != dic.end(); it2++)
        if (vis[it2->first]) continue;
        else dfs(it2->first);
#ifdef DEBUG
    if (cout_first)
        puts("****************FIRST集***************************");
    map<string, set<char> >::iterator it = first.begin();
    for (; it != first.end(); it++)
    {
        if (cout_first)
            printf("FIRST(%s)={", it->first.c_str());
        set<char>& temp = it->second;
        set<char>::iterator it1 = temp.begin();
        bool flag = false;
        for (; it1 != temp.end(); it1++)
        {
            if (flag)
                if (cout_first)
                    printf(",");
            if (cout_first) printf("%c", *it1);
            flag = true;
        }
        if (cout_first) puts("}");
    }
#endif 
}

void append(const string& str1, const string& str2)
{
    set<char>& from = follow[str1];
    set<char>& to = follow[str2];
    set<char>::iterator it = from.begin();
    for (; it != from.end(); it++)
        to.insert(*it);
}

bool _check(const vector<int>& id, const string str)
{
    for (int i = 0; i < id.size(); i++)
    {
        int x = id[i];
        if (wf[x].right == str) return true;
    }
    return false;
}

void make_follow()
{
    while (true)
    {
        bool goon = false;
        map<string, vector<int> >::iterator it2 = VN_set.begin();
        for (; it2 != VN_set.end(); it2++)
        {
            vector<int>& id = it2->second;
            for (int i = 0; i < id.size(); i++)
            {
                bool flag = true;
                WF& tt = wf[id[i]];
                string& left = tt.left;
                const string& right = tt.right;
                for (int j = right.length() - 1; j >= 0; j--)
                    if (isupper(right[j]))
                    {
                        if (flag)
                        {
                            int tx = follow[right.substr(j, 1)].size();
                            append(left, right.substr(j, 1));
                            int tx1 = follow[right.substr(j, 1)].size();
                            if (tx1 > tx) goon = true;
                            if (_check(id, "~"))
                                flag = false;
                        }
                        for (int k = j + 1; k < right.length(); k++)
                            if (isupper(right[k]))
                            {
                                string idd = right.substr(k, 1);
                                set<char>& from = first[idd];
                                set<char>& to = follow[right.substr(j, 1)];
                                set<char>::iterator it1 = from.begin();
                                int tx = follow[right.substr(j, 1)].size();
                                for (; it1 != from.end(); it1++)
                                    if (*it1 != '~')
                                        to.insert(*it1);
                                int tx1 = follow[right.substr(j, 1)].size();
                                if (tx1 > tx) goon = true;
                                if (_check(id, "~"))
                                    break;
                            }
                            else
                            {
                                int tx = follow[right.substr(j, 1)].size();
                                follow[right.substr(j, 1)].insert(right[k]);
                                int tx1 = follow[right.substr(j, 1)].size();
                                if (tx1 > tx) goon = true;
                                break;
                            }
                    }
                    else flag = false;
            }
        }
        if (!goon) break;
    }
#ifdef DEBUG
    if (cout_follow)
        puts("***************FOLLOW集*******************");
    map<string, set<char> >::iterator it = follow.begin();
    for (; it != follow.end(); it++)
    {
        if (cout_follow)printf("FOLLOW(%s)={", it->first.c_str());
        set<char>& temp = it->second;
        //if ( it->first[0] == 'S' )
        temp.insert('#');
        set<char>::iterator it1 = temp.begin();
        bool flag = false;
        for (; it1 != temp.end(); it1++)
        {
            if (flag) if (cout_follow)printf(",");
            if (cout_follow)printf("%c", *it1);
            flag = true;
        }
        if (cout_follow)puts("}");
    }
#endif
}

void make_set()
{
    bool has[MAX];
    for (int i = 0; i < items.size(); i++)
        if (items[i].left[0] == 'S' && items[i].right[0] == CH)
        {
            Closure temp;
            string& str = items[i].right;
            vector<WF>& element = temp.element;
            element.push_back(items[i]);
            int x = 0;
            for (x = 0; x < str.length(); x++)
                if (str[x] == CH)
                    break;
            memset(has, 0, sizeof(has));
            has[i] = 1;
            if (x != str.length() - 1)
            {
                queue<string> q;
                q.push(str.substr(x + 1, 1));
                while (!q.empty())
                {
                    string u = q.front();
                    q.pop();
                    vector<int>& id = dic[u];
                    for (int j = 0; j < id.size(); j++)
                    {
                        int tx = id[j];
                        if (items[tx].right[0] == CH)
                        {
                            if (has[tx]) continue;
                            has[tx] = 1;
                            if (isupper(items[tx].right[1]))
                                q.push(items[tx].right.substr(1, 1));
                            element.push_back(items[tx]);
                        }
                    }
                }
            }
            collection.push_back(temp);
        }
    for (int i = 0; i < collection.size(); i++)
    {
        map<int, Closure> temp;
        for (int j = 0; j < collection[i].element.size(); j++)
        {
            string str = collection[i].element[j].right;
            int x = 0;
            for (; x < str.length(); x++)
                if (str[x] == CH) break;
            if (x == str.length() - 1)
                continue;
            int y = str[x + 1];
            int ii;
            //cout << i << "previous: " << str << endl;
            str.erase(str.begin() + x);
            str.insert(str.begin() + x + 1, CH);
            //cout << i <<"after: " << str << endl;
            WF cmp = WF(collection[i].element[j].left, str, -1, -1);
            for (int k = 0; k < items.size(); k++)
                if (items[k] == cmp)
                {
                    ii = k;
                    break;
                }
            //string& str1 = items[ii].right;
            memset(has, 0, sizeof(has));
            vector<WF>& element = temp[y].element;
            element.push_back(items[ii]);
            has[ii] = 1;
            x++;
            if (x != str.length() - 1)
            {
                queue<string> q;
                q.push(str.substr(x + 1, 1));
                while (!q.empty())
                {
                    string u = q.front();
                    q.pop();
                    vector<int>& id = dic[u];
                    for (int j = 0; j < id.size(); j++)
                    {
                        int tx = id[j];
                        if (items[tx].right[0] == CH)
                        {
                            if (has[tx]) continue;
                            has[tx] = 1;
                            if (isupper(items[tx].right[1]))
                                q.push(items[tx].right.substr(1, 1));
                            element.push_back(items[tx]);
                        }
                    }
                }
            }
        }
        map<int, Closure>::iterator it = temp.begin();
        for (; it != temp.end(); it++)
            collection.push_back(it->second);
        for (int i = 0; i < collection.size(); i++)
            sort(collection[i].element.begin(), collection[i].element.end());
        for (int i = 0; i < collection.size(); i++)
            for (int j = i + 1; j < collection.size(); j++)
                if (collection[i] == collection[j])
                    collection.erase(collection.begin() + j);
    }
#ifdef DEBUG
    //puts("-------------CLOSURE---------------------");
    stringstream sin;
    for (int i = 0; i < collection.size(); i++)
    {
        sin.clear();
        string out;
        sin << "closure-I" << i;
        sin >> out;
        collection[i].print(out);
    }
    //puts("");
#endif  
}

void make_V()
{
    memset(used, 0, sizeof(used));
    for (int i = 0; i < wf.size(); i++)
    {
        string& str = wf[i].left;
        for (int j = 0; j < str.length(); j++)
        {
            if (used[str[j]]) continue;
            used[str[j]] = 1;
            V.push_back(str[j]);
        }
        string& str1 = wf[i].right;
        for (int j = 0; j < str1.length(); j++)
        {
            if (used[str1[j]]) continue;
            used[str1[j]] = 1;
            V.push_back(str1[j]);
        }
    }
    sort(V.begin(), V.end());
    V.push_back('#');
}

void make_cmp(vector<WF>& cmp1, int i, char ch)
{
    for (int j = 0; j < collection[i].element.size(); j++)
    {
        string str = collection[i].element[j].right;
        int k;
        for (k = 0; k < str.length(); k++)
            if (str[k] == CH)
                break;
        if (k != str.length() - 1 && str[k + 1] == ch)
        {
            str.erase(str.begin() + k);
            str.insert(str.begin() + k + 1, CH);
            cmp1.push_back(WF(collection[i].element[j].left, str, -1, -1));
        }
    }
    sort(cmp1.begin(), cmp1.end());
}

void make_go()
{
    memset(go, -1, sizeof(go));
    int m = collection.size();
    for (int t = 0; t < V.size(); t++)
    {
        char ch = V[t];
        for (int i = 0; i < m; i++)
        {
            vector<WF> cmp1;
            make_cmp(cmp1, i, ch);
            //cout << cmp1.size() << endl;
            if (cmp1.size() == 0) continue;
            for (int j = 0; j < m; j++)
            {
                vector<WF> cmp2;
                for (int k = 0; k < collection[j].element.size(); k++)
                {
                    string& str = collection[j].element[k].right;
                    int x;
                    for (x = 0; x < str.length(); x++)
                        if (str[x] == CH)
                            break;
                    if (x && str[x - 1] == ch)
                        cmp2.push_back(WF(collection[j].element[k].left, str, -1, -1));
                }
                sort(cmp2.begin(), cmp2.end());
                //cout << cmp2.size() << endl;
                bool flag = true;
                if (cmp2.size() != cmp1.size()) continue;
                //cout << cmp1.size() << endl;
                for (int k = 0; k < cmp1.size(); k++)
                    if (cmp1[k] == cmp2[k]) continue;
                    else flag = false;
                //cout << "out " << endl;
                if (flag)
                    go[i][ch] = j;
            }
            //cout << "YES" << endl;
        }
    }
#ifdef DEBUG
    //puts("---------------EDGE----------------------");
    stringstream sin;
    string out;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            for (int k = 0; k < MAX; k++)
                if (go[i][k] == j)
                {
                    sin.clear();
                    sin << "I" << i << "--" << (char)(k) << "--I" << j;
                    sin >> out;
                    //printf("%s\n", out.c_str());
                }
#endif
}

void make_table()
{
    memset(Goto, -1, sizeof(Goto));
    //write s to the table 
    for (int i = 0; i < collection.size(); i++)
        for (int j = 0; j < V.size(); j++)
        {
            char ch = V[j];
            int x = go[i][ch];
            if (x == -1) continue;
            if (!isupper(ch))
                action[i][ch] = Content(0, x);
            else
                Goto[i][ch] = x;
        }
    //write r and acc to the table 
    for (int i = 0; i < collection.size(); i++)
        for (int j = 0; j < collection[i].element.size(); j++)
        {
            WF& tt = collection[i].element[j];
            if (tt.right[tt.right.length() - 1] == CH)
            {
                if (tt.left[0] == 'S')
                    action[i]['#'] = Content(2, -1);
                else
                    for (int k = 0; k < V.size(); k++)
                    {
                        int y = V[k];
                        //cout << "YES " << endl;
                        //cout << tt.left << "->" << tt.right << " " << tt.back << endl;
                        if (!follow[tt.left].count(V[k])) continue;
                        //cout <<tt.left << "->" << tt.right << " " <<  i << " " << V[k] << " " << tt.back << endl;
                        action[i][y] = Content(1, tt.back);
                    }
            }
        }
#ifdef DEBUG
    if (cout_action) {
        puts("------------------------------------------LR(0)分析表--------------------------------------------------------");
        printf("%10s%5c%5s", "|", V[0], "|");
        for (int i = 1; i < V.size(); i++)
            printf("%5c%5s", V[i], "|");
        puts("");
        for (int i = 0; i < (V.size() + 1) * 10; i++)
            printf("-");
        puts("");
    }
    stringstream sin;
    for (int i = 0; i < collection.size(); i++)
    {
        if (cout_action)printf("%5d%5s", i, "|");
        for (int j = 0; j < V.size(); j++)
        {
            char ch = V[j];
            if (isupper(ch))
            {
                if (cout_action) {
                    if (Goto[i][ch] == -1)
                        printf("%10s", "|");
                    else
                        printf("%5d%5s", Goto[i][ch], "|");
                }
            }
            else
            {
                sin.clear();
                if (action[i][ch].type == -1);
                if (cout_action)printf("%10s", "|");
                else
                {
                    Content& temp = action[i][ch];
                    if (temp.type == 0)
                        sin << "S";
                    if (temp.type == 1)
                        sin << "R";
                    if (temp.type == 2)
                        sin << "acc";
                    if (temp.num != -1)
                        sin << temp.num;
                    sin >> temp.out;
                    if (cout_action)printf("%7s%3s", temp.out.c_str(), "|");
                }
            }
        }
        if (cout_action)puts("");
    }
    if (cout_action) {
        for (int i = 0; i < (V.size() + 1) * 10; i++)
            printf("-");
        puts("");
    }
#endif
}

void print(string s1, string s2, string s3, string s4, string s5, string s6, string s7)
{
    printf("%-15s|%-15s%-15s%-20s|%-15s%-15s%-15s\n", s1.c_str(), s2.c_str(), s3.c_str(), s4.c_str(), s5.c_str(),
        s6.c_str(), s7.c_str());
}

string get_steps(int x)
{
    stringstream sin;
    sin << x;
    string ret;
    sin >> ret;
    return ret;
}

template<class T>
string get_stk(vector<T> stk)
{
    stringstream sin;
    for (int i = 0; i < stk.size(); i++)
        sin << stk[i];
    string ret;
    sin >> ret;
    return ret;
}

string get_shift(WF& temp)
{
    stringstream sin;
    sin << "reduce(" << temp.left << "->" << temp.right << ")";//推导过程
    string out;
    sin >> out;
    return out;
}

void analyse(string src)
{
    ofstream reduce_file("reduce.xml", ios::trunc);
    if (cout_list)print("steps", "op-stack", "input", "operation", "state-stack", "ACTION", "GOTO");
    vector<char> op_stack;
    vector<int> st_stack;
    src += "#";
    op_stack.push_back('#');
    st_stack.push_back(0);
    int steps = 1;
    for (int i = 0; i < src.length(); i++)
    {
        char u = src[i];
        int top = st_stack[st_stack.size() - 1];
        Content& act = action[top][u];
        //cout << "YES : " << i << " " << u << " " << top << " " << act.type << endl;
        if (act.type == 0)
        {
            if (cout_list)print(get_steps(steps++), get_stk(op_stack), src.substr(i), "shift", get_stk(st_stack), act.out, "");
            op_stack.push_back(u);
            st_stack.push_back(act.num);
        }
        else if (act.type == 1)
        {
            WF& tt = wf[act.num];
            int y = st_stack[st_stack.size() - tt.right.length() - 1];
            int x = Goto[y][tt.left[0]];
            //cout << y << " " << tt.left[0] << " " << x << endl;
            if (cout_induction)
                cout << get_shift(tt) << endl;
            reduce_file << act.out << endl;
            if (cout_list)print(get_steps(steps++), get_stk(op_stack), src.substr(i), get_shift(tt), get_stk(st_stack), act.out, get_steps(x));
            for (int j = 0; j < tt.right.length(); j++)
            {
                st_stack.pop_back();
                op_stack.pop_back();
            }
            op_stack.push_back(tt.left[0]);
            st_stack.push_back(x);
            i--;
        }
        else if (act.type == 2)
        {
            if (cout_list)
                print(get_steps(steps++), get_stk(op_stack), src.substr(i), "Accept", get_stk(st_stack), act.out, "");
            //i--;
        }
        else continue;
    }
    if (op_stack.size() > 2) {
        cout << "Error [error:#]" << endl;
        cout_ERROR = true;
    }
    else
        cout_ERROR = false;
    reduce_file.close();
}

void cout_varlist() {
    cout << "符号表" << endl;
    for (int i = 0; i < var_list_size; i++) {
        if (var_list_symbol[i] == 5)
            cout << "[ " << "integer" << " " << var_list_name[i] << " ]" << endl;
        else
            cout << "[ " << "decimal" << " " << var_list_name[i] << " ]" << endl;
    }
}

void add_symbols_txt() {
    ofstream outfile("symbols.xml", ios::trunc);
    for (int i = 0; i < var_list_size; i++)
        outfile << "[ " << var_list_symbol[i] << " " << var_list_name[i] << " " << var_list_value[i] << " ]" << endl;
    outfile.close();
    return;
}

void add_var(int n, string token_str) {//添加符号表
    token_str = token_str.substr(2, token_str.length());
    for (int i = 0; i < var_list_size; i++) {
        if (var_list_name[i] == token_str)
            return;
    }
    var_list_symbol[var_list_size] = n;
    var_list_name[var_list_size] = token_str;
    var_list_size++;
    add_symbols_txt();
}

string get_tokens() {
    string token_temp = "";
    string token_temp1 = "";
    ifstream file("out.xml");
    while (getline(file, token_temp1)) {
        int num_token = atoi(token_temp1.c_str());
        switch (num_token) {
        case 1:token_temp.append("id"); break;
        case 2:token_temp.append("true"); break;
        case 3:token_temp.append("false"); break;
        case 4:token_temp.append("number"); break;
        case 5:
            getline(file, token_temp1);
            if (atoi(token_temp1.c_str()) == 8)
                add_var(5, token_temp1);
            break;
        case 6:
            getline(file, token_temp1);
            if (atoi(token_temp1.c_str()) == 8)
                add_var(6, token_temp1);
            break;
        case 7:token_temp.append("bool"); break;
        case 8:token_temp.append("var"); break;
        case 10:token_temp.append("("); break;
        case 11:token_temp.append(")"); break;
        case 21:token_temp.append("!"); break;
        case 22:token_temp.append("++"); break;
        case 23:token_temp.append("--"); break;
        case 31:token_temp.append("*"); break;
        case 32:token_temp.append("/"); break;
        case 33:token_temp.append("%"); break;
        case 41:token_temp.append("+"); break;
        case 42:token_temp.append("-"); break;
        case 51:token_temp.append("<"); break;
        case 52:token_temp.append("<="); break;
        case 53:token_temp.append(">"); break;
        case 54:token_temp.append(">="); break;
        case 61:token_temp.append("=="); break;
        case 62:token_temp.append("!="); break;
        case 71:token_temp.append("&&"); break;
        case 81:token_temp.append("||"); break;
        case 91:token_temp.append("="); break;
        case 999:token_temp.append(""); break;
        case 1000:token_temp.append(""); break;
        }
    }
    file.close();
    return token_temp;
}

void init() {
    int n;
    char s[MAX];
    ifstream myfile("production.xml");
    string temp;
    getline(myfile, temp);
    n = atoi(temp.c_str());
    for (int i = 0; i < n; i++)
    {
        getline(myfile, temp);
        int k = 0;
        for (k = 0; k < temp.length(); k++) {
            s[k] = temp[k];
        }
        s[k] = '\0';
        int len = strlen(s), j;
        for (j = 0; j < len; j++)
            if (s[j] == '-') break;
        s[j] = 0;
        wf.push_back(WF(s, s + j + 2, -1, -1));
#ifdef DEBUG
        wf[wf.size() - 1].print();
#endif
    }
    make_item();
    make_first();
    make_follow();
    make_set();
    make_V();
    make_go();
    make_table();
}

void rush_var_list() {
    for (int i = 0; i < var_list_size; i++) {
        if (var_list_symbol[i] == 5)
            var_list_value[i] = int(var_list_value[i]);
    }
}

void make_production() {
    ifstream production_file("production.xml");
    string str;
    getline(production_file, str);
    while (getline(production_file, str)) {
        cout << str << endl;
    }
}

void result() {
    rush_var_list();
    stack<double> result_stack;
    ifstream production_file("production.xml");
    ifstream token_file("out.xml");
    ifstream reduce_file("reduce.xml");
    string reduce_str;
    string token_str, t;
    double num[50];
    string str[50];
    int _str_size = 0;//非关键字指针
    int _str_size_now = 0;
    int num_ptr = 0;//num最大指针
    int reduce_ptr = 0;//reduce栈用到的num当前指针
    while (getline(token_file, token_str)) {//token提取数字
        stringstream linestream;
        linestream.str(token_str);
        string first = token_str.substr(0, 1);
        int n = atoi(token_str.c_str());
        if (n == 1) {
            token_str = token_str.substr(1, token_str.length());
            double num1 = atof(token_str.c_str());
            num[num_ptr] = num1;
            num_ptr++;
        }
    }
    token_file.clear();//读文件指针回到起始
    token_file.seekg(ios_base::beg);
    while (getline(token_file, token_str)) {//token提取非关键字
        stringstream linestream;
        linestream.str(token_str);
        linestream >> t;
        char c = t[0];
        int num1 = atoi(t.c_str());
        if (num1 == 8) {
            str[_str_size] = token_str.substr(2, token_str.length());
            _str_size++;
        }
    }
    while (getline(reduce_file, reduce_str)) {
        reduce_str = reduce_str.substr(1, reduce_str.length());
        int n = atoi(reduce_str.c_str());
        n = n + 2;
        double a, b;
        bool not_found_flag = true;
        string symbols_str, symbols_str1;
        switch (n) {
            //32 寻找符号表中值
        case 32:
            token_file.clear();//读文件指针回到起始
            token_file.seekg(ios_base::beg);
            getline(token_file, symbols_str);
            if (!getline(token_file, symbols_str1)) {
                symbols_str = symbols_str.substr(2, symbols_str.length());
                for (int i = 0; i < var_list_size; i++) {
                    if (var_list_name[i] == symbols_str) {
                        if (cout_type) {
                            if (var_list_symbol[i] == 6)
                                cout << "[Decimal, " << var_list_value[i] << "]" << endl;
                            else if (var_list_symbol[i] == 5)
                                cout << "[Integer, " << var_list_value[i] << "]" << endl;
                        }
                        else
                            cout << var_list_value[i] << endl;
                        not_found_flag = false;
                    }
                }
                if (not_found_flag)
                    cout << "Error [undefined identifier:" << symbols_str << "]" << endl;
            }
            else {
                string str_now = str[_str_size_now];
                for (int i = 0; i < var_list_size; i++) {
                    if (var_list_name[i] == str_now) {
                        result_stack.push(var_list_value[i]);
                        _str_size_now++;
                    }
                }
            }
            break;
        case 31: result_stack.push(0); break;
        case 30: result_stack.push(1); break;
        case 29: result_stack.push(num[reduce_ptr]); reduce_ptr++; break;
        case 28:break;
        case 27:break;
        case 26:
            a = result_stack.top();
            result_stack.pop();
            if (a != 0)result_stack.push(0);
            else result_stack.push(1);
            break;
        case 25:
            a = result_stack.top();
            result_stack.pop();
            result_stack.push(a--);
            break;
        case 24:
            a = result_stack.top();
            result_stack.pop();
            result_stack.push(a++);
            break;
        case 23:break;
        case 22:
            a = result_stack.top();
            result_stack.pop();
            b = result_stack.top();
            result_stack.pop();
            result_stack.push(int(b) % int(a));
            break;
        case 21:
            a = result_stack.top();
            result_stack.pop();
            b = result_stack.top();
            result_stack.pop();
            if (a == 0)result_stack.push(b);
            else result_stack.push(b / a);
            break;
        case 20:
            a = result_stack.top();
            result_stack.pop();
            b = result_stack.top();
            result_stack.pop();
            result_stack.push(b * a);
            break;
        case 19:break;
        case 18:
            a = result_stack.top();
            result_stack.pop();
            b = result_stack.top();
            result_stack.pop();
            result_stack.push(a + b);
            break;
        case 17:
            a = result_stack.top();
            result_stack.pop();
            b = result_stack.top();
            result_stack.pop();
            result_stack.push(b - a); break;
        case 16:break;
        case 15:
            a = result_stack.top();
            result_stack.pop();
            b = result_stack.top();
            result_stack.pop();
            if (b >= a) result_stack.push(1);
            else result_stack.push(0); break;
        case 14:
            a = result_stack.top();
            result_stack.pop();
            b = result_stack.top();
            result_stack.pop();
            if (b > a) result_stack.push(1);
            else result_stack.push(0); break;
        case 13:
            a = result_stack.top();
            result_stack.pop();
            b = result_stack.top();
            result_stack.pop();
            if (b <= a) result_stack.push(1);
            else result_stack.push(0); break;
        case 12:
            a = result_stack.top();
            result_stack.pop();
            b = result_stack.top();
            result_stack.pop();
            if (b < a) result_stack.push(1);
            else result_stack.push(0); break;
        case 11:break;
        case 10:
            a = result_stack.top();
            result_stack.pop();
            b = result_stack.top();
            result_stack.pop();
            if (b == a) result_stack.push(1);
            else result_stack.push(0); break;
        case 9:
            a = result_stack.top();
            result_stack.pop();
            b = result_stack.top();
            result_stack.pop();
            if (b != a) result_stack.push(1);
            else result_stack.push(0); break;
        case 8:break;
        case 7:
            a = result_stack.top();
            result_stack.pop();
            b = result_stack.top();
            result_stack.pop();
            if (b && a) result_stack.push(1);
            else result_stack.push(0); break;
        case 6:break;
        case 5:
            a = result_stack.top();
            result_stack.pop();
            b = result_stack.top();
            result_stack.pop();
            if (b || a) result_stack.push(1);
            else result_stack.push(0); break;
        case 4:break;
        case 3:
            token_file.clear();//读文件指针回到起始
            token_file.seekg(ios_base::beg);
            getline(token_file, symbols_str);
            symbols_str = symbols_str.substr(2, symbols_str.length());
            for (int i = 0; i < var_list_size; i++) {
                if (var_list_name[i] == symbols_str) {
                    var_list_value[i] = result_stack.top();
                    not_found_flag = false;
                }
            }
            if (not_found_flag)
                cout << "not found " << symbols_str << endl;
            break;
        case 2:break;
        }
    }
    if (!result_stack.empty() && !cout_ERROR) {
        double result = result_stack.top();
        if (result == 1) {
            if (cout_type)
                cout << "[Bool, true]" << endl;
            else
                cout << "true" << endl;
        }
        else if (result == 0) {
            if (cout_type)
                cout << "[Bool, false]" << endl;
            else
                cout << "false" << endl;
        }
        else {
            ifstream file1("reduce.xml");
            string temp1, temp2;
            while (getline(file1, temp1))
                temp2 = temp1;
            file1.close();
            bool flag = true;
            if (temp2 == "R1") {//=产生式
                flag = false;
            }
            if (cout_type) {
                int result1 = result;
                float temp = result1 - result;
                if (fabs(temp) < 1e-6) {
                    if (flag)cout << "[Integer, " << result << "]" << endl;
                }
                else
                    if (flag)cout << "[decimal, " << result << "]" << endl;
            }
            else
                if (flag)cout << result << endl;
        }
    }
    production_file.close();
    token_file.close();
    reduce_file.close();
}

void cout_help() {
    cout << "---------------------------------" << endl;
    cout << "编辑器中所有变量都需要定义才可使用，数据类型有integer和decimal" << endl;
    cout << "可输入命令如下：默认只输出值 " << endl;
    cout << "    $type（输出类型和值）" << endl;
    cout << "    $induction(输出推到产生式）" << endl;
    cout << "    $symbols(输出符号表）" << endl;
    cout << "    $default(恢复默认值）" << endl;
    cout << "    $first(输出文法first集）" << endl;
    cout << "    $follow(输出文法follow集）" << endl;
    cout << "    $production(输出所有产生式）" << endl;
    cout << "    $help(输出帮助）" << endl;
    cout << "    $list(输出语法分析动作表）" << endl;
    cout << "    ##(退出）" << endl;
    cout << "---------------------------------" << endl;
}

int main()
{
    init();
    string input_str;
    bool flag = true;
    cout_help();
    while (flag) {
        cout << ">>>";
        getline(cin, input_str);
        if (input_str == "##") {
            cout << "quit" << endl;
            return 0;
        }
        else if (input_str == "$symbols") {
            cout_varlist();
        }
        else if (input_str == "$first") {
            cout_first = !cout_first;
            make_first();
            cout_first = !cout_first;
        }
        else if (input_str == "$follow") {
            cout_follow = !cout_follow;
            make_follow();
            cout_follow = !cout_follow;
        }
        else if (input_str == "$production") {
            make_production();
        }
        else if (input_str == "$type") {
            cout_type = !cout_type;
            cout << "$type=" << cout_type << endl;
        }
        else if (input_str == "$induction") {
            cout_induction = !cout_induction;
            cout << "$induction=" << cout_induction << endl;
        }
        else if (input_str == "$default") {
            bool cout_type = false;
            bool cout_induction = false;
            bool cout_action = false;
            bool cout_list = false;
            bool cout_first = false;
            bool cout_follow = false;
            bool cout_itemset = false;
            bool cout_ERROR = false;
            cout << "init" << endl;
        }
        else if (input_str == "$list") {
            cout_list = !cout_list;
            cout << "$list=" << cout_list << endl;
        }
        else if (input_str == "$help") {
            cout_help();
        }
        else {
            words(input_str);
            analyse(get_tokens());
            result();
        }
    }
    return 0;
}