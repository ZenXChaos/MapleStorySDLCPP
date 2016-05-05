#ifndef CAMERA_H
#define CAMERA_H
class Camera
{
public:
	LFRect pos = { 0,0,0,0 };

	Camera();
	virtual ~Camera();
};
#endif

#ifndef MAIN_CAMERA
extern Camera MainCamera;
#else
#undef MAIN_CAMERA
Camera MainCamera;
#endif