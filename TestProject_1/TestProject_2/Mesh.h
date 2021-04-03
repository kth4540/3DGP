#pragma once
class CPoint3D
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	//점의 x,y,z 좌표

	CPoint3D() { }
	CPoint3D(float x, float y, float z)
	{
		this->x = x; 
		this->y = y;
		this->z = z;
	}
	virtual ~CPoint3D() { }
	
};
class CVertex
{
public:
	CPoint3D m_f3Position;	// 점의 float 좌표(x,y,z)
	CVertex() { }
	CVertex(float x, float y, float z) 
	{
		m_f3Position = CPoint3D(x, y,z);
	}
	virtual ~CVertex() { }
};
class CPolygon
{
public:
	CPolygon() { }
	CPolygon(int nVertices);
	virtual ~CPolygon();

	//다각형(면)을 구성하는 정점들의 리스트이다.
	int m_nVertices = 0;	// vertex의 갯수
	CVertex* m_pVertices = NULL;	// vertex들을 가리키는 포인터
	void SetVertex(int nIndex, CVertex vertex);
};
class CMesh
{
public:
	CMesh() { }
	CMesh(int nPolygons);
	virtual ~CMesh();
private:
	//인스턴싱(Instancing)을 위하여 메쉬는 게임 객체들에 공유될 수 있다.
	//다음 참조값(Reference Count)은 메쉬가 공유되는 게임 객체의 개수를 나타낸다.
	int m_nReferences = 1;	//-> 메쉬가 공유되는 객체의 수
public:
	//메쉬가 게임 객체에 공유될 때마다 참조값을 1씩 증가시킨다.
	void AddRef() { m_nReferences++; }	//-> 객체 추가

	//메쉬를 공유하는 게임 객체가 소멸될 때마다 참조값을 1씩 감소시킨다.
	//참조값이 0이되면 메쉬를 소멸시킨다.
	void Release() 
	{
		m_nReferences--;	//-> 객체 1개 감소
		if (m_nReferences <= 0)	
			delete this;	//-> 객체 수가 0이 되면 메쉬 소멸
	}
private:
	//메쉬를 구성하는 다각형(면)들의 리스트이다.
	int m_nPolygons = 0;	
	//-> 메쉬를 구성하는 폴리곤의 수
	
	CPolygon** m_ppPolygons = NULL;	
	//-> 폴리곤을 가리키는 포인터
public:
	void SetPolygon(int nIndex, CPolygon* pPolygon);
	//-> 폴리곤 설정
	
	//메쉬를 렌더링한다.
	virtual void Render(HDC hDCFrameBuffer);
};
//직육면체 클래스를 선언한다.
class CCubeMesh : public CMesh
{
public:
	CCubeMesh(float fWidth = 4.0f, float fHeight = 4.0f, float fDepth= 4.0f);
	virtual ~CCubeMesh();
};