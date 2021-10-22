# ----------------------------------------------------------------
# Imports
# ----------------------------------------------------------------
import struct



# ----------------------------------------------------------------
# Globals
# ----------------------------------------------------------------

RESPONSE_HEADERS_STRUCT_FORMAT = '<BHI'
RESPONSE_HEADERS_SIZE = 7



# ----------------------------------------------------------------
# Classes
# ----------------------------------------------------------------




"""
A simple Stracture to hold response data
"""
class Response():
	
	def __init__(self, version, code, payload_size, payload):
		self.version = version
		self.code = code
		self.payload_size = payload_size
		self.payload = payload


	def packed_data(self):
		response_hedaers = struct.pack(RESPONSE_HEADERS_STRUCT_FORMAT, self.version, self.code, self.payload_size)
		return (response_hedaers + self.payload)


	def __str__(self):
		formatted_str = f"RESPONSE:\n Version = {self.version}\n Code = {self.code}\n Payload size = {self.payload_size}\n Payload = {self.payload}\n"
		return formatted_str