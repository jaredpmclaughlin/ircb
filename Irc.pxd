from libcpp.string cimport string

cdef extern from "irc.cpp":
    pass

cdef extern from "irc.h" namespace "irc":
    cdef cppclass connection:
        connection(string,int) except +
        void send_str(string)
        string get_str()
        void handshake(string)
