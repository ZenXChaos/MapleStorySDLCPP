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
		if (obj == nullptr) {
			return;
		}
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
		std::for_each(begin(this->objects), end(this->objects), [&](T* object) {

			if (object == nullptr) {
				return;
			}
			if (!object->active) {
				this->objects.erase(std::remove(this->objects.begin(), this->objects.end(), object));
				return;
			}
			object->Core();

			return;
		});
	}

	int distanceFrom(T* obj) {
		if (obj == nullptr) {
			return -1;
		}
		return this->pos->x > obj->pos-> ? this->pos->x - obj->pos->x : obj->pos->x - this->pos->x;
	}

	T* Find(std::string UniqID)
	{
		//Search for an entity
		//Loop through all objects
		T* found = nullptr;
		std::for_each(begin(this->objects), end(this->objects), [&](T* object) {
			if (object == nullptr) {
				return nullptr;
				//Just in case?
			}

			//If the object is active
			//If object UniqID is equal to searched ID
			if (!object->active && object->uniq_id == UniqID) {
				found = object;
			}

			return object;
		});


		return found;
	}
};
#endif
