/**
 * @file
 *  Zenderer/CoreGraphics/Drawable.hpp - A vertex-agnostic convex primitive.
 *
 * @author      George Kudrayvtsev (halcyon)
 * @version     1.0
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

#ifndef ZENDERER__GRAPHICS__POLYGON_HPP
#define ZENDERER__GRAPHICS__POLYGON_HPP

#include "Zenderer/CoreGraphics/Drawable.hpp"

namespace zen
{
namespace gfx
{
    /// An arbitrary n-vertex convex polygon.
    class ZEN_API zPolygon : public gfxcore::zDrawable
    {
    public:
        zPolygon(asset::zAssetManager& Assets);
        ~zPolygon();

        /**
         * Adds a vertex to the polygon at the specified position.
         *  Vertices form triangles from the first vertex added, and thus
         *  will not look right if there is any notion of concavity.
         *
         * @param   Position    The location to create the next vertex at
         **/
        void AddVertex(const math::vector_t& Position);

        /**
         * Creates the polygon from the internally stored vertices.
         * @pre     >= 3 vertices have been added to the polygon.
         * @post    No temporary vertices are stored until AddVertex()
         *          is called again.
         **/
        zDrawable& Create();
        
        /**
         * Overrides default index creation for the added vertices.
         *  Indices are considered valid if none of them are larger than
         *  the number of vertices. This function does *not* check for that
         *  in order to save speed.
         *
         * @param   Indices The indices to override defaults with
         **/
        void SetIndices(const std::vector<gfxcore::index_t>& Indices);

        /// Sets the vertex color of the *temporary* buffer.
        void SetColor(const color4f_t& Color);
        
        /// Calculates maximum height for the current vertices.
        uint16_t GetH() const;
        
        /// Calculates maximum width for the current vertices.
        uint16_t GetW() const;
        
        inline std::vector<math::vector_t> Triangulate() const
        { return gfx::triangulate(m_Verts); }

    private:
        std::vector<math::vector_t> m_Verts;
        color4f_t m_Color;
    };
}   // namespace gfx
}   // namespace zen

#endif // ZENDERER__GRAPHICS__POLYGON_HPP

/**
 * @class zen::gfx::zPolygon
 * @details
 *  This is a class representing an arbitrary convex polygon that can
 *  accept any number of vertices. It does NOT work with concave polygons,
 *  meaning there can be no way to connect any two vertices with a line that
 *  goes outside of the polygon. The polygon will be generated, of course,
 *  since there is no check for concavity, but it will simply not look
 *  correct when rendered on-screen.
 *
 *  The polygon is generated by drawing triangles to each individual vertex,
 *  beginning from the first vertex added.
 *
 *  Polygons are created a little differently than pre-baked primitives
 *  such as gfx::zQuad, they store vertices in a temporary buffer until
 *  Create() is called. Thus calls to the various `Set*()` methods are
 *  overidden in order to use the temporary buffer rather than the true
 *  internal vertex buffer as specified by the base class gfxcore::zDrawable.
 *
 * @example Polygons
 * @section A Variety of Polgyons
 * @subsection quad Simple Quad
 *  This can obviously be replicated through zen::gfx::zQuad, but is here
 *  for example's sake. We will compare the creation of the two.
 *
 *  @code
 *  // Assuming a manager, etc has already been defined prior.
 *  gfx::zPolygon PolyQuad(Manager);
 *  PolyQuad.AddVertex(0,   0);
 *  PolyQuad.AddVertex(32,  0);
 *  PolyQuad.AddVertex(32,  32);
 *  PolyQuad.AddVertex(0,   32);
 *  PolyQuad.SetColor(color4f_t(1, 0, 0));
 *  PolyQuad.Create();
 *
 *  // Equivalent quad-creating code.
 *  gfx::zQuad RealQuad(32, 32);
 *  RealQuad.SetColor(color4f_t(1, 0, 0));
 *  RealQuad.Create();
 *  @endcode
 *
 *  The benefit, of course, of using the polygon interface vs. the quad
 *  interface is that it eliminates the requirement of being axis-aligned,
 *  thus not rotated and in a rectangular form. With the polygon, we
 *  can make rhombuses and other arbitrary four-sided shapes.
 *
 * @subsection circle   Approximated Circle
 *  Now we will create a circle, or an approximation there-of. It's impossible
 *  of course to create a perfect circle since it's composed of lines, but
 *  we approximate with 360 sides :)
 *
 *  @code
 *  // Assuming a manager, etc has already been defined prior.
 *  gfx::zPolygon Circle(Manager);
 *
 *  real_t radius = 32;
 *  Circle.AddVertex(32, 32);   // Center of the circle
 *  for(uint16_t i = 0; i < 360; ++i)
 *  {
 *      Circle.AddVertex(sin(math::rad(i)) * radius,
 *                       cos(math::rad(i)) * radius);
 *  }
 *
 *  Circle.Create();
 *  @endcode
 **/

/** @} **/
