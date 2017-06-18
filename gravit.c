//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 1000


#define default_mass_maximum    1.5e4 /* kg */
#define default_domain_size      270 /* m  */


struct Galaxy
{
    float x[N];
    float y[N];
    float z[N];
    float m[N];
    float ax[N];
    float ay[N];
    float az[N];
    int cell[N];
};

struct Galaxy MilkWay;

struct Quad
{
    float x;
    float y;
    float z;
    float w;
    float h;
    float l;
    float m;
    int body;
};

struct QuadTree
{
    struct Quad nw;
    struct Quad ne;
    struct Quad sw;
    struct Quad se;

    struct QuadTree *child;
};

struct QuadTree RootNode;

struct QuadTree ChildNodes[N];
int childNodeCount = 0;



//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
//The window we'll be rendering to
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* screenSurface = NULL;
SDL_Renderer* gRenderer = NULL;
int quit = 0;
SDL_Event e;

int i, j;
float EPS = 1;
float rx, ry, rz;
float d, f;
float G = 6.670e-11;

float deg2Rad = M_PI/180.0f;
void initializ()
{
    float r,x,y;

    genBHunt();
    for (i = 1; i < N; i++)
    {
        r = default_domain_size * (float)(random() / ((float)RAND_MAX + 1.0));
        x = r*cosf(i);
        y = r*sinf(i);

        MilkWay.x[i] =x+400;
        MilkWay.y[i] = y+300;
        MilkWay.z[i] = 0;
        MilkWay.m[i] = 1e5;

        MilkWay.ax[i] = 0.0f;
        MilkWay.ay[i] = 0.0f;
        MilkWay.az[i] = 0.0f;
        MilkWay.cell[i] = -1;

    }

}

void genBHunt()
{
    int w = SCREEN_WIDTH/2;
    int h = SCREEN_HEIGHT/2;
    RootNode.nw.x = 0;
    RootNode.nw.y = 0;
    RootNode.nw.w = w+1;
    RootNode.nw.h = h+1;
    RootNode.nw.l = 0;
    RootNode.nw.body = -1;

    RootNode.ne.x = w;
    RootNode.ne.y = 0;
    RootNode.ne.w = w;
    RootNode.ne.h = h+1;
    RootNode.ne.l = 0;
    RootNode.ne.body = -1;

    RootNode.se.x = 0;
    RootNode.se.y = h;
    RootNode.se.w = w+1;
    RootNode.se.h = h;
    RootNode.se.l = 0;
    RootNode.ne.body = -1;

    RootNode.sw.x = w;
    RootNode.sw.y = h;
    RootNode.sw.w = w;
    RootNode.sw.h = h;
    RootNode.sw.l = 0;
    RootNode.sw.body = -1;

    RootNode.child = &ChildNodes[childNodeCount];
    childNodeCount++;
};

void drawQuadTree(struct QuadTree _qt)
{
    SDL_Rect rect;
    rect.x = _qt.nw.x;
    rect.y = _qt.nw.y;
    rect.w = _qt.nw.w;
    rect.h = _qt.nw.h;

    SDL_RenderDrawRect(gRenderer,&rect);

    rect.x = _qt.ne.x;
    rect.y = _qt.ne.y;
    rect.w = _qt.ne.w;
    rect.h = _qt.ne.h;

    SDL_RenderDrawRect(gRenderer,&rect);

    rect.x = _qt.sw.x;
    rect.y = _qt.sw.y;
    rect.w = _qt.sw.w;
    rect.h = _qt.sw.h;

    SDL_RenderDrawRect(gRenderer,&rect);

    rect.x = _qt.se.x;
    rect.y = _qt.se.y;
    rect.w = _qt.se.w;
    rect.h = _qt.se.h;

    SDL_RenderDrawRect(gRenderer,&rect);
}



void FillTree()
{
    struct QuadTree *currentNode = &RootNode;
    for (i = 0;i<N;i++)
    {
        if (MilkWay.cell[N] < 0)
        {

        }
    }
}

float dt = 1/1.0f;
void computeForces()
{


    for (i = 0; i < N; i++)
    {

        for (j = 0; j < N; j++)
        {
            if (j != i)
            {
                rx = MilkWay.x[j] - MilkWay.x[i];
                ry = MilkWay.y[j] - MilkWay.y[i];
                //rz = p[j].z - p[i].z;

                d = sqrt(rx*rx + ry*ry)+1;

                f = (G * MilkWay.m[j] * MilkWay.m[i]) / (d*d);


                MilkWay.ax[i] += (f * rx /d)*dt;
                MilkWay.ay[i] += (f * ry /d)*dt;
                //MilkWay.az[i] += (f * rz /d)*dt;

            }
        }



    }

}

void applyForces()
{
    for (i = 0; i < N; i++)
    {
        MilkWay.x[i] += MilkWay.ax[i];
        MilkWay.y[i] += MilkWay.ay[i];
        //MilkWay.z[i] += MilkWay.az[i]

    }

}



void mainLoop()
{

    int px,py;
    computeForces();
    applyForces();
    //computeAABB();

    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear( gRenderer );


    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    drawQuadTree(RootNode);



    for (i = 0; i < N; i++)
    {
        px = (int)MilkWay.x[i];
        py = (int)MilkWay.y[i];
        SDL_RenderDrawPoint(gRenderer,px,py);
    }


    SDL_RenderPresent( gRenderer );




}
int main( int argc, char* args[] )
{

    initializ();


    //Initialize SDL

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        //Create window
        window = SDL_CreateWindow( "Gravidade", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get window surface
            screenSurface = SDL_GetWindowSurface( window );
            gRenderer = SDL_GetRenderer(window);
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );

            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );



            }
            while(quit == 0)
            {
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = 1;
                    }
                }

                mainLoop();

            }


            //Wait two seconds

        }
    }

    //Destroy window
    SDL_DestroyWindow( window );

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}
