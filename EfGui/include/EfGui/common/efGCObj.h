#pragma once
#include <stdint.h>
#include <memory>
#include <type_traits>

namespace efgui
{
	struct EfGCObj
	{
	private:
		using T = uint32_t;

	public:
		virtual ~EfGCObj() {}
		T getRefCount() { return mrefCount; }
		void addRef() { mrefCount++; }
		void delRef() { mrefCount--; if (mrefCount == 0) delete this; }
	private:
		T mrefCount = 1;

		template <typename T>friend struct EfGCPtr;
	};

	template<typename T>
	struct EfGCPtr {
		~EfGCPtr() { _del(); }
		EfGCPtr() {}
		EfGCPtr(const EfGCPtr& p) { _copy(p); }
		EfGCPtr(EfGCPtr&& p) { _move(std::move(p)); }

		EfGCPtr(T* p) { mgcObj = p; }

		EfGCPtr& operator = (const EfGCPtr& p) { _copy(p); return *this; }
		EfGCPtr& operator = (EfGCPtr&& p) { _move(std::move(p)); return *this; }

		T* operator ->() {
			return static_cast<T*>(mgcObj);
		}

		T* get() { return mgcObj; }

	private:
		T* mgcObj = nullptr;
		void _copy(const EfGCPtr& p)
		{
			_del();
			mgcObj = p.mgcObj;
			mgcObj->addRef();
		}
		void _move(EfGCPtr&& p)
		{
			_del();
			mgcObj = p.mgcObj;
			p.mgcObj = nullptr;
		}
		void _del()
		{
			if (mgcObj) {
				mgcObj->delRef();
			}
		}
	};

}