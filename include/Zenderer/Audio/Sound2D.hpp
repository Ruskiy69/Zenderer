/**
 * @file
 *  Zenderer/Audio/Sound2D.hpp - Short `.wav` file sound effects used
 *  in-game.
 *
 * @author      George Kudrayvtsev (halcyon)
 * @version     2.0
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
 * @addtogroup Audio
 * @{
 **/

#ifndef ZENDERER__AUDIO__SOUND_2D_HPP
#define ZENDERER__AUDIO__SOUND_2D_HPP

#include "Audio2D.hpp"

namespace zen
{
namespace sfx
{
    /// Sound effects (`.wav` files)
    class ZEN_API CSound2D : public CAudio2D
    {
    public:
        CSound2D();
        virtual ~CSound2D();

        bool LoadFromFile(const string_t& filename);
        bool LoadFromExisting(const CAsset* const pCopy){ return true; }

        /// Plays the sound effect no matter what (if loaded).
        void Play();

        /// This only checks for looping.
        inline void Update()
        {
            if(this->IsLoaded() && this->GetAudioState() != AL_PLAYING)
            {
                this->Play();
            }
        }

        /// Always returns `true` if the file is loaded.
        inline bool Ready() const { return true; }
    };
}   // namespace sfx
}   // namespace zen

#endif // ZENDERER__AUDIO__SOUND_2D_HPP
/** @} **/
