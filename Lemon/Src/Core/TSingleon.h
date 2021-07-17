#pragma once
#include "Core.h"

namespace Lemon
{
	template<class T>
	class LEMON_API TSingleton
	{
	public:
		using Super = TSingleton<T>;
		
		TSingleton() { s_Instance = static_cast<T*>(this); }

		static T* Get() { return s_Instance; }

	protected:
		static inline T* s_Instance = nullptr;
	};
}
