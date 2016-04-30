#ifndef GLOBAL_H
#define GLOBAL_H
class Entity;
class MessageDispatch;
class Player;
class AnimatedSprite;

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
		void clear();
		void push(T const&);  // push element 
		void pop();               // pop element 
		T top() const;            // return top element 
		bool empty() const {       // return true if empty.
			return elems.empty();
		}
	};

	template <class T>
	void GLOBAL_MMORPG_GAME::SpawnManagerMobs<T>::clear()
	{
		this->elems.clear();
	}

	template <class T>
	void GLOBAL_MMORPG_GAME::SpawnManagerMobs<T>::push(T const&)
	{
		elems.push(T);
	}

	template <class T>
	T GLOBAL_MMORPG_GAME::SpawnManagerMobs<T>::top() const
	{
		return elems.begin();
	}

	template <class T>
	void GLOBAL_MMORPG_GAME::SpawnManagerMobs<T>::pop()
	{
		elems.erase(T);
	}

	static Player* m_Player;
	static void* m_SpawnManager;
	static void* m_GameMain;

	static SpawnManagerMobs<Entity*> mobs_spawned;
	static DispatchedMobMessageHandles<std::string, MessageDispatch*> EntityDispatchedHandles;

	static std::map<std::string, void*> m_entities;
	static std::map<std::string, void*> m_MsgDispatch;
	static void RegisterEntity(std::string entity_id, Entity* e) {
		m_entities.insert(std::pair<std::string, void*>(entity_id, nullptr));
		m_entities[entity_id] = e;

	}

	static Player* GetMainPlayer() {
		return static_cast<Player*>(m_Player);
	}
}
#endif