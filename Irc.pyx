# distutils: language = c++
from libcpp.string cimport string
from Irc cimport connection 

cdef class Connection:
    cdef connection *c_connection

    def __init__(self, string host, int port):
        self.c_connection = new connection(host,port)

    def Send(self, string something):
        self.c_connection.send_str(something)

    def Get(self):
        return self.c_connection.get_str()

    def Handshake(self, string nick):
        self.c_connection.handshake(nick)
