#ifndef GLOBAL_H
#define GLOBAL_H
class Entity;
class MessageDispatch;
class Player;
class AnimatedSprite;
class SpawnManager;
class Skill;
template<class T> class GameObject;

namespace GLOBAL_MMORPG_GAME {

	template <class T, class TT>
	class HUDE {
	private:
		std::map<T, TT*> msgs;     // elements 
	};

	template <class T, class TT>
	class DispatchedMobMessageHandles {
	private:
		std::map<T, TT*> msgs;     // elements 

	public:
		DispatchedMobMessageHandles<T, TT>* RegisterEntity(std::string entityid);
		DispatchedMobMessageHandles<T, TT>* RegisterEntityHandle(std::string entityid, MessageDispatch* mdispatch);
		DispatchedMobMessageHandles<T, TT>* SendEntityDispatchedMessage(std::string entityid, std::string msgname, void(*callback)(Entity *e), Entity* sender);
	};

	template <class T, class TT>
	DispatchedMobMessageHandles<T, TT>* GLOBAL_MMORPG_GAME::DispatchedMobMessageHandles<T, TT>::SendEntityDispatchedMessage(std::string entityid, std::string msgname, void(*callback)(Entity *e), Entity* sender)
	{
		this->msgs[T]->RegisterMessage(msgname, callback, e);

		return this;
	}

	template <class T, class TT>
	DispatchedMobMessageHandles<T, TT>* GLOBAL_MMORPG_GAME::DispatchedMobMessageHandles<T, TT>::RegisterEntityHandle(std::string entityid, MessageDispatch* mdispatch)
	{
		this->msgs[entityid] = &mdispatch;

		return this;
	}

	template <class T, class TT>
	DispatchedMobMessageHandles<T, TT>* GLOBAL_MMORPG_GAME::DispatchedMobMessageHandles<T, TT>::RegisterEntity(std::string entityid)
	{
		this->msgs.insert(std::pair<T, TT*>(entityid, nullptr));

		return this;
	}

	template <class T>
	class SpawnManagerMobs {
	private:
		std::vector<T> elems;     // elements 

	public:
		void clear() { this->elems.clear(); }
		void push(T const& t) { this->elems.push(t); }
		void pop(T const &t) { elems.erase(t); }
		T top() const { return elems.begin(); }
		bool empty() const { return elems.empty(); }
	};

	static SpawnManager* m_SpawnManager=nullptr;
	static void* m_GameMain;

	static SpawnManagerMobs<Entity*> mobs_spawned;
	static DispatchedMobMessageHandles<std::string, MessageDispatch*> EntityDispatchedHandles;

	static std::map<std::string, void*> m_entities;
	static std::map<std::string, void*> m_MsgDispatch;
	static void RegisterEntity(std::string entity_id, Entity* e) {
		m_entities.insert(std::pair<std::string, void*>(entity_id, nullptr));
		m_entities[entity_id] = e;

	}

}
#endif

#ifndef MOUSE_HANDLE
extern int MH_mouseX;
extern int MH_mouseY;
extern bool MH_clicked;
#else
#undef MOUSE_HANDLE
int MH_mouseX = 0;
int MH_mouseY = 0;
bool MH_clicked = false;
#endif

#ifndef MAIN_HANDLE
extern SDL_sem* mainLock;
extern SDL_sem* mainSpawnMGRLock;
extern bool mainRunning;
namespace GLOBAL_MMORPG_GAME {
	extern Player* m_Player;
}
extern GameObject<Skill> skillGameObjects;
#else
#undef  MAIN_HANDLE
namespace GLOBAL_MMORPG_GAME {
	Player* m_Player;
}
#endif