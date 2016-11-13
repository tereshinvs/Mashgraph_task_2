#pragma once

#include "io.h"

const double PI = std::acos(-1.0);

double get_brightness(const std::tuple<uint, uint, uint> &);

bool is_correct_point(int, int, int, int);

std::pair<Matrix<double>, Matrix<double>> get_G_O(const Image &);

Matrix<double> resize(const Matrix<double> &, const double);

class Gradient_Module_OP {
	public:
		double operator()(const Matrix<double> &m1, const Matrix<double> &m2) const;
		static const int radius1 = 0, radius2 = 0;
};

class Gradient_Direction_OP {
	public:
		double operator()(const Matrix<double> &m1, const Matrix<double> &m2) const;
		static const int radius1 = 0, radius2 = 0;
};

class Grayscale_OP {
	public:
		double operator()(const Image &m) const;
		static const int radius = 0;
};

#include "image_tools.hpp"