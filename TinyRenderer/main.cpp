#include <cmath>
#include <cstdlib>
#include <ctime>
#include "tgaimage.h"

void line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color) {
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    if (steep) { // if the line is steep, we transpose the image
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax > bx) { // make it left−to−right
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    int y = ay;
    int ierror = 0;
    for (int x = ax; x <= bx; x++) {
        if (steep) // if transposed, de−transpose
            framebuffer.set(y, x, color);
        else
            framebuffer.set(x, y, color);
        ierror += 2 * std::abs(by - ay);
        y += (by > ay ? 1 : -1) * (ierror > bx - ax);
        ierror -= 2 * (bx - ax) * (ierror > bx - ax);
    }
}

int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < (1 << 18); i++) { // reduce iterations for faster testing
        int ax = rand() % width, ay = rand() % height;
        int bx = rand() % width, by = rand() % height;

        std::uint8_t b = static_cast<std::uint8_t>(rand() % 256);
        std::uint8_t g = static_cast<std::uint8_t>(rand() % 256);
        std::uint8_t r = static_cast<std::uint8_t>(rand() % 256);

        // BGRA ordering; bytespp must match image’s bpp
        TGAColor color = { {b, g, r, 255}, 3 };

        line(ax, ay, bx, by, framebuffer, color);
    }


    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}