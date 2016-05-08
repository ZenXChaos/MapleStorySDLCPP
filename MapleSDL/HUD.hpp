#ifndef HUD_H
#define HUD_H

static class HUD
{
public:
	HUD();
	virtual ~HUD();

	static void readMouseInput();

} UHUD_m;

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

class HUD_Button {

public:
	AnimatedSprite normal;
	AnimatedSprite pressed;
	AnimatedSprite hover;
	AnimatedSprite disabled;

	std::map<std::string, void(*)(void*)> methods;

	bool clicked = false;

	void Present(SDL_Rect pos);

	void BindAction(std::string method, void(*methodFunc)(void* context), void* context);

	HUD_Button();
	virtual ~HUD_Button();
	bool entered = false;
	bool mouseleft = false;
	bool registerOutOfBoundUp = false;
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

class HUD_TextBlock {
	std::map<int, int> mapping_wordcount; // So we know how many letters each word has in it.
public:
	enum TextWrapping {
		WrapLetter, WrapWord
	};

	TextWrapping TextWrapMode = TextWrapping::WrapWord;
	std::vector<std::string> words;

	int width = 0, height = 0;
	int letterSpacing = 2, lineSpacing = 5, wordSpacing = 10;
	int localX = 0, localY = 0;

	std::vector<HUDObject> elements;

	std::string e_ID = "0"; //Effector ID

	void AddWObject(std::string word);

	void DrawPanel(int x, int y);
};


#ifndef HUD_EFFECTS
extern void HUD_ANIM_TransitionDown(HUDObject* h_Obj);
extern void HUD_ANIM_DMGNO_TransitionUp(HUDObject* h_Obj);
extern std::map<std::string, HUDObject> registeredEffects;
#endif
#endif

#ifndef HUD_ELEMENTS
extern std::map<std::string, AnimatedSprite> HUDElements;
#else
#undef HUD_ELEMENTS
std::map<std::string, AnimatedSprite> HUDElements;
#endif