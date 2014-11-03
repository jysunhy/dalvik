/*
 * ch.usi.dag.dislre.AREDispatch
 */
#include "Dalvik.h"
#include "native/InternalNativePriv.h"

/* 
 * print a String to Android console with TAG ShadowVM
 */
static void AREDispatch_NativeLog(const u4* args, JValue* pResult)
{
    StringObject* logContentObj = (StringObject*) args[0];
    if(logContentObj != NULL) {
        char* logContent = dvmCreateCstrFromString(logContentObj);
        ALOG(LOG_DEBUG, "ShadowVM", "%s", logContent);
        free(logContent);
    }
}

/*
 * return object tag in jlong
 */
static void AREDispatch_getObjectId(const u4* args, JValue *pResult){
}

/*
 * return current thread's id in jint
 */
static void AREDispatch_getThisThreadId(const u4* args, JValue *pResult){
}

/*
 * return current process id in jint
 */
static void AREDispatch_getThisProcId(const u4* args, JValue *pResult){
}

/*
 * get time clock of CPU in nanoseconds in jlong
 */
static void AREDispatch_getCPUClock(const u4* args, JValue *pResult){
}

/*
 * register a method at remote and return the method id in jshort
 */
static void AREDispatch_registerMethod(const u4* args, JValue *pResult){
}

/*
 * analysis start with analysis id
 */
static void AREDispatch_analysisStart__S(const u4* args, JValue *pResult){
}

/*
 * analysis start with analysis id and ordering id
 */
static void AREDispatch_analysisStart__SB(const u4* args, JValue *pResult){

}

/*
 * analysis end
 */
static void AREDispatch_analysisEnd(const u4* args, JValue *pResult){

}

/*
 * send argument
 */
static void AREDispatch_sendBoolean(const u4* args, JValue *pResult){

}
static void AREDispatch_sendByte(const u4* args, JValue *pResult){

}
static void AREDispatch_sendChar(const u4* args, JValue *pResult){

}
static void AREDispatch_sendShort(const u4* args, JValue *pResult){

}
static void AREDispatch_sendInt(const u4* args, JValue *pResult){

}
static void AREDispatch_sendLong(const u4* args, JValue *pResult){

}
static void AREDispatch_sendFloat(const u4* args, JValue *pResult){

}
static void AREDispatch_sendDouble(const u4* args, JValue *pResult){

}
static void AREDispatch_sendObject(const u4* args, JValue *pResult){

}
static void AREDispatch_sendObjectPlusData(const u4* args, JValue *pResult){

}
static void AREDispatch_sendObjectSize(const u4* args, JValue *pResult){

}
static void AREDispatch_sendCurrentThread(const u4* args, JValue *pResult){

}

/*
 * shut down analysis
 */
static void AREDispatch_manuallyClose(const u4* args, JValue *pResult){

}



const DalvikNativeMethod dvm_ch_usi_dag_dislre_AREDispatch[] = {
    {"NativeLog",      "(Ljava/lang/String;)V", AREDispatch_NativeLog },
    {"getObjectId", "(Ljava/lang/Object;)J", AREDispatch_getObjectId},
    {"getThisThreadId", "()I", AREDispatch_getThisThreadId},
    {"getThisProcId", "()I", AREDispatch_getThisProcId},
//    {"CallAPI", "(I)V", AREDispatch_CallAPI},
//    {"methodEnter", "()V", AREDispatch_methodEnter},
//    {"methodExit", "()V", AREDispatch_methodExit},
//    {"printStack", "()V", AREDispatch_printStack},
//    {"checkThreadPermission", "()I", AREDispatch_checkThreadPermission},
    {"getCPUClock", "()J", AREDispatch_getCPUClock},
    {"registerMethod", "(Ljava/lang/String;)S", AREDispatch_registerMethod},
    {"analysisStart", "(S)V", AREDispatch_analysisStart__S},
    {"analysisStart", "(SB)V", AREDispatch_analysisStart__SB},
    {"analysisEnd", "()V", AREDispatch_analysisEnd},
    {"sendBoolean", "(Z)V", AREDispatch_sendBoolean},
    {"sendByte", "(B)V", AREDispatch_sendByte},
    {"sendChar", "(C)V", AREDispatch_sendChar},
    {"sendShort", "(S)V", AREDispatch_sendShort},
    {"sendInt", "(I)V", AREDispatch_sendInt},
    {"sendLong", "(J)V", AREDispatch_sendLong},
    {"sendFloat", "(F)V", AREDispatch_sendFloat},
    {"sendDouble", "(D)V", AREDispatch_sendDouble},
    {"sendObject", "(Ljava/lang/Object;)V", AREDispatch_sendObject},
    {"sendObjectPlusData", "(Ljava/lang/Object;)V", AREDispatch_sendObjectPlusData},
    {"sendObjectSize", "(Ljava/lang/Object;)V", AREDispatch_sendObjectSize},
    {"sendCurrentThread", "()V", AREDispatch_sendCurrentThread},
    {"manuallyClose", "()V", AREDispatch_manuallyClose},
    { NULL, NULL, NULL },
};
