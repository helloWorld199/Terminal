#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <thread>
#include <chrono>
#include "Cursor.h"


class Terminal {
private:
    HANDLE console;


public:
    Cursor cursor;

    Terminal()
        : console(GetStdHandle(STD_OUTPUT_HANDLE)), cursor() {
        CONSOLE_SCREEN_BUFFER_INFO info;

        if (!GetConsoleScreenBufferInfo(console, &info))
            return;

        COORD newSize = info.dwSize;

        newSize.Y = (SHORT)30000;

        SetConsoleScreenBufferSize(console, newSize);
    }

    // Synchronize the Windows console cursor with our Cursor
    void update() {
        COORD pos = {
            (SHORT)cursor.x,
            (SHORT)cursor.y
        };

        SetConsoleCursorPosition(console, pos);

    }

    void moveTo(int x, int y) {
        cursor.moveTo(x, y);
        update();
    }

    void write(char c, int wait_ms) {
        update();
        std::cout << c << std::flush;
        cursor.advance();

        std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
    }

    void write(const std::string& text, int wait_ms = 30, int wait_ms_newline = 300) {
        for (char c : text) {
            if (c == '\n') {
                newLine(1, wait_ms_newline);
            }
            else {
                write(c, wait_ms);
            }
        }
    }

    void newLine(int lines = 1, int wait_ms_newline = 300) {
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms_newline));

        cursor.newLine(lines);

        scrollIfNeeded();
        update();
    }

    void down(int lines = 1) {
        cursor.down(lines);
        update();
    }

    void up(int lines = 1) {
        cursor.up(lines);
        update();
    }

    void clearLine() {
        moveTo(0, cursor.y);
        write(std::string(120, ' '));
        moveTo(0, cursor.y);
    }

    void clear() {
        system("cls");
        cursor.moveTo(0, 0);
        update();
    }

    void fullScreen() {
        INPUT inputs[2] = {};

        // Press F11
        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = VK_F11;

        // Release F11
        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = VK_F11;
        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

        // Send both events
        UINT sent = SendInput(2, inputs, sizeof(INPUT));

        if (sent != 2) {
            std::cerr << "Failed to send F11 key.\n";
            std::this_thread::sleep_for(std::chrono::seconds(30));
        }
    }


    void scrollIfNeeded() {

        CONSOLE_SCREEN_BUFFER_INFO info;

        if (!GetConsoleScreenBufferInfo(console, &info))
            return;

        // The cursor is still inside the buffer.
        if (cursor.y < info.dwSize.Y)
            return;

        SMALL_RECT scrollRect = {
            0,
            1,
            (SHORT)(info.dwSize.X - 1),
            (SHORT)(info.dwSize.Y - 1)
        };

        CHAR_INFO fill = {};
        fill.Char.UnicodeChar = L' ';
        fill.Attributes = info.wAttributes;

        COORD destination = { 0, 0 };

        if (ScrollConsoleScreenBufferW(
            console,
            &scrollRect,
            nullptr,
            destination,
            &fill
        )) {
            cursor.y = info.dwSize.Y - 1;
        }
    }
    

};