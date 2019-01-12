#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <map>

#include "EntityParam.h"



class IGESFileParser
{
public:
	IGESFileParser(std::string);
	IGESFileParser() {};
	~IGESFileParser();

	void setIGESFileName(char* fileName);

	void infoByteParse();							 // ��ȡIGES�ļ���Ϣ
	void GInfoSegProcess();
	void DInfoSegProcess();
	void PInfoSegProcess();

	int getNumOfEntity() { return PInfoStorageClass.size(); };
	EntityParam* getEachEntityClass(int num = 0);

private:
	int infoLength;									// IGES�ļ��ַ����鳤��
	std::ifstream IGESFileIn;						// ���ļ�����ʼ����������������ϵ
	std::string lineType;							// ������string���͸���һЩ
	const short lineCharCount = 80;
	const short lineInfoCount = 72;
	std::string infoByteStorage;					// IGES�ļ��ַ�ASCII������

	std::array<std::string, 25> GStringStorage;		// ����ʹ��array��vectorЧ�ʸ�
	std::map<short, float> GNumStorage;				// �洢GStringStorage������Ӧ�ĸ�����

	struct DInfoStorageType {
		int entityTypeNum;							// ʵ�����ͺ�
		int paramStartLineInd;						// ��������
		int transformMatPtr;						// �任����
		int colorNumOrPtr;							// ��ɫ��
		int formNum;
	};
	std::vector<DInfoStorageType> DInfoStorage;

	std::vector< EntityParam* > PInfoStorageClass;	// ��̬�洢ʵ��������Ϣ ���ǲ���������
};

