<template>
	<view class="wrap">
		<!-- 内边距 -->
		<view class="dev-area">
			<view class="dev-card">
				<!-- 水位 -->
				<view class="">
					<view class="dev-name">水位</view>
					<image class="dev-logo" src="../../static/hight.png" mode=""></image>
				</view>
				<view class="dev-data">{{Level}}</view>
			</view>
			<view class="dev-card">
				<!-- TDS -->
				<view class="">
					<view class="dev-name">水质</view>
					<image class="dev-logo" src="../../static/pH.png" mode=""></image>
				</view>
				<view class="dev-data">{{TDS}}</view>
			</view>
			<view class="dev-card">
				<!-- 浑浊度 -->
				<view class="">
					<view class="dev-name">浑浊度</view>
					<image class="dev-logo" src="../../static/ntu.png" mode=""></image>
				</view>
				<view class="dev-data">{{NTU}}</view>
			</view>
			<view class="dev-card">
				<!-- 浑浊度 -->
				<view class="">
					<view class="dev-name">PH值</view>
					<image class="dev-logo" src="../../static/tds.png" mode=""></image>
				</view>
				<view class="dev-data">{{PH}}</view>
			</view>
			<view class="dev-card">
				<!-- 水温 -->
				<view class="">
					<view class="dev-name">水温</view>
					<image class="dev-logo" src="../../static/temp.png" mode=""></image>
				</view>
				<view class="dev-data">{{temperature}}</view>
			</view>
			<view class="dev-card">
				<!-- kong -->
				<view class="">
					<view class="dev-name">更多数据</view>
				</view>
			</view>
			<view class="dev-card">
				<!-- led卡片 -->
				<view class="">
					<view class="dev-name">增氧泵</view>
					<image class="dev-logo" src="../../static/ox.png" mode=""></image>
				</view>
				<switch :checked="led" @change="onLedSwitch" color="#2b9939" />
			</view>
			<view class="dev-card">
				<!-- hester卡片 -->
				<view class="">
					<view class="dev-name">喂食</view>
					<image class="dev-logo" src="../../static/ws.png" mode=""></image>
				</view>
				<switch :checked="heater" @change="onheaterSwitch" color="#2b9939" />
			</view>
			<view class="dev-card">
				<!-- Oxygen卡片 -->
				<view class="">
					<view class="dev-name">加热</view>
					<image class="dev-logo" src="../../static/zg.png" mode=""></image>
				</view>
				<switch :checked="Oxygen" @change="onOxygenSwitch" color="#2b9939" />
			</view>

			<view class="dev-card">
				<!-- Pump卡片 -->
				<view class="">
					<view class="dev-name">水 泵</view>
					<image class="dev-logo" src="../../static/bump.png" mode=""></image>

				</view>
				<switch :checked="Pump" @change="onPumpSwitch" color="#2b9939" />
			</view>
			<!-- 		<view class="dev-card">

	
		<view class="dev-name1"> 警告!</view>
		<switch :checked="beep" @change="onbeepSwitch" color="red" />
	</view -->
			<view v-if="mode == true" class="dev-card" style="background-color: green;">
				手动
			</view>
			<view v-else class="dev-card" style="background-color: green;">
				自动
			</view>

			<view v-if="beep == true" class="dev-card" style="background-color: red;">
				危险
			</view>
			<view v-else class="dev-card" style="background-color: green;">
				正常
			</view>
		</view>

		<!-- 水温阈值长卡片 -->
		<view class="device-cart-l">
			<view>
				<view class="dev-name">水温下限</view>
			</view>
			<view class="ctrl-slider">
				<slider :value="tempflag" @change="sliderChange($event,'slider1')" min="0" max="24" step="0.5"
					block-size="20" show-value />
			</view>
		</view>
		<!-- 水温下限阈值长卡片 -->
		<view class="device-cart-l">
			<view>
				<view class="dev-name">水温上限</view>
			</view>
			<view class="ctrl-slider">
				<slider :value="tempflag_h" @change="sliderChange($event,'slider1_h')" min="18" max="39" step="0.5"
					block-size="20" show-value />
			</view>
		</view>
		<!-- ph阈值长卡片 -->
		<view class="device-cart-l">
			<view>
				<view class="dev-name">PH下限</view>
			</view>
			<view class="ctrl-slider">
				<slider :value="hightflag" @change="sliderChange($event,'sliderhight')" min="0" max="7.5" step="0.1"
					block-size="20" show-value />
			</view>
		</view>

		<!-- ph下限阈值长卡片 -->
		<view class="device-cart-l">
			<view>
				<view class="dev-name">PH上限</view>
			</view>
			<view class="ctrl-slider">
				<slider :value="hightflag_h" @change="sliderChange($event,'sliderhight_h')" min="7" max="14" step="0.1"
					block-size="20" show-value />
			</view>
		</view>
		<!-- 水质阈值长卡片 -->
		<view class="device-cart-l">
			<view>
				<view class="dev-name">水质下限</view>
			</view>
			<view class="ctrl-slider">
				<slider :value="tdsflag" @change="sliderChange($event,'slidertds')" min="0" max="200" step="1"
					block-size="20" show-value />
			</view>
		</view>

		<!-- 水质下限阈值长卡片 -->
		<view class="device-cart-l">
			<view>
				<view class="dev-name">水质上限</view>
			</view>
			<view class="ctrl-slider">
				<slider :value="tdsflag_h" @change="sliderChange($event,'slidertds_h')" min="200" max="600" step="1"
					block-size="20" show-value />
			</view>
		</view>

		<!-- 浑浊度阈值长卡片 -->
		<view class="device-cart-l">
			<view>
				<view class="dev-name">浑浊度下限</view>
			</view>
			<view class="ctrl-slider">
				<slider :value="ntuflag" @change="sliderChange($event,'sliderntu')" min="0" max="20" step="1"
					block-size="20" show-value />
			</view>
		</view>

		<!-- 浑浊度下限阈值长卡片 -->
		<view class="device-cart-l">
			<view>
				<view class="dev-name">浑浊度上限</view>
			</view>
			<view class="ctrl-slider">
				<slider :value="ntuflag_h" @change="sliderChange($event,'sliderntu_h')" min="8" max="50" step="1"
					block-size="20" show-value />
			</view>
		</view>

	</view>
</template>

<script>
	import {
		stringify
	} from 'querystring';
	const {
		createCommonToken
	} = require('@/key.js')
	export default {
		data() {
			return {
				Level: "",
				TDS: "",
				NTU: "",
				PH: "",
				temperature: "",
				led: true,
				mode: true,
				heater: true,
				Oxygen: true,
				Pump: true,
				beep: true,
				token: 'version=2018-10-31&res=products%2FAfva6w9eoy%2Fdevices%2Fv1&et=2041175704&method=md5&sign=any2hI357MAVgwkL5uuwdA%3D%3D',
				tempflag: 18,
				hightflag: 6,
				tdsflag: 100,
				ntuflag: 0,
				tempflag_h: 25,
				hightflag_h: 8,
				tdsflag_h: 300,
				ntuflag_h: 20,
				switch_flag: 0, // 标记开关操作
				// 更新MQTT连接参数
				productId: 'Afva6w9eoy',
				deviceName: 'v1',
				authCode: '12345688',
				psk: 'CgSmv8sqMzba9jTi',
				mqttConfig: {
					broker: '183.230.40.96',
					port: 1883,
					clientId: 'v1',
					username: 'Afva6w9eoy',
					password: 'version=2018-10-31&res=products%2FAfva6w9eoy%2Fdevices%2Fv1&et=2041175704&method=md5&sign=any2hI357MAVgwkL5uuwdA%3D%3D'
				}
			};
		},
		onLoad() {
			// 直接连接 MQTT
			this.connectMQTT()
			
			// 保持原有的数据轮询
			setInterval(() => {
				this.fetchDevData()
			}, 5000)
		},
		onShow() {
			this.readStoredThresholds(); // 读取已存储的阈值数据
		},
		methods: {
			connectMQTT() {
				console.log('正在连接到 MQTT...')
				
				// 使用 MQTT over WebSocket
				uni.connectSocket({
					url: 'wss://mqtts.heclouds.com:443/mqtt',
					header: {
						'Sec-WebSocket-Protocol': 'mqtt'
					},
					success: () => {
						console.log('WebSocket 连接创建成功')
					},
					fail: (error) => {
						console.error('WebSocket 连接失败:', error)
					}
				})

				uni.onSocketOpen(() => {
					console.log('WebSocket 已连接，发送 MQTT CONNECT 包')
					
					// MQTT CONNECT 包格式
					const connectPacket = new Uint8Array([
						0x10, // CONNECT
						0x0C + this.mqttConfig.clientId.length + this.mqttConfig.username.length + this.mqttConfig.password.length,
						0x00, 0x04, // Protocol Name Length
						0x4D, 0x51, 0x54, 0x54, // "MQTT"
						0x04, // Protocol Version
						0xC2, // Connect Flags (Username, Password, Clean Session)
						0x00, 0x3C, // Keep Alive (60 seconds)
						0x00, this.mqttConfig.clientId.length, // Client ID Length
						...Array.from(this.mqttConfig.clientId).map(c => c.charCodeAt(0)),
						0x00, this.mqttConfig.username.length, // Username Length
						...Array.from(this.mqttConfig.username).map(c => c.charCodeAt(0)),
						0x00, this.mqttConfig.password.length, // Password Length
						...Array.from(this.mqttConfig.password).map(c => c.charCodeAt(0))
					])
					
					// 发送二进制数据
					uni.sendSocketMessage({
						data: connectPacket.buffer,
						success: () => console.log('MQTT CONNECT 包发送成功'),
						fail: (error) => console.error('MQTT CONNECT 包发送失败:', error)
					})
				})

				uni.onSocketMessage((res) => {
					try {
						// 处理二进制数据
						const data = new Uint8Array(res.data)
						const packetType = data[0] >> 4
						
						if (packetType === 2) { // CONNACK
							console.log('MQTT 连接成功')
							this.subscribeMQTTTopics()
						} else if (packetType === 3) { // PUBLISH
							// 解析 PUBLISH 包
							const message = this.decodeMQTTPublish(data)
							this.handleMQTTMessage(message)
						}
					} catch (e) {
						console.error('消息解析错误:', e)
					}
				})

				uni.onSocketClose(() => {
					console.log('WebSocket 连接已关闭，5秒后重试')
					setTimeout(() => this.connectMQTT(), 5000)
				})

				uni.onSocketError((error) => {
					console.error('WebSocket 错误:', error)
				})
			},

			// 编码 SUBSCRIBE 包
			encodeSubscribe(topic, messageId = 1) {
				const topicLength = topic.length
				const packet = new Uint8Array([
					0x82, // SUBSCRIBE
					2 + 2 + topicLength + 1, // Remaining Length
					messageId >> 8, messageId & 0xFF, // Message ID
					topicLength >> 8, topicLength & 0xFF, // Topic Length
					...Array.from(topic).map(c => c.charCodeAt(0)), // Topic
					0x00 // QoS
				])
				return packet
			},

			// 解码 PUBLISH 包
			decodeMQTTPublish(data) {
				// 简单的解码逻辑，实际应该更复杂
				const topicLength = (data[2] << 8) | data[3]
				const topic = String.fromCharCode(...data.slice(4, 4 + topicLength))
				const payload = String.fromCharCode(...data.slice(4 + topicLength))
				try {
					return JSON.parse(payload)
				} catch (e) {
					return payload
				}
			},

			subscribeMQTTTopics() {
				const topics = [
					`$sys/${this.mqttConfig.username}/${this.mqttConfig.clientId}/thing/property/post`,
					`$sys/${this.mqttConfig.username}/${this.mqttConfig.clientId}/thing/event/post`
				]

				topics.forEach((topic, index) => {
					const packet = this.encodeSubscribe(topic, index + 1)
					uni.sendSocketMessage({
						data: packet.buffer,
						success: () => console.log('订阅成功:', topic),
						fail: (error) => console.error('订阅失败:', error)
					})
				})
			},

			handleMQTTMessage(message) {
				if (message.properties) {
					const props = message.properties
					if (props.Level !== undefined) this.Level = props.Level
					if (props.TDS !== undefined) this.TDS = props.TDS
					if (props.NTU !== undefined) this.NTU = props.NTU
					if (props.PH !== undefined) this.PH = props.PH
					if (props.temperature !== undefined) this.temperature = props.temperature
					if (props.led !== undefined) this.led = props.led === 'true'
					if (props.heater !== undefined) this.heater = props.heater === 'true'
					if (props.Oxygen !== undefined) this.Oxygen = props.Oxygen === 'true'
					if (props.Pump !== undefined) this.Pump = props.Pump === 'true'
					if (props.beep !== undefined && this.switch_flag !== 1) {
						this.beep = props.beep === 'true'
					}
					if (props.mode !== undefined) this.mode = props.mode === 'true'
				}
			},
			fetchDevData() {
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/query-device-property',
					method: 'GET',
					data: {
						product_id: 'Afva6w9eoy',
						device_name: 'v1'
					},
					header: {
						'authorization': this.token,
						'Content-Type': 'application/json'
					},
					success: (res) => {
						console.log('API响应:', res.data)
						if (res.data.code === 10403) {
							console.error('Token验证失败，当前token:', this.token)
						}
						if(res.data && res.data.data) {
							this.Level = res.data.data[0].value;
							this.TDS = res.data.data[5].value;
							this.NTU = res.data.data[1].value;
							this.PH = res.data.data[3].value;
							this.temperature = res.data.data[16].value;
							this.led = res.data.data[10].value === 'true';
							this.heater = res.data.data[7].value === 'true';
							this.Oxygen = res.data.data[2].value === 'true';
							this.Pump = res.data.data[4].value === 'true';
							this.beep = res.data.data[6].value === 'true';
							this.mode = res.data.data[11].value === 'true';

							if (this.switch_flag == 1) this.switch_flag = 0;
							else this.beep = res.data.data[6].value === 'true';
						}
					},
					fail: (err) => {
						console.error('请求失败:', err)
					}
				});
			},
			onLedSwitch(event) {
				console.log(event.detail.value);
				let value = event.detail.value;
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property',
					method: 'POST',
					data: {
						product_id: 'Afva6w9eoy',
						device_name: 'v1',
						params: {
							"led": value
						}
					},
					header: {
						'authorization': this.token,
						'Content-Type': 'application/json'
					},
					success: (res) => {
						console.log('LED ' + (value ? 'ON' : 'OFF') + '!');
					}
				});
			},
			onheaterSwitch(event) {
				console.log(event.detail.value);
				let value = event.detail.value;
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property',
					method: 'POST',
					data: {
						product_id: 'Afva6w9eoy',
						device_name: 'v1',
						params: {
							"heater": value
						}
					},
					header: {
						'authorization': this.token,
						'Content-Type': 'application/json'
					},
					success: () => {
						console.log('heater ' + (value ? 'ON' : 'OFF') + '!');
					}
				});
			},
			onOxygenSwitch(event) {
				console.log(event.detail.value);
				let value = event.detail.value;
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property',
					method: 'POST',
					data: {
						product_id: 'Afva6w9eoy',
						device_name: 'v1',
						params: {
							"Oxygen": value
						}
					},
					header: {
						'authorization': this.token,
						'Content-Type': 'application/json'
					},
					success: () => {
						console.log('Oxygen ' + (value ? 'ON' : 'OFF') + '!');
					}
				});
			},
			onPumpSwitch(event) {
				console.log(event.detail.value);
				let value = event.detail.value;
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property',
					method: 'POST',
					data: {
						product_id: 'Afva6w9eoy',
						device_name: 'v1',
						params: {
							"Pump": value
						}
					},
					header: {
						'authorization': this.token,
						'Content-Type': 'application/json'
					},
					success: () => {
						console.log('Pump ' + (value ? 'ON' : 'OFF') + '!');
					}
				});
			},
			sliderChange(e, id) {
				console.log('value 发生变化：' + e.detail.value);
				let dataToSend = {};
				if (id === 'slider1') {
					this.tempflag = e.detail.value;
					dataToSend = {
						product_id: 'Afva6w9eoy',
						device_name: 'v1',
						params: {
							tempflag: this.tempflag
						}
					};
					this.storeThreshold('tempflag', this.tempflag);
				} else if (id === 'sliderhight') {
					this.hightflag = e.detail.value;
					dataToSend = {
						product_id: 'Afva6w9eoy',
						device_name: 'v1',
						params: {
							hightflag: this.hightflag
						}
					};
					this.storeThreshold('hightflag', this.hightflag);
				} else if (id === 'slidertds') {
					this.tdsflag = e.detail.value;
					dataToSend = {
						product_id: 'Afva6w9eoy',
						device_name: 'v1',
						params: {
							tdsflag: this.tdsflag
						}
					};
					this.storeThreshold('tdsflag', this.tdsflag);
				} else if (id === 'sliderntu') {
					this.ntuflag = e.detail.value;
					dataToSend = {
						product_id: 'Afva6w9eoy',
						device_name: 'v1',
						params: {
							ntuflag: this.ntuflag
						}
					};
					this.storeThreshold('ntuflag', this.ntuflag);
				} else if (id === 'slider1_h') {
					this.tempflag_h = e.detail.value;
					dataToSend = {
						product_id: 'Afva6w9eoy',
						device_name: 'v1',
						params: {
							tempflag_h: this.tempflag_h
						}
					};
					this.storeThreshold('tempflag_h', this.tempflag_h);
				} else if (id === 'sliderhight_h') {
					this.hightflag_h = e.detail.value;
					dataToSend = {
						product_id: 'Afva6w9eoy',
						device_name: 'v1',
						params: {
							hightflag_h: this.hightflag_h
						}
					};
					this.storeThreshold('hightflag_h', this.hightflag_h);
				} else if (id === 'slidertds_h') {
					this.tdsflag_h = e.detail.value;
					dataToSend = {
						product_id: 'Afva6w9eoy',
						device_name: 'v1',
						params: {
							tdsflag_h: this.tdsflag_h
						}
					};
					this.storeThreshold('tdsflag_h', this.tdsflag_h);
				} else if (id === 'sliderntu_h') {
					this.ntuflag_h = e.detail.value;
					dataToSend = {
						product_id: 'Afva6w9eoy',
						device_name: 'v1',
						params: {
							ntuflag_h: this.ntuflag_h
						}
					};
					this.storeThreshold('ntuflag_h', this.ntuflag_h);
				}
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property',
					method: 'POST',
					data: dataToSend,
					header: {
						'authorization': this.token,
						'Content-Type': 'application/json'
					},
					success: (res) => {
						console.log('更新成功，返回结果：', res);
					},
					fail: (err) => {
						console.error('更新失败，错误信息：', err);
					}
				});
			},
			// 存储单个阈值数据到本地存储
			storeThreshold(key, value) {
				const thresholds = {
					...this.getStoredThresholds(),
					[key]: value
				};
				uni.setStorage({
					key: 'thresholds',
					data: thresholds,
					success: () => {
						console.log(`${key} 阈值存储成功`);
					},
					fail: (err) => {
						console.log(`${key} 阈值存储失败，错误信息：`, err);
					}
				});
			},
			// 从本地存储中读取所有阈值数据
			getStoredThresholds() {
				return new Promise((resolve, reject) => {
					uni.getStorage({
						key: 'thresholds',
						success: (res) => {
							resolve(res.data || {});
						},
						fail: (err) => {
							console.log('读取阈值数据失败，错误信息：', err);
							resolve({});
						}
					});
				});
			},
			// 在页面显示时读取已存储的阈值数据并赋值给对应变量
			async readStoredThresholds() {
				const storedThresholds = await this.getStoredThresholds();
				this.tempflag = storedThresholds.tempflag || this.tempflag;
				this.hightflag = storedThresholds.hightflag || this.hightflag;
				this.tdsflag = storedThresholds.tdsflag || this.tdsflag;
				this.ntuflag = storedThresholds.ntuflag || this.ntuflag;
				this.tempflag_h = storedThresholds.tempflag_h || this.tempflag_h;
				this.hightflag_h = storedThresholds.hightflag_h || this.hightflag_h;
				this.tdsflag_h = storedThresholds.tdsflag_h || this.tdsflag_h;
				this.ntuflag_h = storedThresholds.ntuflag_h || this.ntuflag_h;
			},
		}
	}
</script>

<style>
	.wrap {
		padding: 30rpx;
	}

	.dev-area {
		display: flex;
		justify-content: space-between;
		flex-wrap: wrap;
		/* 自动换行 */
	}

	.dev-card {
		/* 大卡片 */
		height: 130rpx;
		width: 300rpx;
		border-radius: 30rpx;
		/* 边框圆弧线 */
		margin-top: 30rpx;
		/* 设置元素的顶部外边距外边距区域。 */
		display: flex;
		justify-content: space-around;
		align-items: center;
		box-shadow: 0 0 15rpx #000000;
		/* 轮廓边框线 #cc为阴影*/
		background-color: #fffcea;
	}

	/* 长设备卡片样式 */
	.device-cart-l {
		height: 70rpx;
		/* 设置高度为150像素 */
		width: 700rpx;
		/* 设置宽度为700像素 */
		border-radius: 20rpx;
		/* 设置边框圆角为30像素 */
		margin-top: 40rpx;
		/* 设置上外边距为30像素 */
		display: flex;
		/* 使用弹性盒子布局 */
		justify-content: space-around;
		/* 在弹性容器中均匀分布子元素，两端对齐 */
		align-items: center;
		/* 在弹性容器中垂直居中对齐子元素 */
		box-shadow: 0 0 15rpx #000000;
		/* 设置盒子阴影，颜色为灰色 */
		background-color: #fffcea;
	}

	.dev-name {
		/* 检测对象字体 */
		font-size: 20rpx;
		/* 字体大小 */
		text-align: center;
		/* 水平对齐 */
		color: black;
	}

	.dev-name1 {
		/* 检测对象字体 */
		font-size: 40rpx;
		/* 字体大小 */
		text-align: center;
		/* 水平对齐 */
		color: red;
	}

	.dev-logo {
		/* 小卡片logo */
		height: 70rpx;
		width: 70rpx;
		margin-top: 10rpx;
		/* 设置元素的顶部外边距外边距区域。 */
	}

	.dev-data {
		/* 检测数据字体 */
		font-size: 50rpx;
		/* 字体大小 */
		color: black;
	}

	._button {
		margin-top: 10rpx;
		width: 130rpx;
		height: 50rpx;
		line-height: 50rpx;
	}

	.title {
		font-size: 40rpx;
		color: black;
	}

	/* 滑动条样式 */
	.ctrl-slider {
		color: black;
		width: 580rpx;
		/* 设置宽度为580像素 */
		margin: 20rpx 0;
		/* 上下添加20rpx的外边距，让滑动条在布局中有合适间隔 */

	}



	.ctrl-slider::-webkit-slider-thumb {
		-webkit-appearance: none;
		background-color: #007BFF;
		/* 拇指背景颜色设为蓝色 */
		width: 30rpx;
		/* 拇指宽度 */
		height: 30rpx;
		/* 拇指高度 */
		border-radius: 50%;
		/* 拇指设置为圆形 */
		margin-top: -12rpx;
		/* 调整拇指垂直方向位置，使其在轨道上居中，这里的值可能需要根据轨道和拇指实际尺寸微调 */
	}
</style>