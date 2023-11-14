#pragma once

#include <array>
#include "CsrGraph.h"


struct UnsMesh2D
{
	static constexpr int invalid_id = -1; // ��Ч�ı�ţ�ͨ����ʾԪ�ز�����

	int
		nnode{ 0 }, // �ڵ����
		nface{ 0 }, // �����
		ncell{ 0 }; // ��Ԫ����

	std::vector<double> x, y; // �ڵ�����

	CsrGraph cell_nodes; // ��Ԫ��Χ�ڵ���Ϣ

	std::vector<std::array<int, 2>>
		face_nodes, // ����Χ�ڵ���Ϣ
		face_cells  // ����Χ��Ԫ��Ϣ
		;

	//! @brief ��ȡgmsh��ʽ�������ļ�
	//! @param file �ļ������ַ���
	void read_gmsh_file(const char* file);

	void test();
};