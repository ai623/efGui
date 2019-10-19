#pragma once
#include <stdint.h>
#include <memory>

namespace efgui
{
	template <typename T>
	struct EfArray
	{
		~EfArray() { delete[] mdata; }

		EfArray() = default;
		EfArray(const EfArray& arr) { _copy(arr); }
		EfArray(EfArray&& arr) { _move(std::move(arr)); }

		EfArray& operator = (const EfArray& arr) { _copy(arr); return *this; }
		EfArray&& operator = (EfArray&& arr) { _move(std::move(arr)); return *this; }

		T& operator[] (size_t index) { return mdata[index]; }

		void init(size_t size) { delete[]mdata; msize = size; mdata = new T[msize]; }

		T* getData() { return mdata; }
		size_t getSize() const { return msize; }

	protected:
		T* mdata = nullptr;
		size_t msize = 0;

		//void _init() { delete[] mdata; mdata = nullptr; msize = 0; }
		void _copy(const EfArray& arr) 
		{ 
			init(arr.msize); 
			memcpy(mdata, arr.mdata, msize); 
		}

		void _move(EfArray&& arr) { 
			delete[]mdata; 
			mdata = arr.mdata; 
			arr.mdata = nullptr; 
			msize = arr.msize; 
			arr.msize = 0; 
		}

	};

	template <typename T>
	struct EfMatrix : EfArray<T>
	{
		EfMatrix() = default;
		EfMatrix(const EfMatrix& mat) { _copy(mat); }
		EfMatrix(EfMatrix&& mat) {_move(std::move(mat));}

		EfMatrix& operator = (const EfMatrix& mat) { _copy(mat); }
		EfMatrix& operator = (EfMatrix&& mat) { _move(std::move(mat)); }

		void init(size_t row, size_t column) { mrows = row; mcolumns = column; EfArray<T>::init(mrows * mcolumns); }

		T* operator[] (size_t i) { return mdata + i*mcolumns; }

		size_t getRows() const { return mrows; }
		size_t getColumns() const { return mcolumns; }
	protected:
		size_t mrows = 0;
		size_t mcolumns = 0;

		void _copy(const EfMatrix& mat) \
		{ 
			init(mat.mrows, mat.mcolumns); 
			memcpy(mdata, mat.mdata, msize); 
		}

		void _move(EfMatrix&& mat)
		{
			delete[]mdata;
			mdata = arr.mdata;
			arr.mdata = nullptr;
			msize = arr.msize;
			arr.msize = 0;
			mrows = mat.mrows;
			mat.mrows = 0;
			mcolumns = mat.mcolumns;
			mat.mcolumns = 0;
		}
	};
}