#pragma once
#include "Intergration.h"
#include <iostream>
#define FRAME_TIME 16

class Application
{
public:
	static Application& GetInstance();
	const AkGameObjectID listenerID = 10000;
	void Start();
	void Update();
	void Stop();
	void LoadBank(const char* bankName);
	void LoadPreloadBanks();
	Application();
	~Application();
	AkPlayingID PostEventById(unsigned id, AkGameObjectID gameObjID,AkCallbackType callbackType, AkCallbackFunc callbackFunc,void * in_pCookie);
	AkPlayingID PostEventByName(const char* eventName, AkGameObjectID gameObjID,AkCallbackType callbackType, AkCallbackFunc callbackFunc,void * in_pCookie);
private:
	AkVector64 position;
	AkVector orientationFront;
	AkVector orientationTop;
	AkSoundPosition soundPos;
	
	static void CallBackFunc(AkCallbackType in_eType,AkCallbackInfo* in_pCallbackInfo);
};