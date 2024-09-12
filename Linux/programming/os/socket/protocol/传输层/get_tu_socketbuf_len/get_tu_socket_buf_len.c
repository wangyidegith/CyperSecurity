#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

void print_buffer_sizes(int sockfd, int level, int option, const char* protocol) {
    int size;
    socklen_t optlen = sizeof(size);

    if (getsockopt(sockfd, level, option, &size, &optlen) == -1) {
        perror("getsockopt");
        exit(EXIT_FAILURE);
    }

    printf("%s %s buffer size: %d bytes\n", protocol, (option == SO_SNDBUF) ? "send" : "receive", size);
}

int main() {
    int tcp_socket, udp_socket;

    // 创建TCP套接字
    if ((tcp_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("TCP socket");
        exit(EXIT_FAILURE);
    }

    // 创建UDP套接字
    if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("UDP socket");
        exit(EXIT_FAILURE);
    }

    // 获取TCP发送缓冲区大小
    print_buffer_sizes(tcp_socket, SOL_SOCKET, SO_SNDBUF, "TCP");

    // 获取TCP接收缓冲区大小
    print_buffer_sizes(tcp_socket, SOL_SOCKET, SO_RCVBUF, "TCP");

    // 获取UDP发送缓冲区大小
    print_buffer_sizes(udp_socket, SOL_SOCKET, SO_SNDBUF, "UDP");

    // 获取UDP接收缓冲区大小
    print_buffer_sizes(udp_socket, SOL_SOCKET, SO_RCVBUF, "UDP");

    // 关闭套接字
    close(tcp_socket);
    close(udp_socket);

    return 0;
}

/*
 * 在数台ubuntu20和ubuntu22机器上进行测试后，得出以下结果：
 * TCP SO_SNDBUF'len : 16KB
 * TCP SO_RCVBUF'len : 128KB
 * UDP SO_SNDBUF'len : 208KB
 * UDP SO_RCVBUF'len : 208KB
 *
*/
