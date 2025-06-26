#pragma once

#include <cstdint>
#include <string>
#include <iostream>

#include <unistd.h> // for isatty and fileno

using std::string;
using std::to_string;
using std::ostream;

class Color
{
public:
    Color();
    Color(int r, int g, int b);
    Color(const string& hex);
    static Color fromCode(const string& colorCode);

    // add a ostream operator<< to use Color with cout and cerr so as not to fill files with ansi codes
    friend ostream& operator<<(ostream& os, const Color& color);
private:
    string code;
    static bool enabled;
};

// standard asni colors
extern const Color red;
extern const Color green;
extern const Color blue;
extern const Color yellow;
extern const Color magenta;
extern const Color cyan;
extern const Color white;
extern const Color black;

// bright ansi colors
extern const Color brightRed;
extern const Color brightGreen;
extern const Color brightBlue;
extern const Color brightYellow;
extern const Color brightMagenta;
extern const Color brightCyan;
extern const Color brightWhite;
extern const Color brightBlack;

// bold ansi colors
extern const Color boldRed;
extern const Color boldGreen;
extern const Color boldBlue;
extern const Color boldYellow;
extern const Color boldMagenta;
extern const Color boldCyan;
extern const Color boldWhite;
extern const Color boldBlack;

// special ansi colors
extern const Color reset;
extern const Color bold;
extern const Color underline;