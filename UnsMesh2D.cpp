#include "UnsMesh2D.h"
#include "read_gmsh.h"
#include <map>
#include <iostream>

struct Less
{
	using key = std::array<int, 2>;

	bool operator()(const key& lhs, const key& rhs)const {
		int n0_l = std::min(lhs[0], lhs[1]);
		int n1_l = std::max(lhs[0], lhs[1]);
		int n0_r = std::min(rhs[0], rhs[1]);
		int n1_r = std::max(rhs[0], rhs[1]);

		return n0_l < n0_r || (n0_l == n0_l && n1_l < n1_r);
	}
};

void create_faces_from_cell_nodes(const CsrGraph& cell_nodes,
								  std::vector<std::array<int,2>>& face_nodes, 
								  std::vector<std::array<int, 2>>& face_cells) {
	// nodes: [1,2,3]
	// i=0 n0=1 n1=2
	// i=1 n0=2 n1=3
	// i=2 n0=3 n1=1

	// key -- value
	// {n0,n1} -- face_id
	// {n0,n1} == {n1,n0}
	using key = std::array<int, 2>;
	std::map<key, int, Less>faces;

	face_nodes.clear();
	face_cells.clear();

	// ѭ��ÿ����Ԫ
	key fnodes{};
	for (int cell = 0; cell < cell_nodes.nrow(); ++cell) {
		auto nodes = cell_nodes[cell]; // ��Ԫ��Χ�ڵ㣬����

		for (size_t i = 0; i < nodes.size(); i++) {
			fnodes[0] = nodes[i];// ��i�������ʼ�ڵ�
			fnodes[1] = i + 1 < nodes.size() ? 
				        static_cast<int>(nodes[i + 1]) 
				        : static_cast<int>(nodes[0]); // ��i�����ĩβ�ڵ�

			// ��� n0-n1 ���Ƿ��Ѿ�����
			// �����ڣ����Ϊ�µ��棬nodes=[n0,n1],cells[cell,-1]
			// ���ڣ���������Ҳ൥Ԫ��face_cells[face][1] = cell
			auto it = faces.find(fnodes);
			if (it == faces.end()) {
				// �����ڣ�����һ���µ���
				int face_id = static_cast<int>(face_nodes.size());
				face_nodes.push_back(fnodes);
				face_cells.push_back(key{ cell,UnsMesh2D::invalid_id });
				faces.emplace(fnodes, face_id);
			}
			else {
				// ����
				int face_id = it->second;      // ����
				face_cells[face_id][1] = cell; // �������Ҳ൥Ԫ���

			}
		}
	}
}

void UnsMesh2D::read_gmsh_file(const char* file) {
	// 1. ��ȡ�ļ�������һ������
	// nnode,ncell,x,y,cell_nodes
	read_gmsh(file, x, y, cell_nodes);
	nnode = static_cast<int>(x.size());
	ncell = cell_nodes.nrow();

	// 2. ���� cell_nodes ������������
	// nface, face_nodes, face_cells
	create_faces_from_cell_nodes(cell_nodes, face_nodes, face_cells);
	nface = static_cast<int>(face_nodes.size());
}

void UnsMesh2D::test() {
	//			 +0+
	//		  /	     \
	//		 /        \
	//	    1+--------+2
	//		|		   |
	//		|		   |
	//	    3+--------+4
	//		 \		  /
	//		  \		 /
	//		     +5+

	nnode = 6;
	ncell = 3;

	cell_nodes.clear();
	cell_nodes.push_back({ 0,1,2 });
	cell_nodes.push_back({ 1,3,4,2 });
	cell_nodes.push_back({ 3,5,4 });

	// 2. ���� cell_nodes ������������
	// nface, face_nodes, face_cells
	create_faces_from_cell_nodes(cell_nodes, face_nodes, face_cells);
	nface = static_cast<int>(face_nodes.size());

	// ��ӡ����
	for (const auto& c : face_nodes) {
		std::cout << c[0] << "--" << c[1] << std::endl;
	}
}