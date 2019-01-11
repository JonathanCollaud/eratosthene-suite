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
    er_cell->ce_prim_cnt = 0;
    
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
    
    /* compute raster size */
    le_size_t span = le_address_get_span(&er_cell->ce_addr);;
    le_size_t two_span = 1 << span;
    le_size_t raster_size = two_span * two_span * two_span;
    
    /* rasters initialization */
    le_byte_t * raster = calloc( raster_size, sizeof(le_byte_t));
    if (!raster) return 0;
    le_data_t * r_red = calloc( raster_size, sizeof(le_data_t));
    if (!r_red) return 0;
    le_data_t * r_green = calloc( raster_size, sizeof(le_data_t));
    if (!r_green) return 0;
    le_data_t * r_blue = calloc( raster_size, sizeof(le_data_t));
    if (!r_blue) return 0;
    
    /* compute the voxel size according to the cell's depth and span */
    le_real_t denom = 1 << er_cell_get_size(er_cell);
    le_real_t size[3] = {
        LE_ADDRESS_RAN_L / denom,
        2 * LE_ADDRESS_RAN_A / denom,
        1024 * LE_ADDRESS_RAN_H / denom};
    
    /* initialize raster coordinates */
    le_size_t x = 0;
    le_size_t y = 0;
    le_size_t z = 0;
    
    /* initialize index */
    le_size_t index = 0;
    
    /* retrive cell position */
    le_real_t er_pose[3];
    le_address_get_pose(& er_cell->ce_addr, er_pose);
    
    /* parsing socket array and filling rasters */
    while ( ( er_head - er_base ) < er_size ) {
        x = round((( ( le_real_t * ) er_head )[0] - er_pose[0]) * two_span / size[0]);
        y = round((( ( le_real_t * ) er_head )[1] - er_pose[1]) * two_span / size[1]);
        z = round((( ( le_real_t * ) er_head )[2] - er_pose[2]) * two_span / size[2]);
        
        index = (((x * two_span) + y ) * two_span) + z;
        
        raster[index] = 1;
        r_red[index] = (er_head + LE_ARRAY_UF3_POSE)[0];
        r_green[index] = (er_head + LE_ARRAY_UF3_POSE)[1];
        r_blue[index] = (er_head + LE_ARRAY_UF3_POSE)[2];
        
        er_head += LE_ARRAY_UF3;
    }
    
    /* primitives pointers initialization to the max possible size, 3 coord * 4 vertices * 6 faces per point */
    
    le_size_t nb_coord = 72 * er_size / LE_ARRAY_UF3;
    
    er_cell->ce_vertex_pt = realloc(er_cell->ce_vertex_pt, nb_coord * sizeof(le_real_t));
    if (!er_cell->ce_vertex_pt) return 0;
    er_cell->ce_color_pt = realloc(er_cell->ce_color_pt, nb_coord * sizeof(le_data_t));
    if (!er_cell->ce_color_pt) return 0;
    er_cell->ce_normal_pt = realloc(er_cell->ce_normal_pt, nb_coord * sizeof(le_real_t));
    if (!er_cell->ce_normal_pt) return 0;
    
    /* generate primitives in the three dimensions */
    for (le_size_t axis = 0; axis < 3; axis ++) {
        er_cell_gen_prim(er_cell, raster, r_red, r_green, r_blue, axis, two_span, size);
    }
    
    /* free rasters */
    free( raster ); raster = NULL;
    free( r_red ); r_red = NULL;
    free( r_green ); r_green = NULL;
    free( r_blue ); r_blue = NULL;
    
    /* adapt the primitives pointers size to their effective size */
    
    nb_coord = er_cell->ce_prim_cnt * 3;
    
    er_cell->ce_vertex_pt = realloc(er_cell->ce_vertex_pt, nb_coord * sizeof(le_real_t));
    if (!er_cell->ce_vertex_pt) return 0;
    er_cell->ce_color_pt = realloc(er_cell->ce_color_pt, nb_coord * sizeof(le_data_t));
    if (!er_cell->ce_color_pt) return 0;
    er_cell->ce_normal_pt = realloc(er_cell->ce_normal_pt, nb_coord * sizeof(le_real_t));
    if (!er_cell->ce_normal_pt) return 0;
    
    /* return cell size */
    return( er_size );
}

le_void_t er_cell_gen_prim(er_cell_t * const er_cell,
                           const le_byte_t * const raster,
                           const le_data_t * const r_red,
                           const le_data_t * const r_green,
                           const le_data_t * const r_blue,
                           const le_size_t axis,
                           const le_size_t two_span,
                           const le_real_t * const size){
    le_real_t corner[3] = {0.0, 0.0, 0.0};
    le_real_t edge[3] = {0.0, 0.0, 0.0};
    
    le_real_t er_pose[3] = {0.0, 0.0, 0.0};
    le_address_get_pose(& er_cell->ce_addr, er_pose);
    
    le_size_t color[3] = {0, 0, 0};
    le_real_t normal[3] = {0.0, 0.0, 0.0};
    
    le_size_t curr_i = 0;
    le_size_t prev_i = 0;
    le_size_t curr_raster = 0;
    le_size_t prev_raster = 0;
    
    le_size_t a[3] = {axis, (axis + 1) % 3, (axis + 2) % 3};
    le_size_t e[3] = {0, 0, 0};
    le_size_t f_v[3] = {0, 0, 0};

    /* iterate through the raster */
    for (e[0] = 0; e[0] < two_span; e[0]++) {
        
        for (e[1] = 0; e[1] < two_span; e[1]++) {
            prev_raster = 0;
            
            for (e[2] = 0; e[2] <= two_span; e[2]++) {
                
                if (e[2] == two_span) {
                    curr_raster = 0;
                } else {
                    curr_i = er_cell_get_raster_index(e, a, two_span);
                    curr_raster = raster[curr_i];
                }
                
                /* Different values, set a face */
                if (prev_raster != curr_raster) {
                    
                    /* Retrive face's (or voxel) corner, according to the axis */
                    for (le_size_t i = 0; i < 3; i++) {
                        corner[a[i]] = er_pose[a[i]] + (e[i] * size[a[i]] / two_span);
                    }
                    
                    /* iterate over face's vertices */
                    for (le_size_t v = 0; v < 4; v++) {
                        for (le_size_t i = 0; i < 3; i++) {
                            edge[i] = corner[i];
                            f_v[i] = e[i];
                            color[i] = 0;
                            normal[i] = 0.0;
                        }
                        
                        /* If the current raster is 1, then we enter in the voxel (clockwise rotation).
                         If it's 0 (exit the voxel) or we reach the cell's border, then counter-clockwise rotate */
                        if (curr_raster) {
                            if (v == 1 || v == 2) {
                                edge[a[1]] += size[a[1]] / two_span;
                                f_v[1]++;
                            }
                            if (v >= 2) {
                                edge[a[0]] += size[a[0]] / two_span;
                                f_v[0]++;
                            }
                        } else {
                            if (v == 1 || v == 2) {
                                edge[a[0]] += size[a[0]] / two_span;
                                f_v[0]++;
                            }
                            if (v >= 2) {
                                edge[a[1]] += size[a[1]] / two_span;
                                f_v[1]++;
                            }
                        }
                        
                        /* convert vertex edge to cartesian coordinates */
                        er_cell_para2cart(er_cell, edge);
                        
                        /* convolve color and normal on the vertex neighbours */
                        er_cell_vertex_convolution(raster, r_red, r_green, r_blue,
                                                   f_v, a, two_span, color, normal);
                        
                        /* add primitive to cell */
                        er_cell_set_primitive(er_cell, er_cell->ce_prim_cnt + v, edge, color, normal);
                    }
                    
                    er_cell->ce_prim_cnt += 4;
                }
                
                prev_i = curr_i;
                prev_raster = curr_raster;
            }
        }
    }
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
                                      const le_data_t * const r_red,
                                      const le_data_t * const r_green,
                                      const le_data_t * const r_blue,
                                      const le_size_t * const f_v,
                                      const le_size_t * const a,
                                      le_size_t const two_span,
                                      le_size_t * const color,
                                      le_real_t * const normal ){
    /* for each neighbour of a vertex */
    le_size_t neighbours_cnt = 0;
    for (le_size_t n = 0 ; n < 8; n++) {
        
        /* retrive its raster coordinates */
        // faces vertices live in range [0, two_span], hence kernel vertices live in [-1, two_span].
        le_size_t k_v[3] = {f_v[0], f_v[1], f_v[2]};
        
        /* convolve according to the axis */
        if (n >= 4) k_v[a[0]]--;
        if (n % 4 >= 2) k_v[a[1]]--;
        if (n % 2 == 1) k_v[a[2]]--;
        
        /* clamp the kernel vertices to the border of the cell */
        for (le_size_t i = 0; i < 3; i++) {
            if (k_v[i] < 0 || k_v[i] >= two_span)
                k_v[i] = k_v[i] == two_span ? two_span - 1 : 0;
        }
        
        le_size_t n_index = er_cell_get_raster_index(k_v, a, two_span);
        
        /* if the neighbour is full, adapt color and normal */
        if (raster[n_index]) {
            color[0] += r_red[n_index];
            color[1] += r_green[n_index];
            color[2] += r_blue[n_index];
            
            if (n >= 4) normal[a[0]]++;
            else normal[a[0]]--;
            if (n % 4 >= 2) normal[a[1]]++;
            else normal[a[1]]--;
            if (n % 2 == 1) normal[a[2]]++;
            else normal[a[2]]--;
            
            neighbours_cnt++;
        }
    }
    
    for (le_size_t i = 0; i < 3; i++) {
        /* average the color */
        color[i] /= neighbours_cnt;
        
        /* normalize normals for cell borders */
        if (f_v[i] == 0 || f_v[i] >= two_span){
            normal[i] = 0.0;
            normal[(i + 1) % 3] /= 2.0;
            normal[(i + 2) % 3] /= 2.0;
        }
    }
    
}

le_void_t er_cell_set_primitive( er_cell_t * const er_cell,
                                const le_size_t index,
                                const le_real_t * const edge,
                                const le_size_t * const color,
                                const le_real_t * const normal ){
    for (le_size_t i = 0; i < 3; i++) {
        er_cell->ce_vertex_pt[index * 3 + i] = edge[i];
        er_cell->ce_color_pt[index * 3 + i] = color[i];
        er_cell->ce_normal_pt[index * 3 + i] = normal[i];
    }
}

le_void_t er_cell_display( er_cell_t * const er_cell ) {
    glVertexPointer(3, GL_DOUBLE, 0, er_cell->ce_vertex_pt);
    glColorPointer (3, GL_UNSIGNED_BYTE, 0, er_cell->ce_color_pt);
    glNormalPointer(GL_DOUBLE, 0, er_cell->ce_normal_pt);
    glDrawArrays(GL_QUADS, 0, er_cell->ce_prim_cnt);
}
