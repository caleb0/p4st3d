#define RandomInt(min, max) (rand() % (max - min + 1) + min)

void LocationSpam() {
	static std::string name;
	PlayerInfo_t Info;
	CBaseEntity* Entity = NULL;
	while (!Entity) {
		int index = RandomInt(0, EngineClient->GetMaxClients());
		Entity = (CBaseEntity*)ClientEntityList->GetClientEntity(index);
		if (!Entity) return;
		if (!Entity->GetHealth() > 0) return;
		EngineClient->GetPlayerInfo(index, &Info);
		name = Info.szName;
		if (name == "GOTV" || name == "") return;
		char szCmd[256];
		sprintf(szCmd, "say \"%s is at %s with %d health!\"", name, Entity->GetLastPlace(), Entity->GetHealth());
		EngineClient->CliendCmd_Unrestricted(szCmd);
	}

}
