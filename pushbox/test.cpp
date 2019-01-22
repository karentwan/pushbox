#include <SDL.h>
#include <iostream>
#include "core.h"
#include <SDL_image.h>

using namespace std;

#define DESKTOPW 800
#define DESKTOPH 600


int test()
{
	
	//����SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	//����һ������
	SDL_Window *win = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		DESKTOPW, DESKTOPH, SDL_WINDOW_SHOWN);
	/**
	  ����һ����Ⱦ��,���еĻ��ߡ���Բ�������ζ������render���滭
	*/
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//�����ɫ
	SDL_SetRenderDrawColor(ren, 200, 200, 100, 0);
	//����
	SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(ren, 64, 62, 300, 200);
	//����һ��ͼƬ
	std::string imagePath = "res/Wall_Black.png";
	std::string imagePath1 = "res/EndPoint_Red.png";
	SDL_Surface * wall = IMG_Load(imagePath.c_str());
	SDL_Surface * pit = IMG_Load(imagePath1.c_str());
	SDL_Texture * wallTex = SDL_CreateTextureFromSurface(ren, wall);
	SDL_Texture * wallTex1 = SDL_CreateTextureFromSurface(ren, pit);
	SDL_FreeSurface(wall);
	
	//����Ҫ���Ƶ������Լ�ǽ�Ĵ�С
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

	//�����Ⱦ��
	SDL_RenderClear(ren);
	//��texture����Ⱦ����
	//SDL_RenderCopy(ren, tex, NULL, NULL);
	/*
	  ��ߵ�Rect��ͼƬ�ģ��ұߵ�RectҪ���Ƶ�Render��
	 */
	SDL_RenderCopy(ren, wallTex,  &wallRect, &dest );
	//SDL_RenderClear(ren);
	//SDL_RenderPresent(ren);
	dest.x = 64;
	dest.y = 64;
	SDL_RenderCopy(ren, wallTex1, &wallRect, &dest);
	
	//������Ļ
	SDL_RenderPresent(ren);

	//�����˳��¼�
	SDL_bool quit = SDL_FALSE;
	

	//�����¼�ѭ��
	while (!quit) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				//�����˳��¼�
			case SDL_QUIT:
				quit = SDL_TRUE;
				break;

				//���̰����¼�
			case SDL_KEYDOWN:
			//	handleKeyPress(event);
				break;

			}
				
		}
		//cout << "exit window!!!" << endl;
	}

	//SDL_PollEvent(&event);
	//��Դ�ͷ�
	//SDL_DestroyTexture(tex);
	SDL_DestroyTexture(wallTex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
	
}
