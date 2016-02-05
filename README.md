###A server to forward data between two client
####About request header
#####0. wait
    WAIT [id(should not exceed int)] [timeout(0 for forever)]
#####1. send
    SEND [length(in byte)] [data]
####About response header
#####0. active
    ACTIVE
#####1. receive
    RECEIVE [length] [data]