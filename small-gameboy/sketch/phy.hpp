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

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

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
	point minXPoint();
	point minYPoint();
	point maxXPoint();
	point maxYPoint();
};

point Line::minXPoint()
{
	if (head.x < tail.x)
	{
		return head;
	}
	return tail;
}
point Line::minYPoint()
{
	if (head.y < tail.y)
	{
		return head;
	}
	return tail;
}
point Line::maxXPoint()
{
	if (head.x > tail.x)
	{
		return head;
	}
	return tail;
}
point Line::maxYPoint()
{
	if (head.y > tail.y)
	{
		return head;
	}
	return tail;
}

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
};

Vector Vector::normalVector()
{
	short normalX = -delY();
	short normalY = delX();
	return Vector(head.x, head.y, head.x + normalX, head.y + normalY);
}

Vector Vector::projectVector(Vector *v)
{
	// 內部函式，用於計算單個點的投影
	auto projectPoint = [&](const point &srcP)
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
	};

	// 分別投影線段 A 的兩個端點
	point projectedStart = projectPoint(head);
	point projectedEnd = projectPoint(tail);

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
		auto min2 = MIN(v->head.y, v->head.y);
		auto max2 = MAX(v->head.y, v->head.y);

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
		auto min2 = MIN(v->head.x, v->head.x);
		auto max2 = MAX(v->head.x, v->head.x);

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
	Vector *vecList;

public:
	short vecLen;

	/// @brief get shape project from one of inner vector
	virtual Vector getVecProject(short index) = 0;

	/// @brief get shape project from one of outside vector
	virtual Vector getVecProject(Vector *v) = 0;

	/// @brief release memory in vecList
	~Shape();
};

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
};

Square::Square(short x, short y, short w, short h)
{
	this->vecList = new Vector[2]{
		Vector(x, y, x + w, y),
		Vector(x, y, x, y + h),
	};
	vecLen = 2;
}

Vector Square::getVecProject(short index)
{
	Vector normalVec = vecList[index].normalVector();
	Vector projectVecList[1];
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
	// merge project
	// square can skip this step
	return Vector(projectVecList[0].head.x, projectVecList[0].head.y, projectVecList[0].tail.x, projectVecList[0].tail.y);
}

Vector Square::getVecProject(Vector *v)
{
	Vector projectVecList[2];
	// get all line project
	for (size_t i = 0; i < vecLen; i++)
	{
		projectVecList[i] = vecList[i].projectVector(v);
		std::cout << i << ": " << projectVecList[i].head.x << std::endl;
		std::cout << i << ": " << projectVecList[i].head.y << std::endl;
		std::cout << i << ": " << projectVecList[i].tail.x << std::endl;
		std::cout << i << ": " << projectVecList[i].tail.y << std::endl;
	}
	// get merge project
	short minX = std::numeric_limits<short>::max(); // Initialize with maximum possible value
	short minY = std::numeric_limits<short>::max();
	short maxX = std::numeric_limits<short>::min(); // Initialize with minimum possible value
	short maxY = std::numeric_limits<short>::min();

	for (size_t i = 0; i < vecLen; i++)
	{
		minX = MIN(minX, MIN(projectVecList[i].head.x, projectVecList[i].tail.x));
		minY = MIN(minY, MIN(projectVecList[i].head.y, projectVecList[i].tail.y));
		maxX = MAX(maxX, MAX(projectVecList[i].head.x, projectVecList[i].tail.x));
		maxY = MAX(maxY, MAX(projectVecList[i].head.y, projectVecList[i].tail.y));
	}

	return Vector(minX, minY, maxX, maxY);
}
