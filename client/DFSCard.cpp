#include "DFSCard.h"

DFSCard::DFSCard()//初始化变量
{
	n = 0;
	ans = 20;
	first_flag = 0;
	for (int i = 0; i <= 13; i++)
	{
		a[i] = 0;
		first[i] = 0;
		first_1[i] = 0;
		first_2[i] = 0;
	}
}

void DFSCard::add_to_dfs(int* c, int n)
{
	this->n = n;
	for (int i = 0; i < n; i++)//按照规则来给牌分类
	{
		int x;
		if (c[i] / 4 >= 0 && c[i] / 4 <= 10)// 3-K
			x = c[i] / 4 + 3;
		else if (c[i] / 4 == 11)// A
			x = 1;
		else if (c[i] / 4 == 12)// 2
			x = 2;
		else if (c[i] / 4 == 13)// 王
			x = 0;
		if (x == 1)//如果是A，编号13
			x = 13;
		else //否则编号为x-1
			if (x)
				x--;
		this->a[x]++;//加入到对应编号项
	}
}

int DFSCard::work(int* x)
{
	int y[10], num = 0;
	memset(y, 0, sizeof(y));
	for (int i = 0; i <= 13; i++)//计算单牌、对牌、三带、四带的个数
		y[x[i]]++;
	while (y[4] >= 1 && y[2] >= 2)//四带一对
	{
		num++;
		y[4]--;
		y[2] -= 2;
	}
	while (y[4] >= 1 && y[1] >= 2) //四带二单
	{
		num++;
		y[4]--;
		y[1] -= 2;
	}
	while (y[4] >= 1 && y[2] >= 1) //四带二对
	{
		num++;
		y[4]--;
		y[2]--;
	}
	while (y[3] >= 1 && y[2] >= 1) //三带一对
	{
		num++;
		y[3]--;
		y[2]--;
	}
	while (y[3] >= 1 && y[1] >= 1)//三带单
	{
		num++;
		y[3]--;
		y[1]--;
	}
	return num + y[1] + y[2] + y[3] + y[4];//返回本次计算的手数
}

void DFSCard::dfs(int* x, int step)//贪心+深搜+剪枝，如果得出的手数比上次的多，返回，否则接着计算
{
	int yu = work(x);
	ans = std::min(ans, step + yu);
	if (step >= ans)
		return;
	first_flag++;

	/* for (int i = 0; i <= 13; i++)
		 std::cout << x[i] << " ";
	 std::cout << "\n";*/

	if (first_flag == 1)
	{
		for (int i = 0; i <= 13; i++)
		{
			int n;
			if (i == 0)
				n = 13;
			else if (i == 1)
				n = 12;
			else if (i == 13)
				n = 11;
			else
				n = i - 2;
			first_1[n] = a[i] - x[i];
		}
	}
	if (first_flag == 2)
	{
		for (int i = 0; i <= 13; i++)
		{
			int n;
			if (i == 0)
				n = 13;
			else if (i == 1)
				n = 12;
			else if (i == 13)
				n = 11;
			else
				n = i - 2;
			first_2[n] = a[i] - x[i];
		}
	}
	int y[20];
	memset(y, 0, sizeof(y));
	for (int i = 0; i <= 13; i++)
		y[i] = x[i];
	for (int i = 2; i <= 13; i++)
	{
		int j = i;
		while (x[j] >= 3)
			j++;
		while (j - i >= 2)
		{
			for (int k = i; k < j; k++)
				y[k] -= 3;
			dfs(y, step + 1);
			for (int k = i; k < j; k++)
				y[k] += 3;
			j--;
		}
	}
	for (int i = 2; i <= 13; i++)
	{
		int j = i;
		while (x[j] >= 2)
			j++;
		while (j - i >= 3)
		{
			for (int k = i; k < j; k++)
				y[k] -= 2;
			dfs(y, step + 1);
			for (int k = i; k < j; k++)
				y[k] += 2;
			j--;
		}
	}
	for (int i = 2; i <= 13; i++)
	{
		int j = i;
		while (x[j] >= 1)
			j++;
		while (j - i >= 5)
		{
			for (int k = i; k < j; k++)
				y[k]--;
			dfs(y, step + 1);
			for (int k = i; k < j; k++)
				y[k]++;
			j--;
		}
	}
}

void DFSCard::getFirst()//获取第一手牌
{
	bool flag1 = false, flag2 = false;
	for (int i = 0; i <= 13; i++)
		if (first_1[i])
			flag1 = true;
	for (int i = 0; i <= 13; i++)
		if (first_2[i])
			flag2 = true;
	if (flag2)
	{
		for (int i = 0; i <= 13; i++)
			first[i] = first_2[i];
	}
	else if (flag1)
	{
		for (int i = 0; i <= 13; i++)
			first[i] = first_1[i];
	}
	else
	{
		for (int i = 0; i <= 13; i++)
		{
			int n;
			if (i == 0)
				n = 13;
			else if (i == 1)
				n = 12;
			else if (i == 13)
				n = 11;
			else
				n = i - 2;
			first[n] = a[i];
		}
	}
}

void DFSCard::clear()//清空
{
	for (int i = 0; i <= 13; i++)
	{
		a[i] = 0;
		first[i] = 0;
		first_1[i] = 0;
		first_2[i] = 0;
	}
	first_flag = 0;
	ans = 20;
	n = 0;
}