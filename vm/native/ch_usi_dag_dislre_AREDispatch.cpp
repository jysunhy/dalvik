/*
 * ch.usi.dag.dislre.AREDispatch
 */
#include "Dalvik.h"
#include "native/InternalNativePriv.h"

static void AREDispatch_NativeLog(const u4* args, JValue* pResult)
{
    ALOG(LOG_DEBUG, "ShadowVMNative", "test");
}

const DalvikNativeMethod dvm_ch_usi_dag_dislre_AREDispatch[] = {
    { "NativeLog",      "(Ljava/lang/String;)V",                   AREDispatch_NativeLog },
    { NULL, NULL, NULL },
};
