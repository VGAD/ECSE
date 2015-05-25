//! Check if an STL container contains something.
template <typename ContainerType, typename ItemType>
bool contains(ContainerType& container, ItemType item)
{
    return std::find(container.begin(), container.end(), item) != container.end();
}

//! Use this to check angles taking wrapping into account.
#define ASSERT_ANGLE_EQ(val1, val2) \
    ASSERT_NEAR(ECSE::wrapDifference(val1, val2, 2.f * ECSE::pi), 0.f, ECSE::degToRad(0.01f))

//! Use this when results come back from transcendental functions to make up for lost accuracy.
#define ASSERT_NEAR_TRANS(val1, val2) \
    ASSERT_NEAR(val1, val2, 0.0001f)