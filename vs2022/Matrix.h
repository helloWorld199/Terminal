#pragma once

#include <iostream>
#include <vector>
#include <windows.h>
#define INFO_HEIGHT 1

class Matrix
{
private:
    int width;
    int height;

    HANDLE console;
    COORD origin;

    std::vector<CHAR_INFO> buffer;

public:

    Matrix(int w, int h)
        : width(w), height(h)
    {
        console = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(console, &info);

        origin.X = info.srWindow.Left;
        origin.Y = info.srWindow.Top;

        buffer.resize(width * (height + INFO_HEIGHT));

        clear();
    }

    // Access matrix element
    CHAR_INFO& at(int x, int y)
    {
        return buffer[y * width + x];
    }

    // Change a character and immediately display it
    void set(int x, int y, char c,
        WORD color = FOREGROUND_RED)
    {
        if (!valid(x, y))
            return;

        CHAR_INFO& pixel = at(x, y);

        pixel.Char.AsciiChar = c;

        float dice = (rand() % 1001) / 1000.0;
        if (dice < 0.33)
            pixel.Attributes = FOREGROUND_RED;
        else if (dice < 0.66)
            pixel.Attributes = FOREGROUND_GREEN;
        else
            pixel.Attributes = FOREGROUND_BLUE;




        COORD size = { 1, 1 };
        COORD position = { 0, 0 };

        SMALL_RECT rect = {
            static_cast<SHORT>(origin.X + x),
            static_cast<SHORT>(origin.Y + y),
            static_cast<SHORT>(origin.X + x),
            static_cast<SHORT>(origin.Y + y)
        };




        WriteConsoleOutputA(
            console,
            &pixel,
            size,
            position,
            &rect
        );
    }

    // Read character
    char get(int x, int y) const
    {
        if (!valid(x, y))
            return '\0';

        return buffer[y * width + x].Char.AsciiChar;
    }

    // Clear matrix
    void clear(char c = ' ')
    {
        for (auto& pixel : buffer)
        {
            pixel.Char.AsciiChar = c;
            pixel.Attributes = FOREGROUND_GREEN;
        }

        render();
    }

    // Render entire matrix
    void render()
    {
        COORD size = {
            static_cast<SHORT>(width),
            static_cast<SHORT>(height + INFO_HEIGHT)
        };

        COORD position = { 0, 0 };

        SMALL_RECT rect = {
            origin.X,
            origin.Y,
            static_cast<SHORT>(origin.X + width - 1),
            static_cast<SHORT>(origin.Y + height + INFO_HEIGHT - 1)
        };

        WriteConsoleOutputA(
            console,
            buffer.data(),
            size,
            position,
            &rect
        );
    }

    void set_time(std::chrono::duration<double> time)
    {
        auto total_seconds = static_cast<long long>(time.count());

        int hours = static_cast<int>(total_seconds / 3600);
        int minutes = static_cast<int>((total_seconds % 3600) / 60);
        int seconds = static_cast<int>(total_seconds % 60);

        int milliseconds = static_cast<int>(
            (time.count() - total_seconds) * 1000
            );

        char time_str[64];
        sprintf_s(time_str, "Tempo trascorso: %02d:%02d:%02d : %03d",
            hours, minutes, seconds, milliseconds);

        std::string text = time_str;

        int y = height;

        for (int x = 0; x < width; x++)
        {
            CHAR_INFO& pixel = at(x, y);

            pixel.Char.AsciiChar = (x < text.size()) ? text[x] : ' ';
            pixel.Attributes = FOREGROUND_GREEN;
        }

        COORD size = { static_cast<SHORT>(width), 1 };
        COORD position = { 0, 0 };

        SMALL_RECT rect = {
            origin.X,
            static_cast<SHORT>(origin.Y + y),
            static_cast<SHORT>(origin.X + width - 1),
            static_cast<SHORT>(origin.Y + y)
        };

        BOOL success = WriteConsoleOutputA(
            console,
            buffer.data() + y * width,
            size,
            position,
            &rect
        );

        if (!success)
            OutputDebugStringA("WriteConsoleOutputA failed!\n");
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:

    bool valid(int x, int y) const
    {
        return x >= 0 && x < width &&
            y >= 0 && y < height;
    }
};
