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
	int n, ans, a[14], first[14], first_1[14], first_2[14], first_flag;//�ֱ�Ϊ������������һЩ��������
	DFSCard();//���캯��
	int work(int* x);//�����ú���
	void add_to_dfs(int* x, int n);//���Ƽӵ������������
	void dfs(int* x, int step);//����
	void getFirst();//��ȡ��һ����
	void clear();//���
};