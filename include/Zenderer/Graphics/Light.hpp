/**
 * @file
 *  Zenderer/Graphics/Light.hpp - An instance of lighting in the game scene.
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
 * @addtogroup Graphics
 * @{
 **/

#ifndef ZENDERER__GRAPHICS__LIGHT_HPP
#define ZENDERER__GRAPHICS__LIGHT_HPP

#include "Zenderer/Math/Matrix.hpp"
#include "Zenderer/CoreGraphics/Renderer.hpp"
#include "Zenderer/CoreGraphics/ShaderSet.hpp"

namespace zen
{
namespace gfx
{
    /// Lighting types.
    ZEN_API enum class LightType : int16_t
    {
        ZEN_NO_LIGHT = -1,  ///< An invalid light type
        ZEN_AMBIENT,        ///< Uniform ambient light
        ZEN_SPOTLIGHT,      ///< Angled spot light
        ZEN_POINT           ///< Omnidirectional point light
    };

    /**
     * Represents a light object acting on a graphical scene.
     *  Each lighting type has its own special variables and parameters 
     *  that can be modified and accessed. See the raw shader files to learn
     *  the specific parameter names.
     **/
    class ZEN_API CLight
    {
    public:
        /**
         * Constructs a light instance.
         *  The window height is required for the shader to perform some weird
         *  offset calculations that would otherwise give bad results in terms
         *  of light positioning. It defaults to 800px.
         *
         * @param   m_Assets    The asset manager to make the effect from
         * @param   Type        The light type you want
         * @param   window_h    The height of the OpenGL context 
         **/
        CLight(asset::CAssetManager& m_Assets, const LightType& Type,
               const uint16_t window_h = 800);

        ~CLight();

        bool Init();

        bool Enable();
        bool Disable();

        void SetBrightness(const real_t brightness);
        void SetColor(const real_t r, const real_t g, const real_t b);
        void SetColor(const color3f_t& Color);
        void SetAttenuation(const real_t c, const real_t l, const real_t q);
        void SetAttenuation(const math::vector_t& Att);
        void SetPosition(const real_t x, const real_t y);
        void SetPosition(const math::vector_t& Pos);
        void SetMaximumAngle(const real_t degrees);
        void SetMinimumAngle(const real_t degrees);

    private:
        // Default values
        static math::vector_t   s_DefaultAttenuation,
                                s_DefaultPosition;
        static color3f_t        s_DefaultColor;
        static real_t           s_DefaultBrightness, s_DefaultMaxAngle,
                                s_DefaultMinAngle;

        // We don't use the high-level effect to get more control
        // and minimize uniform parameter lookups.
        gfxcore::CShaderSet m_Shader;

        math::vector_t  m_Att, m_Position, m_Max, m_Min;
        color3f_t       m_Color;
        real_t          m_brt;

        LightType m_type;

        // Uniform locations
        GLint m_loccol,     // Color
              m_locbrt,     // Brightness
              m_locpos,     // Position
              m_locatt,     // Attenuation
              m_loctmx,     // Minimum angle ([loc]ation of [t]heta [m]a[x])
              m_loctmn;     // Maximum angle

        // OpenGL context height (in pixels).
        uint16_t m_height;
    };
}   // namespace gfx
}   // namespace zen

#endif // ZENDERER__GRAPHICS__LIGHT_HPP

/** @} **/