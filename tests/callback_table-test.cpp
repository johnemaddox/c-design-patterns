#include "gtest/gtest.h"
extern "C" {
#include "callback_table.h"
}

namespace
{

typedef struct {
    uint32_t started_calls;
    uint32_t data_calls;
    size_t last_data_size;
    const uint8_t *last_data_buffer;
    uint32_t error_calls;
    int32_t last_error_code;
} test_table_context_t;

// Define static callback implementations
static void test_started_cb(void *context) {
    test_table_context_t *ctx = (test_table_context_t *)context;
    if (ctx != NULL) {
        ctx->started_calls++;
    }
}

static void test_data_cb(void *context, const uint8_t *buffer, size_t size) {
    test_table_context_t *ctx = (test_table_context_t *)context;
    if (ctx != NULL) {
        ctx->data_calls++;
        ctx->last_data_size = size;
        ctx->last_data_buffer = buffer;
    }
}

static void test_error_cb(void *context, int32_t error_code) {
    test_table_context_t *ctx = (test_table_context_t *)context;
    if (ctx != NULL) {
        ctx->error_calls++;
        ctx->last_error_code = error_code;
    }
}

// Static const operations table (resides in Flash/ROM)
static const ct_table_t app_callbacks = {
    .on_started = test_started_cb,
    .on_data = test_data_cb,
    .on_error = test_error_cb
};

TEST(CallbackTableTest, NormalExecution)
{
    ct_handle_t handle;
    test_table_context_t ctx = {};

    ct_init(&handle, &app_callbacks, &ctx);

    // Call started
    ct_fire_started(&handle);
    EXPECT_EQ(ctx.started_calls, 1);

    // Call data
    const uint8_t dummy_data[] = {0xA1, 0xB2, 0xC3};
    ct_fire_data(&handle, dummy_data, sizeof(dummy_data));
    EXPECT_EQ(ctx.data_calls, 1);
    EXPECT_EQ(ctx.last_data_size, sizeof(dummy_data));
    EXPECT_EQ(ctx.last_data_buffer, dummy_data);

    // Call error
    ct_fire_error(&handle, -5);
    EXPECT_EQ(ctx.error_calls, 1);
    EXPECT_EQ(ctx.last_error_code, -5);
}

TEST(CallbackTableTest, NullCallbacksOrHandles)
{
    ct_handle_t handle;
    
    // Ops table containing NULL callbacks
    static const ct_table_t sparse_callbacks = {
        .on_started = NULL,
        .on_data = NULL,
        .on_error = NULL
    };

    ct_init(&handle, &sparse_callbacks, NULL);

    // Firing should handle NULL callbacks gracefully
    ct_fire_started(&handle);
    ct_fire_data(&handle, NULL, 0);
    ct_fire_error(&handle, 0);

    // Firing with NULL handle should not crash
    ct_fire_started(NULL);
    ct_fire_data(NULL, NULL, 0);
    ct_fire_error(NULL, 0);
}

} // namespace
