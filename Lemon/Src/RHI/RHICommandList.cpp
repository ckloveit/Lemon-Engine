#include "LemonPCH.h"
#include "RHICommandList.h"
#include "Log/Log.h"

namespace Lemon
{
	RHICommandList* RHICommandList::s_Instance = nullptr;

	RHICommandList::RHICommandList(Renderer* renderer)
	{
		if (s_Instance)
		{
			delete s_Instance;
			s_Instance = nullptr;
		}

		s_Instance = this;
	}

	RHICommandList& RHICommandList::Get()
	{
		LEMON_CORE_ASSERT(s_Instance, "ERROR: RHICommandList::s_Instance is null");
		return *s_Instance;
	}



}