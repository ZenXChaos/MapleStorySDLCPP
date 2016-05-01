
namespace MOB_EXT {
	template<class T>
	class MobExt
	{
		int index;

		std::vector<T> entities;

		T* e_Index;
	public:

		MobExt<T>* MOB_EXT::MobExt<T>::push(T e);
		MobExt<T>* MOB_EXT::MobExt<T>::next();


		MobExt<T>& operator++ ();
		MobExt<T> operator++ (int);
	};

	template<class T>
	MobExt<T> MobExt<T>::operator++(int)
	{
		if (this->entities.size() - 1 < index + 1) {
			e_Index = nullptr;
		}
		else {
			index++;
			this->e_Index = this->entities.at(index);
		}
	}
}