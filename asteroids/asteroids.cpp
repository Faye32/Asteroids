/* write asteroids using trig for rendering

    **** NOTES ****
add a gravity mode?
idk how any of this works btw, i wrote 80% of it when i was either half asleep or adhd

https://stackoverflow.com/questions/30787335/segmentation-fault-using-vector-c <-- still couldnt fix this btw

*/

#include <iostream>
#include <math.h>
#include <array>
#include <ctime>
#include <fstream>
#include <string>
#include "raylib.h"

using namespace std;

//structs
    struct asteroids {
        double X; //x pos
        double Y; //y pos
        double Vel; //velocity
        double Rot; //radian rotation value
        bool isOn = false; //checks if on or off
    }ast[32];

    struct projectile {
        double X = 1000;
        double Y = 1000;
        double Vel;
        double Rot;
    }shot[10];


//vector functions
double toX(double length, double angle) {
    //returns the x coordinate given a vector length and angle
    double x = length * (cos(angle));
    return x;
}

double toY(double length, double angle) {
    //returns the y coordinate given a vector length and angle
    double y = length * (sin(angle));
    return y;
}

double toVectorAngle(double X, double Y) { //finds the angle between xy and the origin
    return atan((X/Y));
}

double toVectorLength(double X, double Y) { //finds the distance between xy and the origin
    return sqrt(pow(X, 2) + pow(Y, 2));
}

double distance(double X1, double Y1, double X2, double Y2) {
    return sqrt(pow((X1 - X2), 2) + pow((Y1 - Y2), 2));
}

//render functions
void render(double X, double Y, double rot, double vectordata[], int sizeOfData) {
    //render all lines from points 0 to max
    for (int i = 0; i < sizeOfData - 2; i += 2) {
        DrawLine(toX(vectordata[i + 1], (vectordata[i] + rot)) + X, (toY(vectordata[i + 1], (vectordata[i] + rot)) + Y), toX(vectordata[i + 3], (vectordata[i + 2] + rot)) + X, toY(vectordata[i + 3], (vectordata[i + 2] + rot)) + Y, GREEN);
    } //render a line from max back to zero
    DrawLine((toX(vectordata[sizeOfData - 1], (vectordata[sizeOfData - 2] + rot)) + X), toY(vectordata[sizeOfData - 1], (vectordata[sizeOfData - 2] + rot)) + Y , toX(vectordata[1], (vectordata[0] + rot)) + X, toY(vectordata[1], (vectordata[0] + rot)) + Y, GREEN);
}

//main menu func
int mainMenu(void) {
    int frametime = 0;
        int starsX[300];
        int starsY[300];
        bool gameshouldstart = false;

        //randomly generate stars
        for (int i = 0; i < 300; i++) {
            starsX[i] = GetRandomValue(0, 800);
            starsY[i] = GetRandomValue(0, 800);
        }

        while(!gameshouldstart) {
                
            //clear
                BeginDrawing();
                ClearBackground(BLACK);
                
                //render stars
                    for (int i = 0; i < 300; i++) {
                        DrawPixel(starsX[i], starsY[i], WHITE);
                    }

                //render text
                    //title text
                    DrawText("ASTEROIDS", 205, 50, 60, RED);
                    DrawText("ASTEROIDS", 210, 55, 60, ORANGE);
                    DrawText("ASTEROIDS", 215, 60, 60, YELLOW);
                    DrawText("ASTEROIDS", 220, 65, 60, GREEN);
                    //menu text
                    if (frametime%100 > 40 && frametime%100 < 90)
                    DrawText("PRESS ENTER", 270, 400, 30, GREEN);

                EndDrawing();

                //if window should close exit the program
                if (WindowShouldClose())
                    exit(0);
                
            frametime++;

            //if game should start
            if (IsKeyDown(KEY_ENTER)) {
                if (frametime > 10)
                    gameshouldstart = true;
            }
        }

    return 0;
}


int main(void) {
//render data
    const short int screenwidth = 800;
    const short int screenheight = 600;
    const short int infopanelwidth = 200;

//create a window
InitWindow(screenwidth, screenheight, "ASTEROIDS");

SetTargetFPS(60);


//start
START: //place to return to when game over && enter is pressed


    //highscore vars
    const char* highscore = "highscores";
    int topscores[5] = {0,0,0,0,0};
    int shifter[5] = {0,0,0,0,0};

    ifstream fileIN(highscore,ios::binary); //open file for reading as binary, take the top 5 scores
        for (int i = 0; i < 5; i++) {
            if (!fileIN.eof()) {
                fileIN.read((char*)&topscores[i], sizeof(topscores[i])); //get 5 high scores
            }
        }

    fileIN.seekg(0); //reset pos

        for (int i = 0; i < 5; i++) {
            if (!fileIN.eof()) {
                fileIN.read((char*)&shifter[i], sizeof(shifter[i])); //get 5 high scores
            }
        }

    fileIN.close();

    //display top scores DEBUG
    cout << "highscores\n";
    for (int i = 0; i < 5; i++) {
        cout << topscores[i] << "\n";
    } 




    
    mainMenu(); //start the main menu
    //when done return

    //data
        //player vars
            int keypress = 0;
            const double rotAccel = PI / 64; //rotational accelration constant

            //ship data
                double ship[8];  //ship verticies, contain x and y
                double shipvect[8]; //ship vectors from origin, contain length and rotations
                double dir = 0; //radian rot value 
                double velocityX = 0; //xy velocity components
                double velocityY = 0;
                double shipoffsetX = 300, shipoffsetY = 300; //starting pos and offset
        
        //asteroid data  JW MAKE THIS AN ARRAY OF 16 ASTEROID OBJECT
            
            //ast[0].X;     //would access the x var of asteroid 0
            //ast.erase(ast.begin() + 0);     //would erase the entire struct, useful for deleting asts


        //gamedata
            int difficulty = 4;
            bool gameover = false;
            int lives = 3;
            int score = 0;
            bool paused = false;

        //vector data
            //player vector data JW CLEVER GIRL
                //lengths
                for (int i = 1; i < 8; i += 2) {
                    shipvect[i] = 10;
                }
            
                shipvect[5] = 4;
            
                //angles
                shipvect[0] = 0;
                shipvect[2] = PI * 3/4;
                shipvect[4] = PI;
                shipvect[6] = PI * 5/4;

            //asteroid vector data
                //small
                    double smallast[14];
                    //lengths
                        smallast[1] = 10;
                        smallast[3] = 10;
                        smallast[5] = 5;
                        smallast[7] = 10;
                        smallast[9] = 10;
                        smallast[11] = 12;
                        smallast[13] = 10;

                    //angles
                        smallast[0] = 0;
                        smallast[2] = PI / 5;
                        smallast[4] = PI / 3;
                        smallast[6] = PI / 2;
                        smallast[8] = PI;
                        smallast[10] = PI * 6/4;
                        smallast[12] = PI * 7/4;

                //large
                    double largeast[24];
                    //lengths
                        largeast[1] = 13;
                    //angles
                        largeast[0] = 0;

            //shot
            double shotSprite[] = {0, 4, PI, 4};
            int shotcooldown = 0;

        //other
        double vel = 0; //velocity input
        double rotation = 0; //rotation input

        short int starsX[200]; //star pos coords
        short int starsY[200];

        long long int frametime = 0; //used for timing

        double heading; //playerinfo
            
        //set seed accoring to system time
            time_t t;
            SetRandomSeed(t);

        //randomly generate stars
        for (int i = 0; i < 200; i++) {
            starsX[i] = GetRandomValue(0, 600);
            starsY[i] = GetRandomValue(0, 600);
        }
        
        SetTargetFPS(60);

    //intro cutscene
    

    //main game loop
    while (!WindowShouldClose()) {
        //main menu






        //main game loop
        if (lives < 1) {
            gameover = true;        
        }
        
        
        //keypresses
                //rotation
                rotation = 0; //reset rot

                if (IsKeyDown(KEY_A))
                    rotation -= rotAccel; //rotate left  JW ID STORE THESE AS CONSTANTS IF YOU USE THEM MULTIPLE TIMES, FOR EASIER TUNING
                if (IsKeyDown(KEY_D))
                    rotation += rotAccel; //rotate right

                //velocity
                vel = 0; //reset velocity
                if (IsKeyDown(KEY_W))
                    vel += 0.05;    //JW SAME AS W/ ROTATION, HAVE ACCELERATION CONSTANT
                //if (IsKeyDown(KEY_S))
                //    vel -= 0.05;

                //stop movement (USED TO SEE IF IT ADDS ANYTHING TO PLAYER EXPERIENCE)
                if (IsKeyDown(KEY_Q) && IsKeyDown(KEY_E)) {
                    if (velocityX > 0)
                    velocityX -= 0.01;
                    if (velocityY > 0)
                    velocityY -= 0.01;
                    if (velocityX < 0)
                    velocityX += 0.01;
                    if (velocityY < 0)
                    velocityY += 0.01;
                }

            //shot handling
                //if cooldown == 0
                
                shotcooldown--;
                    if (frametime > 10 && !gameover && !paused) {
                    if (IsKeyDown(KEY_ENTER)) {
                        for (int i = 1; i < 10; i++) {
                            if (shotcooldown <= 0) {
                                if (shot[i].X > 600 || shot[i].X < 0 || shot[i].Y > 600 || shot[i].Y < 0) {
                                    shot[i].Rot = dir;
                                    shot[i].Vel = 5;
                                    shot[i].X = shipoffsetX;
                                    shot[i].Y = shipoffsetY;
                                    shotcooldown = 20;
                                }
                            }
                        }
                    }
                    }

                    //pausing
                    if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (paused == true) {
                            paused = false;
                        } else {
                            paused = true;
                        }
                    }

        //asteroid logic
            //update difficulty as needed
                if (difficulty < 16) {
                    if (frametime % 2000 == 0)
                        difficulty++;
                }
            

            //create random asteroids (MAX 16 SMALL, 8 LARGE)
            for (int i = 0; i < difficulty; i++) {
                //check if off
                if (ast[i].isOn == false) {
                    ast[i].Vel = 1;//set velocity
                    ast[i].isOn = true;

                    if (GetRandomValue(0, 40)%2 > 0) { //if value is even
                        ast[i].X = GetRandomValue(-20, 620); //get random x

                        if (GetRandomValue(0, 40)%2 > 0) {
                            ast[i].Y = 600; //set max y
                        
                            ast[i].Rot = (3 * PI) / 2; //set rotation

                        }
                        else {
                            ast[i].Y = 0; //set min y

                            ast[i].Rot = (PI) / 2; //set rotation
                        }
                    }
                    else { //if odd
                    
                            ast[i].Y = GetRandomValue(-20, 620); //generate random y

                            if (GetRandomValue(0, 40)%2 > 0) {
                                ast[i].X = 600;  //set max x

                                ast[i].Rot = PI;
                            }
                            else {
                                ast[i].X = 0; //set min x

                                ast[i].Rot = 0;
                        
                            }

                    }

                    //set the angle of the asteroid given a direct line from its starting pos to the center of the screen with a random xy offset
                    ast[i].Rot = toVectorAngle(ast[i].X - 300, ast[i].Y - 300);
                    }
                }
            
            //if asteroid is on screen update its data if its out of bounds
            for (int i = 0; i < 32; i++) {
                if (ast[i].X > 610)
                    ast[i].X = -10;
                if (ast[i].X < -10)
                    ast[i].X = 610;
                if (ast[i].Y > 610)    
                    ast[i].Y = -10;
                if (ast[i].Y < -10)
                    ast[i].Y = 610;
            }

        if (gameover != true) {
            if (!paused) {
        //physics
            //shot handling
            for (int i = 0; i < 10; i++) { //velocity updates
                shot[i].X += toX(shot[i].Vel, shot[i].Rot);
                shot[i].Y += toY(shot[i].Vel, shot[i].Rot);
            }

            //rotation handling  JW WHY LOOP THIS? FB resolved
                dir += rotation;

            //velocity handling
                //ship
                    //xy components 
                    velocityX += toX(vel, dir);
                    velocityY += toY(vel, dir);
                    //limit vel
                    if (velocityX > 5)
                        velocityX = 5;
                    if (velocityX < -5)
                        velocityX = -5;
                    if (velocityY > 5)
                        velocityY = 5;
                    if (velocityY < -5)
                        velocityY = -5;
                    //write
                    shipoffsetX += velocityX;
                    shipoffsetY += velocityY;

 
                //asteroid 
                    for(int i = 0; i < 32; i++) {
                        ast[i].X += toX(ast[i].Vel, ast[i].Rot);
                        ast[i].Y += toY(ast[i].Vel, ast[i].Rot);
                    }

                //collision
                    for (int i = 0; i < 32; i++) {
                        for (int j = 0; j < 10; j++){
                            //check if the shot is within a square around the asteroid
                            if (abs(shot[j].X - ast[i].X) < 15 && abs(shot[j].Y - ast[i].Y) < 15){
                                //check if shots are within the asteroid collider
                                if (distance(shot[j].X, shot[j].Y, ast[i].X, ast[i].Y) <= 11){
                                    ast[i].isOn = false;
                                    shot[j].X = 1000;
                                    shot[j].Vel = 0;
                                    score+=100;
                                }
                            }
                        }
                        if (abs(ast[i].X - shipoffsetX) < 80 && abs(ast[i].Y - shipoffsetY) < 80){
                            for (int j = 0; j < 8; j += 2){
                                if (distance(toX(shipvect[j], shipvect[j + 1]) + shipoffsetX, toY(shipvect[j], shipvect[j+1]) + shipoffsetY, ast[i].X, ast[i].Y) <= 11 && ast[i].isOn){
                                    
                                    lives--;
                                    ast[i].isOn = false;

                                }
                            }
                        }
                    }

            //wrap around on screen   JW WHY DO YOU LOOP THIS MULTIPLE TIMES?
            //FB resolved
                
                    if (shipoffsetX > 600)
                        shipoffsetX = 0;
                    if (shipoffsetX < 0)
                        shipoffsetX = 600;
                    if (shipoffsetY > 600)    
                        shipoffsetY = 0;
                    if (shipoffsetY < 0)
                        shipoffsetY = 600;
                
        }          
        }

        //drawing
            BeginDrawing();
            ClearBackground(BLACK);

            //render border
            DrawLine(601,0, 601, 599, GREEN);
            DrawLine(0, 1, 800, 1, GREEN);
            DrawLine(0, 599, 800, 599, GREEN);
            DrawLine(1, 0, 1, 599, GREEN);

            //render stars
            for (int i = 0; i < 200; i++) {
                DrawPixel(starsX[i], starsY[i], WHITE);
            }

            //render ship
                    //draw lines between each point
                    //JW DO THIS WITH A FOR LOOP
                    //FB resolved
                    render(shipoffsetX, shipoffsetY, dir, shipvect, 8);

            //render asteroids
                //smallast

                //render all
                
                for (int i = 0; i < 32; i++) {
                    if (ast[i].isOn == true)
                        render(ast[i].X, ast[i].Y, ast[i].Rot, smallast, 14);
                }
                
            //render shots
                for (int i = 0; i < 10; i++) {
                    render(shot[i].X, shot[i].Y, shot[i].Rot, shotSprite, 4);
                }

            //infopane
                //infopane background
                    DrawRectangle(601, 2, 200, 596, BLACK);
                //title
                    DrawText("ASTEROIDS", 606, 6, 30, RED);
                    DrawText("ASTEROIDS", 608, 8, 30, ORANGE);
                    DrawText("ASTEROIDS", 610, 10, 30, YELLOW);
                    DrawText("ASTEROIDS", 612, 12, 30, GREEN);
                    DrawText(TextFormat("lives: %i", lives), 610, 40, 20, GREEN);
                    
                    DrawText(TextFormat("score: %i", score), 610, 60, 20, GREEN);
                //render player info
                    heading = (dir * (180/3.14));
                    heading = 360 - heading;
                    heading = (int) heading % 360;
                    
                    if (heading < 0)
                        heading += 360;
                    
                    DrawText(TextFormat("score: %i", score), 610, 60, 20, GREEN);
                    DrawText(TextFormat("HEADING: %i", (int) heading), 610, 100, 20, YELLOW);
                    DrawText(TextFormat("X. VEL: %d", (int) (velocityX * 100)), 610, 120, 20, YELLOW);
                    DrawText(TextFormat("Y. VEL: %d", (int) (-1 * velocityY * 100)), 610, 140, 20, YELLOW);

                //render highscores
                    DrawText("HIGHSCORES", 610, 180, 20, GREEN);
                    DrawText(TextFormat("1.  %i", topscores[0]), 610, 200, 20, ORANGE);
                    DrawText(TextFormat("2. %i", topscores[1]), 610, 220, 20, ORANGE);
                    DrawText(TextFormat("3. %i", topscores[2]), 610, 240, 20, ORANGE);
                    DrawText(TextFormat("4. %i", topscores[3]), 610, 260, 20, ORANGE);
                    DrawText(TextFormat("5. %i", topscores[4]), 610, 280, 20, ORANGE);

                //render instructions
                    DrawText("INSTRUCTIONS", 610, 480, 20, ORANGE);
                    DrawText("press W to thrust forward\npress A and D to turn\npress Q and E to brake\npress ENTER to fire\npress BACKSPACE to pause\npress ESC to quit", 610, 500, 10, ORANGE);
            
            //game over
                if (gameover) {
                    DrawRectangle(200, 250, 200, 100, BLACK);
                    DrawRectangleLines(200, 250, 200, 100, GREEN);
                    DrawText("GAME OVER", 207, 257, 30, GREEN);
                    
                    if (frametime%100 > 40 && frametime%10 < 90 ) {
                        DrawText("Press ENTER", 230, 320, 20, GREEN);
                    }

                }

            //if paused display as such
                if (paused && !gameover)
                    DrawText("- PAUSED -", 230, 260, 30, ORANGE);
                //test logic
                //if (IsKeyDown(KEY_BACKSPACE))
                 //   gameover = true;

                //gameover handling
                if (gameover && IsKeyDown(KEY_ENTER)) {
                    //ask if score is greater than any of the current scores
                for (int i = 0; i < 5; i++) {
                    if (score > topscores[i]) {
                    //write the score
                    topscores[i] = score;

                    //shift the rest down
                    for (int j = i; j < 4; j++) {
                        topscores[j + 1] = shifter[j];
                    }

                    //exit the loop
                    i = 5;


                    }
            
                }

                ofstream fileOUT(highscore,ios::binary); //open for writing as binary
                    fileOUT.seekp(0); //set seek pos to beginning of file
                    for (int i = 0, byte = 0; i < 5; i++, byte += 4) {
                        fileOUT.seekp(byte); //advance to the current byte
                        //write the array
                        fileOUT.write((char*)&topscores[i], sizeof(topscores));            
                    }


                fileOUT.close();
   
                    goto START; //return to the beginnning of main to reset everything
                }

            EndDrawing();
        frametime++; //increment game time
        }

        
        



    
    
    CloseWindow();
    
    return 0;
}
