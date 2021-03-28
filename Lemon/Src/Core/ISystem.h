#pragma once
#include <memory>
#include "Core.h"

namespace Lemon
{
	class Engine;

	class LEMON_API ISystem : public std::enable_shared_from_this<ISystem>
	{
	public:
		ISystem(Engine* engine) :m_Engine(engine)
		{}
		virtual ~ISystem() = default;

		virtual bool Initialize() { return true; }
		virtual void Tick(float deltaTime) {}

		template <typename T>
		Ref<T> GetRefPtr() { return dynamic_pointer_cast<T>(shared_from_this()); }

		Engine* GetEngine() const { return m_Engine; }
	protected:
		Engine* m_Engine;
	};

	template<typename T>
	constexpr void ValidateSystemType() { static_assert(std::is_base_of<ISystem, T>::value, "Provided type does not implement ISubystem"); }
}
