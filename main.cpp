#include "Renderer.h"

#define SQ_PER_STR 3
#define STR_NUM 3

Renderer* Renderer::instance = nullptr;

int main(int argc, char** argv)
{
    std::vector<Square*> squares;

    float startX = -10.0;
    float startY = 7.0;
    float gap = 3.5; // Distance between quad centres

    for (int i = 0; i < STR_NUM; ++i) {
        for (int j = 0; j < SQ_PER_STR; ++j) {
            squares.push_back(new Square(startX + j * gap, startY - i * gap, ""));
        }
    }

    Renderer renderer(squares);
    renderer.run(argc, argv);

    for (Square* square : squares) {
        delete square;
    }
}
