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
    le_size_t raster_side = two_span * two_span * two_span;
    
    le_byte_t * raster = calloc( raster_side, sizeof(le_byte_t));
    if (!raster) return 0;
    
    le_size_t * r_red = calloc( raster_side, sizeof(le_size_t));
    if (!r_red) return 0;
    
    le_size_t * r_green = calloc( raster_side, sizeof(le_size_t));
    if (!r_green) return 0;
    
    le_size_t * r_blue = calloc( raster_side, sizeof(le_size_t));
    if (!r_blue) return 0;
    
    le_real_t denom = 1 << er_cell_get_size(er_cell);
    
    le_real_t size[3] = {
        LE_ADDRESS_RAN_L / denom,
        2 * LE_ADDRESS_RAN_A / denom,
        1024 * LE_ADDRESS_RAN_H / denom};
    
    le_size_t x = 0;
    le_size_t y = 0;
    le_size_t z = 0;
    
    le_real_t er_pose[3];
    le_address_get_pose(& er_cell->ce_addr, er_pose);
    
    /* parsing socket array */
    while ( ( er_head - er_base ) < er_size ) {
        x = round((( ( le_real_t * ) er_head )[0] - er_pose[0]) * two_span / size[0]);
        y = round((( ( le_real_t * ) er_head )[1] - er_pose[1]) * two_span / size[1]);
        z = round((( ( le_real_t * ) er_head )[2] - er_pose[2]) * two_span / size[2]);
        
        raster[(((x * two_span) + y ) * two_span) + z] = 1;
        r_red[(((x * two_span) + y ) * two_span) + z] = (er_head + LE_ARRAY_UF3_POSE)[0];
        r_green[(((x * two_span) + y ) * two_span) + z] = (er_head + LE_ARRAY_UF3_POSE)[1];
        r_blue[(((x * two_span) + y ) * two_span) + z] = (er_head + LE_ARRAY_UF3_POSE)[2];
        
        er_head += LE_ARRAY_UF3;
    }
    
    if (!er_cell->ce_vertex_pt) free( er_cell->ce_vertex_pt );
    if (!er_cell->ce_color_pt) free( er_cell->ce_color_pt );
    if (!er_cell->ce_normal_pt) free( er_cell->ce_normal_pt );
    
    er_cell->ce_vertex_pt = calloc(4 * 3 * 3 * 2 * er_size, sizeof(le_real_t));
    if (!er_cell->ce_vertex_pt) return 0;
    
    er_cell->ce_color_pt = calloc(4 * 3 * 3 * 2 * er_size, sizeof(le_size_t));
    if (!er_cell->ce_color_pt) return 0;
    
    er_cell->ce_normal_pt = calloc(4 * 3 * 3 * 2 * er_size, sizeof(le_real_t));
    if (!er_cell->ce_normal_pt) return 0;
    
    er_cell->ce_prim_cnt = 0;
    
    for (le_size_t axis = 0; axis < 3; axis ++) {
        er_cell_gen_prim(er_cell, raster, r_red, r_green, r_blue, axis);
    }
    
    free( raster );
    free( r_red );
    free( r_green );
    free( r_blue );
    
    er_cell->ce_color_pt = realloc(er_cell->ce_color_pt, er_cell->ce_prim_cnt * 3 * sizeof(le_size_t));
    if (!er_cell->ce_color_pt) return 0;
    er_cell->ce_vertex_pt = realloc(er_cell->ce_vertex_pt, er_cell->ce_prim_cnt * 3 * sizeof(le_real_t));
    if (!er_cell->ce_vertex_pt) return 0;
    er_cell->ce_normal_pt = realloc(er_cell->ce_normal_pt, er_cell->ce_prim_cnt * 3 * sizeof(le_real_t));
    if (!er_cell->ce_normal_pt) return 0;
    
    /* return cell size */
    return( er_size );
    
}

le_void_t er_cell_gen_prim(er_cell_t * const er_cell,
                           const le_byte_t * const raster,
                           const le_size_t * const r_red,
                           const le_size_t * const r_green,
                           const le_size_t * const r_blue,
                           const le_size_t axis){
    le_real_t corner[3] = {0.0, 0.0, 0.0};
    le_real_t edge[3] = {0.0, 0.0, 0.0};
    le_real_t er_pose[3] = {0.0, 0.0, 0.0};
    le_address_get_pose(& er_cell->ce_addr, er_pose);
    
    le_size_t color[3] = {0, 0, 0};
    le_real_t normal[3] = {0.0, 0.0, 0.0};
    le_real_t normal1[3] = {1.0, 1.0, 1.0};
    
    le_size_t span = le_address_get_span(&er_cell->ce_addr);
    le_size_t two_span = 1 << span;
    
    le_real_t denom = 1 << er_cell_get_size(er_cell);
    le_real_t size[3] = {
        LE_ADDRESS_RAN_L / denom,
        2 * LE_ADDRESS_RAN_A / denom,
        1024 * LE_ADDRESS_RAN_H / denom};
    
    le_size_t prim_cnt = er_cell->ce_prim_cnt;
    le_size_t curr_i = 0;
    le_size_t prev_i = 0;
    le_size_t curr_raster = 0;
    le_size_t prev_raster = 0;
    
    le_size_t a[3] = {axis, (axis + 1) % 3, (axis + 2) % 3};
    le_size_t e[3] = {0, 0, 0};
    
    for (e[0] = 0; e[0] < two_span; e[0]++) {
        
        for (e[1] = 0; e[1] < two_span; e[1]++) {
            prev_raster = 0;
            
            for (e[2] = 0; e[2] < two_span; e[2]++) {
                
                curr_i = er_cell_get_raster_index(e, a, two_span);
                curr_raster = raster[curr_i];
                
                if (prev_raster != curr_raster) {
                    corner[a[0]] = er_pose[a[0]] + (e[0] * size[a[0]] / two_span);
                    corner[a[1]] = er_pose[a[1]] + (e[1] * size[a[1]] / two_span);
                    corner[a[2]] = er_pose[a[2]] + (e[2] * size[a[2]] / two_span);
                    
                    /* enter in shape if current raster is one, exit otherwise */
                    le_size_t inside_index = curr_raster == 1? curr_i : prev_i;
                    
                    for (le_size_t v = 0; v < 4; v++) {
                        edge[0] = corner[0];
                        edge[1] = corner[1];
                        edge[2] = corner[2];
                        
                        if (curr_raster == 1){
                            if (v >= 2) edge[a[0]] += size[a[0]] / two_span;
                            if (v == 1 || v == 2) edge[a[1]] += size[a[1]] / two_span;
                        } else {
                            if (v == 1 || v == 2) edge[a[0]] += size[a[0]] / two_span;
                            if (v >= 2) edge[a[1]] += size[a[1]] / two_span;
                        }
                        
                        er_cell_para2cart(er_cell, edge);
                        
                        er_cell_vertex_convolution(raster, r_red, r_green, r_blue,
                                                   e, a, inside_index, v, two_span, color, normal);
                        
                        er_cell_set_primitive(er_cell, (prim_cnt + v), edge, color, normal1);
                    }
                    
                    prim_cnt += 4;
                }
                prev_i = curr_i;
                prev_raster = curr_raster;
            }
            
            // e[2] == two_span, cell's border case
            if (prev_raster == 1) {
                corner[a[0]] = er_pose[a[0]] + (e[0] * size[a[0]] / two_span);
                corner[a[1]] = er_pose[a[1]] + (e[1] * size[a[1]] / two_span);
                corner[a[2]] = er_pose[a[2]] + (e[2] * size[a[2]] / two_span);
                
                for (le_size_t v = 0; v < 4; v++) {
                    edge[0] = corner[0];
                    edge[1] = corner[1];
                    edge[2] = corner[2];
                    
                    if (v == 1 || v == 2) edge[a[0]] += size[a[0]] / two_span;
                    if (v >= 2) edge[a[1]] += size[a[1]] / two_span;
                    
                    er_cell_para2cart(er_cell, edge);
                    
                    //er_cell_vertex_convolution(raster, r_red, r_green, r_blue,
                    //                           e, a, prev_i, v, two_span, color, normal);
                    
                    er_cell_set_primitive(er_cell, (prim_cnt + v), edge, color, normal1);
                }
                
                prim_cnt += 4;
            }
        }
    }
    er_cell->ce_prim_cnt = prim_cnt;
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

le_size_t er_cell_get_raster_index( const le_size_t * const e, const le_size_t * const a, le_size_t const two_span) {
    return (((e[(3 - a[0]) % 3] * two_span) + e[2 - a[1]] ) * two_span) + e[(4 - a[2]) % 3];
}

le_void_t er_cell_vertex_convolution ( const le_byte_t * const raster,
                                      const le_size_t * const r_red,
                                      const le_size_t * const r_green,
                                      const le_size_t * const r_blue,
                                      const le_size_t * const e,
                                      const le_size_t * const a,
                                      le_size_t const index,
                                      le_size_t const v,
                                      le_size_t const two_span,
                                      le_size_t * color,
                                      le_real_t * normal ){
    
    le_size_t r1 = 0;
    le_size_t r2 = 0;
    le_size_t r3 = 0;
    
    /*if (curr_raster == 1){
        o1 = (v == 1 || v == 2) ? 1 : -1;
        o2 = (v >= 2) ? 1 : -1;
    } else {
        o1 = (v >= 2) ? 1 : -1;
        o2 = (v == 1 || v == 2) ? 1 : -1;
    }*/
    
    le_size_t o1 = (v == 1 || v == 2) ? 1 : -1;
    le_size_t o2 = (v >= 2) ? 1 : -1;
    
    le_size_t nb_v = 1;
    for (le_size_t n = 0; n <= 2; n++) {
        /*printf("e1 = %d; e2 = %d; o1 = %d; o2 = %d; i = %d; (%d, %d, %d); (", e[0], e[1], o1, o2, index,
               index + o1 * two_span * two_span,
               index + (o1 * two_span + o2) * two_span,
               index + o2 * two_span);*/
        
        if ((e[0] == 0 && o1 == -1) || (e[0] == two_span - 1 && o1 == 1)) {
            r1 = 0;
        } else {
            r1 = raster[index + o1 * two_span * two_span];
            nb_v++;
        }
        
        //printf("%d, ", r1);
        
        if ((e[0] == 0 && o1 == -1) || (e[0] == two_span - 1 && o1 == 1) || (e[1] == 0 && o2 == -1) || (e[1] == two_span - 1 && o2 == 1)) {
            r2 = 0;
        } else {
            r2 = raster[index + (o1 * two_span + o2) * two_span];
            nb_v++;
        }
        
        //printf("%d, ", r2);
        
        if ((e[1] == 0 && o2 == -1) || (e[1] == two_span - 1 && o2 == 1)){
            r3 = 0;
        } else {
            r3 = raster[index + o2 * two_span];
            nb_v++;
        }
        
        //printf("%d)\n", r3);
        
        normal[n] = (1 + r1 + r2 + r3) / nb_v;
    }
    
    color[0] = r_red[index];
    color[1] = r_green[index];
    color[2] = r_blue[index];
}

le_void_t er_cell_set_primitive( er_cell_t * const er_cell,
                                const le_size_t index,
                                const le_real_t * const edge,
                                const le_size_t * const color,
                                const le_real_t * const normal ){
    for (le_size_t j = 0; j <= 2; j++) {
        er_cell->ce_vertex_pt[index * 3 + j] = edge[j];
        er_cell->ce_color_pt[index * 3 + j] = color[j];
        er_cell->ce_normal_pt[index * 3 + j] = normal[j];
    }
}

le_void_t er_cell_display( er_cell_t * const er_cell ){
    glVertexPointer(3, GL_DOUBLE, 0, er_cell->ce_vertex_pt);
    glColorPointer (3, GL_UNSIGNED_BYTE, 0, er_cell->ce_color_pt);
    glNormalPointer(GL_DOUBLE, 0, er_cell->ce_normal_pt);
    glDrawArrays(GL_QUADS, 0, er_cell->ce_prim_cnt);
}
