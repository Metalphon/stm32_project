const crypto = require('crypto')

function createCommonToken(params) {
    // 验证必要参数
    if (!params.author_key || !params.version || !params.user_id) {
        throw new Error('Missing required parameters')
    }

    const access_key = Buffer.from(params.author_key, "base64")
    const version = params.version
    const user_id = params.user_id.toString() // 确保user_id是字符串
    const et = Math.ceil((Date.now() + 365 * 24 * 3600 * 1000) / 1000)
    const method = 'md5'  // 改为 md5
    
    // 使用与工具相同的资源路径格式
    let res = 'products/Afva6w9eoy/devices/v1'
    
    // 生成签名字符串
    const signStr = [et, method, res, version].join('\n')
    
    // 计算签名
    const sign = crypto.createHash('md5')  // 使用 md5 而不是 hmac
        .update(signStr)
        .digest('base64')
    
    // URL编码
    const encoded_res = encodeURIComponent(res)
    const encoded_sign = encodeURIComponent(sign)
    
    // 打印详细的调试信息
    console.log('Token generation details:', {
        input: {
            author_key: params.author_key,
            version,
            user_id,
            et,
            method,
            res
        },
        signStr,
        sign,
        encoded_sign,
        final_token: `version=${version}&res=${encoded_res}&et=${et}&method=${method}&sign=${encoded_sign}`
    })
    
    // 生成token
    const token = `version=${version}&res=${encoded_res}&et=${et}&method=${method}&sign=${encoded_sign}`
    return token
}

module.exports = {
  createCommonToken
};


