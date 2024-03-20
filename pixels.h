#include <SFML/Graphics.hpp>
#include "general.h"
#include "color.h"

struct Pixels {
    Pixels(unsigned w, unsigned h)
        : width{w},
          height{h},
          data{new float[width * height * 5]()},      // RGBA + sample count
          pixels{new sf::Uint8[width * height * 4]()} // RGBA
    {
    }

    ~Pixels()
    {
        delete[] data;
        delete[] pixels;
    }

    sf::Uint8 *get_pixels(int WIDTH, int HEIGHT)
    {
        // convert accumulated pixels values so we can display them
        for (int i = 0; i < HEIGHT; i++)
            for (int j = 0; j < WIDTH; j++)
            {
                const unsigned data_pos = (i * width + j) * 5;
                const unsigned pix_pos = (i * width + j) << 2;
                const float ns = data[data_pos + 4]; // number of accumulated samples
                pixels[pix_pos + 0] =
                    sf::Uint8(255.99f * (sqrtf(data[data_pos + 0] / ns)));
                pixels[pix_pos + 1] =
                    sf::Uint8(255.99f * (sqrtf(data[data_pos + 1] / ns)));
                pixels[pix_pos + 2] =
                    sf::Uint8(255.99f * (sqrtf(data[data_pos + 2] / ns)));
                pixels[pix_pos + 3] = 255u;
            }

        return pixels;
    }

    inline void accumulate(unsigned x, unsigned y, float r, float g, float b)
    {
        const unsigned pos = (y * width + x) * 5;
        data[pos + 0] += r;
        data[pos + 1] += g;
        data[pos + 2] += b;
        data[pos + 3] += 255.f; // opaque
        data[pos + 4] += 1.f;   // number of samples
    }

    inline void accumulate(unsigned x, unsigned y, const color &col)
    {
        accumulate(x, y, col.x(), col.y(), col.z());
    }

    unsigned width;
    unsigned height;
    float *data; // RGBA + sample count

private:
    // use get_pixels()
    sf::Uint8 *pixels; // RGBA
};