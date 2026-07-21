#pragma once

#include <cassert>
#include <cstdint>

namespace pyre::internal
{
	template<typename>
	struct EntityTraitsConfig;

	// For EntityTraits to work properly, when choosing
	// uint size for IndexType and GenerationType always choose
	// the smallest one that fits the corresponding bit width.

	template<>
	struct EntityTraitsConfig<uint32_t>
	{
		using ValueType = uint32_t;

		using IndexType = uint32_t;
		using GenerationType = uint16_t;

		static constexpr std::uint8_t kIndexBitWidth = 20;
		static constexpr std::uint8_t kGenerationBitWidth = 12;
	};

	template<>
	struct EntityTraitsConfig<uint64_t>
	{
		using ValueType = uint64_t;

		using IndexType = uint64_t;
		using GenerationType = uint32_t;

		static constexpr std::uint8_t kIndexBitWidth = 40;
		static constexpr std::uint8_t kGenerationBitWidth = 24;
	};

	template<typename TraitsConfig>
	class EntityTraits
	{
		static_assert(TraitsConfig::kIndexBitWidth >= 1,
			"EntityTraits: kIndexBitWidth must be >= 1");

		static_assert(TraitsConfig::kGenerationBitWidth >= 1,
			"EntityTraits: kGenerationBitWidth must be >= 1");

		static_assert(TraitsConfig::kIndexBitWidth + TraitsConfig::kGenerationBitWidth <= sizeof(typename TraitsConfig::ValueType) * 8,
			"EntityTraits: kIndexBitWidth + kGenerationBitWidth must be <= ValueType bit width");

	public:
		using ValueType = typename TraitsConfig::ValueType;
		using IndexType = typename TraitsConfig::IndexType;
		using GenerationType = typename TraitsConfig::GenerationType;

		// Cast the 1 to the target type before shifting, because 1 is
		// a 32-bit int and shifting it 32 or more bits is UB.

		static constexpr IndexType kIndexMask =
			(static_cast<IndexType>(1) << TraitsConfig::kIndexBitWidth) - 1;

		static constexpr GenerationType kGenerationMask =
			(static_cast<GenerationType>(1) << TraitsConfig::kGenerationBitWidth) - 1;

		static constexpr IndexType kNullIndex = kIndexMask;

		static constexpr ValueType Pack(IndexType index, GenerationType generation)
		{
			// Use '<' (not '<='), when checking for overflow against a mask
			// for both index and generation, because if the check uses '<='
			// and index or generation reaches the max value before wrapping to 0,
			// it's not possible to detect the overflow. This way the last valid
			// number is lost, but this allows overflow detection and having
			// sentinels like kNullIndex.

			assert(index < kIndexMask &&
				"EntityTraits::Pack: index overflows the reserved range, in release build will truncate");

			assert(generation < kGenerationMask &&
				"EntityTraits::Pack: generation overflows the reserved range, in release build will truncate");

			// Cast index to ValueType before shifting by kGenerationBitWidth,
			// because if the type of generation is >= than the type of index,
			// the shift is UB.

			return static_cast<ValueType>(index & kIndexMask) << TraitsConfig::kGenerationBitWidth | (generation & kGenerationMask);
		}

		static constexpr IndexType UnpackIndex(ValueType handle)
		{
			return handle >> TraitsConfig::kGenerationBitWidth;
		}

		static constexpr GenerationType UnpackGeneration(ValueType handle)
		{
			return handle & kGenerationMask;
		}
	};
}
