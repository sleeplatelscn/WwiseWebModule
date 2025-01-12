
#include <iostream>
#include "Application.h"
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <emscripten/fetch.h>

#include <string>
//#include <sstream>

using namespace emscripten;

std::string extractFilename(const std::string& path) {
    size_t lastSlashPos = path.find_last_of('/');
    if (lastSlashPos == std::string::npos || lastSlashPos == path.length() - 1) {
        return path;
    }
    return path.substr(lastSlashPos + 1);
}


extern "C" {

void WwiseCallBackFuncCPP(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
{
	std::cout<<"WwiseCallback,"<<static_cast<unsigned int>(in_eType)<<","<<reinterpret_cast<unsigned int>(in_pCallbackInfo->pCookie)<<std::endl;
}

void main_loop_iteration()
{
	Application::GetInstance().Update();
}


void InitWwise()
{
	Application::GetInstance().Start();
	printf("initial setup complete.\n");
	emscripten_set_main_loop(main_loop_iteration, 0, 0);
}

void onerror(emscripten_fetch_t *fetch) {
    printf("Download failed!\n");
    emscripten_fetch_close(fetch); 
}



void onload(emscripten_fetch_t *fetch)
{
	std::string filename = extractFilename(fetch->url);
	std::cout <<filename<< " Download completed!\n";
	Application::GetInstance().LoadBank(filename.c_str());
	emscripten_fetch_close(fetch);
}

void LoadPreloadBanks()
{
	Application::GetInstance().LoadPreloadBanks();
}

void LoadBank(const std::string& str)
{
	emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = onload;
    attr.onerror = onerror;
    emscripten_fetch(&attr,str.c_str());
}

unsigned int PostEventById(unsigned id, unsigned int gameObjID, unsigned int callbackType,unsigned int callbackFunc) 
{
	void* in_pCookie = reinterpret_cast<void*>(callbackFunc);
	return Application::GetInstance().PostEventById(id, gameObjID, static_cast<AkCallbackType>(callbackType),WwiseCallBackFuncCPP,in_pCookie);
}

unsigned int PostEventByName(const std::string& str, unsigned int gameObjID, unsigned int callbackType,unsigned int callbackFunc)
{
	void* in_pCookie = reinterpret_cast<void*>(callbackFunc);
	return Application::GetInstance().PostEventByName(str.c_str(), gameObjID, static_cast<AkCallbackType>(callbackType),WwiseCallBackFuncCPP,in_pCookie);
}

unsigned int SetRtpcValue(const std::string& str, float value, unsigned int gameObjID, unsigned int msTime)
{
	AkGameObjectID id = static_cast<AkGameObjectID>(gameObjID);
	if(gameObjID==0)
	{
		id=AK_INVALID_GAME_OBJECT;
	}
	return AK::SoundEngine::SetRTPCValue(str.c_str(), static_cast<AkRtpcValue>(value), id,msTime);
}
unsigned int RegisterGameObj(unsigned int id)
{
	return AK::SoundEngine::RegisterGameObj(static_cast<AkGameObjectID>(id));
}
void ExecuteByPlayingID(unsigned int type,unsigned int playingID, unsigned int msTime)
{
	AK::SoundEngine::ExecuteActionOnPlayingID(static_cast<AK::SoundEngine::AkActionOnEventType>(type),static_cast<AkPlayingID>(playingID) ,
	msTime);
}


// export
EMSCRIPTEN_BINDINGS(WwiseModule)
{
	function("InitWwise", &InitWwise);
	function("LoadBank", &LoadBank);
	function("LoadPreloadBanks", &LoadPreloadBanks);
	function("PostEventById", &PostEventById);
	function("PostEventByName", &PostEventByName);
	function("SetRtpcValue", &SetRtpcValue);
	function("RegisterGameObj", &RegisterGameObj);
	function("ExecuteByPlayingID", &ExecuteByPlayingID);
}
}