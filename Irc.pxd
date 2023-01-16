from libcpp.string cimport string
from libcpp.memory cimport unique_ptr 

cdef extern from "irc.cpp":
    pass

cdef extern from "irc.h" namespace "irc":
    cdef cppclass message:
        string get_source()
        string get_command()
        string get_parameters()
        int get_cmd()
        string get_channel()
        string toString()
    
    cdef cppclass connection:    
        connection(string,int) except +
        void send_str(string)
        string get_str()
        void handshake(string)
        void pong(string)
        void join(string)
        unique_ptr[message] next_msg()
