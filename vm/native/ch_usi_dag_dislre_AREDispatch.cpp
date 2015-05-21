/*
 * ch.usi.dag.dislre.AREDispatch
 */
#include "Dalvik.h"
#include "native/InternalNativePriv.h"

#include <pthread.h>
#include "interface/ShadowVMInterface.h"
#include "utils/ObjTagHelper.h"
#include <map>
using namespace std;

pthread_mutex_t gl_mtx;

extern u8 getObjectTag(Object* obj);
extern void setObjectTag(Object* obj, u8 tag);
extern u8 setAndGetTag(Object* obj);

int64_t getTimeNsec(){
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (int64_t) now.tv_sec*1000000000LL + now.tv_nsec;
}

static jlong ot_object_id = 1;
static jint ot_class_id = 1;

jlong newClass(ClassObject *obj){
    if(obj == NULL)  {
        return 0;
    }
    setObjectTag(obj,_set_net_reference(ot_object_id++,ot_class_id++,1,1));
    jlong superTag = setAndGetTag(obj->super);
    jlong loaderTag = setAndGetTag(obj->classLoader);
    svmClassEvent(getpid(), obj->descriptor, strlen(obj->descriptor), loaderTag);
    svmClassInfo(getpid(), getObjectTag(obj), obj->descriptor, strlen(obj->descriptor), loaderTag, superTag);
    return getObjectTag(obj);
}

u8 setAndGetTag(Object* obj){
    //TODO
    //need to consider concurrency here
    jlong res;
    if(obj == NULL)
    {
        res = 0;
    }else if(getObjectTag(obj)!=0){
        res = getObjectTag(obj);
    }else if(dvmIsClassObject(obj)){
        res = newClass((ClassObject*)obj);
    }else {
        jlong clazzTag = setAndGetTag(obj->clazz);
        res = _set_net_reference(ot_object_id++,net_ref_get_class_id(clazzTag),0,0);
        setObjectTag(obj, res);
    }
    return res;
}

void _markSpecBit(Object* obj){
    if(obj == NULL)
        return;
    jlong tag = setAndGetTag(obj);
    net_ref_set_spec((jlong*)&tag, 1);
    setObjectTag(obj, tag);
}


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
    Object* obj = (Object*)args[0];
    pResult->j = setAndGetTag(obj);
}

/*
 * return current thread's id in jint
 */
static void AREDispatch_getThisThreadId(const u4* args, JValue *pResult){
    pResult->i = dvmThreadSelf()->threadId;
}

/*
 * return current process id in jint
 */
static void AREDispatch_getThisProcId(const u4* args, JValue *pResult){
    pResult->i = getpid();
}

/*
 * get time clock of CPU in nanoseconds in jlong
 */
static void AREDispatch_getCPUClock(const u4* args, JValue *pResult){
    pResult->j = getTimeNsec();
}

/*
 * register a method at remote and return the method id in jshort
 */
static void AREDispatch_registerMethod(const u4* args, JValue *pResult){
    StringObject* methodNameObj = (StringObject*)args[0];
    char* name = dvmCreateCstrFromString(methodNameObj);
    pResult->s = svmRegisterMethod(getpid(), name);
    free(name);
}

/*
 * analysis start with analysis id
 */
static void AREDispatch_analysisStart__S(const u4* args, JValue *pResult){
    int tid = dvmThreadSelf()->threadId;
    svmAnalysisStart(getpid(), tid, (short)args[0]);
}

/*
 * analysis start with analysis id and ordering id
 */
static void AREDispatch_analysisStart__SB(const u4* args, JValue *pResult){
    int tid = dvmThreadSelf()->threadId;
    svmAnalysisStart(getpid(), tid, (short)args[0],(long)args[1]);
}

/*
 * analysis end
 */
static void AREDispatch_analysisEnd(const u4* args, JValue *pResult){
    int tid = dvmThreadSelf()->threadId;
    svmAnalysisEnd(getpid(), tid);
}

/*
 * send argument
 */
static void AREDispatch_sendBoolean(const u4* args, JValue *pResult){
    svmSendBoolean(dvmThreadSelf()->threadId, (jboolean)args[0]);
}
static void AREDispatch_sendByte(const u4* args, JValue *pResult){
    svmSendByte(dvmThreadSelf()->threadId, (jbyte)args[0]);
}
static void AREDispatch_sendChar(const u4* args, JValue *pResult){
    svmSendChar(dvmThreadSelf()->threadId, (jchar)args[0]);
}
static void AREDispatch_sendShort(const u4* args, JValue *pResult){
    svmSendShort(dvmThreadSelf()->threadId, (jshort)args[0]);
}
static void AREDispatch_sendInt(const u4* args, JValue *pResult){
    svmSendInt(dvmThreadSelf()->threadId, (jint)args[0]);
}
static void AREDispatch_sendLong(const u4* args, JValue *pResult){
    svmSendLong(dvmThreadSelf()->threadId, *((jlong*)args));
}
static void AREDispatch_sendFloat(const u4* args, JValue *pResult){
    svmSendFloat(dvmThreadSelf()->threadId, *((jfloat*)args));
}
static void AREDispatch_sendDouble(const u4* args, JValue *pResult){
    svmSendDouble(dvmThreadSelf()->threadId, *((jdouble*)args));
}
static void AREDispatch_sendObject(const u4* args, JValue *pResult){
    Object* obj = (Object*) args[0];
    svmSendObject(dvmThreadSelf()->threadId, setAndGetTag(obj));
}
static void AREDispatch_sendObjectPlusData(const u4* args, JValue *pResult){
    Object* obj = (Object*) args[0];
    if(obj == NULL) {
        svmSendObject(dvmThreadSelf()->threadId, 0);
        return;
    }
    if(net_ref_get_spec(setAndGetTag(obj))){
        svmSendObject(dvmThreadSelf()->threadId, getObjectTag(obj));
        return;
    }
    if(obj->clazz == gDvm.classJavaLangString){
        char* str = dvmCreateCstrFromString((StringObject*)(obj));
        svmStringInfo(getpid(), getObjectTag(obj), str, strlen(str));
        free(str);
    }else if(obj->clazz == gDvm.classJavaLangThread){
        char* threadName = dvmCreateCstrFromString((StringObject*)dvmGetFieldObject(dvmThreadSelf()->threadObj,gDvm.offJavaLangThread_name));
        bool isDaemon = dvmGetFieldBoolean(dvmThreadSelf()->threadObj,gDvm.offJavaLangThread_daemon);
        svmThreadInfo(getpid(), dvmThreadSelf()->threadId, getObjectTag(obj), threadName, strlen(threadName), isDaemon);
        free(threadName);
    }
    _markSpecBit(obj);
    svmSendObject(dvmThreadSelf()->threadId, getObjectTag(obj));
}
static void AREDispatch_sendObjectSize(const u4* args, JValue *pResult){
    svmSendObjectSize(dvmThreadSelf()->threadId, ((Object*)args[0])->clazz->objectSize);
}
static void AREDispatch_sendCurrentThread(const u4* args, JValue *pResult){
    Object* obj = dvmThreadSelf()->threadObj;
    if(obj == NULL) {
        svmSendObject(dvmThreadSelf()->threadId, 0);
        return;
    }
    if(net_ref_get_spec(setAndGetTag(obj))){
        return;
    }
    if(obj->clazz == gDvm.classJavaLangThread){
        char* threadName = dvmCreateCstrFromString((StringObject*)dvmGetFieldObject(dvmThreadSelf()->threadObj,gDvm.offJavaLangThread_name));
        bool isDaemon = dvmGetFieldBoolean(dvmThreadSelf()->threadObj,gDvm.offJavaLangThread_daemon);
        svmThreadInfo(getpid(), dvmThreadSelf()->threadId, getObjectTag(obj), threadName, strlen(threadName), isDaemon);
        free(threadName);
    }
    _markSpecBit(obj);
    svmSendObject(dvmThreadSelf()->threadId, getObjectTag(obj));
}

/*
 * shut down analysis
 */
static void AREDispatch_manuallyClose(const u4* args, JValue *pResult){
    svmCloseAnalysis(getpid());
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
