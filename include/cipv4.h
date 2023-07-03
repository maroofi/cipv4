/** @file */
#include <stdint.h>

#ifndef _CIPV4_H_
#define _CIPV4_H_


#define DOT '.'
#define CIPV4_PRAVATE_ARRAY_LENGTH 14

/**
* @details Type definition of the struct _cipv4_ctx
* 
* cipv4_ctx: structure of type _cipv4_ctx
* 
*/
typedef struct _cipv4_ctx cipv4_ctx;


/**
 * @details This structure contains all the necessary information for IPv4.
 */
struct _cipv4_ctx{
    char * raw;             ///< raw (string) representation of the IP address
    uint32_t addr;          ///< This is the integer representation of the IP address
    uint8_t network_prefix;   ///< This is the network prefix len which is between 1~32
    int error;                ///< if any error occurred, it will appear here
    char * err_msg;           ///< description of the error code goes here
    uint32_t addr_start;      ///< first IP address in range
    uint32_t addr_end;        ///< end of ip address range for this prefix
};



cipv4_ctx * cipv4_parse_ip(const char * ip);
char * cipv4_get_network_address(cipv4_ctx ctx);
char * cipv4_get_broadcast_address(cipv4_ctx ctx);
char * cipv4_get_subnet_mask(cipv4_ctx* ctx, char * buffer);
char * cipv4_get_host_mask(cipv4_ctx* ctx, char * buffer);
int cipv4_is_address_in(cipv4_ctx* ctx, char * addr);
int cipv4_is_ip_valid(const char * ip);
uint32_t cipv4_str_to_uint(const char * ip);
const char * cipv4_uint_to_str(uint32_t addr, const char * buffer);
unsigned long int cipv4_count_ips_in_range(cipv4_ctx*ctx);

/*************check ip type functions***********/
int cipv4_is_reserved_from_string(const char * ip);
int cipv4_is_reserved(cipv4_ctx * ctx);
int cipv4_is_multicast(cipv4_ctx * ctx);
int cipv4_is_multicast_from_string(const char * ip);
int cipv4_is_loopback(cipv4_ctx * ctx);
int cipv4_is_loopback_from_string(const char * ip);
int cipv4_is_linklocal(cipv4_ctx * ctx);
int cipv4_is_linklocal_from_string(const char * ip);
int cipv4_is_unspecified(cipv4_ctx * ctx);
int cipv4_is_unspecified_from_string(const char * ip);
int cipv4_is_public_network(cipv4_ctx * ctx);
int cipv4_is_public_network_from_string(const char * ip);
int cipv4_is_global_from_string(const char * ip);
int cipv4_is_global(cipv4_ctx * ctx);
int cipv4_is_private(cipv4_ctx * ctx);
int cipv4_is_private_from_string(const char * ip);
/*************end of check ip type functions*******/

#endif
