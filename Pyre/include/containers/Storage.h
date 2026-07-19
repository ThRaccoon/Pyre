#pragma once

#include <cstdlib>
#include <iostream>
#include <vector>

#include "containers/SparseSet.h"

namespace pyre::internal
{
	template<typename Element, typename Handle, typename Traits = EntityTraits<EntityTraitsConfig<Handle>>>
	class Storage : public SparseSet<Handle, Traits>
	{
	public:
		using Base = SparseSet<Handle, Traits>;

		Storage(size_t initialCapacity = Base::kInitialCapacity)
			: Base(initialCapacity)
		{
			m_elements.reserve(initialCapacity);
		}

		void Insert(Element& element, Handle handle)
		{
			Base::Insert(handle);

			m_elements.push_back(element);
		}

		bool Contains(Handle handle) const
		{
			return Base::Contains(handle);
		}

		Element& Get(Handle handle)
		{
			if (!Contains(handle))
			{
				std::cerr << "Storage::Get: handle (" << handle << ") does not exist\n";
				abort();
			}

			return m_elements[Base::DenseIndexOf(handle)];
		}

		const Element& Get(Handle handle) const
		{
			if (!Contains(handle))
			{
				std::cerr << "Storage::Get: handle (" << handle << ") does not exist\n";
				abort();
			}

			return m_elements[Base::DenseIndexOf(handle)];
		}

		void Remove(Handle handle)
		{
			Base::Remove(handle);
		}

		void Clear()
		{
			Base::Clear();

			m_elements.clear();
		}

		void Reset()
		{
			Base::Reset();

			std::vector<Element>().swap(m_elements);
		}

	protected:
		void OnSwapAndPop(typename Traits::IndexType index) override
		{
			m_elements[index] = m_elements.back();
			m_elements.pop_back();
		}

	private:
		std::vector<Element> m_elements;
	};
}
