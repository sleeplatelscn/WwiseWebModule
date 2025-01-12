#include "Application.h"

Application::Application()
{
}
Application::~Application()
{
}
Application& Application::GetInstance()
{
	static Application instance;
	return instance;
}
void Application::Start()
{
	Intergration::GetInstance().GetDefaultSettings();
	auto res = Intergration::GetInstance().Init();

	AK::SoundEngine::RegisterGameObj(1,"emitter");
	AK::SoundEngine::RegisterGameObj(listenerID, "Listener (Default)");
	AK::SoundEngine::AddDefaultListener(listenerID);
	// auto id=AK::SoundEngine::GetIDFromString("test_random");
	// std::cout << "test_random id: " << id << std::endl;
	// auto playingID = AK::SoundEngine::PostEvent(
	// 	 			"test_random", 
	// 	 			1, 
	// 	 			AkCallbackType::AK_EndOfEvent, 
	// 				Application::CallBackFunc 
	// 	 		);
	// std::cout << "Event posted. playingId: " << playingID << std::endl;
}
void Application::LoadBank(const char* bankName)
{
	AkBankID bankID;
	auto res = AK::SoundEngine::LoadBank(bankName, bankID);
	if (res != AK_Success)
	{
		std::cout << res << " Could not load bank " <<bankName<< std::endl;
	}
	else
	{
		std::cout<<res << "bank loaded " <<bankName<< std::endl;
	}
}
void Application::LoadPreloadBanks()
{
	AkBankID bankID;
	auto res = AK::SoundEngine::LoadBank("Init.bnk", bankID);
	if (res != AK_Success)
	{
		std::cout << res << " Could not load bank " <<"Init.bnk"<< std::endl;
	}
	else
	{
		std::cout<<res << "bank loaded " <<"Init.bnk"<< std::endl;
	}
	res = AK::SoundEngine::LoadBank("Main.bnk", bankID);
	if (res != AK_Success)
	{
		std::cout << res << " Could not load bank " <<"Main.bnk"<< std::endl;
	}
	else
	{
		std::cout<<res << "bank loaded " <<"Main.bnk"<< std::endl;
	}
	//PostEventByName("test_random", 1, AkCallbackType::AK_EndOfEvent, CallBackFunc, nullptr);
}
void Application::Update()
{

	AK::SoundEngine::RenderAudio();

}
void Application::Stop()
{
	AK::SoundEngine::UnregisterGameObj(1);
	AK::SoundEngine::UnregisterGameObj(10000);
	AK::SoundEngine::UnloadBank("Main.bnk", NULL);
	AK::SoundEngine::UnloadBank("Init.bnk", NULL);
	AK::MusicEngine::Term();

	if (AK::SoundEngine::IsInitialized())
	{
		AK::SoundEngine::Term();
	}

	AK::JobWorkerMgr::TermWorkers();
	
	Intergration::GetInstance().m_pLowLevelIO->Term();
	AK::IAkStreamMgr::Get()->Destroy();
	if (AK::MemoryMgr::IsInitialized())
	{
		AK::MemoryMgr::Term();
	}
}
void Application::CallBackFunc(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
{
	if (in_eType == AkCallbackType::AK_EndOfEvent)
	{
		std::cout << "done game object id " << in_pCallbackInfo->gameObjID<< std::endl;
	}
}
AkPlayingID Application::PostEventByName(const char* eventName, AkGameObjectID gameObjID, AkCallbackType callbackType, AkCallbackFunc callbackFunc,void * in_pCookie)
{
	auto playingID = AK::SoundEngine::PostEvent(
		eventName,
		gameObjID,
		callbackType,
		callbackFunc,
		in_pCookie
	);
	std::cout <<eventName<< " Event posted. playingId: " << playingID << std::endl;
	return playingID;
}

AkPlayingID Application::PostEventById(unsigned id, AkGameObjectID gameObjID, AkCallbackType callbackType, AkCallbackFunc callbackFunc,void * in_pCookie)
{
	
	auto playingID = AK::SoundEngine::PostEvent(
		id,
		gameObjID,
		callbackType,
		callbackFunc,
		in_pCookie);

	std::cout << id << " Event posted. playingId: " << playingID << std::endl;
	return playingID;
}