/*
PROTEUS MAZE GAME
by Gent Semaj and Liam Kerry
12/04/2019 (repo 07/06/2020)
*/

#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHSD.h>
#include <FEHRandom.h>

#define TILESIZE 20
#define MAZEMAX 30

// Tile definitions
#define FINISH 4
#define START 3
#define FLOOR 1
#define WALL 2
#define SPACE 0

// FUNDAMENTAL: User-defined Function
int getHorizontalLength();
int getVerticalLength();

/* Maze map class */
class Maze // FUNDAMENTAL: At least one class/object
{
public:
    Maze(const char *); // constructor takes file name
    int tiles[MAZEMAX][MAZEMAX]; // tiles are stored as integers

    // practical dimensions
    int width;
    int height;
};

void start(Maze *);

class Player
{
public:
    Player(Maze *); // constructor
    bool move(int);
    int x; // x position
    int y; // y position
    int dir; // what direction is being faced
    Maze *map; // maze pointer
};

void paintScreen(Player *);

/* Maze constructor */
Maze::Maze(const char *fname) // param: maze file name
{
    LCD.Write("Loading ");
    LCD.Write(fname);
    LCD.Write("...");

    // Open the maze file
    FEHFile *fptr = SD.FOpen(fname, "r"); // FUNDAMENTAL: Operators - relational

    int w, h; // maze practical dimensions
    SD.FScanf(fptr, "%i", &w); // read width
    SD.FScanf(fptr, "%i", &h); // read height

    // save practical dimensions
    width = w;
    height = h;

    // initialize tilemap
    for(int i = 0; i < MAZEMAX; i++)
        for(int j = 0; j < MAZEMAX; j++)
            tiles[i][j] = SPACE;

    int tile; // temporary value variable
    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            SD.FScanf(fptr, "%d", &tile); // read tile value from file
            tiles[i][j] = tile; // assign value to proper spot in tilemap
        }
    }

    // Close the maze file
    SD.FClose(fptr);
    LCD.WriteLine("Done");
}

/* Player constructor */
Player::Player(Maze *m)
{
    map = m;
    dir = 0;

    // find start tile
    for(int i = 0; i < m->width; i++)
    {
        for(int j = 0; j < m->height; j++)
        {
            // FUNDAMENTAL: Selection Structures - if/else
            if(m->tiles[i][j] == START) // FUNDAMENTAL: Operators - logical
            {
                x = i;
                y = j;
            }
        }
    }
}

/* Move the player in a direction */
bool Player::move(int moveDir) // parameter: desired direction; return: whether game is over
{
    // update the player's direction
    if (dir != moveDir)
        dir = moveDir;

    switch (dir)
    {
    case 0: // up
        if(map->tiles[x][y-1] != WALL && map->tiles[x][y-1] != SPACE) // collision detection
            y--;
        break;
    case 1: // right
        if(map->tiles[x+1][y] != WALL && map->tiles[x+1][y] != SPACE)
            x++;
        break;
    case 2: // down
        if(map->tiles[x][y+1] != WALL && map->tiles[x][y+1] != SPACE)
            y++;
        break;
    case 3: // left
        if(map->tiles[x-1][y] != WALL && map->tiles[x-1][y] != SPACE)
            x--;
        break;
    }

    return (map->tiles[x][y] == FINISH);
}

int main(void)
{
    // Initial LCD screen sclear
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);

    // Load mazes
    Maze easy[] { Maze("easy/1.txt"),  Maze("easy/2.txt"),  Maze("easy/3.txt"),  Maze("easy/4.txt"),  Maze("easy/5.txt"),  Maze("easy/6.txt"),  Maze("easy/7.txt") };
    Maze medium[] { Maze("medium/1.txt"),  Maze("medium/2.txt"),  Maze("medium/3.txt"),  Maze("medium/4.txt")};
    Maze hard[] { Maze("hard/1.txt"),  Maze("hard/2.txt")};

    float xt, yt; // variables for position of touch
    float xtr, ytr; // trash variables for release
    bool exit = false; // loop controller
    int menu = 0; // menu state
    int oldmenu = 0;
    while(!exit) // main game loop
    {
        // Display the main menu

        /* DEPRECATED
        if(menu != oldmenu) // Don't clear the screen unless the menu changed
        {
            LCD.Clear(BLACK);
            LCD.SetFontColor(WHITE);
            oldmenu = menu;
        }
        */

        int rand = Random.RandInt(); // generate random integer for maze load

        LCD.Clear(BLACK);
        LCD.SetFontColor(WHITE);
        switch(menu)
        {
        case 0: // top-level menu
            // Draw the menu elements
            LCD.FillRectangle(30,15,260,60);
            LCD.FillRectangle(40,120,100,40);
            LCD.FillRectangle(180,120,100,40);
            LCD.FillRectangle(110,180,100,40);
            LCD.SetFontColor(BLUE);
            LCD.FillRectangle(40,20,15,50);
            LCD.FillRectangle(80,20,15,50);
            LCD.FillRectangle(155,20,45,13);
            LCD.FillRectangle(155,57,45,13);
            LCD.FillRectangle(210,20,20,50);
            LCD.FillRectangle(230,20,25,15);
            LCD.FillRectangle(230,40,25,10);
            LCD.FillRectangle(230,55,25,15);
            LCD.FillRectangle(260,20,20,35);
            LCD.FillRectangle(260,60,20,10);
            LCD.SetBackgroundColor(WHITE);
            LCD.SetFontColor(BLACK);
            LCD.WriteAt("Start",50,130);
            LCD.WriteAt("Rules",190,130);
            LCD.WriteAt("Quit",120,190);
            while(!LCD.Touch(&xt, &yt)); // wait for touch
            while(LCD.Touch(&xtr, &ytr)); // wait for release
            // check which button was pressed
            if ((xt >= 40 && xt <= 140) && (yt >= 120 && yt <= 160)) menu = 1; // go to the difficulty selection menu
            else if ((xt >= 180 && xt <= 280) && (yt >= 120 && yt <= 160)) menu = 2; // go to the instructions menu
            else if ((xt >= 110 && xt <= 210) && (yt >= 180 && yt <= 220)) exit = true; // stop the game loop
            break;
        case 1: // difficulty selection menu
            // Draw the menu elements
            LCD.FillRectangle(10,10,60,30);
            LCD.FillRectangle(100,80,120,30);
            LCD.FillRectangle(100,120,120,30);
            LCD.FillRectangle(100,160,120,30);
            LCD.SetBackgroundColor(WHITE);
            LCD.SetFontColor(BLACK);
            LCD.WriteAt("Back",20,15);
            LCD.WriteAt("Easy",110,85);
            LCD.WriteAt("Medium",110,125);
            LCD.WriteAt("Hard",110,165);
            while(!LCD.Touch(&xt, &yt)); // wait for touch
            while(LCD.Touch(&xtr, &ytr)); // wait for release
            // check which button was pressed
            if ((xt >= 10 && xt <= 70) && (yt >= 10 && yt <= 40)) menu = 0; // return to the top-level menu
            else if ((xt >= 100 && xt <= 220) && (yt >= 80 && yt <= 110)) start(&easy[rand % 7]); // start the easyt maze
            else if ((xt >= 100 && xt <= 220) && (yt >= 120 && yt <= 150)) start(&medium[rand % 4]); // start the medium maze
            else if ((xt >= 100 && xt <= 220) && (yt >= 160 && yt <= 190)) start(&hard[rand % 2]); // start the hard maze
            break;
        case 2: // instructions menu
            // Draw the menu elements
            LCD.WriteAt("Instructions:",20,40);
            LCD.WriteLine("Using the left, right, upper, and lower sections of");
            LCD.WriteLine("the proteus touchscreen, move your character through");
            LCD.WriteLine("the maze to the end square, avoiding traps and dead ends");
            LCD.SetFontColor(WHITE);
            LCD.FillRectangle(10,10,60,30);
            LCD.SetBackgroundColor(WHITE);
            LCD.SetFontColor(BLACK);
            LCD.WriteAt("Back",20,15);
            while(!LCD.Touch(&xt, &yt)); // wait for touch
            while(LCD.Touch(&xtr, &ytr)); // wait for release
            // check if the button was pressed
            if((xt >= 10 && xt <= 70) && (yt >= 10 && yt <= 40)) menu = 0; // return to the top-level menu
            break;
        }
    }

    LCD.Clear(BLACK);
    LCD.WriteLine("Please restart the proteus");

    return 0;
}

void start(Maze *maze)
{
    // The player
    Player p(maze);

    paintScreen(&p); // initial screen paint

    float xt, yt; // variables for position of touch
    float xtr, ytr; // trash variables for release
    bool done = false; // whether or not the maze has been completed
    float timeStart = TimeNow();
    while(!done)
    {
        while(!LCD.Touch(&xt, &yt)); // wait for touch
        while(LCD.Touch(&xtr, &ytr)); // wait for release

        // Move the player in the direction of the touch
        // Controls are 40 px-wide bounds on the inside borders of the screen
        if(yt <= 40) done = p.move(0);
        else if(yt >= 200) done = p.move(2);
        else if(xt <= 40) done = p.move(3);
        else if(xt >= 280) done = p.move(1);

        paintScreen(&p); // refresh screen
    }

    // Calculate how long it took to finish the maze
    float timeTot = TimeNow() - timeStart;
    // Write win message to the screen
    LCD.Clear(BLACK);
    LCD.WriteLine("Congratulations!");
    LCD.WriteLine("You solved the maze!");
    LCD.Write("Your time was ");
    LCD.Write((int)(timeTot) / 60);
    LCD.Write(":");
    LCD.WriteLine((int)(timeTot) % 60);
    LCD.WriteLine("Returning to menu in 5 seconds...");
    Sleep(5.0); // sleep for 5 secs
}

/* Draw the current frame on the Proteus screen */
void paintScreen(Player *p) // param: pointer to player
{
    Maze *m = p->map;
    // Clear the screen
    LCD.Clear(BLACK);
    // Temporary tile position variables
    int drawX;
    int drawY;
    // Loop through visible tiles
    for(int i = p->x - getHorizontalLength() / 2; i <= p->x + getHorizontalLength() / 2; i++)
    {
        for(int j = p->y - getVerticalLength() / 2; j <= p->y + getVerticalLength() / 2; j++)
        {
            if(i >= 0 && i < m->width && j >= 0 && j < m->height) // check if tile exists
            {
                // change tile color based on tile value
                LCD.SetFontColor(BLACK);
                switch(m->tiles[i][j])
                {
                case WALL:
                    LCD.SetFontColor(RED);
                    break;
                case FLOOR:
                    LCD.SetFontColor(LIGHTGRAY);
                    break;
                case START:
                    LCD.SetFontColor(GREEN);
                    break;
                case FINISH:
                    LCD.SetFontColor(YELLOW);
                    break;
                }

                // set coordinates for the corner of the tile
                drawX = (i - p->x) * TILESIZE + 160 - (TILESIZE / 2);
                drawY = (j - p->y) * TILESIZE + 120 - (TILESIZE / 2);

                LCD.FillRectangle(drawX, drawY, TILESIZE, TILESIZE); // fill the rectangle
            }
        }
    }

    // Draw the player sprite
    LCD.SetFontColor(VIOLET);

    // point constants
    int pxl = 159 - TILESIZE/2 + 2; // low x
    int pxm = 159; // mid x
    int pxh = 159 + TILESIZE/2 - 2; // high x
    int pyl = 119 - TILESIZE/2 + 2; // low y
    int pym = 119; // mid y
    int pyh = 119 + TILESIZE/2 + 2; // high y

    // fill a lil square
    LCD.FillRectangle(pxm - 4, pym - 4, 8, 8);

    // switch based on direction
    switch(p->dir)
    {
    case 0: // up
        // draw three lines between the three points of the triangle
        LCD.DrawLine(pxm, pyl, pxh, pyh);
        LCD.DrawLine(pxh, pyh, pxl, pyh);
        LCD.DrawLine(pxl, pyh, pxm, pyl);
        break;
    case 2: // down
        LCD.DrawLine(pxm, pyh, pxh, pyl);
        LCD.DrawLine(pxh, pyl, pxl, pyl);
        LCD.DrawLine(pxl, pyl, pxm, pyh);
        break;
    case 1: // right
        LCD.DrawLine(pxh, pym, pxl, pyl);
        LCD.DrawLine(pxl, pyl, pxl, pyh);
        LCD.DrawLine(pxl, pyh, pxh, pym);
        break;
    case 3: // left
        LCD.DrawLine(pxl, pym, pxh, pyl);
        LCD.DrawLine(pxh, pyl, pxh, pyh);
        LCD.DrawLine(pxh, pyh, pxl, pym);
        break;
    }
}

/* Screen width in tiles */
int getHorizontalLength()
{
    return 320 / TILESIZE;
}

/* Screen height in tiles */
int getVerticalLength()
{
    return 240 / TILESIZE;
}
