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

# include "eratosthene-client-cell.h"

/*
 source - constructor/destructor methods
 */

er_cell_t er_cell_create( le_void_t )
{
    
    /* created structure variables */
    er_cell_t er_cell = ER_CELL_C;
    
    /* create array structure */
    er_cell.ce_data = le_array_create();
    
    /* return created structure */
    return( er_cell );
    
}

le_void_t er_cell_delete( er_cell_t * const er_cell )
{
    
    /* deleted structure variables */
    er_cell_t er_delete = ER_CELL_C;
    
    /* delete array structure */
    le_array_delete( & er_cell->ce_data );
    
    /* free primitive pointers */
    free( er_cell->ce_vertex_pt );
    free( er_cell->ce_color_pt );
    free( er_cell->ce_normal_pt );
    
    /* delete structure */
    ( * er_cell ) = er_delete;
    
}

/*
 source - accessor methods
 */


le_byte_t er_cell_get_flag( er_cell_t const * const er_cell, le_byte_t const er_state )
{
    
    /* return state */
    return( er_cell->ce_flag & er_state );
    
}

le_enum_t er_cell_get_equal( er_cell_t const * const er_cell, er_cell_t const * const er_targ )
{
    
    /* compare addresses */
    return( le_address_get_equal( & er_cell->ce_addr, & er_targ->ce_addr ) );
    
}

le_enum_t er_cell_get_drop( er_cell_t const * const er_cell, le_address_t const * const er_addr )
{
    
    /* compare addresses */
    return( le_address_get_equal( & er_cell->ce_addr, er_addr ) );
    
}

le_size_t er_cell_get_record( er_cell_t const * const er_cell )
{
    
    /* return number of uf3 records */
    return( le_array_get_size( & er_cell->ce_data ) / LE_ARRAY_UF3 );
    
}

le_size_t er_cell_get_size( er_cell_t const * const er_cell )
{
    
    /* return address length */
    return( le_address_get_size( & er_cell->ce_addr ) );
    
}

le_real_t * er_cell_get_pose( er_cell_t * const er_cell )
{
    
    /* return position pointer */
    return( ( le_real_t * ) le_array_get_byte( & er_cell->ce_data ) );
    
}

le_data_t * er_cell_get_data( er_cell_t * const er_cell )
{
    
    /* return data pointer */
    return( ( le_data_t * ) ( le_array_get_byte( & er_cell->ce_data ) + LE_ARRAY_UF3_POSE ) );
    
}

le_real_t * er_cell_get_edge( er_cell_t * const er_cell )
{
    
    /* return edge pointer */
    return( er_cell->ce_edge );
    
}

le_size_t er_cell_get_sync( er_cell_t * const er_cell, le_array_t * const er_array, le_size_t const er_offset )
{
    
    /* de-serialise address */
    return( le_address_serial( & er_cell->ce_addr, er_array, er_offset, _LE_SET ) );
    
}

le_array_t * er_cell_get_array( er_cell_t const * const er_cell )
{
    
    /* return array */
    return( ( le_array_t * ) & er_cell->ce_data );
    
}

/*
 source - mutator methods
 */

le_void_t er_cell_set_flag( er_cell_t * const er_cell, le_byte_t const er_state )
{
    
    /* update cell state */
    er_cell->ce_flag |= er_state;
    
}

le_void_t er_cell_set_zero( er_cell_t * const er_cell, le_byte_t const er_state )
{
    
    /* update cell state */
    er_cell->ce_flag &= ~ er_state;
    
}

le_void_t er_cell_set_push( er_cell_t * const er_cell, le_address_t const * const er_addr )
{
    
    /* assign address */
    er_cell->ce_addr = ( * er_addr );
    
}

le_size_t er_cell_set_sync( er_cell_t * const er_cell, le_array_t * const er_array, le_size_t const er_offset )
{
    
    /* serialise address */
    return( le_address_serial( & er_cell->ce_addr, er_array, er_offset, _LE_GET ) );
    
}

le_size_t er_cell_set_data( er_cell_t * const er_cell )
{
    
    /* size variables */
    le_size_t er_size = 0;
    
    /* pointer variables */
    le_byte_t * er_head = NULL;
    le_byte_t * er_base = NULL;
    
    /* optimisation variables */
    le_real_t er_opta = 0.0;
    le_real_t er_optb = 0.0;
    
    /* retrieve array size */
    if ( ( er_size = le_array_get_size( & er_cell->ce_data ) ) == 0 ) {
        
        /* return cell size */
        return( er_size );
        
    }
    
    le_real_t er_corner[3];
    le_address_get_pose(& er_cell->ce_addr, er_corner);
    
    /* retrieve array pointer */
    er_head = er_base = le_array_get_byte( & er_cell->ce_data );
    
    /* coordinates conversion - edge */
    er_cell->ce_edge[2] = ( ( le_real_t * ) er_head )[2] + LE_ADDRESS_WGS_A;
    
    /* coordinates conversion - edge */
    er_cell->ce_edge[1] = er_cell->ce_edge[2] * sin( ( ( le_real_t * ) er_head )[1] );
    er_cell->ce_edge[2] = er_cell->ce_edge[2] * cos( ( ( le_real_t * ) er_head )[1] );
    er_cell->ce_edge[0] = er_cell->ce_edge[2] * sin( ( ( le_real_t * ) er_head )[0] );
    er_cell->ce_edge[2] = er_cell->ce_edge[2] * cos( ( ( le_real_t * ) er_head )[0] );
    
    /* inital points coordinates */
    ( ( le_real_t * ) er_head )[0] = 0.0;
    ( ( le_real_t * ) er_head )[1] = 0.0;
    ( ( le_real_t * ) er_head )[2] = 0.0;
    
    le_size_t span = le_address_get_span(&er_cell->ce_addr);;
    le_size_t two_span = 1 << span;
    
    le_byte_t * raster = calloc( two_span * two_span * two_span, sizeof(le_byte_t));
    if ( raster == NULL ) {
        return 0;
    }
    
    le_real_t denom = 1 << er_cell_get_size(er_cell);
    
    le_real_t size[3] = {
        LE_ADDRESS_RAN_L / denom,
        2 * LE_ADDRESS_RAN_A / denom,
        1024 * LE_ADDRESS_RAN_H / denom};
    
    //printf("voxel size = (%lf; %lf; %lf)\n", size[0], size[1], size[2]);
    // printf("corner = (%lf; %lf; %lf)\n", er_corner[0], er_corner[1], er_corner[2]);
    
    le_size_t x = 0;
    le_size_t y = 0;
    le_size_t z = 0;
    
    /* parsing socket array */
    while ( ( ( er_head += LE_ARRAY_UF3 ) - er_base ) < er_size ) {
        
        x = floor((( ( le_real_t * ) er_head )[0] - er_corner[0]) * two_span / size[0]);
        y = floor((( ( le_real_t * ) er_head )[1] - er_corner[1]) * two_span / size[1]);
        z = floor((( ( le_real_t * ) er_head )[2] - er_corner[2]) * two_span / size[2]);
        
        if (((((x * two_span) + y ) * two_span) + z) >= two_span * two_span * two_span){
            printf("Fails at\nposition = (%d; %d; %d)\n", x, y, z);
            printf("voxel index = %d\n\n", (((x * two_span) + y ) * two_span) + z);
        } else {
            raster[(((x * two_span) + y ) * two_span) + z] = 1;
        }
        
        er_cell->ce_prim_cnt++;
        
        //printf("index = %d; ", (((x * two_span) + y ) * two_span) + z);
        //printf("position = (%d; %d; %d)", x, y, z);
        //printf(" == (%lf; %lf; %lf)\n", ( ( le_real_t * ) er_head )[0], ( ( le_real_t * ) er_head )[1], ( ( le_real_t * ) er_head )[2]);
    }
    
    le_real_t edge[3] = {0.0, 0.0, 0.0};
    le_size_t color[3] = {255, 0, 0};
    le_real_t normal[3] = {0.0, 0.0, 1.0};
    
    er_cell->ce_vertex_pt = calloc(3 * er_cell->ce_prim_cnt, sizeof(le_real_t));
    if (er_cell->ce_vertex_pt == NULL) {
        return 0;
    }
    
    for(le_size_t i = 0; i < two_span * two_span * two_span; i++){
        if (raster[i] == 1) {
            
            edge[0] = i / (two_span * two_span);
            edge[1] = (i / two_span) % two_span;
            edge[2] = i % two_span;
            
            printf("index = %d\n", i);
            printf("position = (%.0f; %.0f; %.0f)\n", edge[0], edge[1], edge[2]);
            
            edge[0] = er_corner[0] + (edge[0] * size[0] / two_span);
            edge[1] = er_corner[1] + (edge[1] * size[1] / two_span);
            edge[2] = er_corner[2] + (edge[2] * size[2] / two_span);
            
            printf("position = (%lf; %lf; %lf)\n", edge[0], edge[1], edge[2]);
            
            /* coordinates conversion - points */
            edge[2] += LE_ADDRESS_WGS_A;
            
            /* coordinates conversion - points */
            er_opta = edge[0];
            er_optb = edge[1];
            
            /* coordinates conversion - points */
            edge[1] = edge[2] * sin( er_optb ) - er_cell->ce_edge[1];
            edge[2] = edge[2] * cos( er_optb );
            edge[0] = edge[2] * sin( er_opta ) - er_cell->ce_edge[0];
            edge[2] = edge[2] * cos( er_opta ) - er_cell->ce_edge[2];
            
            printf("position = (%lf; %lf; %lf)\n", edge[0], edge[1], edge[2]);
            er_cell->ce_vertex_pt[i * 3] = edge[0];
            er_cell->ce_vertex_pt[i * 3 + 1] = edge[1];
            er_cell->ce_vertex_pt[i * 3 + 2] = edge[2];
        }
    }
    
    
    for(le_size_t d = 0; d < 3; d++){
        le_size_t prev_raster = 2;
        le_size_t curr_raster = 2;
        for(le_size_t i = 0; i < two_span; i++){
            curr_raster = raster[i];
            if (raster[i] == 1) {
            }
            prev_raster = curr_raster;
        }
    }
    
    er_cell->ce_color_pt = calloc(3, sizeof(le_size_t));
    if (er_cell->ce_color_pt == NULL) {
        return 0;
    }
    
    er_cell->ce_normal_pt = calloc(3, sizeof(le_real_t));
    if (er_cell->ce_normal_pt == NULL) {
        return 0;
    }
    
    free( raster );
    
    /* return cell size */
    return( er_size );
    
}

le_void_t er_cell_display( er_cell_t * const er_cell ){
    glBegin(GL_QUADS);
    glColor3i(255, 0, 0);
    for(le_size_t i = 0; i < 3 * er_cell->ce_prim_cnt; i+=3){
        glVertex3f(er_cell->ce_vertex_pt[i], er_cell->ce_vertex_pt[i + 1], er_cell->ce_vertex_pt[i + 2]);
    }
    glEnd();
    
    //glVertexPointer(3, GL_DOUBLE, 0, er_cell->ce_vertex_pt);
    //glColorPointer (3, GL_UNSIGNED_BYTE, 0, er_cell->ce_color_pt);
    //glNormalPointer(GL_DOUBLE, 0, er_cell->ce_normal_pt);
    //glDrawArrays(GL_POINTS, 0, er_cell->ce_prim_cnt);
}
