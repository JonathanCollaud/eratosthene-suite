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

le_void_t er_voxel_set_color(er_voxel_t * const voxel, le_size_t const color[3])
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

le_void_t er_voxel_display_cube(er_voxel_t const * const voxel)
{

    le_real_t * edge = voxel->vx_edge;
    le_real_t point[24];
    le_real_t * size = voxel->vx_size;

    glColorPointer ( 3, ER_MODEL_COLORS, 0, voxel->vx_color);

    for (int s = -1; s <= 1; s = s + 2) {
        point[0] = edge[0] + s * size[0] / 2.0;
        point[1] = edge[1] + s * size[1] / 2.0;
        point[2] = edge[2] +  s * size[2] / 2.0;

        point[3] = point[0] - s * size[0];
        point[4] = point[1];
        point[5] = point[2];

        point[6] = point[3];
        point[7] = point[4];
        point[8] = point[5] - s * size[2];

        point[9] = point[6] + s * size[0];
        point[10] = point[7];
        point[11] = point[8];

        point[12] = point[9];
        point[13] = point[10] - s * size[1];
        point[14] = point[11];

        point[15] = point[12];
        point[16] = point[13];
        point[17] = point[14] + s * size[2];

        point[18] = point[15] - s * size[0];
        point[19] = point[16];
        point[20] = point[17];

        point[21] = point[18];
        point[22] = point[19] + s * size[1];
        point[23] = point[20];

        glVertexPointer(3, ER_MODEL_VERTEX, 0, point);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
    }


    /*// White side - FRONT
    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(point[0] + size[0], point[1] - size[1], point[2] - size[2]);
    glVertex3f(point[0] + size[0], point[1] + size[1], point[2] - size[2]);
    glVertex3f(point[0] - size[0], point[1] + size[1], point[2] - size[2]);
    glVertex3f(point[0] + size[0], point[1] - size[1], point[2] - size[2]);
    glEnd();

    // White side - BACK
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(point[0] + size[0], point[1] - size[1], point[2] + size[2]);
    glVertex3f(point[0] + size[0], point[1] + size[1], point[2] + size[2]);
    glVertex3f(point[0] - size[0], point[1] + size[1], point[2] + size[2]);
    glVertex3f(point[0] - size[0], point[1] - size[1], point[2] + size[2]);
    glEnd();

    // Purple side - RIGHT
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(point[0] + size[0], point[1] - size[1], point[2] - size[2]);
    glVertex3f(point[0] + size[0], point[1] + size[1], point[2] - size[2]);
    glVertex3f(point[0] + size[0], point[1] + size[1], point[2] + size[2]);
    glVertex3f(point[0] + size[0], point[1] - size[1], point[2] + size[2]);
    glEnd();

    // Green side - LEFT
    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(point[0] - size[0], point[1] - size[1], point[2] + size[2]);
    glVertex3f(point[0] - size[0], point[1] + size[1], point[2] + size[2]);
    glVertex3f(point[0] - size[0], point[1] + size[1], point[2] - size[2]);
    glVertex3f(point[0] - size[0], point[1] - size[1], point[2] - size[2]);
    glEnd();

    // Blue side - TOP
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(point[0] + size[0], point[1] + size[1], point[2] + size[2]);
    glVertex3f(point[0] + size[0], point[1] + size[1], point[2] - size[2]);
    glVertex3f(point[0] - size[0], point[1] + size[1], point[2] - size[2]);
    glVertex3f(point[0] - size[0], point[1] + size[1], point[2] + size[2]);
    glEnd();

    // Red side - BOTTOM
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(point[0] + size[0], point[1] - size[1], point[2] - size[2]);
    glVertex3f(point[0] + size[0], point[1] - size[1], point[2] + size[2]);
    glVertex3f(point[0] - size[0], point[1] - size[1], point[2] + size[2]);
    glVertex3f(point[0] - size[0], point[1] - size[1], point[2] - size[2]);
    glEnd();*/
}

le_size_t er_voxel_is_full(er_voxel_t const * const voxel)
{
    return voxel->vx_nb_points > 3;
}
