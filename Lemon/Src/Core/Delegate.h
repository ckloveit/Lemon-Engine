#pragma once

#include <typeinfo.h>
#include <list>
#include <vector>

namespace Lemon
{

	// IDelegate

	template<typename ReturnType, typename ...ParamType>
	class IDelegate
	{
	public:
		IDelegate() {}
		virtual ~IDelegate() {}
		virtual bool IsType(const std::type_info& type) = 0;
		virtual ReturnType Execute(ParamType ... params) = 0;
		virtual bool Compare(IDelegate<ReturnType, ParamType...>* delegate) const = 0;
	};


	//StaticDelegate
	template<typename ReturnType, typename ...ParamType>
	class StaticDelegate :
		public IDelegate<ReturnType, ParamType...>
	{
	public:

		typedef  ReturnType(*Func)(ParamType...);

		StaticDelegate(Func _func) : m_Func(_func) { }

		virtual bool IsType(const std::type_info& type) { return typeid(StaticDelegate<ReturnType, ParamType...>) == type; }

		virtual ReturnType Execute(ParamType ... params) override { return mFunc(params...); }

		virtual bool Compare(IDelegate<ReturnType, ParamType ...>* delegate)const
		{
			if (nullptr == delegate || !delegate->IsType(typeid(StaticDelegate<ReturnType, ParamType ...>))) return false;
			StaticDelegate<ReturnType, ParamType ...>* cast = static_cast<StaticDelegate<ReturnType, ParamType ...>*>(delegate);
			return cast->mFunc == mFunc;
		}

		virtual ~StaticDelegate() {}
	private:
		Func m_Func;
	};


	//StaticDelegate Special Template
	template<typename ReturnType, typename ...ParamType>
	class StaticDelegate<ReturnType(*)(ParamType ...)> :
		public IDelegate<ReturnType, ParamType ...>
	{
	public:
		typedef  ReturnType(*Func)(ParamType...);

		StaticDelegate(Func func) : m_Func(func) { }

		virtual bool IsType(const std::type_info& type) { return typeid(StaticDelegate<ReturnType(*)(ParamType ...)>) == type; }

		virtual ReturnType Execute(ParamType ... params) { return mFunc(params...); }

		virtual bool Compare(IDelegate<ReturnType, ParamType ...>* delegate)const
		{
			if (nullptr == delegate || !delegate->IsType(typeid(StaticDelegate<ReturnType(*)(ParamType ...)>))) return false;
			StaticDelegate<ReturnType(*)(ParamType ...)>* cast = static_cast<StaticDelegate<ReturnType(*)(ParamType ...)>*>(delegate);
			return cast->m_Func == m_Func;
		}

		virtual ~StaticDelegate() {}
	private:
		Func m_Func;
	};

	//Class Member Delegate
	template<typename T, typename ReturnType, typename ...ParamType>
	class MethodDelegate :
		public IDelegate<ReturnType, ParamType...>
	{
	public:
		typedef ReturnType(T::* Method)(ParamType...);

		MethodDelegate(T* object, Method method) : m_Object(object), m_Method(method) { }

		virtual bool IsType(const std::type_info& type) { return typeid(MethodDelegate<T, ReturnType, ParamType...>) == type; }

		virtual ReturnType Execute(ParamType...params)
		{
			(m_Object->*m_Method)(params...);
		}

		virtual bool Compare(IDelegate<ReturnType, ParamType...>* delegate) const
		{
			if (nullptr == delegate || !delegate->IsType(typeid(MethodDelegate<ReturnType, ParamType...>))) return false;
			MethodDelegate<ReturnType, ParamType...>* cast = static_cast<MethodDelegate<ReturnType, ParamType...>*>(delegate);
			return cast->m_Object == m_Object && cast->m_Method == m_Method;
		}

		MethodDelegate() {}
		virtual ~MethodDelegate() {}
	private:
		T* m_Object;
		Method m_Method;
	};

	//Class Member Delegate Special Template
	template<typename T, typename ReturnType, typename ...ParamType>
	class MethodDelegate<T, ReturnType(T::*)(ParamType...)> :
		public IDelegate<ReturnType, ParamType...>
	{
	public:
		typedef ReturnType(T::* Method)(ParamType...);

		MethodDelegate(T* object, Method method) : m_Object(object), m_Method(method) { }

		virtual bool IsType(const std::type_info& type) { return typeid(MethodDelegate<T, ReturnType(T::*)(ParamType...)>) == type; }

		virtual ReturnType Execute(ParamType...params)
		{
			return (m_Object->*m_Method)(params...);
		}

		virtual bool Compare(IDelegate<ReturnType, ParamType...>* delegate) const
		{
			if (nullptr == delegate || !delegate->IsType(typeid(MethodDelegate<T, ReturnType(T::*)(ParamType...)>))) return false;
			MethodDelegate<T, ReturnType(T::*)(ParamType...)>* cast = static_cast<MethodDelegate<T, ReturnType(T::*)(ParamType...)>*>(delegate);
			return cast->m_Object == m_Object && cast->m_Method == m_Method;
		}

		MethodDelegate() {}
		virtual ~MethodDelegate() {}
	private:
		T* m_Object;
		Method m_Method;
	};

	//MultiCast Delegate
	template<typename ReturnType, typename ...ParamType>
	class MultiDelegate
	{
	public:
		typedef std::list<IDelegate<ReturnType, ParamType...>*> ListDelegate;
		typedef typename ListDelegate::iterator ListDelegateIterator;
		typedef typename ListDelegate::const_iterator ConstListDelegateIterator;

		MultiDelegate() { }
		~MultiDelegate() { Clear(); }

		bool Empty() const
		{
			for (ConstListDelegateIterator iter = m_ListDelegates.begin(); iter != m_ListDelegates.end(); ++iter)
			{
				if (*iter) return false;
			}
			return true;
		}

		void Clear()
		{
			for (ListDelegateIterator iter = m_ListDelegates.begin(); iter != m_ListDelegates.end(); ++iter)
			{
				if (*iter)
				{
					delete (*iter);
					(*iter) = nullptr;
				}
			}
		}

		template<typename T>
		MultiDelegate<ReturnType, ParamType...>& AddStatic(T func)
		{
			StaticDelegate<T>* delegate = new StaticDelegate<T>(func);
			for (ListDelegateIterator iter = m_ListDelegates.begin(); iter != m_ListDelegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->Compare(delegate))
				{
					delete delegate;
					return *this;
				}
			}
			m_ListDelegates.push_back(delegate);
			return *this;
		}

		template<typename T, typename F>
		MultiDelegate<ReturnType, ParamType...>& AddDynamic(T* object, F func)
		{
			MethodDelegate<T, F>* delegate = new MethodDelegate<T, F>(object, func);

			for (ListDelegateIterator iter = m_ListDelegates.begin(); iter != m_ListDelegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->Compare(delegate))
				{
					delete delegate;
					return *this;
				}
			}
			m_ListDelegates.push_back(delegate);
			return *this;
		}

		MultiDelegate<ReturnType, ParamType...>& Remove(IDelegate<ReturnType, ParamType...>* delegate)
		{
			for (ListDelegateIterator iter = m_ListDelegates.begin(); iter != m_ListDelegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->Compare(delegate))
				{
					if ((*iter) != delegate) delete (*iter);  
					(*iter) = 0;
					break;
				}
			}
			delete delegate;
			return *this;
		}

		std::vector<ReturnType> Broadcast(ParamType... params)
		{
			ListDelegateIterator iter = m_ListDelegates.begin();
			std::vector<ReturnType> results;
			while (iter != m_ListDelegates.end())
			{
				if (nullptr == (*iter))
				{
					iter = m_ListDelegates.erase(iter);
				}
				else
				{
					results.push_back((*iter)->Execute(params...));
					++iter;
				}
			}
			return results;
		}

	private:
		MultiDelegate<ReturnType, ParamType...>(const MultiDelegate& _event) = delete;
		MultiDelegate<ReturnType, ParamType...>& operator=(const MultiDelegate& _event) = delete;

	private:
		ListDelegate m_ListDelegates;
	};

	template< typename ...ParamType>
	class MultiDelegate<void, ParamType...>
	{

	public:

		typedef std::list<IDelegate<void, ParamType...>*> ListDelegate;
		typedef typename ListDelegate::iterator ListDelegateIterator;
		typedef typename ListDelegate::const_iterator ConstListDelegateIterator;

		MultiDelegate() { }
		~MultiDelegate() { Clear(); }

		bool Empty() const
		{
			for (ConstListDelegateIterator iter = m_ListDelegates.begin(); iter != m_ListDelegates.end(); ++iter)
			{
				if (*iter) return false;
			}
			return true;
		}

		void Clear()
		{
			for (ListDelegateIterator iter = m_ListDelegates.begin(); iter != m_ListDelegates.end(); ++iter)
			{
				if (*iter)
				{
					delete (*iter);
					(*iter) = nullptr;
				}
			}
		}

		template<typename T>
		MultiDelegate<void, ParamType...>& AddStatic(T func)
		{
			StaticDelegate<T>* delegate = new StaticDelegate<T>(func);
			for (ListDelegateIterator iter = m_ListDelegates.begin(); iter != m_ListDelegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->Compare(delegate))
				{
					delete delegate;
					return *this;
				}
			}
			m_ListDelegates.push_back(delegate);
			return *this;
		}

		template<typename T, typename F>
		MultiDelegate<void, ParamType...>& AddDynamic(T* object, F func)
		{
			MethodDelegate<T, F>* delegate =  new MethodDelegate<T, F>(object, func);

			for (ListDelegateIterator iter = m_ListDelegates.begin(); iter != m_ListDelegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->Compare(delegate))
				{
					delete delegate;
					return *this;
				}
			}
			m_ListDelegates.push_back(delegate);
			return *this;
		}

		MultiDelegate<void, ParamType...>& Remove(IDelegate<void, ParamType...>* delegate)
		{
			for (ListDelegateIterator iter = m_ListDelegates.begin(); iter != m_ListDelegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->Compare(delegate))
				{
					if ((*iter) != delegate) delete (*iter);
					(*iter) = 0;
					break;
				}
			}
			delete delegate;
			return *this;
		}

		void Broadcast(ParamType... params)
		{
			ListDelegateIterator iter = m_ListDelegates.begin();
			while (iter != m_ListDelegates.end())
			{
				if (nullptr == (*iter))
				{
					iter = m_ListDelegates.erase(iter);
				}
				else
				{
					(*iter)->Execute(params...);
					++iter;
				}
			}
		}
	private:
		MultiDelegate<void, ParamType...>(const MultiDelegate& _event) = delete;
		MultiDelegate<void, ParamType...>& operator=(const MultiDelegate& _event) = delete;

	private:
		ListDelegate m_ListDelegates;
	};

}


