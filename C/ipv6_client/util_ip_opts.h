#ifndef UTIL_IP_OPTS_H
#define UTIL_IP_OPTS_H


#define DEF_OPTLEN_V4 24
#define DEF_OPTLEN_V6 56

#ifndef IPV6_DSTOPTS
#define IPV6_DSTOPTS IPV6_2292DSTOPTS
#endif

extern char default_buffer_v4[DEF_OPTLEN_V4];
extern char default_buffer_v6[DEF_OPTLEN_V6];

int init_default_ip_opts_buffer();

#endif