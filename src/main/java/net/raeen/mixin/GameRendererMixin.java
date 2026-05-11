package net.raeen.mixin;

import net.minecraft.client.render.GameRenderer;
import net.raeen.nvidiacraft.JitterManager;
import net.raeen.nvidiacraft.NativeLoader;
import org.joml.Matrix4f;
import org.spongepowered.asm.mixin.Mixin;
import org.spongepowered.asm.mixin.Unique;
import org.spongepowered.asm.mixin.injection.At;
import org.spongepowered.asm.mixin.injection.Inject;
import org.spongepowered.asm.mixin.injection.callback.CallbackInfo;
import org.spongepowered.asm.mixin.injection.callback.CallbackInfoReturnable;

@Mixin(GameRenderer.class)
public class GameRendererMixin {
    @Unique
    private final float[] modelViewArray = new float[16];
    @Unique
    private final float[] projectionArray = new float[16];

    @Inject(method = "renderWorld", at = @At("HEAD"))
    private void onRenderWorldHead(float tickDelta, long limitTime, CallbackInfo ci) {
        JitterManager.nextFrame();
    }

    @Inject(method = "getBasicProjectionMatrix", at = @At("RETURN"))
    private void onGetBasicProjectionMatrix(double fov, CallbackInfoReturnable<Matrix4f> cir) {
        Matrix4f proj = cir.getReturnValue();
        // Apply jitter
        proj.translate(JitterManager.getJitterX(), JitterManager.getJitterY(), 0);
    }

    @Inject(method = "renderWorld", at = @At(value = "INVOKE", target = "Lnet/minecraft/client/render/GameRenderer;getBasicProjectionMatrix(D)Lorg/joml/Matrix4f;"))
    private void onBeforeRenderWorld(float tickDelta, long limitTime, CallbackInfo ci) {
        // Capture matrices
        // Note: In 1.21, we might need to access the matrices from the current RenderSystem or context
    }
}
