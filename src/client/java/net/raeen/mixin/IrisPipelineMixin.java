package net.raeen.mixin;

import net.raeen.nvidiacraft.NativeLoader;
import org.spongepowered.asm.mixin.Mixin;
import org.spongepowered.asm.mixin.Pseudo;
import org.spongepowered.asm.mixin.injection.At;
import org.spongepowered.asm.mixin.injection.Inject;
import org.spongepowered.asm.mixin.injection.callback.CallbackInfo;

@Pseudo
@Mixin(targets = "net.coderbot.iris.pipeline.WorldRenderingPipeline")
public class IrisPipelineMixin {
    @Inject(method = "renderOpaque", at = @At("RETURN"), remap = false)
    private void onRenderOpaqueReturn(CallbackInfo ci) {
        // NativeLoader.nativeSetColorBuffer(irisColorTextureId);
        // NativeLoader.nativeSetDepthBuffer(irisDepthTextureId);
        NativeLoader.nativeEvaluateDLSS();
    }
}
