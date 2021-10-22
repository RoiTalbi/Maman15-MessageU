
# ----------------------------------------------------------------
# Imports
# ----------------------------------------------------------------
import logging
import sys

from errors import *

# ----------------------------------------------------------------
# Globals
# ----------------------------------------------------------------

CLIENT_NAME_MAX_SIZE = 255


# ----------------------------------------------------------------
# Classes
# ----------------------------------------------------------------

class Client():

    def __init__(self, id, name, public_key, last_seen):
        self._id = id
        self._public_key = public_key
        self._last_seen = last_seen
        self._name = name

    def __eq__(self, other): 
        if not isinstance(other, Client):
            return False

        if other.get_id() == self._id or other.get_name() == self._name:
            return True
        else:
            return False


    def get_id(self):
        return self._id

    def get_name(self):
        return self._name

    def get_public_key(self):
        return self._public_key

    #TODO - REMOVE !!!!!!!!!!!!!!!
    def __str__(self):
        return f" ID= {self._id} | Name= {self._name}| \n\n";



