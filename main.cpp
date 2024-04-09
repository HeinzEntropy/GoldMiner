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
constexpr auto Mine_Quantity = (const int)18;

IMAGE imgs[IMGS_QUANTITY];
IMAGE imgbk[11];
IMAGE img[10];
IMAGE imgl[3];

//矿藏的坐标结构体
typedef struct mine_location{
	int x;
	int y;
	int size;
	struct mine_location * next;
}Mine_Location;

//加载背景
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

//钩子方向
enum Hook_Direction
{
	left,
	right
};

//钩子状态
enum Hook_State
{
	normal,
	extending,
	shortening
};

class Mine
{
	//友元矿钩
	friend class Hook;

public:
	Mine();
	~Mine();
	//声明类方法
	void M_loadimage();
	void M_Putimage(Mine *mine);
	static int Value_Sum;
	static void putValueSum();
	void M_Putimages(Mine* mine, int M_quantity);

private:
	//创建精灵图和掩码图
	IMAGE img1,img2;
	//坐标及其大小
	int x;
	int y;
	int size;
	//矿的价值
	int value;
	//矿的存在性
	bool exist;

};
//Mine类的静态变量集中初始化区
int Mine::Value_Sum = 0;

void Mine::putValueSum()
{
	setbkmode(TRANSPARENT);
	settextcolor(BLUE);
	settextstyle(width / 12, height / 18, _T("宋体"));
	static TCHAR ValueSum[50];
	sprintf_s(ValueSum, _T("得分：%d"), Mine::Value_Sum);
	outtextxy(0, 0, ValueSum);
}

void Mine::M_Putimages(Mine* mine, int M_quantity)
{
	for (int i = 0; i < M_quantity; i++)
	{
		(mine + i)->M_Putimage(mine + i);
	};
}

//重新放置照片
void Mine::M_Putimage(Mine* mine)
{
	if (mine->exist == true)//不存在则不放置
	{
		if (mine->y <= 160 && (mine->x>=width/16*6&&mine->x<=width/16*10))
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
	//加载照片
	cout <<"M_loadimage:" << "size is " << size << endl;
	if (Mine::value <=1000)//较低价值加载金矿
	{
		loadimage(&img1, "./file/images/small_gold_mask.bmp", size, size);
		loadimage(&img2, "./file/images/small_gold.bmp", size, size);
		cout << "M_loadimage:" << "size is " << size << endl;
	}
	else//较高价值加载钻石
	{
		loadimage(&img1, "./file/images/diamond_mask.png", size/4, size/4);
		loadimage(&img2, "./file/images/diamond.png", size/4, size/4);
		size = size / 4;
		cout << "M_loadimage:" << "size is " << size << endl;
	}
}

Mine::Mine()
{
	//构造函数，使用了链表保证矿藏不隐藏
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
		//goto标记点
	create_xysize:
		Mine_Location* s = h;
		p->x = width / 256 * (rand() % (256 + 1));
		p->y = height / 256 * (rand() % (256 - 16 * 3 + 1)) + height / 16 * 4;
		p->size = width / 128 * (rand() % (16 + 1)) + width / 32;
		p->next = nullptr;
		while (s->next != nullptr)
		{
			cout << "New linked list element is generating, "<<"count is "<<++count << endl;
			//坐标是否取用的判断条件，为保证矿藏不隐藏
			if (((p->x + p->size) >= s->x && (p->x <= (s->x + s->size))) && (((p->y + p->size) >= s->y && p->y <= (s->y + s->size))))
			{
				cout << "Build failed, start regenerate" << endl;
				goto create_xysize;//重新进行赋值并比较
			};
			if (s != nullptr)s = s->next;
		}
		r->next = p;
		r = p;
		x = p->x;
		y = p->y;
		size = p->size;
	};
	//控制台打印信息，不影响游玩（反正也关不掉控制台
	cout << "X is " << x << "," << "Y is " << y << "," << "Its size is " << size << endl;
	//矿的数量计量
	static int Mine_count = 0;
	Mine_count += 1;
	cout << "Mine_count is " << Mine_count << endl;
	//矿的价值赋予
	value = size * 10;
	//矿的存在性赋予
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
	//考虑在增加关卡时实现析构函数
	cout << "Object has been deleted" << endl;
}

//矿钩类型
class Hook
{
	//友元Mine类型
public: friend class Mine;
public:
	Hook();
	~Hook();
	//声明类方法
	void H_Round(Hook* hook);
	void H_Extending(Mine* mine, Hook* hook);
	void drawline(Hook* hook);
	void putsole();
	bool collisiondetection(Mine* mine, Hook* hook);

	//声明公开的对象内容
	Hook_Direction hook_direction;
	double angle;
	int ex;
	int ey;
	int length;
	Hook_State state;

private:
	//私有内容
	int x;
	int y;

	IMAGE hookimage1, hookimage2;
	IMAGE soleimage1, soleimage2;
};

Hook::Hook()
{
	//构造函数
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
	line(x, y, ex, ey);
	loadimage(&soleimage1, "./file/images/char1.jpg", width / 8, height / 8);
	loadimage(&soleimage2, "./file/images/char1_mask.jpg", width / 8, height / 8);

}

Hook::~Hook()
{
	//没有用到堆区，且矿钩对象会一直存在，无须单独的析构函数
}

//碰撞检测函数
bool Hook::collisiondetection(Mine* mine, Hook* hook)
{
	if (mine->exist == true)//不存在就不会碰撞
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


//矿钩伸长的函数
void Hook::H_Extending(Mine* mine, Hook* hook)
{
	if (hook->state == normal)//矿钩不正常不应当读取新的状态
	{
		if (GetAsyncKeyState(32) != 0&&hook->state==normal)//按下空格开始伸长
		{
			cout << "Starting move" << endl;
			hook->state = extending;//改变状态
			while (true)
			{
				BeginBatchDraw();
				Sleep(1);
				if (hook->state == extending)
				{
					hook->length += 5;
					//绘制
					hook->drawline(hook);
					putbackgraound();
					hook->H_Round(hook);
					hook->drawline(hook);
					mine->M_Putimages(mine, Mine_Quantity);
				};
				//碰撞到矿藏时矿钩缩短
				for (int i = 0; i < Mine_Quantity; i++)
				{
					if (hook->collisiondetection(mine + i, hook) == true)
					{
						hook->state = shortening;
						cout << "hook->state = shortening;" << hook->state << " 1" << endl;
					};
					(mine + i)->M_Putimage(mine + i);
				};
				//碰撞到边界时矿钩缩短
				if (hook->ex <= 0 || hook->ex >= width || hook->ey <= hook->y || hook->ey >= height)
				{
					hook->state = shortening;
					cout << "hook->state = shortening;" << hook->state << " 2" << endl;
				};
				//矿钩缩短时的画面绘制
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

void Hook::drawline(Hook* hook)
{
	Hook::putsole();
	setlinecolor(BROWN);
	setlinestyle(PS_COSMETIC, 5);
	hook->ex = cos(hook->angle) * hook->length + hook->x;
	hook->ey = sin(hook->angle) * hook->length + hook->y;
	line(hook->x, hook->y, hook->ex, hook->ey);
}

void Hook::putsole()
{
	putimage(width / 2-20, 60-5, &soleimage2, SRCPAINT);
	putimage(width / 2-20, 60-5, &soleimage1, SRCAND);
}

void put_exitReminder()
{
	setbkmode(TRANSPARENT);
	settextcolor(BLUE);
	settextstyle(width / 24, height / 32, _T("宋体"));
	static TCHAR reminder1[50];
	static TCHAR reminder2[50];
	sprintf_s(reminder1, _T("按ESC键退出游戏"));
	sprintf_s(reminder2, _T("按P键下一关游戏"));
	outtextxy(width / 16 * 11, 0, reminder1);
	outtextxy(width / 16 * 11, height / 16, reminder2);
}

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

int GoldMiner()
{
	srand((unsigned)time(NULL));//生成随机数种子
	initgraph(width, height);
	putimage(0, 120, imgs + 4);
	//生成类对象
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

int main()
{
	GoldMiner();
}