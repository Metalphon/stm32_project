import paho.mqtt.client as mqtt
import time
import json
import logging

# 配置日志
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

# MQTT服务器配置
MQTT_BROKER = "broker.emqx.io"  # 公共MQTT服务器，也可以替换为你自己的服务器
MQTT_PORT = 1883
MQTT_KEEPALIVE = 60
MQTT_USERNAME = ""  # 如果需要认证，请填写用户名
MQTT_PASSWORD = ""  # 如果需要认证，请填写密码
CLIENT_ID = f"python-mqtt-client-{time.time()}"  # 创建唯一的客户端ID

# 主题配置
TOPIC_SUBSCRIBE = "test/topic"
TOPIC_PUBLISH = "test/topic"

# 当连接到MQTT代理时的回调函数
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        logger.info("已成功连接到MQTT服务器")
        # 连接成功后订阅主题
        client.subscribe(TOPIC_SUBSCRIBE)
        logger.info(f"已订阅主题: {TOPIC_SUBSCRIBE}")
    else:
        logger.error(f"连接失败，返回码: {rc}")

# 当收到消息时的回调函数
def on_message(client, userdata, msg):
    try:
        payload = msg.payload.decode()
        logger.info(f"收到消息 [{msg.topic}]: {payload}")
        
        # 尝试解析JSON数据
        try:
            data = json.loads(payload)
            logger.info(f"解析的JSON数据: {data}")
        except json.JSONDecodeError:
            logger.info("消息不是JSON格式")
    except Exception as e:
        logger.error(f"处理消息时出错: {e}")

# 当消息发布时的回调函数
def on_publish(client, userdata, mid):
    logger.info(f"消息ID {mid} 已发布")

# 当断开连接时的回调函数
def on_disconnect(client, userdata, rc):
    if rc != 0:
        logger.warning("意外断开连接，尝试重新连接...")
    else:
        logger.info("已断开与MQTT服务器的连接")

def create_mqtt_client():
    # 创建MQTT客户端实例
    client = mqtt.Client(CLIENT_ID)
    
    # 设置回调函数
    client.on_connect = on_connect
    client.on_message = on_message
    client.on_publish = on_publish
    client.on_disconnect = on_disconnect
    
    # 如果需要认证
    if MQTT_USERNAME and MQTT_PASSWORD:
        client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)
    
    # 连接到MQTT服务器
    try:
        logger.info(f"正在连接到MQTT服务器 {MQTT_BROKER}:{MQTT_PORT}")
        client.connect(MQTT_BROKER, MQTT_PORT, MQTT_KEEPALIVE)
    except Exception as e:
        logger.error(f"连接MQTT服务器失败: {e}")
        return None
    
    return client

def publish_message(client, topic, message):
    """发布消息到指定主题"""
    try:
        # 如果消息是字典，转换为JSON字符串
        if isinstance(message, dict):
            message = json.dumps(message)
        
        result = client.publish(topic, message)
        status = result[0]
        if status == 0:
            logger.info(f"消息已发送到主题 {topic}")
        else:
            logger.error(f"发送消息失败，状态码: {status}")
    except Exception as e:
        logger.error(f"发布消息时出错: {e}")

def main():
    # 创建MQTT客户端
    client = create_mqtt_client()
    if not client:
        return
    
    # 启动网络循环
    client.loop_start()
    
    try:
        # 等待连接建立
        time.sleep(2)
        
        # 发布一些测试消息
        for i in range(5):
            message = {
                "id": i,
                "timestamp": time.time(),
                "value": f"测试消息 {i}",
                "status": "正常"
            }
            publish_message(client, TOPIC_PUBLISH, message)
            time.sleep(2)
        
        # 保持程序运行一段时间以接收消息
        logger.info("等待接收消息...")
        time.sleep(30)
        
    except KeyboardInterrupt:
        logger.info("程序被用户中断")
    finally:
        # 停止网络循环并断开连接
        client.loop_stop()
        client.disconnect()
        logger.info("程序已退出")

if __name__ == "__main__":
    main() 