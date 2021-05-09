#pragma once
#include "Core/Core.h"
#include "ResourceArray.h"
#include <vector>

namespace Lemon
{
	template<typename ElementType>
	class TResourceArray : public ResourceArrayInterface
	{
	public:
		/**
		 * @return A pointer to the resource data.
		 */
		virtual const void* GetResourceData() const override { return m_Resource.data(); }

		/**
		 * @return size of resource data allocation
		 */
		virtual uint32_t GetResourceDataSize() const override { return (uint32_t)m_Resource.size(); }

		//============Container function=================//
		template<typename... Args>
		decltype(auto) EmplaceBack(Args&& ... args)
		{
			return m_Resource.emplace_back(std::forward<Args>(args)...);
		}

		void PushBack(const ElementType& value)
		{
			EmplaceBack(value);
		}
	private:
		std::vector<ElementType> m_Resource;
	};
}