

#include "at_atp_proc.h"

#include "product_config.h"
#include "PsLogdef.h"
#include "AtParse.h"
#include "at_parse_core.h"
#include "at_mntn.h"
#include "at_private.h"
#include "at_cmd_proc.h"
#include "at_input_proc.h"
#include "dms_port_i.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID PS_FILE_ID_AT_ATP_PROC_C

struct AT_ATP_ReturnCodeMap {
    ATP_ReturnCodeUint32 atpCode;
    AT_RreturnCodeUint32 atCode;
};

AT_ParCmdElement  *g_atAtpCmdTbl = VOS_NULL_PTR;
VOS_UINT32         g_atAtpCmdNum = 0;

/* ATP AT 返回码对应表 */
STATIC const struct AT_ATP_ReturnCodeMap g_atAtpRetCodeMap[] = {
    { ATP_SUCCESS, AT_SUCCESS },
    { ATP_FAILURE, AT_FAILURE },

    { ATP_BASIC_CODE_ENUM_BEGAIN, AT_BASIC_CODE_ENUM_BEGAIN },
    { ATP_OK, AT_OK },
    { ATP_ERROR, AT_ERROR },
    { ATP_BASIC_CODE_ENUM_END, AT_BASIC_CODE_ENUM_END },

    { ATP_CME_ERROR_ENUM_BEGAIN, AT_CME_ERROR_ENUM_BEGAIN },
    { ATP_CME_MEMORY_FAILURE,           AT_CME_MEMORY_FAILURE},
    { ATP_CME_INCORRECT_PARAMETERS, AT_CME_INCORRECT_PARAMETERS },
    { ATP_CME_ERROR_ENUM_END, AT_CME_ERROR_ENUM_END },

    { ATP_DEVICE_ERROR_BEGIN,           AT_DEVICE_ERROR_BEGIN},
    { ATP_DATA_UNLOCK_ERROR,            AT_DATA_UNLOCK_ERROR},
    { ATP_DEVICE_OTHER_ERROR,           AT_DEVICE_OTHER_ERROR},
    { ATP_DEVICE_NOT_SUPPORT,           AT_DEVICE_NOT_SUPPORT},
    { ATP_DEVICE_ERR_UNKNOWN,           AT_DEVICE_ERR_UNKNOWN},
    { ATP_DEVICE_INVALID_PARAMETERS,    AT_DEVICE_INVALID_PARAMETERS},
    { ATP_DEVICE_NV_WRITE_FAIL_UNKNOWN, AT_DEVICE_NV_WRITE_FAIL_UNKNOWN},
    { ATP_DEVICE_NV_READ_FAILURE,       AT_DEVICE_NV_READ_FAILURE},
    { ATP_DEVICE_INVALID_OP,            AT_DEVICE_INVALID_OP},
    { ATP_DEVICE_NOT_ENOUGH_MEMORY,     AT_DEVICE_NOT_ENOUGH_MEMORY},
    { ATP_DEVICE_ERROR_END,             AT_DEVICE_ERROR_END},

    { ATP_CMD_NOT_SUPPORT, AT_CMD_NOT_SUPPORT },
    { ATP_RETURN_CODE_BUTT, AT_RRETURN_CODE_BUTT }
};

AT_ParCmdElement *AT_GetAtpCmdTbl(VOS_VOID)
{
    return g_atAtpCmdTbl;
}

VOS_VOID AT_SetAtpCmdTbl(AT_ParCmdElement *tbl)
{
    g_atAtpCmdTbl = tbl;
}

VOS_UINT32 *AT_GetAtpCmdCount(VOS_VOID)
{
    return &g_atAtpCmdNum;
}

STATIC AT_RreturnCodeUint32 AT_GetRetCodeFromAtp(ATP_ReturnCodeUint32 atpCode)
{
    for (VOS_UINT32 i = 0; i < ATP_RETURN_CODE_BUTT; i++) {
        if (g_atAtpRetCodeMap[i].atpCode == atpCode) {
            return g_atAtpRetCodeMap[i].atCode;
        }
    }

    return AT_RRETURN_CODE_BUTT;
}

VOS_BOOL AT_IsMsgBuffValid(VOS_CHAR *buffer, VOS_UINT32 bufLen, VOS_UINT32 maxBufLen,
    VOS_UINT32 totalLen, VOS_UINT32 structSize)
{
    if (bufLen > maxBufLen) {
        AT_PR_LOGE("bufLen too long %d", bufLen);
        return VOS_FALSE;
    }

    if (structSize + bufLen != totalLen) {
        AT_PR_LOGE("length abnormal, len:%d, buflen:%d", totalLen, bufLen);
        return VOS_FALSE;
    }

    if (bufLen == 0) {
        return VOS_TRUE;
    }

    if (buffer[bufLen - 1] != '\0') {
        AT_PR_LOGE("the last char not the terminating null byte");
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

STATIC VOS_UINT32 AT_FillAtpCmdName(struct AT_ATP_CmdReq *request, VOS_UINT8 clientId)
{
    AT_ParseContext *parseContext = AT_GetParseContext(clientId);
    VOS_CHAR        *cmdName = (VOS_CHAR *)parseContext->cmdElement->cmdName;
    VOS_UINT32       cmdNameLen;
    errno_t          result;

    if (cmdName == VOS_NULL_PTR) {
        AT_PR_LOGE("cmdName is null.");
        return VOS_ERR;
    }

    cmdNameLen = VOS_StrLen(cmdName);
    if (cmdNameLen > MAX_ATP_CMD_NAME_LEN) {
        AT_PR_LOGE("cmdName length is %d, too long.", cmdNameLen);
        return VOS_ERR;
    }
    result = strncpy_s((VOS_CHAR *)request->cmdName, MAX_ATP_CMD_NAME_LEN, cmdName, cmdNameLen);
    TAF_STRCPY_CHK_RTN_VAL_CONTINUE(result, MAX_ATP_CMD_NAME_LEN, cmdNameLen);
    AT_PR_LOGD("name len: %d, name: %s", cmdNameLen, (VOS_CHAR *)request->cmdName);

    return VOS_OK;
}

STATIC VOS_UINT32 AT_FillAtpCmdPara(struct AT_ATP_CmdReq *request, VOS_UINT32 *poolPos)
{
    AT_ParseParaType *paraList = VOS_NULL_PTR;
    VOS_UINT8        *paraPool = VOS_NULL_PTR;
    const VOS_UINT32  poolLen = MAX_AT_ATP_CMD_REQ_LEN - (sizeof(struct AT_ATP_MsgHead) + sizeof(*request));
    errno_t           memResult;
    VOS_UINT8         paraIndex = AT_GetParaIndex();
    VOS_UINT8         *para = VOS_NULL_PTR;

    if (paraIndex > MAX_ATP_PARA_NUMBER) {
        AT_PR_LOGE("%d para is too many.", paraIndex);
        return VOS_ERR;
    }

    paraPool = (VOS_UINT8 *)request->paraPool;

    for (VOS_UINT32 i = 0; i < paraIndex; i++) {
        paraList = AT_GetParaList((VOS_UINT8)i);
        if (*poolPos + paraList->paraLen > poolLen) {
            AT_PR_LOGE("poolPos %d, paraid %d, paralen %d.", *poolPos, i, paraList->paraLen);
            return VOS_ERR;
        }

        if (paraList->paraLen == 0) {
            request->paraList[i].paraValue = paraList->paraValue;
            request->paraList[i].paraLen   = 0;
            request->paraList[i].offsetInPool = 0;
            AT_PR_LOGD("value: %d", paraList->paraValue);
            continue;
        }

        request->paraList[i].paraValue = paraList->paraValue;
        request->paraList[i].paraLen   = paraList->paraLen;
        request->paraList[i].offsetInPool = *poolPos;

        memResult = memcpy_s(paraPool + *poolPos, poolLen - *poolPos, paraList->para, paraList->paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, poolLen - *poolPos, paraList->paraLen);
        *poolPos += paraList->paraLen;

        paraPool[*poolPos] = '\0';
        (*poolPos)++;

        para = paraPool + request->paraList[i].offsetInPool;
        AT_PR_LOGD("value: %d, len: %d, offset: %d, para: %s", paraList->paraValue, paraList->paraLen,
            request->paraList[i].offsetInPool, para);
    }

    return VOS_OK;
}

STATIC VOS_UINT32 AT_SendAtpReq(VOS_UINT8 clientId, ATP_CmdTypeUint8 type)
{
    static VOS_INT8         atpCmdReqBuf[MAX_AT_ATP_CMD_REQ_LEN];
    static VOS_UINT32       atMagicRand = MIN_ATP_CMD_MAGIC_LEN;

    struct AT_ATP_MsgHead  *msgHead = VOS_NULL_PTR;
    struct AT_ATP_CmdReq   *request = VOS_NULL_PTR;
    VOS_UINT32              poolPos = 0;
    VOS_UINT32              result;

    if (clientId >= AT_MAX_CLIENT_NUM) {
        AT_PR_LOGE("clientId is %d.", clientId);
        return AT_ERROR;
    }

    (VOS_VOID)memset_s(atpCmdReqBuf, sizeof(atpCmdReqBuf), 0, sizeof(atpCmdReqBuf));
    msgHead = (struct AT_ATP_MsgHead *)atpCmdReqBuf;
    request = (struct AT_ATP_CmdReq *)msgHead->data;

    result = AT_FillAtpCmdName(request, clientId);
    if (result == VOS_ERR) {
        AT_PR_LOGE("fill atp cmdName error.");
        return AT_ERROR;
    }

    result = AT_FillAtpCmdPara(request, &poolPos);
    if (result == VOS_ERR) {
        AT_PR_LOGE("fill atp para error.");
        return AT_ERROR;
    }

    /* 魔术字, 累加越界后复原 */
    if ((atMagicRand++) >= MAX_ATP_CMD_MAGIC_LEN) {
        AT_PR_LOGI("atp cmd magic overflow.");
        atMagicRand = MIN_ATP_CMD_MAGIC_LEN;
    }

    request->magic      = atMagicRand;
    request->atClientId = clientId;
    request->cmdType    = type;
    request->paraCount  = AT_GetParaIndex();
    request->poolLen    = poolPos;
    AT_PR_LOGD("magic: %d, clientId: %d, type: %d, paraCount: %d, poolLen: %d",
        atMagicRand, clientId, type, request->paraCount, poolPos);

    msgHead->msgId = ID_AT_ATP_CMD_REQ;
    msgHead->len   = sizeof(*request) + poolPos;
    AT_MntnTraceAtAtpCmdReq((VOS_UINT8 *)msgHead, sizeof(*msgHead) + msgHead->len);
    result = DMS_PORT_SendData(DMS_PORT_ATP, (VOS_UINT8 *)msgHead, (VOS_UINT16)(sizeof(*msgHead) + msgHead->len));
    if (result != VOS_OK) {
        return AT_ERROR;
    }

    /* 设置当前操作模式 */
    AT_ClientManage *clientManage = AT_GetClientManage(clientId);
    clientManage->magic = atMagicRand;
    clientManage->cmdCurrentOpt = AT_CMD_ATP_OPT;

    return AT_WAIT_ASYNC_RETURN;
}

STATIC VOS_UINT32 AT_SendAtpSetReq(VOS_UINT8 clientId)
{
    return AT_SendAtpReq(clientId, ATP_CMD_TYPE_SET);
}

STATIC VOS_UINT32 AT_SendAtpQryReq(VOS_UINT8 clientId)
{
    return AT_SendAtpReq(clientId, ATP_CMD_TYPE_QUERY);
}

STATIC VOS_UINT32 AT_SendAtpTestReq(VOS_UINT8 clientId)
{
    return AT_SendAtpReq(clientId, ATP_CMD_TYPE_TEST);
}

STATIC VOS_UINT32 AT_SendAtpAbortReq(VOS_UINT8 clientId)
{
    return AT_SendAtpReq(clientId, ATP_CMD_TYPE_ABORT);
}

STATIC VOS_INT32 AT_RcvAtpStream(VOS_UINT16 clientId, VOS_UINT8 *data, VOS_UINT32 len)
{
    struct AT_ATP_MsgHead   *atpMsg = (struct AT_ATP_MsgHead *)data;
    AT_Msg       *msg = TAF_NULL_PTR;
    VOS_UINT_PTR  tmpAddr;
    VOS_UINT32    length;
    errno_t       memResult;

    if (len < sizeof(*atpMsg) || len != atpMsg->len + sizeof(*atpMsg)) {
        AT_PR_LOGE("ATP data len abnormal: %d.", len);
        return VOS_ERR;
    }

    if (len > MAX_AT_ATP_MSG_LEN) {
        AT_PR_LOGE("ATP stream len is %d, too long.", len);
        return VOS_ERR;
    }

    /* 增加自定义的ITEM，共4个字节 */
    length = AT_GetAtMsgStruMsgLength(len);
    msg = (AT_Msg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);
    if (msg == TAF_NULL_PTR) {
        AT_PR_LOGE("Alloc Msg");
        return VOS_ERR;
    }

    TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, WUEPS_PID_AT, length);

    msg->msgId = ID_AT_ATP_PORT;
    msg->type  = AT_ATP_MSG;        /* 类型 */
    msg->len   = (VOS_UINT16)len;

    /* 以下信息当前不填 */
    msg->indexNum  = 0;
    msg->modemId   = 0;
    msg->versionId = 0;
    msg->filterAtType = 0;
    msg->userType     = 0;

    /* 填写新消息内容 */
    tmpAddr   = (VOS_UINT_PTR)(msg->value);
    memResult = memcpy_s((VOS_UINT8 *)tmpAddr, len, data, len);
    TAF_MEM_CHK_RTN_VAL(memResult, len, len);

    /* 发送消息到AT_PID */
    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_PR_LOGE("AT_RcvAtpStream:ERROR:VOS_SendMsg");
        return VOS_ERR;
    }

    return VOS_OK;
}

STATIC VOS_UINT8 *AT_SaveAtpStreamStr(const VOS_UINT8 *stream, VOS_UINT32 *streamPos,
    VOS_UINT8 *cmdPool, VOS_UINT32 *poolPos, VOS_UINT32 len)
{
    VOS_UINT8 *dst = cmdPool + *poolPos;
    errno_t    memResult;

    memResult = memcpy_s(dst, len, stream + *streamPos, len);
    TAF_MEM_CHK_RTN_VAL(memResult, len, len);
    AT_PR_LOGD("str is: %s", dst);

    *poolPos += len;
    *streamPos += len;
    return dst;
}

STATIC VOS_VOID AT_FillAtCmdElem(AT_ParCmdElement *cmd, const struct ATP_AT_CmdElementStream *stream)
{
    cmd->setTimeOut = stream->setTimeOut;
    cmd->qryTimeOut = stream->qryTimeOut;
    cmd->testTimeOut = stream->testTimeOut;
    cmd->abortTimeOut = stream->abortTimeOut;
    cmd->paramErrCode = AT_GetRetCodeFromAtp(stream->returnCode);
    cmd->chkFlag = stream->chkFlag;
    cmd->cmdIndex = AT_CMD_ATP;
    AT_PR_LOGD("setLen: %d, qryLen: %d, testLen: %d, abortLen: %d, errCode: %d, chkFlg: %d",
        cmd->setTimeOut, cmd->qryTimeOut, cmd->testTimeOut, cmd->abortTimeOut, cmd->paramErrCode, cmd->chkFlag);

    cmd->setProc   = (stream->setFuncFlg   != 0) ? AT_SendAtpSetReq   : VOS_NULL_PTR;
    cmd->qryProc   = (stream->qryFuncFlg   != 0) ? AT_SendAtpQryReq   : VOS_NULL_PTR;
    cmd->testProc  = (stream->testFuncFlg  != 0) ? AT_SendAtpTestReq  : VOS_NULL_PTR;
    cmd->abortProc = (stream->abortFuncFlg != 0) ? AT_SendAtpAbortReq : VOS_NULL_PTR;
    AT_PR_LOGD("setFlg: %d, qryFlg: %d, testFlg: %d, abortFlg: %d",
        stream->setFuncFlg, stream->qryFuncFlg, stream->testFuncFlg, stream->abortFuncFlg);
}

STATIC VOS_UINT32 AT_CheckAtpRegStreamLen(struct ATP_AT_CmdElementStream *streamElem, VOS_UINT32 pos, VOS_UINT32 len)
{
    /*
     * 码流构造时，命令名和参数限制字符串之后都多加一个0，计算偏移量时需要 len + 1
     * 如果参数字符串为空，则不多加0，
     * 即命令名字符串之后仅有1个0，之后码流结束或紧接着下一个命令的信息
     */
    if (streamElem->paraLen == 0) {
        if (pos + (streamElem->nameLen + 1) > len) {
            return VOS_ERR;
        }
    } else {
        if (pos + (streamElem->nameLen + 1) + (streamElem->paraLen + 1) > len) {
            return VOS_ERR;
        }
    }

    return VOS_OK;
}

STATIC VOS_UINT32 AT_GenerateAtpCmdTbl(VOS_UINT8 *destCmdTbl, const struct ATP_AT_RegCmdTblReq *atpInfo,
    VOS_UINT32 streamLen, VOS_UINT8 *cmdPool, VOS_UINT32 poolLen)
{
    struct ATP_AT_CmdElementStream *streamElem = VOS_NULL_PTR;
    const VOS_UINT8                *stream = atpInfo->cmdStream;
    AT_ParCmdElement                cmdElem;
    VOS_UINT32                      streamPos = 0;
    VOS_UINT32                      poolPos = 0;
    errno_t                         result;
    VOS_UINT16                      count = 0;

    while (streamPos + sizeof(*streamElem) < streamLen) {
        streamElem = (struct ATP_AT_CmdElementStream *)(stream + streamPos);
        (VOS_VOID)memset_s((VOS_VOID *)&cmdElem, sizeof(cmdElem), 0, sizeof(cmdElem));

        if (count >= atpInfo->cmdNum) {
            AT_PR_LOGE("stream exception. cmdNum is %d, too less.", atpInfo->cmdNum);
            break;
        }

        if (streamElem->nameLen == 0) {
            AT_PR_LOGE("stream exception. nameLen is null, generate %d cmd.", count);
            break;
        }

        result = (errno_t)AT_CheckAtpRegStreamLen(streamElem, poolPos, poolLen);
        if (result == VOS_ERR) {
            AT_PR_LOGE("stream exception. nameLen is %d, paraLen is %d.", streamElem->nameLen, streamElem->paraLen);
            return VOS_ERR;
        }

        AT_FillAtCmdElem(&cmdElem, streamElem);
        streamPos += sizeof(*streamElem);

        cmdElem.cmdName = AT_SaveAtpStreamStr(stream, &streamPos, cmdPool, &poolPos, streamElem->nameLen + 1);

        if (streamElem->paraLen == 0) {
            cmdElem.param = VOS_NULL_PTR;
        } else {
            cmdElem.param = AT_SaveAtpStreamStr(stream, &streamPos, cmdPool, &poolPos, streamElem->paraLen + 1);
        }

        result = memcpy_s(destCmdTbl + count * sizeof(cmdElem), sizeof(cmdElem), &cmdElem, sizeof(cmdElem));
        TAF_MEM_CHK_RTN_VAL(result, sizeof(cmdElem), sizeof(cmdElem));

        count++;
    }

    if (streamPos > streamLen) {
        AT_PR_LOGE("stream exception. %d byte stream was parsed, but streamLen is %d.", streamPos, streamLen);
        return VOS_ERR;
    }

    if (count == 0) {
        AT_PR_LOGE("stream exception. None stream was parsed.");
        return VOS_ERR;
    }

    return VOS_OK;
}

STATIC VOS_VOID AT_ProcRegAtpCmdTblReq(const struct ATP_AT_RegCmdTblReq *atpInfo, VOS_UINT32 len)
{
    AT_ParCmdElement *cmdTbl = AT_GetAtpCmdTbl();
    VOS_UINT32       *cmdNum = AT_GetAtpCmdCount();
    VOS_UINT8        *atpCmdTbl = VOS_NULL_PTR;
    VOS_UINT8        *cmdPool = VOS_NULL_PTR;
    VOS_UINT32        atpCmdTblLen;
    VOS_UINT32        poolLen;
    VOS_UINT32        streamLen;
    VOS_UINT32        ret;

    AT_PR_LOGI("entry, req len is %d", len);

    if (cmdTbl != VOS_NULL_PTR) {
        AT_PR_LOGE("cmdTbl is already registered.");
        return;
    }

    if (len <= sizeof(*atpInfo)) {
        AT_PR_LOGE("ATP cmdStream is null.");
        return;
    }
    streamLen = len - sizeof(atpInfo->cmdNum);

    if (streamLen <= atpInfo->cmdNum * sizeof(struct ATP_AT_CmdElementStream)) {
        AT_PR_LOGE("ATP cmdStream exception. cmdNum is %d, too many.", atpInfo->cmdNum);
        return;
    }
    /* cmdPool保存cmdName和para校验参数字符串 */
    poolLen = streamLen - atpInfo->cmdNum * sizeof(struct ATP_AT_CmdElementStream);
    cmdPool = (VOS_UINT8 *)AT_MALLOC(poolLen);
    if (cmdPool == VOS_NULL_PTR) {
        AT_PR_LOGE("malloc cmdPool error.");
        return;
    }
    (VOS_VOID)memset_s(cmdPool, poolLen, 0, poolLen);

    atpCmdTblLen = atpInfo->cmdNum * sizeof(AT_ParCmdElement);
    atpCmdTbl = (VOS_UINT8 *)AT_MALLOC(atpCmdTblLen);
    if (atpCmdTbl == VOS_NULL_PTR) {
        AT_PR_LOGE("malloc atpCmdTbl error.");
        AT_FREE(cmdPool);
        return;
    }
    (VOS_VOID)memset_s(atpCmdTbl, atpCmdTblLen, 0, atpCmdTblLen);

    ret = AT_GenerateAtpCmdTbl(atpCmdTbl, atpInfo, streamLen, cmdPool, poolLen);
    if (ret == VOS_ERR) {
        AT_FREE(atpCmdTbl);
        AT_FREE(cmdPool);
        return;
    }

    AT_SetAtpCmdTbl((AT_ParCmdElement *)atpCmdTbl);
    cmdTbl = AT_GetAtpCmdTbl();
    *cmdNum = atpInfo->cmdNum;

    AT_NORM_LOG("_________AT_RegisterCmdTable in atproxy_register_to_at_____________");
    AT_RegisterCmdTable(cmdTbl, (VOS_UINT16)(*cmdNum));
}

 VOS_VOID AT_ReplaceStr(VOS_CHAR* dst, VOS_UINT32 dstLen,
    VOS_CHAR* src, const VOS_CHAR* srch, const VOS_CHAR* rep)
{
    TAF_CHAR  *srcPos = src;
    TAF_CHAR  *cur    = NULL;
    TAF_CHAR  *dstPos = dst;
    TAF_UINT32 repLen;
    TAF_UINT32 srchLen;
    TAF_INT32  nLen = 0;
    errno_t    result;

    repLen  = strlen(rep);
    srchLen = strlen(srch);

    do {
        cur = strstr(srcPos, srch);
        if (cur == NULL) {
            result = strcpy_s(dstPos, (TAF_UINT32)(dstLen - (dstPos - dst)), srcPos);
            TAF_STRCPY_CHK_RTN_VAL_CONTINUE(result, (TAF_UINT32)(dstLen - (dstPos - dst)),
                (TAF_UINT32)(dstLen - (dstPos - dst)));
            return;
        }

        /* 拷贝上一个替换点和下一个替换点中间的字符串 */
        nLen = cur - srcPos;
        if (nLen > 0) {
            result = memcpy_s(dstPos, (TAF_UINT32)(dstLen - (dstPos - dst)), srcPos, (TAF_UINT32)nLen);
            TAF_MEM_CHK_RTN_VAL(result, (TAF_UINT32)(dstLen - (dstPos - dst)), (TAF_UINT32)nLen);
        }
        /* 拷贝需要替换的字符串 */
        if (repLen > 0) {
            result = memcpy_s(dstPos + nLen, (TAF_UINT32)(dstLen - (dstPos - dst) - nLen), rep, repLen);
            TAF_MEM_CHK_RTN_VAL(result, (TAF_UINT32)(dstLen - (dstPos - dst) - nLen), repLen);
        }

        srcPos = cur + srchLen;
        dstPos = dstPos + nLen + repLen;

    }while(cur);
}

STATIC VOS_VOID AT_ReplaceCrLfInAtpBuffer(VOS_CHAR *buffer, VOS_UINT32 bufLen)
{
    VOS_UINT8       *sendDataBuffer = (VOS_UINT8 *)AT_GetSendDataBuffer();
    const VOS_UINT8 *atCrLf = AT_GetCrlf();
    errno_t          memResult;

    (VOS_VOID)memset_s(sendDataBuffer, bufLen, 0, bufLen);

    AT_ReplaceStr((VOS_CHAR *)sendDataBuffer, AT_CMD_MAX_LEN,
        (VOS_CHAR *)buffer, "\r\n", (VOS_CHAR *)atCrLf);

    memResult = memcpy_s(buffer, bufLen, sendDataBuffer, bufLen);
    TAF_MEM_CHK_RTN_VAL(memResult, bufLen, bufLen);
}

STATIC VOS_VOID AT_ProcAtpResponseData(struct ATP_AT_CmdRsp *atpData, VOS_UINT32 len)
{
    AT_ClientManage    *clientManage = VOS_NULL_PTR;
    AT_SEND_DataBuffer *sendDataBuff = AT_GetSendDataBuff();
    VOS_CHAR           *sndCodeAddress = (VOS_CHAR *)AT_GetSndCodeAddress();
    const VOS_UINT8    *atCrLf = AT_GetCrlf();
    VOS_BOOL            ret = VOS_FALSE;
    VOS_UINT8           clientId;

    if (len < sizeof(*atpData)) {
        AT_PR_LOGE("ATP cmdRsp length too short.");
        return;
    }

    ret = AT_IsMsgBuffValid((VOS_CHAR *)atpData->buffer, atpData->bufLen,
        MAX_ATP_AT_RSP_BUF_LEN, len, sizeof(*atpData));
    if (ret == VOS_FALSE) {
        AT_PR_LOGE("AT_IsMsgBuffValid buff invalid");
        return;
    }

    /* 判断当前的回复是否正在等待的AT回复 */
    clientId = (VOS_UINT8)(atpData->atClientId);
    clientManage = AT_GetClientManage(clientId);
    if (clientManage->cmdCurrentOpt != AT_CMD_ATP_OPT) {
        AT_PR_LOGE("client cmdCurrentOpt = %d.", clientManage->cmdCurrentOpt);
        return;
    }

    if (atpData->magic != clientManage->magic) {
        AT_PR_LOGE("ATP magic = %d AT Magic = %d.", atpData->magic, clientManage->magic);
        return;
    }

    AT_StopTimerCmdReady(clientId);
    AT_RreturnCodeUint32 atRetCode = AT_GetRetCodeFromAtp(atpData->retCode);

    if (atRetCode != AT_OK || atpData->bufLen == 0) {
        sendDataBuff->bufLen = 0;
        /* 调用At_FormatResultData发送命令结果 */
        At_FormatResultData(clientId, atRetCode);
        return;
    }

    /* 如果<CR><LF>不是\r\n，此处需要把\r\n换为<CR><LF> */
    if (VOS_StrCmp((const VOS_CHAR *)atCrLf, "\r\n") != 0) {
        AT_ReplaceCrLfInAtpBuffer(atpData->buffer, atpData->bufLen);
    }

    AT_ParseContext *parseContext = AT_GetParseContext(clientId);
    sendDataBuff->bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, sndCodeAddress, sndCodeAddress,
        "%s: %s%s", (VOS_CHAR *)parseContext->cmdElement->cmdName, atpData->buffer, atCrLf);

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(clientId, atRetCode);
}

VOS_UINT8 AT_GetModemBroadCastIndex(VOS_UINT32 modemId)
{
    VOS_UINT8 id = 0;

    if (modemId == MODEM_ID_0) {
        id = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    }

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    if (modemId == MODEM_ID_1) {
        id = AT_BROADCAST_CLIENT_INDEX_MODEM_1;
    }

#if (MULTI_MODEM_NUMBER == 3)
    if (modemId == MODEM_ID_2) {
        id = AT_BROADCAST_CLIENT_INDEX_MODEM_2;
    }
#endif
#endif

    return id;
}

STATIC VOS_VOID AT_ProcAtpUnsolRpt(struct ATP_AT_UnsolicitReport *unsolRpt, VOS_UINT32 len)
{
    AT_SEND_DataBuffer *sendData = AT_GetSendDataBuff();
    TAF_UINT8          *sendCodeAdd = AT_GetSndCodeAddress();
    VOS_UINT8          *atCrLf = AT_GetCrlf();
    VOS_BOOL            ret = VOS_FALSE;
    VOS_UINT8           indexNum;

    if (len <= sizeof(*unsolRpt)) {
        AT_PR_LOGE("ATP Unsolicit length too short.");
        return;
    }

    ret = AT_IsMsgBuffValid((VOS_CHAR *)unsolRpt->buffer, unsolRpt->bufLen,
        MAX_ATP_UNSOL_RPT_BUF_LEN, len, sizeof(*unsolRpt));
    if (ret == VOS_FALSE) {
        AT_PR_LOGE("AT_IsMsgBuffValid buff invalid");
        return;
    }

    /* 通过modemId获取index */
    indexNum = AT_GetModemBroadCastIndex(unsolRpt->modemId);
    if (indexNum == 0) {
        AT_PR_LOGE("modemId not found");
        return;
    }

    /* 如果<CR><LF>不是\r\n，此处需要把\r\n换为<CR><LF> */
    if (VOS_StrCmp((const VOS_CHAR *)atCrLf, "\r\n") != 0) {
        AT_ReplaceCrLfInAtpBuffer(unsolRpt->buffer, unsolRpt->bufLen);
    }

    sendData->bufLen = (VOS_UINT16)AT_FormatReportString(MAX_ATP_UNSOL_RPT_BUF_LEN, (VOS_CHAR *)sendCodeAdd,
        (VOS_CHAR *)sendCodeAdd, "%s%s", unsolRpt->buffer, atCrLf);

    At_SendResultData(indexNum, sendCodeAdd, sendData->bufLen);
}

VOS_VOID AT_ProcAtpMsg(VOS_VOID *data, VOS_UINT32 dataLen)
{
    struct AT_ATP_MsgHead  *head = VOS_NULL_PTR;
    VOS_VOID               *atpInfo = VOS_NULL_PTR;
    AT_ATP_MsgIdUint32      msgId;
    VOS_UINT32              atpInfoLen;

    head = (struct AT_ATP_MsgHead *)data;
    msgId = head->msgId;
    atpInfo = head->data;
    atpInfoLen = head->len;

    if (atpInfoLen == 0) {
        AT_PR_LOGE("data is null, %u", VOS_GetSlice());
        return;
    }

    switch (msgId) {
        case ID_ATP_AT_REG_CMD_TBL_REQ:
            AT_ProcRegAtpCmdTblReq((struct ATP_AT_RegCmdTblReq *)atpInfo, atpInfoLen);
            break;
        case ID_ATP_AT_CMD_RSP:
            AT_ProcAtpResponseData((struct ATP_AT_CmdRsp *)atpInfo, atpInfoLen);
            break;
        case ID_ATP_AT_UNSOL_RPT:
            AT_ProcAtpUnsolRpt((struct ATP_AT_UnsolicitReport *)atpInfo, atpInfoLen);
            break;
        default:
            break;
    }
}

VOS_VOID AT_InitAtpPort(VOS_VOID)
{
    struct DMS_ClientInfo clientInfo;

    (VOS_VOID)memset_s(&clientInfo, sizeof(clientInfo), 0, sizeof(clientInfo));

    /*
     * ATP通道和AT端口为两类端口，ATP通道端口client和AT的client分开，
     * 当前ATP client id默认填0
     */
    clientInfo.id         = 0;
    clientInfo.type       = DMS_CLIENT_TYPE_COM;
    clientInfo.RcvCmdData = AT_RcvAtpStream;

    DMS_PORT_RegClientInfo(DMS_PORT_ATP, &clientInfo);
}

MODULE_EXPORTED VOS_VOID AT_ShowAtpCmdTblInfo(VOS_VOID)
{
    AT_ParCmdElement *cmdTbl = AT_GetAtpCmdTbl();
    VOS_UINT32       *cmdNum = AT_GetAtpCmdCount();

    AT_PR_LOGD("ATP cmd tbl cmd num is %d.", *cmdNum);

    if (cmdTbl == VOS_NULL_PTR) {
        AT_PR_LOGE("AtpCmdTbl is NULL.");
        return;
    }

    for (VOS_UINT32 i = 0; i < *cmdNum; i++) {
        AT_PR_LOGD("index: %d name: %s, check para: %s", i, cmdTbl[i].cmdName, cmdTbl[i].param);
        AT_PR_LOGD("cmdIdx: %5d, errCod: %5d, chkFlg : %5d",
            cmdTbl[i].cmdIndex, cmdTbl[i].paramErrCode, cmdTbl[i].chkFlag);
        AT_PR_LOGD("setLen: %5d, qryLen: %5d, testLen: %5d, abortLen: %5d",
            cmdTbl[i].setTimeOut, cmdTbl[i].qryTimeOut, cmdTbl[i].testTimeOut, cmdTbl[i].abortTimeOut);
        AT_PR_LOGD("setFlg: %5d, qryFlg: %5d, testFlg: %5d, abortFlg: %5d",
            (cmdTbl[i].setProc   == VOS_NULL_PTR) ? VOS_FALSE : VOS_TRUE,
            (cmdTbl[i].qryProc   == VOS_NULL_PTR) ? VOS_FALSE : VOS_TRUE,
            (cmdTbl[i].testProc  == VOS_NULL_PTR) ? VOS_FALSE : VOS_TRUE,
            (cmdTbl[i].abortProc == VOS_NULL_PTR) ? VOS_FALSE : VOS_TRUE);
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

