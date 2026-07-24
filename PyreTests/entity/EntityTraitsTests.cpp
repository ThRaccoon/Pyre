#include <gtest/gtest.h>

#include <cstdint>

#include "entity/EntityTraits.h"

using namespace pyre::internal;

//
// For general test conventions read TESTING.md
//
// Shorthands used in this Tests.cpp:
// Idx -> Index
// Gen -> Generation
//

template<typename T>
class EntityTraitsTypedTest : public testing::Test
{
public:
	using Traits = EntityTraits<EntityTraitsConfig<T>>;

	using ValueType = typename Traits::ValueType;
	using IndexType = typename Traits::IndexType;
	using GenerationType = typename Traits::GenerationType;
};

using MyTypes = testing::Types<uint32_t, uint64_t>;
TYPED_TEST_SUITE(EntityTraitsTypedTest, MyTypes);

//
// Pack tests:
//

TYPED_TEST(EntityTraitsTypedTest, Pack_Control_Handle)
{
	using Traits = typename TestFixture::Traits;

	typename Traits::IndexType index = 12;
	typename Traits::GenerationType generation = 3;

	typename Traits::ValueType expected = (index << Traits::kGenerationBitWidth) | generation;

	EXPECT_EQ(Traits::Pack(index, generation), expected);
}


TYPED_TEST(EntityTraitsTypedTest, Pack_IdxAndGenAtZero_PackedToZeroHandle)
{
	using Traits = typename TestFixture::Traits;

	typename Traits::IndexType index = 0;
	typename Traits::GenerationType generation = 0;

	typename Traits::ValueType expected = (index << Traits::kGenerationBitWidth) | generation;

	EXPECT_EQ(Traits::Pack(index, generation), expected);
}


TYPED_TEST(EntityTraitsTypedTest, Pack_IdxAtMaxValid_IdxDoesNotTruncateOrBleedIntoGen)
{
	using Traits = typename TestFixture::Traits;

	typename Traits::IndexType index = Traits::kIndexMask - 1;
	typename Traits::GenerationType generation = 0;

	typename Traits::ValueType expected = (index << Traits::kGenerationBitWidth) | generation;

	EXPECT_EQ(Traits::Pack(index, generation), expected);
}


TYPED_TEST(EntityTraitsTypedTest, Pack_GenAtMax_GenDoesNotTruncateOrBleedIntoIdx)
{
	using Traits = typename TestFixture::Traits;

	typename Traits::IndexType index = 0;
	typename Traits::GenerationType generation = Traits::kGenerationMask;

	typename Traits::ValueType expected = (index << Traits::kGenerationBitWidth) | generation;

	EXPECT_EQ(Traits::Pack(index, generation), expected);
}


TYPED_TEST(EntityTraitsTypedTest, Pack_IdxAtMaxValidGenAtMax_PackedToMaxHandle)
{
	using Traits = typename TestFixture::Traits;

	typename Traits::IndexType index = Traits::kIndexMask - 1;
	typename Traits::GenerationType generation = Traits::kGenerationMask;

	typename Traits::ValueType expected = (index << Traits::kGenerationBitWidth) | generation;

	EXPECT_EQ(Traits::Pack(index, generation), expected);
}


TYPED_TEST(EntityTraitsTypedTest, Pack_IdxOverflowsByOne_IdxTruncatedToZeroNoBleedIntoGen)
{
	using Traits = typename TestFixture::Traits;

	typename Traits::IndexType index = Traits::kIndexMask + 1;
	typename Traits::GenerationType generation = 0;

	EXPECT_EQ(Traits::Pack(index, generation), 0);
}


TYPED_TEST(EntityTraitsTypedTest, Pack_GenOverflowsByOne_GenTruncatedToZeroNoBleedIntoIdx)
{
	using Traits = typename TestFixture::Traits;

	typename Traits::IndexType index = 0;
	typename Traits::GenerationType generation = Traits::kGenerationMask + 1;

	EXPECT_EQ(Traits::Pack(index, generation), 0);
}

//
// UnpackIndex tests:
//

TYPED_TEST(EntityTraitsTypedTest, UnpackIndex_Control_Idx)
{
	using Traits = typename TestFixture::Traits;

	typename Traits::ValueType handle = Traits::Pack(1, 0);

	EXPECT_EQ(Traits::UnpackIndex(handle), 1);
}

TYPED_TEST(EntityTraitsTypedTest, UnpackIndex_HandleIdxAtZero_Zero)
{
	using Traits = typename TestFixture::Traits;

	typename Traits::ValueType handle = Traits::Pack(0, 0);

	EXPECT_EQ(Traits::UnpackIndex(handle), 0);
}

TYPED_TEST(EntityTraitsTypedTest, UnpackIndex_HandleIdxAtMaxValid_IdxMaxValid)
{
	using Traits = typename TestFixture::Traits;

	typename Traits::ValueType handle = Traits::Pack(Traits::kIndexMask - 1, 0);

	EXPECT_EQ(Traits::UnpackIndex(handle), Traits::kIndexMask - 1);
}

//
// UnpackGeneration tests:
//

TYPED_TEST(EntityTraitsTypedTest, UnpackGeneration_Control_Gen)
{
	using Traits = typename TestFixture::Traits;

	typename Traits::ValueType handle = Traits::Pack(0, 1);

	EXPECT_EQ(Traits::UnpackGeneration(handle), 1);
}

TYPED_TEST(EntityTraitsTypedTest, UnpackGeneration_HandleGenAtZero_Zero)
{
	using Traits = typename TestFixture::Traits;

	typename Traits::ValueType handle = Traits::Pack(0, 0);

	EXPECT_EQ(Traits::UnpackGeneration(handle), 0);
}

TYPED_TEST(EntityTraitsTypedTest, UnpackGeneration_HandleGenAtMax_GenMax)
{
	using Traits = typename TestFixture::Traits;

	typename Traits::ValueType handle = Traits::Pack(0, Traits::kGenerationMask);

	EXPECT_EQ(Traits::UnpackGeneration(handle), Traits::kGenerationMask);
}
