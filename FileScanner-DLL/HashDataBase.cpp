#include "HashDataBase.h"

HashDataBase::HashDataBase() {}

std::string HashDataBase::getVerdict(const std::string& md5_hash)
{
	std::string status = "";
	auto it = hash_db.find(md5_hash);

	if (it != hash_db.end()) {

		status = it->second;
	}

	return status;
}

void HashDataBase::loadDataBase(const std::string& csvFilePath) {

	std::ifstream csvFile(csvFilePath);

	if (!csvFile.is_open()) {

		throw std::runtime_error("Cannot open CSV file: " + csvFilePath);
	}

	std::string line;
	while (std::getline(csvFile, line)) {

		if (line.empty()) continue;

		std::stringstream ss(line);
		std::string hash, status;

		if (std::getline(ss, hash, ';') && std::getline(ss, status)) {
			hash_db[hash] = status;
		}
	}
	
	csvFile.close();
	
}
