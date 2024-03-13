
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "util_common.h"
#include "common.h"


global_stats_t global_stats = {0,   0,0,0,  0,0,0};


int is_hex_char(char c){
    return ('0' <= c && c <= '9') || ('A' <= c && c <= 'F') ;
}

// Expected format "XX XX XX XX XX"
// length n => len+1/3 bytes
int hex_str_to_buffer(const char* hex_str, char* buffer, int max_len){
    int hex_str_len = strlen(hex_str);
    int n_bytes = (hex_str_len+1)/3 ;

    //printf("max_len = %d, hex_str_len = %d", max_len, hex_str_len);
    
    assert(max_len >= n_bytes);

    char cur_hex[3];
    cur_hex[2] = '\0';
    int i=0;
    int bsize = 0;
    while(i<hex_str_len){
        if( is_hex_char(hex_str[i]))
        {
            assert(i+1 < hex_str_len);
            assert(is_hex_char(hex_str[i+1]));
            if(i+2 < hex_str_len)
                assert(!is_hex_char(hex_str[i+2]));

            cur_hex[0] = hex_str[i];
            cur_hex[1] = hex_str[i+1];

            buffer[bsize] = (char) strtol(cur_hex, NULL, 16);
            bsize++;
            i = i+3;
        }
    }

    return bsize;
}


char* get_hex_dump(const char* src, int len){
    /* Creating Hex Dump*/
    int i;
    char* hex_dump = (char*)malloc((len*3+1)*sizeof(char));
    
    
    for(i=0; i<len; i++){
        // ngx_sprintf wont work
        sprintf(hex_dump + i*3, "%02X", (unsigned char)src[i]);
        //ngx_log_stderr(0, "[CUSTOM LOG] i=%ui, out : %02X, out2 : %02x, out3 : %x", 
        //i, opts[i], opts[i], opts[i]);
        hex_dump[i*3+2] = ' ';
    }
    hex_dump[len*3] = '\0';
    return hex_dump;
}

