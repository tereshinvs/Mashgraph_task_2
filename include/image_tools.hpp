#pragma once

#include <tuple>

#include "sobel.h"
#include "io.h"

template<typename T>
double get_brightness(const std::tuple<T, T, T> &p) {
	double r, g, b;
	std::tie(r, g, b) = p;
	return 0.2125 * r + 0.7154 * g + 0.0721 * b;
}

bool is_correct_point(int i, int j, int rows, int cols) {
	return i >= 0 && i < rows && j >= 0 && j < cols;
}

std::pair<Matrix<double>, Matrix<double>> get_G_O(const Image &img) {
	Matrix<double> grayscale = img.unary_map(Grayscale_OP());
	auto Ix = sobel_x_double(grayscale);
	auto Iy = sobel_y_double(grayscale);
	auto G = binary_map<double, Gradient_Module_OP>(Gradient_Module_OP(), Ix, Iy);
	auto O = binary_map<double, Gradient_Direction_OP>(Gradient_Direction_OP(), Ix, Iy);
	return std::make_pair(G, O);
}

Matrix<double> resize(const Matrix<double> &src, const double scale) {
	auto get_colour = [](const double C[4], // 1st point is left-up, 2nd is right-up, 3rd is left-down, 4th is right-down
			double x, double y) {
			return C[0] * (1 - x) * (1 - y) + C[1] * x * (1 - y) + C[2] * (1 - x) * y + C[3] * x * y;
		};

	auto big = src.edge_reflection(1);
	Matrix<double> res(src.n_rows * scale, src.n_cols * scale);
	for (uint i = 0; i < res.n_rows; i++)
		for (uint j = 0; j < res.n_cols; j++) {
			double old_y = i / scale;
			double old_x = j / scale;

			uint old_col = std::floor(old_x);
			uint old_row = std::floor(old_y);

			old_x -= old_col; old_y -= old_row;
			old_col += 1; old_row += 1;

			// 1st point is left-up, 2nd is right-up,
			// 3rd is left-down, 4th is right-down
			double C[4];
			C[0] = big(old_row, old_col);
			C[1] = big(old_row, old_col + 1);
			C[2] = big(old_row + 1, old_col);
			C[3] = big(old_row + 1, old_col + 1);

			res(i, j) = get_colour(C, old_x, old_y);
		}
	return res;
}

double Gradient_Module_OP::operator()(const Matrix<double> &m1, const Matrix<double> &m2) const {
	double br_x = m1(0, 0), br_y = m2(0, 0);
	return std::sqrt(br_x * br_x + br_y * br_y);
}

double Gradient_Direction_OP::operator()(const Matrix<double> &m1, const Matrix<double> &m2) const {
	return std::atan2(m2(0, 0), m1(0, 0));
}

double Grayscale_OP::operator()(const Image &m) const {
	double r, g, b;
	std::tie(r, g, b) = m(0, 0);
	return 0.299 * r + 0.587 * g + 0.114 * b;
}