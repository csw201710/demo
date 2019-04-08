//将SDL函数和数据类型包含进来

#define LOAD_JPG

#include "SDL.h"
#include "SDL_image.h" 
#include <windows.h>
#include <string>


#define SESSION2


#ifdef SESSION2

//窗口属性
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

SDL_Surface *load_image( std::string filename ) ;
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
#endif

#ifdef SESSION3



#endif
int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
#ifdef SESSION1
        //声明表面
        SDL_Surface* hello = NULL;
        SDL_Surface* screen = NULL;
        //启动SDL
        SDL_Init( SDL_INIT_EVERYTHING );
     
        //设置窗口
        screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );
     
        //加载图像
        hello = SDL_LoadBMP( "1.bmp" );
        //将图像应用到窗口上
        SDL_BlitSurface( hello, NULL, screen, NULL );
     
        //更新窗口
        SDL_Flip( screen );
     
        //暂停
        SDL_Delay( 2000 );
    //释放已加载的图像
    SDL_FreeSurface( hello );
 
    //退出SDL
    SDL_Quit();
#elif defined SESSION2
    //将要使用的表面
    SDL_Surface *message = NULL;
    SDL_Surface *background = NULL;
    SDL_Surface *screen = NULL;
        //初始化SDL的所有子系统
        if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        {
            return 1;    
        }
        //设置窗口
        screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
        //如果设置窗口时出现错误
        if( screen == NULL )
        {
            return 1;    
        }
        //设置窗口标题
        SDL_WM_SetCaption( "Hello World", NULL );
        //加载图片
        message = load_image( "2.bmp" );
        background = load_image( "1.bmp" ); //
        //将背景图片background应用到screen上
        apply_surface( 0, 0, background, screen );
        apply_surface( 320, 0, background, screen );
        //apply_surface( 0, 240, background, screen );
        //apply_surface( 320, 240, background, screen );

        //将message表面应用到窗口中
        apply_surface( 180, 140, message, screen );

        //更新窗口
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;    
        }
#if 0
        //等待2秒
        SDL_Delay( 2000 );

#else
		//将要用到的事件结构体
		SDL_Event event;
		 bool quit = false;
		//当用户还不想退出时
        while( quit == false )
        {
            //当有事件发生时，我们需要处理它们
            while( SDL_PollEvent( &event ) )
            {
				//如果用户点击了窗口右上角的关闭按钮
				if( event.type == SDL_QUIT )
				{
					//退出程序
					quit = true;
				}    
			}
		}
#endif
		//释放表面
		SDL_FreeSurface( message );
		SDL_FreeSurface( background );
	    
		//退出SDL
		SDL_Quit();
#elif defined SESSION3

#endif
		return 0;
}

#ifdef SESSION2
SDL_Surface *load_image( std::string filename ) 
{
#if 0
    //临时的空间，用于存储刚刚加载好的图像
    SDL_Surface* loadedImage = NULL;
    
    //优化后的图像，实际使用的是这个图像
    SDL_Surface* optimizedImage = NULL;
        //加载图像
	loadedImage = SDL_LoadBMP( filename.c_str() );
    //如果加载图片没有出错
    if( loadedImage != NULL )
    {
        //创建一个优化了的图像
        optimizedImage = SDL_DisplayFormat( loadedImage );
        
        //释放临时的图像
        SDL_FreeSurface( loadedImage );
    }

    //返回优化后的表面
    return optimizedImage;
#else
   //加载的图像
    SDL_Surface* loadedImage = NULL;
    
    //优化后的图像
    SDL_Surface* optimizedImage = NULL;
    
    //使用SDL_image加载图像
    loadedImage = IMG_Load( filename.c_str() );
	//SDL_RWops *rwops = SDL_RWFromFile(filename.c_str(),"rb");
    //loadedImage = IMG_LoadJPG_RW(rwops);
    //如果图像加载成功
    if( loadedImage != NULL )
    {
        //创建一个优化后的图像
        optimizedImage = SDL_DisplayFormat( loadedImage );
        
        //释放原先加载的图像
        SDL_FreeSurface( loadedImage );

        //如果图像优化成功
        if( optimizedImage != NULL )
        {
            //映射关键色
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0, 0 );
            //将所有颜色为（R 0, G 0xFF, B 0xFF）的像素设为透明。
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }
    }
    
    //返回优化后的图像
    return optimizedImage;
#endif
}


void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //新建一个临时的矩形来保存偏移量
    SDL_Rect offset;
    
    //将传入的偏移量保存到矩形中
    offset.x = x;
    offset.y = y;
    //执行表面的Blit
    SDL_BlitSurface( source, NULL, destination, &offset );
}
#endif


#ifdef SESSION3

#endif



