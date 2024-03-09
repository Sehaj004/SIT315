from socket import *  # Importing necessary socket functions

serverPort = 11500  # Port number on which the server will listen for incoming connections.

# Creating a UDP socket
serverSocket = socket(AF_INET, SOCK_DGRAM)

# Binding the socket to the specified port on all available network interfaces
serverSocket.bind(('', serverPort))
print("The Server is Listening")

while True:
    # Receiving initial message from client
    initial_message, clientAddress = serverSocket.recvfrom(2048)
    print(initial_message.decode())

    # Sending response asking for the name
    serverSocket.sendto("Hello, What’s your name?".encode(), clientAddress)

    # Receiving the name from client
    name, clientAddress = serverSocket.recvfrom(2048)
    print("Name received from client:", name.decode())

    # Constructing the final response with the received name
    final_response = "Hello " + name.decode() + ", Welcome to SIT202"

    # Sending the final response back to the client
    serverSocket.sendto(final_response.encode(), clientAddress)
