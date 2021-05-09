#pragma once
#include "Core/Core.h"

namespace Lemon
{
	class ResourceArrayInterface
	{
	public:
		virtual ~ResourceArrayInterface() {}

		/**
		 * @return A pointer to the resource data.
		 */
		virtual const void* GetResourceData() const = 0;

		/**
		 * @return size of resource data allocation
		 */
		virtual uint32_t GetResourceDataSize() const = 0;

	};
}