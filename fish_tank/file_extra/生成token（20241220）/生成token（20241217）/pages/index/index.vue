<template>
	<view class="container">
		<text>MQTT Status: {{connectionStatus}}</text>
		<button @click="connectMQTT">Connect MQTT</button>
		<text>Last Message: {{lastMessage}}</text>
	</view>
</template>

<script>
export default {
	data() {
		return {
			connectionStatus: 'Disconnected',
			lastMessage: 'No messages yet',
			mqttConfig: {
				broker: '183.230.40.96',
				port: 1883,
				clientId: 'v1',
				username: 'Afva6w9eoy',
				password: 'version=2018-10-31&res=products%2FAfva6w9eoy%2Fdevices%2Fv1&et=2041175704&method=md5&sign=any2hI357MAVgwkL5uuwdA%3D%3D',
				connectionTimeout: 30,
				keepAliveInterval: 60,
				cleanSession: true,
				autoReconnect: false,
				maxInflight: 10,
				protocolVersion: '3.1.1'
			}
		}
	},
	methods: {
		connectMQTT() {
			this.connectionStatus = 'Connecting...'
			console.log('Connecting to MQTT...')

			// 使用 MQTT over WebSocket
			const wsUrl = 'ws://183.230.40.96:8083/mqtt'  // OneNET 的 WebSocket 端口
			console.log('Connecting to:', wsUrl)

			uni.connectSocket({
				url: wsUrl,
				header: {
					'Sec-WebSocket-Protocol': 'mqtt'  // 必须指定 MQTT 协议
				},
				success: () => {
					console.log('WebSocket connection created successfully')
				},
				fail: (error) => {
					console.error('WebSocket connection failed:', error)
					this.connectionStatus = 'Connection Failed'
				},
				complete: () => {
					console.log('WebSocket connection attempt completed')
				}
			})

			// 连接成功后发送 CONNECT 包
			uni.onSocketOpen(() => {
				console.log('Socket connected, sending MQTT CONNECT packet')
				this.connectionStatus = 'Socket Connected'
				
				// 构建 MQTT CONNECT 包
				const packet = this.buildConnectPacket()
				console.log('CONNECT packet:', Array.from(packet))

				// 发送二进制数据
				this.sendPacket(packet)
			})

			// 接收消息
			uni.onSocketMessage((res) => {
				console.log('Received data:', res)
				this.handleMQTTPacket(res.data)
			})

			// 连接错误和关闭处理
			uni.onSocketError((error) => {
				console.error('Connection error:', error)
				this.connectionStatus = 'Error'
				// 尝试重新连接
				setTimeout(() => this.connectMQTT(), 5000)
			})

			uni.onSocketClose(() => {
				console.log('Connection closed')
				this.connectionStatus = 'Disconnected'
				setTimeout(() => this.connectMQTT(), 5000)
			})
		},

		// 构建 CONNECT 包
		buildConnectPacket() {
			const clientId = this.mqttConfig.clientId
			const username = this.mqttConfig.username
			const password = this.mqttConfig.password

			// MQTT 固定头部
			const header = [
				0x10,  // CONNECT 类型
				0x00   // 长度（稍后填充）
			]

			// 可变头部
			const variableHeader = [
				0x00, 0x04,  // 协议名长度
				0x4D, 0x51, 0x54, 0x54,  // "MQTT"
				0x03,  // 协议版本 3.1.1
				0xC2,  // 连接标志 (用户名、密码、清理会话)
				0x00, 0x3C  // 保持连接时间 60s
			]

			// 有效载荷
			const payload = [
				...this.encodeString(clientId),
				...this.encodeString(username),
				...this.encodeString(password)
			]

			// 计算剩余长度
			const remainingLength = variableHeader.length + payload.length
			header[1] = remainingLength

			return new Uint8Array([...header, ...variableHeader, ...payload])
		},

		// 编码 MQTT 字符串
		encodeString(str) {
			const bytes = Array.from(str).map(c => c.charCodeAt(0))
			return [bytes.length >> 8, bytes.length & 0xFF, ...bytes]
		},

		// 发送数据包
		sendPacket(packet) {
			uni.sendSocketMessage({
				data: packet.buffer,
				success: () => console.log('Packet sent successfully'),
				fail: (error) => console.error('Failed to send packet:', error)
			})
		},

		// 处理接收到的 MQTT 包
		handleMQTTPacket(data) {
			try {
				const packet = new Uint8Array(data)
				const type = packet[0] >> 4

				switch (type) {
					case 2:  // CONNACK
						console.log('Connected to MQTT broker')
						this.connectionStatus = 'Connected'
						this.subscribe()
						break
					case 3:  // PUBLISH
						this.handlePublish(packet)
						break
					default:
						console.log('Received packet type:', type)
				}
			} catch (e) {
				console.error('Error handling packet:', e)
			}
		},

		// 订阅主题
		subscribe() {
			const topic = `$sys/${this.mqttConfig.username}/${this.mqttConfig.clientId}/thing/property/post`
			const topicLength = topic.length
			
			// 构建 SUBSCRIBE 包
			const packet = new Uint8Array([
				0x82, // SUBSCRIBE
				2 + 2 + topicLength + 1, // Remaining Length
				0x00, 0x01, // Message ID
				0x00, topicLength, // Topic Length
				...Array.from(topic).map(c => c.charCodeAt(0)), // Topic
				0x00 // QoS
			])

			uni.sendSocketMessage({
				data: packet.buffer,
				success: () => console.log('SUBSCRIBE packet sent'),
				fail: (error) => console.error('Failed to send SUBSCRIBE packet:', error)
			})
		},

		// 处理 PUBLISH 消息
		handlePublish(data) {
			try {
				// 解析主题长度
				const topicLength = (data[2] << 8) | data[3]
				// 解析主题
				const topic = String.fromCharCode(...data.slice(4, 4 + topicLength))
				// 解析负载
				const payload = String.fromCharCode(...data.slice(4 + topicLength))
				
				console.log('Received message on topic:', topic)
				this.lastMessage = payload
				
				try {
					const jsonData = JSON.parse(payload)
					this.lastMessage = JSON.stringify(jsonData, null, 2)
				} catch (e) {
					// 如果不是 JSON 格式，直接显示原始数据
					this.lastMessage = payload
				}
			} catch (e) {
				console.error('Failed to parse PUBLISH packet:', e)
			}
		}
	},
	
	mounted() {
		this.connectMQTT()
	}
}
</script>

<style>
.container {
	padding: 20px;
}
</style>