import Irc

zoite=Irc.Connection(b"irc.zoite.net",6667)
print("Connecting to irc.zoite.net...")
zoite.Handshake(b"n2_bot")
print("Handshake complete.")

while True:
    msg=zoite.nextMessage()
    print(msg.String().decode("utf-8"))
    command = msg.Command().decode("utf-8")
    
    if command == "PING":
        key=msg.Parameters()
        zoite.Pong(key)
        print("PONG "+key.decode("utf-8"))
    elif command == "CAP":
        zoite.Send(bytes("CAP END","utf-8"))
        print("CAP END")
    elif command == "INVITE":
        zoite.Join(msg.Channel())


