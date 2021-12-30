import paho.mqtt.client as mqtt

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("temperature")
    client.subscribe("humidity")

pairs = [["temperature", "tempValue"], ["humidity", "tempValue"]]

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    for i in range(len(pairs)):
        if msg.topic == pairs[i][0]:
            pairs[i][1] = str(msg.payload)
            updateDB()

def updateDB():
    file1 = open("database.txt","w")
    for sensor, value in pairs:
        file1.write(sensor + ": " + value + "\n")
    file1.close()


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("127.0.0.1", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()