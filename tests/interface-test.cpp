#include "gtest/gtest.h"
extern "C" {
#include "camera_intf.h"
#include "camera_hw.h"
#include "camera_sim.h"
#include "sensor_intf.h"
#include "sensor_hw.h"
#include "sensor_sim.h"
}

namespace
{

TEST(IntfCreate, NullPtr)
{
    ASSERT_EQ(camera_hw_create(nullptr), INTF_ERR);
    ASSERT_EQ(camera_sim_create(nullptr), INTF_ERR);
}

TEST(IntfCreate, Hardware)
{
    intf_handle_t cam;
    ASSERT_EQ(camera_hw_create(&cam), INTF_OK);
    EXPECT_NE(cam.ops, nullptr);
    EXPECT_NE(cam.context, nullptr);
}

TEST(IntfCreate, Simulator)
{
    intf_handle_t cam;
    ASSERT_EQ(camera_sim_create(&cam), INTF_OK);
    EXPECT_NE(cam.ops, nullptr);
    EXPECT_NE(cam.context, nullptr);
}

TEST(IntfSafeWrappers, NullPtr)
{
    // Test wrapper null robustness
    EXPECT_EQ(camera_init(nullptr).intf_flag, INTF_ERR);
    EXPECT_EQ(camera_capture(nullptr, 5).intf_flag, INTF_ERR);
    EXPECT_EQ(camera_start(nullptr).intf_flag, INTF_ERR);
    EXPECT_EQ(camera_stop(nullptr).intf_flag, INTF_ERR);

    // Test missing ops
    intf_handle_t empty_cam = { .ops = nullptr, .context = nullptr };
    EXPECT_EQ(camera_init(&empty_cam).intf_flag, INTF_ERR);
}

TEST(IntfSelect, SwitchHW)
{
    intf_handle_t cam;
    camera_state_t state;

    // Test HW Cam
    ASSERT_EQ(camera_hw_create(&cam), INTF_OK);
    state = camera_init(&cam);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_INIT);

    // Test SIM Cam
    ASSERT_EQ(camera_sim_create(&cam), INTF_OK);
    state = camera_init(&cam);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_INIT);
}

class IntfFncs : public ::testing::Test
{
    public:
        intf_handle_t  cam;
        camera_state_t state;

    private:
        void SetUp() override
        {
            ASSERT_EQ(camera_hw_create(&cam), INTF_OK);
        }
};

TEST_F(IntfFncs, Init)
{
    state = camera_init(&cam);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_INIT);
}

TEST_F(IntfFncs, Capture)
{
    const uint32_t captures = 5;
    state = camera_capture(&cam, captures);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_CAPTURE);
    EXPECT_EQ(state.cam_state.extra, captures);
}

TEST_F(IntfFncs, Start)
{
    state = camera_start(&cam);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_START);
}

TEST_F(IntfFncs, Stop)
{
    state = camera_stop(&cam);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.cam_state.flag, CAM_STOP);
}

TEST(SensorIntfCreate, NullPtr)
{
    ASSERT_EQ(sensor_hw_create(nullptr), INTF_ERR);
    ASSERT_EQ(sensor_sim_create(nullptr), INTF_ERR);
}

TEST(SensorIntfCreate, Hardware)
{
    intf_handle_t sens;
    ASSERT_EQ(sensor_hw_create(&sens), INTF_OK);
    EXPECT_NE(sens.ops, nullptr);
    EXPECT_NE(sens.context, nullptr);
}

TEST(SensorIntfCreate, Simulator)
{
    intf_handle_t sens;
    ASSERT_EQ(sensor_sim_create(&sens), INTF_OK);
    EXPECT_NE(sens.ops, nullptr);
    EXPECT_NE(sens.context, nullptr);
}

TEST(SensorIntfSafeWrappers, NullPtr)
{
    float val = 0.0f;
    EXPECT_EQ(sensor_init(nullptr).intf_flag, INTF_ERR);
    EXPECT_EQ(sensor_read(nullptr, &val).intf_flag, INTF_ERR);
    EXPECT_EQ(sensor_calibrate(nullptr).intf_flag, INTF_ERR);
    EXPECT_EQ(sensor_enable(nullptr).intf_flag, INTF_ERR);
    EXPECT_EQ(sensor_disable(nullptr).intf_flag, INTF_ERR);

    intf_handle_t empty_sens = { .ops = nullptr, .context = nullptr };
    EXPECT_EQ(sensor_init(&empty_sens).intf_flag, INTF_ERR);
    EXPECT_EQ(sensor_read(&empty_sens, &val).intf_flag, INTF_ERR);
    EXPECT_EQ(sensor_calibrate(&empty_sens).intf_flag, INTF_ERR);
    EXPECT_EQ(sensor_enable(&empty_sens).intf_flag, INTF_ERR);
    EXPECT_EQ(sensor_disable(&empty_sens).intf_flag, INTF_ERR);
}

TEST(SensorIntfSelect, SwitchHW)
{
    intf_handle_t sens;
    sensor_state_t state;

    // Test HW Sensor
    ASSERT_EQ(sensor_hw_create(&sens), INTF_OK);
    state = sensor_init(&sens);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.sens_state.flag, SENS_INIT);

    // Test SIM Sensor
    ASSERT_EQ(sensor_sim_create(&sens), INTF_OK);
    state = sensor_init(&sens);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.sens_state.flag, SENS_INIT);
}

class SensorIntfFncs : public ::testing::Test
{
    public:
        intf_handle_t  sens;
        sensor_state_t state;

    private:
        void SetUp() override
        {
            ASSERT_EQ(sensor_hw_create(&sens), INTF_OK);
        }
};

TEST_F(SensorIntfFncs, Init)
{
    state = sensor_init(&sens);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.sens_state.flag, SENS_INIT);
}

TEST_F(SensorIntfFncs, EnableDisableRead)
{
    float value = 0.0f;
    // Reading before enabling should fail in this driver design
    state = sensor_read(&sens, &value);
    EXPECT_EQ(state.intf_flag, INTF_ERR);

    // Enable the sensor
    state = sensor_enable(&sens);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.sens_state.flag, SENS_ENABLE);

    // Read should now succeed
    state = sensor_read(&sens, &value);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.sens_state.flag, SENS_READ);
    EXPECT_GT(value, 25.0f); // Mock starts at 25.0f and increments on read

    // Disable the sensor
    state = sensor_disable(&sens);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.sens_state.flag, SENS_DISABLE);

    // Read should fail again
    state = sensor_read(&sens, &value);
    EXPECT_EQ(state.intf_flag, INTF_ERR);
}

TEST_F(SensorIntfFncs, Calibrate)
{
    state = sensor_calibrate(&sens);
    EXPECT_EQ(state.intf_flag, INTF_OK);
    EXPECT_EQ(state.sens_state.flag, SENS_CALIBRATE);
    EXPECT_FLOAT_EQ(state.sens_state.value, 25.0f);
}

} // namespace
