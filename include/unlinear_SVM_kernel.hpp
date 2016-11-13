#pragma once

#include <vector>
#include <complex>
#include <cmath>

const double EPS = 10e-9;
const std::complex<double> I(0.0, 1.0);

std::complex<double> sech(const std::complex<double> &x) {
	return 1.0 / std::cosh(x);
}

std::vector<float> unlinear_SVM_kernel(const std::vector<float> &src, const int N, const double L) {
	std::vector<std::complex<double>> PHI;
	for (uint i = 0; i < src.size(); i++)
		for (int j = -N; j <= N; j++) {
			std::complex<double> lambda(j * L, 0.0);
			std::complex<double> x(double(src[i]), 0.0);
			PHI.push_back(std::abs(src[i] - 0) > EPS
				? std::exp(-I * lambda * std::log(x)) * std::sqrt(x * sech(PI * lambda))
				: 0.0);
		}
	std::vector<float> res;
	for (uint i = 0; i < PHI.size(); i++) {
		res.push_back(float(PHI[i].real()));
		res.push_back(float(PHI[i].imag()));
	}
	return res;
}
