#pragma once
#include<iostream>
#include<cstdio>
#include<cmath>
#include<algorithm>
#include<cstdlib>
#include<cstring>

class DFSCard
{
public:
	int n, ans, a[14], first[14], first_1[14], first_2[14], first_flag;//分别为牌数、手数，一些辅助数组
	DFSCard();//构造函数
	int work(int* x);//辅助用函数
	void add_to_dfs(int* x, int n);//将牌加到待计算的数组
	void dfs(int* x, int step);//深搜
	void getFirst();//获取第一手牌
	void clear();//清空
};