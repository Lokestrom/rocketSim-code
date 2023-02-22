/*
Athor: Loke Strøm
*/
#include "Exception.hpp"
#include <fstream>


namespace Database {

	template<typename T>
	WriteFile<T>::WriteFile() {
		file = new std::ofstream();
	}
	template<typename T>
	WriteFile<T>::WriteFile(String filename) {
		file = new std::ofstream(toSTD(filename), std::ios::binary);
	}

	template<typename T>
	WriteFile<T>::~WriteFile() {
		if (file->is_open()) {
			file->close();
		}
		delete file;
	}

	template<typename T>
	void WriteFile<T>::open(String filename) {
		if (file->is_open)
			throw WriteFileError("File is alredy open");
		file->open(filename, std::ios::binary);
	}

	template<typename T>
	void WriteFile<T>::close() {
		if (!file->is_open())
			throw WriteFileError("File is alredy closed");
		file->close();
	}

	template<typename T>
	void WriteFile<T>::addcolumns(Vector<String> columnNames) {
		unsigned int s = columnNames.size();
		file->write(reinterpret_cast<const char*>(&s), sizeof(unsigned int));
		for (const String& i : columnNames) {
			for (const char c : i)
				file->write(reinterpret_cast<const char*>(&c), sizeof(char));
			file->write(reinterpret_cast<const char*>(&splitByte), sizeof(char));
		}
	}

	template<typename T>
	void WriteFile<T>::addData(Vector<T> data) {
		for (T i : data) {
			file->write(reinterpret_cast<const char*>(&i),sizeof(i));
		}
	}
}