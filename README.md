###A server to forward data between two client
####About request header
#####0. wait
    WAIT[   7483648](of fixed total length 14)
#####1. send
    SEND\r\n[id]\r\n[length(in byte)]\r\n[data]
####About response header
#####0. active
    ACTV
#####1. receive
    RCEV\r\n[length]\r\n[data]
###About Architecture
This is a producer and consumer problem

