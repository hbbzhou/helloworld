



#if 0
#include <openssl/aes.h>
#include "base64.h"


//https://blog.poxiao.me/p/advanced-encryption-standard-and-block-cipher-mode/

std::string fromhex(const char *str)
{
	size_t len = strlen(str) / 2;
	std::string strBuff;
	strBuff.resize(len);
	if (len > 512) len = 512;
	for (size_t i = 0; i < len; i++) {
		unsigned char c = 0;
		if (str[i * 2] >= '0' && str[i * 2] <= '9')
			c += (str[i * 2] - '0') << 4;
		if ((str[i * 2] & ~0x20) >= 'A' && (str[i * 2] & ~0x20) <= 'F')
			c += (10 + (str[i * 2] & ~0x20) - 'A') << 4;
		if (str[i * 2 + 1] >= '0' && str[i * 2 + 1] <= '9')
			c += (str[i * 2 + 1] - '0');
		if ((str[i * 2 + 1] & ~0x20) >= 'A' && (str[i * 2 + 1] & ~0x20) <= 'F')
			c += (10 + (str[i * 2 + 1] & ~0x20) - 'A');
		strBuff[i] = c;
	}
	return strBuff;
}

const char * hex_to_str( char *pSrc, int len)
{
	static char buf[512];
	for (int i = 0; i < len; i++){
		unsigned char cTmp = (unsigned char)pSrc[i];
		cTmp = (cTmp & 0xff);
		sprintf(buf + (i * 2), "%02x", cTmp);
	}
	return buf;
}

void test_main(int argc, char* argv[]) {
	std::string key = "hbbhehedadatttts";
	std::string plan_vec = "1234567890123456";
	std::vector<unsigned char> encode_vec(16);
	std::vector<unsigned char> decode_vec(16);

	// aes encrypt
	AES_KEY aes_enc_ctx;
	AES_set_encrypt_key( (const unsigned char *)key.data(), 128, &aes_enc_ctx);
	AES_ecb_encrypt((const unsigned char *)plan_vec.data(), encode_vec.data(), &aes_enc_ctx , AES_ENCRYPT);

	// aes decrypt
	AES_KEY aes_dec_ctx;
	AES_set_decrypt_key((const unsigned char *)key.data(), 128, &aes_dec_ctx);
	AES_ecb_encrypt(encode_vec.data(), decode_vec.data(), &aes_dec_ctx, AES_DECRYPT);

	std::string encoded_str = base64_encode((unsigned char const*)encode_vec.data(), encode_vec.size() );

	std::cout << "C++:"<< std::endl;
	std::cout << "源数据\t\t:" << plan_vec << std::endl;
	std::cout << "加密数据\t:" << encoded_str << std::endl;
	std::cout << "解密数据\t:" << std::string( (char *)decode_vec.data() , decode_vec.size() ) << std::endl;

}

#endif



#if 1

#include <boost/crc.hpp>


void test_main(int argc, char* argv[]) {
	std::string str_ = "womemjintianqushangxuezhou";
	boost::crc_32_type crc32;
	crc32.process_bytes(str_.data(), str_.size() );
	unsigned int ret = crc32.checksum();
	std::cout << "C++:" << std::endl;
	std::cout << "源数据\t\t:" << str_ << std::endl;
	std::cout << "校验值\t\t:" << ret << std::endl;
}

#endif