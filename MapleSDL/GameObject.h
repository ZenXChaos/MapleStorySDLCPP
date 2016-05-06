#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
enum GAMEObjectType {
	g_Player = 0, g_Mob = 1, g_Skill = 2, g_Misc = 3
};

class GAMEObject {
protected:
public:
	Uint32 OID = 0;
	std::string UniqID = "";

	GAMEObjectType type = GAMEObjectType::g_Misc;
	
	std::string LayerID = "default";
	GAMEObject();
	virtual ~GAMEObject();
};

template<class T>
class GameObject
{
public:

	std::vector<T*> objects;
	std::vector<std::string> layers;

	void Instantiate(T* obj = nullptr, std::string layer = "default") {
		obj->OID = this->objects.size() + 1;
		obj->UniqID = GameUtils::UniqID();
		obj->LayerID = layer;
		this->objects.insert(this->objects.end(), obj);
	}

	void Remove(int index) {
		std::vector<T>::iterator it;
		int iindex = 0;
		for (it = this->objects.begin(); it != this->objects.end(); it++) {

			if (index == iindex) {
				this->Remove(it);
			}else{
				iindex++;
			}
		}
	}

	void Remove(T obj) {
		this->objects.erase(obj);
	}

	void Manage();

	T* Find(std::string UniqID)
	{
		std::vector<T*>::iterator it;

		it = this->objects.begin();
		for (it = this->objects.begin(); it != this->objects.end(); it++) {
			if ((*it)->UniqID == UniqID) {
				return (*it);
				
			}
		}

		return nullptr;
	}
};
#endif