#pragma once
#include <vector>
#include <span>
#include <cassert>

class CsrGraph
{
public:
	CsrGraph() = default;

	void clear() {
		ia.clear();
		ja.clear();
	}
	// 访问某行的元素数据:[]
	// CsrGraph cell_nodes;
	// ...
	// auto nodes = cell_nodes[5];
	auto operator [] (int i) const {
		return std::span<const int>(&ja[ia[i]], static_cast<std::size_t>(ia[i + 1] - ia[i]));
	}

	auto operator [] (int i) {
		return std::span<int>(&ja[ia[i]], static_cast<std::size_t>(ia[i + 1] - ia[i]));
	}

	// 添加一行信息；
	// CsrGraph cell_nodes;
	// cell_nodes.push_back({25,42,41,36})
	void push_back(std::initializer_list<int> list) {
		if (ia.empty()) ia.push_back(0);

		// m = list.size();
		// ia : {0,1,....,n} -> {0,1,...,n,...,n+m}
		ia.push_back(ia.back() + static_cast<int>(list.size()));
		for (auto x : list)ja.push_back(x);
		assert(ia.back() == ja.size());
	}

	void push_back(std::span<const int> list) {
		if (ia.empty()) ia.push_back(0);

		// m = list.size();
		// ia : {0,1,....,n} -> {0,1,...,n,...,n+m}
		ia.push_back(ia.back() + static_cast<int>(list.size()));
		for (auto x : list)ja.push_back(x);
		assert(ia.back() == ja.size());
	}

	void push_back(std::span<int> list) {
		if (ia.empty()) ia.push_back(0);

		// m = list.size();
		// ia : {0,1,....,n} -> {0,1,...,n,...,n+m}
		ia.push_back(ia.back() + static_cast<int>(list.size()));
		for (auto x : list)ja.push_back(x);
		assert(ia.back() == ja.size());
	}

	int nrow()const { return ia.empty() ? 0 : static_cast<int>(ia.size() - 1); }

private:
	std::vector<int> ia;
	std::vector<int> ja;
};
