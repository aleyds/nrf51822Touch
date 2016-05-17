
#include <base_type.h>
#include <aes_decrypt.h>
#include <nrf_soc.h>
#include <nrf_ecb.h>
#include "wy_uart.h"
#include <wy_aes_cipher.h>

static aes_transit_t g_paes_transit;

static aes_transit_t* __get_aes_transit(void)
{
	return (aes_transit_t*)&g_paes_transit;
}

void wy_cipher_setkeyiv(H_U32 mode, H_U32 len, const H_U8 *key, const H_U8 *init_vector)
{
	memset(&g_paes_transit, 0, sizeof(aes_transit_t));
	g_paes_transit.type = mode;
	g_paes_transit.len = len;
	memcpy(g_paes_transit.key, key, AES_CIPHER_LENGHT);
	memcpy(g_paes_transit.init_vector, init_vector, AES_CIPHER_LENGHT);
}

static void __get_cleartxt(aes_transit_t *ptransit, const H_U8 *value, H_U8 *out)
{
	switch(ptransit->type)
	{
		case AES_ECB_NORMAL_TYPE:
			memcpy(out, value , AES_CIPHER_LENGHT);
			break;
		case  AES_ECB_XOR_TYPE:
			{
				for(H_U32 i = 0; i < AES_CIPHER_LENGHT; i++)
				{
					out[i] = (value[i] ^ ptransit->init_vector[i]);
				}
			}
			break;
		case AES_CBC_TYPE:
			break;
		default:
			memcpy(out, value, AES_CIPHER_LENGHT);
			break;
	}
}

void wy_cipher_encrypt(H_U8 *in, H_U8 *out)
{
	nrf_ecb_hal_data_t ecb_dat;
	H_U8 clear_txt[AES_CIPHER_LENGHT] = {0};
	aes_transit_t *paes_transit = __get_aes_transit();

	wy_memset(&ecb_dat, 0, sizeof(nrf_ecb_hal_data_t));
	__get_cleartxt(paes_transit, in, clear_txt);
	wy_memcpy(ecb_dat.key, paes_transit->key , AES_CIPHER_LENGHT);
	wy_memcpy(ecb_dat.cleartext, clear_txt , AES_CIPHER_LENGHT);
	sd_ecb_block_encrypt(&ecb_dat);
	wy_memcpy(out, ecb_dat.ciphertext, AES_CIPHER_LENGHT);
}


void wy_cipher_decrypt(H_U8 *in, H_U8 *out)
{
	//H_U8 clear_txt[AES_CIPHER_LENGHT] = {0};
	aes_transit_t *paes_transit = __get_aes_transit();
	H_U8 clear_txt[AES_CIPHER_LENGHT] = {0};
	
	decryptions(in, paes_transit->key, clear_txt);
	__get_cleartxt(paes_transit, clear_txt, out);
}

