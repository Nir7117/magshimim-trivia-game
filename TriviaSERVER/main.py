import socket

def main():
    print("Hello Welcome to the client side!\n")
    port = 8826
    if port < 1024 or port > 65535:
        print("Error Port is not in the valid range!")
        return -1
    else:
        client_program(port)






def client_program(port):
    host = socket.gethostname()  # as both code is running on same pc

    client_socket = socket.socket()  # instantiate
    client_socket.connect((host, port))  # connect to the server

    while True:
        data = client_socket.recv(1024).decode()  # receive response

        print('Received from server: ' + data)  # show in terminal
        if data == "Hello":
            message = 'Hello'
        else:
            message = 'Not hello'

        client_socket.send(message.encode())  # send message


    client_socket.close()  # close the connection





main()
