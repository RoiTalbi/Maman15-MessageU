
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



"""
    ----------------------------------------------------------------------
    ServerNetworkManager handles all the networking with clients for the Sever.
    it uses socket API to send packets and selectors API to be able to connect 
    with multiple clients at once. 

    It parses each packet with struct and assamble Request object
    then process each request using Server's handle request function.
    it receives Response object as an output. which is later sent to the client
    ----------------------------------------------------------------------
"""
class ServerNetworkManager():


    """
    ----------------------------------------------------------------------
    Request: Process new request received from connection using Server's
    handle request function. return the response to be sent to client
    ----------------------------------------------------------------------
    """
    def process_new_request(data):
        # Crete new request and process it
        request  = Request(data)
        response = server.Server.handle_request(request)
        return response


    """
    ----------------------------------------------------------------------
    Accept connection
    ----------------------------------------------------------------------
    """
    def accept(sock, mask):
        conn, addr = sock.accept()  
        conn.setblocking(False)
        ServerNetworkManager.selector.register(conn, selectors.EVENT_READ, ServerNetworkManager.read)


    """
    ----------------------------------------------------------------------
    Read data from connection and process it 
    ----------------------------------------------------------------------
    """
    def read(connection, mask):

        try:
            data = connection.recv(SERVER_PACKET_SIZE)

            if data:
                respone = ServerNetworkManager.process_new_request(data)
                connection.send(respone.packed_data())
                
            else:
                ServerNetworkManager.selector.unregister(connection)
                connection.close()

        except Exception as ex:
            print("|ERROR|      \t" + str(ex))
            

    """
    ----------------------------------------------------------------------
    Start listening for requests on given port number
    ----------------------------------------------------------------------
    """
    def start(port_num):
        print(f"Start server on {port_num}")
        ServerNetworkManager.selector = selectors.DefaultSelector()

        server_socket = socket.socket()
        server_socket.bind(('0.0.0.0', port_num))
        server_socket.listen(SERVER_MAXIMUM_CLIENTS)
        server_socket.setblocking(False)
        ServerNetworkManager.selector.register(server_socket, selectors.EVENT_READ, ServerNetworkManager.accept)  

        while True:
            try:
                events = ServerNetworkManager.selector.select()
                for key, mask in events:
                    callback = key.data
                    callback(key.fileobj, mask)

            except Exception as ex:
                print("|ERROR|      \t" + str(ex))




