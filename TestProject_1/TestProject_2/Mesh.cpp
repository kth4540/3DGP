#include "stdafx.h"
#include "Mesh.h"
#include "GraphicsPipeline.h"

CPolygon::CPolygon(int nVertices)
{
	m_nVertices = nVertices;
	//vertex 갯수 설정
	
	m_pVertices = new CVertex[nVertices];
	//vertex 갯수만큼 동적할당
}
CPolygon::~CPolygon()
{
	if (m_pVertices)
		delete[] m_pVertices;
	//	할당 해제
}
void CPolygon::SetVertex(int nIndex, CVertex vertex)
{
	if ((0 <= nIndex) && (nIndex < m_nVertices) && m_pVertices)
	//vertex number가 vertex 갯수 이내에 있고, 포인터로 할당되어있을 때
	{
		m_pVertices[nIndex] = vertex;
		//vertex of number에 대입
	}
}
CMesh::CMesh(int nPolygons)
{
	m_nPolygons = nPolygons;
	//폴리곤의 갯수

	m_ppPolygons = new CPolygon * [nPolygons];
	//폴리곤의 갯수만큼 동적할당
}
CMesh::~CMesh()
{
	if (m_ppPolygons)
	{
		for (int i = 0; i < m_nPolygons; i++) if (m_ppPolygons[i])
			delete m_ppPolygons[i];
		delete[] m_ppPolygons;
	}
}
void CMesh::SetPolygon(int nIndex, CPolygon* pPolygon)
{
	//메쉬의 다각형을 설정한다.
	if ((0 <= nIndex) && (nIndex < m_nPolygons))
		m_ppPolygons[nIndex] = pPolygon;
	//vertex와 같은 방식으로 폴리곤 설정
}
void Draw2DLine(HDC hDCFrameBuffer, CPoint3D& f3PreviousProject,
	CPoint3D& f3CurrentProject)
{
	//투영 좌표계의 2점을 화면 좌표계로 변환하고 변환된 두 점(픽셀)을 선분으로 그린다.
	CPoint3D f3Previous =
		CGraphicsPipeline::ScreenTransform(f3PreviousProject);
	CPoint3D f3Current =
		CGraphicsPipeline::ScreenTransform(f3CurrentProject);
	//투영 좌표계의 x,y를 화면 좌표계로 변환 
	
	::MoveToEx(hDCFrameBuffer, (long)f3Previous.x,
		(long)f3Previous.y, NULL);
	::LineTo(hDCFrameBuffer, (long)f3Current.x, (long)f3Current.y);
	//변환된 두 점을 선분으로 그림
}
void CMesh::Render(HDC hDCFrameBuffer)
{
	CPoint3D f3InitialProject, f3PreviousProject, f3Intersect;
	bool bPreviousInside = false, bInitialInside = false,
		bCurrentInside = false, bIntersectInside = false;
	//메쉬를 구성하는 모든 다각형들을 렌더링한다.
	for (int j = 0; j < m_nPolygons; j++)
	{
		int nVertices = m_ppPolygons[j]->m_nVertices;
		//j번째 폴리곤의 vertex 갯수

		CVertex* pVertices = m_ppPolygons[j]->m_pVertices;
		//j번째 폴리곤의 vertex들 복사

		//다각형의 첫 번째 정점을 원근 투영 변환한다.
		f3PreviousProject = f3InitialProject =
			CGraphicsPipeline::Project(pVertices[0].m_f3Position);
		
		//변환된 점이 투영 사각형에 포함되는 가를 계산한다.
		bPreviousInside = bInitialInside = (-1.0f <=
			f3InitialProject.x) && (f3InitialProject.x <= 1.0f) && (-1.0f <=
				f3InitialProject.y) && (f3InitialProject.y <= 1.0f);
		//투영 사각형의 크기 = -1.0f ~ 1.0f 이므로 그 사이에 포함되는지 계산
		
		//다각형을 구성하는 모든 정점들을 원근 투영 변환하고 선분으로 렌더링한다.
		for (int i = 1; i < nVertices; i++)
		{
			CPoint3D f3CurrentProject =
				CGraphicsPipeline::Project(pVertices[i].m_f3Position);

			//변환된 점이 투영 사각형에 포함되는 가를 계산한다.
			bCurrentInside = (-1.0f <= f3CurrentProject.x) &&
				(f3CurrentProject.x <= 1.0f) && (-1.0f <= f3CurrentProject.y) &&
				(f3CurrentProject.y <= 1.0f);
			
			//변환된 점이 투영 사각형에 포함되면 이전 점과 현재 점을 선분으로 그린다.
			if (((f3PreviousProject.z >= 0.0f) || (f3CurrentProject.z >=
				0.0f)) && ((bCurrentInside || bPreviousInside)))
				::Draw2DLine(hDCFrameBuffer, f3PreviousProject, f3CurrentProject);
			f3PreviousProject = f3CurrentProject;
			bPreviousInside = bCurrentInside;
		}

		//다각형의 마지막 정점과 다각형의 시작점을 선분으로 그린다.
		if (((f3PreviousProject.z >= 0.0f) || (f3InitialProject.z >=
			0.0f)) && ((bInitialInside || bPreviousInside)))
			::Draw2DLine(hDCFrameBuffer, f3PreviousProject, f3InitialProject);
	}
}
CCubeMesh::CCubeMesh(float fWidth, float fHeight, float fDepth) :
	CMesh(6)
{
	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;
	
	CPolygon* pFrontFace = new CPolygon(4);
	//앞쪽 폴리곤의 vertex 4개
	pFrontFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight,
		-fHalfDepth));
	pFrontFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight,
		-fHalfDepth));
	pFrontFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight,
		-fHalfDepth));
	pFrontFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight,
		-fHalfDepth));
	SetPolygon(0, pFrontFace);

	CPolygon* pTopFace = new CPolygon(4);
	//윗쪽 폴리곤의 vertex 4개
	pTopFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight,
		+fHalfDepth));
	pTopFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight,
		+fHalfDepth));
	pTopFace->SetVertex(2, CVertex(+fHalfWidth, +fHalfHeight,
		-fHalfDepth));
	pTopFace->SetVertex(3, CVertex(-fHalfWidth, +fHalfHeight,
		-fHalfDepth));
	SetPolygon(1, pTopFace);

	CPolygon* pBackFace = new CPolygon(4);
	//뒷쪽 폴리곤의 vertex 4개
	pBackFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight,
		+fHalfDepth));
	pBackFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight,
		+fHalfDepth));
	pBackFace->SetVertex(2, CVertex(+fHalfWidth, +fHalfHeight,
		+fHalfDepth));
	pBackFace->SetVertex(3, CVertex(-fHalfWidth, +fHalfHeight,
		+fHalfDepth));
	SetPolygon(2, pBackFace);

	CPolygon* pBottomFace = new CPolygon(4);
	//아랫쪽 폴리곤의 vertex 4개
	pBottomFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight,
		-fHalfDepth));
	pBottomFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight,
		-fHalfDepth));
	pBottomFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight,
		+fHalfDepth));
	pBottomFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight,
		+fHalfDepth));
	SetPolygon(3, pBottomFace);

	CPolygon* pLeftFace = new CPolygon(4);
	//왼쪽 폴리곤의 vertex 4개
	pLeftFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight,
		+fHalfDepth));
	pLeftFace->SetVertex(1, CVertex(-fHalfWidth, +fHalfHeight,
		-fHalfDepth));
	pLeftFace->SetVertex(2, CVertex(-fHalfWidth, -fHalfHeight,
		-fHalfDepth));
	pLeftFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight,
		+fHalfDepth));
	SetPolygon(4, pLeftFace);

	CPolygon* pRightFace = new CPolygon(4);
	//오른쪽 폴리곤의 vertex 4개
	pRightFace->SetVertex(0, CVertex(+fHalfWidth, +fHalfHeight,
		-fHalfDepth));
	pRightFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight,
		+fHalfDepth));
	pRightFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight,
		+fHalfDepth));
	pRightFace->SetVertex(3, CVertex(+fHalfWidth, -fHalfHeight,
		-fHalfDepth));
	SetPolygon(5, pRightFace);
}
CCubeMesh::~CCubeMesh()
{
}