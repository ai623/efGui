#pragma once
#include <stdint.h>
#include <memory>

namespace efgui
{
	template<typename T>
	struct EfBuffer
	{
		T* data = nullptr;
		size_t size = 0;

		~EfBuffer() { delete[] data; }

		EfBuffer() {}
		EfBuffer(size_t size) { data = new T[size], this->size = size; }
		EfBuffer(const EfBuffer<T>& buff) { _copy(buff); }
		EfBuffer(EfBuffer<T>&& buff) { _move(std::move(buff)); }

		EfBuffer<T>& operator = (const EfBuffer<T>& buff) { _copy(buff); return *this; }
		EfBuffer<T>& operator = (EfBuffer<T>&& buff) { _move(std::move(buff)); return *this; }

		T& operator[] (size_t index) { return data[index]; }

		void init(size_t size) { delete[]data; data = new T[size]; this->size = size; }
	private:
		void _copy(const EfBuffer<T>& buff)
		{
			if (size != buff.size) {
				delete[] data;
				data = new T[buff.size];
			}
			memcpy(data, buff.data, sizeof(T)*size);
		}

		void _move(EfBuffer<T>&& buff) 
		{
			data = buff.data;
			buff.data = nullptr;
			size = buff.size;
			buff.size = 0;
		}
	};

	template<typename T>
	struct EfBuffer2D
	{
		T* data = nullptr;
		unsigned int rows = 0;
		unsigned int cols = 0;

		~EfBuffer2D() { delete[] data; }

		EfBuffer2D(){}
		EfBuffer2D(unsigned int cols) 
		{
			data = new T[cols]; 
			this->cols = cols;
			this->rows = 1; 
		}
		EfBuffer2D(unsigned int rows, unsigned int cols)
		{
			data = new T[rows * cols];
			this->rows = rows;
			this->cols = cols;
		}

		EfBuffer2D(const EfBuffer2D<T>& buff) { _copy(buff); }
		EfBuffer2D(EfBuffer2D<T>&& buff) { _move(std::move(buff)); }

		EfBuffer2D<T>& operator = (const EfBuffer2D<T>& buff) { _copy(buff); return *this; }
		EfBuffer2D<T>& operator = (EfBuffer2D<T>&& buff) { _move(std::move(buff)); return*this; }

		T* operator[] (unsigned int i) {return data + i;}

		size_t size() const { return rows * cols; }
		size_t byteSize() const { return sizeof(T) * rows * cols; }

		size_t rowByteSize() const { return cols * sizeof(T); }
	private:
		void _copy(const EfBuffer2D<T>& buff) {
			auto thisSize = size();
			auto buffSize = buff.size();
			if (thisSize != buffSize) {
				delete[] data;
				data = new T[buffSize];
			}
			rows = buff.rows;
			cols = buff.cols;
			memcpy(data, buff.data, sizeof(T) * buffSize);
		}

		void _move(EfBuffer2D<T>&& buff) {
			delete[] data;
			data = buff.data;
			rows = buff.rows;
			cols = buff.cols;
			buff.data = nullptr;
			buff.rows = 0;
			buff.cols = 0;
		}
	};

	template<typename T, size_t Size>
	struct EfArray
	{
		

		T* data() { return arr; }
		size_t size() const { return Size; }

		T& operator[] (size_t index) { return arr[index]; }
	private:
		T arr[Size];
	};

}