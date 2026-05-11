package net.raeen.nvidiacraft;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class NativeLoader {
    private static final Logger LOGGER = LoggerFactory.getLogger("Nvidiacraft/NativeLoader");
    private static boolean loaded = false;

    public static void initialize() {
        if (loaded) return;

        try {
            System.loadLibrary("nvidiacraft_native");
            LOGGER.info("Successfully loaded nvidiacraft_native library.");
            loaded = true;
            
            // Initialize native Streamline context
            int slResult = nativeInitStreamline();
            if (slResult == 0) {
                LOGGER.info("NVIDIA Streamline initialized successfully.");
            } else {
                LOGGER.warn("NVIDIA Streamline initialization failed with code: " + slResult);
            }
        } catch (UnsatisfiedLinkError e) {
            LOGGER.error("Failed to load nvidiacraft_native library: " + e.getMessage());
            e.printStackTrace();
        }
    }

    // Native JNI stubs
    private static native int nativeInitStreamline();
    public static native void nativeSetMotionVector(float vx, float vy);
    public static native void nativeUpdateMatrices(float[] modelView, float[] projection);
    public static native void nativeSetDepthBuffer(long glTextureId);
    public static native void nativeSetColorBuffer(long glTextureId);
    public static native void nativeEvaluateDLSS();
}
