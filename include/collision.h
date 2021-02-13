#ifndef collision_h_
#define collision_h_

#include <math.h>
#include <vector>
#include <SDL.h>

//Circle wrapper class
class Circle
{
private:
	int x = 0, y = 0;
	int r = 0;
public:
	Circle():x(0), y(0), r(0) {}
	Circle(int x, int y):x(x), y(y), r(0) {}
	Circle(int x, int y, int r):x(x), y(y), r(r) {}
	friend double Distance(Circle a, Circle b);
};

/*
 * \brief Calculate the distance between two centers of circle.
 */
inline double Distance(Circle a, Circle b)
{
	return sqrt(pow(((double)a.x - (double)b.x), 2) + pow(((double)a.y - (double)b.y), 2));
}

/*
 * \brief Determine if two rectangles collide externally.
 * \param rect1, rect2 The target rectangles.
 * \return 1 if collided, or 0 if not collided.
 */
bool OutsideCollided(SDL_Rect rect1, SDL_Rect rect2)
{
	if (rect1.x + rect1.w <= rect2.x || rect2.x + rect2.w <= rect1.x || rect1.y + rect1.h <= rect2.y || rect2.y + rect2.h <= rect1.y)
		return 0;
	else
		return 1;
}

/*
 * \brief Determine if a set of collision boxes and a rectangle collide externally.
 * \param A The target collision boxes.
 * \param rect The target rectangle.
 * \return 1 if collided, or 0 if not collided.
 */
bool OutsideCollided(std::vector<SDL_Rect> A, SDL_Rect rect)
{
	for (int i = 0; i < A.size(); i++)
	{
		if (OutsideCollided(A[i], rect))
			return 1;
	}
	return 0;
}

/*
 * \brief Determine if two sets of collision boxes collide externally.
 * \param A, B The target collision boxes.
 * \return 1 if collided, or 0 if not collided.
 */
bool OutsideCollided(std::vector<SDL_Rect> A, std::vector<SDL_Rect> B)
{
	for (int i = 0; i < A.size(); i++)
	{
		for (int j = 0; j < B.size(); j++)
			if (OutsideCollided(A[i], B[j]))
				return 1;
	}
	return 0;
}

/*
 * \brief Determine if two rectangles collide internally.
 * \param rect1, rect2 The target rectangles.
 * \return 1 if collided, or 0 if not collided.
 */
bool InsideCollided(SDL_Rect rect1, SDL_Rect rect2)
{
	if (rect1.x >= rect2.x && rect1.x + rect1.w <= rect2.x + rect2.w && rect1.y >= rect2.y && rect1.y + rect1.h <= rect2.y + rect2.h)
		return 0;
	else if (rect2.x >= rect1.x && rect2.x + rect2.w <= rect1.x + rect1.w && rect2.y >= rect1.y && rect2.y + rect2.h <= rect1.y + rect1.h)
		return 0;
	else
		return 1;
}

/*
 * \brief Determine if a set of collision boxes and a rectangle collide internally.
 * \param A The target collision boxes.
 * \param rect The target rectangle.
 * \return 1 if collided, or 0 if not collided.
 */
bool InsideCollided(std::vector<SDL_Rect> A, SDL_Rect rect)
{
	for (int i = 0; i < A.size(); i++)
	{
		if (InsideCollided(A[i], rect))
			return 1;
	}
	return 0;
}

/*
 * \brief Determine if two sets of collision boxes collide internally.
 * \param A, B The target collision boxes.
 * \return 1 if collided, or 0 if not collided.
 */
bool InsideCollided(std::vector<SDL_Rect> A, std::vector<SDL_Rect> B)
{
	for (int i = 0; i < A.size(); i++)
	{
		for (int j = 0; j < B.size(); j++)
			if (InsideCollided(A[i], B[j]))
				return 1;
	}
	return 0;
}


#endif // !collision_h_