#include"grammar.h"
#include"block.h"

#include<vector>
#include "symbol.h"
using namespace std;

int main() {
	Recursive();	//�﷨������ʼ

	cout << "�����ܱ�����: " << synbollist_main_g.size() << endl;
	int test_1 = synbollist_main_g.size();
	for (int i = 0; i < test_1; i++) {
		cout << "��" << i + 1 << "�ź������ű������: " << synbollist_main_g.at(i)->func << '\t' << "����: " << synbollist_main_g.at(i)->synbollist.size() << endl;
	}
	//���
	//printGlobal();
	//printSynbollistMain_g();
	writeGlobalToFile("C:\\byresult\\Synbol.txt");
	writeSynbollistMain_gToFile("C:\\byresult\\Synbol.txt");

	//delete����  
	clearLists();	//�ͷ������Ѿ�����е�ָ��		
	clearGlobalVariable();//�ͷ�ȫ�ֱ���

	token null_point;
	null_point.name = "";
	null_point.type = 0;

	qt.push_back({ "" , null_point, null_point,null_point });//�������
	divide();			//�����黮��
	WriteQt("C:\\byresult\\Qt.txt", "��Ԫʽ��");

	firstview();
	secondview();
	reviewresQT();
	thirdview();

	system("pause");
	return 0;
}


