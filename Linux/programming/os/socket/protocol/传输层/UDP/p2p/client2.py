#!/usr/bin/python3
#coding=utf-8

import random
import socket
import string
import time

if __name__ == "__main__":
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    serv = ("101.133.228.115", 12345)
    print("server =>", serv)

    # 1/2.send message to server, server can get our export ip:port
    sock.sendto("REGISTER".encode("utf-8"), serv)
    print("1/2.send REGISTER message to server")

    # 3/4.receive the export address of the peer from the server
    data, conn = sock.recvfrom(1024)
    array = data.decode("utf-8").split(":")
    addr = (array[0], int(array[1]))
    print("3/4.receive the export address of the peer, %s" % str(addr))

    # 5/6.send KNOCK message to export address of peer
    wait = random.randint(2, 5)
    print("5/6.send KNOCK message to export address of peer (wait %d s)" % wait)
    # in order to stagger the two clients
    # so that the router can better create the mapping
    time.sleep(wait)
    sock.sendto("KNOCK".encode("utf-8"), addr)

    name = "".join(random.sample(string.ascii_letters, 8))
    print("my name is %s, start to communicate" % name)
    # 7.communicate each other
    count = 0
    sock.settimeout(30)
    while True:
        try:
            data, conn = sock.recvfrom(1024)
            print("%s => %s" % (str(conn), data.decode("utf-8")))
        except Exception as e:
            print(e)
        msg = "%s: %d" % (name, count)
        sock.sendto(msg.encode("utf-8"), conn)
        count += 1
        time.sleep(1)
    # end while()

    sock.close()
# end main()
