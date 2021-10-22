# ----------------------------------------------------------------
# Imports
# ----------------------------------------------------------------
import struct



# ----------------------------------------------------------------
# Globals
# ----------------------------------------------------------------

REQUEST_STRUCT_FORMAT = '<16sBHI'
REQUEST_HEADERS_SIZE = 23


# ----------------------------------------------------------------
# Classes
# ----------------------------------------------------------------




"""
A simple Stracture to hold request data
"""
class Request():

    def init_request(self, client_id, version, code, payload_size, payload):
        self.client_id = client_id
        self.version = version
        self.code = code
        self.payload_size = payload_size
        self.payload = payload

    def __init__(self, request_raw_data):
        unpacked_data = struct.unpack(REQUEST_STRUCT_FORMAT, request_raw_data[:REQUEST_HEADERS_SIZE]);
        payload = request_raw_data[REQUEST_HEADERS_SIZE:]
        self.init_request(*unpacked_data + (payload,))

    
    def __str__(self):
        formatted_str = f"REQUEST:\n Cliend ID = {self.client_id} \n Version = {self.version}\n Code = {self.code}\n Payload size = {self.payload_size}\n Payload = {self.payload}\n"
        return formatted_str


