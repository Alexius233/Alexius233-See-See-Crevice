
#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cstdlib>
#include<math.h>
#include"wordanaly.h"
#define itp 0
#define rtp 1
#define ctp 2

using namespace std;
void symbollist::showlist(){
	int i;
	 cout << endl << "SYMBOL:";
     cout << endl<< "name "<< " " << "type "<< ' ' << "cat "<< ' ' << "add"<<endl;
    for (i = 1; i < SYMBOL.size(); i++) {
        cout << SYMBOL[i].name<<"      "<< SYMBOL[i].type<<"     "<< SYMBOL[i].cat<<endl;
        //if (i != SYMBOL.size() - 1)
           // cout << " ";

    }

    cout << endl << "C1 :";
    for (i = 1; i < C_list.size(); i++) {
        cout << C_list[i];
        if (i != C_list.size() - 1)
            cout << " ";


    }
    cout << endl << "C2 :";
    for (i = 1; i < C2_list.size(); i++) {
        cout << C2_list[i];
        if (i != C2_list.size() - 1)
            cout << " ";


    }
    cout << endl << "CT :";
    for (i = 1; i < CT_list.size(); i++) {
        cout << CT_list[i];
        if (i != CT_list.size() - 1)
            cout << " ";


    }
    cout << endl << "ST :";
    for (i = 1; i < ST_list.size(); i++) {
        cout << ST_list[i];
        if (i != ST_list.size() - 1)
            cout << " ";


    }
}
	
void symbollist:: creat_list() {
    symbol s;
    SYMBOL.push_back(s);
    C_list.push_back(0);
    CT_list.push_back("#");
    C2_list.push_back("#");
    ST_list.push_back("#");
    K_list.push_back("#");
    K_list.push_back("int");
    K_list.push_back("void");
    K_list.push_back("break");
    K_list.push_back("float");
    K_list.push_back("while");
    K_list.push_back("do");
    K_list.push_back("struct");
    K_list.push_back("const");
    K_list.push_back("case");
    K_list.push_back("for");
    K_list.push_back("return");
    K_list.push_back("if");
    K_list.push_back("default");
    K_list.push_back("else");
    K_list.push_back("main");
    K_list.push_back("char");
    P_list.push_back("#");
    P_list.push_back("-");
    P_list.push_back("/");
    P_list.push_back("(");
    P_list.push_back(")");
    P_list.push_back("==");
    P_list.push_back("<=");
    P_list.push_back("!=");
    P_list.push_back(">=");
    P_list.push_back("<");
    P_list.push_back("+");
    P_list.push_back("*");
    P_list.push_back(">");
    P_list.push_back("=");
    P_list.push_back(",");
    P_list.push_back(";");
    P_list.push_back("++");
    P_list.push_back("{");
    P_list.push_back("}");
    P_list.push_back("[");
    P_list.push_back("]");
    P_list.push_back("||");
    P_list.push_back("&&");
}

void symbollist:: search_P(string& str) {

    int i;
    for (i = 1; i < P_list.size(); i++)
    {

        if (P_list[i] == str) {
        	t.type="P";
        	t.point=i;
            //cout << "(P " << i << ")";
        }
    }
    return;
}
int symbollist:: search_K(string& str) {
    int i = 1;
    for (i = 1; i < K_list.size(); i++)
    {
        if (K_list[i] == str)
        
        {
        	t.type="K";
        	t.point=i;
          // cout << "(K " << i << ")";
            return 1;

        }
    }
    return 0;
}

void symbollist::insert_CT(string& str) {
    int i = 1;
    for (i = 1; i < CT_list.size(); i++)
    {
        if (CT_list[i] == str) {
        	t.type="CT";
        	t.point=i;
           // cout << "(CT " << i << ")";
            return;
        }

    }
    CT_list.push_back(str);
    t.type="CT";
    t.point=i;
   // cout << "(CT " << i << ")";
    return;

}
void symbollist::insert_I(string& str) {
    int i = 1;
    symbol s;
    for (i = 1; i < SYMBOL.size(); i++)//�����ؼ��֣������Ƿ����
    {
        if (SYMBOL[i].name == str) {
        	t.type="I";
            t.point=i;
           
           // cout << "(I " << i << ")";
            return;
        }

    }
    s.name = str;
    SYMBOL.push_back(s);//δ���ֹ������
    t.type="I";
    t.point=i;
    //cout << "(I " << i << ")";
    return;

}
void symbollist:: insert_C(int str) {
    int i = 1;
    for (i = 1; i < C_list.size(); i++)
    {
        if (C_list[i] == str) {
        	t.type="C1";
            t.point=i;
            //cout << "(C1 " << i << ")";
            return;
        }

    }
    C_list.push_back(str);
    t.type="C1";
    t.point=i;
    //cout << "(C1 " << i << ")";
    return;

}

void symbollist::insert_C2(string& str) {
    int i = 1;
    for (i = 1; i < C2_list.size(); i++)
    {
        if (C2_list[i] == str) {
        	t.type="C2";
            t.point=i;

            //cout << "(C2 " << i << ")";
            return;
        }

    }
    C2_list.push_back(str);
    t.type="C2";
    t.point=i;
   // cout << "(C2 " << i << ")";
    return;
}
void symbollist:: insert_ST(string& str) {
    int i = 1;
    for (i = 1; i < ST_list.size(); i++)
    {
        if (ST_list[i] == str) {
        	t.type="ST";
            t.point=i;
           // cout << "(ST " << i << ")";
            return;
        }
    }
    ST_list.push_back(str);
    t.type="ST";
	t.point=i;
   // cout << "(ST " << i << ")";
    return;

}

int Machine::test(string& sin) {
    int point = 0;
    int step = 0;//初始状�?
    int nextstep = 0;//次�?
    while (point < sin.length()) {

        nextstep = move(step, sin[point]);
      // cout << step <<"->"<< sin[point] << nextstep<<endl;
        if (nextstep == -1)
        {
            cout << "ERROR";
            exit(1);
        }
        step = nextstep;
        switch (step) {
        case 2:
        case 4:
        case 7:
        case 8:
        case 11:
        case 12:
        case 15:
        case 16:
        case 18:
        case 35:
        case 36:
        case 26:
        case 30:
        case 37:
        case 34:step = 0; break;
        }
        point++;//指针移动

    }
    return 1;
}
void Machine::showMap() {

    int i = 0;
    for (i = 0; i < nodeNumber; i++)
    {
        mapNodePoint p1 = mapList[i].first;
        while (p1)
        {
            cout << i << '-' << p1->ch << '-' << p1->step << endl;
            p1 = p1->next;
        }
    }
}
void Machine::createMap() {//构建图节点关�?
    int i, j;
    for (i = 0; i < nodeNumber; i++)
        mapList[i].first = NULL;

    insertNode(0, ' ', 0);
    insertNode(0, '\n', 0);
    insertNode(0, '#', 55);
    insertNode(0, '_', 1);
    insertNode(0, '0', 31);
    insertNode(31, 'x', 32);
    insertNode(31, 'X', 32);
    for (i = 65; i <= 90; i++)
    {   
        insertNode(0, char(i), 1);
        insertNode(0, char(i + 32), 1);
        insertNode(1, char(i), 1);//A-Z
        insertNode(1, char(i + 32), 1);//a-z

        insertNode(19, char(i), 19);
        insertNode(21, char(i), 22);
        insertNode(5, char(i), 8);
        insertNode(6, char(i), 7);
        insertNode(9, char(i), 12);
        insertNode(10, char(i), 11);
        insertNode(13, char(i), 16);
        insertNode(14, char(i), 15);
        insertNode(17, char(i), 18);
        insertNode(38, char(i), 40);//��ֹ
        insertNode(39, char(i), 41);//��ֹ
        insertNode(42, char(i), 44);//��ֹ
        insertNode(43, char(i), 45);//��ֹ
        insertNode(51, char(i), 54);//��ֹ
        insertNode(52, char(i), 53);//��ֹ
        insertNode(47, char(i), 50);//��ֹ
        insertNode(48, char(i), 49);//��ֹ

        insertNode(38, char(i+32), 40);//��ֹ
        insertNode(39, char(i+32), 41);//��ֹ
        insertNode(42, char(i+32), 44);//��ֹ
        insertNode(43, char(i+32), 45);//��ֹ
        insertNode(51, char(i+32), 54);//��ֹ
        insertNode(52, char(i+32), 53);//��ֹ
        insertNode(47, char(i+32), 50);//��ֹ
        insertNode(48, char(i+32), 49);//��ֹ


        insertNode(5, char(i + 32), 8);
        insertNode(6, char(i + 32), 7);
        insertNode(9, char(i + 32), 12);
        insertNode(10, char(i + 32), 11);
        insertNode(13, char(i + 32), 16);
        insertNode(14, char(i + 32), 15);
        insertNode(17, char(i + 32), 18);
        insertNode(19, char(i + 32), 19);
        insertNode(21, char(i + 32), 22);
       
        if (i <= 70)
        {
            insertNode(32, char(i + 32), 33);
            insertNode(33, char(i + 32), 33);
        }

    }
    insertNode(1, '_', 1);//1 _ 1
    //insertNode(0, '-', 3);//0 - 3
    for (i = 48; i <= 58; i++) {
        insertNode(1, char(i), 1);// 1 0~9 1
     if(i!=48)
         insertNode(0, char(i), 3);//0 0~9 3
        insertNode(3, char(i), 3);//3 0~9 3
        insertNode(5, char(i), 8);
        insertNode(6, char(i), 7);
        insertNode(9, char(i), 12);
        insertNode(10, char(i), 11);
        insertNode(13, char(i), 16);
        insertNode(14, char(i), 15);
        insertNode(17, char(i), 18);
        insertNode(24, char(i), 25);
        insertNode(25, char(i), 25);
        insertNode(28, char(i), 29);
        insertNode(27, char(i), 29);
        insertNode(29, char(i), 29);
        insertNode(31, char(i), 3);
        insertNode(32, char(i), 33);
        insertNode(33, char(i), 33);
        insertNode(19, char(i), 19);
        insertNode(21, char(i), 22);
        insertNode(38, char(i), 40);//��ֹ
        insertNode(39, char(i), 41);//��ֹ
        insertNode(42, char(i), 44);//��ֹ
        insertNode(43, char(i), 45);//��ֹ
        insertNode(51, char(i), 54);//��ֹ
        insertNode(52, char(i), 53);//��ֹ
        insertNode(47, char(i), 50);//��ֹ
        insertNode(48, char(i), 49);//��ֹ
       

    }


    insertNode(0, '=', 5);//
    insertNode(5, '=', 6);//
    insertNode(0, '<', 9);//
    insertNode(9, '=', 10);//
    insertNode(0, '+', 13);//
    insertNode(13, '+', 14);
    insertNode(0, '|', 38);//
    insertNode(38, '|', 39);
    insertNode(0, '&', 42);//
    insertNode(42, '&', 43);
    insertNode(0, '>', 51);
    insertNode(51, '=', 52);
    insertNode(0, '!', 47);
    insertNode(47, '=', 48);


    insertNode(0, '-', 17);//
    insertNode(0, '/', 17);//
    insertNode(0, '(', 17);//
    insertNode(0, ')', 17);//
    insertNode(0, '*', 17);//
   // insertNode(0, '>', 17);//
    insertNode(0, ',', 17);
    insertNode(0, ';', 17);
    insertNode(0, '{', 17);
    insertNode(0, '}', 17);
    insertNode(0, ']', 17);
    insertNode(0, '[', 17);
   // insertNode(0, '!', 17);


    insertNode(3, '.', 24);
    insertNode(31, '.', 24);
    insertNode(3, 'e', 27);
    insertNode(25, 'e', 27);
    insertNode(27, '+', 28);
    insertNode(27, '-', 28);


    insertNode(0, char(34), 19);
    insertNode(19, char(34), 20);


    insertNode(0, char(39), 21);
    insertNode(22, char(39), 23);


    string str = "!=<+/ [](){}*,;->#'\"\n!&|";
    for (i = 0; i < str.length(); i++)
    {
        insertNode(1, str[i], 2);
        insertNode(3, str[i], 4);
        if (str[i] != '=')
        {
            insertNode(5, str[i], 8);
            insertNode(9, str[i], 12);
            insertNode(47, str[i], 50);
            insertNode(51, str[i], 54);
        }
        insertNode(6, str[i], 7);
        insertNode(10, str[i], 11);
        insertNode(14, str[i], 15);
        if (str[i] != '+')
            insertNode(13, str[i], 16);

        insertNode(17, str[i], 18);

        insertNode(25, str[i], 26);
        insertNode(31, str[i], 37);
        insertNode(29, str[i], 30);
        insertNode(33, str[i], 34);
        insertNode(20, str[i], 35);
        insertNode(23, str[i], 36);

        insertNode(48, str[i], 49);
        insertNode(52, str[i], 53);
        //insertNode(23, str[i], 36);
    }
}
void  Machine::getString(int& point, string sin,symbollist& sym) {
    int step = 0;//初始状�?
    int nextstep = 0;//次�?
    string sout = "";//暂存通过识别的字�?
    int num = 0;
    float num_afterpoint = 0;
    float real = 0;
    int e_num = 0;
    int sign = 1;
    int n = 0;//小数位数




    while (true) {
       // cout << "��ǰ�ַ���";
        //cout << sin[point];
        nextstep = move(step, sin[point]);//状态转移函�?
       
      // cout << step <<"->"<< sin[point] << nextstep<<endl;
        if (nextstep == -1)
        {
 
            cout << "�ʷ�ERROR��";//无可跳转状态，接受失败�?
            exit(1);
        }
        step = nextstep;

        switch (step) {
        case 0:break;

        case 2:if (!sym.search_K(sout))//识别关键字与标识�?
            sym.insert_I(sout);
            point++;
            return;


        case 3:num = 10 * num + (sin[point] - 48);//识别整数
            sout += char(sin[point]);
            break;
        case 4:sym.insert_C(num);
            point++;
            return;

        
        case 7://识别运算�?
        case 8:
        case 11:
        case 12:
        case 15:
        case 16:
        case 18: 
        case 40:
        case 41:
        case 44:
        case 45:
        case 50:
        case 49:
        case 53:
        case 54:
        case 55:

            sym.search_P(sout);
            point++;
            return;


        case 19:if (sin[point] != '"')//识别字符�?
        {
            //cout << sin[point];
            sout += char(sin[point]);
        }
        case 20:break;
        case 35:
            sym.insert_ST(sout);
            point++;
            return;


        case 21://识别字符
        case 23:break;
        case 36:sym.insert_CT(sout);
            point++;
            return;


        case 25:n++;//识别浮点�?
            num_afterpoint = num_afterpoint + pow(0.1, n) * (sin[point] - 48);
            sout += char(sin[point]);
            break;
        case 26:real = num + num_afterpoint;
            sym.insert_C2(sout);
            point++;
            return;

        case 27:real = num + num_afterpoint;
            sout += char(sin[point]);
            break;//识别科学计数�?
        case  28:sign = (sin[point] == '+' ? 1 : -1);
            sout += char(sin[point]);
            break;
        case 29:e_num = 10 * e_num + (sin[point] - 48);
            sout += char(sin[point]);
            break;
        case 30:real = real * pow(10, sign * e_num);
            sym.insert_C2(sout);
            point++;
            return;

        case 31:sout += char(sin[point]);
        case 32:break;
        case 33:num = num * 16 + (sin[point] > '9' ? (10 + sin[point] - 97) : (sin[point] - 48));
            break;
        case 34:sym.insert_C(num);
            point++;
            return;
        case 37:
            sym.insert_C(num);
            point++;
            return;
        default:sout += char(sin[point]); break;//其余状态字符串追加
        }
        
            point++;
       

    }
}

void Machine::insertNode(int oristep, char ch, int nextstep)//构建图插入节点函�?
{

    mapNodePoint p1 = NULL, p2 = NULL;
    p2 = new mapNode;//开辟空�?
    p2->ch = ch;
    p2->step = nextstep;
    p1 = mapList[oristep].first;//原first指针
    mapList[oristep].first = p2;//头插�?
    p2->next = p1;
    return;
}
int Machine::move(int x0, char ch)
{
    mapNodePoint p1 = mapList[x0].first;
    while (p1)
    {
        if (ch == p1->ch)//查询x0状态可跳转的状�?
            return p1->step;//查询成功，返回次态！
        p1 = p1->next;//工作指针移动
    }
    return -1;//查找失败，返�?1

}





