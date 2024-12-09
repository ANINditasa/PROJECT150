
#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <time.h>

// Constants
#define SCREEN_WIDTH 980
#define SCREEN_HEIGHT 880
#define SHAFT_COLOR 4
#define BALLOON_COLOR 15
#define ARROW_HEIGHT 50 // Height of the arrow area
#define SCOREBOARD_HEIGHT 250 // Height of the scoreboard area
#define MAX_CONSECUTIVE_FAILS 3 // Maximum consecutive failures allowed

// Function prototypes
void draw_screen();
void show_score(int chance, int success, int score, int level, int color);
void draw_shaft(int x, int y, int c);
void draw_balloon(int x, int y, int c);
void level_1();
void level_2(int score);
void over_1(int score);
void finish(int score);

// Collision detection function
bool check_collision(int shaft_y, int balloon_y);

// Function to ensure balloon position does not overlap scoreboard
int get_valid_balloon_y() {
    return rand() % (SCREEN_HEIGHT - SCOREBOARD_HEIGHT - ARROW_HEIGHT - 100) + 100;
}

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");
    srand(time(NULL)); // Seed for random number generation
    draw_screen();
    outtextxy(100, 100, "Press S to Play the game");
    char key = getch();
    if (key == 's' || key == 'S')
    {
        level_1();
    }
    else
    {
        outtextxy(100, 200, "Wrong Key. Please Enter The Right Key");
        getch();
    }
    closegraph();
    return 0;
}

void draw_screen()
{
    setfillstyle(1, 2);
    bar(0, 0, 150, 20);
    outtextxy(30, 5, "Score Board");
    setfillstyle(1, 3);
    bar(0, 20, 150, SCOREBOARD_HEIGHT);
    setcolor(1);
    outtextxy(30, 40, "Total Chance");
    outtextxy(30, 80, "Success");
    outtextxy(30, 120, "  Score");
    outtextxy(30, 160, "  Level");
    setcolor(15);
    setfillstyle(1, 2);
    bar(0, SCOREBOARD_HEIGHT, 150, 275);
    outtextxy(30, 260, "Control key");
    setfillstyle(1, 3);
    bar(0, 275, 150, 479);

    setcolor(1);
    outtextxy(30, 290, "UP");
    setcolor(4);
    for (int i = 0; i <= 5; i++)
        line(100 - i, 285 + i, 100 + i, 285 + i);
    line(100, 290, 100, 300);

    setcolor(1);
    outtextxy(30, 330, "DOWN");
    setcolor(4);
    for (int i = 0; i <= 5; i++)
        line(95 + i, 330 + i, 105 - i, 330 + i);
    line(100, 320, 100, 330);

    setcolor(1);
    outtextxy(30, 370, "Shoot");
    setcolor(4);
    outtextxy(100, 370, "Enter");

    setcolor(1);
    outtextxy(30, 410, "Exit");
    setcolor(4);
    outtextxy(100, 410, "Esc");
    setfillstyle(1, 1);
    bar(151, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void show_score(int chance, int success, int score, int level, int color)
{
    setcolor(color);
    char buffer[20]; // Buffer for converting integers to strings

    // Convert and display "chance"
    sprintf(buffer, "%d", chance);
    outtextxy(70, 60, buffer);

    // Convert and display "success"
    sprintf(buffer, "%d", success);
    outtextxy(70, 100, buffer);

    // Convert and display "score"
    sprintf(buffer, "%d", score);
    outtextxy(70, 140, buffer);

    // Convert and display "level"
    sprintf(buffer, "%d", level);
    outtextxy(70, 180, buffer);
}

void draw_shaft(int x, int y, int c)
{
    setcolor(c);
    rectangle(x - 1, y - 1, x - 30, y + 1);
    line(x, y, x - 30, y);
    line(x, y, x - 6, y - 4);
    line(x, y, x - 6, y + 4);
}

void draw_balloon(int x, int y, int c)
{
    setcolor(c);
    setfillstyle(1, c);
    sector(x, y, 0, 180, 10, 10);
    for (int i = 0; i <= 12; i++)
        line(x - 10 + i, y + i, x + 10 - i, y + i);
}

bool check_collision(int shaft_y, int balloon_y)
{
    // Check if the shaft and balloon are at the same y coordinate
    return abs(shaft_y - balloon_y) <= 10; // Collision detected
}

void level_1()
{
    int shaft_x = 185, shaft_y = 200, shaft_color = SHAFT_COLOR;
    int balloon_x, balloon_y, balloon_color = BALLOON_COLOR;
    int balloon_speed = 5; // Initial speed for balloon

    draw_screen();
    int chance = 0, success = 0, score = 0, level = 1;
    int consecutive_fails = 0; // Track consecutive failures
    show_score(chance, success, score, level, 0);
    draw_shaft(shaft_x, shaft_y, shaft_color);

    // Randomly position the balloon at the start
    balloon_x = rand() % (SCREEN_WIDTH - 150) + 150;
    balloon_y = get_valid_balloon_y();

    draw_balloon(balloon_x, balloon_y, balloon_color);

    char ch;
    while (1)
    {
        int chance_end = 0;
        while (!chance_end)
        {
            delay(20);
            if (balloon_y > ARROW_HEIGHT)
            {
                draw_balloon(balloon_x, balloon_y, 1); // Erase the previous balloon
                balloon_y -= balloon_speed;
                draw_balloon(balloon_x, balloon_y, balloon_color); // Redraw the balloon
            }
            else
            {
                balloon_x = rand() % (SCREEN_WIDTH - 150) + 150; // Ensure it stays within screen
                balloon_y = get_valid_balloon_y();
                draw_balloon(balloon_x, balloon_y, balloon_color);
                chance++; // Increment chances as the balloon is reset
                show_score(chance, success, score, level, 0);
            }

            if (kbhit())
            {
                ch = getch();
                if (ch == 27) // Exit on Esc key
                    exit(0);
                if (ch == 0)
                    ch = getch();
                if (ch == 72 && shaft_y > 30) // Move up
                {
                    draw_shaft(shaft_x, shaft_y, 0);
                    shaft_y -= 20;
                    draw_shaft(shaft_x, shaft_y, shaft_color);
                }
                if (ch == 80 && shaft_y < 350) // Move down
                {
                    draw_shaft(shaft_x, shaft_y, 0);
                    shaft_y += 20;
                    draw_shaft(shaft_x, shaft_y, shaft_color);
                }
                if (ch == 13) // Check if Enter key is pressed
                {
                    // Check if the shaft and balloon are at the same y coordinate
                    if (check_collision(shaft_y, balloon_y))
                    {
                        balloon_color = 4; // Change the balloon color to red on hit
                        draw_balloon(balloon_x, balloon_y, balloon_color);
                        delay(500); // Wait for 0.5 seconds to see the hit effect

                        // Randomize balloon position after hit
                        balloon_x = rand() % (SCREEN_WIDTH - 150) + 150;
                        balloon_y = get_valid_balloon_y();

                        balloon_color = BALLOON_COLOR; // Reset balloon color
                        draw_balloon(balloon_x, balloon_y, balloon_color);
                        success++;
                        score += 10; // Increase score by 10
                        show_score(chance, success, score, level, 0);
                        consecutive_fails = 0; // Reset consecutive fails on success

                        // Transition to Level 2 after 5 successful hits
                        if (success >= 5)
                        {
                            level_2(score);
                            return; // Exit level 1
                        }
                    }
                    else
                    {
                        chance++;
                        consecutive_fails++; // Increment consecutive failures on a missed shot
                        show_score(chance, success, score, level, 0);
                    }

                    // Check for game over condition
                    if (consecutive_fails >= MAX_CONSECUTIVE_FAILS)
                    {
                        over_1(score);
                        return; // Exit level 1
                    }
                }
            }
        }
    }
}

void level_2(int score)
{
    int shaft_x = 185, shaft_y = 200, shaft_color = SHAFT_COLOR;
    int balloon_x, balloon_y, balloon_color = BALLOON_COLOR;
    int balloon_speed = 7; // Increased speed for level 2

    draw_screen();
    int chance = 0, success = 0, level = 2;
    int consecutive_fails = 0; // Track consecutive failures
    show_score(chance, success, score, level, 0);
    draw_shaft(shaft_x, shaft_y, shaft_color);

    // Randomly position the balloon at the start
    balloon_x = rand() % (SCREEN_WIDTH - 150) + 150;
    balloon_y = get_valid_balloon_y();

    draw_balloon(balloon_x, balloon_y, balloon_color);

    char ch;
    while (1)
    {
        int chance_end = 0;
        while (!chance_end)
        {
            delay(20);
            if (balloon_y > ARROW_HEIGHT)
            {
                draw_balloon(balloon_x, balloon_y, 1); // Erase the previous balloon
                balloon_y -= balloon_speed;
                draw_balloon(balloon_x, balloon_y, balloon_color); // Redraw the balloon
            }
            else
            {
                balloon_x = rand() % (SCREEN_WIDTH - 150) + 150; // Ensure it stays within screen
                balloon_y = get_valid_balloon_y();
                draw_balloon(balloon_x, balloon_y, balloon_color);
                chance++; // Increment chances as the balloon is reset
                show_score(chance, success, score, level, 0);
            }

            if (kbhit())
            {
                ch = getch();
                if (ch == 27) // Exit on Esc key
                    exit(0);
                if (ch == 0)
                    ch = getch();
                if (ch == 72 && shaft_y > 30) // Move up
                {
                    draw_shaft(shaft_x, shaft_y, 0);
                    shaft_y -= 20;
                    draw_shaft(shaft_x, shaft_y, shaft_color);
                }
                if (ch == 80 && shaft_y < 350) // Move down
                {
                    draw_shaft(shaft_x, shaft_y, 0);
                    shaft_y += 20;
                    draw_shaft(shaft_x, shaft_y, shaft_color);
                }
                if (ch == 13) // Check if Enter key is pressed
                {
                    // Check if the shaft and balloon are at the same y coordinate
                    if (check_collision(shaft_y, balloon_y))
                    {
                        balloon_color = 4; // Change the balloon color to red on hit
                        draw_balloon(balloon_x, balloon_y, balloon_color);
                        delay(500); // Wait for 0.5 seconds to see the hit effect

                        // Randomize balloon position after hit
                        balloon_x = rand() % (SCREEN_WIDTH - 150) + 150;
                        balloon_y = get_valid_balloon_y();

                        balloon_color = BALLOON_COLOR; // Reset balloon color
                        draw_balloon(balloon_x, balloon_y, balloon_color);
                        success++;
                        score += 20; // Increase score by 20 for level 2
                        show_score(chance, success, score, level, 0);
                        consecutive_fails = 0; // Reset consecutive fails on success
                    }
                    else
                    {
                        chance++;
                        consecutive_fails++; // Increment consecutive failures on a missed shot
                        show_score(chance, success, score, level, 0);
                    }

                    // Check for game over condition
                    if (consecutive_fails >= MAX_CONSECUTIVE_FAILS)
                    {
                        over_1(score);
                        return; // Exit level 2
                    }
                }
            }
        }
    }
}

void over_1(int score)
{
    setcolor(RED);
    outtextxy(100, 400, "Game Over!");
    char buffer[20]; // Buffer for score display
    sprintf(buffer, "Your Score: %d", score);
    outtextxy(100, 450, buffer);
    getch();
    finish(score);
}

void finish(int score)
{
    setcolor(RED);
    outtextxy(100, 400, "Thank You For Playing!");
    char buffer[20]; // Buffer for score display
    sprintf(buffer, "Your Final Score: %d", score);
    outtextxy(100, 450, buffer);
    getch();
}
