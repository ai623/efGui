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
		T getRefCount() { return refCount; }
	private:
		void addRef() { refCount++; }
		void delRef() { refCount--; }
		T refCount = 1;

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
				if (mgcObj->getRefCount() == 0) {
					delete mgcObj;
					mgcObj = nullptr;
				}
			}
		}
	};

}