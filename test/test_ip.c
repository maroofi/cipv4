#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <cipv4.h>

void test_if_ip_valid(){
    assert(cipv4_is_ip_valid("123.36.58.203") == 1);
    assert(cipv4_is_ip_valid("123.36.58.0203") == 0);
    assert(cipv4_is_ip_valid("123.36.58.0") == 1);
    assert(cipv4_is_ip_valid("....") == 0);
    assert(cipv4_is_ip_valid("123.36.58.") == 0);
    assert(cipv4_is_ip_valid("123.36..203") == 0);
    assert(cipv4_is_ip_valid("123.36.58.203") == 1);
    assert(cipv4_is_ip_valid("023.36.58.203") == 0);
    assert(cipv4_is_ip_valid("1230.36.58.203") == 0);
    assert(cipv4_is_ip_valid("123.036.58.203") == 0);
    assert(cipv4_is_ip_valid("123.36.058.03") == 0);
    assert(cipv4_is_ip_valid("0.36.58.203") == 1);
    assert(cipv4_is_ip_valid("255.255.255.255") == 1);
    assert(cipv4_is_ip_valid("255.255.255.255/32") == 0);
    assert(cipv4_is_ip_valid("0.0.0.0") == 1);
    assert(cipv4_is_ip_valid("a.b.c.d") == 0);
    assert(cipv4_is_ip_valid("1.2.3.x") == 0);
    assert(cipv4_is_ip_valid("3.2.1.a") == 0);
    assert(cipv4_is_ip_valid("a.1.2.34") == 0);
    assert(cipv4_is_ip_valid("0") == 0);
    assert(cipv4_is_ip_valid("1.2.333.1") == 0);
    assert(cipv4_is_ip_valid(".") == 0);
}

void test_ip_to_int(){
    assert(cipv4_str_to_uint("0.1.2.3") == 66051);
    assert(cipv4_str_to_uint("0.0.0.0") == 0);
    assert(cipv4_str_to_uint("255.255.255.255") == 4294967295);
    assert(cipv4_str_to_uint("1.1.1.1") == 16843009);
    assert(cipv4_str_to_uint("1.255.1.255") ==33489407 );
    assert(cipv4_str_to_uint("0.1.0.1") == 65537);
    assert(cipv4_str_to_uint("1.0.1.0") == 16777472);
    assert(cipv4_str_to_uint("127.127.127.127") == 2139062143);
    assert(cipv4_str_to_uint("7.7.7.7") == 117901063);
    assert(cipv4_str_to_uint("111.111.111.111") == 1869573999);
}

void test_int_to_ip(){
    char buffer[20] = {0};
    assert(strcmp(cipv4_uint_to_str(1869573999, buffer), "111.111.111.111") == 0);
    assert(strcmp(cipv4_uint_to_str(66051, buffer), "0.1.2.3") == 0);
    assert(strcmp(cipv4_uint_to_str(0, buffer), "0.0.0.0") == 0);
    assert(strcmp(cipv4_uint_to_str(16843009, buffer), "1.1.1.1") == 0);
    assert(strcmp(cipv4_uint_to_str(4294967295, buffer), "255.255.255.255") == 0);
    assert(strcmp(cipv4_uint_to_str(65537, buffer), "0.1.0.1") == 0);
}

void test_general(){
    char buffer[20];
    cipv4_ctx * ctx = NULL;
    ctx = cipv4_parse_ip("10.20.30.40/24");
    assert(ctx != NULL);
    assert(ctx->error == 0);
    assert(ctx->network_prefix == 24);
    assert(ctx->addr == 169090600);
    assert(strcmp(cipv4_get_subnet_mask(ctx, buffer), "255.255.255.0") == 0);
    assert(ctx->addr_start == 169090560);
    assert(ctx->addr_end == 169090815);
    assert(strcmp(cipv4_uint_to_str(ctx->addr_end, buffer), "10.20.30.255") == 0);
    assert(cipv4_count_ips_in_range(ctx) == 256);
    assert(cipv4_is_address_in(ctx, "10.20.30.0") == 1);
    assert(cipv4_is_address_in(ctx, "10.20.30.255") == 1);
    assert(cipv4_is_address_in(ctx, "10.20.30.256") == -1); // invalid IP
    assert(cipv4_is_address_in(ctx, "10.20.31.25") == 0);
    assert(cipv4_is_reserved(cipv4_parse_ip("240.0.0.5")) == 1);
    assert(cipv4_is_reserved_from_string("241.0.0.5") == 1);
    cipv4_free(ctx);
}

int main(){
    test_if_ip_valid();
    test_ip_to_int();
    test_int_to_ip();
    test_general();
    return 0;
}
