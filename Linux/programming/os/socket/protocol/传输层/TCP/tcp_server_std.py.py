import socket
import sys

def start_echo_server(host, port):
    # 创建一个TCP套接字
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # 绑定服务器地址和端口
    server_socket.bind((host, port))
    
    # 开始监听连接，最大连接数为1
    server_socket.listen(1)
    print(f"服务器正在监听 {host}:{port} 上的连接...")
    
    try:
        while True:
            # 等待客户端连接
            client_socket, client_address = server_socket.accept()
            print(f"接受来自 {client_address} 的连接")
            
            # 从客户端接收数据并发送回去
            data = client_socket.recv(1024)
            while data:
                print(f"接收到数据: {data.decode('utf-8')}")
                client_socket.send(data)
                data = client_socket.recv(1024)
            
            # 关闭客户端连接
            print(f"关闭来自 {client_address} 的连接")
            client_socket.close()
            
    except KeyboardInterrupt:
        print("服务器关闭")
    finally:
        # 关闭服务器套接字
        server_socket.close()

if __name__ == "__main__":
    # 检查命令行参数是否足够
    if len(sys.argv) != 3:
        print("用法: python3 tcp_server.py <IP地址> <端口号>")
        sys.exit(1)
    
    # 从命令行参数获取服务器地址和端口号
    host = sys.argv[1]
    port = int(sys.argv[2])
    
    # 启动回显服务器
    start_echo_server(host, port)
