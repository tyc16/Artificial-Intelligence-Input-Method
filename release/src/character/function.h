#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <string>
#include <cmath>

inline void ERROR(int x)
{
	if (x == 1) puts("过多参数。");
	if (x == 2) puts("过少参数，请指定输入文件和输出文件的路径。");
	if (x == 2) puts("例如 pinyin.exe ../data/input.txt ../data/output.txt");
	if (x == 3) puts("找不到输入文件。");
	if (x == 4) puts("找不到数据文件 1-gram");
	if (x == 5) puts("找不到数据文件 2-gram");
	exit(1);
}

#define MAXPINYIN 500		// 拼音总数
#define MAXCHAR 30000		// 单字总数

#define Int(i, j) i*MAXCHAR+j

using namespace std;

int PinyinTotal = 0, CharTotal = 0;

string Pinyin[MAXPINYIN];
map<string,int> Pinyin_map;
vector<int> Pinyin_vec[MAXPINYIN];

pair<char,char> Char[MAXCHAR];
int Char_pinyin[MAXCHAR];
map<pair<pair<char,char>,int>,int> Char_map;
map<pair<char,char>,vector<int> > Char_vec;

map<int,int> Times_2;
int Times_1[MAXCHAR];

inline int Character_ID(pair<char,char> ch, string py)
{
	if (py=="n") py="en";
	if (py=="lve") py="lue";
	if (py=="nve") py="nue";

	if (Char_map.count(make_pair(ch, Pinyin_map[py])))
		return Char_map[make_pair(ch, Pinyin_map[py])];

	if (Pinyin_map[py] == 0)
		Pinyin[++PinyinTotal] = py,
		Pinyin_map[py] = PinyinTotal;

	int py_id = Pinyin_map[py];

	CharTotal ++;
	Char[CharTotal] = ch;
	Char_pinyin[CharTotal] = py_id;
	Char_map[make_pair(ch, py_id)] = CharTotal;
	Pinyin_vec[py_id].push_back(CharTotal);
	Char_vec[ch].push_back(py_id);

	return CharTotal;
}

inline void ReadData()
{
	char str[50], py[50];

	// ============================================ 1-Gram

	ifstream fin1("1-gram");
	if (!fin1) ERROR(4);

	while (fin1 >> str >> py)
		fin1 >> Times_1[Character_ID(make_pair(str[0], str[1]), py)];

	// ============================================ 2-Gram

	ifstream fin2("2-gram");
	if (!fin2) ERROR(5);

	while (fin2 >> str)
	{
		int id1 = Char_map[make_pair(make_pair(str[0], str[1]), Char_vec[make_pair(str[0], str[1])][str[2]-'0'])];
		int id2 = Char_map[make_pair(make_pair(str[3], str[4]), Char_vec[make_pair(str[3], str[4])][str[5]-'0'])];

		fin2 >> Times_2[Int(id1, id2)];
	}
}

inline int ChineseChar(const char ch, const char ch2)
{
	if (((int)0xffffffa1 <= (int)ch && (int)ch <= (int)0xffffffa8) || ((int)ch == (int)0xffffffa9 && (int)ch2 != (int)0xffffff96))
		return -1;		// 中文字符
	if (ch & 0x80)
		return 1;		// 汉字
	return 0;
}

#include <sstream>
string Int_to_String(int n)
{
	ostringstream stream;
	stream << n;
	return stream.str();
}

inline string OutputChar(const int id)
{
	//cout << "Character " << id << ": " << Char[id].first << Char[id].second << " " << Pinyin[Char_pinyin[id]] << endl;

	string tmp = ""; int i = 0;
	while (Char_vec[Char[id]][i] != Char_pinyin[id]) i++;
	return tmp + Char[id].first + Char[id].second + Int_to_String(i);
}

inline void OutputChar2(const int id)
{
	cout << "Character " << id << ": " << Char[id].first << Char[id].second << " " << Pinyin[Char_pinyin[id]] << endl;
}
