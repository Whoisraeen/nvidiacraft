package net.raeen.mixin;

import net.minecraft.client.MinecraftClient;
import org.spongepowered.asm.mixin.Mixin;
import org.spongepowered.asm.mixin.injection.At;
import org.spongepowered.asm.mixin.injection.Inject;
import org.spongepowered.asm.mixin.injection.callback.CallbackInfo;

@Mixin(MinecraftClient.class)
public class MinecraftClientMixin {
    @Inject(method = "runTick", at = @At("HEAD"))
    private void onRunTickHead(boolean render, CallbackInfo ci) {
        // In a full implementation, we would call slReflex marker here via JNI
    }
}
