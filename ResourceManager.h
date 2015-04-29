#pragma once

#include <string>
#include <map>

#include "Common.h"

//! A data store intended for use with SFML Texture and SoundBuffer types.
namespace ECSE
{

/*! Allows loading and storing based on filenames so data can be re-used. */
template <typename T>
class ResourceManager
{
public:
    //! Construct the ResourceManager.
    ResourceManager();

    //! Deallocate all data.
    ~ResourceManager();
    
    //! Returns a reference to the data with the given filename.
    /*!
    * If the file has not yet been loaded, it will be loaded into memory.
    * \param filename The name of the file to load.
    */
    const T& get(const std::string filename);

    //! Deletes all data in the resource manager.
    void clear();

private:
    //! The map from filename to the data type.
    std::map<std::string, T> dataMap;
};

template <typename T>
ResourceManager<T>::ResourceManager()
{

}

template <typename T>
ResourceManager<T>::~ResourceManager()
{
    clear();
}

template <typename T>
const T& ResourceManager<T>::get(const std::string filename)
{
    // Check for data in the map
    auto data = dataMap.find(filename);
    if (data != dataMap.end()) return data->second;

    // Data is not yet in map, so add it
    if (!dataMap[filename].loadFromFile(filename))
    {
        throw std::runtime_error("Failed to load data from file \"" + filename + "\"!");
    }

    return dataMap[filename];
}

template <typename T>
void ResourceManager<T>::clear()
{
    dataMap.clear();
}

}
