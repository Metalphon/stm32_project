import tkinter as tk
from tkinter import ttk, messagebox
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

# 全局变量
mqtt_client = None
led_state = False
connection_status = "未连接"
last_response = ""

class MQTTControlApp:
    def __init__(self, root):
        self.root = root
        self.root.title("MQTT LED 控制")
        self.root.geometry("600x400")
        self.root.resizable(True, True)
        
        # 设置样式
        self.style = ttk.Style()
        self.style.configure("TButton", font=("微软雅黑", 12))
        self.style.configure("TLabel", font=("微软雅黑", 12))
        self.style.configure("Status.TLabel", font=("微软雅黑", 10))
        
        # 创建主框架
        self.main_frame = ttk.Frame(root, padding="20")
        self.main_frame.pack(fill=tk.BOTH, expand=True)
        
        # 连接状态标签
        self.status_frame = ttk.Frame(self.main_frame)
        self.status_frame.pack(fill=tk.X, pady=(0, 20))
        
        self.status_label = ttk.Label(self.status_frame, text=f"状态: {connection_status}", style="Status.TLabel")
        self.status_label.pack(side=tk.LEFT)
        
        # 连接按钮
        self.connect_button = ttk.Button(self.status_frame, text="连接", command=self.connect_mqtt)
        self.connect_button.pack(side=tk.RIGHT, padx=5)
        
        # LED 控制部分
        self.control_frame = ttk.LabelFrame(self.main_frame, text="LED 控制", padding="10")
        self.control_frame.pack(fill=tk.BOTH, expand=True)
        
        # LED 状态显示
        self.led_status_frame = ttk.Frame(self.control_frame)
        self.led_status_frame.pack(fill=tk.X, pady=10)
        
        self.led_label = ttk.Label(self.led_status_frame, text="LED 状态:")
        self.led_label.pack(side=tk.LEFT, padx=(0, 10))
        
        self.led_status = ttk.Label(self.led_status_frame, text="关闭", foreground="gray")
        self.led_status.pack(side=tk.LEFT)
        
        # LED 控制按钮
        self.button_frame = ttk.Frame(self.control_frame)
        self.button_frame.pack(fill=tk.X, pady=10)
        
        self.toggle_button = ttk.Button(self.button_frame, text="切换 LED", command=self.toggle_led, state=tk.DISABLED)
        self.toggle_button.pack(side=tk.LEFT, padx=5)
        
        self.on_button = ttk.Button(self.button_frame, text="打开 LED", command=lambda: self.set_led(True), state=tk.DISABLED)
        self.on_button.pack(side=tk.LEFT, padx=5)
        
        self.off_button = ttk.Button(self.button_frame, text="关闭 LED", command=lambda: self.set_led(False), state=tk.DISABLED)
        self.off_button.pack(side=tk.LEFT, padx=5)
        
        # 响应消息显示
        self.response_frame = ttk.LabelFrame(self.main_frame, text="MQTT 响应", padding="10")
        self.response_frame.pack(fill=tk.BOTH, expand=True, pady=(20, 0))
        
        self.response_text = tk.Text(self.response_frame, height=5, wrap=tk.WORD)
        self.response_text.pack(fill=tk.BOTH, expand=True)
        self.response_text.config(state=tk.DISABLED)
        
        # 创建滚动条
        scrollbar = ttk.Scrollbar(self.response_text, command=self.response_text.yview)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        self.response_text.config(yscrollcommand=scrollbar.set)
        
        # 定时更新状态
        self.update_status()
    
    def connect_mqtt(self):
        global mqtt_client, connection_status
        
        if connection_status == "已连接":
            self.disconnect_mqtt()
            return
        
        try:
            # 禁用连接按钮，防止重复点击
            self.connect_button.config(state=tk.DISABLED)
            self.connect_button.config(text="连接中...")
            
            # 创建新线程进行连接，避免阻塞UI
            threading.Thread(target=self._connect_mqtt_thread, daemon=True).start()
        except Exception as e:
            logger.error(f"连接MQTT服务器失败: {e}")
            messagebox.showerror("连接错误", f"无法连接到MQTT服务器: {e}")
            self.connect_button.config(state=tk.NORMAL)
            self.connect_button.config(text="连接")
    
    def _connect_mqtt_thread(self):
        global mqtt_client, connection_status
        
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
            mqtt_client = client
            
        except Exception as e:
            logger.error(f"连接MQTT服务器失败: {e}")
            self.root.after(0, lambda: messagebox.showerror("连接错误", f"无法连接到MQTT服务器: {e}"))
            self.root.after(0, lambda: self.connect_button.config(state=tk.NORMAL, text="连接"))
            connection_status = "连接失败"
    
    def disconnect_mqtt(self):
        global mqtt_client, connection_status
        
        if mqtt_client:
            try:
                mqtt_client.loop_stop()
                mqtt_client.disconnect()
                mqtt_client = None
                connection_status = "未连接"
                
                # 更新UI
                self.toggle_button.config(state=tk.DISABLED)
                self.on_button.config(state=tk.DISABLED)
                self.off_button.config(state=tk.DISABLED)
                self.connect_button.config(text="连接")
                
                self.add_response("已断开与MQTT服务器的连接")
                logger.info("已断开与MQTT服务器的连接")
            except Exception as e:
                logger.error(f"断开连接时出错: {e}")
    
    def on_connect(self, client, userdata, flags, rc):
        global connection_status
        
        if rc == 0:
            connection_status = "已连接"
            logger.info("已成功连接到MQTT服务器")
            
            # 订阅主题
            client.subscribe(TOPIC_PROPERTY_REPLY)
            client.subscribe(TOPIC_CMD)
            
            # 更新UI（在主线程中）
            self.root.after(0, lambda: self.connect_button.config(state=tk.NORMAL, text="断开连接"))
            self.root.after(0, lambda: self.toggle_button.config(state=tk.NORMAL))
            self.root.after(0, lambda: self.on_button.config(state=tk.NORMAL))
            self.root.after(0, lambda: self.off_button.config(state=tk.NORMAL))
            
            self.add_response("已成功连接到MQTT服务器")
        else:
            connection_status = "连接失败"
            logger.error(f"连接失败，返回码: {rc}")
            
            # 更新UI（在主线程中）
            self.root.after(0, lambda: self.connect_button.config(state=tk.NORMAL, text="连接"))
            self.root.after(0, lambda: messagebox.showerror("连接错误", f"连接失败，返回码: {rc}"))
            
            self.add_response(f"连接失败，返回码: {rc}")
    
    def on_message(self, client, userdata, msg):
        global last_response
        
        try:
            payload = msg.payload.decode()
            logger.info(f"收到消息 [{msg.topic}]: {payload}")
            
            # 保存响应
            last_response = f"[{msg.topic}] {payload}"
            self.add_response(last_response)
            
            # 尝试解析JSON数据
            try:
                data = json.loads(payload)
                
                # 如果是属性上报回复，检查是否成功
                if msg.topic == TOPIC_PROPERTY_REPLY:
                    if data.get("code") == 200 and "led" in str(payload):
                        # 更新LED状态显示
                        if "true" in str(payload).lower():
                            self.update_led_status(True)
                        elif "false" in str(payload).lower():
                            self.update_led_status(False)
            except json.JSONDecodeError:
                logger.info("消息不是JSON格式")
        except Exception as e:
            logger.error(f"处理消息时出错: {e}")
    
    def on_disconnect(self, client, userdata, rc):
        global connection_status
        
        if rc != 0:
            connection_status = "意外断开"
            logger.warning("意外断开连接")
            self.add_response("意外断开连接，尝试重新连接...")
            
            # 更新UI（在主线程中）
            self.root.after(0, lambda: self.connect_button.config(state=tk.NORMAL, text="连接"))
            self.root.after(0, lambda: self.toggle_button.config(state=tk.DISABLED))
            self.root.after(0, lambda: self.on_button.config(state=tk.DISABLED))
            self.root.after(0, lambda: self.off_button.config(state=tk.DISABLED))
    
    def toggle_led(self):
        global led_state
        self.set_led(not led_state)
    
    def set_led(self, state):
        global mqtt_client, led_state
        
        if not mqtt_client:
            messagebox.showwarning("未连接", "请先连接到MQTT服务器")
            return
        
        try:
            # 更新LED状态
            led_state = state
            
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
            result = mqtt_client.publish(TOPIC_PROPERTY, json.dumps(message))
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
            messagebox.showerror("错误", f"设置 LED 状态时出错: {e}")
    
    def update_led_status(self, state):
        # 在主线程中更新UI
        self.root.after(0, lambda: self._update_led_status_ui(state))
    
    def _update_led_status_ui(self, state):
        if state:
            self.led_status.config(text="开启", foreground="green")
        else:
            self.led_status.config(text="关闭", foreground="gray")
    
    def add_response(self, text):
        # 在主线程中更新UI
        self.root.after(0, lambda: self._add_response_ui(text))
    
    def _add_response_ui(self, text):
        self.response_text.config(state=tk.NORMAL)
        self.response_text.insert(tk.END, f"{time.strftime('%H:%M:%S')} - {text}\n")
        self.response_text.see(tk.END)  # 滚动到底部
        self.response_text.config(state=tk.DISABLED)
    
    def update_status(self):
        # 更新状态标签
        self.status_label.config(text=f"状态: {connection_status}")
        
        # 每500毫秒更新一次
        self.root.after(500, self.update_status)

if __name__ == "__main__":
    root = tk.Tk()
    app = MQTTControlApp(root)
    root.mainloop()
    
    # 确保在程序退出时断开连接
    if mqtt_client:
        try:
            mqtt_client.loop_stop()
            mqtt_client.disconnect()
        except:
            pass 