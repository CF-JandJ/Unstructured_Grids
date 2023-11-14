#pragma once
#include <vector>
#include <fstream>
#include <string>

#include "CsrGraph.h"

void read_gmsh(const char* file,std::vector<double>& x,std::vector<double>& y,CsrGraph& cell_nodes) {
	static constexpr int nn2e[8] = { 0,2,3,4,4,8,6,5 };
	
	int n2e[8]{ 0 };
	//���ļ�
	std::ifstream ifs(file);

	std::string buffer;

	x.clear();
	y.clear();
	cell_nodes.clear();

	while (!ifs.eof())
	{
		std::getline(ifs, buffer);

		// $Nodes
		// ��ȡ�ڵ�����
		// ����
		// ��� x,y,z
		if (buffer == "$Nodes") {
			int nnode{ 0 };
			ifs >> nnode;

			x.resize(nnode);
			y.resize(nnode);

			int id{ 0 };
			double z{ 0.0 };
			for (int i = 0; i < nnode; i++) {
				ifs >> id >> x[i] >> y[i] >> z;
			}
		}

		// $Elements
		// ��ȡ��Ԫ����
		// ��ţ����ͣ���ǩ���������ţ��ڵ��б�
		else if (buffer == "$Elements") {
			int nelem{ 0 };
			ifs >> nelem;

			int id{ 0 }, type{ 0 }, tag{ 0 }, phy{ 0 }, grp{ 0 };
			for (int i = 0; i < nelem; i++) {
				ifs >> id >> type >> tag >> phy >> grp;
				assert(type >= 1 && type <= 7);

				for (int j = 0; j < nn2e[type]; j++) {
					ifs >> n2e[j];
					--n2e[j]; //���ת��Ϊ��0��ʼ
				}

				// �ѽڵ���ӵ�cell_nodesĩβ
				if(type == 2 || type ==3)
					cell_nodes.push_back(std::span<int>(n2e, static_cast<std::size_t>(nn2e[type])));
			}
		}
	}

	ifs.close();
}