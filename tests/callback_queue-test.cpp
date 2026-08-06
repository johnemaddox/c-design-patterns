#include "gtest/gtest.h"
extern "C" {
#include "callback_queue.h"
}

namespace
{

typedef struct {
    uint32_t call_count;
    uint32_t last_payload;
    void *last_context;
} test_deferred_context_t;

static void test_deferred_cb(void *context, uint32_t payload) {
    test_deferred_context_t *ctx = (test_deferred_context_t *)context;
    if (ctx != NULL) {
        ctx->call_count++;
        ctx->last_payload = payload;
        ctx->last_context = context;
    }
}

TEST(CallbackQueueTest, InitSuccessAndFail)
{
    cq_handle_t handle;
    uint8_t buffer[128];

    // Correct init
    EXPECT_TRUE(cq_init(&handle, buffer, sizeof(buffer)));

    // Fail if capacity is not power of two
    EXPECT_FALSE(cq_init(&handle, buffer, 100));

    // Fail on NULLs
    EXPECT_FALSE(cq_init(NULL, buffer, 128));
    EXPECT_FALSE(cq_init(&handle, NULL, 128));
}

TEST(CallbackQueueTest, PostAndDispatch)
{
    cq_handle_t handle;
    uint8_t buffer[128];
    ASSERT_TRUE(cq_init(&handle, buffer, sizeof(buffer)));

    test_deferred_context_t ctx = {};

    // Initially empty
    EXPECT_TRUE(cq_is_empty(&handle));
    EXPECT_FALSE(cq_dispatch_one(&handle));

    // Post one event
    EXPECT_TRUE(cq_post(&handle, test_deferred_cb, &ctx, 12345));
    EXPECT_FALSE(cq_is_empty(&handle));

    // Dispatch it
    EXPECT_TRUE(cq_dispatch_one(&handle));
    EXPECT_EQ(ctx.call_count, 1);
    EXPECT_EQ(ctx.last_payload, 12345);
    EXPECT_EQ(ctx.last_context, &ctx);

    // Now empty again
    EXPECT_TRUE(cq_is_empty(&handle));
}

TEST(CallbackQueueTest, FIFOOrder)
{
    cq_handle_t handle;
    uint8_t buffer[128];
    ASSERT_TRUE(cq_init(&handle, buffer, sizeof(buffer)));

    test_deferred_context_t ctx1 = {};
    test_deferred_context_t ctx2 = {};

    EXPECT_TRUE(cq_post(&handle, test_deferred_cb, &ctx1, 100));
    EXPECT_TRUE(cq_post(&handle, test_deferred_cb, &ctx2, 200));

    // Dispatch first
    EXPECT_TRUE(cq_dispatch_one(&handle));
    EXPECT_EQ(ctx1.call_count, 1);
    EXPECT_EQ(ctx1.last_payload, 100);
    EXPECT_EQ(ctx2.call_count, 0);

    // Dispatch second
    EXPECT_TRUE(cq_dispatch_one(&handle));
    EXPECT_EQ(ctx2.call_count, 1);
    EXPECT_EQ(ctx2.last_payload, 200);

    EXPECT_TRUE(cq_is_empty(&handle));
}

TEST(CallbackQueueTest, QueueFull)
{
    cq_handle_t handle;
    uint8_t buffer[64]; // Small buffer
    ASSERT_TRUE(cq_init(&handle, buffer, sizeof(buffer)));

    test_deferred_context_t ctx = {};

    // Fill the queue
    size_t post_count = 0;
    while (cq_post(&handle, test_deferred_cb, &ctx, post_count)) {
        post_count++;
    }

    // Since buffer size is 64, and size of cb_deferred_event_t is either 12 (32-bit) or 24 (64-bit),
    // and max capacity is 63 bytes, we can post at least 2 events on 64-bit (2*24 = 48 bytes) or
    // 5 events on 32-bit (5*12 = 60 bytes).
    EXPECT_GE(post_count, 2);

    // Future posts should fail
    EXPECT_FALSE(cq_post(&handle, test_deferred_cb, &ctx, 999));

    // Dispatch all posted
    for (size_t i = 0; i < post_count; i++) {
        EXPECT_TRUE(cq_dispatch_one(&handle));
        EXPECT_EQ(ctx.last_payload, i);
    }

    EXPECT_TRUE(cq_is_empty(&handle));
    EXPECT_FALSE(cq_dispatch_one(&handle));
}

} // namespace
