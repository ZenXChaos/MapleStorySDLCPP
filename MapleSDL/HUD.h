#ifndef HUD_H
#define HUD_H

class HUDObject {
public:
	int row = 0, column = 0;
	int localX = 0, localY = 0;

	std::string e_ID = "0"; //Effector ID

	AnimatedSprite* sprites;
};

template <class T>
class HUD_Animation {
public:
	template<class T>
	void ApplyEffect(T* h_Obj, void(*effect)(T* obj)) {
		effect(h_Obj);
	}

	template<class T>
	void RegisterHUDEffect(T* h_Obj) {
		if (registeredEffects[h_Obj->e_ID].e_ID != "") {
			h_Obj->localY = registeredEffects[h_Obj->e_ID].localY;
			h_Obj->localX = registeredEffects[h_Obj->e_ID].localX;
		}
		else {
			registeredEffects[h_Obj->e_ID] = *h_Obj;
		}
	}
};

class HUD_GridPanel {
public:
	int columns = 0, rows = 0;
	int width = 0, height = 0;
	int localX = 0, localY = 0;

	std::map<std::string, HUDObject> elements;

	std::string e_ID = "0"; //Effector ID

	void AddObject(std::string elem_name, HUDObject obj);

	void DrawPanel(int x, int y);
};

class HUD_FlowPanel {
public:
	int width = 0, height = 0;
	int spacingY = 5, spacingX = 5;
	int localX = 0, localY = 0;

	std::vector<HUDObject> elements;

	std::string e_ID = "0"; //Effector ID

	void AddObject(HUDObject obj);

	void DrawPanel(int x, int y);
};
static class HUD
{
public:
	HUD();
	virtual ~HUD();

} UIHUD;

#ifndef HUD_EFFECTS
extern void HUD_ANIM_TransitionDown(HUDObject* h_Obj);
extern std::map<std::string, HUDObject> registeredEffects;
#endif
#endif

#ifndef HUD_ELEMENTS
extern std::map<std::string, AnimatedSprite> HUDElements;
#else
#undef HUD_ELEMENTS
std::map<std::string, AnimatedSprite> HUDElements;
#endif