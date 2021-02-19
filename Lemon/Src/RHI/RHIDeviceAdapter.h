#pragma once
#include <string>

namespace Lemon
{
	struct RHIDeviceAdapter
	{
	public:
		RHIDeviceAdapter(const uint32_t venderId, const char* name, const uint32_t memory, void* adapter)
		{
			this->VendorId = venderId;
			this->Memory = memory / 1024 / 1024; // mb
			this->Name = name;
			this->Adapter = adapter;
		}

		const std::string& GetName() const { return Name; }
		uint32_t GetMemory() const { return Memory; }
		void* GetAdapter() const { return Adapter; }
	private:
		uint32_t VendorId = 0;// unique identifier of the vendor
		std::string Name = "Unknown";
		uint32_t Memory = 0;
		void* Adapter = nullptr;
	};
}