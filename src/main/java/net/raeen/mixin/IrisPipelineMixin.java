package net.raeen.mixin;

import net.coderbot.iris.pipeline.WorldRenderingPipeline;
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
        // At this point, the opaque pass is done. 
        // We can grab the current textures from the render targets.
        // In a real implementation, we'd cast 'this' or use an accessor to get RenderTargets.
        // For this scaffold, we'll assume a way to get the IDs.
        
        // NativeLoader.nativeSetColorBuffer(irisColorTextureId);
        // NativeLoader.nativeSetDepthBuffer(irisDepthTextureId);
        NativeLoader.nativeEvaluateDLSS();
    }
}
