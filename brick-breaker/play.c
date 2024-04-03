#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "gba.h"
#include "play.h"
#include "paddle.h"
#include "forest.h"
#include "block.h"
#include "circle.h"


int play(struct Ball bp, struct Paddle bar) {
    
    u32 previousButtons = BUTTONS;
    u32 currentButtons = BUTTONS;

    char buffer[30];
    int score = 0;
    int num_bricks = 0;
    struct Brick bricks[20];

    drawFullScreenImageDMA(forest);
    // Initializing Game
    sprintf(buffer, "Score: 0");
    drawRectDMA(0, 0, 240, 16, BLACK);
    drawString(5, 5, buffer, WHITE);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 10; j++) {
            int row = i * 16 + 16;
            int col = j * 24;
            struct Brick brick = {row, col, 24, 16};
            bricks[num_bricks] = brick;
            drawImageDMA(row, col, 24, 16, block);
            num_bricks++;
        }
    }

    while (1) {
        currentButtons = BUTTONS;
        struct Ball obp = bp;
        struct Paddle obar = bar;

        //  Backspace to return to START page
        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons) != 0) {
            return 0;
        }
        if (num_bricks == 0) {
            return 1;
        }

        //collision w/ paddle logic
        if (bp.row + bp.radius == bar.row && bp.col >= bar.col && bp.col + bp.radius <= bar.col + bar.width) {
            bp.rd = -bp.rd;
        }
        
        // moving the bar when key pressed up, down, right, left
        if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons)) {
            if (bar.row > 4 * HEIGHT / 5) {
                bar.row -= bar.speed;
                undrawImageDMA(bar.row + bar.height, obar.col, obar.width, obar.row - bar.row, forest);
                drawImageDMA(bar.row, bar.col, bar.width, bar.height, paddle);
            } 
        } else if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons)) {   
            if (bar.row < HEIGHT - bar.height) {
                bar.row += bar.speed;
                undrawImageDMA(obar.row, obar.col, obar.width, bar.row - obar.row, forest);
                drawImageDMA(bar.row, bar.col, bar.width, bar.height, paddle);
            }
        } else if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)) {      
            if (bar.col < WIDTH - bar.width) {
                bar.col += bar.speed;
                undrawImageDMA(obar.row, obar.col, bar.col - obar.col, obar.height, forest);
                drawImageDMA(bar.row, bar.col, bar.width, bar.height, paddle);
            }
        } else if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons)) {      
            if (bar.col > 0) {
                bar.col -= bar.speed;
                undrawImageDMA(obar.row, bar.col + bar.width, obar.col - bar.col, bar.height, forest);
                drawImageDMA(bar.row, bar.col, bar.width, bar.height, paddle);
            }
        }


        //collision w/ brick logic
        for (int i = 0; i < num_bricks; i++) {
            if (bp.row <= bricks[i].row + bricks[i].height &&
                bp.col >= bricks[i].col &&
                bp.col + bp.radius <= bricks[i].col + bricks[i].width) {
                    bp.rd = -bp.rd;
                    undrawImageDMA(bricks[i].row, bricks[i].col, bricks[i].width, bricks[i].height, forest);
                    for (int j = i + 1; j < num_bricks; j++) {
                        bricks[j - 1] = bricks[j];
                    }
                    num_bricks--;
                    score += 100;
                    sprintf(buffer, "Score: %d", score);
                    drawRectDMA(0, 0, 100, 16, BLACK);
                    drawString(5, 5, buffer, WHITE);
                    break;
            }
        }
        
        
        // Wall and border collision logic
        if(bp.row <= 16) {
            bp.row = 16;
            bp.rd = -bp.rd;
        }
        if(bp.row > HEIGHT - bp.radius) {
            return -1; // Losing condition
        }
        if(bp.col < 0) {
            bp.col = 0;
            bp.cd = -bp.cd;
        }
        if(bp.col > WIDTH - bp.radius) {
            bp.col = WIDTH - bp.radius;
            bp.cd = -bp.cd;
        }

        bp.row += bp.rd;
        bp.col += bp.cd;

        waitForVBlank();

        // Erase old ball
        undrawImageDMA(obp.row, obp.col, obp.radius, obp.radius, forest);
        // Draw new ball
        // drawRectDMA(bp.row, bp.col, bp.radius, bp.radius, bp.color);
        drawImageDMA(bp.row, bp.col, bp.radius, bp.radius, circle);
        // Draw a bar
        drawImageDMA(bar.row, bar.col, bar.width, bar.height, paddle);
        // Rerender Bricks
        for (int i = 0; i < num_bricks; i++) {
            struct Brick brick = bricks[i];
            drawImageDMA(brick.row, brick.col, 24, 16, block);
        }
    }
}