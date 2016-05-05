#ifndef INPUT_H
#define INPUT_H
#include <map>
class Input {
	std::map<unsigned char, bool> keysDown;
	std::map<unsigned char, bool> keysUp;
public:
	void KeyDown(unsigned char key);
	void KeyUp(unsigned char key);
	bool IsKeyPressed(unsigned char key);

};
#endif