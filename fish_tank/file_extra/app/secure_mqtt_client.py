import paho.mqtt.client as mqtt
import ssl
import time

# MQTT配置
broker = "test.mosquitto.org"  # 支持TLS的公共MQTT服务器
port = 8883  # TLS端口
topic = "test/topic"
client_id = f"python-mqtt-secure-{time.time()}"

def on_connect(client, userdata, flags, rc):
    print(f"已连接，返回码: {rc}")
    client.subscribe(topic)

def on_message(client, userdata, msg):
    print(f"收到消息: {msg.payload.decode()}")

# 创建客户端
client = mqtt.Client(client_id)
client.on_connect = on_connect
client.on_message = on_message

# 配置TLS
client.tls_set(
    ca_certs=None,  # 可以指定CA证书路径
    certfile=None,  # 客户端证书
    keyfile=None,   # 客户端密钥
    cert_reqs=ssl.CERT_REQUIRED,
    tls_version=ssl.PROTOCOL_TLS,
    ciphers=None
)

# 连接服务器
client.connect(broker, port, 60)

# 启动循环
client.loop_start()

# 发布消息
for i in range(3):
    message = f"安全连接测试消息 {i}"
    client.publish(topic, message)
    print(f"已发布: {message}")
    time.sleep(2)

# 等待接收消息
time.sleep(10)
client.loop_stop() 