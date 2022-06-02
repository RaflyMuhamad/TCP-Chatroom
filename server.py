import threading 
import socket

#define host and port
host = '127.0.0.1' #localhost
port = 65225
#starting server
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((host, port))                                                   #bind socket to host and port
server.listen()                                                             #make server ready to listen for connection

#place for clients and nicknames
clients = []
nicknames = []

def broadcast(message):                                                     #method for sending the chat via server
    for client in clients: 
        client.send(message)

def handle(client):                             
    while True:
        try:                                    
            message = client.recv(1024)
            broadcast(message)
        except:                                                              #handling client error
                index = clients.index(client)
                clients.remove(client)
                client.close()                                               #close client connection
                nickname = nicknames[index]
                broadcast(f'{nickname} left the room'.encode('ascii'))
                nicknames.remove(nickname)                                   #remove client nickname
                break

def receive():                                                               #listening and receiving function
    while True:
        client, address = server.accept()                                    #accept connection
        print(f"Success Connect with {str(address)}")

        client.send('NICK'.encode('ascii'))                                  #request and store nickname
        nickname = client.recv(1024).decode('ascii')
        nicknames.append(nickname)
        clients.append(client)

        print(f'Nickname of the client is {nickname}!')                      #print and broadcast nickname
        broadcast(f'{nickname} joined the chat!'.encode('ascii'))
        client.send('Connected to the server!'.encode('ascii'))

        thread = threading.Thread(target=handle, args=(client,))             #start handling thread for client
        thread.start()

print("Server is listening...")
receive()

