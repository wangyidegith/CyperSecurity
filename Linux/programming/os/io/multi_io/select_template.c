#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <errno.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// 结构体用于存储每个客户端的信息
struct Client {
	int fd;
	char* recv_buf;
};

int main() {
	int server_socket, client_socket, max_fd, activity, i, valread;
	int addrlen;
	struct sockaddr_in server_address, client_address;
	fd_set read_fds;
	char recv_buf[BUFFER_SIZE] = {0};
	struct Client clients[MAX_CLIENTS];
	for (int i = 0; i < MAX_CLIENTS; i++) {
		clients[i].fd = -1;
		clients[i].recv_buf = recv_buf;
	}

	// 创建TCP套接字
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	// 初始化服务器地址结构
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(8888);

	// 将套接字绑定到指定地址和端口
	if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	// 设置套接字为监听状态
	if (listen(server_socket, 5) == -1) {
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}

	addrlen = sizeof(client_address);

	printf("Server listening on port 8888...\n");

	while (1) {
		// 清空文件描述符集合
		FD_ZERO(&read_fds);

		// 添加主服务器套接字到集合
		FD_SET(server_socket, &read_fds);
		max_fd = server_socket;

		// 添加客户端套接字到集合
		for (i = 0; i < MAX_CLIENTS; ++i) {
			if (clients[i].fd == -1) {
				continue;
			}
			int fd = clients[i].fd;

			// 如果有效的套接字描述符，则添加到集合
			if (fd > 0) {
				FD_SET(fd, &read_fds);

				// 更新最大文件描述符值
				if (fd > max_fd) {
					max_fd = fd;
				}
			}
		}

		// 使用 select 监视文件描述符
		activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

		if ((activity < 0) && (errno != EINTR)) {
			perror("Select error");
		}

		// ... 其余的代码 ...
		// 如果主服务器套接字准备好接受新连接
		if (FD_ISSET(server_socket, &read_fds)) {
			if ((client_socket = accept(server_socket, (struct sockaddr*)&client_address, (socklen_t*)&addrlen)) == -1) {
				perror("Accept failed");
				exit(EXIT_FAILURE);
			}

			printf("New connection, socket fd is %d, IP is: %s, port is: %d\n", client_socket,
					inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

			// 将新连接添加到客户端数组
			for (i = 0; i < MAX_CLIENTS; ++i) {
				if (clients[i].fd == 0) {
					clients[i].fd = client_socket;
					break;
				}
			}
		}

		// 处理客户端请求
		for (i = 0; i < MAX_CLIENTS; ++i) {
			int fd = clients[i].fd;

			if (FD_ISSET(fd, &read_fds)) {
				if ((valread = read(fd, clients[i].recv_buf, BUFFER_SIZE)) == 0) {
					// 客户端断开连接
					printf("Client disconnected, socket fd is %d\n", fd);
					close(fd);
					clients[i].fd = 0;
				} else {
					// 处理客户端的数据，这里简单地将数据原样发送回去
					clients[i].recv_buf[valread] = '\0';
					printf("Received from client (fd %d): %s", fd, clients[i].recv_buf);
					write(fd, clients[i].recv_buf, strlen(clients[i].recv_buf));
				}
			}
		}
	}

	return 0;
}
