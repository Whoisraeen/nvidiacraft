package net.raeen.client;

import net.fabricmc.api.ClientModInitializer;

public class NvidiacraftClient implements ClientModInitializer {
	@Override
	public void onInitializeClient() {
		net.raeen.nvidiacraft.NativeLoader.initialize();
	}
}