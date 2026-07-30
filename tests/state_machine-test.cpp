#include "gtest/gtest.h"
extern "C" {
#include "state_machine.h"
#include "traffic_light.h"
}

namespace
{

class StateMachine : public ::testing::Test
{
    public:
        sm_handle_t smh;
        sm_state_t state;

    protected:
        void SetUp() override
        {
            // Transition table stored in Flash (ROM) by using static const
            static const sm_state_config_t state_table[SM_STATES_LEN] = {
                { &green,  SM_YELLOW }, // SM_GREEN -> SM_YELLOW
                { &yellow, SM_RED    }, // SM_YELLOW -> SM_RED
                { &red,    SM_GREEN  }  // SM_RED -> SM_GREEN
            };

            smh.cur_state = SM_GREEN;
            smh.state_table = state_table;
            smh.num_states = SM_STATES_LEN;
            smh.context = nullptr;
        }
};

/* Cycles through the states, each call to sm_run() executes
 * the current state and queues the next state.
*/
TEST_F(StateMachine, CycleAllStates)
{
    sm_state_t expect = SM_GREEN;
    for (int i = 0; i < SM_STATES_LEN * 2; i++)
    {
        state = sm_run(&smh);
        EXPECT_EQ(expect, state);

        if (expect == SM_RED)
        {
            expect = SM_GREEN;
        }
        else
        {
            expect++;
        }
    }
}

/* Callback example, the context struct holds a persistent count while 
 * the test cycles through each state.
*/
typedef struct
{
    int green_calls;
    int yellow_calls;
    int red_calls;
} callback_test_context_t;

sm_state_t green_callback(sm_handle_t *smh, void *context)
{
    (void)smh;
    callback_test_context_t *ctx = (callback_test_context_t *)context;
    if (ctx != NULL)
    {
        ctx->green_calls++;
    }
    return SM_GREEN;
}

sm_state_t yellow_callback(sm_handle_t *smh, void *context)
{
    (void)smh;
    callback_test_context_t *ctx = (callback_test_context_t *)context;
    if (ctx != NULL)
    {
        ctx->yellow_calls++;
    }
    return SM_YELLOW;
}

sm_state_t red_callback(sm_handle_t *smh, void *context)
{
    (void)smh;
    callback_test_context_t *ctx = (callback_test_context_t *)context;
    if (ctx != NULL)
    {
        ctx->red_calls++;
    }
    return SM_RED;
}

TEST(StateMachineTest, CallbackWithContext)
{
    callback_test_context_t context = {0, 0, 0};
    
    static const sm_state_config_t state_table[SM_STATES_LEN] = {
        { &green_callback,  SM_YELLOW }, // SM_GREEN -> SM_YELLOW
        { &yellow_callback, SM_RED    }, // SM_YELLOW -> SM_RED
        { &red_callback,    SM_GREEN  }  // SM_RED -> SM_GREEN
    };

    sm_handle_t smh;
    smh.cur_state = SM_GREEN;
    smh.state_table = state_table;
    smh.num_states = SM_STATES_LEN;
    smh.context = &context;

    // Run first step (cur_state is SM_GREEN, runs green_callback, transitions to SM_YELLOW)
    sm_state_t ret = sm_run(&smh);
    EXPECT_EQ(ret, SM_GREEN);
    EXPECT_EQ(smh.cur_state, SM_YELLOW);
    EXPECT_EQ(context.green_calls, 1);
    EXPECT_EQ(context.yellow_calls, 0);
    EXPECT_EQ(context.red_calls, 0);

    // Run second step (cur_state is SM_YELLOW, runs yellow_callback, transitions to SM_RED)
    ret = sm_run(&smh);
    EXPECT_EQ(ret, SM_YELLOW);
    EXPECT_EQ(smh.cur_state, SM_RED);
    EXPECT_EQ(context.green_calls, 1);
    EXPECT_EQ(context.yellow_calls, 1);
    EXPECT_EQ(context.red_calls, 0);

    // Run third step (cur_state is SM_RED, runs red_callback, transitions to SM_GREEN)
    ret = sm_run(&smh);
    EXPECT_EQ(ret, SM_RED);
    EXPECT_EQ(smh.cur_state, SM_GREEN);
    EXPECT_EQ(context.green_calls, 1);
    EXPECT_EQ(context.yellow_calls, 1);
    EXPECT_EQ(context.red_calls, 1);
}

} // namespace
