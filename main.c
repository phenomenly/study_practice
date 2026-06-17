#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600

typedef struct {
    float x;
    float y;
} Point;

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;

Point calculate_bezier_3(Point p0, Point p1, Point p2, Point p3, float t) {
    Point p;
    float mt = 1.0f - t;
    
    p.x = mt * mt * mt * p0.x + 3.0f * mt * mt * t * p1.x + 3.0f * mt * t * t * p2.x + t * t * t * p3.x;
    p.y = mt * mt * mt * p0.y + 3.0f * mt * mt * t * p1.y + 3.0f * mt * t * t * p2.y + t * t * t * p3.y;
    
    return p;
}

int main() {
    Pixel *buffer = (Pixel *)malloc(WIDTH * HEIGHT * sizeof(Pixel));
    if (!buffer) {
        return 1;
    }

    // Заполнение фона статичным темно-синим цветом
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        buffer[i].r = 10;
        buffer[i].g = 20;
        buffer[i].b = 40;
    }

    // Опорные точки кривой
    Point p0 = { 50.0f,  500.0f };
    Point p1 = { 250.0f,  50.0f };
    Point p2 = { 550.0f, 550.0f };
    Point p3 = { 750.0f, 100.0f };

    // Яркий неоново-бирюзовый цвет линии
    Pixel curve_color = { 0, 230, 255 };

    float dt = 0.0002f;
    for (float t = 0.0f; t <= 1.0f; t += dt) {
        Point pt = calculate_bezier_3(p0, p1, p2, p3, t);
        
        int x = (int)roundf(pt.x);
        int y = (int)roundf(pt.y);
        
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
            // Отрисовка кистью 3х3 пикселя для плотности линии
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                        buffer[ny * WIDTH + nx] = curve_color;
                    }
                }
            }
        }
    }

    // Вывод в бинарном формате P6
    printf("P6\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(buffer, sizeof(Pixel), WIDTH * HEIGHT, stdout);

    free(buffer);
    return 0;
}
