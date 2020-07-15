#include "DFSCard.h"

DFSCard::DFSCard()//��ʼ������
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
	for (int i = 0; i < n; i++)//���չ��������Ʒ���
	{
		int x;
		if (c[i] / 4 >= 0 && c[i] / 4 <= 10)// 3-K
			x = c[i] / 4 + 3;
		else if (c[i] / 4 == 11)// A
			x = 1;
		else if (c[i] / 4 == 12)// 2
			x = 2;
		else if (c[i] / 4 == 13)// ��
			x = 0;
		if (x == 1)//�����A�����13
			x = 13;
		else //������Ϊx-1
			if (x)
				x--;
		this->a[x]++;//���뵽��Ӧ�����
	}
}

int DFSCard::work(int* x)
{
	int y[10], num = 0;
	memset(y, 0, sizeof(y));
	for (int i = 0; i <= 13; i++)//���㵥�ơ����ơ��������Ĵ��ĸ���
		y[x[i]]++;
	while (y[4] >= 1 && y[2] >= 2)//�Ĵ�һ��
	{
		num++;
		y[4]--;
		y[2] -= 2;
	}
	while (y[4] >= 1 && y[1] >= 2) //�Ĵ�����
	{
		num++;
		y[4]--;
		y[1] -= 2;
	}
	while (y[4] >= 1 && y[2] >= 1) //�Ĵ�����
	{
		num++;
		y[4]--;
		y[2]--;
	}
	while (y[3] >= 1 && y[2] >= 1) //����һ��
	{
		num++;
		y[3]--;
		y[2]--;
	}
	while (y[3] >= 1 && y[1] >= 1)//������
	{
		num++;
		y[3]--;
		y[1]--;
	}
	return num + y[1] + y[2] + y[3] + y[4];//���ر��μ��������
}

void DFSCard::dfs(int* x, int step)//̰��+����+��֦������ó����������ϴεĶ࣬���أ�������ż���
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

void DFSCard::getFirst()//��ȡ��һ����
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

void DFSCard::clear()//���
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