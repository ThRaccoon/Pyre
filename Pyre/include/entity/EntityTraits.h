#pragma once

#include <bit>
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

		static constexpr IndexType kIndexMask = 0xFFFFF; // 20 bits
		static constexpr GenerationType kGenerationMask = 0xFFF; // 12 bits
	};

	template<>
	struct EntityTraitsConfig<uint64_t>
	{
		using ValueType = uint64_t;

		using IndexType = uint64_t;
		using GenerationType = uint32_t;

		static constexpr IndexType kIndexMask = 0xFFFFFFFFFF; // 40 bits
		static constexpr GenerationType kGenerationMask = 0xFFFFFF; // 24 bits
	};

	template<typename TraitsConfig>
	class EntityTraits
	{
		static_assert(TraitsConfig::kIndexMask != 0 && TraitsConfig::kGenerationMask != 0,
			"EntityTraits: masks must not be zero");

		static_assert((TraitsConfig::kIndexMask& (TraitsConfig::kIndexMask + 1)) == 0 &&
			(TraitsConfig::kGenerationMask & (TraitsConfig::kGenerationMask + 1)) == 0,
			"EntityTraits: masks must be contiguous low bits (0xFFF not 0xF0F)");

		static_assert(std::popcount(TraitsConfig::kIndexMask) + std::popcount(TraitsConfig::kGenerationMask) <= sizeof(typename TraitsConfig::ValueType) * 8,
			"EntityTraits: combined mask bit count exceeds ValueType capacity");

	public:
		using ValueType = typename TraitsConfig::ValueType;
		using IndexType = typename TraitsConfig::IndexType;
		using GenerationType = typename TraitsConfig::GenerationType;

		static constexpr IndexType kIndexMask = TraitsConfig::kIndexMask;
		static constexpr GenerationType kGenerationMask = TraitsConfig::kGenerationMask;

		static constexpr std::uint8_t kIndexBitWidth = std::popcount(kIndexMask);
		static constexpr std::uint8_t kGenerationBitWidth = std::popcount(kGenerationMask);

		static constexpr IndexType kTombstone = kIndexMask;

		static constexpr ValueType Pack(IndexType index, GenerationType generation)
		{
			return static_cast<ValueType>(index & kIndexMask) << kGenerationBitWidth | (generation & kGenerationMask);
		}

		static constexpr IndexType UnpackIndex(ValueType handle)
		{
			return static_cast<IndexType>(handle >> kGenerationBitWidth);
		}

		static constexpr GenerationType UnpackGeneration(ValueType handle)
		{
			return static_cast<GenerationType>(handle & kGenerationMask);
		}
	};
}
