# distutils: language = c++

from cython.operator cimport dereference as deref
from libcpp.string cimport string
from libcpp.memory cimport unique_ptr
from libcpp.utility cimport move
from Irc cimport connection 

cdef class Message:
    cdef unique_ptr[message] c_message

    @staticmethod
    cdef Message Create(unique_ptr[message] ptr):
        cdef Message result = Message()
        result.c_message = move(ptr)
        return result

    def Source(self):
        return deref(self.c_message).get_source()

    def Command(self):
        return deref(self.c_message).get_command()

    def Parameters(self):
        return deref(self.c_message).get_parameters()

    def Cmd(self):
        return deref(self.c_message).get_cmd()

    def String(self):
        return deref(self.c_message).toString()

    def Channel(self):
        return deref(self.c_message).get_channel()


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

    def nextMessage(self):
        newMessage = Message.Create(self.c_connection.next_msg())
        return newMessage

    def Join(self, chan):
        self.c_connection.join(chan)

    def Pong(self, key):
        self.c_connection.pong(key)
        
