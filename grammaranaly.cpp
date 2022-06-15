#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cstdlib>
#include<math.h>
#define itp 0
#define rtp 1
#define ctp 2
#include"wordanaly.h"

class Grammar{
public:
	void init(string& sin) { input = sin;  point = 0; AM.createMap(); sym.creat_list(); }
	void mainprogramme();
	void YUJU_list();
	void YUJU();
//�����������
void var_define();
void var_type();
void namelist();
void var_init();
//�������ʽ���
void E();
void T();
void F();
void G();
void Q();
void OR();
void AND();
void M();
void L();
void CMP();
void B();
void C();
void EQU();
//��ֵ���
void FUZHI();
void Elist();
void nextw();
//if���
void if_YUJU();
void else_YUJU();

//�������
void com_YUJU();

//�߼����
void logic();


int point;
Machine AM;//ɨ����
symbollist sym;//���ű�
string input;
token t;//��ǰtoken
string type;
};
//---------------------------------------
int typepoint(string type) {
	if (type == "int") {
		return itp;
	}
	else if (type == "float") {
		return rtp;
	}
	else {
		return ctp;
	}


}


//--------------------------------------��������
class complier{
public:
	string input;
	int point;
	symbollist sym;
	Machine AM;
	Grammar GR;
	void begincomplie(string sin);
};
//----------------------------------------------
void complier::begincomplie(string sin){
	input=sin;
    GR.init(input);
    GR.mainprogramme();
	GR.sym.showlist();
	//point = 0;
	//sym.creat_list();
	//AM.createMap();
	//while (point < sin.length() - 1)
	//{
	//	AM.getString(point, input,sym);
	//	point--;
	//}
	//cout << endl<<"����ɹ���";
}
void Grammar::nextw(){//��ȡһ��token
	//cout << point;
	if(point<input.length()-1)
	{	
		AM.getString(point,input,sym);
		point--;
		t = sym.t;

		cout << "("<<t.type << "," << t.point<<")";
	}
	else {

		t.type = "END";
		t.point = 0;
		cout<<endl << "�ʷ������ɹ���"<< endl;
	}
}
void Grammar::mainprogramme() {
	point = 0;
	nextw();
	YUJU_list();//����
	if (t.type == "END") {//����
		cout << "�﷨����ͨ����";
		return;
	}
	else {

		cout << endl<<"δ֪ERROR!";
		exit(0);
	}



}

//��������ķ�
void Grammar::YUJU_list() {//<����>-><���><����> |$
	//cout << t.type;
	if (t.type != "END") {//���ǽ�β
		cout << "����";
		YUJU();//һ�����
		YUJU_list();
	}
	else {//�ǽ�β
		return;
	}
	
}
void Grammar::YUJU() {//<���> -><��ֵ���>|<����˵��>|<if���>|<while���>|$

	if (t.type == "K" && (sym.K_list[t.point] == "int" || sym.K_list[t.point] == "float" || sym.K_list[t.point] == "char")) {//����������ǰ��Ϊ�����������
		var_define();
	}
	else if (t.type == "K" && sym.K_list[t.point] == "if") {//�������
		//cout << "�������";
		if_YUJU();
		return;
	}
	else if (t.type == "K" && sym.K_list[t.point] == "while") {//while���

	}
	else if (t.type == "I" ) {//��ֵ���

		FUZHI();

	}
	else {
		return;
	}
}

//��������-----------------------------------------------------------------------------------------------------------------------
void Grammar::var_define() {
	if (t.type=="K"&&(sym.K_list[t.point] == "int" || sym.K_list[t.point] == "float" || sym.K_list[t.point] == "char")) {//��� 
		
		var_type();
		namelist();
		if (sym.P_list[t.point] == ";") {
			nextw();
			return;

		}
		else {
			cout << endl<<"ERROR�� ȱ�ٷֺţ�";
			exit(0);
		}


	}
	
	else {
		cout << endl<<"ERROR: δ����ı������ͣ�--"<< sym.I_list[t.point];
		exit(0);
	}

}

void Grammar::var_type(){
	if (sym.K_list[t.point] == "int" || sym.K_list[t.point] == "float" || sym.K_list[t.point] == "char") {
		nextw();
		//--------------------------------------------���嶯������ȡ��ǰ��������
		type = sym.K_list[t.point];
		//---------------------------------------------
		return;
	}
	else{
		cout<<"�Ƿ������ͣ�";
		exit(0);
	}
	
	
	
}
void Grammar:: var_init() {
	if (t.type == "I") {

		//---------------------------------------------���嶯������ȡ��ǰ�����������
		if (t.point < sym.SYMBOL.size() - 1) {//֮ǰ���ֹ��������ظ����壡
			cout << endl<<"ERROR ����"<<sym.SYMBOL[t.point].name<<"�ظ����壡";
			exit(0);
		}
		else {
			sym.SYMBOL[t.point].type = typepoint(type);
			sym.SYMBOL[t.point].cat = "v";
		}
		nextw();


		//---------------------------------------------




		if (t.type == "P" && sym.P_list[t.point] == "=") {

			nextw();
			E();
			return;
		}
		else {
			return;
		}
	}
	else {


		cout << "ERROR�� �޷����������";
		exit(0);
	}




}
void Grammar::namelist(){
	var_init();
	if (t.type == "P" && sym.P_list[t.point] == ",") {
		nextw();
		namelist();
		return;
	}
	else {
		return;
	}

	
}
//�������ʽ�ķ�------------------------------------------------------------------------------------------------------------------------------------------
void Grammar:: E() {//E->ORLɾ����ݹ�
	//cout << "�������ʽ";
	OR();
	L();
	return;

}
void Grammar::L() {//L->||ORL
	if (t.type == "P" &&sym.P_list[t.point] == "||" ) {
		nextw();
		OR();
		L();
		return;
	}
	else {

		return;
	}


}
void Grammar::OR() {

	AND();
	M();
	return;
}
void Grammar::M() {//M->&&ANDM
	if (t.type == "P" && sym.P_list[t.point] == "&&") {
		nextw();
		AND();
		M();
		return;
	}
	else {

		return;
	}

}
void Grammar::AND() {//AND->CMPB
	CMP();
	B();
	return;


}

void Grammar::B() {//B->wCMPB
	if (t.type == "P" && (sym.P_list[t.point] == "<"|| sym.P_list[t.point] == ">"|| sym.P_list[t.point] == ">="|| sym.P_list[t.point] == "<=")) {
		nextw();
		CMP();
		B();
		return;
	}
	else {

		return;
	}




}
void Grammar:: CMP() {//CMP->EQUC
	EQU();
	C();

}
void Grammar::C() {//C->wEQUC
	if (t.type == "P" && (sym.P_list[t.point] == "==" || sym.P_list[t.point] == "!=" )) {
		   nextw();
			EQU();
			C();
			return;
	}
	else {

		return;
	}


}
void Grammar::EQU() {//EQU->TG
	T();
	G();


}
void Grammar::G() {//G->wTG|$
	if (t.type=="P"&&(sym.P_list[t.point]=="+"|| sym.P_list[t.point] == "-")) {
		nextw();
		T();
		G();
		return;
	}
	else {

		return;
	}

}
void Grammar::F() {
	if (t.type == "I"||t.type=="C1"||t.type=="C2"||t.type=="CT") {
		nextw();
		return;
	}
	else if (t.type=="P"&&sym.P_list[t.point] =="(") {
		nextw();
		E();

		if (t.type == "P" && sym.P_list[t.point] == ")") {
			nextw();
			return;
		}
		else {
			cout << "ERROR: ȱ��)";
			exit(0);
		}


	}
	else {
		cout << "ERROR: �������޷��������㣡";
		exit(0);
	}

}
void Grammar:: T() {//T->FQ
	F();
	Q();
	return;
}
void Grammar::Q() {//Q->wFQ|$

	if (t.type == "P" && (sym.P_list[t.point] == "*" || sym.P_list[t.point] == "/")) {
		nextw();
		F();
		Q();
		return;
	}
	else {
		return;

	}

}

//��ֵ����ķ�----------------------------------------------------------------------------------------------------------
void Grammar::FUZHI() {

	Elist();
	if (t.type == "P" && sym.P_list[t.point] == ";") {
		nextw();
		return;
	}
	else {
		cout << "ERROR!ȱ�ٷֺţ�";
		exit(0);
	}



}
//<��ʽ��>-> <��ʶ��> = <�������ʽ>��<��ʽ��>| <��ʶ��> = <�������ʽ>

void Grammar::Elist() {
	if (t.type == "I") {
		nextw();

		if (t.type == "P" && sym.P_list[t.point] == "=") {
			nextw();
			E();

			if (t.type == "P" && sym.P_list[t.point] == ",") {
				nextw();
				Elist();
				return;

			}
			else {
				return;

			}
		}
		else {
			cout << "ERROR ȱ��=";
			exit(0);
		}
		
	
	}
}


//�������------------------------------------------------------------------------------------------
void Grammar::com_YUJU() {
	if (t.type == "P" && sym.P_list[t.point] == "{") {
		cout << "{";
		nextw();
		YUJU_list();
		if (t.type == "P" && sym.P_list[t.point] == "}") {
			cout << "pppp";
			nextw();
			cout << t.type;
			return;
		}
		else {

			cout << "ERROR ȱ��}!";
		}
		
	}
	else {//�޻����ţ�ֻ�ɸ�һ�����
		YUJU();
		return;
	}
}
//if���-----------------------------------------------------------------------------------------------
void Grammar::if_YUJU()/*<if���>-><if�ؼ���>(<�������ʽ>)<�������><else���>
                       <else���>-><else�ؼ���>(<�������ʽ>)<�������>|$*/
{

	if (t.type == "K" && sym.K_list[t.point] == "if") {
		cout << "<if�ؼ���>";
		nextw();
		if (t.type == "P" && sym.P_list[t.point] == "(") {
			//cout << "(";
			nextw();
			E();
			cout << "�������ʽ����";
			if (t.type == "P" && sym.P_list[t.point] == ")") {
				cout << ")";
				nextw();
				com_YUJU();
				else_YUJU();
				return;
			}
			else {
				cout << "ERROR�� ȱ�٣�";
			}

			return;

		}
		else {

			cout << "ERROR�� ȱ�٣�";
		}
	}
	
}
void Grammar::else_YUJU() {//<else���>-><else�ؼ���><�������>|$
	//cout << "else���";
	if (t.type == "K" && sym.K_list[t.point] == "else") {
		nextw();
		com_YUJU();
		return;
	}
	else {
		return;
	}

}

int main(){
	fstream in;
	char ch;
	string sin;
	in.open("test.txt", ios::in);
	if (!in)cout << "�ļ���ʧ��";
	while (in >> noskipws >> ch) {
		sin += ch;
	}
	
	sin+="#";
	//cout << sin;
	complier cp;
	cp.begincomplie(sin);
	
	
	
}
