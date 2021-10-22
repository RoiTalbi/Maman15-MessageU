# ----------------------------------------------------------------
# Imports
# ----------------------------------------------------------------
import struct 
import logging
import sys
import uuid
import time

from server_network_manager import ServerNetworkManager
from server_response import Response
from client import Client
from errors import *
from globals import *
from server_codes import *


# ----------------------------------------------------------------
# Globals
# ----------------------------------------------------------------

REQUEST_REGISTER_PAYLOAD_FORMAT = '<255s160s'


# ----------------------------------------------------------------
# Classes
# ----------------------------------------------------------------

class Server():

    def _init_server():
        Server._clients = []
        Server._general_error_response = Response(SERVER_VERSION, SERVER_ERROR_CODE, 0, b'')

    def start_server(port_num):
        Server._init_server();
        ServerNetworkManager.start(port_num)

    def handle_request(request):

        # Invoke matching handler function by using request code
        handler_function = REQUEST_HANDLERS[request.code]
        return handler_function(request)


    def _register_new_client(name, public_key):

        # slice name until null char
        name_str = name[:name.index(b'\x00')].decode('utf-8')

        # check if same client already exsist 
        for client in Server._clients:
            if client.get_name() == name_str:
                raise ClientExsistsException("Client Already Exsists")

        # Create new client
        # TODO - make sure client id unquie!!!!!!!!!!!!!!!
        new_client = Client(uuid.uuid4(), name_str, public_key, time.time())
        Server._clients.append(new_client)

        return new_client



    # -------------------------- Request Handler functions --------------------------

    def _request_register(request):
        print("REGISTER REQUEST!");

        client_data = struct.unpack(REQUEST_REGISTER_PAYLOAD_FORMAT, request.payload)

        try:

            # Register new client and return maching response 
            new_client = Server._register_new_client(*client_data)

            Server.print_clients()

            response = Response(SERVER_VERSION, RESPONSE_CODE_REGISTER, CLIENT_ID_SIZE, new_client.get_id().bytes)
            print(str(response))

            Server.print_clients()
            return response


        except ServerException as ex:
            # TODO - Handle errors correctly (?) !!!!!!!!!!
            print(str(ex))
            return Server._general_error_response 


    def print_clients():
        for c in Server._clients:
            print (str(c))

    def _request_get_clients_list(request):
        pass

    def _request_get_client_public_key(request):
        pass

    def _request_send_message(request):
        pass

    def _request_get_pending_messages(request):
        pass





# ----------------------------------------------------------------
# Request Handlers Map
# ----------------------------------------------------------------

REQUEST_HANDLERS = {
OPCODE_REGISTER : Server._request_register,                           \
OPCODE_GET_CLIENTS_LIST : Server._request_get_clients_list,           \
OPCODE_GET_CLIENT_PUBLIC_KEY : Server._request_get_client_public_key, \
OPCODE_SEND_MESSAGE : Server._request_send_message,                   \
OPCODE_GET_PENDING_MESSAGES : Server._request_get_pending_messages  }










