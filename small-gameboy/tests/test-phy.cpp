#include <iostream>
#include <cassert>
#include <cmath>
#include "../sketch/phy.hpp"

// std::cout << cv_p.head.x << std::endl;
// std::cout << cv_p.head.y << std::endl;
// std::cout << cv_p.tail.x << std::endl;
// std::cout << cv_p.tail.y << std::endl;

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
	assert(s1.vecLen == 4);

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
	assert(proj5.head.x == 10);
	assert(proj5.head.y == 10);
	assert(proj5.tail.x == 35);
	assert(proj5.tail.y == 35);

	Square s2(5, 5, 5, 4);
	Square s3(5, 5, 5, 5);

	assert(checkSATCollision(s2, s3));
	assert(checkSATCollision(s1, s3));
	assert(!checkSATCollision(s1, s2));

	// circle
	Circle c1(10, 10, 5);
	Circle c2(8, 8, 5);

	assert(c1.getVecProject(short(0)).delY() == 5 * 2);
	assert(c2.getVecProject(short(1)).delX() == 5 * 2);

	Vector cv(0, 0, 1, 0);
	Vector cv2(0, 0, 0, 1);
	Vector cv3(0, 0, 1, 1);

	Vector cv_p = c1.getVecProject(&cv);
	assert(cv_p.delX() == 10);
	assert(cv_p.delY() == 0);

	Vector cv_p2 = c1.getVecProject(&cv2);
	assert(cv_p2.delX() == 0);
	assert(cv_p2.delY() == 10);

	Vector cv_p3 = c1.getVecProject(&cv3);
	assert(cv_p3.delX() == 7);
	assert(cv_p3.delY() == 7);

	assert(checkSATCollision(c1, c2));
	assert(checkSATCollision(s2, c1));
	assert(checkSATCollision(s2, c2));

	Circle c3(20, 20, 5);
	Circle c4(15, 5, 4);
	assert(!checkSATCollision(c1, c3));
	assert(!checkSATCollision(s2, c4));

	Vector crossVec(0, 0, 1, 1);
	Vector crossVec2(0, 1, 1, 0);
	assert(crossVec.isIntersect(&crossVec2));

	Vector crossVec3(0, 0, 2, 2);
	Vector crossVec4(1, 0, 3, 2);
	assert(!crossVec3.isIntersect(&crossVec4));

	std::cout << "All tests passed!" << std::endl;
	return 0;
}