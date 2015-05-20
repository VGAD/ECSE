template <typename ContainerType, typename ItemType>
bool contains(ContainerType& container, ItemType item)
{
    return std::find(container.begin(), container.end(), item) != container.end();
}