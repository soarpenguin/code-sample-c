#include <stack>
#include <iostream>
#include <string.h>
//#include <memory>

//using namespace std;

struct position {
	int x;
	int y;
};

struct position up = {0, -1};
struct position right = {1, 0};
struct position down = {0, 1};
struct position left = {-1, 0};
struct position dir[] = {up, right, down, left};

bool hasPathCore(char *matrix, int rows, int cols, char *str, 
		std::stack<position>& path, bool *visited);

bool
hasPath(char *matrix, int rows, int cols, char *str)
{
	bool *visited = NULL;
	int row, col;

	if(NULL == matrix || NULL == str || rows < 1 || cols < 1)
		return false;
	
	visited = new bool[rows * cols];
	memset(visited, 0, rows * cols);

	for(row = 0; row < rows; ++row) {
		for(col = 0; col < cols; ++col) {
			if(matrix[row * cols + col] != str[0])
				continue;

			std::stack<position> path;
			struct position pos = {col, row};
			path.push(pos);
			visited[row * cols + col] = true;

			if(hasPathCore(matrix, rows, cols, str, path, visited))
				return true;
		}
	}

	if(visited)
		delete [] visited;
	return false;
}

bool
getNext(char *matrix, int rows, int cols, char *str,
		std::stack<position>& path, bool *visited, int start)
{
	for(int i = start; i < sizeof(dir); ++i) {
		position next = {path.top().x + dir[i].x, path.top().y + dir[i].y};
		if(next.x >= 0 && next.x < cols 
			&& next.y >= 0 && next.y < rows
			&& matrix[next.y * cols + next.x] == str[path.size()]
			&& !visited[next.y * cols + next.x])
		{
			path.push(next);
			visited[next.y * cols + next.x] = true;

			return true;
		}
	}
	return false;
}

bool
popAndGetNext(char *matrix, int rows, int cols, char *str, 
		std::stack<position>& path, bool *visited)
{
	position tobePoped = path.top();
	path.pop();
	visited[tobePoped.y * cols + tobePoped.x] = false;

	bool hasNext = false;
	if(path.size() >= 1) {
		position prev = path.top();
		int deltaX = tobePoped.x - prev.x;
		int deltaY = tobePoped.y - prev.y;
		for(int index = 0; (index < sizeof(dir) / sizeof(position)
					&& !hasNext); ++index) {
			if(deltaX != dir[index].x || deltaY != dir[index].y)
				continue;

			hasNext = getNext(matrix, rows, cols, str, path, visited, index+1);
		}
	}
	return hasNext;
}

bool 
hasPathCore(char *matrix, int rows, int cols, char *str, 
		std::stack<position>& path, bool *visited)
{
	if(str[path.size()] == '\0')
		return true;

	if(getNext(matrix, rows, cols, str, path, visited, 0))
		return hasPathCore(matrix, rows, cols, str, path, visited);

	bool hasNext = popAndGetNext(matrix, rows, cols, str, path, visited);
	while(!hasNext && !path.empty())
		hasNext = popAndGetNext(matrix, rows, cols, str, path, visited);

	if(!path.empty())
		return hasPathCore(matrix, rows, cols, str, path, visited);

	return false;
}


int
main(int argc, char **argv)
{
	int rows, cols;

	std::cin >> rows;
	std::cin >> cols;

	char *a = new char[rows * cols];

	for(int i = 0; i < rows; ++i)
		for(int j = 0; j < cols; ++j)
			std::cin >> a[i * cols + j];

	bool result = hasPath(a, rows, cols, "ABCH");

	if(result)
		std::cout <<  "Yes\n";
	else
		std::cout << "No\n";

	if(a)
		delete [] a;
	return 0;
}

