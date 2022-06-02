import socket                                                                       #library for socket
import threading        

nickname = input("Please input your name : ")                                       #input nickname

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)                          #initialize socket 
client.connect(('127.0.0.1',65225))                                                 #connect the client to server with spesific ip and port

def receive():                                                                      #listening to server and sending nickname
    while True:
        try:
            message = client.recv(1024).decode('ascii')
            if message == 'NICK':
                client.send(nickname.encode('ascii'))
            else:
                print(message)
        except:                                                                     #close connection when error
            print("You have an error!")
            client.close()
            break

def write():                                                                        #sending message to server
    while True:
        message = f'{nickname}: {input("")}'
        client.send(message.encode('ascii'))

#starting thread for listening and writing
receive_thread = threading.Thread(target=receive)                                  
receive_thread.start()

write_thread = threading.Thread(target=write)
write_thread.start()

