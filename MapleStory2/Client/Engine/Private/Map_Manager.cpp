#include "..\Public\Map_Manager.h"

IMPLEMENT_SINGLETON(CMap_Manager)

CMap_Manager::CMap_Manager()
{
	
}


void CMap_Manager::SetFildNames()
{
	m_MapNames.push_back(string("Map_Henesys"));
	m_MapNames.push_back(string("Map_HenesysHunting"));
	m_MapNames.push_back(string("Map_Elenya"));
	m_MapNames.push_back(string("Map_ElenyaEnter"));
	m_MapNames.push_back(string("Map_GAS"));

	m_MapNames.push_back(string("Map_DHenesys"));
	m_MapNames.push_back(string("Map_DHenesysHunting"));
	m_MapNames.push_back(string("Map_DMini"));
	m_MapNames.push_back(string("Map_DElenyaEnter"));
	m_MapNames.push_back(string("Map_DElenya"));
	m_MapNames.push_back(string("Map_BlackMageJump"));
	m_MapNames.push_back(string("Map_DarkMageEnter"));
	m_MapNames.push_back(string("Map_DarkMage"));
	m_MapNames.push_back(string("Map_WhiteMageds"));
	m_MapNames.push_back(string("Map_END"));


	
	


	m_VoxelNames.push_back(string("Model_House1"));
	m_VoxelNames.push_back(string("Model_House2"));
	m_VoxelNames.push_back(string("Model_Wood1"));
	m_VoxelNames.push_back(string("Model_Wood2"));

	m_VoxelNames.push_back(string("Model_DHouse1"));
	m_VoxelNames.push_back(string("Model_DHouse2"));
	m_VoxelNames.push_back(string("Model_DMood1"));
	m_VoxelNames.push_back(string("Model_DWood2"));

	m_VoxelNames.push_back(string("Model_Die"));
	m_VoxelNames.push_back(string("Model_Lock"));
	m_VoxelNames.push_back(string("Model_MiniOrrange"));
	m_VoxelNames.push_back(string("Model_MiniRed"));
	m_VoxelNames.push_back(string("Model_MiniYerrow"));

	m_cVoxelNames.push_back(L"Model_House1");
	m_cVoxelNames.push_back(L"Model_House2");
	m_cVoxelNames.push_back(L"Model_Wood1");
	m_cVoxelNames.push_back(L"Model_Wood2");

	m_cVoxelNames.push_back(L"Model_DHouse1");
	m_cVoxelNames.push_back(L"Model_DHouse2");
	m_cVoxelNames.push_back(L"Model_DMood1");
	m_cVoxelNames.push_back(L"Model_DWood2");

	m_cVoxelNames.push_back(L"Model_Die");
	m_cVoxelNames.push_back(L"Model_Lock");
	m_cVoxelNames.push_back(L"Model_MiniOrrange");
	m_cVoxelNames.push_back(L"Model_MiniRed");
	m_cVoxelNames.push_back(L"Model_MiniYerrow");








	m_ModelNames.push_back(string("Total_Henesys"));
	m_ModelNames.push_back(string("Total_HenesysHunting"));
	m_ModelNames.push_back(string("Model_ElenyaEnter"));
	m_ModelNames.push_back(string("Total_Elenya"));

	m_ModelNames.push_back(string("Total_DHenesys"));
	//m_ModelNames.push_back(string("Total_DHenesysHunting"));
	m_ModelNames.push_back(string("Total_DElenyaEnter"));

}


HRESULT CMap_Manager::LoadMapData(HWND hWnd)
{
	SetFildNames();
	
	LoadData(hWnd);
	LoadModel(hWnd);
	LoadModelState(hWnd);

	return S_OK;
}

list<CMap_Manager::CUBEDATA>* CMap_Manager::ReadMap(const _tchar * pMapTag)
{
	list<CUBEDATA>* tempList = Find_Map(pMapTag);
	if (nullptr == tempList)
		return nullptr;

	return tempList;
}


list<CMap_Manager::CUBEDATA>* CMap_Manager::Find_Map(const _tchar * pMapTag)
{
	auto	iter = find_if(m_MapPrototypes.begin(), m_MapPrototypes.end(), CTag_Finder(pMapTag));

	if (iter == m_MapPrototypes.end())
		return nullptr;

	return &(iter->second);
}


list<CMap_Manager::MODELDESC>* CMap_Manager::Find_Model(const _tchar* pModelTag)
{
	auto	iter = find_if(m_ModelPrototypes.begin(), m_ModelPrototypes.end(), CTag_Finder(pModelTag));

	if (iter == m_ModelPrototypes.end())
		return nullptr;

	return &(iter->second);
}


void CMap_Manager::LoadData(HWND hWnd)
{

	for (auto& name : m_MapNames)
	{

		string FilePath = name;
		string temp = "../Data/Map/";

		FilePath = temp + FilePath + ".dat";

		wchar_t RealPath[256] = { 0 };

		for (int i = 0; i < FilePath.size(); i++)
		{
			RealPath[i] = FilePath[i];
		}





		HANDLE		hFile = CreateFile(RealPath,
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);



		DWORD		dwByte = 0;
		CMap_Manager::CUBEDATA		tInfo{};

		wchar_t* sFildName = new wchar_t[256];
		ReadFile(hFile, sFildName, sizeof(wchar_t) * 256, &dwByte, nullptr);
		delete sFildName;

		_float4x4 RotationMatrix;
		D3DXMatrixIdentity(&RotationMatrix);
		D3DXMatrixRotationAxis(&RotationMatrix, &_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(45.f));



		while (true)
		{
			ReadFile(hFile, &tInfo, sizeof(CMap_Manager::CUBEDATA), &dwByte, nullptr);

			if (0 == dwByte)	// 더이상 읽을 데이터가 없을 경우
				break;

			CMap_Manager::CUBEDATA CubeData(tInfo);


			D3DXVec3TransformCoord(&CubeData.vPos, &CubeData.vPos, &RotationMatrix);
			// CubeData.vPos.y += 10.f;


			m_TempList.push_back(CubeData);
		}

		// const wchar_t* pTemp = sFildName;
		
		_tchar* pCtemp = new _tchar[256];
		ZeroMemory(pCtemp, sizeof(_tchar) * 256);

		for (int i = 0; i < name.size(); i++)
		{
			pCtemp[i] = name[i];
		}

		m_MapPrototypes.emplace(pCtemp, m_TempList);
		m_TempList.clear();

		// 3. 파일 소멸
		CloseHandle(hFile);

	}

}
void CMap_Manager::LoadModel(HWND hWnd)
{
	for (auto& name : m_VoxelNames)
	{

		string FilePath = name;
		string temp = "../Data/Voxel/";

		FilePath = temp + FilePath + ".dat";

		wchar_t RealPath[256] = { 0 };

		for (int i = 0; i < FilePath.size(); i++)
		{
			RealPath[i] = FilePath[i];
		}





		HANDLE		hFile = CreateFile(RealPath,
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		DWORD		dwByte = 0;
		CMap_Manager::CUBEDATA		tInfo{};

		wchar_t* sFildName = new wchar_t[256];
		ReadFile(hFile, sFildName, sizeof(wchar_t) * 256, &dwByte, nullptr);

		while (true)
		{
			ReadFile(hFile, &tInfo, sizeof(CMap_Manager::CUBEDATA), &dwByte, nullptr);

			if (0 == dwByte)	// 더이상 읽을 데이터가 없을 경우
				break;

			CMap_Manager::CUBEDATA CubeData(tInfo);


			m_TempList.push_back(CubeData);
		}

		const wchar_t* pTemp = sFildName;

		m_MapPrototypes.emplace(pTemp, m_TempList);
		m_TempList.clear();

		// 3. 파일 소멸
		CloseHandle(hFile);

	}

}
void CMap_Manager::LoadModelState(HWND hWnd)
{

	for (auto& name : m_ModelNames)
	{

		string FilePath = name;
		string temp = "../Data/Mod/";

		FilePath = temp + FilePath + ".dat";

		wchar_t RealPath[256] = { 0 };

		for (int i = 0; i < FilePath.size(); i++)
		{
			RealPath[i] = FilePath[i];
		}





		HANDLE		hFile = CreateFile(RealPath,
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		DWORD		dwByte = 0;
		CMap_Manager::MODELDESC		tInfo{};

		wchar_t* sFildName = new wchar_t[256];
		ReadFile(hFile, sFildName, sizeof(wchar_t) * 256, &dwByte, nullptr);

		while (true)
		{
			ReadFile(hFile, &tInfo, sizeof(CMap_Manager::MODELDESC), &dwByte, nullptr);

			if (0 == dwByte)	// 더이상 읽을 데이터가 없을 경우
				break;

			CMap_Manager::MODELDESC CubeData(tInfo);


			m_TempModelList.push_back(CubeData);
		}

		m_ModelPrototypes.emplace(sFildName, m_TempModelList);
		m_TempModelList.clear();

		// 3. 파일 소멸
		CloseHandle(hFile);

	}

}



void CMap_Manager::Free()
{
	for (auto& tag : m_MapPrototypes)
		delete tag.first;
	for (auto& tag : m_ModelPrototypes)
		delete tag.first;

}


