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
    int r;
    int g;
    int b;
} Pixel;

// Функция вычисления точки на кривой Безье
Point calculate_bezier_3(Point p0, Point p1, Point p2, Point p3, float t) {
    Point p;
    float mt = 1.0f - t;
    p.x = mt * mt * mt * p0.x + 3.0f * mt * mt * t * p1.x + 3.0f * mt * t * t * p2.x + t * t * t * p3.x;
    p.y = mt * mt * mt * p0.y + 3.0f * mt * mt * t * p1.y + 3.0f * mt * t * t * p2.y + t * t * t * p3.y;
    return p;
}

int main() {
    Pixel *buffer = (Pixel *)malloc(WIDTH * HEIGHT * sizeof(Pixel));
    if (!buffer) return 1;

    // 1. Создаем идеальный плавный вертикальный градиент фона
    for (int y = 0; y < HEIGHT; y++) {
        float factor = (float)y / (float)(HEIGHT - 1);
        for (int x = 0; x < WIDTH; x++) {
            // Верхний цвет: глубокий темно-синий (15, 23, 42)
            // Нижний цвет: красивый фиолетовый (46, 16, 101)
            buffer[y * WIDTH + x].r = (int)(15 * (1.0f - factor) + 46 * factor);
            buffer[y * WIDTH + x].g = (int)(23 * (1.0f - factor) + 16 * factor);
            buffer[y * WIDTH + x].b = (int)(42 * (1.0f - factor) + 101 * factor);
        }
    }

    // Уникальные координаты твоей кривой Безье
    Point p0 = { 50.0f,  500.0f };
    Point p1 = { 250.0f,  50.0f };
    Point p2 = { 550.0f, 550.0f };
    Point p3 = { 750.0f, 100.0f };

    // 2. Отрисовка кривой Безье с собственным градиентом цвета!
    float dt = 0.0001f;
    for (float t = 0.0f; t <= 1.0f; t += dt) {
        Point pt = calculate_bezier_3(p0, p1, p2, p3, t);
        
        int x = (int)roundf(pt.x);
        int y = (int)roundf(pt.y);
        
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
            // Цвет кривой меняется от зеленого к бирюзовому в процессе рисования (от t=0 до t=1)
            Pixel current_curve_color;
            current_curve_color.r = (int)(34 * (1.0f - t) + 0 * t);
            current_curve_color.g = (int)(197 * (1.0f - t) + 229 * t);
            current_curve_color.b = (int)(94 * (1.0f - t) + 255 * t);

            // Делаем линию чуть толще (3х3 пикселя), чтобы её было хорошо видно
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                        buffer[ny * WIDTH + nx] = current_curve_color;
                    }
                }
            }
        }
    }

    // 3. Правильный вывод данных в формате P3 (без искажений сетки)
    printf("P3\n%d %d\n255\n", WIDTH, HEIGHT);
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        printf("%d %d %d\n", buffer[i].r, buffer[i].g, buffer[i].b);
    }

    free(buffer);
    return 0;
}
