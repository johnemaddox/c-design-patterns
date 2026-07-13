#include "gtest/gtest.h"
extern "C" {
#include "ring_buffer.h"
}

namespace
{

TEST(RingBufferInit, FlagOk)
{
    rb_handle_t *rb;
    ASSERT_EQ(rb_init(&rb, 8, RB_TYPE_STOP_ON_FULL), RB_OK);
    rb_free(&rb);
}

TEST(RingBufferInit, FlagLenSize)
{
    rb_handle_t *rb;
    // NOTE: memory is not allocated when this flag is returned.
    ASSERT_EQ(rb_init(&rb, 9, RB_TYPE_STOP_ON_FULL), RB_LEN_ERR);
}

TEST(RingBufferOverwrite, CycleBuffer)
{
    rb_handle_t *rb;
    ASSERT_EQ(rb_init(&rb, 8, RB_TYPE_OVERWRITE), RB_OK);

    for (uint8_t data_in=0; data_in < 16; data_in++)
    {
        EXPECT_EQ(rb_put(rb, data_in), RB_OK);
    }

    uint8_t data_out = 0;
    for (uint8_t val=9; val < 16; val++)
    {
        EXPECT_EQ(rb_get(rb, &data_out), RB_OK);
        EXPECT_EQ(data_out, val);
    }

    rb_free(&rb);
}

class RingBufferDataStore : public ::testing::Test
{
    public:
        rb_handle_t *rb;
        const int max_len = 8;
        uint8_t data_out;

    private:
        void SetUp() override
        {
            ASSERT_EQ(rb_init(&rb, max_len, RB_TYPE_STOP_ON_FULL), RB_OK);
        }

        void TearDown() override
        {
            rb_free(&rb);
        }
};

TEST_F(RingBufferDataStore, FillBuffer)
{
    for (int i=0; i < max_len-1; i++)
    {
        EXPECT_EQ(rb_put(rb, i), RB_OK);
    }
    EXPECT_EQ(rb_put(rb, 0xFFU), RB_FULL);
}

TEST_F(RingBufferDataStore, CycleBuffer)
{
    for (int i=0; i < max_len*2; i++)
    {
        EXPECT_EQ(rb_put(rb, i), RB_OK);

        EXPECT_EQ(rb_get(rb, &data_out), RB_OK);
        EXPECT_EQ(data_out, i);
    }
}

TEST_F(RingBufferDataStore, ResetBuffer)
{
    EXPECT_EQ(rb_put(rb, 0xFFU), RB_OK);

    rb_reset(rb);

    EXPECT_EQ(rb->head_idx, 0);
    EXPECT_EQ(rb->tail_idx, 0);
    EXPECT_EQ(rb_get(rb, &data_out), RB_EMPTY);
}

}
