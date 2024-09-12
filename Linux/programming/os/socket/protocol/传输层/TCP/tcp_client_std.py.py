import socket
import sys

def start_tcp_client(server_host, server_port):
    # 创建TCP套接字
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    try:
        # 连接到服务器
        client_socket.connect((server_host, server_port))
        print(f"已连接到服务器 {server_host}:{server_port}")
        
        while True:
            # 用户输入要发送的数据
            message = input("请输入要发送的数据 (输入 'exit' 退出): ")
            
            if message.lower() == 'exit':
                break
            
            # 发送数据到服务器
            client_socket.sendall(message.encode('utf-8'))
            
            # 接收服务器的回显数据
            data = client_socket.recv(1024)
            print(f"从服务器接收到的数据: {data.decode('utf-8')}")
    
    finally:
        # 关闭客户端套接字
        client_socket.close()
        print("客户端关闭")

if __name__ == "__main__":
    # 检查命令行参数是否足够
    if len(sys.argv) != 3:
        print("用法: python3 tcp_client.py <目地IP地址> <目地端口号>")
        sys.exit(1)
    
    # 从命令行参数获取服务器地址和端口号
    server_host = sys.argv[1]
    server_port = int(sys.argv[2])
    
    # 启动TCP客户端
    start_tcp_client(server_host, server_port)
