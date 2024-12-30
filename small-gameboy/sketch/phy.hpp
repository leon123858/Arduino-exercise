/**
 * @file phy.hpp
 * @author Leon
 * @brief a SAT collision lib for cpp embedded game
 * @version 0.1
 * @date 2024-12-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _LEON_COLLISION_LIB_
#define _LEON_COLLISION_LIB_

#define NUM_LIMIT_MAX (1024)
#define NUM_LIMIT_MIN (-1024)
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

#define CROSS_PRODUCT(p1, p2, p3) ((p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x))

struct point
{
	short x;
	short y;
};

class Line
{
public:
	point head;
	point tail;
	Line(short x1, short y1, short x2, short y2) : head{x1, y1}, tail{x2, y2} {}
	Line() : head{0, 0}, tail{0, 0} {}
	short delX();
	short delY();
};

short Line::delX()
{
	return tail.x - head.x;
}

short Line::delY()
{
	return tail.y - head.y;
}

class Vector : public Line
{
public:
	using Line::Line;
	Vector normalVector();
	Vector projectVector(Vector *v);
	bool isParallelOverLapping(Vector *v);
	point projectPoint(const point &srcP, Vector *v);
	bool isIntersect(Vector *v);
};

Vector Vector::normalVector()
{
	short normalX = -delY();
	short normalY = delX();
	return Vector(head.x, head.y, head.x + normalX, head.y + normalY);
}

bool Vector::isIntersect(Vector *v)
{
	// quick AABB
	if (MAX(head.x, tail.x) < MIN(v->head.x, v->tail.x) || MAX(head.y, tail.y) < MIN(v->head.y, v->tail.y) ||
		MAX(v->head.x, v->tail.x) < MIN(head.x, tail.x) || MAX(v->head.y, v->tail.y) < MIN(head.y, tail.y))
	{
		return false;
	}

	// cross formula
	if (CROSS_PRODUCT(head, v->head, v->tail) * CROSS_PRODUCT(tail, v->head, v->tail) <= 0 &&
		CROSS_PRODUCT(v->head, head, tail) * CROSS_PRODUCT(v->tail, head, tail) <= 0)
	{
		return true;
	}
	return false;
}

point Vector::projectPoint(const point &srcP, Vector *v)
{
	double baX = static_cast<double>(srcP.x) - static_cast<double>(v->head.x);
	double baY = static_cast<double>(srcP.y) - static_cast<double>(v->head.y);

	double bbX = static_cast<double>(v->tail.x) - static_cast<double>(v->head.x);
	double bbY = static_cast<double>(v->tail.y) - static_cast<double>(v->head.y);

	if (bbX == 0.0)
	{
		point p = {
			static_cast<short>(v->head.x),
			static_cast<short>(srcP.y),
		};
		return p;
	}
	else if (bbY == 0.0)
	{
		point p = {
			static_cast<short>(srcP.x),
			static_cast<short>(v->head.y),
		};
		return p;
	}

	double dotProduct = baX * bbX + baY * bbY;
	double bbSquare = bbX * bbX + bbY * bbY;

	double t = dotProduct / bbSquare;

	double pX = static_cast<double>(v->head.x) + t * bbX;
	double pY = static_cast<double>(v->head.y) + t * bbY;

	point p = {
		static_cast<short>(pX),
		static_cast<short>(pY),
	};
	return p;
}

Vector Vector::projectVector(Vector *v)
{
	// 分別投影線段 A 的兩個端點
	point projectedStart = projectPoint(head, v);
	point projectedEnd = projectPoint(tail, v);

	// 回傳包含兩個投影點的 Pair
	return Vector(projectedStart.x, projectedStart.y, projectedEnd.x, projectedEnd.y);
}

bool Vector::isParallelOverLapping(Vector *v)
{
	if (delX() == 0)
	{
		// if y overlap
		auto min1 = MIN(head.y, tail.y);
		auto max1 = MAX(head.y, tail.y);
		auto min2 = MIN(v->head.y, v->tail.y);
		auto max2 = MAX(v->head.y, v->tail.y);

		if (max1 < min2 || max2 < min1)
		{
			return false;
		}
	}
	else
	{
		// if x overlap
		auto min1 = MIN(head.x, tail.x);
		auto max1 = MAX(head.x, tail.x);
		auto min2 = MIN(v->head.x, v->tail.x);
		auto max2 = MAX(v->head.x, v->tail.x);

		if (max1 < min2 || max2 < min1)
		{
			return false;
		}
	}

	return true;
}

class Shape
{
protected:
	Vector mergeProjectList(Vector list[], short len);

public:
	Vector *vecList;
	short vecLen;

	/// @brief get shape project from one of inner vector
	virtual Vector getVecProject(short index) = 0;

	/// @brief get shape project from one of outside vector
	virtual Vector getVecProject(Vector *v) = 0;

	/// @brief release memory in vecList
	~Shape();
};

Vector Shape::mergeProjectList(Vector list[], short len)
{
	short minX = NUM_LIMIT_MAX; // Initialize with maximum possible value
	short minY = NUM_LIMIT_MAX;
	short maxX = NUM_LIMIT_MIN; // Initialize with minimum possible value
	short maxY = NUM_LIMIT_MIN;

	for (size_t i = 0; i < len; i++)
	{
		minX = MIN(minX, MIN(list[i].head.x, list[i].tail.x));
		minY = MIN(minY, MIN(list[i].head.y, list[i].tail.y));
		maxX = MAX(maxX, MAX(list[i].head.x, list[i].tail.x));
		maxY = MAX(maxY, MAX(list[i].head.y, list[i].tail.y));
	}

	return Vector(minX, minY, maxX, maxY);
}

Shape::~Shape()
{
	delete vecList;
}

class Square : public Shape
{
private:
	/* data */
public:
	Square(short x, short y, short w, short h);
	Vector getVecProject(short index) override;
	Vector getVecProject(Vector *v) override;
	point midPoint();
};

point Square::midPoint()
{
	point p = {
		static_cast<short>((this->vecList[0].head.x + this->vecList[0].tail.x) / short(2)),
		static_cast<short>((this->vecList[1].head.y + this->vecList[1].tail.y) / short(2)),
	};
	return p;
}

Square::Square(short x, short y, short w, short h)
{
	this->vecList = new Vector[4]{
		Vector(x, y, x + w, y),
		Vector(x, y, x, y + h),
		Vector(x, y + h, x + w, y + h),
		Vector(x + w, y, x + w, y + h),
	};
	vecLen = 4;
}

Vector Square::getVecProject(short index)
{
	Vector normalVec = vecList[index].normalVector();
	Vector projectVecList[3];
	short cur = 0;
	// get all line project
	for (size_t i = 0; i < vecLen; i++)
	{
		if (i == index)
		{
			continue;
		}
		projectVecList[cur] = vecList[i].projectVector(&normalVec);
		cur++;
	}

	return mergeProjectList(projectVecList, cur);
}

Vector Square::getVecProject(Vector *v)
{
	Vector projectVecList[4];
	// get all line project
	for (size_t i = 0; i < vecLen; i++)
	{
		projectVecList[i] = vecList[i].projectVector(v);
	}

	return mergeProjectList(projectVecList, vecLen);
}

class Circle : public Shape
{
public:
	/* data */
	short x;
	short y;
	short r;
	Circle(short x, short y, short r);
	Vector getVecProject(short index) override;
	Vector getVecProject(Vector *v) override;
};

Circle::Circle(short x, short y, short r)
{
	this->vecList = new Vector[2]{
		Vector(x - r, y, x + r, y),
		Vector(x, y - r, x, y + r),
	};
	vecLen = 2;

	this->x = x;
	this->y = y;
	this->r = r;
}

Vector Circle::getVecProject(short index)
{
	Vector normalVec = vecList[index].normalVector();
	Vector projectVecList[2];
	short cur = 0;
	// get all line project
	for (size_t i = 0; i < vecLen; i++)
	{
		if (i == index)
		{
			continue;
		}
		projectVecList[cur] = vecList[i].projectVector(&normalVec);
		cur++;
	}

	return mergeProjectList(projectVecList, cur);
};

double fastInverseSqrt(double x)
{
	union
	{
		double d;
		int64_t i;
	} conv = {x};
	conv.i = 0x5fe6ec85e7de30da - (conv.i >> 1);
	return conv.d;
}

Vector Circle::getVecProject(Vector *v)
{
	double bbX = static_cast<double>(v->tail.x) - static_cast<double>(v->head.x);
	double bbY = static_cast<double>(v->tail.y) - static_cast<double>(v->head.y);

	if (bbX == 0 && bbY == 0)
	{
		return Vector(0, 0, 0, 0);
	}
	else if (bbX == 0)
	{
		return Vector(v->head.x, y - r, v->head.x, y + r);
	}
	else if (bbY == 0)
	{
		return Vector(x - r, v->head.y, x + r, v->head.y);
	}

	point circleM = {
		x,
		y,
	};
	point mv = v->projectPoint(circleM, v);

	double bbSquare = bbX * bbX + bbY * bbY;
	double bb = fastInverseSqrt(bbSquare);
	double t = r * bb;

	return Vector(mv.x - t * bbX, mv.y - t * bbY, mv.x + t * bbX, mv.y + t * bbY);
};

bool checkSATCollision(Square &a, Square &b)
{
	for (short i = 0; i < a.vecLen; i++)
	{
		Vector av = a.getVecProject(i);
		Vector bv = b.getVecProject(&av);
		if (!av.isParallelOverLapping(&bv))
		{
			return false;
		}
	}
	return true;
}

bool checkSATCollision(Circle &a, Circle &b)
{
	double ax = static_cast<double>(a.x);
	double bx = static_cast<double>(b.x);
	double ay = static_cast<double>(a.y);
	double by = static_cast<double>(b.y);
	double ar = static_cast<double>(a.r);
	double br = static_cast<double>(b.r);

	double r2sqr = ar * ar + br * br;
	double dist = (ax - bx) * (ax - bx) + (ay - by) * (ay - by);

	return dist < r2sqr;
}

bool checkSATCollision(Square &a, Circle &b)
{
	for (short i = 0; i < a.vecLen; i++)
	{
		Vector av = a.getVecProject(i);
		Vector bv = b.getVecProject(&av);
		if (!av.isParallelOverLapping(&bv))
		{
			return false;
		}
	}
	return true;
}

#endif // _LEON_COLLISION_LIB_