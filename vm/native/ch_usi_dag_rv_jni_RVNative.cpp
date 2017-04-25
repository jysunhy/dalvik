#include "Dalvik.h"
#include "native/InternalNativePriv.h"
#include "sharedmem/client.h"

static void RVNative_getByFlag0(const u4* args, JValue* pResult)
{
    int flag = args[0];
    FlaggedData dat = getByFlag(flag);
    ArrayObject* arrayObj = dvmAllocPrimitiveArray('B', dat.length, ALLOC_DEFAULT);
    if (arrayObj != NULL) {
        memcpy(arrayObj->contents, dat.addr+dat.offset, dat.length);
        dvmReleaseTrackedAlloc((Object*) arrayObj, NULL);
    }
    RETURN_PTR(arrayObj);
}

static void RVNative_getByFlag(const u4* args, JValue* pResult)
{
    int pid = args[0];
    int flag = args[1];
    FlaggedData dat = getByFlag(pid, flag);
    ArrayObject* arrayObj = dvmAllocPrimitiveArray('B', dat.length, ALLOC_DEFAULT);
    if (arrayObj != NULL) {
        memcpy(arrayObj->contents, dat.addr+dat.offset, dat.length);
        dvmReleaseTrackedAlloc((Object*) arrayObj, NULL);
    }
    RETURN_PTR(arrayObj);
}

static void RVNative_setData(const u4* args, JValue* pResult)
{
    ArrayObject* buf = (ArrayObject*) args[0];
    int flag = newData((char*)buf->contents, 0, buf->length);
    pResult->i = flag;
}


static void RVNative_getBinderFlag(const u4* args, JValue* pResult)
{
    pResult->i = dvmThreadSelf()->rvFlag;
}
static void RVNative_setBinderFlag(const u4* args, JValue* pResult)
{
    dvmThreadSelf()->rvFlag = args[0];
}
static void RVNative_clearBinderFlag(const u4* args, JValue* pResult)
{
    dvmThreadSelf()->rvFlag = 0;
}

const DalvikNativeMethod dvm_ch_usi_dag_rv_jni_RVNative[] = {
    { "getByFlag0",      "(I)[B",                  RVNative_getByFlag0 },
    { "getByFlag",      "(II)[B",                  RVNative_getByFlag },
    { "setData",      "([B)I",                  RVNative_setData },
    { "getBinderFlag",      "()I",                  RVNative_getBinderFlag },
    { "setBinderFlag",      "(I)V",                  RVNative_setBinderFlag },
    { "clearBinderFlag",      "()V",                  RVNative_clearBinderFlag },
    { NULL, NULL, NULL },
};
