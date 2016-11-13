#pragma once

#include <vector>
#include <cmath>
#include <algorithm>

#include "io.h"
#include "image_tools.h"
#include "sobel.h"
#include "unlinear_SVM_kernel.h"

int get_direction_num(const double o, const int dir_num) {
	double step_size = 2 * PI / dir_num;
	if (dir_num % 2 == 0) {
		for (int i = 0; i < dir_num; i++)
			if (-PI + step_size * (i + 1) >= o)
				return i;
	} else {
		if (o <= -PI + step_size / 2.0 || o >= PI - step_size / 2.0)
			return 0;
		for (int i = 1; i < dir_num; i++)
			if (-PI + step_size / 2.0 + step_size * i >= o)
				return i;
	}
}

std::vector<float> get_direction_hist(const Matrix<double> &G, const Matrix<double> &O) {
	int dir_num = 11;
	std::vector<float> hist(dir_num);
	for (uint i = 0; i < O.n_rows; i++)
		for (uint j = 0; j < O.n_cols; j++)
			hist[get_direction_num(O(i, j), dir_num)] += G(i, j);
	return hist;
}

std::vector<float> normalize_vector(const std::vector<float> &vect) {
	double vect_lng = 0.0;
	for (uint i = 0; i < vect.size(); i++)
		//vect_lng += vect[i];
		vect_lng = std::max(vect_lng, double(vect[i]));
	//vect_lng = std::sqrt(vect_lng);
	std::vector<float> res(vect.size());
	for (uint i = 0; i < vect.size(); i++)
		res[i] = std::abs(vect_lng) > 10e-6 ? vect[i] / vect_lng : 0.0;
	return res;
}

std::vector<float> get_features_from_G_O(const Matrix<double> &G, const Matrix<double> &O) {
	uint cell_width = 4, cell_height = 4, edge_rows = 6, edge_cols = 6, step_rows = 1, step_cols = 1;
	uint cell_cols = (O.n_cols - 2 * edge_cols - cell_width) / step_cols;
	uint cell_rows = (O.n_rows - 2 * edge_rows - cell_height) / step_rows;
	std::vector<float> res;
	for (uint i = 0; i < cell_rows; i++)
		for (uint j = 0; j < cell_cols; j++) {
			//std::cout << edge_rows + i * step_rows << " " << edge_cols + j * step_cols << std::endl;
			uint row = edge_rows + i * step_rows, col = edge_cols + j * step_cols;
			std::vector<float> cell_hist = get_direction_hist(
				G.submatrix(row, col, cell_height, cell_width), O.submatrix(row, col, cell_height, cell_width));
			for (uint q = 0; q < cell_hist.size(); q++)
				res.push_back(cell_hist[q]);
		}

	//return normalize_vector(unlinear_SVM_kernel(res, 2, 0.3));
	return normalize_vector(res);
}

std::vector<float> get_image_features(const Image &img) {
	Matrix<double> G, O;
	std::tie(G, O) = get_G_O(img);
	return get_features_from_G_O(G, O);
}