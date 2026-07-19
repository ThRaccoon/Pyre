#pragma once

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "entity/EntityTraits.h"

namespace pyre::internal
{
	template<typename Handle, typename Traits = EntityTraits<EntityTraitsConfig<Handle>>>
	class SparseSet
	{
	public:
		static constexpr size_t kInitialCapacity = 16;

		SparseSet(size_t initialCapacity = kInitialCapacity)
			: m_initialCapacity(initialCapacity)
		{
			m_dense.reserve(m_initialCapacity);
			m_sparse.assign(m_initialCapacity, Traits::kNullIndex);
		}

		SparseSet(const SparseSet& other) = delete;
		SparseSet& operator=(const SparseSet& other) = delete;
		virtual ~SparseSet() = default;

		void Insert(Handle handle)
		{
			typename Traits::IndexType index = Traits::UnpackIndex(handle);

			if (index > m_sparse.size())
			{
				std::cerr << "SparseSet::Insert: handle (" << handle << ") is out of range\n";
				abort();
			}

			bool willResize = m_dense.size() == m_dense.capacity();

			if (index == m_sparse.size() && !willResize)
			{
				std::cerr << "SparseSet::Insert: handle (" << handle << ") is out of range\n";
				abort();
			}

			if (index < m_sparse.size() && m_sparse[index] != Traits::kNullIndex)
			{
				std::cerr << "SparseSet::Insert: handle (" << handle << ") already exists\n";
				abort();
			}

			m_dense.push_back(handle);

			if (willResize)
			{
				m_sparse.resize(m_dense.capacity(), Traits::kNullIndex);
			}

			m_sparse[Traits::UnpackIndex(handle)] = m_dense.size() - 1;
		}

		bool Contains(Handle handle) const
		{
			AssertHandleInRange(handle);

			return m_sparse[Traits::UnpackIndex(handle)] != Traits::kNullIndex;
		}

		void Remove(Handle handle)
		{
			AssertHandleInRange(handle);

			typename Traits::IndexType index = Traits::UnpackIndex(handle);

			if (m_sparse[index] == Traits::kNullIndex)
			{
				std::cerr << "SparseSet::Remove: handle (" << handle << ") does not exist\n";
				abort();
			}

			OnSwapAndPop(index);

			m_dense[m_sparse[index]] = m_dense.back();
			m_sparse[Traits::UnpackIndex(m_dense.back())] = m_sparse[index];
			m_sparse[index] = Traits::kNullIndex;

			m_dense.pop_back();
		}

		void Clear()
		{
			m_dense.clear();
			m_sparse.clear();
		}

		void Reset()
		{
			std::vector<Handle>().swap(m_dense);
			std::vector<typename Traits::IndexType>().swap(m_sparse);
		}

	protected:
		virtual void OnSwapAndPop(typename Traits::IndexType) {}

		typename Traits::IndexType DenseIndexOf(Handle handle) const
		{
			return m_sparse[Traits::UnpackIndex(handle)];
		}

	private:
		void AssertHandleInRange(Handle handle) const
		{
			if (Traits::UnpackIndex(handle) >= m_sparse.size())
			{
				std::cerr << "SparseSet::AssertHandleInRange: handle (" << handle << ") is out of range\n";
				abort();
			}
		}

		size_t m_initialCapacity;

		std::vector<Handle> m_dense;
		std::vector<typename Traits::IndexType> m_sparse;
	};
}
