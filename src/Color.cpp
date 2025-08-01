#include "Color.h"

using std::string;
using std::to_string;
using std::stoi;
using std::ostream;

const Color red = Color::fromCode("31");
const Color green = Color::fromCode("32");
const Color blue = Color::fromCode("34");
const Color yellow = Color::fromCode("33");
const Color magenta = Color::fromCode("35");
const Color cyan = Color::fromCode("36");
const Color white = Color::fromCode("37");
const Color black = Color::fromCode("30");

// bright ansi colors
const Color brightRed = Color::fromCode("91");
const Color brightGreen = Color::fromCode("92");
const Color brightBlue = Color::fromCode("94");
const Color brightYellow = Color::fromCode("93");
const Color brightMagenta = Color::fromCode("95");
const Color brightCyan = Color::fromCode("96");
const Color brightWhite = Color::fromCode("97");
const Color brightBlack = Color::fromCode("90");

// bold ansi colors
const Color boldRed = Color::fromCode("1;31");
const Color boldGreen = Color::fromCode("1;32");
const Color boldBlue = Color::fromCode("1;34");
const Color boldYellow = Color::fromCode("1;33");
const Color boldMagenta = Color::fromCode("1;35");
const Color boldCyan = Color::fromCode("1;36");
const Color boldWhite = Color::fromCode("1;37");
const Color boldBlack = Color::fromCode("1;30");

// special ansi colors
const Color reset = Color::fromCode("0");
const Color bold = Color::fromCode("1");
const Color underline = Color::fromCode("4");

bool Color::enabled = isatty(fileno(stderr)) && isatty(fileno(stdout));

Color::Color():
    code("\033[0m")
{ }

Color::Color(int r, int g, int b):
    code("\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m")
{ }

Color::Color(const string& hex)
{
    if (hex.size() == 7 && hex[0] == '#')
    {
        int r = stoi(hex.substr(1, 2), nullptr, 16);
        int g = stoi(hex.substr(3, 2), nullptr, 16);
        int b = stoi(hex.substr(5, 2), nullptr, 16);
        code = "\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m";
    }
    else
    {
        code = "\033[0m"; // Default to reset color if invalid hex
    }
}

Color Color::fromCode(const string& colorCode)
{
    Color color(0, 0, 0); // Default color
    color.code = "\033[" + colorCode + "m";
    return color;
}

ostream& operator<<(ostream& os, const Color& color)
{
    if (!Color::enabled)
    {
        return os; // If color is not enabled, do nothing
    }

    if (!os.good())
    {
        return os; // If the stream is not good, do nothing
    }

    os << color.code; // Apply color code to output stream
    return os;
}
