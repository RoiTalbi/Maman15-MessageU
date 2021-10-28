
# ----------------------------------------------------------------
# Message Type
# ----------------------------------------------------------------

MESSAGE_TYPE_GET_SYMMETRIC_KEY = 1,
MESSAGE_TYPE_SEND_SYMMETRIC_KEY = 2,
MESSAGE_TYPE_REGULAR_MESSAGE = 3	




# ----------------------------------------------------------------
# Class
# ----------------------------------------------------------------

class Message:

	def __init__(self, msg_type, sender_id, content = ""):
		self._type = msg_type
		self._sender_id = sender_id
		self._content = content


	def __str__(self):
		return f"Message of type {self._type}|  From: {self._sender_id}  | Content= {self._content}"


	def get_sender_id(self):
		return self._sender_id

	def get_type(self):
		return self._type

	def get_content(self):
		return self._content

