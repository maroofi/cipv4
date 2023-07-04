import ipaddress
import sys

if __name__ == "__main__":
    f = open("example.db")
    inp = ipaddress.ip_address(sys.argv[1])
    for line in f:
        line = line.strip()
        ip = ipaddress.ip_network(line)
        if inp in ip:
            print(line)
        #
    #
    f.close()
# end
