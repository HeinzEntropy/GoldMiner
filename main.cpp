#include<iostream>
#include<graphics.h>
#include<math.h>
#include<cstdlib>
#include<ctime>

using namespace std;

#define width 1080
#define height 720
#define IMGS_QUANTITY 5
#define PI 3.1415926535
constexpr auto Mine_Quantity = (const int)10;

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
void loadbackgraound()
{
	loadimage(imgs + 4, "./file/images/level-background-0.jpg", width, height/16*14);
}

class Mine
{
	//友元矿钩
	friend class Hook;
public:
	Mine();
	~Mine();
	//声明类方法
	void M_loadimage();
	void M_Putimage(Mine* mine);
	bool collisiondetection(Mine* mine, Hook* hook);
	static int Value_Sum;
	static void putValueSum();
	//friend void H_Extending(Mine* mine, Hook* hook);
	//void M_LocationChange(Mine *mine,Hook *hook);
	//Mine_Location *M_LocationInit();
	//void create_xysize(int* count, Mine_Location* h);

private:
	//创建精灵图和掩码图
	IMAGE img1, img2;
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

//矿钩类型
class Hook
{
	//友元Mine类型
public: friend class Mine;
public:
	Hook();
	~Hook();
	//声明类方法
	void H_Round(Hook *hook);
	void H_Extending(Mine *mine, Hook *hook);
	void drawline(Hook* hook);

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
	y = 120-20;
	angle = PI/2;
	hook_direction = Hook_Direction::left;
	length = width / 16;
	ex = cos(angle) * length + x;
	ey = sin(angle) * length + y;
	setlinecolor(BROWN);
	setlinestyle(PS_COSMETIC, 5);
	line(x, y, ex, ey);
}

Hook::~Hook()
{
	//没有用到堆区，且矿钩对象会一直存在，无须单独的析构函数
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

//碰撞检测函数
bool Mine::collisiondetection(Mine* mine, Hook* hook)
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

void Mine::putValueSum()
{
	setbkmode(TRANSPARENT);
	settextcolor(BLUE);
	settextstyle(width / 8, height / 16, _T("宋体"));
	static TCHAR ValueSum[50];
	//sprintf()
	sprintf_s(ValueSum, _T("得分：%d"), Mine::Value_Sum);
	outtextxy(0, 0, ValueSum);

}



//重新放置照片
void Mine::M_Putimage(Mine* mine)
{
	//检测是否应该存在并被放置
	/*if (mine->y <= 160)
	{
		mine->exist = false;
		Mine::Value_Sum = Mine::Value_Sum + mine->value;
	};*/
	if (mine->exist == true)//不存在则不放置
	{
		if (mine->y <= 160)
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
		p->y = height / 256 * (rand() % (256 - 16 * 3 + 1)) + height / 16 * 3;
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
		p->y = height / 256 * (rand() % (256 - 16 * 3 + 1)) + height / 16 * 3;
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
	/*x = width / 256 * (rand()%(256+1));
	y = height / 256 * (rand()%(256-16*3+1)) + height / 16 * 3;
	size = width / 128 * (rand() % (16 + 1)) + width / 32;*/
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
				Sleep(10);
				if (hook->state == extending)
				{
					hook->length += 5;
					//绘制
					hook->drawline(hook);
					setfillcolor(YELLOW);
					setlinecolor(YELLOW);
					fillrectangle(0, 0, width, 120);
					putimage(0, 120, imgs + 4);
					hook->H_Round(hook);
					hook->drawline(hook);
					for (int i = 0; i < Mine_Quantity; i++)
					{
						(mine + i)->M_Putimage(mine + i);
					};
				};
				//碰撞到矿藏时矿钩缩短
				for (int i = 0; i < Mine_Quantity; i++)
				{
					if ((mine + i)->collisiondetection(mine + i, hook) == true)
					{
						hook->state = shortening;
						cout << "hook->state = shortening;" << hook->state << " 1" << endl;
						//break;
					};
					/*if ((mine + i)->y <= 10)
					{
						mine->exist == false;
					};*/
					(mine + i)->M_Putimage(mine + i);
				};
				//碰撞到边界时矿钩缩短
				if (hook->ex <= 0 || hook->ex >= width || hook->ey <= hook->y || hook->ey >= height)
				{
					hook->state = shortening;
					cout << "hook->state = shortening;" << hook->state << " 2" << endl;
					//break;
				};
				if (hook->state == shortening)
				{
					setfillcolor(YELLOW);
					setlinecolor(YELLOW);
					fillrectangle(0, 0, width, 120);
					putimage(0, 120, imgs + 4);
					hook->H_Round(hook);
					hook->drawline(hook);
					for (int i = 0; i < Mine_Quantity; i++)
					{
						(mine + i)->M_Putimage(mine + i);
					};
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
	setlinecolor(BROWN);
	setlinestyle(PS_COSMETIC, 5);
	hook->ex = cos(hook->angle) * hook->length + hook->x;
	hook->ey = sin(hook->angle) * hook->length + hook->y;
	line(hook->x, hook->y, hook->ex, hook->ey);
};


int main()
{
	//Mine::Value_Sum = 0;
	srand((unsigned)time(NULL));//生成随机数种子
	initgraph(width, height);
	loadbackgraound();
	putimage(0, 120, imgs + 4);

	//cout <<"RAND_MAX is " << RAND_MAX << endl;
	//生成类对象
	BeginBatchDraw();
	Mine mine[Mine_Quantity];
	Hook hook;
	EndBatchDraw();

	while (true)
	{
		Sleep(10);
		hook.H_Extending(mine, &hook);
		BeginBatchDraw();
		hook.drawline(&hook);
		
		setfillcolor(YELLOW);
		setlinecolor(YELLOW);
		fillrectangle(0, 0, width, 120);
		Mine::putValueSum();
		putimage(0, 120, imgs + 4);
		hook.H_Round(&hook);
		for (int i = 0; i < Mine_Quantity; i++)
		{
			(mine + i)->M_Putimage(mine + i);
		};
		EndBatchDraw();
		
		if (GetAsyncKeyState(27) != 0)
		{
			break;
		};
	};
	
};