#include "gtest/gtest.h"
extern "C" {
#include "ring_buffer.h"
}

namespace
{

TEST(RingBufferInit, FlagOk)
{
    rb_handle_t handle;
    uint8_t buf[8];
    ASSERT_EQ(rb_init(&handle, buf, 8, RB_TYPE_STOP_ON_FULL), RB_OK);
}

TEST(RingBufferInit, FlagLenSize)
{
    rb_handle_t handle;
    uint8_t buf[9];
    ASSERT_EQ(rb_init(&handle, buf, 9, RB_TYPE_STOP_ON_FULL), RB_LEN_ERR);
}

TEST(RingBufferNullPtr, Init)
{
    rb_handle_t handle;
    uint8_t buf[8];
    EXPECT_EQ(rb_init(nullptr, buf, 8, RB_TYPE_STOP_ON_FULL), RB_NULLPTR_ERR);
    EXPECT_EQ(rb_init(&handle, nullptr, 8, RB_TYPE_STOP_ON_FULL), RB_NULLPTR_ERR);
}

TEST(RingBufferNullPtr, Put)
{
    rb_handle_t handle;
    EXPECT_EQ(rb_put(nullptr, 0xAA), RB_NULLPTR_ERR);
    
    handle.data = nullptr;
    EXPECT_EQ(rb_put(&handle, 0xAA), RB_NULLPTR_ERR);
}

TEST(RingBufferNullPtr, Get)
{
    rb_handle_t handle;
    uint8_t val;
    EXPECT_EQ(rb_get(nullptr, &val), RB_NULLPTR_ERR);
    
    handle.data = nullptr;
    EXPECT_EQ(rb_get(&handle, &val), RB_NULLPTR_ERR);
    
    uint8_t buf[8];
    handle.data = buf;
    EXPECT_EQ(rb_get(&handle, nullptr), RB_NULLPTR_ERR);
}

TEST(RingBufferOverwrite, CycleBuffer)
{
    rb_handle_t handle;
    uint8_t buf[8];
    ASSERT_EQ(rb_init(&handle, buf, 8, RB_TYPE_OVERWRITE), RB_OK);

    for (uint8_t data_in = 0; data_in < 16; data_in++)
    {
        EXPECT_EQ(rb_put(&handle, data_in), RB_OK);
    }

    uint8_t data_out = 0;
    for (uint8_t val = 9; val < 16; val++)
    {
        EXPECT_EQ(rb_get(&handle, &data_out), RB_OK);
        EXPECT_EQ(data_out, val);
    }
}

class RingBufferDataStore : public ::testing::Test
{
    public:
        rb_handle_t handle;
        uint8_t buf[8];
        const int max_len = 8;
        uint8_t data_out;

    protected:
        void SetUp() override
        {
            ASSERT_EQ(rb_init(&handle, buf, max_len, RB_TYPE_STOP_ON_FULL), RB_OK);
        }
};

TEST_F(RingBufferDataStore, FillBuffer)
{
    for (int i = 0; i < max_len - 1; i++)
    {
        EXPECT_EQ(rb_put(&handle, i), RB_OK);
    }
    EXPECT_EQ(rb_put(&handle, 0xFFU), RB_FULL);
}

TEST_F(RingBufferDataStore, CycleBuffer)
{
    for (int i = 0; i < max_len * 2; i++)
    {
        EXPECT_EQ(rb_put(&handle, i), RB_OK);

        EXPECT_EQ(rb_get(&handle, &data_out), RB_OK);
        EXPECT_EQ(data_out, i);
    }
}

TEST_F(RingBufferDataStore, ResetBuffer)
{
    EXPECT_EQ(rb_put(&handle, 0xFFU), RB_OK);

    rb_reset(&handle);

    EXPECT_EQ(handle.head_idx, 0);
    EXPECT_EQ(handle.tail_idx, 0);
    EXPECT_EQ(rb_get(&handle, &data_out), RB_EMPTY);
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

TEST(RingBufferSpace, OccupiedAndAvailable)
{
    rb_handle_t handle;
    uint8_t buf[8];
    ASSERT_EQ(rb_init(&handle, buf, 8, RB_TYPE_STOP_ON_FULL), RB_OK);

    // Initial state: empty
    EXPECT_EQ(rb_occupied(&handle), 0);
    EXPECT_EQ(rb_available(&handle), 7); // capacity is max_len - 1 = 7

    // Put some elements
    EXPECT_EQ(rb_put(&handle, 0x11), RB_OK);
    EXPECT_EQ(rb_put(&handle, 0x22), RB_OK);
    EXPECT_EQ(rb_occupied(&handle), 2);
    EXPECT_EQ(rb_available(&handle), 5);

    // Fill the buffer
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(rb_put(&handle, i), RB_OK);
    }
    EXPECT_EQ(rb_occupied(&handle), 7);
    EXPECT_EQ(rb_available(&handle), 0);

    // Get some elements
    uint8_t val;
    EXPECT_EQ(rb_get(&handle, &val), RB_OK);
    EXPECT_EQ(rb_get(&handle, &val), RB_OK);
    EXPECT_EQ(rb_occupied(&handle), 5);
    EXPECT_EQ(rb_available(&handle), 2);

    // Null safety
    EXPECT_EQ(rb_occupied(nullptr), 0);
    EXPECT_EQ(rb_available(nullptr), 0);
}

TEST(RingBufferSpace, WrapAroundAndOverwrite)
{
    rb_handle_t handle;
    uint8_t buf[8]; // max_len = 8, capacity = 7
    ASSERT_EQ(rb_init(&handle, buf, 8, RB_TYPE_STOP_ON_FULL), RB_OK);

    // 1. Fill partially to trigger wrap-around later
    EXPECT_EQ(rb_put(&handle, 1), RB_OK);
    EXPECT_EQ(rb_put(&handle, 2), RB_OK);
    EXPECT_EQ(rb_put(&handle, 3), RB_OK); // occupied = 3, available = 4
    
    // Read 2 elements (tail advances)
    uint8_t out;
    EXPECT_EQ(rb_get(&handle, &out), RB_OK);
    EXPECT_EQ(rb_get(&handle, &out), RB_OK); // occupied = 1, available = 6

    // Put 5 more elements to force head index wrap-around past the 8-byte boundary
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(rb_put(&handle, i), RB_OK);
    }
    // Now tail is at index 2, head has wrapped to index 7 (occupied = 6, available = 1)
    EXPECT_EQ(rb_occupied(&handle), 6);
    EXPECT_EQ(rb_available(&handle), 1);

    // 2. Test Overwrite Mode
    rb_handle_t rb_ovr;
    uint8_t buf_ovr[8];
    ASSERT_EQ(rb_init(&rb_ovr, buf_ovr, 8, RB_TYPE_OVERWRITE), RB_OK);

    // Write 10 elements (more than capacity of 7)
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(rb_put(&rb_ovr, i), RB_OK);
    }
    // Under overwrite mode, the buffer remains completely full, capping unread count at 7
    EXPECT_EQ(rb_occupied(&rb_ovr), 7);
    EXPECT_EQ(rb_available(&rb_ovr), 0);

    // Read 3 elements
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(rb_get(&rb_ovr, &out), RB_OK);
    }
    // Check remaining
    EXPECT_EQ(rb_occupied(&rb_ovr), 4);
    EXPECT_EQ(rb_available(&rb_ovr), 3);
}

} // namespace
