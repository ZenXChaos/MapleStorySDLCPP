#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
enum GAMEObjectType {
	g_Player = 0, g_Mob = 1, g_Skill = 2, g_Misc = 3
};

class GAMEObject {
protected:
public:
	bool active = true;

	Uint32 OID = 0;
	std::string UniqID = "";
	std::string LayerID = "default";

	GAMEObjectType type = GAMEObjectType::g_Misc;
	
	GAMEObject();
	virtual ~GAMEObject();

	virtual void Core() = 0;
	
	virtual void OnCreate() {};
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
		obj->OnCreate();
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

	void GameObject<T>::Manage()
	{
		restart:
		std::vector<T*>::iterator it;

		for (it = this->objects.begin(); it != this->objects.end(); it++) {
			(*it)->Core();

			if ((*it)->active == false) {
				this->objects.erase(it);
				goto restart;
			}
		}
	}


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
