#include <termios.h>
#include <unistd.h>
#include <stdio.h>

void setRawMode(int enable)
{
    static struct termios oldt, newt;
    if (enable)
    {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }
    else
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

void indent(int level)
{
    for (int i = 0; i < level; i++)
    {
        putchar('\t'); // Indent with a tab character
    }
}

int main()
{
    setRawMode(1);
    int c;

    const char ctrlD = 4; // Ctrl+D
    const char backspace = 127; // ASCII backspace character

    int indentLevel = 0;

    while ((c = getchar()) != ctrlD)
    {
        if (c == backspace)
        {
            printf("\b \b");
        }
        else if (c == '{')
        {
            putchar('{');
            putchar('\n');
            indentLevel++;
            for (int i = 0; i < indentLevel; i++) {
                putchar('\t'); // Indent with a tab character
            }
        }
        else if (c == '}')
        {
            putchar('\n');
            indentLevel--;
            if (indentLevel < 0) indentLevel = 0; // Prevent negative indentation
            for (int i = 0; i < indentLevel; i++) {
                printf("   ");
            }
            putchar('}');
        }
        else if (c == '\n')
        {
            putchar('\n');
            for (int i = 0; i < indentLevel; i++) {
                printf("   ");
            }
        }
        else
        {
            putchar(c);
        }

        fflush(stdout);
    }
    setRawMode(0);
    return 0;
}