#pragma once

#include "io.h"

//Image custom(const Image &, const Matrix<double> &, const char = 'S', const bool = false);

Matrix<double> sobel_x_double(const Image &);
Matrix<double> sobel_y_double(const Image &);

#include "sobel.hpp"
