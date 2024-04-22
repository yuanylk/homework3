#include<stdio.h>
#include<iostream>
#include<cstring>
using namespace std;
#define M 10
#define MAX 100
int m, k = 0;
int res[MAX];
struct Tree_node {
	int num;//���ӽڵ��м���node
	int data[M];
	Tree_node* high;//��һ���ָ��
	Tree_node* point[M + 1];
	Tree_node() {
		high = NULL;
		*point = NULL;
		num = 0;
	}
};
Tree_node* head = new Tree_node;
Tree_node** cut(Tree_node* p) {
	Tree_node* res[2];
	res[0] = new Tree_node;
	res[1] = new Tree_node;
	int mid_data = (m - 1) / 2, mid_point = (m + 1) / 2, i = 0;
	//���ݺ�ָ��ָ���в���
	res[0]->num = mid_data;
	res[1]->num = m - mid_data - 1;

	for (i = 0; i < mid_data; i++) {
		//����m�м������������ͬ����
		//ż������2-3�ָ�
		res[0]->data[i] = p->data[i];
		res[1]->data[i] = p->data[i + mid_data + 1];
	}
	if (!(m % 2))//��ż�����Ҳಹ��
		res[1]->data[mid_data] = p->data[m - 1];

	if (p->point[0]) {//�ò�֮�»��в㣬��Ҫ����������
		for (i = 0; i < mid_point; i++) {
			//��������ƽ��
			//ż���������
			res[0]->point[i] = p->point[i];
			res[0]->point[i]->high = res[0];
			res[1]->point[i] = p->point[i + mid_point];
			res[1]->point[i]->high = res[1];
		}
		if ((mid_point + 1) % 2) {//����ָ�����鲹��
			res[1]->point[mid_point] = p->point[m];
			res[1]->point[mid_point]->high = res[mid_point];
		}
	}
	return res;
}
int Address(Tree_node* p, int item) {
	//ĳԪ���ڸò��±꣬���򷵻�-1
	for (int i = 0; i < m; i++)
		if (p->data[i] == item)
			return i;
	return -1;
}
void depart_node(Tree_node* p) {
	//ֻ����Ҫ������������ȥ�Ľڵ��,���м�ڵ�Ų����һ�㣨���еģ��������
	int mid_data = (m - 1) / 2, mid_point = (m + 1) / 2, i = 0;
	Tree_node* part[2];
	part[0] = cut(p)[0];
	part[1] = cut(p)[1];
	Tree_node* highp = new Tree_node;

	if (p->high) {//�ϲ���
		highp = p->high;
		part[0]->high = part[1]->high = highp;
		int item = p->data[mid_data];
		//Sert_node(highp, p->data[mid_data]);�м������ϲ㲻��������˳�����
		//������֪�ϲ���ȷ����Ҫ����Ĳ㣬����ֱ�Ӱ�Insert����һ��------------------------------------
		int n = highp->num, flag = n;
		for (int i = 0; i < n; i++) {
			if (item <= highp->data[i]) {
				flag = i;
				break;
			}
		}
		for (int i = n; i >= flag; i--)
			highp->data[i] = highp->data[i - 1];
		highp->data[flag] = item;
		highp->num++;


		//���ϲ�point�в���part-----------------------------------------------------------------------
		int k = Address(highp, p->data[mid_data]);//�ϲ����λ��
		//part��������point���ϲ�һ����������λ��
		for (i = highp->num + 1; i >= k; i--)
			highp->point[i] = highp->point[i - 1];
		//k=2
		//       d1 d2       d1 d2 dk        d1  d2  dk
		//for:(p1 p2 p3)->(p1 p2 p2 p3)->(p1 pleft pright)
		highp->point[k] = part[0];
		highp->point[k + 1] = part[1];
		if (n == m - 1)
			depart_node(highp);//���������ϲ�������

	}
	//���û���ϲ㣬�ǣ�m/2+1���м�λ����Ϊ�½��ϲ�
		//����������point[0]&point[1]
	else {
		part[0]->high = part[1]->high = highp;
		highp->data[0] = p->data[mid_data];
		p->high = highp;
		highp->num = 1;
		highp->point[0] = part[0];
		highp->point[1] = part[1];
		head = highp;
	}
	highp = p->high;
	highp->point[0]->high = highp->point[1]->high = highp;
	//��������ѣ����������high��Ҫ����ָ��
}
void Insert(Tree_node* p, int item) {
	//�Ȳ��룬���ж��Ƿ�Ҫ���
	int n = p->num, flag = n;
	for (int i = 0; i < n; i++) {
		if (item <= p->data[i]) {
			flag = i;
			break;
		}
	}
	for (int i = n; i >= flag; i--)
		p->data[i] = p->data[i - 1];
	p->data[flag] = item;
	p->num++;
	if (n == m - 1)
		depart_node(p);
}
void Sert_node(Tree_node* p, int item) {//�ҵ��ʺϲ���Ĳ�ڵ�
	//���ҵ������
	//�ò�û��������һ��ԭʼ��
	//��һ������ȥ֮�󣬾Ͳ����ڿյ����
	if (!p->num) {
		p->data[0] = item;
		p->num++;
		return;
	}
	//ͨ��point��0���жϸò��»���û��
	if (p->point[0]) {//�²㻹��
		if (item <= p->data[0])
			Sert_node(p->point[0], item);
		for (int i = 0; i < p->num - 1; i++) {//ֻ��һ��ͨ������Ѱ��
			if (item > p->data[i] && item <= p->data[i + 1]) {
				Sert_node(p->point[i + 1], item);
				break;
			}
		}
		if (item > p->data[p->num - 1])
			Sert_node(p->point[p->num], item);
		return;
	}
	else
		Insert(p, item);//��������ײ㣬�ڱ������
}

void Wide_out(Tree_node* p) {
	int i = 0;
	for (i = 0; i < p->num; i++) {
		if (p->point[0])
			Wide_out(p->point[i]);
		cout << p->data[i] << " ";
	}
	if (p->point[0])
		Wide_out(p->point[p->num]);
}
void Deep_out(Tree_node* p) {
	int i = 0;
	for (i = 0; i < p->num; i++)
		cout << p->data[i] << " ";
	if (p->point[0])
		for (i = 0; i <= p->num; i++)
			Deep_out(p->point[i]);
}
void Deep_check(Tree_node* p) {
	int i = 0;
	for (i = 0; i < p->num; i++)
		res[k++] = p->data[i];
	if (p->point[0])
		for (i = 0; i <= p->num; i++)
			Deep_check(p->point[i]);
}
void Del(int item) {
	Deep_check(head);
	for (int i = 0; i < k; i++) {
		if (res[i] == item) {
			res[i] = '#';
			break;
		}
	}
	//cout << res;
	head = new Tree_node;
	for (int i = 0; i < k; i++) {
		if (res[i] != '#') {
			Sert_node(head, res[i]);
		}
	}
}
void menu() {
	cout << "**********************************************" << endl;
	cout << "1.����Add���" << endl;
	cout << "2.����deleteɾ��" << endl;
	cout << "3.����Deep��ȱ���" << endl;
	cout << "4.����Wide��ȱ���" << endl;
	cout << "**********************************************" << endl;
}
int main() {
	menu();
	char order[M];
	int item;
	cout << "���������m:";
	cin >> m;
	cin >> order;

	while (strcmp(order, "Over")) {
		if (!strcmp(order, "Add")) {
			cout << "������Ҫ������ַ���" << endl;
			cin >> item;
			Sert_node(head, item);
		}
		if (!strcmp(order, "Deep")) {
			cout << "����������������";
			Deep_out(head);
		}
		if (!strcmp(order, "Wide")) {
			cout << endl << "������������";
			Wide_out(head);
		}
		if (!strcmp(order, "delete")) {
			k = 0;
			int res = {};
			cout << "������Ҫɾ�����ַ���";
			cin >> item;
			Del(item);
		}
		cin >> order;
	}
}
//BUG1,�����ӷָ��ʱ�򣬷ָ�Ȳ���m+
