import socket

class TCPClient:
    def __init__(self, server_host, server_port):
        self.server_host = server_host
        self.server_port = server_port
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_address = (self.server_host, self.server_port)
        self.client_socket.connect(self.server_address)

    def send_message(self, message):
        self.client_socket.sendall(message.encode())
        data = self.client_socket.recv(1024)
        print("Received:", data.decode())

    def close(self):
        self.client_socket.close()

if __name__ == "__main__":
    client = TCPClient('127.0.0.1', 12346)
    try:
        while True:
            message = input("Enter a message to send: ")
            if message.lower() == 'exit':
                break
            client.send_message(message)
    except KeyboardInterrupt:
        pass
    finally:
        client.close()