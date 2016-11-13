#pragma once

#include <tuple>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <functional>
#include <climits>

#include "image_tools.h"
#include "io.h"

class Custom_Filter_OP {
	public:
		const int radius;
		const bool need_div;

		Custom_Filter_OP(const Matrix<double> &_kernel, bool _need_div = false):
			radius(std::max(_kernel.n_rows, _kernel.n_cols) / 2), need_div(_need_div),
			kernel(_kernel.deep_copy()), sum_k(0.0),
			c_i_r(radius), c_i_c(radius), c_k_r(kernel.n_rows / 2), c_k_c(kernel.n_cols / 2) {
			for (uint i = 0; i < kernel.n_rows; i++)
				for (uint j = 0; j < kernel.n_cols; j++)
					sum_k += kernel(i, j);
		}

		double operator()(const Matrix<double> &m) const {
			double sum = 0.0;
			for (int i = 0; i < int(kernel.n_rows); i++)
				for (int j = 0; j < int(kernel.n_cols); j++)
					sum += kernel(i, j) * m(c_i_r + i - c_k_r, c_i_c + j - c_k_c);
			return need_div ? sum / sum_k : sum;
		}

	private:
		Matrix<double> kernel;
		double sum_k;
		int c_i_r, c_i_c, c_k_r, c_k_c; // Centres of input image-part and kernel
};

Matrix<double>
custom_double(const Matrix<double> &src, const Matrix<double> &kernel, const bool need_div = false) {
	int radius = std::max(kernel.n_rows, kernel.n_cols) / 2;
	Matrix<double> big = src.edge_reflection(radius);
	return big.unary_map(Custom_Filter_OP(kernel, need_div)).submatrix(radius, radius, src.n_rows, src.n_cols);	
}

Matrix<double> sobel_x_double(const Matrix<double> &src) {
	return custom_double(src, { {-1, 0, 1} });
}

Matrix<double> sobel_y_double(const Matrix<double> &src) {
	Matrix<double> sobel_y(3, 1);
	sobel_y(0, 0) = 1; sobel_y(1, 0) = 0; sobel_y(2, 0) = -1;
	return custom_double(src, sobel_y);
}

double normalize_colour(double c, char norm_type = 'S') {
	return norm_type == 'S'
		? std::max(0.0, std::min(255.0, c))
		: std::min(255.0, std::abs(c));
}
