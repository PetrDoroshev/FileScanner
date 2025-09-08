#ifndef HASH_DATA_BASE_H
#define HASH_DATA_BASE_H

#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>  

class HashDataBase {


public:

	HashDataBase();
	std::string getVerdict(const std::string& md5_hash);
	void loadDataBase(const std::string& csvFilePath);

private:
	std::unordered_map<std::string, std::string> hash_db;
};


#endif