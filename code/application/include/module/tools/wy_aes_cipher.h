
#ifndef __M_AES_CIPHER_H__
#define __M_AES_CIPHER_H__

#define MODULE_CIPHER				"CIPHER"

#define AES_ECB_NORMAL_TYPE			(0x100)
#define AES_ECB_XOR_TYPE			(0x101)
#define AES_CBC_TYPE				(0x200)

#define AES_CIPHER_LENGHT			(16)

typedef void (*aes_setkeyiv)(H_U32,H_U32 ,const H_U8*,const H_U8 *);
typedef void (*aes_encrypt_decrypt)(H_U8 *, H_U8*);

typedef struct aes_transit{
	H_U32 type;
	H_U32 len;
	H_U8  key[AES_CIPHER_LENGHT];
	H_U8  init_vector[AES_CIPHER_LENGHT];
}aes_transit_t;

typedef struct aes_cipher{
	aes_setkeyiv _setkeyiv;
	aes_encrypt_decrypt _encrypt;
	aes_encrypt_decrypt _decrypt;
}aes_cipher_t;

void wy_cipher_setkeyiv(H_U32 mode, H_U32 len, const H_U8 *key, const H_U8 *init_vector);
void wy_cipher_encrypt(H_U8 *in, H_U8 *out);
void wy_cipher_decrypt(H_U8 *in, H_U8 *out);

#endif

