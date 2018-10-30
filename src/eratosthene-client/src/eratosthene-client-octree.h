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

/*! \file   eratosthene-client-model.h
 *  \author Nils Hamel <nils.hamel@bluewin.ch>
 *
 *  eratosthene-suite - client - model
 */

/*
 header - inclusion guard
 */

# ifndef __ER_CLIENT_OCTREE
# define __ER_CLIENT_OCTREE

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

/*
 header - external includes
 */

/*
 header - preprocessor definitions
 */

/* define pseudo-constructor */
# define ER_OCTREE_C {NULL, NULL, 0}

/* define pseudo-initialiser */

/* define model stack */

/* define display array types */

/* define model color */

/*
 header - preprocessor macros
 */

/*
 header - type definition
 */

/*
 header - structures
 */
typedef struct er_node_struct {

    struct er_node_struct * nd_parent;
    struct er_node_struct * nd_children;
    le_size_t nd_size;

} er_node_t;

/*
 header - function prototypes
 */
    er_node_t er_octree_create(er_node_t * nd_parent, er_node_t nd_children[8]);
    
    le_void_t er_node_add_point(er_node_t * node);

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
