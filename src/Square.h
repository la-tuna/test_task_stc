#ifndef SQUARE_H
#define SQUARE_H

#include <GL/glut.h>
#include <string>
#include <vector>
#include <map>

#define SIZE 3.0

class Square
{
public:
    Square(float x, float y, std::string color) : x(x), y(y), size(SIZE), color(color) {}
    void draw();
    bool isClicked(float x, float y);
    void changeColor(std::string color);
    std::string getColor() const;

private:
    float size;
    float x, y;
    std::string color;
};

#endif