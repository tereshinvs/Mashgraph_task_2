#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char *argv[]) {
	std::ofstream out;
	out.open(argv[1]);
	for (uint i = 0; i <= 335; i++)
		out << "train/0/file" + std::to_string(i) + ".bmp" << " " << 0 << std::endl;
	out.close();
}