#pragma once

#include <array>
#include "CsrGraph.h"


struct UnsMesh2D
{
	static constexpr int invalid_id = -1; // 无效的编号，通常表示元素不存在

	int
		nnode{ 0 }, // 节点个数
		nface{ 0 }, // 面个数
		ncell{ 0 }; // 单元个数

	std::vector<double> x, y; // 节点坐标

	CsrGraph cell_nodes; // 单元周围节点信息

	std::vector<std::array<int, 2>>
		face_nodes, // 面周围节点信息
		face_cells  // 面周围单元信息
		;

	//! @brief 读取gmsh格式的网格文件
	//! @param file 文件名字字符串
	void read_gmsh_file(const char* file);

	void test();
};