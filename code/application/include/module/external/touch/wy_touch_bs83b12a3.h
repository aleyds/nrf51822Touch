#ifndef __TOUCH_BS83B12A3_H__
#define __TOUCH_BS83B12A3_H__

#define BS83B12A3_SLAVE_ADDR				(0x50<<1)		//оƬI2C�ӵ�ַ
#define BS83B12A3_READ_ADDR1				(0x08)			//оƬ�����ݵ�ַ
#define BS83B12A3_READ_ADDR2				(0x09)			//оƬ�����ݵ�ַ

#define BS83B12A3_READ_ADDR_MIN			(0xB0)			//оƬ���üĴ���
#define BS83B12A3_READ_ADDR_MAX			(0xC0)			//оƬ���üĴ���

#define IRQ_OMS_HOLD					(0x0)			//Level Hold
#define IRQ_OMS_SHOT					(0x1)			//One shot

#define LSC_NORMAL						(0x0)			//һ��ʡ��ģʽ
#define LSC_MORE						(0x1)			//��ʡ��ģʽ(����ʱ��������0.5-1��)

#define KnWU_ENABLE						(0x0)			//�����������ѹ���
#define KnWU_DISABLE					(0x1)			//�رհ������ѹ���



/*���̰����ϱ�����*/
enum touch8112a3_value{
	_TOUCH83B12A3_VALUE_NULL,
	_TOUCH83B12A3_VALUE_NUM0 = 0x0008,
	_TOUCH83B12A3_VALUE_NUM1 = 0x0200,
	_TOUCH83B12A3_VALUE_NUM2 = 0x1000,
	_TOUCH83B12A3_VALUE_NUM3 = 0x4000,
	_TOUCH83B12A3_VALUE_NUM4 = 0x0100,
	_TOUCH83B12A3_VALUE_NUM5 = 0x2000,
	_TOUCH83B12A3_VALUE_NUM6 = 0x8000,
	_TOUCH83B12A3_VALUE_NUM7 = 0x0080,
	_TOUCH83B12A3_VALUE_NUM8 = 0x0004,
	_TOUCH83B12A3_VALUE_NUM9 = 0x0001,
	_TOUCH83B12A3_VALUE_NUMX = 0x0040,//����*
	_TOUCH83B12A3_VALUE_NUMH = 0x0002,//����#
};

H_U32 m2mkey_bs83b12a3_open(void);
H_U32 m2mkey_bs83b12a3_obtain(void);


#endif

