#ifndef TASK_H
#define TASK_H

#include <SFML/Graphics.hpp>
#include "general.h"
#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include <atomic>
#include <thread>
#include <mutex>
#include <iostream>



std::atomic<unsigned> done_count;

struct Pixels
{
    constexpr unsigned N = 16;
    unsigned N_SAMPLES = cam.samples_per_pixel;
    unsigned W_CNT = (cam.image_width + N - 1) / N;
    unsigned H_CNT = (cam.image_height + N - 1) / N;
    Pixels()

    Pixels(unsigned w, unsigned h)
        : width{w},
          height{h},
          data{new float[width * height * 5]()},      // RGBA + sample count
          pixels{new sf::Uint8[width * height * 4]()} // RGBA
    {}

    ~Pixels()
    {
        delete[] data;
        delete[] pixels;
    }

    sf::Uint8 *get_pixels()
    {
        // convert accumulated pixels values so we can display them
        for (int i = 0; i < cam.image_height; i++)
            for (int j = 0; j < cam.image_width; j++)
            {
                const unsigned data_pos = (i * width + j) * 5;
                const unsigned pix_pos = ((cam.image_height - i - 1) * width + j) << 2;
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

    inline void accumulate(unsigned x, unsigned y, const vec3 &col)
    {
        accumulate(x, y, col.x(), col.y(), col.z());
    }

    unsigned width;
    unsigned height;
    float *data; // RGBA + sample count
    unsigned N;
    unsigned N_SAMPLES;
    unsigned W_CNT;
    unsigned H_CNT;

private:
    // use get_pixels()
    sf::Uint8 *pixels; // RGBA
} pixels{cam.image_width, cam.image_height};

struct Task
{
    Task() : my_id{id++} {}

    Task(int x, int y) : sx{x}, sy{y}, my_id{id++} {}

    void move_in_pattern(int &rx, int &ry)
    {
        // snake pattern implementation
        static int x = -1, y = H_CNT - 1;
        static int dir = 0;

        x = dir ? x - 1 : x + 1;
        if (x == W_CNT || x == -1)
        {
            x = y & 1 ? W_CNT - 1 : 0;
            y--;
            dir = !dir;
        }
        rx = x;
        ry = y;
    }

    bool get_next_task()
    {
        bool taken[H_CNT][W_CNT] = {};
        static std::mutex m;

        std::lock_guard<std::mutex> guard{m};

        bool found = false;
        int x, y;
        while (!found)
        {
            move_in_pattern(x, y);
            if (x < 0 || x > W_CNT || y < 0 || y > H_CNT)
                break;

            if (!taken[y][x])
            {
                sx = x * N;
                sy = y * N;
                taken[y][x] = true;
                found = true;
            }
        }

        return found;
    }

    void operator()(const hittable_list& world) {
        bool done = false;
        do {
        if (!get_next_task()) {
            done = true;
            continue;
        }

        for (unsigned s = 0; s < N_SAMPLES; s++)
            cam.render(world, N, pixels, my_id, done);
        } while (!done);

        done_count++;

        std::cout << "Thread " << my_id << " is done!" << std::endl;
    }


    int sx = -1, sy = -1;
    int my_id;
    static int id;
};

#endif