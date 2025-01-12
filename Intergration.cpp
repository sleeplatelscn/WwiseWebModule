#include "Intergration.h"
#include <iostream>
#include <cassert>
#include <AK/Plugin/AllPluginsFactories.h>


AkResourceMonitorDataSummary Intergration::ResourceDataSummary;
Intergration& Intergration::GetInstance()
{
	static Intergration instance;
	return instance;
}
Intergration::~Intergration()
{
	if (m_pLowLevelIO != nullptr)
	{
		delete m_pLowLevelIO;
		m_pLowLevelIO = nullptr;
	}
}
Intergration::Intergration()
{
	m_pLowLevelIO = new CAkFilePackageLowLevelIODeferred();
}
void Intergration::GetDefaultSettings()
{
	AK::MemoryMgr::GetDefaultSettings(m_memSettings);
	AK::StreamMgr::GetDefaultSettings(m_stmSettings);
	AK::StreamMgr::GetDefaultDeviceSettings(m_deviceSettings);
	AK::SoundEngine::GetDefaultInitSettings(m_initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(m_platformInitSettings);
	AK::MusicEngine::GetDefaultInitSettings(m_musicInit);
	// The default job worker manager provided in the samples will generate more appropriate settings for the internal job manager of the sound engine.
	//AK::JobWorkerMgr::GetDefaultInitSettings(m_jobWorkerSettings);
	//m_jobWorkerSettings.uNumWorkerThreads = 0;
	// m_initSettings.settingsJobManager = m_jobWorkerSettings.GetJobMgrSettings();
	// for (AkUInt32 uJobType = 0; uJobType < AK_NUM_JOB_TYPES; ++uJobType)
	// {
	// 	m_initSettings.settingsJobManager.uMaxActiveWorkers[uJobType] = 2;
	// }
}
bool Intergration::Init()
{
	auto res = AK::MemoryMgr::Init(&m_memSettings);
	if (res != AK_Success)
	{
		std::cout << res << " Could not initialize the Memory Manager." << std::endl;
	}
	else
	{
		std::cout << "Memory Manager initialized." << std::endl;
	}
	if ( !AK::StreamMgr::Create( m_stmSettings ) )
	{
		std::cout << "Could not create the Streaming Manager." << std::endl;
	}
	else
	{
		std::cout << "Streaming Manager created." << std::endl;
	}
	
	m_deviceSettings.bUseStreamCache = true;
	res=m_pLowLevelIO->Init(m_deviceSettings);
	if (res != AK_Success)
	{
		std::cout << res << " Could not initialize the Low-Level I/O system." << std::endl;
	}
	else
	{
		std::cout << "Low-Level I/O system initialized." << std::endl;
	}
	
	// if (m_jobWorkerSettings.uNumWorkerThreads > 0)
	// {
	// 	res = AK::JobWorkerMgr::InitWorkers(m_jobWorkerSettings);
	// 	if (res != AK_Success)
	// 	{
	// 		std::cout << res << " Could not initialize the Job Worker Manager." << std::endl;
	// 	}
	// }

	res=AK::SoundEngine::Init(&m_initSettings, &m_platformInitSettings);
	if (res != AK_Success)
	{
		std::cout << res<< " Could not initialize the Sound Engine." << std::endl;
		assert(!"Could not initialize the Sound Engine.");
		return false;
	}
	else
	{
		std::cout << "Sound Engine initialized." << std::endl;
	}

	res = AK::MusicEngine::Init(&m_musicInit);
	if (res != AK_Success)
	{
		std::cout << res << " Could not initialize the Music Engine." << std::endl;
	}

	if (AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)")) != AK_Success)
		std::cout << "SetCurrentLanguage faild" << std::endl;

	AkSpatialAudioInitSettings settings;
	res = AK::SpatialAudio::Init(settings);
	if (res != AK_Success)
	{
		std::cout << res << " Could not initialize the Spatial Audio." << std::endl;
	}
	else
	{
		std::cout << "Spatial Audio initialized." << std::endl;
	}
	AK::SoundEngine::RegisterResourceMonitorCallback(ResourceMonitorDataCallback);
	AK::Monitor::SetLocalOutput(AK::Monitor::ErrorLevel_All, LocalErrorCallback);
	m_pLowLevelIO->SetBasePath("Resources/");
	return true;

}
void Intergration::SetBasePath(const AkOSChar* in_pszBasePath)
{
	auto res = m_pLowLevelIO->SetBasePath(in_pszBasePath);
	if (res != AKRESULT::AK_Success)
	{
		std::cout << "set basepath faild" << std::endl;
	}
	else
	{
		std::cout << "set basepath success" << std::endl;
	}
}
void Intergration::ResourceMonitorDataCallback(const AkResourceMonitorDataSummary* in_pdataSummary)
{
	static int ResourceMonitorUpdateCooldown = 0;
	if (ResourceMonitorUpdateCooldown <= 0)
	{
		Intergration::ResourceDataSummary = *in_pdataSummary;
		ResourceMonitorUpdateCooldown = DATA_SUMMARY_REFRESH_COOLDOWN;
	}
	else
		ResourceMonitorUpdateCooldown--;
}

void Intergration::LocalErrorCallback(AK::Monitor::ErrorCode in_eErrorCode, const AkOSChar* in_pszError, AK::Monitor::ErrorLevel in_eErrorLevel, AkPlayingID in_playingID, AkGameObjectID in_gameObjID)
{
	char* pszErrorStr;
	CONVERT_OSCHAR_TO_CHAR(in_pszError, pszErrorStr);
	std::cout << pszErrorStr << std::endl;
}
