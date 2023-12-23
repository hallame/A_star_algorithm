#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>



using namespace std;



class Peak
{
public:
	Peak(vector<vector<int>> area)                                            // area: line; if n = m, so n = m = area with n^2 possibilities moving
	{
		this->area = area;
		f = h = evr();                                                         // // Estate Valuation Report
		g = 0;
		prev = nullptr;
	}

	Peak(vector<vector<int>> area, int g)
	{
		this->area = area;
		this->g = g;                                                          // f(n) = g(n) + h(n) : Algorithm A* formula for first moving
		h = evr();
		f = g + h;
	}

	Peak(vector<vector<int>> area, int h, int g, Peak* parent)
	{
		this->area = area;
		this->h = h;
		this->g = g;
		prev = parent;
		f = h + g;                                                             // f(n) = g(n) + h(n) : Algorithm A* formula generalization
	}

	vector<vector<int>> getArea() { return area; }
	int getF() { return f; }
	int getG() { return g; }
	int getH() { return h; }
	void setPrev(Peak* parent) { prev = parent; }
	void setG(int g) { this->g = g; }
	void setH(int h) { this->h = h; }
	void setF(int f) { this->f = f; }
	void printArea();

	bool operator == (const Peak& other);                                    // Defines how the class is convertible to a boolean value
                                                                             // operator is converted to a boolean value
	~Peak() {};

private:
	vector<vector<int>> area;
	Peak* prev;
	int h; int g; int f;
	int evr();
	friend Peak left_rule(Peak parent, int i, int j);                          // to access private, protected and public members of a class.
	friend Peak right_rule(Peak parent, int i, int j);
	friend Peak up_rule(Peak parent, int i, int j);
	friend Peak down_rule(Peak parent, int i, int j);
	friend vector<Peak> search_adjacent_vertices(Peak parent);
};





void Peak::printArea()                                                          // State printing
{
	for (int i = 0; i < area.size(); i++)
	{
		for (int j = 0; j < area[i].size(); j++)
			cout << area[i][j];
		cout << endl;
	}
	cout << endl;
}






int Peak::evr()                                                                  // Estate Valuation Report rapport d'evaluation Successorale
{                                                                                 // for checking
	int right_value = 0;
	int counter = 0;
	vector<vector<int>> goal{{ 1, 2, 3 },{ 8, 0, 4 },{ 7, 6, 5 }};

	/*vector<vector<int>> goal(area.size(), vector<int>(area.size(), 0));	       // Matrix of correct values, initialized to zero
	for (int i = 0; i < goal.size(); i++)
		for (int j = 0; j < goal[i].size(); j++)
		{
			right_value++;
			goal[i][j] = right_value;
		}
	goal[goal.size() - 1][goal.size() - 1] = 0; */                               // // we place the empty case to right lower edge
                                                                               // Simple heuristics
                                                                               //for (int i = 0; i < area.size(); i++){for (int j = 0; j < area[i].size(); j++){if (area[i][j] == 0){ continue; }; if (area[i][j] != goal[i][j]){counter++;	// zero checking}}}

	// Distance of Manhattan

	for (int i = 0; i < area.size(); i++)
	{
		for (int j = 0; j < area[i].size(); j++)
		{
			if (area[i][j] == 0)	                                           // checking on zero, we're don't about the empty case
				continue;

			if (area[i][j] != goal[i][j])                                     // when the case number is not in his right place
				for (int k = 0; k < goal.size(); k++)
				{
					for (int l = 0; l < goal[k].size(); l++)
					{
						if (area[i][j] == goal[k][l])
						{
							counter += (abs(i - k) + abs(j - l));              //  distance Manhattan :counter equal to absolute value of the difference
						}
					}
				}
		}
	}

	// Linear conflict

	for (int i = 0; i < area.size(); i++)
	{
		vector<int> lin(0);
		for (int j = 0; j < area[i].size(); j++)
		{
			if (area[i][j] == 0)	                                            // checking on zero, we're don't about the empty case
				continue;

			if (area[i][j] != goal[i][j])                                       // when the case number is not in his right place
				for (int k = 0; k < goal.size(); k++)
					if (area[i][j] == goal[i][k])                                // in the each if the game table contains the same number as the goal
						lin.push_back(area[i][j]);                               // we put this number in the vector lin
		}
		if (lin.size() >= 2)
			if (lin[0] > lin[1])                                                  // if the first number in the vector is superior than the second,
				counter += 2;                                                     // so we move 2 times. it means we should move pawn/pion the first and the second
	}

	return counter;







	// The last move
	/*
	bool flag = false;
	for (int i = 0; i < area.size(); i++)
		// cout << area[i][area.size() - 1] << endl;
		// cout << goal[goal.size() - 1][goal.size() - 2] << endl << endl;
		// The rightmost column is compared with 8/15/...
		if (area[i][area.size() - 1] == goal[goal.size() - 1][goal.size() - 2])
			flag = true;
	for (int i = 0; i < area.size(); i++)
		// cout << area[area.size() - 1][i] << endl;
		// cout << goal[goal.size() - 2][goal.size() - 1] << endl << endl;
		// The bottom line is compared to 6/12/...
		if (area[area.size() - 1][i] == goal[goal.size() - 2][goal.size() - 1])
			flag = true;
	if (!flag)
		counter += 2;
	*/

	//	The last move 2.0
	/*
	if (!(area[area.size() - 1][area.size() - 1] == goal[goal.size() - 1][goal.size() - 2] or area[area.size() - 1][area.size() - 1] == goal[goal.size() - 2][goal.size() - 1]))
	{
		counter += 2;
	}
	*/
	//return counter;
}

bool Peak::operator==(const Peak& other)
{
	if (area == other.area)                                                     // each peak checking
		return true;
	return false;
}



  //////////////////////
 // RULES DEFINITION //
//////////////////////



Peak left_rule(Peak parent, int i, int j)
{
	Peak child(parent.area, parent.h, parent.g, &parent);	               // parent copy: parent number, his h,, his g and itself
	try                                                                     // first we test this block
	{
		child.area.at(i).at(j) = child.area.at(i).at(j - 1);                // we move to left
		child.area.at(i).at(j - 1) = 0;                                     // the empty case change too
	}
	catch (const std::exception&)                                           // we execute this block, if an error occurs in the try block.
	{
		child.f = (int)1e7;
		return child;
	}

	child.g++;                                                               // f(n) = g(n) + h(n) : Algorithm A*
	child.h = child.evr();                                                   // we should check if we get goal before continuing
	child.f = child.g + child.h;
	return child;
}

Peak right_rule(Peak parent, int i, int j)
{
	Peak child(parent.area, parent.h, parent.g, &parent);
	try
	{
		child.area.at(i).at(j) = child.area.at(i).at(j + 1);
		child.area.at(i).at(j + 1) = 0;
	}
	catch (const std::exception&)
	{
		child.f = (int)1e7;                                                    //  1e7: 1 times 10 to the 7th power, or 10000000
		return child;
	}

	child.g++;
	child.h = child.evr();
	child.f = child.g + child.h;
	return child;
}

Peak up_rule(Peak parent, int i, int j)
{
	Peak child(parent.area, parent.h, parent.g, &parent);
	try
	{
		child.area.at(i).at(j) = child.area.at(i - 1).at(j);
		child.area.at(i - 1).at(j) = 0;
	}
	catch (const std::exception&)
	{
		child.f = (int)1e7;
		return child;
	}

	child.g++;
	child.h = child.evr();
	child.f = child.g + child.h;
	return child;
}

Peak down_rule(Peak parent, int i, int j)
{
	Peak child(parent.area, parent.h, parent.g, &parent);
	try
	{
		child.area.at(i).at(j) = child.area.at(i + 1).at(j);
		child.area.at(i + 1).at(j) = 0;
	}
	catch (const std::exception&)
	{
		child.f = (int)1e7;                                                 //  1e7: 1 times 10 to the 7th power, or 10000000
		return child;
	}

	child.g++;
	child.h = child.evr();
	child.f = child.g + child.h;
	return child;
}

vector<Peak> search_adjacent_vertices(const Peak parent)	                  // Search for adjacent vertices
{
	vector<Peak> array;
	for (int i = 0; i < parent.area.size(); i++)
		for (int j = 0; j < parent.area[i].size(); j++)
			if (parent.area[i][j] == 0)
			{
				Peak left = left_rule(parent, i, j);
				if (left.getF() != (int)1e7)                                   // in order words : if there are not any errors
				{
					array.push_back(left);                                      // we put the new peak in this array
					//cout << "Possible shift to the left (f = " << left.getF() << "):" << endl;
					//left.printArea();
				}

				Peak right = right_rule(parent, i, j);
				if (right.getF() != (int)1e7)
				{
					array.push_back(right);
					//cout << "Possible shift to the right (f = " << right.getF() << "):" << endl;
					//right.printArea();
				}

				Peak up = up_rule(parent, i, j);
				if (up.getF() != (int)1e7)
				{
					array.push_back(up);
					//cout << "Possible shift to the up (f = " << up.getF() << "):" << endl;
					//up.printArea();
				}

				Peak down = down_rule(parent, i, j);
				if (down.getF() != (int)1e7)
				{
					array.push_back(down);
					//cout << "Possible shift to the down (f = " << down.getF() << "):" << endl;
					//down.printArea();
				}
			}
	return array;
}

Peak find_with_min_f(vector<Peak> array)                                                  // now we search the min value of f
{
	int min = (int)1e6;                                                                  // arbitrarily chosen 1000 000

	for (int i = 0; i < array.size(); i++)
		if (array[i].getF() < min)
			min = array[i].getF();                                                        //we assign the smallest value to it

	for (int i = 0; i < array.size(); i++)
		if (array[i].getF() == min)
			return array[i];                                                              // if the f of the peak equal to the min , so just keep this peak
}

Peak find_with_min_h(vector<Peak> array)                                                  // now we search the min value ofh
{
	int min = (int)1e6;

	for (int i = 0; i < array.size(); i++)
		if (array[i].getH() < min)
			min = array[i].getH();

	for (int i = 0; i < array.size(); i++)
		if (array[i].getH() == min)
			return array[i];
}

bool find_same_area(vector<Peak> array, Peak to_find)                                        // bool for same case searching
{
	for (int i = 0; i < array.size(); i++)
		if (array[i].getArea() == to_find.getArea())
			return true;
	return false;
}

Peak same_area(vector<Peak> array, Peak to_find)                                             // same case searching
{
	for (int i = 0; i < array.size(); i++)
		if (array[i].getArea() == to_find.getArea())
			return array[i];
	return to_find;
}

void print_matrix(vector<vector<int>> area)
{
	for (int i = 0; i < area.size(); i++)
	{
		for (int j = 0; j < area[i].size(); j++)
			cout << area[i][j];
		cout << endl;
	}
	cout << endl;
}




string Algo_A_Star (Peak start)
{
	// Final Matrix
	int right_value = 0;


	//vector<vector<int>> goal(start.getArea().size(), vector<int>(start.getArea().size(), 0));
	vector<vector<int>> goal{{ 1, 2, 3 },{ 8, 0, 4 },{ 7, 6, 5 } };


	/*for (int i = 0; i < goal.size(); i++)
		for (int j = 0; j < goal[i].size(); j++)
		{
			right_value++;
			goal[i][j] = right_value;
		}
	goal[goal.size() - 1][goal.size() - 1] = 0;  */                                                          // we place the empty case to right lower edge

	vector<Peak> Q;                                                                                         // set of the peaks to be visited
	vector<Peak> U;                                                                                         // set of visited peaks

	cout << "Initial matrix: " << endl;
	print_matrix(start.getArea());
	cout << " ================ RUNNING OF ALGORITHM A*======================"<< endl << endl;

	Q.push_back(start);
	int counter = 0;
	while (Q.size() != 0)
	{
		Peak current = find_with_min_f(Q);                                                                    // candidates peaks
		cout << "The vertex with the minimum f is taken = " << current.getF() << endl;
		print_matrix(current.getArea());

		if (current.getArea() == goal)
			return "Correct matrix is found!!!";

		for (auto it = Q.begin(); it != Q.end(); it++)
			if (*it == current)
			{
				Q.erase(it);                                                                                    //we remove the current peak from Q
				break;
			}
		U.push_back(current);                                                                                   // and  we stock it in U

		vector<Peak> adjacent_vertices;
		adjacent_vertices = search_adjacent_vertices(current);

		for (int i = 0; i < adjacent_vertices.size(); i++)
		{
			int atgoalt_score = current.getG() + 1;
			if (find_same_area(U,adjacent_vertices[i]) && atgoalt_score >= adjacent_vertices[i].getG())
			{
				continue;
			}
			if (!(find_same_area(U, adjacent_vertices[i])) || atgoalt_score < adjacent_vertices[i].getG())       //same_area(U, adjacent_vertices[i]).getN
			{
				adjacent_vertices[i].setPrev(&current);

				if (find_same_area(Q, adjacent_vertices[i]) == 0)
				{
					cout << "A vertex has been added to Q (g = " << adjacent_vertices[i].getG() << ", h = " << adjacent_vertices[i].getH() << ", f = "  << adjacent_vertices[i].getF() << "):" << endl;
					adjacent_vertices[i].printArea();

					Q.push_back(adjacent_vertices[i]);
				}
			}
		}
		counter++;

		cout << " Number of passes: " << counter << endl;

	}

	return "Not found...";
}

void print_matrix_plant(vector<vector<int>> area)                                                  // to print the resulted matrix for each step
{
	for (int i = 0; i < area.size(); i++)
		for (int j = 0; j < area[i].size(); j++)
			cout << area[i][j];
	cout << endl;
}

int DFS_recursion(int deep, int& step, Peak previous, vector<Peak> & U, const vector<vector<int>>& goal)
{
	vector<Peak> adjacent_vertices;
	vector<Peak> good_vertices;
	deep++;
	step++;

	adjacent_vertices = search_adjacent_vertices(previous);
	for (int i = 0; i < adjacent_vertices.size(); i++)
	{
		if (!(find_same_area(U, adjacent_vertices[i])))
		{
			good_vertices.push_back(adjacent_vertices[i]);
		}
	}

	for (int i = 0; i < good_vertices.size(); i++)
	{
		Peak current = find_with_min_h(good_vertices);
		cout << "Step: " << step << ", Deep: " << deep << ", Distance: " << current.getH() << ", Matrix: ";
		print_matrix_plant(current.getArea());
		if (current.getArea() == goal)
		{
		    cout << endl;
			cout << "Correct matrix is found !!!" << endl;
			print_matrix(current.getArea());
			cout << endl;
			return 0;
		}
		U.push_back(current);

		if (!DFS_recursion(deep, step, current, U, goal))
			return 0;
	}

	return 1;
}

void DFS(Peak start)
{
    vector<vector<int>> goal{{ 1, 2, 3 },{ 8, 0, 4 },{ 7, 6, 5 }};
	// Final Matrix

	/*int right_value = 0;
	vector<vector<int>> goal(start.getArea().size(), vector<int>(start.getArea().size(), 0));
	for (int i = 0; i < goal.size(); i++)
		for (int j = 0; j < goal[i].size(); j++)
		{
			right_value++;
			goal[i][j] = right_value;
		}
	goal[goal.size() - 1][goal.size() - 1] = 0;*/

	cout << "Initial Matrix: " << endl;
	print_matrix(start.getArea());
	cout << " ================ RUNNING OF DFS ALGORITHM ======================" << endl<< endl;

	int deep = 0;
	int step = 0;
	vector<Peak> U;
	U.push_back(start);
	DFS_recursion(deep, step, start, U, goal);
}

bool check_suitability(vector<vector<int>> area)	              // just checking                     // Checking the tag for solvability, my code can work well without this checking
{
	int n = 0;
	bool flag = false;
	for (int i = 0; i < area.size(); i++)	                                           // We find the row number of an empty cell
	{
		if (flag)
			break;
		for (int j = 0; j < area[i].size(); j++)
		{
			if (area[i][j] == 0)
			{
				n = j + 1;	                                                            // Because elements with 0
				flag = true;
				break;
			};
		};
	};

	int current;
	for (int i = 0; i < area.size(); i++)
		for (int j = 1; j < area[i].size(); j++)
		{
			current = area[i][j];
			if (current == 0)
				break;

			for (int k = 0; k <= i; k++)
				for (int h = 0; h < j; h++)
					if (area[k][h] > current)
						n++;
		}

	if (!(n % 2))                                                                     // a pawn should be in good place at least
	{
		cout << "The classic solution of the game exists!" << endl;
		return true;
	}

	return false;
}








int main()
{

	vector<vector<int>> test {{ 5, 3, 4 },{ 6, 0, 7 },{ 8, 2, 1 }};
	//goal = {{ 1, 2, 3 },{ 8, 0, 4 },{ 7, 6, 5 }};
    /*{{ 2, 1, 7 },{ 3, 4, 6 },{ 5, 8, 0 } };*/
	if (check_suitability(test))
	{
		Peak test_start(test);
		cout << Algo_A_Star(test_start) << endl << endl <<endl;
		cout << "////////////////////////////////////////////////////////////////////////////////////////////////////" << endl << endl;
		DFS(test);
	}
	else
	{
		cout<<"Unsolvable Puzzle!"<<endl;
	}

	return 0;
}


