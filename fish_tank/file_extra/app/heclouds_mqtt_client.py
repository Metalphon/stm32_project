import paho.mqtt.client as mqtt
import time
import json
import logging
import hmac
import hashlib
import base64
import urllib.parse

# 配置日志
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

# MQTT服务器配置（根据图片信息）
MQTT_BROKER = "mqtts.heclouds.com"  # 从图片中看到的 Broker Address
MQTT_PORT = 1883  # 从图片中看到的 Broker Port
CLIENT_ID = "d1"  # 从图片中看到的 Client ID

# 用户凭证（从图片和token信息中获取）
USER_NAME = "f04YhrRc9T"  # 从图片中看到的 User Name
PASSWORD = "version=2018-10-31&res=products%2Ff04YhrRc9T%2Fdevices%2Fd1&et=2042093691&method=md5&sign=xDPyAWq9Zb%2FsFzqSbniJ8g%3D%3D"  # 您提供的token

# 主题配置（根据HECloud的主题格式）
TOPIC_PROPERTY = f"$sys/{USER_NAME}/{CLIENT_ID}/thing/property/post"  # 属性上报主题
TOPIC_PROPERTY_REPLY = f"$sys/{USER_NAME}/{CLIENT_ID}/thing/property/post/reply"  # 属性上报回复主题
TOPIC_CMD = f"$sys/{USER_NAME}/{CLIENT_ID}/thing/command/invoke"  # 命令下发主题
TOPIC_CMD_RESP = f"$sys/{USER_NAME}/{CLIENT_ID}/thing/command/invoke/response"  # 命令响应主题

# 当连接到MQTT代理时的回调函数
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        logger.info("已成功连接到HECloud MQTT服务器")
        # 连接成功后订阅相关主题
        client.subscribe(TOPIC_PROPERTY_REPLY)
        logger.info(f"已订阅主题: {TOPIC_PROPERTY_REPLY}")
        client.subscribe(TOPIC_CMD)
        logger.info(f"已订阅主题: {TOPIC_CMD}")
    else:
        logger.error(f"连接失败，返回码: {rc}")
        # 返回码含义：
        # 0: 连接成功
        # 1: 协议版本错误
        # 2: 无效的客户端标识
        # 3: 服务器不可用
        # 4: 用户名或密码错误
        # 5: 未授权
        if rc == 4:
            logger.error("用户名或密码错误，请检查凭证信息")

# 当收到消息时的回调函数
def on_message(client, userdata, msg):
    try:
        payload = msg.payload.decode()
        logger.info(f"收到消息 [{msg.topic}]: {payload}")
        
        # 尝试解析JSON数据
        try:
            data = json.loads(payload)
            logger.info(f"解析的JSON数据: {data}")
            
            # 如果是命令下发，则响应命令
            if msg.topic == TOPIC_CMD:
                handle_command(client, data)
        except json.JSONDecodeError:
            logger.info("消息不是JSON格式")
    except Exception as e:
        logger.error(f"处理消息时出错: {e}")

# 处理下发的命令
def handle_command(client, data):
    try:
        # 提取命令ID和参数
        cmd_id = data.get("id")
        cmd_name = data.get("params", {}).get("cmdName", "")
        cmd_params = data.get("params", {}).get("cmdParams", {})
        
        logger.info(f"收到命令: {cmd_name}, 参数: {cmd_params}")
        
        # 构建命令响应
        response = {
            "id": cmd_id,
            "code": 200,  # 200表示成功
            "msg": "success",
            "data": {
                "result": f"命令 {cmd_name} 已执行成功"
            }
        }
        
        # 发送命令响应
        client.publish(TOPIC_CMD_RESP, json.dumps(response))
        logger.info(f"已发送命令响应: {response}")
    except Exception as e:
        logger.error(f"处理命令时出错: {e}")

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
    # 创建MQTT客户端实例，指定回调API版本为1
    client = mqtt.Client(client_id=CLIENT_ID, callback_api_version=mqtt.CallbackAPIVersion.VERSION1)
    
    # 设置回调函数
    client.on_connect = on_connect
    client.on_message = on_message
    client.on_publish = on_publish
    client.on_disconnect = on_disconnect
    
    # 设置用户名和密码
    client.username_pw_set(USER_NAME, PASSWORD)
    
    # 连接到MQTT服务器
    try:
        logger.info(f"正在连接到MQTT服务器 {MQTT_BROKER}:{MQTT_PORT}")
        client.connect(MQTT_BROKER, MQTT_PORT, 60)
    except Exception as e:
        logger.error(f"连接MQTT服务器失败: {e}")
        return None
    
    return client

def publish_property(client, properties):
    """发布设备属性数据"""
    try:
        # 只保留已知的属性（目前只有 led）
        valid_properties = {}
        if "led" in properties:
            valid_properties["led"] = properties["led"]
        
        # 如果没有有效属性，直接返回
        if not valid_properties:
            logger.info("没有有效的属性可上报")
            return
        
        # 构建属性上报消息
        current_time = int(time.time() * 1000)  # 转换为毫秒
        formatted_properties = {}
        
        # 格式化每个属性，添加时间戳
        for key, value in valid_properties.items():
            formatted_properties[key] = {
                "value": value,
                "time": current_time
            }
        
        message = {
            "id": str(int(time.time())),
            "version": "1.0",
            "params": formatted_properties
        }
        
        # 发布消息
        result = client.publish(TOPIC_PROPERTY, json.dumps(message))
        status = result[0]
        if status == 0:
            logger.info(f"属性数据已发送: {message}")
        else:
            logger.error(f"发送属性数据失败，状态码: {status}")
    except Exception as e:
        logger.error(f"发布属性数据时出错: {e}")

def control_led(client, state):
    """控制 LED 灯的开关状态"""
    try:
        # 构建属性上报消息 - 根据 HECloud 文档格式
        current_time = int(time.time() * 1000)  # 转换为毫秒
        
        message = {
            "id": str(int(time.time())),
            "version": "1.0",
            "params": {
                "led": {
                    "value": state,
                    "time": current_time
                }
            }
        }
        
        # 发布消息
        result = client.publish(TOPIC_PROPERTY, json.dumps(message))
        status = result[0]
        if status == 0:
            logger.info(f"LED 状态已设置为: {'开启' if state else '关闭'}")
        else:
            logger.error(f"设置 LED 状态失败，状态码: {status}")
    except Exception as e:
        logger.error(f"控制 LED 时出错: {e}")

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
        
        # 首先将 LED 设置为 false (关闭)
        logger.info("正在将 LED 设置为关闭状态...")
        control_led(client, False)
        time.sleep(3)  # 增加等待时间，确保命令被处理
        
        # 循环控制 LED
        for i in range(3):  # 减少循环次数，避免过多操作
            # 切换 LED 状态
            new_state = i % 2 == 0  # 偶数次为 True，奇数次为 False
            control_led(client, new_state)
            logger.info(f"LED 已{'打开' if new_state else '关闭'} (循环 {i+1}/3)")
            time.sleep(5)
        
        # 最后确保 LED 为关闭状态
        logger.info("准备最终关闭 LED...")
        control_led(client, False)
        logger.info("已发送最终关闭 LED 命令")
        
        # 等待确认关闭命令被处理
        time.sleep(3)
        
        # 再次确认 LED 已关闭
        logger.info("再次确认 LED 关闭...")
        control_led(client, False)
        
        # 保持程序运行一段时间以接收消息
        logger.info("等待接收消息...")
        time.sleep(5)
        
        # 最后一次确认关闭
        logger.info("程序结束前最后一次确认 LED 关闭...")
        control_led(client, False)
        time.sleep(2)
        
    except KeyboardInterrupt:
        logger.info("程序被用户中断")
        # 即使被中断也尝试关闭 LED
        try:
            control_led(client, False)
            time.sleep(1)
        except:
            pass
    finally:
        # 停止网络循环并断开连接
        client.loop_stop()
        client.disconnect()
        logger.info("程序已退出")

if __name__ == "__main__":
    main() 