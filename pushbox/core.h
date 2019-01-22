#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <fstream>
#include <SDL_test.h>
#include <SDL_ttf.h>

/**
 * NONE:空白的地方
 * WALL:墙壁
 * BOX:箱子
 * PIT:坑位
 */
enum TileType {
	NONE,WALL,BOX,USER,PIT
};

/*
  墙壁、箱子、箱子的目的地（坑位）、人物都用这个类来表示
  上面TileType枚举类就表示表示该类代表什么
 */
class Tile {
public:
	Tile();

	~Tile();

	Tile(TileType);

	int getX();//得到横坐标

	int getY();//得到纵坐标

	void setXY(int x, int y);//设置坐标

	void paintTile(SDL_Renderer *);//绘制自身

	void setRect(int x, int y, int w, int h);

	void deleteTile();

protected:
	int w;//宽
	int h;//高
	int x;//横坐标
	int y;//纵坐标
	TileType mType;//代表瓦块的类型
	SDL_Rect mRect;//
	std::string mImagePath;//保存该Tile类型的图片
	SDL_Surface *mSurface; //该Tile所转换的Surface，方便SDL_Render来绘制
public:
	Tile *mNext;//连接下一个同样类型的Tile

};


/**
	Main
 */
class Game {
public:
	Game();

	~Game();

	/*
	初始游戏,加载关卡、箱子、人物、坑位等等
	 */
	void initGame();

	/*
	 绘制整个游戏，该函数在主循环里面被调用、并且会达到刷新的作用
	 */
	void paintGame();

	void print();

	/*
	 游戏启动，并在里面有一个while循环（也就是游戏主循环）
	 */
	void run();

	bool userMove(int x, int y);//判断用户是否可以移动

	bool boxMove(Tile *box, int x, int y);//箱子移动

	Tile *findWhichBox(int x, int y);//找到是哪一个箱子被推动

	void handleKeyPress(const SDL_Event &event);//处理键盘响应

	void loadRoundFromFile(std::string path, int a[20][20]);//从文件加载关卡

	bool isWin();//游戏是否赢了
	/**
	 * @parameter str :要绘制的文字
	 * @parameter x:要绘制的横坐标
	 * @parameter y:要绘制的纵坐标
	 * @parameter size:要绘制的文字大小
	 * @parameter color:要绘制的文字颜色
	 */
	void paintTxt(std::string str, int x, int y, int size = 100, SDL_Color color = { 100, 200, 200 });

private :
	int mChessbord[20][20];//初始布局
	int mWin[20][20];//赢了的布局
	Tile *mUser;//用户
	Tile *mBoxs;//箱子
	Tile *mPits;//坑位
	Tile *mWalls;//墙
	SDL_Window *mWindows;
	SDL_Renderer *mRender;
};

