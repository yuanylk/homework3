#include<stdio.h>
#include<iostream>
#include<cstring>
using namespace std;
#define M 10
#define MAX 100
int m, k = 0;
int res[MAX];
struct Tree_node {
	int num;//该子节点有几个node
	int data[M];
	Tree_node* high;//上一层的指针
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
	//数据和指针分割点有差异
	res[0]->num = mid_data;
	res[1]->num = m - mid_data - 1;

	for (i = 0; i < mid_data; i++) {
		//奇数m中间提出左右两个同样大
		//偶数左右2-3分割
		res[0]->data[i] = p->data[i];
		res[1]->data[i] = p->data[i + mid_data + 1];
	}
	if (!(m % 2))//对偶数的右侧补充
		res[1]->data[mid_data] = p->data[m - 1];

	if (p->point[0]) {//该层之下还有层，需要划分下属层
		for (i = 0; i < mid_point; i++) {
			//奇数左右平衡
			//偶数左比右少
			res[0]->point[i] = p->point[i];
			res[0]->point[i]->high = res[0];
			res[1]->point[i] = p->point[i + mid_point];
			res[1]->point[i]->high = res[1];
		}
		if ((mid_point + 1) % 2) {//奇数指针数组补充
			res[1]->point[mid_point] = p->point[m];
			res[1]->point[mid_point]->high = res[mid_point];
		}
	}
	return res;
}
int Address(Tree_node* p, int item) {
	//某元素在该层下标，无则返回-1
	for (int i = 0; i < m; i++)
		if (p->data[i] == item)
			return i;
	return -1;
}
void depart_node(Tree_node* p) {
	//只有需要创建基础挤出去的节点层,把中间节点挪到上一层（已有的）两种情况
	int mid_data = (m - 1) / 2, mid_point = (m + 1) / 2, i = 0;
	Tree_node* part[2];
	part[0] = cut(p)[0];
	part[1] = cut(p)[1];
	Tree_node* highp = new Tree_node;

	if (p->high) {//上层有
		highp = p->high;
		part[0]->high = part[1]->high = highp;
		int item = p->data[mid_data];
		//Sert_node(highp, p->data[mid_data]);中间点插入上层不可行由于顺序编译
		//由于已知上层是确定的要插入的层，所以直接把Insert复述一下------------------------------------
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


		//在上层point中插入part-----------------------------------------------------------------------
		int k = Address(highp, p->data[mid_data]);//上层插入位置
		//part左右两个point在上层一定连续两个位置
		for (i = highp->num + 1; i >= k; i--)
			highp->point[i] = highp->point[i - 1];
		//k=2
		//       d1 d2       d1 d2 dk        d1  d2  dk
		//for:(p1 p2 p3)->(p1 p2 p2 p3)->(p1 pleft pright)
		highp->point[k] = part[0];
		highp->point[k + 1] = part[1];
		if (n == m - 1)
			depart_node(highp);//如果插入的上层有问题

	}
	//如果没有上层，那（m/2+1）中间位置作为新建上层
		//其余两部份point[0]&point[1]
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
	//当本层分裂，下面两层的high需要从新指向
}
void Insert(Tree_node* p, int item) {
	//先插入，后判断是否要拆分
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
void Sert_node(Tree_node* p, int item) {//找到适合插入的层节点
	//先找到插入层
	//该层没有数，第一个原始数
	//第一个数进去之后，就不存在空的情况
	if (!p->num) {
		p->data[0] = item;
		p->num++;
		return;
	}
	//通过point【0】判断该层下还有没有
	if (p->point[0]) {//下层还有
		if (item <= p->data[0])
			Sert_node(p->point[0], item);
		for (int i = 0; i < p->num - 1; i++) {//只有一个通道向下寻找
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
		Insert(p, item);//本层是最底层，在本层插入
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
	cout << "1.输入Add添加" << endl;
	cout << "2.输入delete删除" << endl;
	cout << "3.输入Deep深度遍历" << endl;
	cout << "4.输入Wide广度遍历" << endl;
	cout << "**********************************************" << endl;
}
int main() {
	menu();
	char order[M];
	int item;
	cout << "请输入阶数m:";
	cin >> m;
	cin >> order;

	while (strcmp(order, "Over")) {
		if (!strcmp(order, "Add")) {
			cout << "请输入要输入的字符：" << endl;
			cin >> item;
			Sert_node(head, item);
		}
		if (!strcmp(order, "Deep")) {
			cout << "深度优先搜索结果：";
			Deep_out(head);
		}
		if (!strcmp(order, "Wide")) {
			cout << endl << "广度搜索结果：";
			Wide_out(head);
		}
		if (!strcmp(order, "delete")) {
			k = 0;
			int res = {};
			cout << "请输入要删除的字符：";
			cin >> item;
			Del(item);
		}
		cin >> order;
	}
}
//BUG1,当复杂分割的时候，分割长度不是m+
