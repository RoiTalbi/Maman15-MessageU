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


RESPONSE_CLIENTS_LIST_PAYLOAD_FORMAT = '<16s255s'
RESPONSE_CLIENT_PUBLIC_KEY_FORMAT = "<16s160s"



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



    def _find_client_by_id(clients_list, client_id):

        for client in clients_list:
            if client_id == client.get_id():
                return client



    def _request_get_clients_list(request):

        other_clients = Server._clients[:]

        # find requesting client by it's id and remove it from other clients list
        client = Server._find_client_by_id(other_clients, request.client_id)
        other_clients.remove(client)

        # Assamble response payload (other clinets list)
        packed_clients_list = b''

        for client in other_clients:
            client_terminated_name = client.get_name() + '\x00' * (CLIENT_NAME_SIZE - len(client.get_name()))
            client_terminated_name = client_terminated_name.encode('utf-8')

            packed_clients_list += struct.pack(RESPONSE_CLIENTS_LIST_PAYLOAD_FORMAT, 
             client.get_id().bytes, client_terminated_name)

        # Create response and send it
        response = Response(SERVER_VERSION, RESPONSE_CODE_GET_CLIENTS_LIST, len(packed_clients_list) , packed_clients_list)
        return response



    def _request_get_client_public_key(request):

        print("REQUEST===>" + str(request))

        # find client requsted public key
        requested_client_id = uuid.UUID(bytes=request.payload)
        client = Server._find_client_by_id(Server._clients, requested_client_id)

        print("Found client matching this ID ===> " + str(client))

        response_payload = struct.pack(RESPONSE_CLIENT_PUBLIC_KEY_FORMAT, client.get_id().bytes, client.get_public_key())
        response = Response(SERVER_VERSION, RESPONSE_CODE_GET_CLIENT_PUBLIC_KEY, len(response_payload) , response_payload)


        print("RESPONSE===>" + str(response))

        return response


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










