#pragma once

#include <cstdint>

namespace pyre::internal
{
	template<typename>
	struct EntityTraitsConfig;

	template<>
	struct EntityTraitsConfig<uint32_t>
	{
		using ValueType = uint32_t;

		using IndexType = uint32_t;
		using GenerationType = uint16_t;

		static constexpr IndexType kIndexBitWidth = 20;
		static constexpr GenerationType kGenerationBitWidth = 12;
	};

	template<>
	struct EntityTraitsConfig<uint64_t>
	{
		using ValueType = uint64_t;

		using IndexType = uint64_t;
		using GenerationType = uint32_t;

		static constexpr IndexType kIndexBitWidth = 40;
		static constexpr GenerationType kGenerationBitWidth = 24;
	};

	template<typename TraitsConfig>
	class EntityTraits
	{
	public:
		using ValueType = typename TraitsConfig::ValueType;
		using IndexType = typename TraitsConfig::IndexType;
		using GenerationType = typename TraitsConfig::GenerationType;

		static constexpr IndexType kNullIndex = (static_cast<IndexType>(1) << TraitsConfig::kIndexBitWidth) - 1;

		static constexpr ValueType Pack(IndexType index, GenerationType generation)
		{
			return (index << TraitsConfig::kGenerationBitWidth) | (generation & kGenerationMask);
		}

		static constexpr IndexType UnpackIndex(ValueType handle)
		{
			return handle >> TraitsConfig::kGenerationBitWidth;
		}

		static constexpr GenerationType UnpackGeneration(ValueType handle)
		{
			return handle & kGenerationMask;
		}

	private:
		static constexpr GenerationType kGenerationMask = (1 << TraitsConfig::kGenerationBitWidth) - 1;
	};
}
