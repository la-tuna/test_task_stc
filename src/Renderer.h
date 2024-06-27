#ifndef RENDERER_H
#define RENDERER_H

#define CURL_STATICLIB
#include <windows.h>
#include "Square.h"
#include "curl/curl.h"

class Renderer
{
private:
    std::vector<Square*>& squares;
    int clickCount;
    std::vector<std::string> availColors;
    std::vector<std::string> usedColors;

public:
    Renderer(std::vector<Square*>& squares) : squares(squares), clickCount(0) {
        availColors = { "white", "black", "brown", "blue", "cyan", "red", "yellow", "green", "grey", "pink" };
    }
    static Renderer* instance;

    static void display();
    void reshape(int width, int height);
    static void mouseClick(int button, int state, int x, int y);

    static void displayWrapper() { instance->display(); }
    static void reshapeWrapper(int width, int height) { instance->reshape(width, height); }
    static void mouseClickWrapper(int button, int state, int x, int y) { instance->mouseClick(button, state, x, y); }

    void run(int argc, char** argv);
};

#endif