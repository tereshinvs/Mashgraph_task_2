#pragma once

#include "matrix.h"
//#include "EasyBMP/EasyBMP.h"

#include <tuple>

typedef Matrix<std::tuple<uint, uint, uint>> Image;

Image load_image(const char*);
void save_image(const Image&, const char*);

#include "io.hpp"
