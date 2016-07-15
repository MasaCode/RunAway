#include "IOManager.h"

#include <fstream>
#include <filesystem>

//Namespace alias
namespace fs = std::tr2::sys;

namespace MasaEngine{

	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer){
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail()){
			perror(filePath.c_str());
			return false;
		}

		//seek to the end
		file.seekg(0, std::ios::end); //going to the end

		//Get the file size
		unsigned int fileSize = (unsigned int)file.tellg(); //tell us the size of byte begin to current location. So the current location must be the end of file to get the size of file.

		file.seekg(0, std::ios::beg); //going to the beginning 

		//Reduce the file size by any header bytes that might be present.
		fileSize -= (unsigned int)file.tellg();

		buffer.resize(fileSize);
		file.read((char *)&(buffer[0]), fileSize); //must be char pointer.

		file.close();

		return true;
	}

	bool IOManager::readFileToBuffer(std::string filePath, std::string& buffer){
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail()){
			perror(filePath.c_str());
			return false;
		}

		//seek to the end
		file.seekg(0, std::ios::end); //going to the end

		//Get the file size
		unsigned int fileSize = (unsigned int)file.tellg(); //tell us the size of byte begin to current location. So the current location must be the end of file to get the size of file.

		file.seekg(0, std::ios::beg); //going to the beginning 

		//Reduce the file size by any header bytes that might be present.
		fileSize -= (unsigned int)file.tellg();

		buffer.resize(fileSize);
		file.read((char *)&(buffer[0]), fileSize); //must be char pointer.

		file.close();

		return true;
	}


	bool IOManager::getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries){
		auto dpath = fs::path(path);

		//Must be directory
		if (!fs::is_directory(dpath)) return false;

		for (auto it = fs::directory_iterator(dpath); it != fs::directory_iterator(); ++it){
			rvEntries.emplace_back();
			rvEntries.back().path = it->path().string();
			if (is_directory(it->path())){
				rvEntries.back().isDirectory = true;
			}
			else{
				rvEntries.back().isDirectory = false;
			}
		}

		return true;
	}

	bool IOManager::makeDirectory(const char* path){
		return fs::create_directories(fs::path(path));
	}

}