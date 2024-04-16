#include<iostream>
#include<graphics.h>
#include<math.h>
#include<cstdlib>
#include<ctime>
#include<thread>
#include<Windows.h>

using namespace std;

#define width 1080
#define height 720
#define IMGS_QUANTITY 5
#define PI 3.1415926535
#define Liquid_Quantity 3
constexpr int Mine_Quantity = (const int)18;

//��ص�����ṹ��
typedef struct mine_location{
	int x;
	int y;
	int size;
	struct mine_location * next;
}Mine_Location;

//���ر���
void putbackgraound()
{
	setfillcolor(YELLOW);
	setlinecolor(YELLOW);
	fillrectangle(0, 0, width, 120);
	static bool flag = true;
	static IMAGE backgroundimage;
	if (flag == true)
	{
		loadimage(&backgroundimage, "./file/images/level-background-0.jpg", width, height / 16 * 14);
		flag = false;
	};
	putimage(0, 120, &backgroundimage);
}

class Mine
{
	//��Ԫ��
	friend class Hook;

public:
	Mine();
	~Mine();
	//�����෽��
	void M_loadimage();
	void M_Putimage(Mine *mine);
	static int Value_Sum;
	static int getValeSum();
	static void putValueSum();
	void M_Putimages(Mine* mine, int M_quantity);

private:
	//��������ͼ������ͼ
	IMAGE img1,img2;
	//���꼰���С
	int x;
	int y;
	int size;
	//��ļ�ֵ
	int value;
	//��Ĵ�����
	bool exist;

};
//Mine��ľ�̬�������г�ʼ����
int Mine::Value_Sum = 0;

int Mine::getValeSum()
{
	return Mine::Value_Sum;
}

void Mine::putValueSum()
{
	setbkmode(TRANSPARENT);
	settextcolor(BLUE);
	settextstyle(width / 12, height / 18, _T("����"));
	static TCHAR ValueSum[50];
	sprintf_s(ValueSum, _T("�÷֣�%d"), Mine::Value_Sum);
	outtextxy(0, 0, ValueSum);
}

void Mine::M_Putimages(Mine* mine, int M_quantity)
{
	for (int i = 0; i < M_quantity; i++)
	{
		(mine + i)->M_Putimage(mine + i);
	};
}

//���·�����Ƭ
void Mine::M_Putimage(Mine* mine)
{
	if (mine->exist == true)//�������򲻷���
	{
		if (mine->y <= 165 && (mine->x>=width/16*6&&mine->x<=width/16*10))
		{
			mine->exist = false;
			Mine::Value_Sum = Mine::Value_Sum + mine->value;
		}
		else 
		{
			putimage(mine->x, mine->y, &(mine->img1), SRCPAINT);
			putimage(mine->x, mine->y, &(mine->img2), SRCAND);
		};
	};
};


void Mine::M_loadimage()
{
	//������Ƭ
	cout <<"M_loadimage:" << "size is " << size << endl;
	if (Mine::value <=1000)//�ϵͼ�ֵ���ؽ��
	{
		loadimage(&img1, "./file/images/small_gold_mask.bmp", size, size);
		loadimage(&img2, "./file/images/small_gold.bmp", size, size);
		cout << "M_loadimage:" << "size is " << size << endl;
	}
	else//�ϸ߼�ֵ������ʯ
	{
		loadimage(&img1, "./file/images/diamond_mask.png", size/4, size/4);
		loadimage(&img2, "./file/images/diamond.png", size/4, size/4);
		size = size / 4;
		cout << "M_loadimage:" << "size is " << size << endl;
	}
}

Mine::Mine()
{
	//���캯����ʹ��������֤��ز�����
	static Mine_Location* h = nullptr;
	static Mine_Location* r = h;
	Mine_Location* p = new Mine_Location;
	if (h == nullptr)
	{
		p->x = width / 256 * (rand() % (256 + 1));
		p->y = height / 256 * (rand() % (256 - 16 * 3 + 1)) + height / 16 * 4;
		p->size = width / 128 * (rand() % (16 + 1)) + width / 32;
		p->next = nullptr;
		h = p;
		r = p;
		x = p->x;
		y = p->y;
		size = p->size;
	}
	else
	{
		int count = 0;
		//goto��ǵ�
	create_xysize:
		Mine_Location* s = h;
		p->x = width / 256 * (rand() % (256 + 1));
		p->y = height / 256 * (rand() % (256 - 16 * 3 + 1)) + height / 16 * 4;
		p->size = width / 128 * (rand() % (16 + 1)) + width / 32;
		p->next = nullptr;
		while (s->next != nullptr)
		{
			cout << "New linked list element is generating, "<<"count is "<<++count << endl;
			//�����Ƿ�ȡ�õ��ж�������Ϊ��֤��ز�����
			if (((p->x + p->size) >= s->x && (p->x <= (s->x + s->size))) && (((p->y + p->size) >= s->y && p->y <= (s->y + s->size))))
			{
				cout << "Build failed, start regenerate" << endl;
				goto create_xysize;//���½��и�ֵ���Ƚ�
			};
			if (s != nullptr)s = s->next;
		}
		r->next = p;
		r = p;
		x = p->x;
		y = p->y;
		size = p->size;
	};
	//����̨��ӡ��Ϣ����Ӱ����Ϸ������Ҳ�ز�������̨
	cout << "X is " << x << "," << "Y is " << y << "," << "Its size is " << size << endl;
	//�����������
	static int Mine_count = 0;
	Mine_count += 1;
	cout << "Mine_count is " << Mine_count << endl;
	//��ļ�ֵ����
	value = size * 10;
	//��Ĵ����Ը���
	exist = true;
	Mine::M_loadimage();
	//BeginBatchDraw();
	putimage(x, y, &img1, SRCPAINT);
	putimage(x, y, &img2, SRCAND);
	//EndBatchDraw();
	cout << "Object has been created" << endl;
}

Mine::~Mine()
{
	//���������ӹؿ�ʱʵ����������
	cout << "Object has been deleted" << endl;
}


//���ӷ���
enum Hook_Direction
{
	left,
	right
};

//����״̬
enum Hook_State
{
	normal,
	extending,
	shortening
};

//������
class Hook
{
	//��ԪMine����
public: friend class Mine;
public:
	Hook();
	~Hook();
	//�����෽��
	void H_Round(Hook* hook);
	void H_Extending(Mine* mine, Hook* hook);
	void drawline(Hook* hook);
	void putsole();
	bool collisiondetection(Mine* mine, Hook* hook);

	//���������Ķ�������
	Hook_Direction hook_direction;
	double angle;
	int ex;
	int ey;
	int length;
	Hook_State state;
	static int Hook_Speed;

private:
	//˽������
	int x;
	int y;
	IMAGE hookimage1, hookimage2;
	IMAGE soleimage1, soleimage2;
};

//����ľ�̬������ʼ������
int Hook::Hook_Speed = 4;

//����Ĺ��캯��
Hook::Hook()
{
	//��ʼ����״̬
	state = normal;
	x = width / 2;
	y = 120 - 20;
	angle = PI / 2;
	hook_direction = Hook_Direction::left;
	length = width / 16;
	ex = cos(angle) * length + x;
	ey = sin(angle) * length + y;
	setlinecolor(BROWN);
	setlinestyle(PS_COSMETIC, 5);
	//��ʼ������
	line(x, y, ex, ey);
	//���ؿ�ͼƬ
	loadimage(&soleimage1, "./file/images/char1.jpg", width / 8, height / 8);
	loadimage(&soleimage2, "./file/images/char1_mask.jpg", width / 8, height / 8);
}

Hook::~Hook()
{
	//û���õ��������ҿ󹳶����һֱ���ڣ����뵥������������
}

//��ײ��⺯��
bool Hook::collisiondetection(Mine* mine, Hook* hook)
{
	if (mine->exist == true)//�����ھͲ�����ײ
	{
		if (hook->ex >= mine->x && hook->ex <= (mine->x + mine->size) && hook->ey >= mine->y && hook->ey <= (mine->y + mine->size))
		{
			mine->x = hook->ex - mine->size / 2;
			mine->y = hook->ey - mine->size / 2;
			return true;
		};
	};
	return false;
}

void Hook::H_Round(Hook* hook)
{
	//cout<<"angle is" << angle << endl;
	//cout << "Direction is " << hook_direction << endl;
	if (hook->state == normal) {
		if (angle <= 10.0 / 180 * PI)
		{
			hook->hook_direction = Hook_Direction::left;
			//cout << "l" << endl;
		}
		else if (angle >= PI - 10.0 / 180 * PI)
		{
			hook->hook_direction = Hook_Direction::right;
			//cout << "r" << endl;
		};
		if (hook_direction == Hook_Direction::left)
		{
			hook->angle += PI / 128;
			//cout << "+" << PI / 128 << endl;
		}
		else if (hook_direction == Hook_Direction::right)
		{
			hook->angle -= PI / 128;
			//cout << "-" << PI / 128 << endl;
		}
		setlinecolor(BROWN);
		setlinestyle(PS_COSMETIC, 5);
		hook->ex = cos(angle) * length + x;
		hook->ey = sin(angle) * length + y;
		line(x, y, ex, ey);
	};
};
//���쳤�ĺ���
void Hook::H_Extending(Mine* mine, Hook* hook)
{
	if (hook->state == normal)//�󹳲�������Ӧ����ȡ�µ�״̬
	{
		if (GetAsyncKeyState(32) != 0&&hook->state==normal)//���¿ո�ʼ�쳤
		{
			cout << "Starting move" << endl;
			hook->state = extending;//�ı�״̬
			while (true)
			{
				GetAsyncKeyState(32);
				BeginBatchDraw();
				Sleep(1);
				if (hook->state == extending)
				{
					hook->length += 5;
					//����
					hook->drawline(hook);
					putbackgraound();
					hook->H_Round(hook);
					hook->drawline(hook);
					mine->M_Putimages(mine, Mine_Quantity);
				};
				//��ײ�����ʱ������
				for (int i = 0; i < Mine_Quantity; i++)
				{
					if (hook->collisiondetection(mine + i, hook) == true)
					{
						hook->state = shortening;
						cout << "hook->state = shortening;" << hook->state << " 1" << endl;
					};
					(mine + i)->M_Putimage(mine + i);
				};
				//��ײ���߽�ʱ������
				if (hook->ex <= 0 || hook->ex >= width || hook->ey <= hook->y || hook->ey >= height)
				{
					hook->state = shortening;
					cout << "hook->state = shortening;" << hook->state << " 2" << endl;
				};
				//������ʱ�Ļ������
				if (hook->state == shortening)
				{
					putbackgraound();
					hook->H_Round(hook);
					hook->drawline(hook);
					mine->M_Putimages(mine, Mine_Quantity);
					hook->length -= 5;
					cout << "hook->length -= 5;" << endl;
					if (hook->length <= width / 16)
					{
						hook->length = width / 16;
						hook->state = normal;
						break;
					};
				};
				EndBatchDraw();
			};
		};
	};
}
//���ߺ���
void Hook::drawline(Hook* hook)
{
	Hook::putsole();
	setlinecolor(BROWN);
	setlinestyle(PS_COSMETIC, 5);
	hook->ex = cos(hook->angle) * hook->length + hook->x;
	hook->ey = sin(hook->angle) * hook->length + hook->y;
	line(hook->x, hook->y, hook->ex, hook->ey);
}
//���ÿ�ͼ��
void Hook::putsole()
{
	putimage(width / 2-20, 60-5, &soleimage2, SRCPAINT);
	putimage(width / 2-20, 60-5, &soleimage1, SRCAND);
}
//�����˳����ѵ�����
void put_exitReminder()
{
	setbkmode(TRANSPARENT);
	settextcolor(BLUE);
	settextstyle(width / 24, height / 32, _T("����"));
	static TCHAR reminder1[50];
	static TCHAR reminder2[50];
	sprintf_s(reminder1, _T("��ESC���˳���Ϸ"));
	sprintf_s(reminder2, _T("��P����һ����Ϸ"));
	outtextxy(width / 16 * 11, 0, reminder1);
	outtextxy(width / 16 * 11, height / 16, reminder2);
}
//���ý���Ԫ��
void putinterface(Mine *mine,Hook *hook)
{
	BeginBatchDraw();
	putbackgraound();
	Mine::putValueSum();
	hook->putsole();
	hook->drawline(hook);
	put_exitReminder();
	hook->H_Round(hook);
	mine->M_Putimages(mine, Mine_Quantity);
	EndBatchDraw();
}

enum liquidtype//ҩˮ
{
	s_super = 16,//������ҩˮ
	super = 8,//����ҩˮ
	s_slow = 1,//������ҩˮ
	slow = 2,//����ҩˮ
};

struct Liquid
{
	int x;
	int y;
	int size = 120;
	bool flag;
	int type;
}liquid[Liquid_Quantity];

IMAGE shopbkimg;
IMAGE LiquidIMG[Liquid_Quantity];

void shopinit()//�̵��ʼ��
{
	loadimage(&shopbkimg, "./file/images/shop.png", width, height);//�̵걳��
	loadimage(&LiquidIMG[0], "./file/images/liquid.jpg", liquid[0].size, liquid[0].size);
	loadimage(&LiquidIMG[1], "./file/images/liquid2.jpg", liquid[1].size, liquid[1].size);
	loadimage(&LiquidIMG[2], "./file/images/liquid3.jpg", liquid[2].size, liquid[2].size);
	for (int i = 0; i < 3; i++)//��ʼ��ҩˮλ��-------��Ҫ����
	{
		liquid[i].x = 165 + i * 175;
		liquid[i].y = 355;
	}
	//srand(GetTickCount());
	for (int i = 0; i < Liquid_Quantity; i++)
	{
		int liquidtypeswitch = rand() % 4 + 1;
		switch (liquidtypeswitch)
		{
		case 1:
			liquid[i].type = s_super;
			break;
		case 2:
			liquid[i].type = super;
			break;
		case 3:
			liquid[i].type = s_slow;
			break;
		case 4:
			liquid[i].type = slow;
			break;
		}
		//liquid[i].type = rand() % 4 + 1;
		liquid[i].flag = true;
	}

}
void shopping()
{
	//sole.state = 0;
	shopinit();
	BeginBatchDraw();
	int type = 0;
	MOUSEMSG shopm;
	while (1)
	{
		putimage(0, 0, &shopbkimg);
		char score2[30] = "";//����
		sprintf_s(score2, "����:%d", Mine::getValeSum());
		settextcolor(WHITE);//������ɫ
		setbkmode(TRANSPARENT);//����͸����
		settextstyle(50, 0, "����");
		outtextxy(20, 20, score2);
		for (int i = 0; i < Liquid_Quantity; i++)
		{
			if (liquid[i].flag == true)
			{
				putimage(liquid[i].x, liquid[i].y, &LiquidIMG[i]);
			}
			if (type != 0)
			{
				char name[50] = " ";//ҩˮ����
				switch (type)
				{
				case s_super:
					sprintf_s(name, "����ó�������ҩˮ�������ٶ������ӿ�!");
					break;
				case super:
					sprintf_s(name, "����ô���ҩˮ,�����ٶȼӿ�!");
					break;
				case s_slow:
					sprintf_s(name, "����ó�������ҩˮ,�����ٶ���������!");
					break;
				case slow:
					sprintf_s(name, "����û���ҩˮ�������ٶȼ���!");
					break;
				}
				settextcolor(RED);//������ɫ
				setbkmode(TRANSPARENT);//����͸����
				settextstyle(50, 0, "����");
				outtextxy(100, 550, name);
			}
		}
		shopm = GetMouseMsg();
		if (shopm.uMsg == WM_LBUTTONDOWN)
		{
			if ((shopm.x > liquid[0].x && shopm.x < (liquid[0].x + liquid[0].size)) && (shopm.y > liquid[0].y && shopm.y < (liquid[0].y + liquid[0].size)))
			{
				//mciSendString("play shopmusic.mp3", NULL, 0, NULL);//��Ч
				Hook::Hook_Speed = liquid[0].type;
				liquid[0].flag = false;
				type = liquid[0].type;
				Mine::Value_Sum -= 200;
				continue;
			}
			if ((shopm.x > liquid[1].x && shopm.x < (liquid[1].x + liquid[1].size)) && (shopm.y > liquid[1].y && shopm.y < (liquid[1].y + liquid[1].size)))
			{
				//mciSendString("play shopmusic.mp3 ", NULL, 0, NULL);
				Hook::Hook_Speed = liquid[0].type;
				liquid[1].flag = false;
				type = liquid[1].type;
				Mine::Value_Sum -= 200;
				continue;
			}
			if ((shopm.x > liquid[2].x && shopm.x < (liquid[2].x + liquid[2].size)) && (shopm.y > liquid[2].y && shopm.y < (liquid[2].y + liquid[2].size)))
			{
				//mciSendString("play shopmusic.mp3", NULL, 0, NULL);
				Hook::Hook_Speed = liquid[0].type;
				liquid[2].flag = false;
				type = liquid[2].type;
				Mine::Value_Sum -= 200;
				continue;
			}
			if ((shopm.x > 832.5 && shopm.x < 985.5) && (shopm.y > 110 && shopm.y < 250))
			{
				break;
			}
		}
		FlushBatchDraw();
	}
	EndBatchDraw();
};
//����Ϸ����
int GoldMiner()
{
	srand((unsigned)time(NULL));//�������������
	initgraph(width, height);
	//���������
	BeginBatchDraw();
	Mine mine[Mine_Quantity];
	Hook hook;
	EndBatchDraw();
	while (true)
	{
		Sleep(10);
		hook.H_Extending(mine, &hook);
		putinterface(mine, &hook);
		if (GetAsyncKeyState(27) != 0)
		{
			break;
		};
	};
	return 0;
};

//˫�����ĺ���
void DoubleTick_Detection(int *flag)
{
	static int g_clickCount = 0;
	static DWORD g_lastClickTime = 0;
	DWORD current_time = 0;
	while (true)
	{

		/*if (g_clickCount < 2)
		{
			g_lastClickTime = GetTickCount();
		}*/
		if (GetAsyncKeyState(32))
		{
			current_time = GetTickCount();
			if (current_time - g_lastClickTime < 500 && current_time - g_lastClickTime > 50)
			{ // ˫����ʱ��������������Ϊ500����
				cout << "current_time is: " << current_time << endl << "g_lastClickTime is: " << g_lastClickTime << endl;
				g_clickCount++;
			}
			else {
				g_clickCount = 1;
				*flag = 0;
				//std::cout << "g_clickCount is: " << g_clickCount << std::endl;
			}
		}

		g_lastClickTime = current_time;
		//g_lastClickTime = GetTickCount();
		if (g_clickCount == 2) {
			// ˫���¼�����
			std::cout << "Double click detected! " << "g_clickCount is: " << g_clickCount << std::endl;
			*flag = 1;
			g_clickCount = 0; // ���õ������
			std::cout << "g_clickCount is: " << g_clickCount << std::endl;
		}
		if (GetAsyncKeyState(27) != 0)
		{
			break;
		};
	}
	return;
}

int main()
{
	int *flag = new int;
	*flag = 0;
	thread t1(DoubleTick_Detection,flag);
	GoldMiner();
	t1.join();
}