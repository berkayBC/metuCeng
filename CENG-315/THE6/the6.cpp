#include "the6.h"
// Don't add any additional includes

/*
  N: Number of racers
  updates: Updates you have heard
*/
std::pair<bool, std::vector<int> > isCyclicHelper(int v, bool visited[], bool *recStack, std::vector<int> adjList[], std::vector<int> &cycle_list)
{
	if (visited[v] == false)
	{
		// Mark the current node as visited and part of recursion stack
		visited[v] = true;
		recStack[v] = true;

		// Recur for all the vertices adjacent to this vertex
		std::vector<int>::iterator itr;
		for (itr = adjList[v].begin(); itr != adjList[v].end(); itr++)
		{
			if (!visited[*itr] && isCyclicHelper(*itr, visited, recStack, adjList, cycle_list).first)
			{
				cycle_list.push_back(*itr);
				return {true, cycle_list};
			}
			else if (recStack[*itr])
			{
				cycle_list.push_back(*itr);
				return {true, cycle_list};
			}
		}
	}
	recStack[v] = false; // remove the vertex from recursion stack
	return {false, {}};
}

std::pair<bool, std::vector<int> > isCyclic(int N, std::vector<int> adjList[], std::vector<int> &cycle_list)
{
	// Mark all the vertices as not visited and not part of recursion stack
	bool *visited = new bool[N];
	bool *recStack = new bool[N];
	for (int i = 0; i < N; i++)
	{
		visited[i] = false;
		recStack[i] = false;
	}

	for (int i = 0; i < N; i++)
	{
		if (isCyclicHelper(i, visited, recStack, adjList, cycle_list).first)
		{
			return {true, cycle_list};
		}
	}

	return {false, {}};
}

std::pair<bool, std::vector<int> >
RaceResult(int N, std::vector<std::pair<int, int> > updates)
{

	// create an adjacent list
	std::vector<int> adjList[N];
	std::vector<int> cycle_list;
	std::vector<int> result_cycle_list;

	for (long unsigned int i = 0; i < updates.size(); i++)
	{
		adjList[updates[i].first].push_back(updates[i].second);
	}

	// if there is cycle, return false and the cycle

	if (isCyclic(N, adjList, cycle_list).first)
	{
		for (unsigned long int i = 0; i < cycle_list.size(); i++)
		{
			result_cycle_list.push_back(cycle_list[cycle_list.size() - 1 - i]);
		}
		result_cycle_list.push_back(result_cycle_list[0]);

		return {false, result_cycle_list};
	}

	// if there is not a cycle, use kahn's algo
	std::vector<int> in_degree(N, 0);

	for (int u = 0; u < N; u++)
	{
		std::vector<int>::iterator itr;
		for (itr = adjList[u].begin(); itr != adjList[u].end(); itr++)
		{
			in_degree[*itr]++;
		}
	}
	std::queue<int> q;
	for (int i = 0; i < N; i++)
	{
		if (in_degree[i] == 0)
		{
			q.push(i);
		}
	}

	std::vector<int> result_lineup;

	while (!q.empty())
	{
		int u = q.front();
		q.pop();
		result_lineup.push_back(u);

		std::vector<int>::iterator itr;
		for (itr = adjList[u].begin(); itr != adjList[u].end(); itr++)
		{
			if (--in_degree[*itr] == 0)
			{
				q.push(*itr);
			}
		}
	}

	return {true, result_lineup};
}
