import base64
import hmac
import time
import requests
from hashlib import sha1
import json
from urllib.parse import quote
import hashlib
import paho.mqtt.client as mqtt

class OneNetAPI:
    def __init__(self):
        # OneNET 平台配置
        self.product_id = 'Afva6w9eoy'
        self.device_name = 'v1'
        self.auth_key = "aUUkau1c1z1iRqrgyZQv5b9CNpzI68LGdkHdk3N36ICZuPmt6s3lTPAdFnbXi/bvBGCjrgAspKCO05irolC4yA=="
        self.user_id = "425521"
        self.base_url = "https://iot-api.heclouds.com"
        
        # MQTT配置
        self.mqtt_broker = "183.230.40.96"
        self.mqtt_port = 1883
        self.mqtt_client_id = "v1"  # 设备名称作为客户端ID
        self.mqtt_username = self.product_id  # 产品ID作为用户名
        self.mqtt_password = "your_password_here"  # 从OneNET获取的密码

    def generate_token(self):
        """生成访问令牌"""
        version = '2018-10-31'
        res = f'products/{self.product_id}/devices/{self.device_name}'
        et = int(time.time()) + 365 * 24 * 3600
        method = 'md5'
        
        # 生成签名字符串
        sign_str = f"{et}\n{method}\n{res}\n{version}"
        
        print("Debug info:")
        print(f"Resource path: {res}")
        print(f"Sign string:\n{sign_str}")
        
        # 计算签名 - 使用 md5 而不是 hmac
        key = sign_str.encode('utf-8')
        sign = base64.b64encode(hashlib.md5(key).digest()).decode('utf-8')
        
        # URL编码
        encoded_res = quote(res, safe='')
        encoded_sign = quote(sign, safe='')
        
        # 生成token
        token = f"version={version}&res={encoded_res}&et={et}&method={method}&sign={encoded_sign}"
        
        print(f"\nToken components:")
        print(f"version: {version}")
        print(f"res: {encoded_res}")
        print(f"et: {et}")
        print(f"method: {method}")
        print(f"sign: {encoded_sign}")
        
        return token

    def query_device_property(self):
        """查询设备属性"""
        token = self.generate_token()
        url = f"{self.base_url}/thingmodel/query-device-property"
        
        headers = {
            'authorization': token,
            'Content-Type': 'application/json'
        }
        
        params = {
            'product_id': self.product_id,
            'device_name': self.device_name
        }
        
        print("\nRequest details:")
        print(f"URL: {url}")
        print(f"Headers: {dict(headers)}")
        print(f"Params: {params}")
        
        try:
            response = requests.get(url, headers=headers, params=params)
            print(f"\nResponse details:")
            print(f"Status code: {response.status_code}")
            print(f"Headers: {dict(response.headers)}")
            print(f"Content: {response.text}")
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"Request failed: {e}")
            if hasattr(e, 'response') and e.response is not None:
                print(f"Response text: {e.response.text}")
            return None

    def set_device_property(self, property_name, value):
        """设置设备属性"""
        token = self.generate_token()
        url = f"{self.base_url}/thingmodel/set-device-property"
        
        headers = {
            'authorization': token,
            'Content-Type': 'application/json'
        }
        
        data = {
            'product_id': self.product_id,
            'device_name': self.device_name,
            'params': {
                property_name: value
            }
        }
        
        response = requests.post(url, headers=headers, json=data)
        return response.json()

    def test_different_resources(self):
        """测试不同的资源路径"""
        resource_paths = [
            'products/Afva6w9eoy/devices/v1/properties',  # 设备属性
            'products/Afva6w9eoy/devices/v1/property',   # 单个属性
            'products/Afva6w9eoy/devices/v1/thingmodel', # 设备物模型
            'products/Afva6w9eoy/thingmodel/property',   # 产品物模型属性
            'thingmodel/property',                       # 物模型属性通用路径
            'products/Afva6w9eoy/devices/v1/command'     # 设备命令
        ]

        for res in resource_paths:
            print(f"\n测试资源路径: {res}")
            version = '2018-10-31'
            et = int(time.time()) + 365 * 24 * 3600
            method = 'sha1'

            # 生成签名字符串
            sign_str = f"{et}\n{method}\n{res}\n{version}"
            
            # 计算签名
            key = base64.b64decode(self.auth_key)
            hmac_obj = hmac.new(key, sign_str.encode('utf-8'), sha1)
            sign = base64.b64encode(hmac_obj.digest()).decode('utf-8')

            # URL编码
            encoded_res = quote(res, safe='')
            encoded_sign = quote(sign, safe='')

            # 生成token
            token = f"version={version}&res={encoded_res}&et={et}&method={method}&sign={encoded_sign}"

            print("Token:", token)

            # 测试API调用
            url = f"{self.base_url}/thingmodel/query-device-property"
            headers = {
                'authorization': token,
                'Content-Type': 'application/json'
            }
            params = {
                'product_id': self.product_id,
                'device_name': self.device_name
            }

            try:
                response = requests.get(url, headers=headers, params=params)
                print(f"Status: {response.status_code}")
                print(f"Response: {response.text}\n")
                print(f"Request URL: {response.request.url}")
                print(f"Request Headers: {response.request.headers}")
            except Exception as e:
                print(f"Error: {e}\n")

    def generate_mqtt_password(self):
        """返回固定的token作为MQTT密码"""
        # 使用固定的token
        password = "version=2018-10-31&res=products%2FAfva6w9eoy%2Fdevices%2Fv1&et=2041175704&method=md5&sign=any2hI357MAVgwkL5uuwdA%3D%3D"
        
        print("Using fixed MQTT password:")
        print(password)
        
        return password

    def connect_mqtt(self):
        """连接到MQTT服务器"""
        def on_connect(client, userdata, flags, rc):
            rc_codes = {
                0: "Connection successful",
                1: "Connection refused - incorrect protocol version",
                2: "Connection refused - invalid client identifier",
                3: "Connection refused - server unavailable",
                4: "Connection refused - bad username or password",
                5: "Connection refused - not authorized"
            }
            print(f"Connection result: {rc_codes.get(rc, f'Unknown error ({rc})')}")

        def on_message(client, userdata, msg):
            print(f"Received message: {msg.topic} -> {msg.payload.decode()}")

        # 获取固定的密码
        mqtt_password = self.generate_mqtt_password()
        
        # 创建MQTT客户端
        client = mqtt.Client(client_id=self.mqtt_client_id, protocol=mqtt.MQTTv311)
        client.username_pw_set(self.mqtt_username, mqtt_password)
        
        # 设置回调
        client.on_connect = on_connect
        client.on_message = on_message
        
        # 连接到服务器
        try:
            print(f"\nConnecting to {self.mqtt_broker}:{self.mqtt_port}")
            print(f"Client ID: {self.mqtt_client_id}")
            print(f"Username: {self.mqtt_username}")
            client.connect(self.mqtt_broker, self.mqtt_port)
            return client
        except Exception as e:
            print(f"Connection failed: {e}")
            return None

    def subscribe_topics(self, client):
        """订阅相关主题"""
        # 设备属性上报主题
        property_topic = f"$sys/{self.product_id}/{self.device_name}/thing/property/post"
        # 设备事件上报主题
        event_topic = f"$sys/{self.product_id}/{self.device_name}/thing/event/post"
        
        client.subscribe([(property_topic, 0), (event_topic, 0)])
        print(f"Subscribed to topics:\n{property_topic}\n{event_topic}")

def main():
    api = OneNetAPI()
    
    # 生成并打印MQTT密码
    print("\nMQTT Connection Info:")
    print(f"Broker: {api.mqtt_broker}")
    print(f"Port: {api.mqtt_port}")
    print(f"Client ID: {api.mqtt_client_id}")
    print(f"Username: {api.mqtt_username}")
    
    # 连接MQTT
    mqtt_client = api.connect_mqtt()
    if mqtt_client:
        try:
            # 订阅主题
            api.subscribe_topics(mqtt_client)
            # 开始循环
            mqtt_client.loop_start()  # 使用 loop_start() 而不是 loop_forever()
            
            # 保持程序运行
            while True:
                time.sleep(1)
                
        except KeyboardInterrupt:
            print("\nDisconnecting from MQTT Broker...")
            mqtt_client.disconnect()
            mqtt_client.loop_stop()

if __name__ == "__main__":
    main() 