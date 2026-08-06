#include "gtest/gtest.h"
extern "C" {
#include "callback.h"
}

namespace
{

typedef struct {
    uint32_t call_count;
    uint32_t last_event_data;
} test_context_t;

static void test_callback_func(void *context, uint32_t event_data) {
    test_context_t *ctx = (test_context_t *)context;
    if (ctx != NULL) {
        ctx->call_count++;
        ctx->last_event_data = event_data;
    }
}

TEST(CallbackTest, RegisterAndFire)
{
    cb_handle_t handle;
    test_context_t ctx = { .call_count = 0, .last_event_data = 0 };

    cb_register(&handle, test_callback_func, &ctx);
    cb_fire(&handle, 42);

    EXPECT_EQ(ctx.call_count, 1);
    EXPECT_EQ(ctx.last_event_data, 42);

    cb_fire(&handle, 100);
    EXPECT_EQ(ctx.call_count, 2);
    EXPECT_EQ(ctx.last_event_data, 100);
}

TEST(CallbackTest, NullCallback)
{
    cb_handle_t handle;
    
    // Register NULL callback
    cb_register(&handle, NULL, NULL);
    
    // Firing should not crash
    cb_fire(&handle, 42);
}

TEST(CallbackTest, NullRegistry)
{
    // APIs should handle NULL registry gracefully without crashing
    cb_register(NULL, test_callback_func, NULL);
    cb_fire(NULL, 42);
}

} // namespace
