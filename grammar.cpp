#include "grammar.h"
#include "symbol.h"


Lexer scan;			//ɨ����
Token token_g;			//��������token��


vector<SynbollistToFunc*> synbollist_main_g;		//�����ܱ� 
SynbollistToFunc* synbollistToFunc_g = NULL;			   //��������Ӧ�ķ��ű���ָ��
SynbollistToFunc* globalsynbollistToFunc_g = new SynbollistToFunc;		//ȫ�ֱ���Ҫ����ĵط�(���ָ�벻ѹ��synbollist_main_g)
MainSheet* mainSheet_g = NULL;							//���ű���ָ��
TypeSheet* typeSheet_g = NULL;					   //���ͱ�ָ��
FuncSheet* funcSheet_g = NULL;					   //������ָ��
vector<ParaSheet*>* paras_g = NULL;				   //������ָ��
ParaSheet* para_g = NULL;						   //��������ָ��
LevelAndOffsetAndValue* levelAndOffsetAndValue_g = NULL;		   //LevelAndOffset��ָ��


/*
Լ���ڰ�ָ�븳ֵ���ϼ�֮�󣬽���ָ�븳ֵΪ�գ�
ÿ�������µĿռ�ʱ��Ҫ���жϸ�ָ���Ƿ�Ϊ�գ�
Ϊ������������¿ռ䣬��Ϊ����˵�������ˡ�
*/

vector<string> T_num;//�м����t��̬����

string func_name = "no";//��¼��ǰ��������

string type_g;//��������ĵ�������
string id_g;//��������ĵ������ݣ���ʶ����

string id_qt;//��Ԫʽ��ʶ��
string num_qt;//��Ԫʽ����λ��


string changtype_1 = "no";//���������������
string changtype_2 = "no";//���������Ҳ�����

string func_call = "no";
int parapos = 0;
int arr_num_qt = 0;
int fnum_g;	//������¼�βθ���
int offset_g;	//������¼ƫ����
int global_offset_g;
int func_call_num = 0;
queue<MainSheet*> tempMainSheetpointers;//�����ݴ��βα�ʶ������Ϊ�βμ�Ҫ����ű�ҲҪ��βα���

void Error(int col, string tmp, string tmp2)
{//������ʾ
	ofstream myfile;
	myfile.open("C:\\byresult\\error.txt");
	myfile << "*************   ERROR!  ****************" << endl;
	myfile << tmp2 << endl;
	myfile << "���� " << col << "  ���� " << tmp << "������" << endl;
	myfile.close();
	exit(1);
}

int Recursive()
{//�ӳ���������
	scan.init("C:\\byresult\\cbd.txt");//ȷ��Ҫ��ȡ���ļ�
	global_offset_g = 0;
	token_g = scan.next_Token();//��һ��token
	ZeroOrMoreDeclation();//�����б�
	if (token_g.content == "$")
	{
		cout << "�﷨����Success" << endl;
	}
	else
	{
		cout << token_g.content << endl;
	}
	return 0;
}

int Variable_Type()
{//��������
	if (token_g.content == "int" || token_g.content == "float" || token_g.content == "double" || token_g.content == "char")
	{
		type_g = token_g.content;//�������������
		token_g = scan.next_Token();
		return 1;
	}
	else
		return 0;
}

int ID()
{//��ʶ��
	if (token_g.type == 'i')
	{
		id_g = token_g.content;//��������ĵ���
		id_qt = token_g.content;//��Ԫʽ
		if (token_g.content[0] == 't')//�����м����
			T_num.push_back(token_g.content);
		token_g = scan.next_Token();
		return 1;
	}
	else
		return 0;
}

int NumCharID()
{//��ʼ������
	if (token_g.type == 'n' || token_g.type == 'f')//�����򸡵���
	{
		token_g = scan.next_Token();
		return 1;
	}
	else if (token_g.type == 'c')//�ַ�
	{
		token_g = scan.next_Token();
		return 1;
	}
	else if (ID() == 1)//��ʶ��
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int Declaration()
{//������ǰ�ַ�������
	if (Const_Declaration() == 1)//��������
		return 1;
	else if (Variable_Declaration() == 1)//���û��Զ������������
		return 1;
	else if (Statement() == 1)
		return 1;
	else
		return 0;
}

//��������
int ZeroOrMoreDeclation()
{
	if (Declaration() == 1)
	{
		if (ZeroOrMoreDeclation() == 1)
			return 1;
		else
			Error(token_g.row, token_g.content, "����");
	}
	else//��
		return 1;
}

int Const_Declaration()
{//��������
	if (token_g.content == "const")
	{
		token_g = scan.next_Token();
		if (Variable_Type() == 1)
		{
			if (ID() == 1)
			{
				if (token_g.content == "=")
				{
					token_g = scan.next_Token();
					if (NumCharID() == 1)
					{
						if (token_g.content == ";")
						{
							token_g = scan.next_Token();
							return 1;
						}
						else
							Error(token_g.row, token_g.content, "ȱ��;");
					}
					else
						Error(token_g.row, token_g.content, "������ʼ������");
				}
				else
					Error(token_g.row, token_g.content, "������������");
			}
			else
				Error(token_g.row, token_g.content, "������������");
		}
		else
			Error(token_g.row, token_g.content, "������������");
	}
	else
		return 0;
}

int Variable_Declaration()
{//������������
	if (Variable_Type() == 1)
	{
		if (ID() == 1)
		{
			if (Array_Declaration() == 1)
				return 1;
			else
				Error(token_g.row, token_g.content, "ȱ��;");
		}
		else
			Error(token_g.row, token_g.content, "������������");
	}
	else
		return 0;
}

int Array_Declaration()
{//���鼰��������
	if (token_g.content == "[")
	{
		token_g = scan.next_Token();
		if (token_g.type == 'n')
		{//���[]�������ͳ���
			int arrLength = stringToNum<int>(token_g.content);//������������arrLength��type_g�ͱ���
			if (arrLength <= 0)
			{
				cout << "���鳤��Ӧ�ô�����" << endl;
				clearLists();//�ͷ����д�����е�ָ��
				clearGlobalVariable();//�ͷ�����ȫ�ֱ���
				//clearGlobalVariable();//�ͷ�����ȫ�ֱ���
				exit(1);
			}

			//�����ж����������ǲ���ȫ�ֵ�
			if (synbollistToFunc_g != NULL)
			{//������Ӧ�ķ��ű�ָ���Ǿֲ���

				//����һ��ռλ����
				if (mainSheet_g == NULL)
				{//�½����ű�ָ��
					mainSheet_g = new MainSheet;
				}
				else
				{
					showErrowWhenCreateASheet("MainSheet");
				}
				//��������ܱ�
				mainSheet_g->content = id_g;//��ʶ����
				writeLevelAndOffsetAndValue(-1, -1);//��Ρ�ƫ������ʼ��Ϊ-1
				mainSheet_g->addr = levelAndOffsetAndValue_g;//ָ����Ϣ��
				levelAndOffsetAndValue_g = NULL;//��ʼ��ָ��������

				writeTypeSheet(type_g);//��д���ͱ�
				mainSheet_g->type = typeSheet_g;//�������ͱ���������ܱ�
				typeSheet_g = NULL;//��ʼ��ָ��������

				synbollistToFunc_g->synbollist.push_back(mainSheet_g);//��������Ӧ�ķ��ű���ָ��
				mainSheet_g = NULL;

				for (int i = 0; i < arrLength; i++)
				{//������Ԫ����������
					string currentArrId = id_g + "[" + to_string(i) + "]";//Ҫ������ű��ı��������Է��Ŵ���ʽ����

					checkSynbollistToFunc(synbollistToFunc_g->func, currentArrId);//�麯�����ű�������Ƿ��ض���

					if (mainSheet_g == NULL)
					{//�½������ܱ�
						mainSheet_g = new MainSheet;
					}
					else
					{
						showErrowWhenCreateASheet("MainSheet");
					}
					mainSheet_g->content = currentArrId;//������
					mainSheet_g->category = "v";//�����Ǳ���
					mainSheet_g->flag = 1;//�жϵ�ַָ�������

					writeTypeSheet(type_g);
					mainSheet_g->type = typeSheet_g;
					typeSheet_g = NULL;

					writeLevelAndOffsetAndValue(((FuncSheet*)(synbollistToFunc_g->synbollist.at(0)->addr))->level, offset_g);//��д��Σ���ƫ��
					if (type_g == "int" || type_g == "float")
					{
						offset_g += 4;
					}
					else if (type_g == "char")
					{
						offset_g += 1;
					}
					else if (type_g == "double")
					{
						offset_g += 8;
					}
					else
					{
						cout << "����δ֪����" << type_g << ",�䳤�Ȱ���2���ֽڼ���" << endl;
						offset_g += 2;
					}
					//����һ��LevelAndOffset������
					mainSheet_g->addr = levelAndOffsetAndValue_g;
					levelAndOffsetAndValue_g = NULL;
					synbollistToFunc_g->synbollist.push_back(mainSheet_g);
					mainSheet_g = NULL;
				}
			}
			else
			{//��ȫ�ֵ�
				//����һ��ռλ����
				if (mainSheet_g == NULL)
				{
					mainSheet_g = new MainSheet;
				}
				else
				{
					showErrowWhenCreateASheet("MainSheet");
				}
				//�������ȫ������
				mainSheet_g->content = id_g;
				writeLevelAndOffsetAndValue(-1, -1);
				mainSheet_g->addr = levelAndOffsetAndValue_g;
				levelAndOffsetAndValue_g = NULL;

				writeTypeSheet(type_g);
				mainSheet_g->type = typeSheet_g;
				typeSheet_g = NULL;

				globalsynbollistToFunc_g->synbollist.push_back(mainSheet_g);//ȫ�ֱ���ҪҪ�����ָ��global
				mainSheet_g = NULL;

				for (int i = 0; i < arrLength; i++) {
					string currentArrId = id_g + "[" + to_string(i) + "]";//Ҫ������ű��ı�����

					checkGlobal(currentArrId);//����ȫ�ֱ���

					if (mainSheet_g == NULL)
					{
						mainSheet_g = new MainSheet;
					}
					else
					{
						showErrowWhenCreateASheet("MainSheet");
					}
					mainSheet_g->content = currentArrId;
					mainSheet_g->category = "v";
					mainSheet_g->flag = 1;

					writeTypeSheet(type_g);
					mainSheet_g->type = typeSheet_g;
					typeSheet_g = NULL;

					writeLevelAndOffsetAndValue(0, global_offset_g);//ȫ�ֱ������ڵ�һ��

					mainSheet_g->addr = levelAndOffsetAndValue_g;
					levelAndOffsetAndValue_g = NULL;
					globalsynbollistToFunc_g->synbollist.push_back(mainSheet_g);
					mainSheet_g = NULL;

					if (type_g == "int" || type_g == "float")
					{
						global_offset_g += 4;//ȫ��offetֵ
					}
					else if (type_g == "char")
					{
						global_offset_g += 1;
					}
					else if (type_g == "double")
					{
						global_offset_g += 8;
					}
					else {
						cout << "����δ֪����" << type_g << ",�䳤�Ȱ���2���ֽڼ���" << endl;
						global_offset_g += 2;
					}
				}
			}

			token_g = scan.next_Token();
			if (token_g.content == "]")
			{
				token_g = scan.next_Token();
				if (EqualStringOrNumOrNull() == 1)
				{//�����������ʼ��
					if (token_g.content == ";") {
						token_g = scan.next_Token();
						return 1;
					}
					else {
						Error(token_g.row, token_g.content, "��Ӧ����';'�ĵط�����ȷ");
					}
				}
				else {
					Error(token_g.row, token_g.content, "������ַ�������ȷ");
				}
			}
			else {
				Error(token_g.row, token_g.content, "���������鶨�岻��ȷ");
			}
		}
		else {
			Error(token_g.row, token_g.content, "���������鶨�岻��ȷ");
		}
	}

	else if (token_g.content == "=")
	{// ����Ƶ��������˵��������ʱ�Ա�����ʼ�����磺int a = 1;
		/*���嶯����ʼ������������ű���ʱ��*/
		token in;
		in = { id_qt,2,false };//��ʶ������2��ʾ����ʱ��������Ծ��Ϣ��ʼ��Ϊfalse
		PUSHSEM(in);//ѹ������ջ

		if (synbollistToFunc_g != NULL)
		{//˵���������Ǿֲ���������ǰ����ж�������ÿ��Ԫ�صķ���һ��
			cout << type_g << " " << id_g << "�Ǿֲ�����" << endl;
			/*��ʱ��Ӧ������ǣ�Ҫ��дchar c
			  int f(int m,int n){
				  char c = 'a';
			  }
			 ��ʱ��ȫ��ָ�������
			  synbollistToFunc_g ��ֵ����û��ѹ�뵽synbollist_main_g��
			  ��������ȫ��ָ��Ϊ��
			*/

			//���Ȳ���
			checkSynbollistToFunc(synbollistToFunc_g->func, id_g);	//�Ƿ��ض���

			if (mainSheet_g == NULL)
			{
				mainSheet_g = new MainSheet;
			}
			else
			{
				showErrowWhenCreateASheet("MainSheet");
			}
			mainSheet_g->content = id_g;//������
			mainSheet_g->category = "v";//�����Ǳ���
			mainSheet_g->flag = 1;

			writeTypeSheet(type_g);
			mainSheet_g->type = typeSheet_g;
			typeSheet_g = NULL;

			writeLevelAndOffsetAndValue(((FuncSheet*)(synbollistToFunc_g->synbollist.at(0)->addr))->level, offset_g);//
			if (type_g == "int" || type_g == "float")
			{
				offset_g += 4;
			}
			else if (type_g == "char")
			{
				offset_g += 1;
			}
			else if (type_g == "double")
			{
				offset_g += 8;
			}
			else
			{
				cout << "����δ֪����" << type_g << ",�䳤�Ȱ���2���ֽڼ���" << endl;
				offset_g += 2;
			}
			//����һ��LevelAndOffset������
			mainSheet_g->addr = levelAndOffsetAndValue_g;
			levelAndOffsetAndValue_g = NULL;
			synbollistToFunc_g->synbollist.push_back(mainSheet_g);
			mainSheet_g = NULL;
		}
		else
		{//��������ȫ�ֱ���
			cout << type_g << " " << id_g << "��ȫ�ֱ���" << endl;
			/*��ʱ��Ӧ������ǣ�Ҫ��дchar c
			  char c = 'a';
			  int f(int m,int n){
				  char b = 'a';
			  }
			 ��ʱ��ȫ��ָ�������
			  ȫ��ָ���Ϊ��
			*/

			//���Ȳ���
			checkGlobal(id_g);

			if (mainSheet_g == NULL)
			{
				mainSheet_g = new MainSheet;
			}
			else
			{
				showErrowWhenCreateASheet("MainSheet");
			}
			mainSheet_g->content = id_g;
			mainSheet_g->category = "v";
			mainSheet_g->flag = 1;

			writeTypeSheet(type_g);

			mainSheet_g->type = typeSheet_g;
			typeSheet_g = NULL;

			writeLevelAndOffsetAndValue(0, global_offset_g);

			mainSheet_g->addr = levelAndOffsetAndValue_g;
			levelAndOffsetAndValue_g = NULL;
			globalsynbollistToFunc_g->synbollist.push_back(mainSheet_g);
			mainSheet_g = NULL;
			if (type_g == "int" || type_g == "float")
			{
				global_offset_g += 4;
			}
			else if (type_g == "char")
			{
				global_offset_g += 1;
			}
			else if (type_g == "double")
			{
				global_offset_g += 8;
			}
			else
			{
				cout << "����δ֪����" << type_g << ",�䳤�Ȱ���2���ֽڼ���" << endl;
				global_offset_g += 2;
			}
		}
		/*���嶯������*/
		token_g = scan.next_Token();
		if (ExpressionStatement() == 1)
		{
			ASSI();//��ֵ��Ԫʽ
			return 1;
		}
		else
		{
			Error(token_g.row, token_g.content, "������䲻��ȷ");
		}
	}
	else if (token_g.content == "(")
	{//��������   int f (

		if (id_g == "main")
		{//������
			func_name = "main";
			MAIN();
		}
		else
		{
			token in;//��ͨ��������
			in = { id_qt,2,false };
			PUSHSEM(in);
			func_name = id_qt;
			FUNC();
		}
		/*���嶯�����ֿ�ʼ*/
		if (synbollistToFunc_g == NULL)
		{
			synbollistToFunc_g = new SynbollistToFunc;//����һ��ָ��SynbollistToFunc�����ָ��
		}
		else
		{
			showErrowWhenCreateASheet("synbollistToFunc");
		}
		synbollistToFunc_g->func = id_g;//����������func��

		checkSynbollist_main(id_g);//���أ�����ض����˻��������

		if (mainSheet_g == NULL)
		{
			mainSheet_g = new MainSheet;//����һ��ָ��MainSheet�����ָ��
		}
		else
		{
			showErrowWhenCreateASheet("MainSheet");
		}
		mainSheet_g->content = id_g;						//���ʶ��������
		mainSheet_g->category = "f";						//���ʶ��������
		mainSheet_g->flag = 0;								//���ʶ���ı�־

		writeTypeSheet(type_g);//ͨ��typeSheet_g�½�һ�����ͱ�

		mainSheet_g->type = typeSheet_g;
		typeSheet_g = NULL;

		if (funcSheet_g == NULL)
		{
			funcSheet_g = new FuncSheet;//�½�һ��������
			if (type_g == "int")
			{
				funcSheet_g->value = "1";
			}
			else if (type_g == "float")
			{
				funcSheet_g->value = "1.0";
			}
			else if (type_g == "char")
			{
				funcSheet_g->value = "a";
			}
		}
		else
		{
			showErrowWhenCreateASheet("FuncSheet");
		}
		funcSheet_g->level = 1;//Ŀǰ����levelΪ1 
		if (paras_g == NULL)
		{
			paras_g = new vector<ParaSheet*>; //����һ��ָ���βα�vector��ָ��
		}
		else
		{
			showErrowWhenCreateASheet("paras_g");
		}
		fnum_g = 0;	//�����βθ���Ϊ0
		offset_g = 0;//����offsetΪ0
		/*���嶯�����ֽ���
		�ܽ᣺
			Ŀǰ
				synbollistToFunc_g��û�н�����vector
				mainSheet_g���к�����û��д
				funcSheet_g�����βθ���������������ڵ�ַûд
			������Ҫ���뵽 paraters
		*/

		/*��¼��������*/
		func_name = id_g;
		/*��¼��������*/

		token_g = scan.next_Token();
		if (Paraters() == 1) {
			if (token_g.content == ")") {		//�����βα�����

				/*���嶯����ʼ*/
				funcSheet_g->para = paras_g;	//����������д��������
				paras_g = NULL;
				funcSheet_g->fnum = fnum_g;		//����������д��������

				mainSheet_g->addr = funcSheet_g; //����������д��MainSheet_g
				funcSheet_g = NULL;
				synbollistToFunc_g->synbollist.push_back(mainSheet_g);  //��MainSheet_gѹ�뵽synbollistToFunc_g->synbollist
				mainSheet_g = NULL;

				//��������tempMainSheetpointers�е�ָ��ѹ�뵽synbollistToFunc_g->synbollist
				while (!tempMainSheetpointers.empty()) {
					synbollistToFunc_g->synbollist.push_back(tempMainSheetpointers.front());
					tempMainSheetpointers.pop();
				}
				/*���嶯������
				�ܽ᣺
					Ŀǰ��������ʶ���Ѿ���д
						  mianSheet_gΪ��
						  synbollistToFunc_g��û��ѹ����vector
				��������
					Ҫ���뺯����
				*/
				token_g = scan.next_Token();
				if (Block() == 1) {
					return 1;
				}
				else {
					Error(token_g.row, token_g.content, "������������ȷ");
				}
			}
			else {
				Error(token_g.row, token_g.content, "������������ȷ");
			}
		}
		else {
			Error(token_g.row, token_g.content, "������������ȷ");
		}
	}
	else if (token_g.content == ";")
	{// ����Ƶ���������,˵����һ���������������.��:int a;

		/*���嶯����ʼ������������ű���ʱ��*/
		if (synbollistToFunc_g != NULL) {													//˵���������Ǿֲ�����
			cout << type_g << " " << id_g << "�Ǿֲ�����" << endl;
			/*��ʱ��Ӧ������ǣ�Ҫ��дchar c
			  int f(int m,int n){
				  char c = 'a';
			  }
			 ��ʱ��ȫ��ָ�������
			  synbollistToFunc_g ��ֵ����û��ѹ�뵽synbollist_main_g��
			  ��������ȫ��ָ��Ϊ��
			*/

			//���Ȳ���
			checkSynbollistToFunc(synbollistToFunc_g->func, id_g);	//�Ƿ��ض���

			if (mainSheet_g == NULL) {
				mainSheet_g = new MainSheet;
			}
			else {
				showErrowWhenCreateASheet("MainSheet");
			}
			mainSheet_g->content = id_g;		//������
			mainSheet_g->category = "v";		//�����Ǳ���
			mainSheet_g->flag = 1;

			writeTypeSheet(type_g);
			mainSheet_g->type = typeSheet_g;
			typeSheet_g = NULL;

			writeLevelAndOffsetAndValue(((FuncSheet*)(synbollistToFunc_g->synbollist.at(0)->addr))->level, offset_g);//
			if (type_g == "int" || type_g == "float")
			{
				offset_g += 4;
			}
			else if (type_g == "char")
			{
				offset_g += 1;
			}
			else if (type_g == "double")
			{
				offset_g += 8;
			}
			else
			{
				cout << "����δ֪����" << type_g << ",�䳤�Ȱ���2���ֽڼ���" << endl;
				offset_g += 2;
			}
			//����һ��LevelAndOffset������
			mainSheet_g->addr = levelAndOffsetAndValue_g;
			levelAndOffsetAndValue_g = NULL;
			synbollistToFunc_g->synbollist.push_back(mainSheet_g);
			mainSheet_g = NULL;
		}
		else
		{//��������ȫ�ֱ���
			cout << type_g << " " << id_g << "��ȫ�ֱ���" << endl;
			/*��ʱ��Ӧ������ǣ�Ҫ��дchar c
			  char c = 'a';
			  int f(int m,int n){
				  char b = 'a';
			  }
			 ��ʱ��ȫ��ָ�������
			  ȫ��ָ���Ϊ��
			*/

			//���Ȳ���
			checkGlobal(id_g);

			if (mainSheet_g == NULL)
			{
				mainSheet_g = new MainSheet;
			}
			else
			{
				showErrowWhenCreateASheet("MainSheet");
			}
			mainSheet_g->content = id_g;
			mainSheet_g->category = "v";
			mainSheet_g->flag = 1;

			writeTypeSheet(type_g);

			mainSheet_g->type = typeSheet_g;
			typeSheet_g = NULL;

			writeLevelAndOffsetAndValue(0, global_offset_g);

			mainSheet_g->addr = levelAndOffsetAndValue_g;
			levelAndOffsetAndValue_g = NULL;
			globalsynbollistToFunc_g->synbollist.push_back(mainSheet_g);
			mainSheet_g = NULL;
			if (type_g == "int" || type_g == "float")
			{
				global_offset_g += 4;
			}
			else if (type_g == "char")
			{
				global_offset_g += 1;
			}
			else if (type_g == "double")
			{
				global_offset_g += 8;
			}
			else
			{
				cout << "����δ֪����" << type_g << ",�䳤�Ȱ���2���ֽڼ���" << endl;
				global_offset_g += 2;
			}
		}
		/*���嶯������*/
		token_g = scan.next_Token();
		return 1;
	}
	else {
		return 0;
	}
}

int EqualStringOrNumOrNull()
{//����������ʼ��or����ʼ��
	if (token_g.content == "=")
	{
		token_g = scan.next_Token();
		if (StringOrNum() == 1)
			return 1;
		else
			Error(token_g.row, token_g.content, "��Ӧ�����ַ���������ĵط�����ȷ");
	}
	else
		return 1;
}

int StringOrNum()
{//�����ʼ��
	if (token_g.content == "{")
	{
		token_g = scan.next_Token();
		if (token_g.type == 'n' || token_g.type == 'f')
		{//���ͻ򸡵��ͳ���
			if (token_g.type == 'n')
			{//����
				type_g = "int";
			}
			else
			{
				type_g = "float";
			}
			if (synbollistToFunc_g != NULL)
			{//��������
				if (!checkSynbollistToFuncNum(token_g.content))
				{
					if (mainSheet_g == NULL)
					{
						mainSheet_g = new MainSheet;
					}
					else
					{
						showErrowWhenCreateASheet("MainSheet");
					}
					if (type_g == "int")
					{
						mainSheet_g->content = to_string(stringToNum<int>(token_g.content));
					}
					else
					{
						mainSheet_g->content = token_g.content;		//������
					}
					mainSheet_g->category = "num";		//�����Ǳ���
					mainSheet_g->flag = 1;

					writeTypeSheet(type_g);
					mainSheet_g->type = typeSheet_g;
					typeSheet_g = NULL;

					writeLevelAndOffsetAndValue(((FuncSheet*)(synbollistToFunc_g->synbollist.at(0)->addr))->level, offset_g);
					levelAndOffsetAndValue_g->value = token_g.content;//����¼���ֵ
					mainSheet_g->addr = levelAndOffsetAndValue_g;
					levelAndOffsetAndValue_g = NULL;
					synbollistToFunc_g->synbollist.push_back(mainSheet_g);
					mainSheet_g = NULL;
				}
			}
			else
			{//ȫ��
				if (!checkGlobalNum(token_g.content))
				{
					if (mainSheet_g == NULL)
					{
						mainSheet_g = new MainSheet;
					}
					else
					{
						showErrowWhenCreateASheet("MainSheet");
					}
					if (type_g == "int")
					{
						mainSheet_g->content = to_string(stringToNum<int>(token_g.content));
					}
					else
					{
						mainSheet_g->content = token_g.content;		//������
					}
					mainSheet_g->category = "num";
					mainSheet_g->flag = 1;

					writeTypeSheet(type_g);

					mainSheet_g->type = typeSheet_g;
					typeSheet_g = NULL;

					writeLevelAndOffsetAndValue(0, global_offset_g);
					levelAndOffsetAndValue_g->value = token_g.content;
					mainSheet_g->addr = levelAndOffsetAndValue_g;
					levelAndOffsetAndValue_g = NULL;
					globalsynbollistToFunc_g->synbollist.push_back(mainSheet_g);
					mainSheet_g = NULL;
				}
			}

			/*����������Ԫʽ*/
			token in;
			in = { id_qt + "[" + to_string(arr_num_qt++) + "]",2,false };
			PUSHSEM(in);
			in = { token_g.content,1,false };
			PUSHSEM(in);
			ASSI();
			/*����������Ԫʽ*/

			token_g = scan.next_Token();
			while (token_g.content == ",")
			{
				token_g = scan.next_Token();
				if (token_g.type == 'n' || token_g.type == 'f')
				{
					if (token_g.type == 'n')
					{
						type_g = "int";
					}
					else
					{
						type_g = "float";
					}
					if (synbollistToFunc_g != NULL)
					{
						if (!checkSynbollistToFuncNum(token_g.content))
						{
							if (mainSheet_g == NULL)
							{
								mainSheet_g = new MainSheet;
							}
							else
							{
								showErrowWhenCreateASheet("MainSheet");
							}
							if (type_g == "int")
							{
								mainSheet_g->content = to_string(stringToNum<int>(token_g.content));
							}
							else
							{
								mainSheet_g->content = token_g.content;//������
							}
							mainSheet_g->category = "num";//�����Ǳ���
							mainSheet_g->flag = 1;

							writeTypeSheet(type_g);
							mainSheet_g->type = typeSheet_g;
							typeSheet_g = NULL;

							writeLevelAndOffsetAndValue(((FuncSheet*)(synbollistToFunc_g->synbollist.at(0)->addr))->level, offset_g);//
							levelAndOffsetAndValue_g->value = token_g.content;
							mainSheet_g->addr = levelAndOffsetAndValue_g;
							levelAndOffsetAndValue_g = NULL;
							synbollistToFunc_g->synbollist.push_back(mainSheet_g);
							mainSheet_g = NULL;
						}
					}
					else {
						if (!checkGlobalNum(token_g.content)) {
							if (mainSheet_g == NULL) {
								mainSheet_g = new MainSheet;
							}
							else {
								showErrowWhenCreateASheet("MainSheet");
							}
							if (type_g == "int") {
								mainSheet_g->content = to_string(stringToNum<int>(token_g.content));
							}
							else {
								mainSheet_g->content = token_g.content;		//������
							}
							mainSheet_g->category = "num";
							mainSheet_g->flag = 1;

							writeTypeSheet(type_g);

							mainSheet_g->type = typeSheet_g;
							typeSheet_g = NULL;

							writeLevelAndOffsetAndValue(0, global_offset_g);
							levelAndOffsetAndValue_g->value = token_g.content;
							mainSheet_g->addr = levelAndOffsetAndValue_g;
							levelAndOffsetAndValue_g = NULL;
							globalsynbollistToFunc_g->synbollist.push_back(mainSheet_g);
							mainSheet_g = NULL;
						}
					}


					/*����������Ԫʽ*/
					token in;
					in = { id_qt + "[" + to_string(arr_num_qt++) + "]",2 ,false };
					PUSHSEM(in);
					in = { token_g.content,1 ,false };
					PUSHSEM(in);
					ASSI();
					/*����������Ԫʽ*/

					token_g = scan.next_Token();
				}
				else
				{
					Error(token_g.row, token_g.content, "���������鶨�岻��ȷ");
				}
			}
			if (token_g.content == "}")
			{
				arr_num_qt = 0;//����λ�ù���
				token_g = scan.next_Token();
				return 1;
			}
			else
			{
				Error(token_g.row, token_g.content, "���������鶨�岻��ȷ");
			}
		}
		else
		{
			Error(token_g.row, token_g.content, "���������鶨�岻��ȷ");
		}
	}
	/*else if (token_g.type == 's')
	{//�ַ���
		int arr_length = token_g.content.length();
		for (int arr_i = 1; arr_i < arr_length - 1; arr_i++)
		{
			token in;
			in = { id_qt + "[" + to_string(arr_num_qt++) + "]", 2 ,false };
			PUSHSEM(in);
			in = { token_g.content.substr(arr_i, 1) ,3 ,false };
			PUSHSEM(in);
			ASSI();
		}
		token_g = scan.next_Token();
		arr_num_qt = 0;//����λ�ù���
		return 1;
	}*/
	else
	{
		return 0;
	}
}

int Statement()
{
	if (token_g.content == "printf")
	{	// statement --> printf'('expression')'';' ��ʼ
		token_g = scan.next_Token();						//������һ��token
		if (token_g.content == "(")
		{
			token_g = scan.next_Token();					//������һ��token
			if (Expression() == 1)
			{
				if (token_g.content == ")")
				{
					token_g = scan.next_Token();			//������һ��token
					if (token_g.content == ";")
					{
						token_g = scan.next_Token();		//������һ��token
						return 1;
					}
					else
						Error(token_g.row, token_g.content, "��Ӧ�ö���';'�ĵط�����");
				}
				else
					Error(token_g.row, token_g.content, "��Ӧ�ö���')'�ĵط�����");
			}
			else
				Error(token_g.row, token_g.content, "printf������ʽ����");
		}
		else
			Error(token_g.row, token_g.content, "��Ӧ�ö���'('�ĵط�����");//�У� ���ݣ�����
	}		// statement --> printf'('expression')'';'����
	else if (Block() == 1)
	{	// �������statement --> block
		return 1;
	}
	else if (token_g.content == "if")
	{	// statement --> IF '(' experssion ')' statement ElSE statement
		token_g = scan.next_Token();
		if (token_g.content == "(")
		{
			token_g = scan.next_Token();
			if (Expression() == 1)
			{
				if (token_g.content == ")")
				{
					IF();/*if�����Ԫʽ���ɣ������if   ��һ�������Ϊջ��Ԫ��SEM[sem],�ڶ��������ͽ��Ϊ��
		 ջ��Ԫ�ص�ջ*/
					token_g = scan.next_Token();
					if (Statement() == 1)
					{
						if (token_g.content == "else")
						{
							EL();/*else�����Ԫʽ���ɣ������el ����Ϊ�� SEM�޲���*/
							token_g = scan.next_Token();
							if (Statement() == 1)
							{
								IE();/*������Ԫʽ�������ie ����Ϊ�� SEM�޲���*/
								return 1;
							}
							else
								Error(token_g.row, token_g.content, "else�����");
						}
						else
							Error(token_g.row, token_g.content, "if ���û��else");
					}
					else
						Error(token_g.row, token_g.content, "if�����");
				}
				else
					Error(token_g.row, token_g.content, "��Ӧ�ö���')'�ĵط�����");
			}
			else
				Error(token_g.row, token_g.content, "if������ʽ����");
		}
		else
			Error(token_g.row, token_g.content, "��Ӧ�ö���'('�ĵط�����");
	}  // statement --> IF '(' experssion ')' statement ElSE statement ����
	else if (token_g.content == "while")
	{  // statement --> WHILE '(' expression ')' statement
		WH();/*whileѭ��ͷ������wh ����Ϊ�� SEM�޲���*/
		token_g = scan.next_Token();
		if (token_g.content == "(")
		{
			token_g = scan.next_Token();
			if (Expression() == 1)
			{
				if (token_g.content == ")")
				{
					DO();/*do������do ��һ������Ϊջ��Ԫ�� �ڶ�Ϊ�� ���Ϊ��
			 ջ��Ԫ�ص���*/
					token_g = scan.next_Token();
					if (Statement() == 1)
					{
						WE();/*ѭ��β��we ������ SEM�޲���*/
						return 1;
					}
					else
						Error(token_g.row, token_g.content, "while�����");
				}
				else
					Error(token_g.row, token_g.content, "��Ӧ�ö���')'�ĵط�����");
			}
			else
				Error(token_g.row, token_g.content, "while������ʽ����");
		}
		else
			Error(token_g.row, token_g.content, "��Ӧ�ö���'('�ĵط�����");
	}  // statement --> WHILE '(' expression ')' statement ����
	else if (ExpressionStatement() == 1)
	{
		return 1;
	}
	else if (token_g.content == "return")
	{
		token_g = scan.next_Token();
		if (ExpressionStatement() == 1)
		{
			RET();
		}
		else
			Error(token_g.row, token_g.content, "return������");
	}
	return 0;
}

int Block()
{//������һ������
	if (token_g.content == "{")
	{
		token_g = scan.next_Token();
		if (ZeroOrMoreDeclation() == 1)
		{
			if (token_g.content == "}")
			{
				/*���嶯����ʼ*/
				if (synbollistToFunc_g != NULL) {
					synbollistToFunc_g->offset_max = offset_g;			//�ж��Ƿ�����һ��SynbollistToFunc����										 
					synbollist_main_g.push_back(synbollistToFunc_g);     //��������ˣ���synbollistToFunc_gָ��ѹ������ܱ�
					synbollistToFunc_g = NULL;								 //��synbollistToFunc_g��Ϊ��
				}
				/*���嶯������
				�ܽ᣺
					Ŀǰ��synbollistToFunc_gΪ��
				*/
				token_g = scan.next_Token();
				return 1;
			}
			else
			{
				Error(token_g.row, token_g.content, "Ӧ���������ŵĵط�����");
			}
		}
		else
		{
			Error(token_g.row, token_g.content, "����");
		}
	}
	else
	{
		return 0;
	}
}

int ExpressionStatement()
{//Ϊ���鸳ֵ
	if (Expression() == 1)
	{
		if (token_g.content == ";")
		{
			token_g = scan.next_Token();
			return 1;
		}
		else
		{
			Error(token_g.row, token_g.content, "ȱ�ٷֺ�");
		}
	}
	else
		return 0;
}

int Expression()
{
	if (Assignment() == 1)
	{
		if (Assignment_1() == 1)
			return 1;
		else
			Error(token_g.row, token_g.content, "��ֵ������");
	}
	else
		return 0;
}



//��ֵ���
int Assignment()
{
	if (Logical_Expression() == 1)
	{
		if (Other_Logical() == 1)
			return 1;
		else
			Error(token_g.row, token_g.content, "��ֵ������");
	}
	else
		return 0;
}

int Assignment_1()
{
	if (token_g.content == "=")
	{
		token_g = scan.next_Token();
		if (Assignment() == 1)
		{
			ASSI();//��ֵ��Ԫʽ����ʽ
			return 1;
		}
		else
			Error(token_g.row, token_g.content, "��ֵ������");
	}
	else
		return 1;
}

int Logical_Expression()
{
	if (Equality() == 1)
	{
		if (Equality_2() == 1)
			return 1;
		else
			Error(token_g.row, token_g.content, "�߼�����ʽ����");
	}
	else
		return 0;
}

int Other_Logical()
{
	if (token_g.content == "&&")
	{
		token_g = scan.next_Token();
		if (Logical_Expression() == 1)
		{
			GEQ("&&");//��Ԫʽ����ʽ
			if (Other_Logical() == 1)
				return 1;
			else
				Error(token_g.row, token_g.content, "�߼�����ʽ����");
		}
		else
			Error(token_g.row, token_g.content, "�߼�����ʽ����");
	}
	if (token_g.content == "||")
	{
		token_g = scan.next_Token();
		if (Logical_Expression() == 1)
		{
			GEQ("||");//��Ԫʽ����ʽ
			if (Other_Logical() == 1)
				return 1;
			else
				Error(token_g.row, token_g.content, "�߼�����ʽ����");
		}
		else
			Error(token_g.row, token_g.content, "�߼�����ʽ����");
	}
	else
		return 1;
}

int Equality()
{
	if (Arithmetic_Expression() == 1)
	{
		if (Arithmetic_Expression_2() == 1)
			return 1;
		else
			Error(token_g.row, token_g.content, "�߼�����ʽ����");
	}
	else
		return 0;
}

int Equality_2()
{
	if (token_g.content == "!=")
	{
		token_g = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ("!=");//��Ԫʽ����ʽ
			return 1;
		}
		else
			Error(token_g.row, token_g.content, "�߼�����ʽ����");
	}
	if (token_g.content == "==")
	{
		token_g = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ("==");//��Ԫʽ����ʽ
			return 1;
		}
		else
			Error(token_g.row, token_g.content, "�߼�����ʽ����");
	}
	else
		return 1;
}
int Arithmetic_Expression()
{
	if (MD_Expression() == 1)
	{
		if (Arithmetic_Expression_1() == 1)
			return 1;
		else
			Error(token_g.row, token_g.content, "����");
	}
	else
		return 0;
}

//�߼�����ʽ
int Arithmetic_Expression_2()
{
	if (token_g.content == ">")
	{
		token_g = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ(">");//��Ԫʽ����ʽ
			return 1;
		}
		else
			Error(token_g.row, token_g.content, "�߼��Ƚϱ���ʽ����");
	}
	else if (token_g.content == ">=")
	{
		token_g = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ(">=");//��Ԫʽ����ʽ
			return 1;
		}
		else
			Error(token_g.row, token_g.content, "�߼��Ƚϱ���ʽ����");
	}
	else if (token_g.content == "<")
	{
		token_g = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ("<");//��Ԫʽ����ʽ
			return 1;
		}
		else
			Error(token_g.row, token_g.content, "�߼��Ƚϱ���ʽ����");
	}
	else if (token_g.content == "<=")
	{
		token_g = scan.next_Token();
		if (Equality() == 1)
		{
			GEQ("<=");//��Ԫʽ����ʽ
			return 1;
		}
		else
			Error(token_g.row, token_g.content, "�߼��Ƚϱ���ʽ����");
	}
	else
		return 1;
}

int MD_Expression()
{
	if (Factor() == 1)
	{
		if (MD_Expression_1() == 1)
			return 1;
		else
			Error(token_g.row, token_g.content, "�Ӽ�������ʽ����");
	}
	else
		return 0;
}

int Arithmetic_Expression_1()
{
	if (token_g.content == "+")
	{
		token_g = scan.next_Token();
		if (MD_Expression() == 1)
		{
			GEQ("+");//��Ԫʽ���ɺ���
			if (Arithmetic_Expression_1() == 1)
				return 1;
			else
				Error(token_g.row, token_g.content, "�ӷ�����ʽ����");
		}
		else
			Error(token_g.row, token_g.content, "�Ӽ�������ʽ����");
	}
	if (token_g.content == "-")
	{
		token_g = scan.next_Token();
		if (MD_Expression() == 1)
		{
			GEQ("-");//��Ԫʽ���ɺ���
			if (Arithmetic_Expression_1() == 1)
				return 1;
			else
				Error(token_g.row, token_g.content, "��������ʽ����");
		}
		else
			Error(token_g.row, token_g.content, "�Ӽ�������ʽ����");
	}
	else//��
		return 1;
}


int MD_Expression_1()
{
	if (token_g.content == "*")
	{
		token_g = scan.next_Token();
		if (Factor() == 1)
		{
			GEQ("*");//��Ԫʽ���ɺ���
			if (MD_Expression_1() == 1)
				return 1;
			else
				Error(token_g.row, token_g.content, "�˷�����ʽ����");
		}
		else
			Error(token_g.row, token_g.content, "�˳�������ʽ����");
	}
	if (token_g.content == "/")
	{
		token_g = scan.next_Token();
		if (Factor() == 1)
		{
			GEQ("/");//��Ԫʽ���ɺ���
			if (MD_Expression_1() == 1)
				return 1;
			else
				Error(token_g.row, token_g.content, "��������ʽ����");
		}
		else
			Error(token_g.row, token_g.content, "�˳�������ʽ����");
	}
	else
		return 1;
}

int Factor()
{//���ӣ�����ʽ�����
	if (token_g.content == "(")
	{
		token_g = scan.next_Token();
		if (Expression() == 1)
		{
			if (token_g.content == ")")
			{
				token_g = scan.next_Token();
				return 1;
			}
			else
				Error(token_g.row, token_g.content, "ȱ��������");
		}
		else
			Error(token_g.row, token_g.content, "���ű���ʽ����");
	}
	else if (Primary() == 1)
		return 1;
	else
		return 0;
}
int Primary()
{//��ʼ����������Ҫ������
	if (token_g.type == 'n' || token_g.type == 'f')
	{//���ͻ��ַ��ͳ���
		if (token_g.type == 'n')
		{
			type_g = "int";
		}
		else
		{
			type_g = "float";
		}
		if (synbollistToFunc_g != NULL)
		{//��ȫ��
			if (!checkSynbollistToFuncNum(token_g.content))
			{
				if (mainSheet_g == NULL)
				{
					mainSheet_g = new MainSheet;
				}
				else
				{
					showErrowWhenCreateASheet("MainSheet");
				}
				if (type_g == "int")
				{
					mainSheet_g->content = to_string(stringToNum<int>(token_g.content));
				}
				else {
					mainSheet_g->content = token_g.content;//������
				}
				mainSheet_g->category = "num";//�����Ǳ���
				mainSheet_g->flag = 1;

				writeTypeSheet(type_g);
				mainSheet_g->type = typeSheet_g;
				typeSheet_g = NULL;

				writeLevelAndOffsetAndValue(((FuncSheet*)(synbollistToFunc_g->synbollist.at(0)->addr))->level, offset_g);
				levelAndOffsetAndValue_g->value = token_g.content;
				mainSheet_g->addr = levelAndOffsetAndValue_g;
				levelAndOffsetAndValue_g = NULL;
				synbollistToFunc_g->synbollist.push_back(mainSheet_g);
				mainSheet_g = NULL;
			}
		}
		else
		{
			if (!checkGlobalNum(token_g.content))
			{
				if (mainSheet_g == NULL)
				{
					mainSheet_g = new MainSheet;
				}
				else
				{
					showErrowWhenCreateASheet("MainSheet");
				}
				if (type_g == "int")
				{
					mainSheet_g->content = to_string(stringToNum<int>(token_g.content));
				}
				else {
					mainSheet_g->content = token_g.content;
				}
				mainSheet_g->category = "num";
				mainSheet_g->flag = 1;

				writeTypeSheet(type_g);

				mainSheet_g->type = typeSheet_g;
				typeSheet_g = NULL;

				writeLevelAndOffsetAndValue(0, global_offset_g);
				levelAndOffsetAndValue_g->value = token_g.content;
				mainSheet_g->addr = levelAndOffsetAndValue_g;
				levelAndOffsetAndValue_g = NULL;
				globalsynbollistToFunc_g->synbollist.push_back(mainSheet_g);
				mainSheet_g = NULL;
			}
		}

		/***************************************/
		token in;
		in = { token_g.content,1,false };
		PUSHSEM(in);//��������ջ
		/***************************************/
		token_g = scan.next_Token();
		return 1;

	}
	else if (token_g.type == 'c')
	{
		/***************************************/
		token in;
		in = { token_g.content,1,false };
		PUSHSEM(in);//��������ջ
		/***************************************/
		token_g = scan.next_Token();
		return 1;
	}
	else if (ID() == 1)
	{
		if (ArrFunc() == 1)
		{
			return 1;
		}
		else
		{
			Error(token_g.row, token_g.content, "����ȷ");
		}
	}
	else
	{
		return 0;
	}
}

//****************************************************************************************


int Paraters() {
	if (Variable_Type() == 1) {
		if (ID() == 1) {

			token in;//��������
			in = { id_g + func_name ,2,false };
			PUSHSEM(in);
			PARA_n();
			/*���嶯����ʼ*/
			fnum_g++;								//�βθ�����1

			checkParas(id_g);		//���βα��в��أ�����ض����ˣ���������


			MainSheet* tempMainSheet = new MainSheet;		//����һ��ָ��MainSheet��ָ��
			tempMainSheet->flag = 1;

			if (para_g == NULL) {
				para_g = new ParaSheet;					//����һ��ָ���βα���ָ��
			}
			else {
				showErrowWhenCreateASheet("para_g");
			}
			para_g->content = id_g;
			tempMainSheet->content = id_g;

			tempMainSheet->category = "vn";

			writeTypeSheet(type_g);					   //ʹ��typeSheet_g����һ���µ����ͱ�
			TypeSheet* tempTypeSheet = new TypeSheet;

			tempTypeSheet->typevalue = type_g;
			tempTypeSheet->tpoint = NULL;

			para_g->type = typeSheet_g;
			tempMainSheet->type = tempTypeSheet;
			;
			typeSheet_g = NULL;
			tempTypeSheet = NULL;


			writeLevelAndOffsetAndValue(funcSheet_g->level, offset_g);//
			if (type_g == "int") {
				levelAndOffsetAndValue_g->value = "1";
			}
			else if (type_g == "char") {
				levelAndOffsetAndValue_g->value = "a";
			}
			else if (type_g == "float" || type_g == "double") {
				levelAndOffsetAndValue_g->value = "1.0";
			}

			LevelAndOffsetAndValue* tempLevelAndOffsetAndValue = new LevelAndOffsetAndValue;

			tempLevelAndOffsetAndValue->level = funcSheet_g->level;
			tempLevelAndOffsetAndValue->offset = offset_g;
			if (type_g == "int") {
				tempLevelAndOffsetAndValue->value = "1";
			}
			else if (type_g == "char") {
				tempLevelAndOffsetAndValue->value = "a";
			}
			else if (type_g == "float" || type_g == "double") {
				tempLevelAndOffsetAndValue->value = "1.0";
			}

			if (type_g == "int" || type_g == "float") {
				offset_g += 4;
			}
			else if (type_g == "char") {
				offset_g += 1;
			}
			else if (type_g == "double") {
				offset_g += 8;
			}
			else {
				offset_g += 2;
			}											//����һ��LevelAndOffset������

			para_g->addr = levelAndOffsetAndValue_g;
			tempMainSheet->addr = tempLevelAndOffsetAndValue;

			levelAndOffsetAndValue_g = NULL;
			tempLevelAndOffsetAndValue = NULL;
			paras_g->push_back(para_g);					//��һ���βα�ָ��ѹ��paras_g��
			para_g = NULL;
			//tempMainSheet�����У����Ժ���д�����ű�
			tempMainSheetpointers.push(tempMainSheet);
			/*���嶯������
			�ܽ᣺
				Ŀǰ��synbollistToFunc_g��û�н�����vector
				mainSheet_g���к�����û��д
				funcSheet_g�����βθ���������������ڵ�ַûд
			��������
				Ҫ����)
			*/

			if (Other_Type_ID() == 1) {
				return 1;
			}
			else {
				Error(token_g.row, token_g.content, "����");
			}
		}
		else {
			Error(token_g.row, token_g.content, "Ӧ���Ǳ�ʶ���ĵط����Ǳ�ʶ��");
		}
	}
	else {
		return 1;
	}
}

int Other_Type_ID() {
	if (token_g.content == ",") {
		token_g = scan.next_Token();
		if (Variable_Type() == 1) {
			if (ID() == 1) {							//����ű��������β�
				token in;//��ͨ��������
			in = { id_g + func_name ,2,false };
			PUSHSEM(in);
				PARA_n();

				/*���嶯����ʼ*/
				fnum_g++;								//�βθ�����1

				checkParas(id_g);		//���βα��в��أ�����ض����ˣ���������


				MainSheet* tempMainSheet = new MainSheet;		//����һ��ָ��MainSheet��ָ��
				tempMainSheet->flag = 1;

				if (para_g == NULL) {
					para_g = new ParaSheet;					//����һ��ָ���βα���ָ��
				}
				else {
					showErrowWhenCreateASheet("para_g");
				}
				para_g->content = id_g;
				tempMainSheet->content = id_g;

				tempMainSheet->category = "vn";

				writeTypeSheet(type_g);					   //ʹ��typeSheet_g����һ���µ����ͱ�
				TypeSheet* tempTypeSheet = new TypeSheet;

				tempTypeSheet->typevalue = type_g;
				tempTypeSheet->tpoint = NULL;

				para_g->type = typeSheet_g;
				tempMainSheet->type = tempTypeSheet;
				;
				typeSheet_g = NULL;
				tempTypeSheet = NULL;



				writeLevelAndOffsetAndValue(funcSheet_g->level, offset_g);//
				if (type_g == "int") {
					levelAndOffsetAndValue_g->value = "1";
				}
				else if (type_g == "char") {
					levelAndOffsetAndValue_g->value = "a";
				}
				else if (type_g == "float" || type_g == "double") {
					levelAndOffsetAndValue_g->value = "1.0";
				}

				LevelAndOffsetAndValue* tempLevelAndOffsetAndValue = new LevelAndOffsetAndValue;

				tempLevelAndOffsetAndValue->level = funcSheet_g->level;
				tempLevelAndOffsetAndValue->offset = offset_g;
				if (type_g == "int") {
					tempLevelAndOffsetAndValue->value = "1";
				}
				else if (type_g == "char") {
					tempLevelAndOffsetAndValue->value = "a";
				}
				else if (type_g == "float" || type_g == "double") {
					tempLevelAndOffsetAndValue->value = "1.0";
				}

				if (type_g == "int" || type_g == "float") {
					offset_g += 4;
				}
				else if (type_g == "char") {
					offset_g += 1;
				}
				else if (type_g == "double") {
					offset_g += 8;
				}
				else {
					cout << "����δ֪����" << type_g << ",�䳤�Ȱ���2���ֽڼ���" << endl;
					offset_g += 2;
				}											//����һ��LevelAndOffset������

				para_g->addr = levelAndOffsetAndValue_g;
				tempMainSheet->addr = tempLevelAndOffsetAndValue;

				levelAndOffsetAndValue_g = NULL;
				tempLevelAndOffsetAndValue = NULL;
				paras_g->push_back(para_g);					//��һ���βα�ָ��ѹ��paras_g��
				para_g = NULL;
				//tempMainSheet�����У����Ժ���д�����ű�
				tempMainSheetpointers.push(tempMainSheet);
				/*���嶯������*/


				if (Other_Type_ID() == 1) {
					return 1;
				}
				else {
					Error(token_g.row, token_g.content, "����");
				}
			}
			else {
				Error(token_g.row, token_g.content, "Ӧ�ó��ֱ�ʶ���ĵط�����");
			}
		}
		else {
			Error(token_g.row, token_g.content, "����");
		}
	}
	else {
		return 1;
	}
}



int ArrFunc() {
	if (token_g.content == "(") {

		func_call = id_qt;
		token_g = scan.next_Token();
		if (Realparaters() == 1) {
			if (token_g.content == ")") {

				token in;
				in = { func_call,2,false };
				PUSHSEM(in);
				CALL();
				if (parapos != NumOfPara(func_call)) {
					Error(token_g.row, token_g.content, "����������ƥ��");
				}
				parapos = 0;
				func_call = "no";
				token_g = scan.next_Token();
				return 1;
			}
			else {
				Error(token_g.row, token_g.content, "��Ӧ�ö���')'�ĵط�����");
			}
		}
		else {
			Error(token_g.row, token_g.content, "��������������");
		}
	}
	else if (token_g.content == "[") {
		token_g = scan.next_Token();
		if (token_g.type == 'n') {

			int arrIndex = stringToNum<int>(token_g.content);

			num_qt = token_g.content;
			token_g = scan.next_Token();
			if (token_g.content == "]") {

				token in;
				in = { id_qt + "[" + to_string(arrIndex) + "]",2,false };
				PUSHSEM(in);

				token_g = scan.next_Token();
				return 1;
			}
			else {
				Error(token_g.row, token_g.content, "������ʸ�ʽ����ȷ");
			}
		}
		else {
			Error(token_g.row, token_g.content, "������ʸ�ʽ����ȷ");
		}
	}
	else {
		token in;
		in = { id_qt,2,false };
		PUSHSEM(in);
		return 1;
	}
}

int Realparaters() {
	if (Expression() == 1) {
		parapos++;
		PARA();
		if (Realparaters_2() == 1) {
			return 1;
		}
		else {
			Error(token_g.row, token_g.content, "������������");
		}
	}
	else {
		return 0;
	}
}

int Realparaters_2() {
	if (token_g.content == ",") {
		token_g = scan.next_Token();
		if (Realparaters_3() == 1) {
			return 1;
		}
		else {
			Error(token_g.row, token_g.content, "����");
		}
	}
	else {
		return 1;
	}
}

int Realparaters_3() {
	if (Expression() == 1) {
		parapos++;
		PARA();
		if (Realparaters_2() == 1) {
			return 1;
		}
		else {
			Error(token_g.row, token_g.content, "����");
		}
	}
	else {
		return 0;
	}
}