// bmp_converter.cpp - Litte program used to convert bmp file into c uint8_t array
// Date: 2018-11-01
// Created by: Mateusz Mamala


#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <iomanip>

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "Wrong number of arguments" << '\n';
		return -1;
	}

	std::string input_file_path = argv[1];
	std::cout << input_file_path << '\n';

	std::ifstream input_file(input_file_path);

	if (!input_file.is_open()) {
		std::cout << "Unable to open file: " << input_file_path << '\n';
		return -1;
	}

	char bmp_header[54];
	std::cout << "Reading BMP header..." << '\n';
	input_file.read(bmp_header, 54);

	if (bmp_header[0] != 0x42 && bmp_header[1] != 0x4D) {
		std::cout << "Wrong BMP header" << '\n';
		return -1;
	}

	uint32_t image_width = 0;
	uint32_t image_height = 0;
	uint16_t bits_per_pixel = (uint8_t)bmp_header[28] + ((uint8_t)bmp_header[29] << 8);

	uint32_t image_data_offset = 
		(uint8_t)bmp_header[10]
		+ ((uint8_t)bmp_header[11] << 8)
		+ ((uint8_t)bmp_header[12] << 16)
		+ ((uint8_t)bmp_header[13] << 24);

	image_width = 
		(uint8_t)bmp_header[18]
		+ ( ((uint8_t)bmp_header[19]) << 8)
		+ ( ((uint8_t)bmp_header[20]) << 16) 
		+ ( ((uint8_t)bmp_header[21]) << 24);

	image_height = (uint8_t)bmp_header[22]; 
	image_height += ( ((uint8_t)bmp_header[23]) << 8); 
	image_height += ( ((uint8_t)bmp_header[24]) << 16); 
	image_height += ( ((uint8_t)bmp_header[25]) << 24);
	uint32_t bytes_to_read = image_width * image_height / 8;

	std::cout << "Found BMP image:\n\tWidth: " << image_width << '\n';
	std::cout << "\tHeight: " << image_height << '\n'; 	
	std::cout << "\tBits per pixel: " << bits_per_pixel << '\n';
	std::cout << "\tImage data offset: " << image_data_offset << '\n';
	std::cout << "\tImage data size [B]: " << bytes_to_read << '\n';

	input_file.seekg(image_data_offset, input_file.beg);
	char test[51];
	input_file.read(test, 51);

	char test2[10];
	input_file.seekg(image_data_offset + 10, input_file.beg);
	input_file.read(test2, 10);

	std::ofstream output_file;
	output_file.open("image_data.c");

	output_file << "#include <stdint.h>" << "\n\n";
	output_file << "const uint8_t img_data[] = {" << "\n\t"; 
	uint8_t file_column_idx = 0;
	uint32_t img_column = 0;
	uint32_t img_row = image_height - 1;
	uint32_t scan_line_length = image_width / 8;
	
	// Round scan_line length to nearest multiple of 4
	scan_line_length = ((scan_line_length + 2) / 4) * 4;

	input_file.seekg(image_data_offset + (img_row * scan_line_length), input_file.beg);
	do {
		char img_byte = 0;
		input_file.read(&img_byte, 1);

		uint32_t img_byte_32 = (uint8_t)img_byte;
		output_file << "0x" << std::setfill('0') << std::setw(2) << std::hex << img_byte_32 << ", ";
		file_column_idx++;
		if (file_column_idx == 16) {
			file_column_idx = 0;
			output_file << "\n\t";
		}
		bytes_to_read--;
		img_column++;
		if (img_column == image_width / 8) {
			input_file.seekg(image_data_offset + (img_row * scan_line_length), input_file.beg);
			img_row--;
			img_column = 0;
		}
	} while (bytes_to_read > 0);

	output_file << "};" << std::endl;
	output_file.close();

	char img_data[30];
	input_file.read(img_data, 30);
	
	input_file.read(img_data, 30);
	return 0;
}


