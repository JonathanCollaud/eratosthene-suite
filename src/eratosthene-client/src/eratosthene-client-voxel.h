/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
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

/*
 header - inclusion guard
 */

# ifndef __ER_CLIENT_VOXEL__
# define __ER_CLIENT_VOXEL__

/*
 header - C/C++ compatibility
 */

# ifdef __cplusplus
extern "C" {
# endif

/*
 header - internal includes
 */

# include "eratosthene-client-common.h"
# include "eratosthene-client-model.h"

/*
 header - external includes
 */

/*
 header - preprocessor definitions
 */

/* define pseudo-constructor */
# define ER_VOXEL_C   {0, {255, 255, 255}, {0.0, 0.0, 0.0}, {5.0, 5.0, 5.0}}

/*
 header - type definition
 */

/*
 header - structures
 */
typedef struct er_voxel_struct {

    le_size_t vx_nb_points;
    le_data_t vx_color[3];
    le_real_t vx_edge[3];
    le_real_t vx_size[3];

} er_voxel_t;

/*
 header - function prototypes
 */
er_voxel_t er_voxel_create(le_void_t);

le_void_t er_voxel_delete( er_voxel_t * const voxel );

le_void_t er_voxel_set_color(er_voxel_t * const voxel, le_data_t const color[3]);

le_void_t er_voxel_set_edge(er_voxel_t * const voxel, le_real_t const edge[3]);

le_void_t er_voxel_set_size(er_voxel_t * const voxel, le_real_t const size[3]);

le_void_t er_voxel_display_cube(er_voxel_t const * const voxel);

le_size_t er_voxel_is_full(er_voxel_t const * const voxel);

/*
 header - C/C++ compatibility
 */

# ifdef __cplusplus
}
# endif

/*
 header - inclusion guard
 */

# endif
