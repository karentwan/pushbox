#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <fstream>
#include <SDL_test.h>
#include <SDL_ttf.h>

/**
 * NONE:�հ׵ĵط�
 * WALL:ǽ��
 * BOX:����
 * PIT:��λ
 */
enum TileType {
	NONE,WALL,BOX,USER,PIT
};

/*
  ǽ�ڡ����ӡ����ӵ�Ŀ�ĵأ���λ�������ﶼ�����������ʾ
  ����TileTypeö����ͱ�ʾ��ʾ�������ʲô
 */
class Tile {
public:
	Tile();

	~Tile();

	Tile(TileType);

	int getX();//�õ�������

	int getY();//�õ�������

	void setXY(int x, int y);//��������

	void paintTile(SDL_Renderer *);//��������

	void setRect(int x, int y, int w, int h);

	void deleteTile();

protected:
	int w;//��
	int h;//��
	int x;//������
	int y;//������
	TileType mType;//�����߿������
	SDL_Rect mRect;//
	std::string mImagePath;//�����Tile���͵�ͼƬ
	SDL_Surface *mSurface; //��Tile��ת����Surface������SDL_Render������
public:
	Tile *mNext;//������һ��ͬ�����͵�Tile

};


/**
	Main
 */
class Game {
public:
	Game();

	~Game();

	/*
	��ʼ��Ϸ,���عؿ������ӡ������λ�ȵ�
	 */
	void initGame();

	/*
	 ����������Ϸ���ú�������ѭ�����汻���á����һ�ﵽˢ�µ�����
	 */
	void paintGame();

	void print();

	/*
	 ��Ϸ����������������һ��whileѭ����Ҳ������Ϸ��ѭ����
	 */
	void run();

	bool userMove(int x, int y);//�ж��û��Ƿ�����ƶ�

	bool boxMove(Tile *box, int x, int y);//�����ƶ�

	Tile *findWhichBox(int x, int y);//�ҵ�����һ�����ӱ��ƶ�

	void handleKeyPress(const SDL_Event &event);//���������Ӧ

	void loadRoundFromFile(std::string path, int a[20][20]);//���ļ����عؿ�

	bool isWin();//��Ϸ�Ƿ�Ӯ��
	/**
	 * @parameter str :Ҫ���Ƶ�����
	 * @parameter x:Ҫ���Ƶĺ�����
	 * @parameter y:Ҫ���Ƶ�������
	 * @parameter size:Ҫ���Ƶ����ִ�С
	 * @parameter color:Ҫ���Ƶ�������ɫ
	 */
	void paintTxt(std::string str, int x, int y, int size = 100, SDL_Color color = { 100, 200, 200 });

private :
	int mChessbord[20][20];//��ʼ����
	int mWin[20][20];//Ӯ�˵Ĳ���
	Tile *mUser;//�û�
	Tile *mBoxs;//����
	Tile *mPits;//��λ
	Tile *mWalls;//ǽ
	SDL_Window *mWindows;
	SDL_Renderer *mRender;
};

