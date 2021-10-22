# ----------------------------------------------------------------
# Error codes
# ----------------------------------------------------------------
SERVER_CONFIG_ERROR = 1



# ----------------------------------------------------------------
# Module Defined Exception 
# ----------------------------------------------------------------

class ServerException(Exception):
	pass



class ClientExsistsException(ServerException):
	pass