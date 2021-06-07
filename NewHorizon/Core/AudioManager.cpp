#include "AudioManager.h"
#include "EngineDefine.h"

#include "../Util/AudioHelper.h"
#include "../Util/JsonLoader.h"
#include "../Util/LogUtil.hpp"
using namespace std;
AudioManager* AudioManager::pInstance = nullptr;

AudioManager::AudioManager() {}

AudioManager * AudioManager::getInstance()
{
	if (pInstance == nullptr)
	{
		pInstance = new AudioManager;
		AudioHelper::EvonInit();
	}
	return pInstance;
}

void AudioManager::LoadAllAudios()
{
	auto jsonFile = JsonLoader::getJsonFromFile(AUDIO_CONFIG_FILE);
	auto audioConfig = (*jsonFile)["audio"];

	for (int i = 0; i < audioConfig.size(); i++)//Load all declare files of object
	{
		string audioName = audioConfig[i]["name"].asString();
		string audioPath = audioConfig[i]["path"].asString();

		LoadAudio(audioName, audioPath);
	}

}

void AudioManager::LoadAudio(const std::string & name, const std::string & path)
{
	if (audioGroup.find(name) != audioGroup.end())
	{
		return;
	}

	ALuint audioHandle = AudioHelper::loadWavByAlut(AUDIO_RESOURCE_FOLDER + path);
	audioGroup[name] = audioHandle;
}

void AudioManager::PlayAudio(const std::string & name)
{
	auto audioIter = audioGroup.find(name);
	if (audioIter == audioGroup.end())
	{
		return;
	}

	ALuint audioHandle = (*audioIter).second;
	AudioHelper::playFromBuffer(audioHandle);
}

void AudioManager::StopAudio(const std::string & name)
{
	auto audioIter = audioGroup.find(name);
	if (audioIter == audioGroup.end())
	{
		return;
	}

	ALuint audioHandle = (*audioIter).second;
	AudioHelper::stopByBuffer(audioHandle);
}
