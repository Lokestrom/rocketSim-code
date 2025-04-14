/*
Athor: Loke Strøm
*/
#pragma once

namespace Database {
	class Exception {
	private:
		char* msg = nullptr;
	public:
		Exception(const char* ExceptionMsg) noexcept;
		~Exception() noexcept;

		char* what() const noexcept;
	};

	class OutOfRange : public Exception {
	public:
		OutOfRange(const char* ExceptionMsg) noexcept;
	};

	class InvalidArgument : public Exception {
	public:
		InvalidArgument(const char* ExceptionMsg) noexcept;
	};

	class LengthError : public Exception {
	public:
		LengthError(const char* ExceptionMsg) noexcept;
	};

	class SystemError : public Exception {
	public:
		SystemError(const char* ExceptionMsg) noexcept;
	};

	class WriteFileError : public Exception {
	public:
		WriteFileError(const char* ExceptionMsg) noexcept;
	};
}

