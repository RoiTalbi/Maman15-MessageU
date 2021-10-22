
# ----------------------------------------------------------------
# Imports
# ----------------------------------------------------------------
import logging
import sys

from errors import *

# ----------------------------------------------------------------
# Globals
# ----------------------------------------------------------------



# ----------------------------------------------------------------
# Classes
# ----------------------------------------------------------------


class MessageEntry():
	def __init__(self, id, client_dest, client_src, type, content):
		self._id = id
		self._client_dest = client_dest
		self._client_src = client_src
		self._type = type
		self._content = content


	def get_dest(self):
		return self._client_dest

	def get_src(self):
		return self._client_src

	def get_type(self):
		return self._type

	def get_content(self):
		return self._content




