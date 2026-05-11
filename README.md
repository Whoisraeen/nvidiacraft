# Nvidiacraft

NVIDIA Streamline integration for Minecraft.

## Setup

For setup instructions, please see the [Fabric Documentation page](https://docs.fabricmc.net/develop/getting-started/creating-a-project#setting-up) related to the IDE that you are using.

## Building and Running

1. **Compile Native Code**: Run `./gradlew buildNative` to compile the C++ shared library.
2. **Add Streamline Binaries**: The Streamline SDK (submodule) does not include binary DLLs. You must download the official Streamline release from the [NVIDIA Developer portal](https://developer.nvidia.com/rtx/streamline) and place the required DLLs (e.g., `sl.common.dll`, `sl.dlss.dll`) into:
   `src/main/resources/assets/nvidiacraft/native/`
3. **Launch Minecraft**: Run `./gradlew runClient` to launch Minecraft with the mod enabled.

## Repository

[https://github.com/whoisraeen/nvidiacraft](https://github.com/whoisraeen/nvidiacraft)

## License

This project is available under the CC0 license.
