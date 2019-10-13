#include <iostream>
#include <time.h>
#include <conio.h>
#include <windows.h>
using namespace std;

void ShowConsoleCursor(bool showFlag);
void gotoxy(int x, int y);
void printFrame(int x, int y, int w, int h);
int playerScore = 0;
int enemyScore = 0;


struct Frame
{
    int width = 60;
    int height = 20;
    int x = 1;
    int y = 1;
} frame;

class Player
{
public:

    float x = 0;
    float y = 0;
    float oldx = 0;
    float oldy = 0;
    int goUp = 0;
    int goDown = 0;
    Player()
    {
        x = 3.0f;
        y = 10.0f;
    }

    int isColide(float x1, float y1)
    {
        for(int c = 1; c <= 5; c++)
        {
            if((int)x1 == (int)(x)  && (int)y1 == (int)(y)+c)
            {
                return 1;
            }
        }
        return 0;
    }

    void update()
    {
        if(goUp == 1)
        {
            goUp = 0;
            if(y > frame.y)
            {
                y--;
            }
        }
        if(goDown == 1)
        {
            goDown = 0;
            if(y < frame.y+frame.height-7)
            {
                y++;
            }
        }
    }

    void render()
    {
        char code = '@';
        char space = ' ';
        // clear old
        for(int c = 1; c <= 5; c++){
            gotoxy(oldx+0,oldy+c);
            cout << space;
        }
        // print new
        for(int c = 1; c <= 5; c++){
            gotoxy(x+0,y+c);
            cout << code;
        }
        oldx = x;
        oldy = y;

    }
};

class Enemy
{
public:
    float x = 0;
    float y = 0;
    float oldx = 0;
    float oldy = 0;
    float ballx = 0;
    float bally = 0;
    float dy = 0.2f;
    Enemy()
    {
        x = frame.width-2;
        y = 3;
    }

    int isColide(float x1, float y1)
    {
        for(int c = 1; c <= 5; c++)
        {
            if((int)x1 == (int)(x)  && (int)y1 == (int)(y)+c)
            {
                return 1;
            }
        }
        return 0;
    }

    void update()
    {
        if(bally > y+3 && y+7 < frame.y+frame.height)
        {
            y+=dy;
        }
        if(bally < y+3 && y > frame.y)
        {
            y-=dy;
        }
    }

    void render()
    {
        char code = '@';
        char space = ' ';
        // clear old
        for(int c = 1; c <= 5; c++){
            gotoxy(oldx+0,oldy+c);
            cout << space;
        }
        // print new
        for(int c = 1; c <= 5; c++){
            gotoxy(x+0,y+c);
            cout << code;
        }
        oldx = x;
        oldy = y;
    }

};
class Ball
{
public:
    float x = 0;
    float y = 0;
    float oldx = 0;
    float oldy = 0;
    float dx = -1;
    float dy = -0.5f;
    int reset = 1;
    Ball()
    {
        x = frame.x+frame.width/2;
        y = frame.y+frame.height/2;
    }

    void update()
    {
        if(reset != 0)
        {
            reset = 0;
            srand(time(NULL));
            x = (frame.x+frame.width/2)-5+rand()%10;
            y = 1+frame.y+rand()%(frame.height-1);
            dx = rand();
            if((int)dx%2 == 0)
            {
                dx = 1;
            }
            else
            {
                dx = -1;
            }
            dy = rand();
            if((int)dy%2 == 0)
            {
                dy = 0.5f;
            }
            else
            {
                dy = -0.5f;
            }

        }
        if( (y+dy > frame.y + frame.height-2) || (y+dy < frame.y+1) )
        {
            dy*=-1;
        }
        x+=dx;
        y+=dy;
    }
    void render()
    {
        gotoxy(oldx,oldy);
        cout << ' ';
        gotoxy(x,y);
        cout << 'o';
        oldx = x;
        oldy = y;
    }
};

class Game
{
public:

    Player player;
    Enemy enemy;
    Ball ball;
    Game()
    {
        player = Player();
        enemy = Enemy();
        ball = Ball();
        printFrame(frame.x,frame.y,frame.width,frame.height);
    }
    void update()
    {
        enemy.ballx = ball.x;
        enemy.bally = ball.y;
        player.update();
        enemy.update();
        ball.update();
        if((int)ball.x <= (int)player.x && player.isColide(ball.x,ball.y) == 0)
        {
            //enemy point
            enemyScore++;
            ball.reset = 1;
            printFrame(frame.x,frame.y,frame.width,frame.height);
        }
        if((int)ball.x >= (int)enemy.x && enemy.isColide(ball.x,ball.y) == 0)
        {
            //player point
            playerScore++;
            ball.reset = 1;
            printFrame(frame.x,frame.y,frame.width,frame.height);
        }
        if(enemy.isColide(ball.x+ball.dx,ball.y) == 1)
        {
            ball.dx*=-1;
        }
        if(player.isColide(ball.x+ball.dx,ball.y) == 1)
        {
            ball.dx*=-1;
        }

    }
    void render()
    {
        player.render();
        enemy.render();
        ball.render();
    }
};



int main()
{
    Game game = Game();
    ShowConsoleCursor(false);
    clock_t stime = clock();
    int time = 0;
    int old_time = 0;
    int FPS = 10;
    SetConsoleTitle("Pong! By Sakamoto");
    int pause = 0;
    while(1)
    {
        time = (clock()-stime)/(CLOCKS_PER_SEC/FPS);
        if(time != old_time)
        {
            old_time = time;
            if(pause == 0){
                game.update();
            }
            game.render();
            gotoxy(1,frame.y+frame.height);
            cout << "Player score: " << playerScore;
            gotoxy(frame.x+frame.width-14,frame.y+frame.height);
            cout << "Enemy score: " << enemyScore;
            if(kbhit())
            {
                int key = getch();
                if(key == 'w')
                {
                    game.player.goUp = 1;
                }
                if(key == 's')
                {
                    game.player.goDown = 1;
                }
                if(key == 27){
                    pause = (pause == 0)?1:0;
                }
            }
        }
    }
    return 0;
}


void printFrame(int x, int y, int w, int h)
{
    for(int xx = 0; xx < w; xx++)
    {
        for(int yy = 0; yy < h; yy++)
        {
            gotoxy(x+xx,y+yy);
            if(xx == 0 || xx == w-1)
            {
                cout << "@";
            }
            else if(yy == 0 || yy == h-1)
            {
                cout << "@";
            }
        }
    }
}

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
