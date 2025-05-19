from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from kivy.uix.label import Label
from kivy.uix.scrollview import ScrollView
from kivy.uix.textinput import TextInput
from kivy.clock import Clock
from kivy.core.window import Window
from kivy.uix.gridlayout import GridLayout
from kivy.properties import StringProperty, BooleanProperty
import threading
import time
import logging
import json
import paho.mqtt.client as mqtt

# 配置日志
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

# MQTT服务器配置
MQTT_BROKER = "mqtts.heclouds.com"
MQTT_PORT = 1883
CLIENT_ID = "d1"
USER_NAME = "f04YhrRc9T"
PASSWORD = "version=2018-10-31&res=products%2Ff04YhrRc9T%2Fdevices%2Fd1&et=2042093691&method=md5&sign=xDPyAWq9Zb%2FsFzqSbniJ8g%3D%3D"

# 主题配置
TOPIC_PROPERTY = f"$sys/{USER_NAME}/{CLIENT_ID}/thing/property/post"
TOPIC_PROPERTY_REPLY = f"$sys/{USER_NAME}/{CLIENT_ID}/thing/property/post/reply"
TOPIC_CMD = f"$sys/{USER_NAME}/{CLIENT_ID}/thing/command/invoke"
TOPIC_CMD_RESP = f"$sys/{USER_NAME}/{CLIENT_ID}/thing/command/invoke/response"

class MQTTControlLayout(BoxLayout):
    connection_status = StringProperty("未连接")
    led_state = BooleanProperty(False)
    
    def __init__(self, **kwargs):
        super(MQTTControlLayout, self).__init__(**kwargs)
        self.orientation = 'vertical'
        self.padding = [20, 20, 20, 20]
        self.spacing = 10
        
        self.mqtt_client = None
        self.last_response = ""
        
        # 状态栏
        status_layout = BoxLayout(size_hint=(1, None), height=40)
        self.status_label = Label(text=f"状态: {self.connection_status}", size_hint=(0.7, 1))
        self.connect_button = Button(
            text="连接", 
            size_hint=(0.3, 1),
            background_color=(0.3, 0.6, 0.9, 1)
        )
        self.connect_button.bind(on_press=self.connect_mqtt)
        
        status_layout.add_widget(self.status_label)
        status_layout.add_widget(self.connect_button)
        self.add_widget(status_layout)
        
        # LED 控制部分
        control_layout = BoxLayout(orientation='vertical', size_hint=(1, 0.4))
        control_layout.add_widget(Label(text="LED 控制", size_hint=(1, 0.3)))
        
        # LED 状态显示
        led_status_layout = BoxLayout(size_hint=(1, 0.3))
        led_status_layout.add_widget(Label(text="LED 状态:", size_hint=(0.3, 1)))
        self.led_status_label = Label(text="关闭", color=(0.5, 0.5, 0.5, 1), size_hint=(0.7, 1))
        led_status_layout.add_widget(self.led_status_label)
        control_layout.add_widget(led_status_layout)
        
        # LED 控制按钮
        button_layout = BoxLayout(size_hint=(1, 0.4), spacing=10)
        
        self.toggle_button = Button(
            text="切换 LED", 
            disabled=True,
            background_color=(0.3, 0.6, 0.9, 1)
        )
        self.toggle_button.bind(on_press=self.toggle_led)
        
        self.on_button = Button(
            text="打开 LED", 
            disabled=True,
            background_color=(0.2, 0.7, 0.2, 1)
        )
        self.on_button.bind(on_press=lambda x: self.set_led(True))
        
        self.off_button = Button(
            text="关闭 LED", 
            disabled=True,
            background_color=(0.7, 0.2, 0.2, 1)
        )
        self.off_button.bind(on_press=lambda x: self.set_led(False))
        
        button_layout.add_widget(self.toggle_button)
        button_layout.add_widget(self.on_button)
        button_layout.add_widget(self.off_button)
        control_layout.add_widget(button_layout)
        
        self.add_widget(control_layout)
        
        # 响应消息显示
        response_layout = BoxLayout(orientation='vertical', size_hint=(1, 0.6))
        response_layout.add_widget(Label(text="MQTT 响应", size_hint=(1, 0.1)))
        
        self.response_scroll = ScrollView(size_hint=(1, 0.9))
        self.response_text = TextInput(
            readonly=True,
            multiline=True,
            size_hint=(1, None),
            height=Window.height * 0.4
        )
        self.response_scroll.add_widget(self.response_text)
        response_layout.add_widget(self.response_scroll)
        
        self.add_widget(response_layout)
        
        # 定时更新状态
        Clock.schedule_interval(self.update_status, 0.5)
    
    def connect_mqtt(self, instance):
        if self.connection_status == "已连接":
            self.disconnect_mqtt()
            return
        
        try:
            # 禁用连接按钮，防止重复点击
            self.connect_button.disabled = True
            self.connect_button.text = "连接中..."
            
            # 创建新线程进行连接，避免阻塞UI
            threading.Thread(target=self._connect_mqtt_thread, daemon=True).start()
        except Exception as e:
            logger.error(f"连接MQTT服务器失败: {e}")
            self.add_response(f"连接错误: 无法连接到MQTT服务器: {e}")
            self.connect_button.disabled = False
            self.connect_button.text = "连接"
    
    def _connect_mqtt_thread(self):
        try:
            # 创建MQTT客户端
            client = mqtt.Client(client_id=CLIENT_ID, callback_api_version=mqtt.CallbackAPIVersion.VERSION1)
            
            # 设置回调函数
            client.on_connect = self.on_connect
            client.on_message = self.on_message
            client.on_disconnect = self.on_disconnect
            
            # 设置用户名和密码
            client.username_pw_set(USER_NAME, PASSWORD)
            
            # 连接到MQTT服务器
            logger.info(f"正在连接到MQTT服务器 {MQTT_BROKER}:{MQTT_PORT}")
            client.connect(MQTT_BROKER, MQTT_PORT, 60)
            
            # 启动网络循环
            client.loop_start()
            
            # 保存客户端实例
            self.mqtt_client = client
            
        except Exception as e:
            logger.error(f"连接MQTT服务器失败: {e}")
            Clock.schedule_once(lambda dt: self.update_ui_after_connect_failure(str(e)), 0)
            self.connection_status = "连接失败"
    
    def update_ui_after_connect_failure(self, error_msg):
        self.add_response(f"连接错误: {error_msg}")
        self.connect_button.disabled = False
        self.connect_button.text = "连接"
    
    def disconnect_mqtt(self):
        if self.mqtt_client:
            try:
                self.mqtt_client.loop_stop()
                self.mqtt_client.disconnect()
                self.mqtt_client = None
                self.connection_status = "未连接"
                
                # 更新UI
                self.toggle_button.disabled = True
                self.on_button.disabled = True
                self.off_button.disabled = True
                self.connect_button.text = "连接"
                
                self.add_response("已断开与MQTT服务器的连接")
                logger.info("已断开与MQTT服务器的连接")
            except Exception as e:
                logger.error(f"断开连接时出错: {e}")
    
    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            self.connection_status = "已连接"
            logger.info("已成功连接到MQTT服务器")
            
            # 订阅主题
            client.subscribe(TOPIC_PROPERTY_REPLY)
            client.subscribe(TOPIC_CMD)
            
            # 更新UI（在主线程中）
            Clock.schedule_once(lambda dt: self.update_ui_after_connect(), 0)
            
            self.add_response("已成功连接到MQTT服务器")
        else:
            self.connection_status = "连接失败"
            logger.error(f"连接失败，返回码: {rc}")
            
            # 更新UI（在主线程中）
            Clock.schedule_once(lambda dt: self.update_ui_after_connect_failure(f"连接失败，返回码: {rc}"), 0)
    
    def update_ui_after_connect(self):
        self.connect_button.disabled = False
        self.connect_button.text = "断开连接"
        self.toggle_button.disabled = False
        self.on_button.disabled = False
        self.off_button.disabled = False
    
    def on_message(self, client, userdata, msg):
        try:
            payload = msg.payload.decode()
            logger.info(f"收到消息 [{msg.topic}]: {payload}")
            
            # 保存响应
            self.last_response = f"[{msg.topic}] {payload}"
            self.add_response(self.last_response)
            
            # 尝试解析JSON数据
            try:
                data = json.loads(payload)
                
                # 如果是属性上报回复，检查是否成功
                if msg.topic == TOPIC_PROPERTY_REPLY:
                    if data.get("code") == 200 and "led" in str(payload):
                        # 更新LED状态显示
                        if "true" in str(payload).lower():
                            Clock.schedule_once(lambda dt: self.update_led_status(True), 0)
                        elif "false" in str(payload).lower():
                            Clock.schedule_once(lambda dt: self.update_led_status(False), 0)
            except json.JSONDecodeError:
                logger.info("消息不是JSON格式")
        except Exception as e:
            logger.error(f"处理消息时出错: {e}")
    
    def on_disconnect(self, client, userdata, rc):
        if rc != 0:
            self.connection_status = "意外断开"
            logger.warning("意外断开连接")
            self.add_response("意外断开连接，尝试重新连接...")
            
            # 更新UI（在主线程中）
            Clock.schedule_once(lambda dt: self.update_ui_after_disconnect(), 0)
    
    def update_ui_after_disconnect(self):
        self.connect_button.disabled = False
        self.connect_button.text = "连接"
        self.toggle_button.disabled = True
        self.on_button.disabled = True
        self.off_button.disabled = True
    
    def toggle_led(self, instance):
        self.set_led(not self.led_state)
    
    def set_led(self, state):
        if not self.mqtt_client:
            self.add_response("错误: 请先连接到MQTT服务器")
            return
        
        try:
            # 更新LED状态
            self.led_state = state
            
            # 构建属性上报消息
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
            result = self.mqtt_client.publish(TOPIC_PROPERTY, json.dumps(message))
            status = result[0]
            
            if status == 0:
                logger.info(f"LED 状态已设置为: {'开启' if state else '关闭'}")
                self.add_response(f"已发送: LED 状态设置为 {'开启' if state else '关闭'}")
                self.update_led_status(state)
            else:
                logger.error(f"发送消息失败，状态码: {status}")
                self.add_response(f"发送失败，状态码: {status}")
        except Exception as e:
            logger.error(f"设置 LED 状态时出错: {e}")
            self.add_response(f"错误: 设置 LED 状态时出错: {e}")
    
    def update_led_status(self, state):
        self.led_state = state
        if state:
            self.led_status_label.text = "开启"
            self.led_status_label.color = (0, 1, 0, 1)  # 绿色
        else:
            self.led_status_label.text = "关闭"
            self.led_status_label.color = (0.5, 0.5, 0.5, 1)  # 灰色
    
    def add_response(self, text):
        Clock.schedule_once(lambda dt: self._add_response_ui(text), 0)
    
    def _add_response_ui(self, text):
        self.response_text.text += f"{time.strftime('%H:%M:%S')} - {text}\n"
        # 滚动到底部
        self.response_text.cursor = (0, len(self.response_text.text))
    
    def update_status(self, dt):
        # 更新状态标签
        self.status_label.text = f"状态: {self.connection_status}"

class MQTTLEDControl(App):
    def build(self):
        return MQTTControlLayout()

if __name__ == "__main__":
    MQTTLEDControl().run() 