#include <iostream>
#include <cassert>
#include <cmath>
#include "phy.hpp"

int main()
{
	// Test point struct
	point p1 = {10, 20};
	assert(p1.x == 10);
	assert(p1.y == 20);

	// Test Line class
	Line l1(1, 2, 3, 4);
	assert(l1.head.x == 1);
	assert(l1.head.y == 2);
	assert(l1.tail.x == 3);
	assert(l1.tail.y == 4);
	assert(l1.delX() == 2);
	assert(l1.delY() == 2);

	Line l2(5, 1, 1, 5);
	assert(l2.minXPoint().x == 1);
	assert(l2.minYPoint().y == 1);
	assert(l2.maxXPoint().x == 5);
	assert(l2.maxYPoint().y == 5);

	// Test Vector class
	Vector v1(1, 2, 4, 6);
	assert(v1.delX() == 3);
	assert(v1.delY() == 4);

	Vector n1 = v1.normalVector();
	assert(n1.head.x == 1);
	assert(n1.head.y == 2);
	assert(n1.delX() == -4);
	assert(n1.delY() == 3);

	Vector v2(0, 0, 5, 0);
	Vector v3(0, 0, 0, 5);
	Vector p2 = v1.projectVector(&v2);
	assert(p2.head.x == 1);
	assert(p2.head.y == 0);
	assert(p2.tail.x == 4);
	assert(p2.tail.y == 0);
	Vector p3 = v1.projectVector(&v3);
	assert(p3.head.x == 0);
	assert(p3.head.y == 2);
	assert(p3.tail.x == 0);
	assert(p3.tail.y == 6);

	Vector v4(0, 0, 5, 5);
	Vector p4 = v1.projectVector(&v4);
	assert(p4.head.x == 1);
	assert(p4.head.y == 1);
	assert(p4.tail.x == 5);
	assert(p4.tail.y == 5);

	Vector v5(1, 1, 1, 5);
	Vector v6(2, 2, 2, 6);
	assert(v5.isParallelOverLapping(&v6));
	Vector v7(1, 1, 1, 5);
	Vector v8(2, 7, 2, 9);
	assert(!v7.isParallelOverLapping(&v8));

	Vector v9(1, 1, 5, 1);
	Vector v10(2, 1, 6, 1);
	assert(v9.isParallelOverLapping(&v10));
	Vector v11(1, 1, 5, 1);
	Vector v12(7, 1, 9, 1);
	assert(!v11.isParallelOverLapping(&v12));

	// Test Square class
	Square s1(10, 10, 20, 30);
	assert(s1.vecLen == 2);

	Vector proj1 = s1.getVecProject(short(0));
	Vector proj2 = s1.getVecProject(short(1));
	assert(proj1.delY() == 30);
	assert(proj2.delX() == 20);

	Vector v_proj(0, 0, 0, 1);
	Vector proj3 = s1.getVecProject(&v_proj);
	assert(proj3.delY() == 30);

	Vector v_proj2(0, 0, 1, 0);
	Vector proj4 = s1.getVecProject(&v_proj2);
	assert(proj4.delX() == 20);

	Vector v_proj3(0, 0, 1, 1);
	Vector proj5 = s1.getVecProject(&v_proj3);
	std::cout << proj5.head.x << std::endl;
	std::cout << proj5.head.y << std::endl;
	std::cout << proj5.tail.x << std::endl;
	std::cout << proj5.tail.y << std::endl;
	assert(proj5.head.x == 10);
	assert(proj5.head.y == 10);
	assert(proj5.tail.x == 25);
	assert(proj5.tail.y == 35);

	// Square s2(0, 0, 10, 10);
	// Vector proj_test1 = s2.getVecProject(short(0));
	// Vector proj_test2 = s2.getVecProject(short(1));

	std::cout << "All tests passed!" << std::endl;
	return 0;
}