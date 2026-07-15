#include "gtest/gtest.h"
extern "C" {
#include "ring_buffer.h"
}

namespace
{

TEST(RingBufferInit, FlagOk)
{
    rb_handle_t rb;
    uint8_t buf[8];
    ASSERT_EQ(rb_init(&rb, buf, 8, RB_TYPE_STOP_ON_FULL), RB_OK);
}

TEST(RingBufferInit, FlagLenSize)
{
    rb_handle_t rb;
    uint8_t buf[9];
    ASSERT_EQ(rb_init(&rb, buf, 9, RB_TYPE_STOP_ON_FULL), RB_LEN_ERR);
}

TEST(RingBufferOverwrite, CycleBuffer)
{
    rb_handle_t rb;
    uint8_t buf[8];
    ASSERT_EQ(rb_init(&rb, buf, 8, RB_TYPE_OVERWRITE), RB_OK);

    for (uint8_t data_in = 0; data_in < 16; data_in++)
    {
        EXPECT_EQ(rb_put(&rb, data_in), RB_OK);
    }

    uint8_t data_out = 0;
    for (uint8_t val = 9; val < 16; val++)
    {
        EXPECT_EQ(rb_get(&rb, &data_out), RB_OK);
        EXPECT_EQ(data_out, val);
    }
}

class RingBufferDataStore : public ::testing::Test
{
    public:
        rb_handle_t rb;
        uint8_t buf[8];
        const int max_len = 8;
        uint8_t data_out;

    protected:
        void SetUp() override
        {
            ASSERT_EQ(rb_init(&rb, buf, max_len, RB_TYPE_STOP_ON_FULL), RB_OK);
        }
};

TEST_F(RingBufferDataStore, FillBuffer)
{
    for (int i = 0; i < max_len - 1; i++)
    {
        EXPECT_EQ(rb_put(&rb, i), RB_OK);
    }
    EXPECT_EQ(rb_put(&rb, 0xFFU), RB_FULL);
}

TEST_F(RingBufferDataStore, CycleBuffer)
{
    for (int i = 0; i < max_len * 2; i++)
    {
        EXPECT_EQ(rb_put(&rb, i), RB_OK);

        EXPECT_EQ(rb_get(&rb, &data_out), RB_OK);
        EXPECT_EQ(data_out, i);
    }
}

TEST_F(RingBufferDataStore, ResetBuffer)
{
    EXPECT_EQ(rb_put(&rb, 0xFFU), RB_OK);

    rb_reset(&rb);

    EXPECT_EQ(rb.head_idx, 0);
    EXPECT_EQ(rb.tail_idx, 0);
    EXPECT_EQ(rb_get(&rb, &data_out), RB_EMPTY);
}

TEST(RingBufferMultiple, ConcurrentInstances)
{
    rb_handle_t uart_rx_rb;
    uint8_t rx_buf[16];
    rb_handle_t uart_tx_rb;
    uint8_t tx_buf[8];

    ASSERT_EQ(rb_init(&uart_rx_rb, rx_buf, 16, RB_TYPE_OVERWRITE), RB_OK);
    ASSERT_EQ(rb_init(&uart_tx_rb, tx_buf, 8, RB_TYPE_STOP_ON_FULL), RB_OK);

    // Put data in RX buffer
    EXPECT_EQ(rb_put(&uart_rx_rb, 0xA1), RB_OK);
    EXPECT_EQ(rb_put(&uart_rx_rb, 0xA2), RB_OK);

    // Put data in TX buffer
    EXPECT_EQ(rb_put(&uart_tx_rb, 0xB1), RB_OK);

    // Verify RX buffer contents
    uint8_t val;
    EXPECT_EQ(rb_get(&uart_rx_rb, &val), RB_OK);
    EXPECT_EQ(val, 0xA1);
    EXPECT_EQ(rb_get(&uart_rx_rb, &val), RB_OK);
    EXPECT_EQ(val, 0xA2);

    // Verify TX buffer contents
    EXPECT_EQ(rb_get(&uart_tx_rb, &val), RB_OK);
    EXPECT_EQ(val, 0xB1);
}

} // namespace
