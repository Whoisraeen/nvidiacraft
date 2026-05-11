#include <jni.h>
#include <vector>
#include <cstring>
#include <iostream>

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
    sl::FrameToken* current_token = nullptr;

    sl::Resource color_buffer;
    sl::Resource depth_buffer;
    sl::Resource mvec_buffer;
    
    void initResources() {
        color_buffer = sl::Resource(sl::ResourceType::eTex2d, nullptr);
        depth_buffer = sl::Resource(sl::ResourceType::eTex2d, nullptr);
        mvec_buffer = sl::Resource(sl::ResourceType::eTex2d, nullptr);
    }
}

extern "C" {

JNIEXPORT jint JNICALL Java_net_raeen_nvidiacraft_NativeLoader_nativeInitStreamline(JNIEnv *env, jclass clazz) {
    if (nvidiacraft::sl_initialized) return 0;

    nvidiacraft::initResources();

    nvidiacraft::sl_prefs.applicationId = 1337;
    nvidiacraft::sl_prefs.engine = sl::EngineType::eCustom;
    nvidiacraft::sl_prefs.engineVersion = "1.0";
    
    static sl::Feature features[] = { sl::kFeatureDLSS, sl::kFeatureDLSS_G, sl::kFeatureReflex };
    nvidiacraft::sl_prefs.featuresToLoad = features;
    nvidiacraft::sl_prefs.numFeaturesToLoad = 3;
    nvidiacraft::sl_prefs.flags |= sl::PreferenceFlags::eUseFrameBasedResourceTagging;

    sl::Result res = slInit(nvidiacraft::sl_prefs);
    if (res != sl::Result::eOk) {
        std::cerr << "[Nvidiacraft Native] slInit failed: " << (int)res << std::endl;
        return (int)res;
    }

    nvidiacraft::sl_initialized = true;
    return 0;
}

JNIEXPORT void JNICALL Java_net_raeen_nvidiacraft_NativeLoader_nativeUpdateMatrices(JNIEnv *env, jclass clazz, jfloatArray modelView, jfloatArray projection) {
    if (!nvidiacraft::sl_initialized) return;
    
    sl::Constants constants;
    constants.mvecScale = {1.0f, 1.0f};
    
    sl::Result res = slGetNewFrameToken(nvidiacraft::current_token, &nvidiacraft::frame_index);
    if (res == sl::Result::eOk && nvidiacraft::current_token) {
        slSetConstants(constants, *nvidiacraft::current_token, nvidiacraft::viewport);
    }
}

JNIEXPORT void JNICALL Java_net_raeen_nvidiacraft_NativeLoader_nativeEvaluateDLSS(JNIEnv *env, jclass clazz) {
    if (!nvidiacraft::sl_initialized || !nvidiacraft::current_token) return;

    sl::ResourceTag tags[] = {
        sl::ResourceTag(&nvidiacraft::color_buffer, sl::kBufferTypeScalingInputColor, sl::eOnlyValidNow),
        sl::ResourceTag(&nvidiacraft::depth_buffer, sl::kBufferTypeDepth, sl::eOnlyValidNow),
        sl::ResourceTag(&nvidiacraft::mvec_buffer, sl::kBufferTypeMotionVectors, sl::eOnlyValidNow)
    };
    slSetTagForFrame(*nvidiacraft::current_token, nvidiacraft::viewport, tags, 3, nullptr);

    sl::DLSSOptions dlss_options;
    dlss_options.mode = sl::DLSSMode::eBalanced;
    const sl::BaseStructure* dlss_inputs[] = { &dlss_options, &nvidiacraft::viewport };
    slEvaluateFeature(sl::kFeatureDLSS, *nvidiacraft::current_token, dlss_inputs, 2, nullptr);

    sl::DLSSGOptions dlssg_options;
    dlssg_options.mode = sl::DLSSGMode::eOn;
    const sl::BaseStructure* dlssg_inputs[] = { &dlssg_options, &nvidiacraft::viewport };
    slEvaluateFeature(sl::kFeatureDLSS_G, *nvidiacraft::current_token, dlssg_inputs, 2, nullptr);
    
    nvidiacraft::frame_index++;
}

JNIEXPORT void JNICALL Java_net_raeen_nvidiacraft_NativeLoader_nativeSetMotionVector(JNIEnv *env, jclass clazz, jfloat vx, jfloat vy) {
}

JNIEXPORT void JNICALL Java_net_raeen_nvidiacraft_NativeLoader_nativeSetDepthBuffer(JNIEnv *env, jclass clazz, jlong glTextureId) {
    nvidiacraft::depth_buffer.native = (void*)glTextureId;
}

JNIEXPORT void JNICALL Java_net_raeen_nvidiacraft_NativeLoader_nativeSetColorBuffer(JNIEnv *env, jclass clazz, jlong glTextureId) {
    nvidiacraft::color_buffer.native = (void*)glTextureId;
}

}
