#pragma once
#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_
#include <map>
#include <string>
class AudioManager {
private:
	std::map<std::string, unsigned int> audioGroup;


	static AudioManager *pInstance;

	AudioManager();
public:
	static AudioManager* getInstance();

	void LoadAllAudios();

	void LoadAudio(const std::string& name, const std::string& path);

	void PlayAudio(const std::string& name);
	bool IsPlaying(const std::string& name);
	void StopAudio(const std::string& name);

};

#endif