# cipv4

# cipv4 
IPv4 library in C

Almost has all the necessary functions implemented in Python3 IPAddress library.

Only supports IPv4 now.

## How to use

All you need is to copy files in `src` and `include` directory into your project.

## Example

Check the `test` directory for more examples.

```c
int main(int argc, char** argv){
    char buffer[20];        // allocate a temporary buffer
    cipv4_ctx * ctx = NULL; 
    ctx = cipv4_parse_ip("10.20.30.40/24");
    if (!ctx){
        fprintf(stdout, "Can not parse the IP address\n");
        return 1;
    }
    if (ctx->error != 0){
        fprintf(stdout, "%s\n", ctx->err_msg);
        return 2;
    }
    fprintf(stdout, "IP address: %s\n", ctx->raw);
    fprintf(stdout, "Network prefix is: %d\n", ctx->network_prefix);
    fprintf(stdout, "Integer representation of the IP: %u\n", ctx->addr);
    // next line prints 255.255.255.0
    fprintf(stdout, "Subnet mask is: %s\n", cipv4_get_subnet_mask(ctx, buffer));
    // next line prints 169090560
    fprintf(stdout, "Start IP address of the range: %u\n", ctx->addr_start);
    // next line prints 169090815
    fprintf(stdout, "End IP address of the range: %u\n", ctx->addr_start);
    // this will print: 10.20.30.255
    fprintf(stdout, "End IP address in string: %s\n", cipv4_uint_to_str(ctx->addr_end, buffer));
    // this will print: 256
    fprintf(stdout, "The number of IPs in range: %u\n", cipv4_count_ips_in_range(ctx));
    // next line prints: 1
    fprintf(stdout, "Is IP address 10.20.30.0 in range? %d\n", cipv4_is_address_in(ctx, "10.20.30.0"));
    // next line prints -1 as the IP is invalid
    fprintf(stdout, "Is IP address 10.20.30.256 in range? %d\n", cipv4_is_address_in(ctx, "10.20.30.256"));
    // avoid memory leak, free the context
    cipv4_free(ctx);
}
```
## Compile
```bash
# compile the library
make

# run tests
make test
```

## Doc

Use doxygen to generate the documentation
