#ifndef GENERAL_H
#define GENERAL_H

#include <cmath>
#include <cstdlib>
<<<<<<< HEAD
=======
#include <iostream>
>>>>>>> 771f2eae743b5c1a914032651343ce16f1e72823
#include <limits>
#include <memory>


<<<<<<< HEAD
=======
// Common Headers

#include "interval.h"
#include "ray.h"
#include "vec3.h"

>>>>>>> 771f2eae743b5c1a914032651343ce16f1e72823
// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double(){
    // Returns a random real in [0,1)
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max){
    // Returns a random real in [min,max)
    return min + (max-min)*random_double();
}

<<<<<<< HEAD

// Common Headers

#include "interval.h"
#include "ray.h"
#include "vec3.h"

=======
>>>>>>> 771f2eae743b5c1a914032651343ce16f1e72823
#endif