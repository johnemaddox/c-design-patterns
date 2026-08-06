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
        sm_handle_t handle;
        sm_state_t state;

    protected:
        void SetUp() override
        {
            // Transition table stored in Flash (ROM) by using static const
            static const sm_state_config_t state_table[TL_STATES_LEN] = {
                { &tl_green,  TL_YELLOW }, // TL_GREEN -> TL_YELLOW
                { &tl_yellow, TL_RED    }, // TL_YELLOW -> TL_RED
                { &tl_red,    TL_GREEN  }  // TL_RED -> TL_GREEN
            };

            handle.cur_state = TL_GREEN;
            handle.state_table = state_table;
            handle.num_states = TL_STATES_LEN;
            handle.context = nullptr;
        }
};

/* Cycles through the states, each call to sm_run() executes
 * the current state and queues the next state.
*/
TEST_F(StateMachine, CycleAllStates)
{
    sm_state_t expect = TL_GREEN;
    for (int i = 0; i < TL_STATES_LEN * 2; i++)
    {
        state = sm_run(&handle);
        EXPECT_EQ(expect, state);

        if (expect == TL_RED)
        {
            expect = TL_GREEN;
        }
        else
        {
            expect++;
        }
    }
}

TEST_F(StateMachine, CallbackWithContext)
{
    tl_context_t context = {0, 0, 0};

    handle.context = &context;

    // Run first step (cur_state is TL_GREEN, runs tl__callback, transitions to TL_YELLOW)
    sm_state_t ret = sm_run(&handle);
    EXPECT_EQ(ret, TL_GREEN);
    EXPECT_EQ(handle.cur_state, TL_YELLOW);
    EXPECT_EQ(context.green_calls, 1);
    EXPECT_EQ(context.yellow_calls, 0);
    EXPECT_EQ(context.red_calls, 0);

    // Run second step (cur_state is TL_YELLOW, runs yellow_callback, transitions to TL_RED)
    ret = sm_run(&handle);
    EXPECT_EQ(ret, TL_YELLOW);
    EXPECT_EQ(handle.cur_state, TL_RED);
    EXPECT_EQ(context.green_calls, 1);
    EXPECT_EQ(context.yellow_calls, 1);
    EXPECT_EQ(context.red_calls, 0);

    // Run third step (cur_state is TL_RED, runs red_callback, transitions to TL_GREEN)
    ret = sm_run(&handle);
    EXPECT_EQ(ret, TL_RED);
    EXPECT_EQ(handle.cur_state, TL_GREEN);
    EXPECT_EQ(context.green_calls, 1);
    EXPECT_EQ(context.yellow_calls, 1);
    EXPECT_EQ(context.red_calls, 1);
}

} // namespace
