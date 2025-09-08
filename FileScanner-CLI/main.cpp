#include <iostream>
#include <cstring>
#include <chrono>
#include "FileScanner.h"
#define MAX_ARGS 7

using namespace std::chrono;

int main(int argc, char* argv[]) {

	if (argc < MAX_ARGS) {

		std::cerr << "Too few arguments" << std::endl;
		return 1;
	}
		
	std::string path_for_scanning, log_path, base_path;
	
	for (size_t i = 1; i < MAX_ARGS; i++) {
		
		if (strcmp(argv[i], "--base") == 0) {
			base_path = argv[++i];
		}
		else if (strcmp(argv[i], "--log") == 0) {
			log_path = argv[++i];
		}
		else if (strcmp(argv[i], "--path") == 0) {
			path_for_scanning = argv[++i];
		} 
		else {
			std::cerr << "unknown flag" << std::endl;
			return 1;
		}
	}

	FileScanner fs(base_path, log_path);

	auto t1 = high_resolution_clock::now();
	fs.scan(path_for_scanning);
	auto t2 = high_resolution_clock::now();

	auto time_ms = duration_cast<milliseconds>(t2 - t1);

	std::cout << time_ms.count() << " ms" << std::endl;

	return 0;
}