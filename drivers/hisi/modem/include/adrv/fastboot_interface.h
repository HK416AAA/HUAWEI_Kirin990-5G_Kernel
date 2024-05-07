/*************************************************************************
*   ��Ȩ����(C) 1987-2015, ���ں�˼�뵼�弼�����޹�˾.
*
*   �� �� �� :  hiboot_interface.h
*
*   ��    �� :  ���ļ�����Ϊ"hiboot_interface.h", ����hiboot�����ֱ�Ӷ����ṩAPI�ӿ�����
*
*   �޸ļ�¼ :  2015��4��01��  v1.00  ����
*************************************************************************/

#ifndef HISI_HIBOOT_DRV_H
#define HISI_HIBOOT_DRV_H

/*************************************************************************
*
*   �궨��
*
*************************************************************************/
#define PARTITION_MODEM_NVDLOAD_NAME        "modemnvm_update"
#define PARTITION_MODEM_NVDLOAD_CUST_NAME   "modemnvm_cust"
#define PARTITION_MODEM_NVDEFAULT_NAME      "modemnvm_factory"
#define PARTITION_MODEM_NVBACKUP_NAME       "modemnvm_backup"
#define PARTITION_MODEM_NVSYS_NAME          "modemnvm_system"
#define PARTITION_MODEM_NVIMG_NAME          "modemnvm_img"

#ifndef FEATURE_NV_SEC_ON
typedef struct ptentry ptentry;
struct ptentry
{
    char name[32];
    unsigned start;
    unsigned length;
    unsigned flags;
};
#endif
/*****************************************************************************
* �� �� ��  : get_operators
*
* ��������  : ��ȡ�����Ļص�����
*
* �������  : string ����"partition"
*             
* �������  : ��
*
* �� �� ֵ  : ��
*
* ����˵��  : hiboot
*
*****************************************************************************/
void * get_operators(char * string);

/*****************************************************************************
* �� �� ��  : storage_read
*
* ��������  : HIBOOT�ж�ȡ��Ӧ������ָ��λ�õ�����
*
* �������  : ptn_name ��Ҫ�����ķ�����
			  offset  ��Է�����ʼ��ַ��ƫ��
			  data �����ݴ�ŵ�Ŀ��buffer
			  bytes  ��Ҫ��ȡ���ֽ���
*             
* �������  : ��
*
* �� �� ֵ  : 0  ��ȡ�ɹ�
			  -1 ��ȡʧ��
*
* ����˵��  : hiboot
*
*****************************************************************************/
int storage_read(const char *ptn_name, unsigned long long offset, void *data, unsigned long long bytes);

/*****************************************************************************
* �� �� ��  : Is_load_modem 
*
* ��������  : �Ƿ����modem
*
* �������  : NULL
*             
* �������  : NULL
*
* �� �� ֵ  : 0  ʧ��
			    1 ��Ҫ����modem��dsp
*
* ����˵��  : hiboot
*
*****************************************************************************/
unsigned int bsp_need_loadmodem (void);

/*****************************************************************************
* �� �� ��  : bsp_socpmem_enable
*
* ��������  : �Ƿ�̬Ԥ��modem socp���ڴ�
*
* �������  : NULL
*
* �������  : NULL
*
* �� �� ֵ  : 0 ����̬Ԥ��modem socp���ڴ�
              1 ��̬Ԥ��modem socp���ڴ�
*
* ����˵��  : hiboot
*
*****************************************************************************/
unsigned int bsp_socpmem_enable(void);

#endif /* HISI_AP_DRV_H */

