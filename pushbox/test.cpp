#include <SDL.h>
#include <iostream>
#include "core.h"
#include <SDL_image.h>

using namespace std;

#define DESKTOPW 800
#define DESKTOPH 600


int test()
{
	
	//启动SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	//创建一个窗口
	SDL_Window *win = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		DESKTOPW, DESKTOPH, SDL_WINDOW_SHOWN);
	/**
	  创建一个渲染器,所有的画线、画圆、画矩形都在这个render上面画
	*/
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//填充颜色
	SDL_SetRenderDrawColor(ren, 200, 200, 100, 0);
	//画线
	SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(ren, 64, 62, 300, 200);
	//加载一个图片
	std::string imagePath = "res/Wall_Black.png";
	std::string imagePath1 = "res/EndPoint_Red.png";
	SDL_Surface * wall = IMG_Load(imagePath.c_str());
	SDL_Surface * pit = IMG_Load(imagePath1.c_str());
	SDL_Texture * wallTex = SDL_CreateTextureFromSurface(ren, wall);
	SDL_Texture * wallTex1 = SDL_CreateTextureFromSurface(ren, pit);
	SDL_FreeSurface(wall);
	
	//设置要复制的区域以及墙的大小
	SDL_Rect wallRect;
	wallRect.w = 64;
	wallRect.h = 64;
	wallRect.x = 0;
	wallRect.y = 0;

	SDL_Rect dest;
	dest.w = 64;
	dest.h = 64;
	dest.x = 0;
	dest.y = 0;

	//清除渲染器
	SDL_RenderClear(ren);
	//画texture到渲染器上
	//SDL_RenderCopy(ren, tex, NULL, NULL);
	/*
	  左边的Rect是图片的，右边的Rect要绘制的Render的
	 */
	SDL_RenderCopy(ren, wallTex,  &wallRect, &dest );
	//SDL_RenderClear(ren);
	//SDL_RenderPresent(ren);
	dest.x = 64;
	dest.y = 64;
	SDL_RenderCopy(ren, wallTex1, &wallRect, &dest);
	
	//更新屏幕
	SDL_RenderPresent(ren);

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
			//	handleKeyPress(event);
				break;

			}
				
		}
		//cout << "exit window!!!" << endl;
	}

	//SDL_PollEvent(&event);
	//资源释放
	//SDL_DestroyTexture(tex);
	SDL_DestroyTexture(wallTex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
	
}
