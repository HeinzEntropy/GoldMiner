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
class Mine
{
public:
	Mine();
	~Mine();
	void M_loadimage();
	Mine_Location *M_LocationInit();
	//void create_xysize(int* count, Mine_Location* h);

private:
	//��������ͼ������ͼ
	IMAGE img1,img2;
	//���꼰���С
	int x;
	int y;
	int size;
	bool exist;
	
};

/*void Mine::create_xysize(int* count, Mine_Location* h)
{
	Mine_Location *s,*p;
	create_xysize:
	p->x = width / 256 * (rand() % (256 + 1));
	p->y = height / 256 * (rand() % (256 - 16 * 3 + 1)) + height / 16 * 3;
	p->size = width / 128 * (rand() % (16 + 1)) + width / 32;
	for (s = h; s->next!=NULL;s=s->next)
	{
		if (s->x>=)
		{

		}
	}
}*/

Mine_Location *Mine::M_LocationInit()
{	
	static Mine_Location* h = nullptr;
	static Mine_Location* r = h;
	Mine_Location* p = new Mine_Location;
	if (h == nullptr)
	{
		p->x= width / 256 * (rand() % (256 + 1));
		p->y= height / 256 * (rand() % (256 - 16 * 3 + 1)) + height / 16 * 3;
		p->size = width / 128 * (rand() % (16 + 1)) + width / 32;
		p->next = nullptr;
		h = p;
		r = p;
	}
	else
	{
		
	create_xysize:
		Mine_Location* s = h; 
		p->x = width / 256 * (rand() % (256 + 1));
		p->y = height / 256 * (rand() % (256 - 16 * 3 + 1)) + height / 16 * 3;
		p->size = width / 128 * (rand() % (16 + 1)) + width / 32;
		p->next = nullptr;
		while(s != nullptr)
		{
			if (p->x >= s->x && (p->x + p->size) <= (s->x + s->size) && p->y >= s->y && (p->y+p->size) <=(s->y+s->size))
			{
				goto create_xysize;
			}
			if (s != nullptr)s = s->next;
		}
		r->next = p;
		r = p;
	};
	return h;
	


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
	Mine::M_LocationInit();
	static Mine_Location* s = Mine::M_LocationInit();
	cout << M_LocationInit()->x<<endl;
	cout << s->x << endl;
	static int Mine_count = 0;
	Mine_count += 1;
	cout << "Mine_count is " << Mine_count << endl;
	/*x = width / 256 * (rand()%(256+1));
	y = height / 256 * (rand()%(256-16*3+1)) + height / 16 * 3;
	size = width / 128 * (rand() % (16 + 1)) + width / 32;*/
	exist = true;
	int count = 0;
	while (count < Mine_count)
	{
		if (count == Mine_count - 1)
		{
			x = s->x;
			y = s->y;
			size = s->size;
		};
		s = s->next;
	}
	
	Mine::M_loadimage();
	BeginBatchDraw();
	putimage(x, y, &img1, SRCPAINT);
	putimage(x, y, &img2, SRCAND);
	EndBatchDraw();
	cout << "Object has been created" << endl;
}

Mine::~Mine()
{
	cout << "Object has been deleted" << endl;
}

int main()
{
	srand((unsigned)time(NULL));//�������������
	initgraph(width, height);
	loadbackgraound();
	putimage(0, 120, imgs + 4);

	//cout <<"RAND_MAX is " << RAND_MAX << endl;
	BeginBatchDraw();
	Mine mine[Mine_Quantity];
	EndBatchDraw();
	

	while (true)
	{
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
		if (GetAsyncKeyState(27) != 0)
		{
			break;
		};
	};
	
};