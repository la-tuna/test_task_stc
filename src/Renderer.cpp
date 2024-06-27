#include "Renderer.h"
#include <algorithm>
#include <random>
#include <ctime>

static size_t callback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string fetchData() {
    CURL* curl;
    CURLcode res;
    std::string buffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://yandex.ru/pogoda/moscow");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return buffer;
}

// Time extracting
std::string extractTime(const std::string& html) {
    std::string timeTag = "<time class=\"time fact__time\"";
    size_t timePos = html.find(timeTag);
    if (timePos != std::string::npos) {
        size_t datetimePos = html.find("datetime=\"", timePos);
        if (datetimePos != std::string::npos) {
            size_t datetimeStart = datetimePos + 21; // 21 - length of str "datetime=\""
                                                     // Full tag is <time class="time fact__time" datetime="yyyy-mm-dd hh:mm+0300">
            size_t datetimeEnd = html.find('"', datetimeStart);
            if (datetimeEnd != std::string::npos) {
                return html.substr(datetimeStart, datetimeEnd - datetimeStart);
            }
        }
    }
    return "N/A";
}

// Temperature extracting
std::string extractTemperature(const std::string& html) {
    std::string tempTag = "<span class=\"temp__value temp__value_with-unit\">";
    size_t tempPos = html.find(tempTag);
    if (tempPos != std::string::npos) {
        size_t tempStart = tempPos + tempTag.length();
        size_t tempEnd = html.find('<', tempStart);
        return html.substr(tempStart, tempEnd - tempStart);
    }
    return "N/A";
}

void Renderer::display() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (Square* square : instance->squares) {
        square->draw();
    }

    // Click counter drawing
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(-8.0f, -3.5f);
    std::string clickText = "Clicks: " + std::to_string(instance->clickCount);
    for (char c : clickText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Time drawing
    std::string timeData = fetchData();
    std::string time = extractTime(timeData);
    std::string timeText = "Current time: " + time;
    glRasterPos2f(1.0f, 3.8f);
    for (char c : timeText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Temperature drawing
    std::string weatherData = fetchData();
    std::string temperature = extractTemperature(weatherData);
    std::string temperatureText = "Current air temperature: " + temperature;
    glRasterPos2f(1.0f, 2.4f);
    for (char c : temperatureText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glFlush();
}

void Renderer::mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

        float aspect = (float)windowWidth / (float)windowHeight;
        float openglX, openglY;
        if (aspect >= 1.0) {
            openglX = ((float(x) / windowWidth) * 20.0f - 10.0f) * aspect;
            openglY = 10.0f - (float(y) / windowHeight) * 20.0f;
        }
        else {
            openglX = (float(x) / windowWidth) * 20.0f - 10.0f;
            openglY = (10.0f - (float(y) / windowHeight) * 20.0f) / aspect;
        }

        // Click in square checking
        for (Square* square : instance->squares) {
            if (square->isClicked(openglX, openglY)) {
                instance->clickCount++;

                // Quad color changing
                std::string oldColor = square->getColor();
                square->changeColor(instance->availColors.back());
                instance->availColors.pop_back();
                instance->availColors.push_back(oldColor);

                glutPostRedisplay();
                break;
            }
        }
    }
}

void Renderer::reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)width / (float)height;
    if (aspect >= 1.0) {
        gluOrtho2D(-10.0 * aspect, 10.0 * aspect, -10.0, 10.0);
    }
    else {
        gluOrtho2D(-10.0, 10.0, -10.0 / aspect, 10.0 / aspect);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Renderer::run(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Result Display");

    // Color shuffle
    std::shuffle(availColors.begin(), availColors.end(), std::default_random_engine(time(0)));
    for (size_t i = 0; i < squares.size(); ++i) {
        squares[i]->changeColor(availColors[i]);
        usedColors.push_back(availColors[i]);
    }
    availColors.erase(availColors.begin(), availColors.begin() + squares.size());

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.1, 0.5, 0.1);

    instance = this;
    glutDisplayFunc(displayWrapper);
    glutMouseFunc(mouseClickWrapper);
    glutReshapeFunc(reshapeWrapper);
    glutMainLoop();
}
