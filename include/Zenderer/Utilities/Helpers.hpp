/**
 * @file
 *  Zenderer/Utilities/Helpers.hpp - A collection of helper functions
 *  primarily used for string manipulation.
 *
 * @author      George Kudrayvtsev (halcyon)
 * @version     1.1
 * @copyright   Apache License v2.0
 *  Licensed under the Apache License, Version 2.0 (the "License").         \n
 *  You may not use this file except in compliance with the License.        \n
 *  You may obtain a copy of the License at:
 *  http://www.apache.org/licenses/LICENSE-2.0                              \n
 *  Unless required by applicable law or agreed to in writing, software     \n
 *  distributed under the License is distributed on an "AS IS" BASIS,       \n
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
 *  See the License for the specific language governing permissions and     \n
 *  limitations under the License.
 *
 * @addtogroup Utilities
 * @{
 **/
 
#ifndef ZENDERER__UTILITIES__HELPERS_HPP
#define ZENDERER__UTILITIES__HELPERS_HPP

#include <vector>

#include "Zenderer/Core/Types.hpp"

#include "Assert.hpp"

namespace zen
{
namespace util
{
    /**
     * Strips leading whitespace from a string (in-place).
     * @param   string_t&    Text to strip whitespace from
     **/
    ZEN_API void stripl(string_t& text);

    /**
     * Strips leading whitespace from a string (original untouched).
     * @param   string_t    Text to strip whitespace from
     * @return  String with stripped leading whitespace.
     **/
    ZEN_API string_t stripl(const string_t& text);

    /**
     * Strips trailing whitespace from a string (in-place).
     * @param   string_t&    Text to strip whitespace from
     **/
    ZEN_API void stript(string_t& text);

    /**
     * Strips trailing whitespace from a string (original untouched).
     * @param   string_t    Text to strip whitespace from
     * @return  String with stripped trailing whitespace.
     **/
    ZEN_API string_t stript(const string_t& text);

    /**
     * Splits a string into a list of substrings based on a delimiter.
     *  
     * @param   string_t    Text to split
     * @param   char        Delimiter value
     * @param   size_t      Guess at how many values there will be (optional)
     *
     * @return  `std::vector` filled with split values.
     **/
    ZEN_API std::vector<string_t> split(const string_t& text,
                                        const char delim,
                                        const size_t approx = 0);

    /**
     * Converts an immutable string to its uppercase equivalent.
     * @param   string_t    String to convert
     * @return  Uppercase equivalent.
     **/
    ZEN_API string_t toupper(const string_t& str);
    
    /**
     * Converts a string to its uppercase equivalent (in-place).
     * @param   string_t&    String to convert
     **/
    ZEN_API void toupper(string_t& str);

    /**
     * Calculates a hash for some value.
     *  This algorithm is ripped from the MurmurHash2 algorithm.
     *  
     *  When called by CAsset::CAsset(), it uses the CAsset::s_seed
     *  value for the seed, which is automatically generated
     *  by the CAssetManager on initialization.
     * 
     * @author  Austin Appleby
     * @version r144
     *
     * @param   void*       Data to hash
     * @param   uint32_t    Size of the data
     * @param   uint32_t    Random seed to hash with (optional=`69`)
     * 
     * @return  The hash value.
     *
     * @see     https://sites.google.com/site/murmurhash/
     **/
    static inline ZEN_API uint32_t hash(const void* pdata, uint32_t size,
                                        const uint32_t seed = 69)
    {
        const unsigned int m = 0x5bd1e995;
        const int r = 24;

        // Initialize the hash to a 'random' value
        uint32_t h = seed ^ size;

        // Mix 4 bytes at a time into the hash
        const unsigned char* data = (const unsigned char*)pdata;

        while(size >= 4)
        {
            uint32_t k = *(uint32_t*)data;

            k *= m; 
            k ^= k >> r; 
            k *= m; 

            h *= m; 
            h ^= k;

            data += 4;
            size -= 4;
        }

        // Handle the last few bytes of the input array
        switch(size)
        {
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0]; h *= m;
        };

        // Do a few final mixes of the hash to ensure the last few
        // bytes are well-incorporated.
        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h;
    }
    
    /**
     * Hashes a string to a unique value.
     *  This is just a shortcut to zen::util::hash() for strings.
     *
     * @param   string_t    The string to hash
     * @param   uint32_t    The hash seed value (optional=`69`)
     *
     * @return  A hash value.
     **/
    static inline ZEN_API uint32_t string_hash(const string_t& str,
                                               const uint32_t seed = 69)
    {
        return hash(str.c_str(), str.size(), seed);
    }
    
}   // namespace util
}   // namespace zen

#endif // ZENDERER__UTILITIES__HELPERS_HPP

/**
 * @fn zen::util::split
 *
 * @description
 *  Empty strings are never returned (unless given one), meaning if
 *  there is no valid delimiter, the string is returned in the list.
 *  Successive instances of a delimiter are ignored as well, meaning
 *  three spaces following a string will be interpreted as a single
 *  delimiter (assuming ' ' is the parameter).
 *
 * @example Utilities
 *
 *  Splitting a String
 *  ------------------
 *
 *  Code:
 *
 *  @code
 *  zen::string_t data("  Leading, Multiple  Single and Trailing   Delimiters   ");
 *
 *  std::vector<zen::string_t> splitString = zen::util::split(data);
 *
 *  std::cout << "Size: " << splitString.size() << std::endl;
 *
 *  for(auto& i : splitString)
 *  {
 *      std::cout << i << ":";
 *  }
 *
 *  std::cout << std::endl;
 *  @endcode
 *
 *  Output:
 *
 *  @code
 *  6
 *  Leading,:Multiple:Single:and:Trailing:Delimiters:
 *  @endcode
 *
 *  Code:
 *
 *  @code
 *  data = "NoDelimiters";
 *
 *  std::vector<zen::string_t> splitString = zen::util::split(data);
 *
 *  std::cout << "Size: " << splitString.size() << std::endl;
 *
 *  for(auto& i : splitString)
 *  {
 *      std::cout << i << ":";
 *  }
 *
 *  std::cout << std::endl;
 *  @endcode
 *
 *  Output:
 *
 *  @code
 *  1
 *  NoDelimiters:
 *  @endcode
 *
 *  Code:
 *
 *  @code
 *  data = "";  // empty string
 *
 *  std::vector<zen::string_t> splitString = zen::util::split(data);
 *
 *  std::cout << "Size: " << splitString.size() << std::endl;
 *
 *  for(auto& i : splitString)
 *  {
 *      std::cout << i << ":";
 *  }
 *
 *  std::cout << std::endl;
 *  @endcode
 *
 *  Output:
 *
 *  @code
 *  1
 *  :
 *  @endcode
 *
 **/

/** @} **/
