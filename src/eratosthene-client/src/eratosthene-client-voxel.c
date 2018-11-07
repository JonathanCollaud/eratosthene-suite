/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
 *
 *  Contributors
 *
 *      Jonathan Collaud - jonathan.collaud@epfl.ch
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "eratosthene-client-voxel.h"

er_voxel_t er_voxel_create(le_void_t)
{
    /* created structure variables */
    er_voxel_t er_voxel = ER_VOXEL_C;

    /* return created structure */
    return er_voxel;
}

le_void_t er_voxel_delete( er_voxel_t * const er_voxel )
{
    /* deleted structure variables */
    er_voxel_t er_delete = ER_VOXEL_C;

    /* delete structure */
    ( * er_voxel ) = er_delete;
}

le_void_t er_voxel_set_color(er_voxel_t * const voxel, le_data_t const color[3])
{
    for (le_size_t i = 0; i < 3; i++) {
        voxel->vx_color[i] = color[i];
    }
}

le_void_t er_voxel_set_edge(er_voxel_t * const voxel, le_real_t const edge[3])
{
    for (le_size_t i = 0; i < 3; i++) {
        voxel->vx_edge[i] = edge[i];
    }
}

le_void_t er_voxel_set_size(er_voxel_t * const voxel, le_real_t const size[3])
{
    for (le_size_t i = 0; i < 3; i++) {
        voxel->vx_size[i] = size[i];
    }
}

le_void_t er_voxel_display_cube(er_voxel_t const * const voxel, const le_real_t er_lon, const le_real_t er_lat)
{
    /*le_real_t point[24];
    le_data_t color[24];
    le_real_t normal[24];*/
    
    le_real_t point[12];
    le_data_t color[12];
    le_real_t normal[12];

    for (le_size_t i = 0; i < 12; i++) {
        color[i] = voxel->vx_color[i % 3];
    }
    
    le_real_t s[3];
    for (le_size_t i = 0; i < 3; i++) {
        s[i] = voxel->vx_size[i];
    }
    
    /* optimisation variables */
    le_real_t clon = cos(er_lon * ER_COMMON_D2R);
    le_real_t slon = sin(er_lon * ER_COMMON_D2R);
    le_real_t clat = cos(er_lat * ER_COMMON_D2R);
    le_real_t slat = sin(er_lat * ER_COMMON_D2R);
    
    le_real_t inv_square_three = 1 / sqrt(3.0);
    
    glCullFace( GL_FRONT );
    
    // EAST
    point[0] = voxel->vx_edge[0] + 0.5 * (clon * s[0] - slon * s[2]);
    point[1] = voxel->vx_edge[1] + 0.5 * (slon * clat * s[0] + slat * s[1] + clon * clat * s[2]);
    point[2] = voxel->vx_edge[2] + 0.5 * (-slon * slat * s[0] + clat * s[1] - clon * slat * s[2]);
    
    point[3] = voxel->vx_edge[0] + 0.5 * (clon * s[0] - slon * s[2]);
    point[4] = voxel->vx_edge[1] + 0.5 * (slon * clat * s[0] - slat * s[1] + clon * clat * s[2]);
    point[5] = voxel->vx_edge[2] + 0.5 * (-slon * slat * s[0] - clat * s[1] - clon * slat * s[2]);
    
    point[6] = voxel->vx_edge[0] + 0.5 * (clon * s[0] + slon * s[2]);
    point[7] = voxel->vx_edge[1] + 0.5 * (slon * clat * s[0] - slat * s[1] - clon * clat * s[2]);
    point[8] = voxel->vx_edge[2] + 0.5 * (-slon * slat * s[0] - clat * s[1] + clon * slat * s[2]);
    
    point[9] = voxel->vx_edge[0] + 0.5 * (clon * s[0] + slon * s[2]);
    point[10] = voxel->vx_edge[1] + 0.5 * (slon * clat * s[0] + slat * s[1] - clon * clat * s[2]);
    point[11] = voxel->vx_edge[2] + 0.5 * (-slon * slat * s[0] + clat * s[1] + clon * slat * s[2]);
    
    normal[0] = normal[3] = normal[6] = normal[9] = 1.0;
    normal[1] = normal[4] = normal[7] = normal[10] = 0.0;
    normal[2] = normal[5] = normal[8] = normal[11] = 0.0;
    
    glVertexPointer(3, ER_MODEL_VERTEX, 0, point);
    glColorPointer(3, ER_MODEL_COLORS, 0, color);
    glNormalPointer(ER_MODEL_VERTEX, 0, normal);
    glDrawArrays(GL_QUADS, 0, 4);
    
    // WEST
    point[0] = voxel->vx_edge[0] - 0.5 * (clon * s[0] - slon * s[2]);
    point[1] = voxel->vx_edge[1] - 0.5 * (slon * clat * s[0] + slat * s[1] + clon * clat * s[2]);
    point[2] = voxel->vx_edge[2] - 0.5 * (-slon * slat * s[0] + clat * s[1] - clon * slat * s[2]);

    point[3] = voxel->vx_edge[0] - 0.5 * (clon * s[0] + slon * s[2]);
    point[4] = voxel->vx_edge[1] - 0.5 * (slon * clat * s[0] + slat * s[1] - clon * clat * s[2]);
    point[5] = voxel->vx_edge[2] - 0.5 * (-slon * slat * s[0] + clat * s[1] + clon * slat * s[2]);
    
    point[6] = voxel->vx_edge[0] - 0.5 * (clon * s[0] + slon * s[2]);
    point[7] = voxel->vx_edge[1] - 0.5 * (slon * clat * s[0] - slat * s[1] - clon * clat * s[2]);
    point[8] = voxel->vx_edge[2] - 0.5 * (-slon * slat * s[0] - clat * s[1] + clon * slat * s[2]);
    
    point[9] = voxel->vx_edge[0] - 0.5 * (clon * s[0] - slon * s[2]);
    point[10] = voxel->vx_edge[1] - 0.5 * (slon * clat * s[0] - slat * s[1] + clon * clat * s[2]);
    point[11] = voxel->vx_edge[2] - 0.5 * (-slon * slat * s[0] - clat * s[1] - clon * slat * s[2]);
    
    normal[0] = normal[3] = normal[6] = normal[9] = -1.0;
    normal[1] = normal[4] = normal[7] = normal[10] = 0.0;
    normal[2] = normal[5] = normal[8] = normal[11] = 0.0;
    
    glVertexPointer(3, ER_MODEL_VERTEX, 0, point);
    glColorPointer(3, ER_MODEL_COLORS, 0, color);
    glNormalPointer(ER_MODEL_VERTEX, 0, normal);
    glDrawArrays(GL_QUADS, 0, 4);
    
    // UP
    point[0] = voxel->vx_edge[0] + 0.5 * (clon * s[0] - slon * s[2]);
    point[1] = voxel->vx_edge[1] + 0.5 * (slon * clat * s[0] + slat * s[1] + clon * clat * s[2]);
    point[2] = voxel->vx_edge[2] + 0.5 * (-slon * slat * s[0] + clat * s[1] - clon * slat * s[2]);
    
    point[3] = voxel->vx_edge[0] + 0.5 * (clon * s[0] + slon * s[2]);
    point[4] = voxel->vx_edge[1] + 0.5 * (slon * clat * s[0] + slat * s[1] - clon * clat * s[2]);
    point[5] = voxel->vx_edge[2] + 0.5 * (-slon * slat * s[0] + clat * s[1] + clon * slat * s[2]);
    
    point[6] = voxel->vx_edge[0] + 0.5 * (-clon * s[0] + slon * s[2]);
    point[7] = voxel->vx_edge[1] + 0.5 * (-slon * clat * s[0] + slat * s[1] - clon * clat * s[2]);
    point[8] = voxel->vx_edge[2] + 0.5 * (slon * slat * s[0] + clat * s[1] + clon * slat * s[2]);
    
    point[9] = voxel->vx_edge[0] + 0.5 * (-clon * s[0] - slon * s[2]);
    point[10] = voxel->vx_edge[1] + 0.5 * (-slon * clat * s[0] + slat * s[1] + clon * clat * s[2]);
    point[11] = voxel->vx_edge[2] + 0.5 * (+slon * slat * s[0] + clat * s[1] - clon * slat * s[2]);
    
    normal[0] = normal[3] = normal[6] = normal[9] = 0.0;
    normal[1] = normal[4] = normal[7] = normal[10] = 1.0;
    normal[2] = normal[5] = normal[8] = normal[11] = 0.0;
    
    glVertexPointer(3, ER_MODEL_VERTEX, 0, point);
    glColorPointer(3, ER_MODEL_COLORS, 0, color);
    glNormalPointer(ER_MODEL_VERTEX, 0, normal);
    glDrawArrays(GL_QUADS, 0, 4);
    
    // BOTTOM
    point[0] = voxel->vx_edge[0] - 0.5 * (clon * s[0] - slon * s[2]);
    point[1] = voxel->vx_edge[1] - 0.5 * (slon * clat * s[0] + slat * s[1] + clon * clat * s[2]);
    point[2] = voxel->vx_edge[2] - 0.5 * (-slon * slat * s[0] + clat * s[1] - clon * slat * s[2]);
    
    point[3] = voxel->vx_edge[0] - 0.5 * (-clon * s[0] - slon * s[2]);
    point[4] = voxel->vx_edge[1] - 0.5 * (-slon * clat * s[0] + slat * s[1] + clon * clat * s[2]);
    point[5] = voxel->vx_edge[2] - 0.5 * (slon * slat * s[0] + clat * s[1] - clon * slat * s[2]);
    
    point[6] = voxel->vx_edge[0] - 0.5 * (-clon * s[0] + slon * s[2]);
    point[7] = voxel->vx_edge[1] - 0.5 * (-slon * clat * s[0] + slat * s[1] - clon * clat * s[2]);
    point[8] = voxel->vx_edge[2] - 0.5 * (slon * slat * s[0] + clat * s[1] + clon * slat * s[2]);
    
    point[9] = voxel->vx_edge[0] - 0.5 * (clon * s[0] + slon * s[2]);
    point[10] = voxel->vx_edge[1] - 0.5 * (slon * clat * s[0] + slat * s[1] - clon * clat * s[2]);
    point[11] = voxel->vx_edge[2] - 0.5 * (-slon * slat * s[0] + clat * s[1] + clon * slat * s[2]);
    
    normal[0] = normal[3] = normal[6] = normal[9] = 0.0;
    normal[1] = normal[4] = normal[7] = normal[10] = -1.0;
    normal[2] = normal[5] = normal[8] = normal[11] = 0.0;
    
    glVertexPointer(3, ER_MODEL_VERTEX, 0, point);
    glColorPointer(3, ER_MODEL_COLORS, 0, color);
    glNormalPointer(ER_MODEL_VERTEX, 0, normal);
    glDrawArrays(GL_QUADS, 0, 4);
    
    // NORTH
    point[0] = voxel->vx_edge[0] + 0.5 * (clon * s[0] - slon * s[2]);
    point[1] = voxel->vx_edge[1] + 0.5 * (slon * clat * s[0] + slat * s[1] + clon * clat * s[2]);
    point[2] = voxel->vx_edge[2] + 0.5 * (-slon * slat * s[0] + clat * s[1] - clon * slat * s[2]);
    
    point[3] = voxel->vx_edge[0] + 0.5 * (-clon * s[0] - slon * s[2]);
    point[4] = voxel->vx_edge[1] + 0.5 * (-slon * clat * s[0] + slat * s[1] + clon * clat * s[2]);
    point[5] = voxel->vx_edge[2] + 0.5 * (slon * slat * s[0] + clat * s[1] - clon * slat * s[2]);
    
    point[6] = voxel->vx_edge[0] + 0.5 * (-clon * s[0] - slon * s[2]);
    point[7] = voxel->vx_edge[1] + 0.5 * (-slon * clat * s[0] - slat * s[1] + clon * clat * s[2]);
    point[8] = voxel->vx_edge[2] + 0.5 * (slon * slat * s[0] - clat * s[1] - clon * slat * s[2]);
    
    point[9] = voxel->vx_edge[0] + 0.5 * (clon * s[0] - slon * s[2]);
    point[10] = voxel->vx_edge[1] + 0.5 * (slon * clat * s[0] - slat * s[1] + clon * clat * s[2]);
    point[11] = voxel->vx_edge[2] + 0.5 * (-slon * slat * s[0] - clat * s[1] - clon * slat * s[2]);
    
    normal[0] = normal[3] = normal[6] = normal[9] = 0.0;
    normal[1] = normal[4] = normal[7] = normal[10] = 0.0;
    normal[2] = normal[5] = normal[8] = normal[11] = -1.0;
    
    glVertexPointer(3, ER_MODEL_VERTEX, 0, point);
    glColorPointer(3, ER_MODEL_COLORS, 0, color);
    glNormalPointer(ER_MODEL_VERTEX, 0, normal);
    glDrawArrays(GL_QUADS, 0, 4);
    
    // SOUTH
    point[0] = voxel->vx_edge[0] - 0.5 * (clon * s[0] - slon * s[2]);
    point[1] = voxel->vx_edge[1] - 0.5 * (slon * clat * s[0] + slat * s[1] + clon * clat * s[2]);
    point[2] = voxel->vx_edge[2] - 0.5 * (-slon * slat * s[0] + clat * s[1] - clon * slat * s[2]);
    
    point[3] = voxel->vx_edge[0] - 0.5 * (clon * s[0] - slon * s[2]);
    point[4] = voxel->vx_edge[1] - 0.5 * (slon * clat * s[0] - slat * s[1] + clon * clat * s[2]);
    point[5] = voxel->vx_edge[2] - 0.5 * (-slon * slat * s[0] - clat * s[1] - clon * slat * s[2]);
    
    point[6] = voxel->vx_edge[0] - 0.5 * (-clon * s[0] - slon * s[2]);
    point[7] = voxel->vx_edge[1] - 0.5 * (-slon * clat * s[0] - slat * s[1] + clon * clat * s[2]);
    point[8] = voxel->vx_edge[2] - 0.5 * (slon * slat * s[0] - clat * s[1] - clon * slat * s[2]);
    
    point[9] = voxel->vx_edge[0] - 0.5 * (-clon * s[0] - slon * s[2]);
    point[10] = voxel->vx_edge[1] - 0.5 * (-slon * clat * s[0] + slat * s[1] + clon * clat * s[2]);
    point[11] = voxel->vx_edge[2] - 0.5 * (slon * slat * s[0] + clat * s[1] - clon * slat * s[2]);
    
    normal[0] = normal[3] = normal[6] = normal[9] = 0.0;
    normal[1] = normal[4] = normal[7] = normal[10] = 0.0;
    normal[2] = normal[5] = normal[8] = normal[11] = 1.0;
    
    glVertexPointer(3, ER_MODEL_VERTEX, 0, point);
    glColorPointer(3, ER_MODEL_COLORS, 0, color);
    glNormalPointer(ER_MODEL_VERTEX, 0, normal);
    glDrawArrays(GL_QUADS, 0, 4);
    
    
    /*glCullFace( GL_FRONT );
    for (int o = -1; o <= 1; o += 2) {
        
        point[0] = voxel->vx_edge[0] + 0.5 * o * (clon * s[0] - slon * s[2]);
        point[1] = voxel->vx_edge[1] + 0.5 * o * (slon * clat * s[0] + slat * s[1] + clon * clat * s[2]);
        point[2] = voxel->vx_edge[2] + 0.5 * o * (-slon * slat * s[0] + clat * s[1] - clon * slat * s[2]);
        
        point[21] = point[3] = point[0] - o * clon * s[0];
        point[22] = point[4] = point[1] - o * slon * clat * s[0];
        point[23] = point[5] = point[2] + o * slon * slat * s[0];
        
        point[6] = point[3] + o * slon * s[2];
        point[7] = point[4] - o * clon * clat * s[2];
        point[8] = point[5] + o * clon * slat * s[2];
        
        point[9] = point[6] + o * clon * s[0];
        point[10] = point[7] + o * slon * clat * s[0];
        point[11] = point[8] - o * slon * slat * s[0];
        
        point[12] = point[9];
        point[13] = point[10] - o * slat * s[1];
        point[14] = point[11] - o * clat * s[1];
        
        point[15] = point[12] - o * slon * s[2];
        point[16] = point[13] + o * clon * clat * s[2];
        point[17] = point[14] - o * clon * slat * s[2];
        
        point[18] = point[15] - o * clon * s[0];
        point[19] = point[16] - o * slon * clat * s[0];
        point[20] = point[17] + o * slon * slat * s[0];
        
        normal[0] = o * inv_square_three;
        normal[1] = o * inv_square_three;
        normal[2] = o * inv_square_three;
        
        normal[21] = normal[3] = - o * inv_square_three;
        normal[22] = normal[4] = o * inv_square_three;
        normal[23] = normal[5] = o * inv_square_three;
        
        normal[6] = - o * inv_square_three;
        normal[7] = o * inv_square_three;
        normal[8] = - o * inv_square_three;
        
        normal[9] = o * inv_square_three;
        normal[10] = o * inv_square_three;
        normal[11] = - o * inv_square_three;
        
        normal[12] = o * inv_square_three;
        normal[13] = - o * inv_square_three;
        normal[14] = - o * inv_square_three;
        
        normal[15] = o * inv_square_three;
        normal[16] = - o * inv_square_three;
        normal[17] = o * inv_square_three;
        
        normal[18] = - o * inv_square_three;
        normal[19] = - o * inv_square_three;
        normal[20] = o * inv_square_three;

        glVertexPointer(3, ER_MODEL_VERTEX, 0, point);
        glColorPointer(3, ER_MODEL_COLORS, 0, color);
        glNormalPointer(ER_MODEL_VERTEX, 0, normal);
        
        glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
        
        glCullFace( GL_BACK );
    }*/
}

le_size_t er_voxel_is_full(er_voxel_t const * const voxel)
{
    return voxel->vx_nb_points > 3;
}
