#include <stdio.h>
#include <string.h>
#include <assert.h>

// max length of buffer

#include "util_ip_opts.h"
#include "common.h"
#include "util_common.h"

char default_buffer_v4[DEF_OPTLEN_V4];
char default_buffer_v6[DEF_OPTLEN_V6];


int init_default_ip_opts_buffer(){

    int len_v6 =  hex_str_to_buffer(
        "06 06 "
        "03 34 "
        "01 02 03 04 "
        "A1 A2 A3 A4 A5 A6 A7 A8 "
        "B1 B2 B3 B4 B5 B6 B7 B8 "
        "C0 C1 C2 C3 C4 C5 C6 C7 C8 C9 CA CB CC CD CE CF "
        "D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF", 
        default_buffer_v6, DEF_OPTLEN_V6);
    
    assert(len_v6 == DEF_OPTLEN_V6);

    int len_v4 =  hex_str_to_buffer(
        "07 17 18 00 00 00 00 00 "
        "00 D2 D1 8C 90 00 02 01 "
        "02 03 04 33 33 33 33 00", 
                default_buffer_v4, DEF_OPTLEN_V4);
    
    assert(len_v4 == DEF_OPTLEN_V4);

    return STATUS_OKAY;

}