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

typedef struct mine_location{
	int x;
	int y;
	int size;
	struct mine_location * next;
}Mine_Location;

void loadbackgraound()
{
	loadimage(imgs + 4, "./file/images/level-background-0.jpg", width, height/16*14);
}

enum Hook_Direction
{
	left,
	right
};

enum Hook_State
{
	normal,
	extending,
	shortening
};

class Hook
{
	friend class Mine;
public:
	Hook();
	~Hook();
	void H_Round(Hook *hook);
	void H_Extending(Mine *mine, Hook *hook);

	Hook_Direction hook_direction;
	double angle;
	int ex;
	int ey;
	int length;
	Hook_State state;

private:
	int x;
	int y;
	
	IMAGE hookimage1, hookimage2;
	IMAGE soleimage1, soleimage2;
};

Hook::Hook()
{
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



class Mine
{
	friend class Hook;
public:
	Mine();
	~Mine();
	void M_loadimage();
	void M_Putimage(Mine *mine);
	bool collisiondetection(Mine *mine,Hook *hook);
	//friend void H_Extending(Mine* mine, Hook* hook);
	//void M_LocationChange(Mine *mine,Hook *hook);
	//Mine_Location *M_LocationInit();
	//void create_xysize(int* count, Mine_Location* h);

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

/*void Mine::M_LocationChange(Mine* mine, Hook* hook)
{

}*/

bool Mine::collisiondetection(Mine* mine, Hook* hook)
{
	if (hook->ex >= mine->x && hook->ex <= (mine->x + mine->size) && hook->ey >= mine->y && hook->ey <= (mine->y + mine->size))
	{
		mine->x = hook->ex - mine->size / 2;
		mine->y = hook->ey - mine->size / 2;
		return true;
	};
	
}

void Mine::M_Putimage(Mine *mine)
{
	putimage(mine->x, mine->y, &(mine->img1), SRCPAINT);
	putimage(mine->x, mine->y, &(mine->img2), SRCAND);
}


void Mine::M_loadimage()
{
	if(Mine::size<width/8)
	{
		loadimage(&img1, "./file/images/small_gold_mask.bmp", size, size);
		loadimage(&img2, "./file/images/small_gold.bmp", size, size);
	}
	else
	{
		loadimage(&img1, "./file/images/diamond_mask.png", size/4, size/4);
		loadimage(&img2, "./file/images/diamond.png", size/4, size/4);
	}
}

Mine::Mine()
{
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
				goto create_xysize;
			}
			/*if ((((p->x + p->size) <= s->x) || (p->x >= (s->x + s->size))) && (((p->y + p->size) <= s->y) || (p->y >= (s->y + s->size))))
			{
				cout << "A new linked list element has been generated successfully!" << endl;
			}
			else
			{
				cout << "Build failed, start regenerate" << endl;
				goto create_xysize;
			};*/
			
			if (s != nullptr)s = s->next;
		}
		r->next = p;
		r = p;
		x = p->x;
		y = p->y;
		size = p->size;
	};
	cout << "X is " << x << "," << "Y is " << y << "," << "Its size is " << size << endl;
	static int Mine_count = 0;
	Mine_count += 1;
	cout << "Mine_count is " << Mine_count << endl;
	/*x = width / 256 * (rand()%(256+1));
	y = height / 256 * (rand()%(256-16*3+1)) + height / 16 * 3;
	size = width / 128 * (rand() % (16 + 1)) + width / 32;*/
	value = size * 10;
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
	cout << "Object has been deleted" << endl;
}

void Hook::H_Extending(Mine* mine, Hook* hook)
{
	if (hook->state == normal)
	{
		if (GetAsyncKeyState(32) != 0)
		{
			cout << "Starting move" << endl;
			hook->state = extending;
			while (true)
			{
				if (hook->state == extending)
				{
					hook->length += 5;
				};
				for (int i = 0; i < Mine_Quantity; i++)
				{
					if ((mine + i)->collisiondetection(mine + i, hook) == true)
					{
						hook->state = shortening;
						cout << "hook->state = shortening;" << hook->state << " 1" << endl;
						//break;
					};
				};
				if (hook->ex <= 0 || hook->ex >= width || hook->ey <= hook->y || hook->ey >= height)
				{
					hook->state = shortening;
					cout << "hook->state = shortening;" << hook->state << " 2" << endl;
					//break;
				};
				if (hook->state == shortening)
				{
					hook->length -= 5;
					cout << "hook->length -= 5;" << endl;
					if (hook->length <= width / 16)
					{
						hook->length = width / 16;
						hook->state = normal;
						break;
					};
				};
				setlinecolor(BROWN);
				setlinestyle(PS_COSMETIC, 5);
				hook->ex = cos(hook->angle) * hook->length + hook->x;
				hook->ey = sin(hook->angle) * hook->length + hook->y;
				line(hook->x, hook->y, hook->ex, hook->ey);
			};
		};
	};
};

int main()
{
	srand((unsigned)time(NULL));//生成随机数种子
	initgraph(width, height);
	loadbackgraound();
	putimage(0, 120, imgs + 4);

	//cout <<"RAND_MAX is " << RAND_MAX << endl;
	
	BeginBatchDraw();
	Mine mine[Mine_Quantity];
	Hook hook;
	EndBatchDraw();
	
	/*setlinecolor(BROWN);
	setlinestyle(PS_COSMETIC, 5);
	int a = 5;
	int ex = cos(a) * 5 + width / 2;
	int ey = sin(a) * 5 + 120;
	line(width / 2, 120, ex, ey);
	a = 10;
	ex = cos(5) * 5 + width/2;
	ey = sin(5) * 5 + 120;*/
	


	while (true)
	{
		Sleep(10);
		/*if (GetAsyncKeyState(32) != 0)
		{
			setlinecolor(RED);
			setfillcolor(WHITE);
			fillcircle(width / 2, height / 2, height / 4);
			Sleep(500);
			setlinecolor(BLACK);
			setfillcolor(BLACK);
			fillcircle(width / 2, height / 2, height / 4);
		};*/
		/*if (hook.state == normal)
		{
			if (GetAsyncKeyState(32) != 0)
			{
				cout << "Starting move" << endl;
				hook.state = extending;
				while (true)
				{
					hook.length += 5;
					for (int i = 0; i < Mine_Quantity; i++)
					{
						if ((mine + i)->collisiondetection(mine + i, &hook) == true)
						{
							hook.state = normal;
							break;
						};
					};
					if (hook.ex <= 0 || hook.ex >= width || hook.ey <= 0 || hook.ey >= height)
					{
						break;
					};
					setlinecolor(BROWN);
					setlinestyle(PS_COSMETIC, 5);
					hook.ex = cos(hook.angle) * hook.length + hook.x;
					hook.ey = sin(hook.angle) * hook.length + hook.y;
					line(x, y, ex, ey);
				}
			}
		}*/
		BeginBatchDraw();
		hook.H_Extending(mine, &hook);
		setfillcolor(YELLOW);
		setlinecolor(YELLOW);
		fillrectangle(0, 0, width, 120);
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