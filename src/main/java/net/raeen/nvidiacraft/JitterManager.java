package net.raeen.nvidiacraft;

import net.minecraft.client.MinecraftClient;

public class JitterManager {
    private static int sampleIndex = 0;
    private static float jitterX = 0;
    private static float jitterY = 0;

    public static void nextFrame() {
        sampleIndex = (sampleIndex + 1) % 16; // 16-sample sequence
        jitterX = halton(sampleIndex + 1, 2) - 0.5f;
        jitterY = halton(sampleIndex + 1, 3) - 0.5f;
    }

    public static float getJitterX() {
        return jitterX / (float)MinecraftClient.getInstance().getWindow().getFramebufferWidth();
    }

    public static float getJitterY() {
        return jitterY / (float)MinecraftClient.getInstance().getWindow().getFramebufferHeight();
    }

    private static float halton(int index, int base) {
        float result = 0;
        float f = 1.0f / base;
        int i = index;
        while (i > 0) {
            result += f * (i % base);
            i = i / base;
            f = f / base;
        }
        return result;
    }
}
