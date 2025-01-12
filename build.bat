@echo off
setlocal

set EMCC="D:\Program Files\EmscriptenSDK\emsdk-main\upstream\emscripten\emcc"
set PROJECT_DIR=f:/Projects/CPP/TestWwiseWeb
set INCLUDE_DIR=%PROJECT_DIR%/wwise/include
set LIB_DIR=%PROJECT_DIR%/wwise/lib
set OUTPUT=%PROJECT_DIR%/TestWwise.js
set EMSDK_INCLUDE="D:\Program Files\EmscriptenSDK\emsdk-main\upstream\emscripten\cache\sysroot\include"



echo Building project...
%EMCC% ^
   "%PROJECT_DIR%/TestWwise.cpp" ^
   "%PROJECT_DIR%/Application.cpp" ^
   "%PROJECT_DIR%/Intergration.cpp" ^
   "%PROJECT_DIR%/wwise/scripts/AkDefaultIOHookDeferred.cpp" ^
   "%PROJECT_DIR%/wwise/scripts/AkDefaultLowLevelIODispatcher.cpp" ^
   "%PROJECT_DIR%/wwise/scripts/AkFileLocationBase.cpp" ^
   "%PROJECT_DIR%/wwise/scripts/AkFilePackage.cpp" ^
   "%PROJECT_DIR%/wwise/scripts/AkFilePackageLUT.cpp" ^
   "%PROJECT_DIR%/wwise/scripts/AkGeneratedSoundBanksResolver.cpp" ^
   "%PROJECT_DIR%/wwise/scripts/AkJobWorkerMgr.cpp" ^
   "%PROJECT_DIR%/wwise/scripts/AkMultipleFileLocation.cpp" ^
   -o "%OUTPUT%" ^
   -I "%INCLUDE_DIR%" -I "%INCLUDE_DIR%/AK/SoundEngine/Common" -I %EMSDK_INCLUDE% ^
   -L "%LIB_DIR%" ^
   -lAk3DAudioBedMixerFX -lAkAudioInputSource -lAkCompressorFX -lAkDelayFX -lAkExpanderFX -lAkFlangerFX -lAkGainFX -lAkGuitarDistortionFX -lAkHarmonizerFX ^
   -lAkMatrixReverbFX -lAkMemoryMgr -lAkMeterFX -lAkMusicEngine -lAkOpusDecoder -lAkParametricEQFX -lAkPeakLimiterFX -lAkPitchShifterFX -lAkRecorderFX ^
   -lAkReflectFX -lAkRoomVerbFX -lAkSilenceSource -lAkSineSource -lAkSoundEngine -lAkSoundSeedWindSource -lAkSoundSeedWooshSource -lAkSpatialAudio ^
   -lAkStereoDelayFX -lAkStreamMgr -lAkSynthOneSource -lAkTimeStretchFX -lAkToneSource -lAkTremoloFX -lAkVorbisDecoder -lAuroHeadphoneFX -lCommunicationCentral ^
   -liZHybridReverbFX -liZTrashBoxModelerFX -liZTrashDelayFX -liZTrashDistortionFX -liZTrashDynamicsFX -liZTrashFiltersFX -liZTrashMultibandDistortionFX ^
   -lMasteringSuiteFX -lMcDSPFutzBoxFX -lMcDSPLimiterFX ^
   -msse2 -msimd128 ^
   -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=15 -s AUDIO_WORKLET=1 -s WASM_WORKERS=1 ^
    -s STACK_OVERFLOW_CHECK=1 ^
   -Wl,--export-dynamic ^
   -s TOTAL_MEMORY=134217728 ^
   -std=c++17 ^
   -fexceptions ^
   -Wno-invalid-offsetof ^
   -fPIC ^
   -fvisibility=default ^
   -matomics ^
   -DEMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES=0 ^
   -s EXPORTED_RUNTIME_METHODS="[\"ccall\",\"cwrap\",\"err\",\"addFunction\",\"removeFunction\",\"getValue\",\"setValue\",\"writeArrayToMemory\",\"writeStringToMemory\"]" -O2 ^
   -s EXPORTED_FUNCTIONS="[\"_malloc\"]" ^
   -s ALLOW_TABLE_GROWTH=1 ^
   --preload-file Resources@/Resources ^
   -g --bind -lembind ^
   -DAK_OPTIMIZED ^
   -fno-rtti -sFETCH -o dist/WwiseModule.js

if %errorlevel% neq 0 (
    echo Build failed with error code %errorlevel%
) else (
    echo Build succeeded
)

pause
endlocal