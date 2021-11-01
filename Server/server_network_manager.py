
# ----------------------------------------------------------------
# Imports
# ----------------------------------------------------------------
from errors import *
from globals import *
from server_codes import *
import server

from server_request import Request
import client

import logging
import sys
import selectors
import socket
import struct
import uuid



# ----------------------------------------------------------------
# Globals
# ----------------------------------------------------------------

SERVER_MAXIMUM_CLIENTS = 100




# ----------------------------------------------------------------
# Classes
# ----------------------------------------------------------------


class ServerNetworkManager():

    def process_new_request(data):

        print(f"Received data of length: {len(data)}")
        # Crete new request and process it
        request  = Request(data)
        print("----->REQUEST ==" + str(request))
        response = server.Server.handle_request(request)
        return response


    def accept(sock, mask):
        conn, addr = sock.accept()  # Should be ready
        #print('accepted', conn, 'from', addr)
        conn.setblocking(False)
        ServerNetworkManager.selector.register(conn, selectors.EVENT_READ, ServerNetworkManager.read)


    def read(connection, mask):

        try:
            data = connection.recv(SERVER_PACKET_SIZE)

            if data:
                #print('Handling request - ', repr(data), 'from', connection)
                respone = ServerNetworkManager.process_new_request(data)
                connection.send(respone.packed_data())
                
            else:
                print('closing', connection)
                ServerNetworkManager.selector.unregister(connection)
                connection.close()

        except Exception as ex:
            # TODO - Handle errors!
            print("General Error:" + str(ex))
            

    def start(port_num):
        print(f"Start server on {port_num}")

        ServerNetworkManager.selector = selectors.DefaultSelector()

        server_socket = socket.socket()
        server_socket.bind(('localhost', port_num))
        server_socket.listen(SERVER_MAXIMUM_CLIENTS)
        server_socket.setblocking(False)
        ServerNetworkManager.selector.register(server_socket, selectors.EVENT_READ, ServerNetworkManager.accept)  

        while True:
            events = ServerNetworkManager.selector.select()
            for key, mask in events:
                callback = key.data
                callback(key.fileobj, mask)



