#pragma once
#include <list>
#include "Camera.h"

class CameraManager
{
public:
	static CameraManager& getInstance() {
		static CameraManager instance;
		return instance;
	}

	inline std::list<Camera*> getCameras() { return m_cameras; }
	void addCamera(Camera* camera) { m_cameras.push_back(camera); }
	void removeCamera(Camera* camera) { m_cameras.remove(camera); }
	void render(GameObject* rootObject);
	Camera* getCurrentCamera() { return m_currentCamera; }

	void operator=(CameraManager const&) = delete;
private:
	CameraManager();
	~CameraManager();

	CameraManager(CameraManager const&);

	static CameraManager m_instance;

	std::list<Camera*> m_cameras;
	Camera* m_currentCamera;
};