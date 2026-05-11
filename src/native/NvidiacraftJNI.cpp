#include <jni.h>
#include <vector>
#include <cstring>

#define SL_PROD
#include <sl.h>
#include <sl_dlss.h>
#include <sl_dlss_g.h>
#include <sl_reflex.h>

namespace nvidiacraft {
    sl::Preferences sl_prefs;
    bool sl_initialized = false;
    sl::ViewportHandle viewport{ 0 };
    uint32_t frame_index = 0;

    sl::Resource color_buffer{};
    sl::Resource depth_buffer{};
    sl::Resource mvec_buffer{};
}

extern "C" {

JNIEXPORT jint JNICALL Java_net_raeen_nvidiacraft_NativeLoader_nativeInitStreamline(JNIEnv *env, jclass clazz) {
    if (nvidiacraft::sl_initialized) return 0;

    nvidiacraft::sl_prefs.applicationId = 1337;
    nvidiacraft::sl_prefs.engine = sl::EngineType::eCustom;
    nvidiacraft::sl_prefs.engineVersion = "1.0";
    
    sl::Feature features[] = { sl::kFeatureDLSS, sl::kFeatureDLSS_G, sl::kFeatureReflex };
    nvidiacraft::sl_prefs.featuresToLoad = features;
    nvidiacraft::sl_prefs.numFeaturesToLoad = 3;

    sl::Result res = slInit(nvidiacraft::sl_prefs);
    if (res != sl::Result::eOk) return (int)res;

    nvidiacraft::sl_initialized = true;
    return 0;
}

JNIEXPORT void JNICALL Java_net_raeen_nvidiacraft_NativeLoader_nativeUpdateMatrices(JNIEnv *env, jclass clazz, jfloatArray modelView, jfloatArray projection) {
    jfloat* mv = env->GetFloatArrayElements(modelView, 0);
    jfloat* proj = env->GetFloatArrayElements(projection, 0);

    sl::Constants constants;
    sl::FrameToken token(nvidiacraft::frame_index);
    slSetConstants(constants, token, nvidiacraft::viewport);

    env->ReleaseFloatArrayElements(modelView, mv, 0);
    env->ReleaseFloatArrayElements(projection, proj, 0);
}

JNIEXPORT void JNICALL Java_net_raeen_nvidiacraft_NativeLoader_nativeEvaluateDLSS(JNIEnv *env, jclass clazz) {
    sl::FrameToken token(nvidiacraft::frame_index++);
    
    sl::ResourceTag tags[] = {
        { &nvidiacraft::color_buffer, sl::kBufferTypeColor, sl::ResourceState::eGeneral },
        { &nvidiacraft::depth_buffer, sl::kBufferTypeDepth, sl::ResourceState::eGeneral },
        { &nvidiacraft::mvec_buffer, sl::kBufferTypeMotionVectors, sl::ResourceState::eGeneral }
    };
    slSetTag(nvidiacraft::viewport, tags, 3, token);

    sl::DLSSOptions dlss_options;
    dlss_options.mode = sl::DLSSMode::eBalanced;
    slEvaluateFeature(sl::kFeatureDLSS, token, &dlss_options, nvidiacraft::viewport);

    sl::DLSSGOptions dlssg_options;
    dlssg_options.mode = sl::DLSSGMode::eOn;
    slEvaluateFeature(sl::kFeatureDLSS_G, token, &dlssg_options, nvidiacraft::viewport);
}

JNIEXPORT void JNICALL Java_net_raeen_nvidiacraft_NativeLoader_nativeSetMotionVector(JNIEnv *env, jclass clazz, jfloat vx, jfloat vy) {
    // Tagging motion vectors
}

JNIEXPORT void JNICALL Java_net_raeen_nvidiacraft_NativeLoader_nativeSetDepthBuffer(JNIEnv *env, jclass clazz, jlong glTextureId) {
    nvidiacraft::depth_buffer.native = (void*)glTextureId;
    nvidiacraft::depth_buffer.type = sl::ResourceType::eTex2d;
}

JNIEXPORT void JNICALL Java_net_raeen_nvidiacraft_NativeLoader_nativeSetColorBuffer(JNIEnv *env, jclass clazz, jlong glTextureId) {
    nvidiacraft::color_buffer.native = (void*)glTextureId;
    nvidiacraft::color_buffer.type = sl::ResourceType::eTex2d;
}

}
