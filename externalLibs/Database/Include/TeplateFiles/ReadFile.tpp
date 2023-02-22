/*
Athor: Loke Strøm
*/
#include "Exception.hpp"
#include <fstream>

namespace Database
{
	template<typename T>
	ReadFile<T>::ReadFile() {
		file = new std::ifstream();
	}

	template<typename T>
	ReadFile<T>::ReadFile(String fileName) {
		ReadFile::fileName = fileName;
		file = new std::ifstream(toSTD(fileName));
		unsigned int columnNummber = 0;
		file->read(reinterpret_cast<char*>(&columnNummber), sizeof(unsigned int));
		dataStart = 4;
		String temps;
		char* tempc = new char[1];
		for (int i = 0; i < columnNummber;) {
			file->read(tempc, sizeof(char));
			dataStart++;
			if (tempc[0] == splitByte) {
				ColumnNames.pushBack(temps);
				temps.clear();
				i++;
				continue;
			}
			temps.pushBack(tempc[0]);
		}
	}

	template<typename T>
	ReadFile<T>::~ReadFile() {
		if (file->is_open())
			file->close();
		delete file;
	}

	template<typename T>
	void ReadFile<T>::open(String fileName) {
		if (file->is_open())
			throw SystemError("A file is alredy open");
		this(fileName);
	}

	template<typename T>
	void ReadFile<T>::close() {
		if (file->is_open())
			file->close();
		file->close();
	}

	template<typename T>
	bool ReadFile<T>::isOpen() {
		return file->is_open();
	}

	template<typename T>
	void ReadFile<T>::getAllDataFromColumn(Vector<T>& data, String columnName) noexcept {
		file->seekg(dataStart, std::ios::beg);
		data.clear();
		T x;
		auto columnNummber = ColumnNames.linearSearch(columnName);
		for (auto i = 0; file->read(reinterpret_cast<char*>(&x), sizeof(T)); i++) {
			if (i == columnNummber)
				data.pushBack(x);
			if (i == ColumnNames.size()-1)
				i = -1;
		}
	}

	template<typename T>
	void ReadFile<T>::getAllRowsWhereColumnIsEqualeToAValue(Vector<Vector<T>>& data, String columnName, T value) noexcept
	{
		file->seekg(dataStart, std::ios::beg);
		data.clear();
		T x;
		auto columnNumber = ColumnNames.linearSearch(columnName);
		data.pushBack(Vector<T>(ColumnNames.size()));
		for (auto i = 0; !file->eof();) {
			for (auto j = 0; j < ColumnNames.size() && file->read(reinterpret_cast<char*>(&x), sizeof(T)); j++) {
				data[i].pushBack(x);
				if (j != columnNumber)
					continue;
				if (j == columnNumber && value != x) {
					data[i].clear();
					break;
				}
				j++;
				while (j < ColumnNames.size() && file->read(reinterpret_cast<char*>(&x), sizeof(T))) {
					data[i].pushBack(x);
					j++;
				}
				data.pushBack(Vector<T>(ColumnNames.size()));
				i++;
				break;
			}
		}
	}

	template<typename T>
	void ReadFile<T>::getRow(Vector<T>& data, unsigned int row) {
		file->seekg(dataStart + (row * sizeof(T) * ColumnNames.size()), std::ios::beg);

		data.clear();
		data.reserve(ColumnNames.size());
		T x;
		for (auto i = 0; i < ColumnNames.size(); i++) {
			if (file->eof())
				throw OutOfRange("Row does not exist in file");
			file->read(reinterpret_cast<char*>(&x), sizeof(T));
			data.pushBack(x);
		}
	}

	template<typename T>
	void ReadFile<T>::getAll(Vector<Vector<T>>& data) noexcept {
		file->seekg(dataStart, std::ios::beg);
		data.clear();
		T x;
		for (auto i = 0; !file->eof(); i++) {
			data.pushBack(Vector<T>(ColumnNames.size()));
			for (auto j = 0; j < ColumnNames.size() && file->read(reinterpret_cast<char*>(&x), sizeof(T)); j++)
				data[i].pushBack(x);
		}
	}
}