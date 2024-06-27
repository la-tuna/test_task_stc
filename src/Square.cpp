#include "Square.h"

std::map<std::string, std::vector<float>> colorMap = {
    {"white", {1.0, 1.0, 1.0}},
    {"black", {0.0, 0.0, 0.0}},
    {"brown", {0.6, 0.3, 0.0}},
    {"blue", {0.0, 0.0, 1.0}},
    {"cyan", {0.0, 1.0, 1.0}},
    {"red", {1.0, 0.0, 0.0}},
    {"yellow", {1.0, 1.0, 0.0}},
    {"green", {0.0, 1.0, 0.0}},
    {"grey", {0.5, 0.5, 0.5}},
    {"pink", {1.0, 0.0, 1.0}}
};

void Square::draw() {
    std::vector<float> colorValues = colorMap[color];
    glColor3f(colorValues[0], colorValues[1], colorValues[2]);

    glBegin(GL_QUADS);
    glVertex2f(x - size / 2, y - size / 2);
    glVertex2f(x + size / 2, y - size / 2);
    glVertex2f(x + size / 2, y + size / 2);
    glVertex2f(x - size / 2, y + size / 2);
    glEnd();

    // Color name drawing
    glColor3f(0.0f, 0.5f, 0.0f);
    glRasterPos2f(x - size / 2 + 0.1, y - 0.1);
    for (char c : color) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

bool Square::isClicked(float openglX, float openglY) {
    return openglX >= (this->x - size / 2) && openglX <= (this->x + size / 2) &&
        openglY >= (this->y - size / 2) && openglY <= (this->y + size / 2);
}

void Square::changeColor(std::string newColor) {
    this->color = newColor;
}

std::string Square::getColor() const {
    return color;
}