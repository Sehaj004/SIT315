from socket import *  # Importing necessary socket functions

serverName = "127.0.0.1"  # IP address of the server
serverPort = 11500  # Port number on which the server is listening

# Creating a UDP socket for the client
clientSocket = socket(AF_INET, SOCK_DGRAM)
message = "Hello"  # Message to be sent to the server

# Sending the message to the server at the specified address and port
clientSocket.sendto(message.encode(), (serverName, serverPort))

# Receiving the server's response and its address
ServerReply, serverAddress = clientSocket.recvfrom(2048)
print(ServerReply.decode())  # Printing the server's response

# Prompting the user to enter their name
name = input("Enter your name: ")

# Sending the name to the server
clientSocket.sendto(name.encode(), (serverName, serverPort))

# Receiving the server's final response and printing it
finalResponse, serverAddress = clientSocket.recvfrom(2048)
print(finalResponse.decode())

# Closing the client socket
clientSocket.close()
