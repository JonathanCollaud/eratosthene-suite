/*
 *  eratosthene-suite - eratosthene indexation server front-end
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016 EPFL CDH DHLAB
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

    /*! \file   eratosthene-client-times.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Eratosthene client - times module
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_CLIENT_TIMES__
    # define __ER_CLIENT_TIMES__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - internal includes
 */

    # include "eratosthene-client-server.h"

/*
    header - external includes
 */

    # include <time.h>
    # include <GL/freeglut.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define ER_TIMES_C         { \
                                NULL, \
                                _LE_USE_PORT, \
                                _LE_TIME_NULL, \
                                2592000, \
                                0, \
                                _LE_TIME_NULL, \
                                _LE_TRUE, \
                                0, \
                                LE_ARRAY_C, \
                                LE_ARRAY_C, \
                                _LE_TRUE \
                                }

    /* Define control modes */
    # define ER_TIMES_DZOOM     ( 0 )
    # define ER_TIMES_IZOOM     ( 1 )
    # define ER_TIMES_DPOSE     ( 0 )
    # define ER_TIMES_IPOSE     ( 1 )
    # define ER_TIMES_DTIME     ( 0 )
    # define ER_TIMES_ITIME     ( 1 )

    /* Define time ranges */
    # define ER_TIMES_DAY       ( 86400 )
    # define ER_TIMES_MONTH     ( ER_TIMES_DAY * 30 )
    # define ER_TIMES_YEAR      ( ER_TIMES_MONTH * 12 )

    /* Define SRTM time */
    # define ER_TIMES_SRTM(t)   ( ( 950486422 / t ) * t )

    /* Define control dimensions */

    # define ER_TIMES_CW        ( 1920 )
    # define ER_TIMES_CH        ( 64+32 )
    # define ER_TIMES_CL        ( 32 )
    # define ER_TIMES_CP        ( ER_TIMES_CL )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    typedef struct er_times_struct {

        le_char_t * tm_svip;
        le_sock_t   tm_port;

        le_time_t   tm_pose;
        le_time_t   tm_zoom;
        le_size_t   tm_curr;

        le_time_t   tm_tdis;
        le_enum_t   tm_mtim;

        le_size_t   tm_count;
        le_array_t  tm_times;
        le_array_t  tm_stack;

    le_enum_t _status; } er_times_t;

/*
    header - function prototypes
 */

    er_times_t er_times_create( le_char_t * const er_ip, le_sock_t const er_port );

    le_void_t er_times_delete( er_times_t * const er_times );

    le_time_t er_times_get( er_times_t const * const er_times );

    le_enum_t er_times_get_update( er_times_t * const er_times );

    le_void_t er_times_set( er_times_t * const er_times );

    le_void_t er_times_set_default( er_times_t * const er_times );

    le_void_t er_times_set_zoom( er_times_t * const er_times, le_size_t const er_mode );

    le_void_t er_times_set_pose( er_times_t * const er_times, le_size_t const er_mode );

    le_void_t er_times_set_time( er_times_t * const er_times, le_size_t const er_mode );

    le_void_t er_times_set_nearest( er_times_t * const er_times );

    le_void_t er_times_display( er_times_t const * const er_times );

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

