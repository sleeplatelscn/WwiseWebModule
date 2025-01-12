#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkMemoryMgrModule.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include "wwise/scripts/AkFilePackageLowLevelIODeferred.h"
#include <AK/SoundEngine/Common/AkTypes.h>
#include "wwise/scripts/AkJobWorkerMgr.h"
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>


#define DATA_SUMMARY_REFRESH_COOLDOWN 7; // Refresh cooldown affecting the refresh rate of the resource monitor data summary

class Intergration
{
public:
	static Intergration& GetInstance();
	bool Init();
	void GetDefaultSettings();
	void SetBasePath(const AkOSChar* in_pszBasePath);
	CAkFilePackageLowLevelIODeferred* m_pLowLevelIO;
private:
	Intergration();
	~Intergration();
	AkMemSettings			m_memSettings;
	AkStreamMgrSettings		m_stmSettings;
	AkDeviceSettings		m_deviceSettings;
	AkInitSettings			m_initSettings;
	AkPlatformInitSettings	m_platformInitSettings;
	AkMusicSettings			m_musicInit;
	
	static void ResourceMonitorDataCallback(const AkResourceMonitorDataSummary* in_pdataSummary);
	static AkResourceMonitorDataSummary ResourceDataSummary;
	AK::JobWorkerMgr::InitSettings m_jobWorkerSettings;
	static void LocalErrorCallback(
		AK::Monitor::ErrorCode in_eErrorCode,	///< Error code number value
		const AkOSChar* in_pszError,	///< Message or error string to be displayed
		AK::Monitor::ErrorLevel in_eErrorLevel,	///< Specifies whether it should be displayed as a message or an error
		AkPlayingID in_playingID,   ///< Related Playing ID if applicable, AK_INVALID_PLAYING_ID otherwise
		AkGameObjectID in_gameObjID ///< Related Game Object ID if applicable, AK_INVALID_GAME_OBJECT otherwise
	);;
};