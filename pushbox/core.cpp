#include "core.h"

using namespace std;

Tile::Tile() {
	cout << "init tile" << endl;
	
}

Tile::~Tile() {
	//cout << "我是析构函数" << endl;
	SDL_FreeSurface(mSurface);
}

Tile::Tile(TileType type) {
	mType = type;
	switch (type) {
	case NONE:
		break;
	case WALL:
		mImagePath = "res/Wall_Black.png";
		break;
	case BOX:
		mImagePath = "res/Crate_Yellow.png";
		break;
	case USER:
		mImagePath = "res/Character4.png";
		break;
	case PIT:
		mImagePath = "res/EndPoint_Red.png";
		//mSurface = IMG_Load(mImagePath.c_str());
		break;
	}
	mSurface = IMG_Load(mImagePath.c_str());
}

void Tile::deleteTile() {
	if (!mNext) {
		delete this;
		return;
	}
	mNext->deleteTile();
	delete this;

}

void Tile::setRect(int x, int y, int w, int h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

int Tile::getX() {
	return x;
}

int Tile::getY() {
	return y;
}

void Tile::paintTile(SDL_Renderer *render) {
	//cout << "绘制自身。。。。。" << endl;
	SDL_Rect rect1, rect2;
	rect1.w = 64;
	rect1.h = 64;
	rect1.x = 0;
	rect1.y = 0;
	rect2.w = 64;
	rect2.h = 64;
	rect2.x = y * 64;
	rect2.y = x * 64;
	SDL_Texture *tex = SDL_CreateTextureFromSurface(render, mSurface);
	SDL_RenderCopy(render, tex, &rect1, &rect2);
	SDL_DestroyTexture(tex);
}

/*
 * 设置x、y坐标
 */
void Tile::setXY(int x, int y) {
	this->x = x;
	this->y = y;
}

Game::Game() {
	mWindows = SDL_CreateWindow("push box", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 600, SDL_WINDOW_SHOWN);
	mRender = SDL_CreateRenderer(mWindows, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	initGame();
}

Game::~Game() {
	//销毁指针成员变量
	mWalls->deleteTile();
	mBoxs->deleteTile();
	mPits->deleteTile();
	mUser->deleteTile();
	SDL_DestroyRenderer(mRender);
	SDL_DestroyWindow(mWindows);
	SDL_Quit();
}

/*
 * 从文件加载关卡
 */
void Game::loadRoundFromFile(std::string path, int a[20][20]) {
	ifstream file(path.c_str());
	char buffer[20] = {'0'};
	int i = 0;
	while (!file.eof())
	{
		file.getline(buffer, 20);
		//cout << buffer << endl;
		for (int j = 0; j < 20; j++) {
			a[i][j] = buffer[j] - '0';
		}
		i++;
	}
}

void Game::initGame() {
	cout << "init chessbord..." << endl;
	loadRoundFromFile("res/round.txt", mChessbord);//加载游戏初始布局
	loadRoundFromFile("res/win.txt", mWin);
	Tile **wallLink = &mWalls;
	Tile **pitLink = &mPits;
	Tile **boxLink = &mBoxs;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			switch (mChessbord[i][j]) {
			case NONE:
				break;
			case WALL:
				//cout << "add wall, i:" << i << "\tj:" << j << "\t[i][j]:" << mChessbord[i][j] << endl;
				*wallLink = new Tile(WALL);
				(*wallLink)->setRect(i, j, 64, 64);
				wallLink = &(*wallLink)->mNext;
				break;
			case USER:
				mUser = new Tile(USER);
				mUser->setRect(i , j , 64, 64);
				mUser->mNext = NULL;
				break;
			case BOX:
				*boxLink = new Tile(BOX);
				(*boxLink)->setRect(i , j , 64, 64);
				boxLink = &(*boxLink)->mNext;
				break;
			case PIT:
				//cout << "pit i:" << i << "\tj:" << j << "\tvalue:" << mChessbord[i][j] << endl;
				*pitLink = new Tile(PIT);
				(*pitLink)->setRect(i , j , 64, 64);
				pitLink = &(*pitLink)->mNext;
				break;
			}
		}
	}
}

bool Game::isWin() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			if (mChessbord[i][j] != mWin[i][j])
				return false;
		}
	}
	return true;
}
/*
 * 绘制整个游戏界面
 */
void Game::paintGame() {
	//清空
	SDL_RenderClear(mRender);
	SDL_SetRenderDrawColor(mRender, 000, 200, 200, SDL_ALPHA_OPAQUE);
	//TODO 绘制墙
	Tile *temp = mWalls;
	while (temp) {
		//cout << "draw wall in paintGame" << endl;
		temp->paintTile(mRender);
		temp = temp->mNext;
	}
	//TODO 绘制坑位
	temp = mPits;
	while (temp) {
		//cout << "draw pit in paintGame" << endl;
		temp->paintTile(mRender);
		temp = temp->mNext;
	}
	//TODO 绘制箱子
	temp = mBoxs;
	while (temp) {
		//cout << "draw wall in paintGame" << endl;
		temp->paintTile(mRender);
		temp = temp->mNext;
	}
	//TODO 绘制玩家
	mUser->paintTile(mRender);
	//绘制文字
	if (isWin()) {
		cout << "win ..." << endl;
		SDL_Color color = { 255, 255, 255 };
		paintTxt("YOU WIN!!!!!", 180, 230, 100, color);
	}
	SDL_RenderPresent(mRender);
}

void Game::print() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			cout << mChessbord[i][j];
		}
		cout << endl;
	}
}

/*
  箱子移动
 */
bool Game::boxMove(Tile *box, int x, int y) {
	int bx = box->getX();
	int by = box->getY();
//	bx += x;
	//by += y;
	if (mChessbord[bx + x][by + y] == WALL) {
		cout << "box move false" << endl;
		return false; 
	}
	else if (mChessbord[bx + x][by + y] == BOX) { // 箱子前进方向是箱子
		return false;
	}	else { // 前进的方向什么都没有
		box->setXY(bx + x, by + y);
		//移动之后更新mChessbord
		mChessbord[bx][by] = 0;
		mChessbord[bx + x][by + y] = BOX;
		return true;
	}
	
}

/*
 * 玩家移动
 */
bool Game::userMove(int x, int y) {
	int ux = mUser->getX();
	int uy = mUser->getY();
	ux += x;
	uy += y;
	//要移动的方向是墙壁
	if (mChessbord[ux][uy] == WALL) {
		cout << "user move false" << endl;
		return false;
	} 	else if (mChessbord[ux][uy] == BOX) { //前进的方向有箱子
		Tile *box = findWhichBox(ux, uy);//找到箱子
		if (boxMove(box, x, y)) {
			mUser->setXY(ux, uy);
			return true;
		}
		return false;

	}	else { // 前进的方向什么都没有
		mUser->setXY(ux, uy);
		return true;
	}
}

/*
 * 根据坐标x和y找到是哪个箱子
 */
Tile *Game::findWhichBox(int x, int y) {
	Tile *tmp = mBoxs;
	while (tmp) {
		if (tmp->getX() == x && tmp->getY() == y)
			return tmp;
		tmp = tmp->mNext;
	}
	return NULL;
}

/**
处理键盘按下事件
*/
void Game::handleKeyPress(const SDL_Event &event) {
	//cout << "handle key pressed " << endl;
	switch (event.key.keysym.sym) {
	case SDLK_UP:
		userMove(-1, 0);
		cout << "up" << endl;
		break;
	case SDLK_DOWN:
		userMove(1, 0);
		cout << "down" << endl;
		break;
	case SDLK_LEFT:
		userMove(0, -1);
		cout << "left" << endl;
		break;
	case SDLK_RIGHT:
		userMove(0, 1);
		cout << "right" << endl;
		break;
	}

}

void Game::paintTxt(std::string str, int x, int y, int size, SDL_Color color) {
	if (TTF_Init() < 0) {
		cout << "init failed..." << endl;
		return;
	}
	
	TTF_Font *font = TTF_OpenFont("res/font.ttf", size);//第二个参数表示字体的大小
	if (!font) {
		cout << "font is null" << endl;
		return;
	}
	//SDL_Color color = { 0, 0, 0 };
	cout << "str:" << str.c_str() << endl;
	SDL_Surface *surf = TTF_RenderText_Solid(font, str.c_str(), color);
	//表面转纹理
	SDL_Texture *text = SDL_CreateTextureFromSurface(mRender, surf);

	SDL_Rect rect = { x, y, surf->w, surf->h };
	cout << "绘制文字。。。。" << endl;
	SDL_RenderCopy(mRender, text, NULL, &rect);
	//释放资源
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	TTF_Quit();
}

void Game::run() {
	//设置退出事件
	SDL_bool quit = SDL_FALSE;

	//整个事件循环
	while (!quit) {
		SDL_Event event;
	
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				//窗口退出事件
			case SDL_QUIT:
				quit = SDL_TRUE;
				break;
				//键盘按下事件
			case SDL_KEYDOWN:
				handleKeyPress(event);
				break;

			}
			paintGame();

		}
	}
}
