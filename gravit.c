//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//power of two
#define N 1024

#define default_mass_maximum    1.5e3 /* kg */
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

float dt = 1 / 30.0;
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

                d = sqrt(rx * rx + ry * ry + EPS);

                f = (G * MilkWay.m[j] * MilkWay.m[i]) / (d*d);

                MilkWay.ax[i] += (f * rx) * dt;
                MilkWay.ay[i] += (f * ry) * dt;
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

    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear( gRenderer );

    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

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
