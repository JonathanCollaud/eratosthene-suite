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
    
    /* retrieve array size */
    if ( ( er_size = le_array_get_size( & er_cell->ce_data ) ) == 0 ) {
        /* return cell size */
        return( er_size );
    }
    
    le_real_t er_pose[3];
    le_address_get_pose(& er_cell->ce_addr, er_pose);
    
    /* retrieve array pointer */
    er_head = er_base = le_array_get_byte( & er_cell->ce_data );
    
    /* coordinates conversion - edge */
    er_cell->ce_edge[2] = ( ( le_real_t * ) er_head )[2] + LE_ADDRESS_WGS_A;
    
    /* coordinates conversion - edge */
    er_cell->ce_edge[1] = er_cell->ce_edge[2] * sin( ( ( le_real_t * ) er_head )[1] );
    er_cell->ce_edge[2] = er_cell->ce_edge[2] * cos( ( ( le_real_t * ) er_head )[1] );
    er_cell->ce_edge[0] = er_cell->ce_edge[2] * sin( ( ( le_real_t * ) er_head )[0] );
    er_cell->ce_edge[2] = er_cell->ce_edge[2] * cos( ( ( le_real_t * ) er_head )[0] );
    
    le_size_t span = le_address_get_span(&er_cell->ce_addr);;
    le_size_t two_span = 1 << span;
    
    le_byte_t * raster = calloc( two_span * two_span * two_span, sizeof(le_byte_t));
    if ( raster == NULL ) return 0;
    
    le_size_t * r_red = calloc( two_span * two_span * two_span, sizeof(le_size_t));
    if ( r_red == NULL ) return 0;
    
    le_size_t * r_green = calloc( two_span * two_span * two_span, sizeof(le_size_t));
    if ( r_green == NULL ) return 0;
    
    le_size_t * r_blue = calloc( two_span * two_span * two_span, sizeof(le_size_t));
    if ( r_blue == NULL ) return 0;
    
    le_real_t denom = 1 << er_cell_get_size(er_cell);
    
    le_real_t size[3] = {
        LE_ADDRESS_RAN_L / denom,
        2 * LE_ADDRESS_RAN_A / denom,
        1024 * LE_ADDRESS_RAN_H / denom};
    
    le_size_t x = 0;
    le_size_t y = 0;
    le_size_t z = 0;
    
    /* parsing socket array */
    while ( ( er_head - er_base ) < er_size ) {
        
        x = floor((( ( le_real_t * ) er_head )[0] - er_pose[0]) * two_span / size[0]);
        y = floor((( ( le_real_t * ) er_head )[1] - er_pose[1]) * two_span / size[1]);
        z = floor((( ( le_real_t * ) er_head )[2] - er_pose[2]) * two_span / size[2]);
        
        if (((((x * two_span) + y ) * two_span) + z) >= two_span * two_span * two_span){
            printf("Fails at\nposition = (%lld; %lld; %lld)\n", x, y, z);
            printf("voxel index = %lld\n\n", (((x * two_span) + y ) * two_span) + z);
        } else {
            raster[(((x * two_span) + y ) * two_span) + z] = 1;
            r_red[(((x * two_span) + y ) * two_span) + z] = (er_head + LE_ARRAY_UF3_POSE)[0];
            r_green[(((x * two_span) + y ) * two_span) + z] = (er_head + LE_ARRAY_UF3_POSE)[1];
            r_blue[(((x * two_span) + y ) * two_span) + z] = (er_head + LE_ARRAY_UF3_POSE)[2];
        }
        
        er_head += LE_ARRAY_UF3;
        er_cell->ce_prim_cnt++;
    }
    
    er_cell->ce_vertex_pt = calloc(4 * 3 * 3 * 2 * er_cell->ce_prim_cnt, sizeof(le_real_t));
    if (er_cell->ce_vertex_pt == NULL) return 0;
    
    er_cell->ce_color_pt = calloc(4 * 3 * 3 * 2 * er_cell->ce_prim_cnt, sizeof(le_size_t));
    if (er_cell->ce_color_pt == NULL) return 0;
    
    er_cell->ce_normal_pt = calloc(4 * 3 * 3 * 2 * er_cell->ce_prim_cnt, sizeof(le_real_t));
    if (er_cell->ce_normal_pt == NULL) return 0;
    
    for (le_size_t axis = 0; axis < 3; axis ++) {
        er_cell_reduce_raster(er_cell, raster, r_red, r_green, r_blue, axis);
    }
    
    free( raster );
    free( r_red );
    free( r_green );
    free( r_blue );
    
    er_cell->ce_color_pt = realloc(er_cell->ce_color_pt, er_cell->ce_prim_cnt * 3 * sizeof(le_size_t));
    er_cell->ce_vertex_pt = realloc(er_cell->ce_vertex_pt, er_cell->ce_prim_cnt * 3 * sizeof(le_real_t));
    er_cell->ce_normal_pt = realloc(er_cell->ce_normal_pt, er_cell->ce_prim_cnt * 3 * sizeof(le_real_t));
    
    /* return cell size */
    return( er_size );
    
}

le_void_t er_cell_reduce_raster(er_cell_t * const er_cell,
                                const le_byte_t * const raster,
                                const le_size_t * const r_red,
                                const le_size_t * const r_green,
                                const le_size_t * const r_blue,
                                const le_size_t axis){
    le_real_t corner[3];
    le_real_t edge[3];
    le_size_t color[3];
    le_real_t normal[3];
    le_real_t er_pose[3];
    le_address_get_pose(& er_cell->ce_addr, er_pose);
    
    le_size_t span = le_address_get_span(&er_cell->ce_addr);;
    le_size_t two_span = 1 << span;
    
    le_real_t denom = 1 << er_cell_get_size(er_cell);
    le_real_t size[3] = {
        LE_ADDRESS_RAN_L / denom,
        2 * LE_ADDRESS_RAN_A / denom,
        1024 * LE_ADDRESS_RAN_H / denom};
    
    le_size_t prim_cnt = (axis == 0) ? 0 : er_cell->ce_prim_cnt;
    le_size_t i = 0;
    le_size_t prev_i = 0;
    le_size_t prev_raster = 0;
    le_size_t curr_raster;
    
    for (le_size_t e1 = 0; e1 < two_span; e1++) {
        
        for (le_size_t e2 = 0; e2 < two_span; e2++) {
            
            for (le_size_t e3 = 0; e3 < two_span; e3++) {
                
                switch (axis) {
                    case 0:
                        i = (((e3 * two_span) + e1 ) * two_span) + e2;
                        break;
                    case 1:
                        i = (((e2 * two_span) + e3 ) * two_span) + e1;
                        break;
                    case 2:
                        i = (((e1 * two_span) + e2 ) * two_span) + e3;
                        break;
                    default: return;
                }
                
                curr_raster = raster[i];
                
                if (prev_raster != curr_raster || (curr_raster == 1 && e3 == two_span - 1)) {
                    
                    corner[axis] = er_pose[axis] + (e3 * size[axis] / two_span);
                    corner[(axis + 1) % 3] = er_pose[(axis + 1) % 3] + (e1 * size[(axis + 1) % 3] / two_span);
                    corner[(axis + 2) % 3] = er_pose[(axis + 2) % 3] + (e2 * size[(axis + 2) % 3] / two_span);
                    
                    /* enter in shape if current raster is one, exit otherwise */
                    le_size_t side = curr_raster == 1 ? i : prev_i;
                    
                    color[0] = r_red[side];
                    color[1] = r_green[side];
                    color[2] = r_blue[side];
                    
                    for (le_size_t c = 0; c < 4; c++) {
                        edge[0] = corner [0];
                        edge[1] = corner [1];
                        edge[2] = corner [2];
                        
                        if (c == 1 || c == 2) edge[(axis + 1) % 3] += size[(axis + 1) % 3] / two_span; //== z=>0
                        if (c >= 2) edge[(axis + 2) % 3] += size[(axis + 2) % 3] / two_span; //== z=>1
                        
                        er_cell_para2cart(er_cell, edge);
                        
                        er_cell->ce_vertex_pt[(prim_cnt + c) * 3] = edge[0];
                        er_cell->ce_vertex_pt[(prim_cnt + c) * 3 + 1] = edge[1];
                        er_cell->ce_vertex_pt[(prim_cnt + c) * 3 + 2] = edge[2];
                        
                        er_cell->ce_color_pt[(prim_cnt + c) * 3] = color[0];
                        er_cell->ce_color_pt[(prim_cnt + c) * 3 + 1] = color[1];
                        er_cell->ce_color_pt[(prim_cnt + c) * 3 + 2] = color[2];
                        
                        er_cell->ce_normal_pt[(prim_cnt + c) * 3] = 0;
                        er_cell->ce_normal_pt[(prim_cnt + c) * 3 + 1] = 0;
                        er_cell->ce_normal_pt[(prim_cnt + c) * 3 + 2] = 1;
                    }
                    
                    prim_cnt += 4;
                }
                prev_i = i;
                prev_raster = curr_raster;
            }
        }
    }
    
    /*for (le_size_t z = 0; z < two_span; z++) { //==
        for (le_size_t x = 0; x < two_span; x++) { //==
            le_size_t prev_raster = 0;
            le_size_t curr_raster;
            for (le_size_t y = 0; y < two_span; y++) { //==
                i = (((x * two_span) + y ) * two_span) + z; //==
                curr_raster = raster[i];
                if (prev_raster != curr_raster) {
                    
                    corner[0] = er_pose[0] + (x * size[0] / two_span); //==
                    corner[1] = er_pose[1] + (y * size[1] / two_span); //==
                    corner[2] = er_pose[2] + (z * size[2] / two_span); //==
                    
                    for (le_size_t c = 0; c < 4; c++) {
                        edge[0] = corner [0];
                        edge[1] = corner [1];
                        edge[2] = corner [2];
                        
                        if (c == 1 || c == 2) edge[2] += size[2] / two_span; //== y=>2
                        if (c >= 2) edge[0] += size[0] / two_span; //== y=>0
                        
                        er_cell_para2cart(er_cell, edge);
                        
                        er_cell->ce_vertex_pt[(prim_cnt + c) * 3] = edge[0];
                        er_cell->ce_vertex_pt[(prim_cnt + c) * 3 + 1] = edge[1];
                        er_cell->ce_vertex_pt[(prim_cnt + c) * 3 + 2] = edge[2];
                        
                        er_cell->ce_color_pt[(prim_cnt + c) * 3] = r_red[i];
                        er_cell->ce_color_pt[(prim_cnt + c) * 3 + 1] = r_green[i];
                        er_cell->ce_color_pt[(prim_cnt + c) * 3 + 2] = r_blue[i];
                        
                        er_cell->ce_normal_pt[(prim_cnt + c) * 3] = 0;
                        er_cell->ce_normal_pt[(prim_cnt + c) * 3 + 1] = 1;
                        er_cell->ce_normal_pt[(prim_cnt + c) * 3 + 2] = 0;
                    }
                    
                    prim_cnt += 4;
                }
                prev_raster = curr_raster;
            }
        }
    }
    
    for (le_size_t y = 0; y < two_span; y++) {
        for (le_size_t z = 0; z < two_span; z++) {
            le_size_t prev_raster = 0;
            le_size_t curr_raster;
            for (le_size_t x = 0; x < two_span; x++) {
                i = (((x * two_span) + y ) * two_span) + z;
                curr_raster = raster[i];
                if (prev_raster != curr_raster) {
                    
                    corner[0] = er_pose[0] + (x * size[0] / two_span);
                    corner[1] = er_pose[1] + (y * size[1] / two_span);
                    corner[2] = er_pose[2] + (z * size[2] / two_span);
                    
                    for (le_size_t c = 0; c < 4; c++) {
                        edge[0] = corner [0];
                        edge[1] = corner [1];
                        edge[2] = corner [2];
                        
                        if (c == 1 || c == 2) edge[1] += size[1] / two_span; //== x=>1
                        if (c >= 2) edge[2] += size[2] / two_span; //== x=>2
                        
                        er_cell_para2cart(er_cell, edge);
                        
                        er_cell->ce_vertex_pt[(prim_cnt + c) * 3] = edge[0];
                        er_cell->ce_vertex_pt[(prim_cnt + c) * 3 + 1] = edge[1];
                        er_cell->ce_vertex_pt[(prim_cnt + c) * 3 + 2] = edge[2];
                        
                        if (curr_raster == 1) {
                            er_cell->ce_color_pt[(prim_cnt + c) * 3] = r_red[i];
                            er_cell->ce_color_pt[(prim_cnt + c) * 3 + 1] = r_green[i];
                            er_cell->ce_color_pt[(prim_cnt + c) * 3 + 2] = r_blue[i];
                        } else {
                            er_cell->ce_color_pt[(prim_cnt + c) * 3] = r_red[i];
                            er_cell->ce_color_pt[(prim_cnt + c) * 3 + 1] = r_green[i];
                            er_cell->ce_color_pt[(prim_cnt + c) * 3 + 2] = r_blue[i];
                        }
                        
                        er_cell->ce_normal_pt[(prim_cnt + c) * 3] = 1;
                        er_cell->ce_normal_pt[(prim_cnt + c) * 3 + 1] = 0;
                        er_cell->ce_normal_pt[(prim_cnt + c) * 3 + 2] = 0;
                    }
                    
                    prim_cnt += 4;
                }
                prev_raster = curr_raster;
            }
        }
    }*/
    
    er_cell->ce_prim_cnt = prim_cnt - 4;
}

le_void_t er_cell_para2cart( er_cell_t * const er_cell, le_real_t * edge ){
    // coordinates conversion - points
    edge[2] += LE_ADDRESS_WGS_A;
    
    // coordinates conversion - points
    le_real_t er_opta = edge[0];
    le_real_t er_optb = edge[1];
    
    // coordinates conversion - points
    edge[1] = edge[2] * sin( er_optb ) - er_cell->ce_edge[1];
    edge[2] = edge[2] * cos( er_optb );
    edge[0] = edge[2] * sin( er_opta ) - er_cell->ce_edge[0];
    edge[2] = edge[2] * cos( er_opta ) - er_cell->ce_edge[2];
}

le_void_t er_cell_display( er_cell_t * const er_cell ){
    glVertexPointer(3, GL_DOUBLE, 0, er_cell->ce_vertex_pt);
    glColorPointer (3, GL_UNSIGNED_BYTE, 0, er_cell->ce_color_pt);
    glNormalPointer(GL_DOUBLE, 0, er_cell->ce_normal_pt);
    glDrawArrays(GL_QUADS, 0, er_cell->ce_prim_cnt);
}
