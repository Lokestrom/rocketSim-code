#pragma once

template<typename T>
class PointerView {
	PointerView() = delete;
public:
	~PointerView() {};

	PointerView(T* p)
		: ptr(p)
	{
	}
	PointerView(const PointerView& other)
		: ptr(other.ptr)
	{
	}
	PointerView& operator=(const PointerView& other) {
		ptr = other.ptr;
		return *this;
	}

	PointerView(const Pointer& poinet)
		: ptr(poinet.ptr)
	{
	}

	PointerView& operator=(const Pointer& poinet) {
		ptr = poinet.ptr;
		return *this;
	}

	Pointer& operator*() {
		return *ptr;
	}

	Pointer operator->() {
		return ptr;
	}

	T* ptr;
};

template<typename T>
class Pointer
{
	Pointer() = delete;
public:
	Pointer(T* p)
		: ptr(p)
	{ }

	Pointer(const Pointer& other) = delete;
	Pointer& operator=(const Pointer& other) = delete;

	Pointer(Pointer&& other) noexcept
		: ptr(other.ptr)
	{
		other.ptr = nullptr;
	}
	Pointer& operator=(Pointer&& other) noexcept {
		ptr = other.ptr;
		other.ptr = nullptr;
		return *this;
	}

	~Pointer() {
		delete ptr;
	};

	Pointer& operator*() {
		return *ptr;
	}

	Pointer operator->() {
		return ptr;
	}

	T* ptr;
};