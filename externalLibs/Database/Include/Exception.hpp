/*
Athor: Loke Strøm
*/
#pragma once

namespace Database {
	class Exception {
	private:
		char* msg = nullptr;
	public:
		Exception(const char* ExceptionMsg);
		~Exception();

		char* what() const noexcept;
	};

	class OutOfRange : public Exception {
	public:
		OutOfRange(const char* ExceptionMsg);
	};

	class InvalidArgument : public Exception {
	public:
		InvalidArgument(const char* ExceptionMsg);
	};

	class LengthError : public Exception {
	public:
		LengthError(const char* ExceptionMsg);
	};

	class SystemError : public Exception {
	public:
		SystemError(const char* ExceptionMsg);
	};

	class WriteFileError : public Exception {
	public:
		WriteFileError(const char* ExceptionMsg);
	};
}

