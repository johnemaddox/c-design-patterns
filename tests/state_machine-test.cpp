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

} // namespace
