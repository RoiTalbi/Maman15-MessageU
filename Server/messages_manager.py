


# ----------------------------------------------------------------
# Imports
# ----------------------------------------------------------------

from message import *
from errors import  *




# ----------------------------------------------------------------
# Classes
# ----------------------------------------------------------------

class MessagesManager:

	def __init__(self):
		self._message_id_counter = 0

		""" Create dictionary of clients. where client is the key. and value is
		    list of messages sent to that client 
		"""
		self._clients_messages = {}


	# When a new client is registed, there is a need to add him to messages pool
	def add_client(self, client):
		self._clients_messages[client] = []

	# TODO - REMOVE!!!!!!!
	def debug_print(self):

		for c in self._clients_messages:
			print ("--------------" * 4)
			print (str(c))

			messages = self._clients_messages[c]

			for m in messages:
				print (str(m))

			print ("--------------" * 4)



	def add_message(self, sender_id, receiver, msg_type, content):

		new_message = Message(msg_type, sender_id, content)

		try:
			client_messages_list = self._clients_messages[receiver]
			client_messages_list.append(new_message)

			# Update and return the unquie message counter (message ID)
			self._message_id_counter += 1
			return self._message_id_counter

		except Exception as ex:
			raise ClientNotExsistException("Client not found in client messages pool")


	def get_client_pending_messages(self, client):

		try:
			client_messages_list = self._clients_messages[receiver]
			return client_messages_list

		except Exception as ex:
			raise ClientNotExsistException("Client not found in client messages pool") 


