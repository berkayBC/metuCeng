#include "the7.h"
// Function to add shortest
// path from source to j
// using parent array
void addPath(std::vector<int> &path, int *parent, int j)
{

  // Base Case : If j is source
  if (parent[j] < 0)
  {
    return;
  }

  addPath(path, parent, parent[j]);

  path.push_back(j);
}

int minDistance(int n, int dist[], bool sptSet[])
{

  // Initialize min value
  int min = INT_MAX, min_index;

  for (int v = 0; v < n; v++)
    if (sptSet[v] == false && dist[v] <= min)
    {
      min = dist[v], min_index = v;
    }

  return min_index;
}

void dijkstra(int n, std::vector<Road> roads, int src, int *dist, int *parent)
{
  bool sptSet[n];

  for (int i = 0; i < n; i++)
  {
    sptSet[i] = false;
  }
  dist[src] = 0;

  for (int j = 0; j < n - 1; j++)
  {
    int u = minDistance(n, dist, sptSet);
    sptSet[u] = true;

    for (int v = 0; v < n; v++)
    {
      for (long unsigned int k = 0; k < roads.size(); k++)
      {
        if (((roads[k].endpoints.first == u && roads[k].endpoints.second == v) ||
             (roads[k].endpoints.first == v && roads[k].endpoints.second == u)) &&
            !sptSet[v] && (dist[u] + roads[k].time < dist[v]))
        {
          parent[v] = u;
          dist[v] = dist[u] + roads[k].time;
          break;
        }
      }
    }
  }
}

void FindRoute(int n, std::vector<Road> roads, int s, int d, int x, int y)
{
  // You can change these variables. These are here for demo only.
  std::vector<int> path;
  int cost1 = 0;
  int cost2 = 0;
  // Write your code here..
  int result_dist1[n], result_dist2[n], result_dist3[n];
  int result_parent1[n], result_parent2[n], result_parent3[n];
  for (int i = 0; i < n; i++)
  {
    result_parent1[i] = -1, result_parent2[i] = -1, result_parent3[i] = -1;
    result_dist1[i] = INT_MAX, result_dist2[i] = INT_MAX, result_dist3[i] = INT_MAX;
  }
  // s->x, s->y
  dijkstra(n, roads, s, result_dist1, result_parent1);
  // x->y, x->d
  dijkstra(n, roads, x, result_dist2, result_parent2);
  // y->x, y->d
  dijkstra(n, roads, y, result_dist3, result_parent3);

  // s->x
  cost1 += result_dist1[x];
  // x->y
  cost1 += result_dist2[y];
  // y->d
  cost1 += result_dist3[d];

  // s->y
  cost2 += result_dist1[y];
  // y->x
  cost2 += result_dist3[x];
  // x->d
  cost2 += result_dist2[d];
  /*
  // debug
  std::cout << result_dist1[x] << "\n";
  std::cout << result_dist2[y] << "\n";
  std::cout << result_dist3[d] << "\n";
  std::cout << result_dist1[y] << "\n";
  std::cout << result_dist3[x] << "\n";
  std::cout << result_dist2[d] << "\n";

  for (int j = 0; j < n; j++)
  {
    std::cout << result_parent1[j] << "\n";
    std::cout << result_parent2[j] << "\n";
    std::cout << result_parent3[j] << "\n";
  }
  */

  if (cost2 < cost1)
  {
    // add source to path
    path.push_back(s);
    // s->y
    addPath(path, result_parent1, y);
    // y->x
    addPath(path, result_parent3, x);
    // x->d
    addPath(path, result_parent2, d);
    std::cout << cost2 << " ";
    PrintRange(path.begin(), path.end());
    std::cout << std::endl;
  }

  else
  {
    // Your output should be like this. You can change this as long as you keep
    // the output format. PrintRange function helps you print elements of
    // containers with iteratos (e.g., std::vector).
    // add source to path
    path.push_back(s);
    // s->x
    addPath(path, result_parent1, x);
    // x->y
    addPath(path, result_parent2, y);
    // y->d
    addPath(path, result_parent3, d);
    std::cout << cost1 << " ";
    PrintRange(path.begin(), path.end());
    std::cout << std::endl;
  }
}