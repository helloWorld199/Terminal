#pragma once
#include <string>

std::string cypher(std::string s){
    int N = 10;
    for (int i = 0; s[i] != '\0'; i++) {
        short pos = ((s[i] + N) - 'a') % 26;
        s[i] = 'a' + pos;
    }

    return s;
}