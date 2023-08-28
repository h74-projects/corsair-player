import socket
import threading
import queue
import requests
from app import chat_gpt
from flask import Flask, request, jsonify

app = Flask(__name__)

class TCPServer:

    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_address = (self.host, self.port)
        self.server_socket.bind(self.server_address)
        self.server_socket.listen(5)  # ??
        print(f"TCP server is listening on {self.host}:{self.port}")
        self.client_queue = queue.Queue()
        self.MAX_CLIENTS = 10
    

    def handle_client(self, client_socket, client_address):
        while True:
            try:
                data = client_socket.recv(1024)
                if not data:
                    break
                decoded_data = data.decode().strip()
                print(f"Received {len(data)} bytes from {client_address[0]}:{client_address[1]}")
                print("Message:", decoded_data)

                # type- chat_gpt
                if decoded_data.startswith("1:"):
                    song_name = decoded_data[2:]  
                    #http to flask server
                    
                    
                else:
                    self.send_message("Invalid message type", client_socket)
            except Exception as e:
                print(f"Error handling client {client_address}: {e}")
                break
        client_socket.close()

    def send_message(self, message, client_socket):
        client_socket.sendall(message.encode())

    def run(self):
        try:
            while True:
                if self.client_queue.qsize() < self.MAX_CLIENTS:
                    client_socket, client_address = self.server_socket.accept()
                    if client_address not in self.client_queue.queue:
                        self.client_queue.put(client_address)
                        client_thread = threading.Thread(target=self.handle_client, args=(client_socket, client_address))
                        client_thread.start()
        except KeyboardInterrupt:
            print("Server shutting down...")
        finally:
            self.server_socket.close()

if __name__ == "__main__":
    server = TCPServer('127.0.0.1', 12346)
    try:
        server.run()
    except KeyboardInterrupt:
        print("Server shutting down...")

