/*********************************************************************
文件描述:基本数据类型定义,为方便编程
**********************************************************************/

#ifndef __BASE_TYPE_H__
#define __BASE_TYPE_H__
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <config.h>



/**************************基本数据类型***********************************/
typedef int8_t H_S8;
typedef uint8_t H_U8;
typedef int16_t H_S16;
typedef uint16_t H_U16;
typedef int32_t H_S32;
typedef uint32_t H_U32;
typedef bool H_BOOL;
typedef char H_CHAR;
typedef unsigned char H_UCHAR;
typedef short	H_SHORT;
 
/***********************基本返回类型*************************************/
#define H_TRUE			(1==1)
#define H_FALSE			(0==1)
#define H_SUCCESS		(0)
#define H_FAULTER		(~0)
#define H_NULL			(0)
#define __EXTERN		extern

/***********************通用函数指针***************************************/
typedef void (*function_void)(void);
typedef H_U32 (*uint32_function)(void);
typedef H_U32 (*uint32_function_uint32)(H_U32);


/***********************函数参数检查**************************************/
#define CHECK_PARAMETER(in,error,ret)\
do{\
	if(in == error)\
	{\
		WYPrintf("CHECK",LEVEL_ERROR,"check param error");\
		return ret;\
	}\
}while(0==1);

#define CHECK_RETURN(in,error,ret)\
do{\
	if(in < error)\
	{\
		WYPrintf("CHECK",LEVEL_ERROR,"CHECK_RETURN error");\
		return ret;\
	}\
	\
}while(0);


/**********************数据组装*******************************************/
#define decode32(in,offset)\
	(((in[offset]<<24)&0xff000000) | ((in[offset+1]<<16)&0xff0000) | ((in[offset+2]<<8)&0xff00) | ((in[offset+3])&0xff))

#define decode_turn32(in,offset)\
	(((in[offset])&0xff) | ((in[offset+1]<<8)&0xff00) | ((in[offset+2]<<16)&0xff0000) | ((in[offset+3]<<24)&0xff000000))

#define decode16(in,offset)\
	(((in[offset]<<8)&0xff00) | ((in[offset+1])&0xff))

#define decode_turn16(in,offset)\
	(((in[offset])&0xff) | ((in[offset+1]<<8)&0xff00))

#define decode8(in,offset)\
	((in[offset])&0xff)

#define encode32(code,out,offset)\
do{\
	     out[offset] = ((code >> 24)&0xff);\
	     out[offset + 1] = ((code >> 16)&0xff);\
	     out[offset + 2] = ((code >> 8)&0xff);\
	     out[offset + 3] = ((code )&0xff);\
	}while(0==1);

#define encode16(code,out,offset)\
do{\
	     out[offset] = ((code>>8)&0xff);\
	     out[offset + 1] = ((code)&0xff);\
	}while(0==1);

#define encode8(code,out,offset)\
do{\
	     out[offset] = ((code)&0xff);\
	}while(0==1);

#define encode_turn32(code,out,offset)\
do{\
	     out[offset] = ((code)&0xff);\
	     out[offset + 1] = ((code >> 8)&0xff);\
	     out[offset + 2] = ((code >> 16)&0xff);\
	     out[offset + 3] = ((code >>24)&0xff);\
	}while(0==1);

#define encode_turn16(code,out,offset)\
do{\
	     out[offset] = ((code)&0xff);\
	     out[offset + 1] = ((code>>8)&0xff);\
}while(0==1);

#define byte16_turn(data)\
		((data>>8)&0xff) | ((data&0xff)<<8)

#define byte32_turn(data)\
		(((data>>24)&0xff) | (((data>>16)&0xff)<<8)|(((data>>8)&0xff)<<16)|(((data)&0xff)<<24))


/******************String mem 封装********************************/
#define wy_memset(x,y,z)   memset((void *)x,y,z)
#define wy_memcpy(x,y,z)   memcpy((void *)x, (const void *)y, z)
#define wy_memcmp(x,y,z)   memcmp((const void *)x, (const void *)y, z)
#define wy_strlen(x)       strlen((const char *)x)
#define wy_strcmp(x,y)     strcmp((const char *)x, (const char *)y)
#define wy_strcpy(x,y)     strcpy((char *)x, (const char *)y)
#define wy_strncpy(x,y,z)  strncpy((char *)x, (const char *)y, z)
#define wy_strstr(x,y)     strstr((const char *)x, (const char *)y)
#define wy_strncmp(x,y,z)  strncmp((const char *)x, (const char *)y, z)
#define wy_strcat(x,y)     strcat((char *)x, (const char *)y)
#define wy_malloc(size)	   malloc(size)
#define wy_free(ptr)	   free(ptr)
#define wy_array_len(arr)	(sizeof(arr)/sizeof(arr[0]))		

#endif

