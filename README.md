###A server to forward data between two client
####About request header
#####0. wait
    WAIT\r\n[id(should not exceed int)]\r\n\r\n
#####1. send
    SEND\r\n[id]\r\n[length(in byte)]\r\n[data]
####About response header
#####0. active
    ACTV
#####1. receive
    RCEV\r\n[length]\r\n[data]


####There should be reset command