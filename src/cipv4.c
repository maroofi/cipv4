/// @file cipv4.c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cipv4.h>
#include <util_string.h>


static char _cipv4_ip_loopback[] = "127.0.0.0/8";
static char _cipv4_ip_multicast[] = "224.0.0.0/4";
static char _cipv4_ip_linklocal[] = "169.254.0.0/16";
static char _cipv4_ip_reserved[] = "240.0.0.0/4";
static char _cipv4_ip_unspecified[] = "0.0.0.0";
static char _cipv4_ip_public_network[] = "100.64.0.0/10";
static char _cipv4_ip_private[CIPV4_PRAVATE_ARRAY_LENGTH][19] = {"0.0.0.0/8", "10.0.0.0/8",
                                         "127.0.0.0/8", "240.0.0.0/4", "255.255.255.255/32",
                                        "169.254.0.0/16", "172.16.0.0/12", "192.0.0.0/29",
                                        "192.0.0.170/31", "192.0.2.0/24", "192.168.0.0/16",
                                        "198.18.0.0/15", "198.51.100.0/24", "203.0.113.0/24"};



static cipv4_ctx* cipv4_init(void);

/**
 * @brief Free the memory allocated for the context created by cipv4_parse_ip()
 * @return nothing
 */
void cipv4_free(cipv4_ctx * ctx){
    if (!ctx)
        return;
    if (ctx->raw)
        free(ctx->raw);
    free(ctx->err_msg);
    free(ctx);
}


/**
 * @brief Test if this address is allocated for private networks.
 * @param ctx A pointer to the cipv4 context created by cipv4_parse_ip()
 * @return An Integer, 1 if the address is reserved per 
 * iana-ipv4-special-registry, 0 otherwise and -1 in case of error.
 */
int cipv4_is_private(cipv4_ctx * ctx){
    static cipv4_ctx* private[CIPV4_PRAVATE_ARRAY_LENGTH] = {NULL};
    if (!ctx)
        return -1;
    if (!private[0]){
        // initialize the array
        for (int i=0; i< CIPV4_PRAVATE_ARRAY_LENGTH; ++i)
            private[i] = cipv4_parse_ip(_cipv4_ip_private[i]);
    }
    for (int i=0; i< CIPV4_PRAVATE_ARRAY_LENGTH; ++i)
        if (cipv4_is_address_in(private[i], ctx->raw))
                return 1;
    return 0;
}

/**
 * @brief Test if this address is allocated for private networks.
 * @param ip a null-terminated string contains IPv4 without CIDR
 * @return An Integer, 1 if the address is reserved per
 * iana-ipv4-special-registry, 0 otherwise and -1 in case of error.
 */
int cipv4_is_private_from_string(const char * ip){
    static cipv4_ctx* private[CIPV4_PRAVATE_ARRAY_LENGTH] = {NULL};
    if (!ip || cipv4_is_ip_valid(ip) == 0)
        return -1;
    if (!private[0]){
        // initialize the array
        for (int i=0; i< CIPV4_PRAVATE_ARRAY_LENGTH; ++i)
            private[i] = cipv4_parse_ip(_cipv4_ip_private[i]);
    }
    uint32_t int_ip = cipv4_str_to_uint(ip);
    for (int i=0; i< CIPV4_PRAVATE_ARRAY_LENGTH; ++i)
        if (int_ip >= private[i]->addr_start && int_ip <= private[i]->addr_end)
            return 1;
    return 0;
}

/**
 * @brief Test if this address is allocated for public networks.
 * @param ctx A pointer to the cipv4 context created by cipv4_parse_ip()
 * @return An integer, 1 if the address is reserved per iana-ipv4-special-registry,
 * 0 otherwise and -1 in case of error.
 */
int cipv4_is_public_network(cipv4_ctx * ctx){
    static cipv4_ctx * public = NULL;
    if (!ctx)
        return -1;
    if (!public)
        public = cipv4_parse_ip(_cipv4_ip_public_network);
    if (cipv4_is_address_in(public, ctx->raw))
        return 1;
    return 0;
}

/**
 * @brief Test if this address is allocated for public networks.
 * @param ip A null-terminated string contains the IPv4 address
 * @return 1 if IP address is public network, 0 otherwise and -1 for errors.
 */
int cipv4_is_public_network_from_string(const char * ip){
    static cipv4_ctx * public = NULL;
    if (!ip || cipv4_is_ip_valid(ip) == 0)
        return -1;
    if (!public)
        public = cipv4_parse_ip(_cipv4_ip_public_network);
    uint32_t int_ip = cipv4_str_to_uint(ip);
    if (int_ip >= public->addr_start && int_ip <= public->addr_end)
        return 1;
    return 0;
}


/**
 * @brief Test if this address is a global address.
 * @param ip A null-terminated string contains the IPv4 address
 * @return 1 if IP address is global, 0 otherwise and -1 for errors.
 */
int cipv4_is_global_from_string(const char * ip){
    return (!cipv4_is_public_network_from_string(ip)) &&
           (!cipv4_is_private_from_string(ip));
}

/**
 * @brief Test if this address is a global address.
 * @param ctx cipv4 context returned by cipv4_parse_ip().
 * @return 1 if IP address is global, 0 otherwise and -1 for errors.
 */
int cipv4_is_global(cipv4_ctx * ctx){
    return (!cipv4_is_private(ctx)) &&
           (!cipv4_is_public_network(ctx));
}


/**
 * @brief A boolean, True if the address is a loopback per RFC 3330.
 * @param ctx cipv4 context returned by cipv4_parse_ip().
 * @return 1 if IP address is loopback, 0 otherwise and -1 for errors.
 */
int cipv4_is_loopback(cipv4_ctx * ctx){
    static cipv4_ctx * loopback = NULL;
    if (!ctx)
        return -1;
    if (!loopback)
        loopback = cipv4_parse_ip(_cipv4_ip_loopback);
    if (cipv4_is_address_in(loopback, ctx->raw))
        return 1;
    return 0;
}


/**
 * @brief A boolean, True if the address is a loopback per RFC 3330.
 * @param ip A null-terminated string contains the IPv4 address
 * @return 1 if IP address is loopback, 0 otherwise and -1 for errors.
 */
int cipv4_is_loopback_from_string(const char * ip){
    static cipv4_ctx * loopback = NULL;
    if (!ip || cipv4_is_ip_valid(ip) == 0)
        return -1;
    if (!loopback)
        loopback = cipv4_parse_ip(_cipv4_ip_loopback);
    uint32_t int_ip = cipv4_str_to_uint(ip);
    if (int_ip >= loopback->addr_start && int_ip <= loopback->addr_end)
        return 1;
    return 0;
}


/**
 * @brief A boolean, True if the address is multicast.
 * @param ctx Context returned by cipv4_parse_ip()
 * @return 1 if IP address is multicast, 0 otherwise and -1 for errors.
 */
int cipv4_is_multicast(cipv4_ctx * ctx){
    static cipv4_ctx * multicast = NULL;
    if (!ctx)
        return -1;
    if (!multicast)
        multicast = cipv4_parse_ip(_cipv4_ip_multicast);
    if (cipv4_is_address_in(multicast, ctx->raw))
        return 1;
    return 0;
}

/**
 * @brief A boolean, True if the address is multicast.
 * @param ip A null-terminated string contains the IPv4 address
 * @return 1 if IP address is multicast, 0 otherwise and -1 for errors.
 */
int cipv4_is_multicast_from_string(const char * ip){
    static cipv4_ctx * multicast = NULL;
    if (!ip || cipv4_is_ip_valid(ip) == 0)
        return -1;
    if (!multicast)
        multicast = cipv4_parse_ip(_cipv4_ip_multicast);
    uint32_t int_ip = cipv4_str_to_uint(ip);
    if (int_ip >= multicast->addr_start && int_ip <= multicast->addr_end)
        return 1;
    return 0;
}



/**
 * @brief A boolean, True if this is the unspecified address as defined in RFC 5735 3.
 * @param ctx Context returned by cipv4_parse_ip()
 * @return 1 if IP address is unspecified, 0 otherwise and -1 for errors.
 */
int cipv4_is_unspecified(cipv4_ctx * ctx){
    static cipv4_ctx * unspecified = NULL;
    if (!ctx)
        return -1;
    if (!unspecified)
        unspecified = cipv4_parse_ip(_cipv4_ip_unspecified);
    if (cipv4_is_address_in(unspecified, ctx->raw))
        return 1;
    return 0;
}

/**
 * @brief A boolean, True if this is the unspecified address as defined in RFC 5735 3.
 * @param ip A null-terminated string contains the IPv4 address
 * @return 1 if IP address is unspecified, 0 otherwise and -1 for errors.
 */
int cipv4_is_unspecified_from_string(const char * ip){
    static cipv4_ctx * unspecified = NULL;
    if (!ip || cipv4_is_ip_valid(ip) == 0)
        return -1;
    if (!unspecified)
        unspecified = cipv4_parse_ip(_cipv4_ip_unspecified);
    uint32_t int_ip = cipv4_str_to_uint(ip);
    if (int_ip >= unspecified->addr_start && int_ip <= unspecified->addr_end)
        return 1;
    return 0;
}



/**
 * @brief A boolean, True if the address is link-local per RFC 3927.
 * @param ip A null-terminated string contains the IPv4 address
 * @return 1 if IP address is link-local, 0 otherwise and -1 for errors.
 */
int cipv4_is_linklocal_from_string(const char * ip){
    static cipv4_ctx * linklocal = NULL;
    if (!ip || cipv4_is_ip_valid(ip) == 0)
        return -1;
    if (!linklocal)
        linklocal = cipv4_parse_ip(_cipv4_ip_linklocal);
    uint32_t int_ip = cipv4_str_to_uint(ip);
    if (int_ip >= linklocal->addr_start && int_ip <= linklocal->addr_end)
        return 1;
    return 0;
}


/**
 * @brief A boolean, True if the address is link-local per RFC 3927.
 * @param ctx Context returned by cipv4_parse_ip()
 * @return 1 if IP address is link-local, 0 otherwise and -1 for errors.
 */
int cipv4_is_linklocal(cipv4_ctx * ctx){
    static cipv4_ctx * linklocal = NULL;
    if (!ctx)
        return -1;
    if (!linklocal)
        linklocal = cipv4_parse_ip(_cipv4_ip_linklocal);
    if (cipv4_is_address_in(linklocal, ctx->raw))
        return 1;
    return 0;
}


/**
 * @brief A boolean, True if the address is within the reserved IPv4 Network range.
 * @param ip A null-terminated string contains the IPv4 address
 * @return 1 if IP address is reserved, 0 otherwise and -1 for errors.
 */
int cipv4_is_reserved_from_string(const char * ip){
    static cipv4_ctx * reserved = NULL;
    if (!ip || cipv4_is_ip_valid(ip) == 0)
        return -1;
    if (!reserved)
        reserved = cipv4_parse_ip(_cipv4_ip_reserved);
    uint32_t int_ip = cipv4_str_to_uint(ip);
    if (int_ip >= reserved->addr_start && int_ip <= reserved->addr_end)
        return 1;
    return 0;
}

/**
 * @brief A boolean, True if the address is within the reserved IPv4 Network range.
 * @param ctx Context returned by cipv4_parse_ip()
 * @return 1 if IP address is reserved, 0 otherwise and -1 for errors.
 */
int cipv4_is_reserved(cipv4_ctx * ctx){
    static cipv4_ctx * reserved = NULL;
    if (!ctx)
        return -1;
    if (!reserved)
        reserved = cipv4_parse_ip(_cipv4_ip_reserved);
    if (cipv4_is_address_in(reserved, ctx->raw))
        return 1;
    return 0;
}


/**
 * @brief Counts the number of IP addresses in the IP Network
 * @param ctx Context returned by cipv4_parse_ip()a
 * @return A number greater than 0
 */
unsigned long int cipv4_count_ips_in_range(cipv4_ctx*ctx){
    return (ctx->addr_end) - (ctx->addr_start) +1;
}

/**
 * @brief Test if the provided IP address is inside the range
 * @param ctx Context returned by cipv4_parse_ip()
 * @param addr a null-terminated string contains the IPv4 address
 * @return 1 if `addr` is in range, 0 otherwise and -1 if `addr` is not valid
 */
int cipv4_is_address_in(cipv4_ctx* ctx, char * addr){
    if (cipv4_is_ip_valid(addr) != 1)
        return -1;
    uint32_t int_addr = cipv4_str_to_uint(addr);
    if (int_addr >= ctx->addr_start && int_addr <= ctx->addr_end)
        return 1;
    return 0;
}

/**
 * @brief Returns the mask (in form of IP) for the host
 * @param ctx Context created by calling cipv4_parse_ip()
 * @param buffrer User-provided buffer to receive the result.
 * @return A pointer to the user-provided buffer in case of success or
 * NULL in case of failure.
 *
 * The buffer must be large enough to receive the IP address. The 
 * biggest possible value is '255.255.255.255' so  a buffer of size 16 is 
 * always enough
 */
char * cipv4_get_host_mask(cipv4_ctx* ctx, char * buffer){
    if (!buffer || !ctx)
        return NULL;
    uint32_t np = ctx->network_prefix;
    int shft = 31;
    uint32_t final = 0;
    for (int i=0;i<np;++i)
        final = final|(1<<shft--);
    final = ~final;
    cipv4_uint_to_str(final, (const char*) buffer);
    return buffer;
}

/**
 * @brief Returns the subnet mask (in a form of IPv4 address).
 * @param ctx Context returned by cipv4_parse_ip()
 * @param buffer user-provided buffer to receive the result.
 * @return A pointer to the user-provided buffer (success) or NULL
 * in case of failure.
 *
 * A buffer of size 16 (char buffer[16]) is enough to cover all cases.
 */
char * cipv4_get_subnet_mask(cipv4_ctx * ctx, char * buffer){
    if (!buffer || !ctx)
        return NULL;
    uint32_t np = ctx->network_prefix;
    int shft = 31;
    uint32_t final = 0;
    for (int i=0;i<np;++i)
        final = final|(1<<shft--);
    cipv4_uint_to_str(final, (const char*) buffer);
    return buffer;
}


static cipv4_ctx* cipv4_init(void){
    cipv4_ctx * ctx = (cipv4_ctx*) malloc(sizeof(cipv4_ctx));
    if (!ctx)
        return NULL;
    ctx->err_msg = (char*) malloc(256);
    if (!ctx->err_msg){
        free(ctx);
        return NULL;
    }
    for(int i=0; i<256; ++i)
        ctx->err_msg[i] = '\0';
    ctx->error = 0;    //success
    ctx->raw = NULL;
    return ctx;
}

/**
 * @brief Coverts an Integer IP address to string representation
 * @param addr IP address in a form of 32-bit integer
 * @param buffer A user-provided buffer to receive the result.
 * @return Returns a pointer to the User provided buffer in case of success
 * or NULL in case of failure.
 */
const char * cipv4_uint_to_str(uint32_t addr, const char * buffer){
    if (!buffer)
        return NULL;
    char * tmp = (char *) buffer;
    uint8_t part = 0;
    int shft = 3;
    unsigned long int len = 0;
    for (int i=0; i<4; ++i){
        part = (addr >> (8* shft--)) & 0xFF;
        sprintf(tmp, "%d", part);
        if (i<3){
            len = str_len(tmp);
            tmp[len] = '.';
            tmp[len +1] = '\0';
        }
        tmp = tmp + str_len(tmp);
    }
    return buffer;
}

/**
 * @brief parse the provided IPv4 address with optional prefix
 * @param A pointer to the null-terminated string contains IPv4 address
 * @return A pointer to the allocated memory of type cipv4_ctx or NULL
 * in case of error.
 * 
 * make sure that the input is not NULL before passing it to the function.
 *
 * @code
 * int main()
 *    ctx = cipv4_parse_ip("10.20.30.40/24");
 *    if (!ctx){
 *        fprintf(stdout, "Can not parse the IP address\n");
 *        return 1;
 *    }
 *    // the ip address is parsed and stored in ctx now.
 *    //do some stuff here...
 *    return 0;
 * }
 * @endcode
 */
cipv4_ctx * cipv4_parse_ip(const char * ip){
    if (NULL == ip || str_len(ip) > 20)     // what can I do?
        return NULL;
    char tmp[20] = {0};
    str_ncpy(tmp, ip, 19);
    int has_prefix = 0;
    char * prefix_pos = NULL;
    prefix_pos = str_chr(tmp, '/');
    if (prefix_pos != NULL)
        has_prefix = 1;
    cipv4_ctx * ctx = cipv4_init();
    if (!ctx)
        return NULL;
    if (has_prefix)
        *prefix_pos = '\0';
    if (cipv4_is_ip_valid(tmp) != 1){
       strcpy(ctx->err_msg, "Provided IP address is not valid");
       ctx->error = 1;
       return ctx;
    }
    // check if ip part is valid
    unsigned int prefix = 32;
    if (has_prefix)
        prefix = str_to_uint(prefix_pos+1);
    if (prefix > 32 || prefix < 1){
        str_cpy(ctx->err_msg, "Wrong prefix provided");
        ctx->error = 2;
        return ctx;
    }
    if (has_prefix && *(prefix_pos+1) == '0'){
        str_cpy(ctx->err_msg, "Wrong prefix provided");
        ctx->error = 2;
        return ctx;
    }
    ctx->network_prefix = prefix;
    ctx->error = 0;
    ctx->err_msg[0] = '\0';
    ctx->raw = (char*) malloc(str_len(ip)+1);
    ctx->addr = cipv4_str_to_uint(tmp);
    if (!ctx->raw){
        ctx->error = 3;
        str_cpy(ctx->err_msg, "Can not allocate memory");
        ctx->raw = NULL;
        return ctx;
    }
    str_cpy(ctx->raw, ip);
    if (ctx->network_prefix == 32){
        ctx->addr_start = ctx->addr;
        ctx->addr_end = ctx->addr;
    }else{
        char buffer[20];
        ctx->addr_start = cipv4_str_to_uint(cipv4_get_subnet_mask(ctx, buffer)) & ctx->addr;
        ctx->addr_end = cipv4_str_to_uint(cipv4_get_host_mask(ctx, buffer)) | ctx->addr;
    }
    return ctx;
}

/**
 * @brief Convert an IP address from string form to integer form
 * @param ip A pointer to a null-terminated string contains the IP address
 * @return an unsigned 32-bit integer number representing `ip`.
 *
 * You need to make sure the input IP address is valid before using
 * this function. To do so, you can call cipv4_is_ip_valid() before
 * calling this function.
 */
uint32_t cipv4_str_to_uint(const char *ip){
    // convert IPv4 ip to a number representation
    // example: 1.2.3.4 -> 16909060
    // you need to make sure the input IP address is valid
    // you can use cipv4_is_ip_valid() function for that.
    uint32_t result = 0;
    int i = 0;
    int j = 0;
    char * tmp = (char*)ip;
    uint32_t part = 0;
    char buffer[4] = {0};
    char * dot_pos = NULL;
    for (i=0; i< 3; i++){
        dot_pos = str_chr(tmp, DOT);
        j = 0;
        while(tmp < dot_pos)
            buffer[j++] = *tmp++;
        buffer[j] = '\0';
        part = str_to_uint(buffer);
        result = result + (part << ((3-i)*8));
        tmp = dot_pos+1;
    }
    i = 0;
    while (*tmp)
        buffer[i++] = *tmp++;
    buffer[i] = '\0';
    part = str_to_uint(buffer);
    result = result + part;
    return result;
}
   
/**
 * @brief Test if the input IP address is valid or not.
 * @param ip A pointer to a null-terminated string contains the IP address.
 * @return 1 if the IP address is valid 0 otherwise.
 */
int cipv4_is_ip_valid(const char * ip){
    if ((!ip) || (str_len(ip) < 7) || (str_count(ip, DOT) != 3) || (str_len(ip) > 15)) 
        return 0;
    char * tmp = (char*)ip;
    char * dot_pos = NULL;
    char buffer[16] = {0};
    int i = 0;
    int is_invalid = 0;
    int j = 0;
    unsigned int part = 0;
    size_t buffer_len = 0;
    for(i =0; i<3; i++){
        dot_pos = str_chr(tmp, DOT);
        j = 0;
        while (tmp<dot_pos){
            if (!is_digit(*tmp))
                return 0;
            buffer[j] = *tmp;
            tmp++;
            j++;
        }
        buffer[j] = '\0';
        buffer_len = str_len(buffer);
        if ((buffer_len > 1 && buffer[0] == '0') || (buffer_len == 0))
            return 0;

        part = str_to_uint(buffer);
        if (part > 255 || part < 0)
            return 0;
        tmp = dot_pos+1;
    }
    i = 0;
    while (*tmp){
        if (!is_digit(*tmp))
            return 0;
        buffer[i++] = *tmp++;
    }
    buffer[i] = '\0';
    buffer_len = str_len(buffer);
    if ((buffer_len > 1 && buffer[0] == '0') || (buffer_len == 0))
        return 0;
    part = str_to_uint(buffer);
    if (part < 0 || part > 255)
        return 0;
    return is_invalid == 1?0:1;           // valid
}
