#include "stdafx.h"
#include "GraphicsPipeline.h"

CGameObject* CGraphicsPipeline::m_pGameObject = NULL;
CCamera* CGraphicsPipeline::m_pCamera = NULL;

CPoint3D CGraphicsPipeline::Project(CPoint3D& f3Model)
//모델좌표계의 점을 월드,카메라,투영 순으로 순차적 변환
//월드변환은 객체의 정보(위치,방향), 카메라 변환은 카메라의 정보(위치,방향)
//투영 변환은 카메라의 정보(시야각,종횡비)가 필요
{
	CPoint3D f3World = m_pGameObject->WorldTransform(f3Model);
	CPoint3D f3Camera = m_pCamera->CameraTransform(f3World);
	CPoint3D f3Projection = m_pCamera->ProjectionTransform(f3Camera);
	return(f3Projection);
}
CPoint3D CGraphicsPipeline::ScreenTransform(CPoint3D& f3Projection)
{
	CPoint3D f3Screen = m_pCamera->ScreenTransform(f3Projection);
	return(f3Screen);
}