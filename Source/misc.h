#define RandomInt(min, max) (rand() % (max - min + 1) + min)

void LocationSpam() {
	static char* name;
	PlayerInfo_t Info;
	CBaseEntity* Entity = NULL;
	while (!Entity) {
		int index = RandomInt(0, 64);
		Entity = (CBaseEntity*)ClientEntityList->GetClientEntity(index);
		if (!Entity) return;
		if (!Entity->GetHealth() > 0) return;
		EngineClient->GetPlayerInfo(index, &Info);
		name = Info.szName;
		if (name == "GOTV" || name == "") return;
		char szCmd[256];
		sprintf_s(szCmd, "say \"%s is at %s with %d health!\"", name, Entity->GetLastPlace(), Entity->GetHealth());
		EngineClient->CliendCmd_Unrestricted(szCmd);
		return;
	}

}


void ShowRank() {
	static auto RevealRank = reinterpret_cast<bool(__cdecl*)(int*)>(Tools->FindTextPattern("client.dll", "55 8B EC 8B 0D ? ? ? ? 68"));
	int fArray[3] = { 0,0,0 };

	if (RevealRank != NULL)
		RevealRank(fArray);
}