#include "gtest/gtest.h"
extern "C" {
#include "callback.h"
}

namespace
{

struct TestEvent
{
    int value;
};

// Test callbacks
void increment_callback(void *context, const void *event_data)
{
    int *counter = static_cast<int *>(context);
    const TestEvent *event = static_cast<const TestEvent *>(event_data);
    if (event != nullptr)
    {
        *counter += event->value;
    }
    else
    {
        (*counter)++;
    }
}

TEST(CallbackPattern, Init)
{
    cb_subject_t subject;
    cb_subject_init(&subject);
    EXPECT_EQ(subject.head, nullptr);
}

TEST(CallbackPattern, NullChecks)
{
    cb_subject_t subject;
    cb_subject_init(&subject);

    cb_observer_t observer = { nullptr, nullptr, nullptr };
    
    // Register checks
    EXPECT_FALSE(cb_subject_register(nullptr, &observer));
    EXPECT_FALSE(cb_subject_register(&subject, nullptr));
    EXPECT_FALSE(cb_subject_register(&subject, &observer)); // NULL callback

    // Unregister checks
    EXPECT_FALSE(cb_subject_unregister(nullptr, &observer));
    EXPECT_FALSE(cb_subject_unregister(&subject, nullptr));

    // Notify checks
    cb_subject_notify(nullptr, nullptr); // Should not crash
}

TEST(CallbackPattern, RegisterAndNotify)
{
    cb_subject_t subject;
    cb_subject_init(&subject);

    int counter = 0;
    cb_observer_t observer = { &increment_callback, &counter, nullptr };

    ASSERT_TRUE(cb_subject_register(&subject, &observer));
    
    // Test duplicate registration prevention
    EXPECT_FALSE(cb_subject_register(&subject, &observer));

    // Notify with event data
    TestEvent event = { 5 };
    cb_subject_notify(&subject, &event);
    EXPECT_EQ(counter, 5);

    // Notify with nullptr
    cb_subject_notify(&subject, nullptr);
    EXPECT_EQ(counter, 6);
}

TEST(CallbackPattern, Unregister)
{
    cb_subject_t subject;
    cb_subject_init(&subject);

    int counter1 = 0, counter2 = 0;
    cb_observer_t obs1 = { &increment_callback, &counter1, nullptr };
    cb_observer_t obs2 = { &increment_callback, &counter2, nullptr };

    ASSERT_TRUE(cb_subject_register(&subject, &obs1));
    ASSERT_TRUE(cb_subject_register(&subject, &obs2));

    // Unregister obs2 (currently head because of prepending)
    EXPECT_TRUE(cb_subject_unregister(&subject, &obs2));
    
    // Try unregistering again
    EXPECT_FALSE(cb_subject_unregister(&subject, &obs2));

    TestEvent event = { 10 };
    cb_subject_notify(&subject, &event);

    EXPECT_EQ(counter1, 10);
    EXPECT_EQ(counter2, 0); // Should not receive notification
}

TEST(CallbackPattern, MultiInstanceSubject)
{
    cb_subject_t subjectA;
    cb_subject_t subjectB;
    cb_subject_init(&subjectA);
    cb_subject_init(&subjectB);

    int countA = 0;
    int countB = 0;

    cb_observer_t obsA = { &increment_callback, &countA, nullptr };
    cb_observer_t obsB = { &increment_callback, &countB, nullptr };

    ASSERT_TRUE(cb_subject_register(&subjectA, &obsA));
    ASSERT_TRUE(cb_subject_register(&subjectB, &obsB));

    TestEvent event = { 7 };
    cb_subject_notify(&subjectA, &event);

    EXPECT_EQ(countA, 7);
    EXPECT_EQ(countB, 0); // Isolated
}

} // namespace
