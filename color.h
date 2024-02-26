#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

using color = vec3;

inline double linear_to_gamma(double linear_component) {
    return std::sqrt(linear_component);
}

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by teh number of samples
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // apply the linear to gamma correction
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * pixel_color.x()) << ' '
        << static_cast<int>(256 * pixel_color.y()) << ' '
        << static_cast<int>(256 * pixel_color.z()) << '\n';
}

#endif