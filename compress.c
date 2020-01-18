/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                           az100 compress libary
*
*                                    (c) Copyright 2011-2014, Sunny China
*                                             All Rights Reserved
*
* File    : compress.c
* By      : Sunny
* Version : v1.0
* Date    : 2011-4-11
* Descript: compress wapper interfaces.
* Update  : date                auther      ver     notes
*           2011-4-11 14:29:31  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "az100_i.h"
#include "azlzma/LzmaLib.h"
#include "orlzma/LzmaLib.h"
#include <stdio.h>
#include <unistd.h>

/*
*********************************************************************************************************
*                                       FULL FUNCTION NAME
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Note       :
*********************************************************************************************************
*/
__s32 AZ100_DataUncompress(__u8 *pDst, __u32 DstLen, __u8 *pSrcData, __u32 SrcLen)
{
    __u8   *pCompressData;
    int status;

    //uncompress data
    pCompressData  = pSrcData + LZMA_PROPS_SIZE;
    SrcLen        -= (LZMA_PROPS_SIZE);
    if (status = LzmaUncompress(pDst, &DstLen, \
                       pCompressData, &SrcLen, \
                       pSrcData, LZMA_PROPS_SIZE))
    {
        printf("lzma uncompress data failed error: %i\n", status);
        return EPDK_FAIL;
    }
    return EPDK_OK;
}

__s32 AZ100_DataCompress(__u8 *pDst, __u32* DstLen, __u8 *pSrcData, __u32 SrcLen)
{
    __u8   *pCompressData;
    size_t props_len;
    void *pProps;

    //compress data
    props_len = LZMA_PROPS_SIZE;
    pProps  = malloc(LZMA_PROPS_SIZE + 1);

    if (ORLzmaCompress(pDst + LZMA_PROPS_SIZE, DstLen, \
                       pSrcData, SrcLen, \
                       pProps, &props_len, 1,
                       (1 << 16), 3, 0, 2, 32 , 1))
    {
        printf("lzma compress data failed\n");
        return EPDK_FAIL;
    }

    memccpy(pDst, pProps, 1, LZMA_PROPS_SIZE);
    return EPDK_OK;
}
