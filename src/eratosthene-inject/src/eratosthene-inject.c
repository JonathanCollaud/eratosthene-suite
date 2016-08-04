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

    # include "eratosthene-inject.h"

/*
    source - injection procedure
 */

    void er_injection( le_sock_t const er_client, FILE * const er_stream ) {

        /* Socket i/o count variables */
        le_size_t er_fread = 1;
        le_size_t er_write = 1;

        /* Socket i/o buffer variables */
        le_byte_t er_buffer[LE_NETWORK_SB_SYNC] = LE_NETWORK_C;

        /* Stream data injection */
        while ( ( er_fread > 0 ) && ( er_fread == er_write ) ) {

            /* Read stream elements */
            if ( ( er_fread = fread( ( le_void_t * ) er_buffer, sizeof( le_byte_t ), _LE_USE_MTU, er_stream ) ) > 0 ) {

                /* Write buffer to socket */
                if ( ( er_write = write( er_client, er_buffer, er_fread ) ) != er_fread ) {

                    /* Display message */
                    fprintf( stderr, "eratosthene-inject : error : partial buffer writing on socket\n" );

                }    

            }

        }

    }

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        /* Stream handle variables */
        FILE * er_stream = NULL;

        /* Socket handle variables */
        le_sock_t er_client = _LE_SOCK_NULL;

        /* Create input stream */
        if ( ( er_stream = fopen( lc_read_string( argc, argv, "--file", "-f" ), "rb" ) ) == NULL ) {

            /* Display message */
            fprintf( stderr, "eratosthene-inject : error : unable to access file\n" );

        } else {

            /* Create client handle */
            if ( ( er_client = le_client_create( ( le_char_t * ) lc_read_string( argc, argv, "--ip", "-i" ), lc_read_uint( argc, argv, "--port", "-t", _LE_USE_PORT ) ) ) == _LE_SOCK_NULL ) {

                /* Display message */
                fprintf( stderr, "eratosthene-inject : error : unable to connect to server\n" );

            } else {

                /* Client-server injection handshake */
                if ( le_client_handshake( er_client, LE_NETWORK_MODE_IMOD, LE_ARRAY_SFD ) != LE_ERROR_SUCCESS ) {

                    /* Display message */
                    fprintf( stderr, "eratosthene-inject : error : authorisation failed\n" );

                } else {

                    /* Injection procedure */
                    er_injection( er_client, er_stream );

                }

                /* Delete client handle */
                er_client = le_client_delete( er_client );

            }

            /* Delete input stream */
            fclose( er_stream );

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

