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

IMAGE imgs[IMGS_QUANTITY];

typedef struct mine_location{
	int x;
	int y;
	int size;
	mine_location * next;
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

private:
	//坐标
	IMAGE img1,img2;
	int x;
	int y;
	int size;
	bool exist;
};

void Mine::M_loadimage()
{
	if(Mine::size<width/16)
	{
		loadimage(&img1, "./file/images/small_gold_mask.bmp", size, size);
		loadimage(&img2, "./file/images/small_gold.bmp", size, size);
	}
	else
	{
		loadimage(&img1, "./file/images/big_gold_mask.jpg", size, size);
		loadimage(&img2, "./file/images/big_gold.jpg", size, size);
	}
}

Mine::Mine()
{
	x = width / 256 * (rand()%(256+1));
	y = height / 256 * (rand()%(256-16*3+1)) + height / 16 * 3;
	size = width / 128 * (rand() % (16 + 1)) + width / 32;
	exist = true;
	Mine::M_loadimage();
	putimage(x, y, &img1, SRCPAINT);
	putimage(x, y, &img2, SRCAND);
	cout << "Object has been created" << endl;
}

Mine::~Mine()
{
	cout << "Object has been deleted" << endl;
}

int main()
{
	srand((unsigned)time(NULL));//生成随机数种子
	initgraph(width, height);
	loadbackgraound();
	putimage(0, 120, imgs + 4);

	//cout <<"RAND_MAX is " << RAND_MAX << endl;
	Mine mine[10];
	

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