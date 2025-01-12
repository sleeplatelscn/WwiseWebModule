

const AkCallbackType = {
    AK_EndOfEvent: 0x0001,                    // 1 in binary: 0001
    AK_EndOfDynamicSequenceItem: 0x0002,       // 2 in binary: 0010
    AK_Marker: 0x0004,                      // 4 in binary: 0100
    AK_Duration: 0x0008,                    // 8 in binary: 1000
    AK_SpeakerVolumeMatrix: 0x0010,           // 16 in binary: 0001 0000
    AK_Starvation: 0x0020,                  // 32 in binary: 0010 0000
    AK_MusicPlaylistSelect: 0x0040,          // 64 in binary: 0100 0000
    AK_MusicPlayStarted: 0x0080,             // 128 in binary: 1000 0000
    AK_MusicSyncBeat: 0x0100,                // 256 in binary: 0001 0000 0000
    AK_MusicSyncBar: 0x0200,                 // 512 in binary: 0010 0000 0000
    AK_MusicSyncEntry: 0x0400,               // 1024 in binary: 0100 0000 0000
    AK_MusicSyncExit: 0x0800,                // 2048 in binary: 1000 0000 0000
    AK_MusicSyncGrid: 0x1000,                // 4096 in binary: 0001 0000 0000 0000
    AK_MusicSyncUserCue: 0x2000,              // 8192 in binary: 0010 0000 0000 0000
    AK_MusicSyncPoint: 0x4000,               // 16384 in binary: 0100 0000 0000 0000
    AK_MIDIEvent: 0x8000,                   // 32768 in binary: 1000 0000 0000 0000
    AK_Callback_Last: 0x10000,              // 65536 in binary: 0001 0000 0000 0000 0000

    // A few useful bitmasks.
    AK_MusicSyncAll: 0x7f00,                 // 32512 in binary: 0111 1111 0000 0000
    AK_CallbackBits: 0xffff,               // 65535 in binary: 1111 1111 1111 1111

    // Not callback types, but need to be part of same bitfield for AK::SoundEngine::PostEvent().
    AK_EnableGetSourcePlayPosition: 0x100000, // 1048576 in binary: 0001 0000 0000 0000 0000 0000
    AK_EnableGetMusicPlayPosition: 0x200000,  // 2097152 in binary: 0010 0000 0000 0000 0000 0000
    AK_EnableGetSourceStreamBuffering: 0x400000, // 4194304 in binary: 0100 0000 0000 0000 0000 0000
    AK_SourceInfo_Last: 0x800000             // 8388608 in binary: 1000 0000 0000 0000 0000 0000
};
const AkActionOnEventType =
{
    AkActionOnEventType_Stop: 0,			///< Stop
    AkActionOnEventType_Pause: 1,			///< Pause
    AkActionOnEventType_Resume: 2,			///< Resume
    AkActionOnEventType_Break: 3,			///< Break
    AkActionOnEventType_ReleaseEnvelope: 4,	///< Release envelope
    AkActionOnEventType_Last: 5				///< End of enum, invalid value.
};

// create a Map to store callback functions
const callbackMap = new Map();
const sharedBuffer = new SharedArrayBuffer(4); // 4 bytes for an Int32
const callbackIDCounter = new Int32Array(sharedBuffer);

// generate callback ID and add callback to callbackMap
function addCallback(callback) {
    // use Atomics to increment callbackIDCounter
    const id = Atomics.add(callbackIDCounter, 0, 1);
    callbackMap.set(id, callback);
    return id;
}

// handle Wwise callback
function WwiseCallbackHandler(...args){
    if(typeof args[0] === 'string'){
        if(args[0].startsWith("WwiseCallback")){
            let arr = args[0].split(',');
            if(arr.length > 1){
                var callbackType = parseInt(arr[1]);
                var callbackID = parseInt(arr[2]);
                //console.log("WwiseCallback: " + arr[1]+" "+arr[2]);
                if(callbackMap.has(callbackID)){
                    callbackMap.get(callbackID)(callbackType);
                    if(callbackType == AkCallbackType.AK_EndOfEvent){
                        callbackMap.delete(callbackID);
                    }
                }
            }
        }
    }
}
// generate random color
function getRandomColor() {
    const letters = '0123456789ABCDEF';
    let color = '#';
    for (let i = 0; i < 6; i++) {
        color += letters[Math.floor(Math.random() * 16)];
    }
    return color;
}

// change color
function changeColor() {
    const colorBlock = document.getElementById('colorBlock');
    colorBlock.style.backgroundColor = getRandomColor();
}
//music playingID
var playingID = 0;


var script = document.createElement('script');
script.src = '../Wwisemodule.js';
script.onload = function() {
    
    // Emscripten模块加载完成后调用
    Module.onRuntimeInitialized = function() {
        Module.InitWwise();
        Module.LoadPreloadBanks();
        //Module.LoadBank("Resources/Init.bnk");
        document.getElementById('playmusic').addEventListener('click', function () {
            
            const tempCallback = function(callbackType){
                //console.log("callbackType: " + callbackType);
                if(callbackType == AkCallbackType.AK_EndOfEvent){
                    console.log("EndOfEvent");
                }
                else if(callbackType == AkCallbackType.AK_MusicSyncBeat){
                    changeColor();
                    //console.log("MusicSyncBeat");
                }
            }
            const callbackID = addCallback(tempCallback);
            callbackMap.set(callbackID, tempCallback);

            playingID = Module.PostEventByName("music",1,AkCallbackType.AK_EndOfEvent|AkCallbackType.AK_MusicSyncBeat,callbackID);

            //res = Module.PostEventById(1157699969,1，，);
            //console.log("PostEvent playingID: " + playingID);
        });
        document.getElementById('playsfx').addEventListener('click', function () {
            playingID = Module.PostEventByName("test_random",1,AkCallbackType.AK_EndOfEvent,0);
        });
        document.getElementById('stopMusic').addEventListener('click', function () {
            Module.ExecuteByPlayingID(AkActionOnEventType.AkActionOnEventType_Stop,playingID,100);
        });

        const slider = document.getElementById('slider');
        const sliderValue = document.getElementById('sliderValue');

        slider.addEventListener('input', function() {
            sliderValue.textContent = slider.value;
            Module.SetRtpcValue("music_lowpass_freq", slider.value, 0, 0);
        });
    };
};

document.body.appendChild(script);
