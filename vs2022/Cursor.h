#pragma once

class Cursor {
public:
    int x;
    int y;

    Cursor(int startX = 0, int startY = 0)
        : x(startX), y(startY) {
    }

    void moveTo(int newX, int newY) {
        x = newX;
        y = newY;
    }

    void newLine(int lines = 1) {
        x = 0;
        y += lines;
    }

    void down(int lines = 1) {
        y += lines;
    }

    void up(int lines = 1) {
        y -= lines;
    }

    void advance(int columns = 1) {
        x += columns;
    }
};