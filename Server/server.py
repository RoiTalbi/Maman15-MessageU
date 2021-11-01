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
from message import *
from messages_manager import MessagesManager


# ----------------------------------------------------------------
# Globals
# ----------------------------------------------------------------


CLIENT_MESSAGE_HEADERS_SIZE = 21



REQUEST_REGISTER_PAYLOAD_FORMAT = '<255s160s'
REQUEST_SEND_MESSAGE_HEADERS_FORMAT = '<16sBI'



RESPONSE_CLIENTS_LIST_PAYLOAD_FORMAT = '<16s255s'
RESPONSE_CLIENT_PUBLIC_KEY_FORMAT = "<16s160s"
RESPONSE_MESSAGE_SENT_FORMAT = "<16sI"
RESPONSE_PENDING_MESSAGE_FORMAT = "<16sIBI"


# ----------------------------------------------------------------
# Classes
# ----------------------------------------------------------------

class Server():

    def _init_server():
        Server._clients = []
        Server._general_error_response = Response(SERVER_VERSION, SERVER_ERROR_CODE, 0, b'')
        Server._messages_manager = MessagesManager()

    def start_server(port_num):
        Server._init_server();
        ServerNetworkManager.start(port_num)

    def handle_request(request):

        try:
            # Invoke matching handler function by using request code
            handler_function = REQUEST_HANDLERS[request.code]
            return handler_function(request)

        except ServerException as ex:
            print("Server Error!!" + str(ex))

        except Exception as ex:
            # TODO - Handle errors!!!! 
            print("General Error:" + str(ex))




    def _register_new_client(name, public_key):

        # slice name until null char
        name_str = name[:name.index(b'\x00')].decode('utf-8')

        # check if same client already exsist 
        for client in Server._clients:
            if client.get_name() == name_str:
                raise ClientExsistsException("Client Already Exsists")

        # Create new client, add it to clients list and update messages manager
        new_client = Client(uuid.uuid1(), name_str, public_key, time.time())
        Server._clients.append(new_client)
        Server._messages_manager.add_client(new_client)

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

        return None



    def _request_get_clients_list(request):

        other_clients = Server._clients[:]

        # find requesting client by it's id and remove it from other clients list
        client = Server._find_client_by_id(other_clients, request.client_id)
        if client:
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

        # find client requsted public key
        requested_client_id = uuid.UUID(bytes=request.payload)
        client = Server._find_client_by_id(Server._clients, requested_client_id)

        print("Found client matching this ID ===> " + str(client))

        response_payload = struct.pack(RESPONSE_CLIENT_PUBLIC_KEY_FORMAT, client.get_id().bytes, client.get_public_key())
        response = Response(SERVER_VERSION, RESPONSE_CODE_GET_CLIENT_PUBLIC_KEY, len(response_payload) , response_payload)

        print("RESPONSE===>" + str(response))

        return response


    def _request_send_message(request):

        content = ''
        # prase request payload (message headers)
        (dest_client_id, msg_type, content_size) = struct.unpack(REQUEST_SEND_MESSAGE_HEADERS_FORMAT, \
         request.payload[:CLIENT_MESSAGE_HEADERS_SIZE]) 

        #print (f"DEST ID= {dest_client_id}  | TYPE= {msg_type} | SIZE= {content_size}")

        if content_size > 0:
            content = request.payload[CLIENT_MESSAGE_HEADERS_SIZE:]

        #print("Content===> " + content)

        dest_client = Server._find_client_by_id(Server._clients, uuid.UUID(bytes=dest_client_id))

        if not dest_client:
            return Server._general_error_response

        message_id = Server._messages_manager.add_message(request.client_id, dest_client, \
         msg_type, content)

        # TODO - REMOVE !! DEBUG !
        Server._messages_manager.debug_print()

        # build respone
        response_payload = struct.pack(RESPONSE_MESSAGE_SENT_FORMAT, dest_client_id, message_id)        
        response = Response(SERVER_VERSION, RESPONSE_CODE_SEND_MESSAGE, len(response_payload) , response_payload)
        
        return response
        

    def _request_get_pending_messages(request):
        
        # find client's pending messages
        client = Server._find_client_by_id(Server._clients, request.client_id)
        clients_messages = Server._messages_manager.get_client_pending_messages(client)

        # Pack each message headers with it's content and add it to response payload
        response_payload = b''
        for msg in clients_messages:
            response_payload += (struct.pack(RESPONSE_PENDING_MESSAGE_FORMAT, msg.get_sender_id().bytes,
             msg.get_message_id(), msg.get_type(), len(msg.get_content())) + msg.get_content())


        # Release all messages of that client. and send back them as a response 
        Server._messages_manager.release_pending_messages(client)

        Server._messages_manager.debug_print()

        response = Response(SERVER_VERSION, RESPONSE_CODE_GET_PENDING_MESSAGES, len(response_payload) , response_payload)
        return response





# ----------------------------------------------------------------
# Request Handlers Map
# ----------------------------------------------------------------

REQUEST_HANDLERS = {
OPCODE_REGISTER : Server._request_register,                           \
OPCODE_GET_CLIENTS_LIST : Server._request_get_clients_list,           \
OPCODE_GET_CLIENT_PUBLIC_KEY : Server._request_get_client_public_key, \
OPCODE_SEND_MESSAGE : Server._request_send_message,                   \
OPCODE_GET_PENDING_MESSAGES : Server._request_get_pending_messages  }










