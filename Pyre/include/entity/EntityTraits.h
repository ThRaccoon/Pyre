#pragma once

#include <cassert>
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
			// Use '<' (not '<=') when checking for overflow against a mask
			// for both index and generation, because if either of the two is using
			// the full range of its type '<=' will always be true and the check
			// will never trigger.

			assert(index < kIndexMask &&
				"EntityTraits::Pack: index overflows the reserved range, in release build will truncate");

			assert(generation < kGenerationMask &&
				"EntityTraits::Pack: generation overflows the reserved range, in release build will truncate");

			// Cast index to ValueType before shifting by kGenerationBitWidth, because
			// if kGenerationBitWidth >= kIndexBitWidth is UB.

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
