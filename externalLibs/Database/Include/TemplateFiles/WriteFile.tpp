/*
Athor: Loke Strøm
*/
#include "Exception.hpp"
#include "string.hpp"
#include <fstream>
#include "..\WriteFile.hpp"


namespace Database {

	template<typename T>
	WriteFile<T>::WriteFile() noexcept {
		file = new std::ofstream();
	}
	template<typename T>
	WriteFile<T>::WriteFile(const String& filename) noexcept {
		file = new std::ofstream(toSTD(filename), std::ios::binary);
	}

	template<typename T>
	WriteFile<T>::WriteFile(WriteFile<T>&& writeFile) noexcept {
		file = writeFile.file;
		writeFile.file = nullptr;
	}

	template<typename T>
	WriteFile<T>::~WriteFile() noexcept {
		if (file == nullptr)
			return;
		if (file->is_open()) {
			file->close();
		}
		delete file;
	}

	template<typename T>
	void WriteFile<T>::open(const String& filename) {
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
	void WriteFile<T>::addcolumns(const Vector<String>& columnNames) noexcept {
		size_t s = columnNames.size();
		file->write(reinterpret_cast<const char*>(&s), sizeof(unsigned int));
		for (const String& i : columnNames) {
			for (const char c : i)
				file->write(reinterpret_cast<const char*>(&c), sizeof(char));
			file->write(reinterpret_cast<const char*>(&splitByte), sizeof(char));
		}
	}

	template<typename T>
	void WriteFile<T>::addData(const Vector<T>& data) noexcept {
		for (T i : data) {
			file->write(reinterpret_cast<const char*>(&i),sizeof(i));
		}
	}
}