/**
 * @file
 *  Zenderer/Math/Matrix.hpp - A 4x4 matrix class wrapper for handling
 *  various graphical math operations.
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
 * @addtogroup Math
 * @{
 **/

#ifndef ZENDERER__MATH__MATRIX_HPP
#define ZENDERER__MATH__MATRIX_HPP

#include "Vector.hpp"

namespace zen
{
namespace math
{
    /**
     * 4x4 matrix representation.
     *  This is in place to support various linear transformation
     *  operations required for moving vertices around in OpenGL
     *  terms. Currently, there is only support for creating projection
     *  matrices for the rendering window / shaders to use.
     **/
    class ZEN_API matrix4x4_t
    {
    public:
        matrix4x4_t();
        matrix4x4_t(const real_t values[4][4]);
        matrix4x4_t(const real_t** ppvalues);
        matrix4x4_t(const matrix4x4_t& Copy);

        matrix4x4_t& operator=(const matrix4x4_t& Copy);

        /**
         * Provides the matrix's elements at that row.
         *  See the following examples:
         *
         *    matrix[0][3]  // 3rd element of row 0 (r0, c3)
         *    matrix[3][1]  // 1st element of row 3 (r3, c1)
         *    matrix[2][1]  // 1st element of row 2 (r2, c1)
         *
         *  The index value is automatically clamped at [0, 3].
         *
         * @param   uint8_t     Index
         * @return  The real_t array at that index.
         **/
        real_t* operator[](uint8_t index);

        matrix4x4_t operator*(matrix4x4_t& Other)   const;

        inline const real_t* GetMatrixPointer() const
        { return reinterpret_cast<const real_t*>(m_values); }

        /**
         * Creates a 2D projection matrix.
         *
         * @param   uint16_t    Projection width
         * @param   uint16_t    Projection height
         * @param   uint16_t    Maximum projection depth
         * @param   int         Minimum projection depth
         *
         * @return  The requested projection matrix.
         **/
        static inline matrix4x4_t Projection2D(
            const uint16_t width,   const uint16_t height,
            const uint16_t max_z,   const int min_z)
        {
            matrix4x4_t result;

            result[0][0] =  2.0f / width;
            result[0][3] = -1.0f;
            result[1][1] = -2.0f / height;
            result[1][3] =  1.0f;
            result[2][2] = -2.0f / (min_z - max_z);
            result[3][3] =  1.0f;

            return result;
        }

        static matrix4x4_t CreateIdentityMatrix();
        static const matrix4x4_t& GetIdentityMatrix();

    private:
        real_t m_values[4][4];
    };
}   // namespace math
}   // namespace zen

#endif // ZENDERER__MATH__MATRIX_HPP

/** @} **/
