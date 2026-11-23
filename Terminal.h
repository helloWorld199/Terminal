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

    void write(char c) {
        update();
        std::cout << c << std::flush;
        cursor.advance();

        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    void write(const std::string& text) {
        for (char c : text) {
            if (c == '\n') {
                newLine();
            }
            else {
                write(c);
            }
        }
    }

    void newLine(int lines = 1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

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

        // Keep the cursor inside the console buffer.
        if (cursor.y >= info.dwSize.Y) {

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

            ScrollConsoleScreenBufferW(
                console,
                &scrollRect,
                nullptr,
                destination,
                &fill
            );

            cursor.y = info.dwSize.Y - 1;
        }
    }

};