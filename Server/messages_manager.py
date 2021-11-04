# ----------------------------------------------------------------
# Imports
# ----------------------------------------------------------------

from message import *
from errors import  *






"""
	----------------------------------------------------------------------
	MessageManager module handles all messages in the server side.
	It contains a dict in which for each client (receiver) is the entry 
	and a list of Message (object) is the value. 

	messages can be added and removed thourgh it's API
	----------------------------------------------------------------------
"""
class MessagesManager:

	def __init__(self):
		self._message_id_counter = 0

		""" Create dictionary of clients. where client is the key. and value is
		    list of messages sent to that client 
		"""
		self._clients_messages = {}


	"""
	----------------------------------------------------------------------
	When a new client is registed, there is a need to add him to messages pool
	----------------------------------------------------------------------
	"""
	def add_client(self, client):
		self._clients_messages[client] = []


	"""
	----------------------------------------------------------------------
	Get all clients messages goten so far. as a list of Message objcet 
	----------------------------------------------------------------------
	"""
	def _get_client_messages_list(self, client):
		try:
			client_messages_list = self._clients_messages[client]
			return client_messages_list

		except Exception as ex:
			raise ClientNotExsistException("Client not found")


	"""
	----------------------------------------------------------------------
	Add message received from sender client to receiver client.
	----------------------------------------------------------------------
	"""
	def add_message(self, sender_id, receiver, msg_type, content):

		#  update message id counter. Create message and add it to the matching clients list
		self._message_id_counter += 1
		new_message = Message(msg_type, sender_id, self._message_id_counter, content)

		client_messages_list = self._get_client_messages_list(receiver)
		client_messages_list.append(new_message)

		# Update and return the unquie message counter (message ID)	
		return self._message_id_counter


	"""
	----------------------------------------------------------------------
	Get messages list for given client
	----------------------------------------------------------------------
	"""
	def get_client_pending_messages(self, client):
		return self._get_client_messages_list(client)


	"""
	----------------------------------------------------------------------
	Delete all pending messages of given client
	----------------------------------------------------------------------
	"""
	def release_pending_messages(self, client):
		try:
			del self._clients_messages[client][:]

		except Exception as ex:
			raise ClientNotExsistException("Client not found")