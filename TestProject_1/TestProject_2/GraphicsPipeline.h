#pragma once
#include "GameObject.h"
#include "Camera.h"

class CGraphicsPipeline
//모델 좌표계의 점을 월드변환,카메라변환,원근투영변환 하기 위한 클래스
{
private:
	static CGameObject* m_pGameObject;
	static CCamera* m_pCamera;
public:
	static void SetGameObject(CGameObject* pGameObject) { m_pGameObject = pGameObject; }
	static void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }
	static CPoint3D ScreenTransform(CPoint3D& f3Projection);
	static CPoint3D Project(CPoint3D& f3Model);
};