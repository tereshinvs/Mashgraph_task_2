#pragma once

#include <tuple>

Image convert_BMP_to_Image(BMP *bmp) {
	uint rows = bmp->TellHeight(), cols = bmp->TellWidth();
	Image res(rows, cols);
	for (uint i = 0; i < rows; i++)
		for (uint j = 0; j < cols; j++) {
			RGBApixel pixel = bmp->GetPixel(j, i);
			res(i, j) = std::make_tuple(pixel.Red, pixel.Green, pixel.Blue);
		}
	return res;
}
