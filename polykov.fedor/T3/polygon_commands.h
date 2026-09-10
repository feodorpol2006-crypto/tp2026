#ifndef POLYGON_COMMANDS_H
#define POLYGON_COMMANDS_H

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>
#include <cstdlib>
#include <string>
#include <utility>
#include <cstddef>

struct Point
{
    int x, y;
};

struct Polygon
{
    std::vector<Point> points;
};

inline std::istream& operator>>(std::istream& in, Point& p)
{
    int x = 0, y = 0;
    char open, semicolon, close;

    if (in >> open >> x >> semicolon >> y >> close)
    {
        if (open == '(' && semicolon == ';' && close == ')')
        {
            p.x = x;
            p.y = y;
        }
        else
        {
            in.setstate(std::ios::failbit);
        }
    }
    return in;
}

inline std::istream& operator>>(std::istream& in, Polygon& pol)
{
    size_t size = 0;
    if (!(in >> size) || size < 3)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::vector<Point> temp;
    temp.reserve(size);

    Point p;
    for (std::size_t i = 0; i < size; ++i)
    {
        if (in >> p)
        {
            temp.push_back(p);
        }
        else
        {
            in.setstate(std::ios::failbit);
            return in;
        }
    }

    pol.points = std::move(temp);
    return in;
}

inline bool operator==(const Point& a, const Point& b)
{
    return a.x == b.x && a.y == b.y;
}

inline bool operator==(const Polygon& a, const Polygon& b)
{
    return a.points == b.points;
}

struct AreaCalculator
{
    const std::vector<Point>& pts;
    size_t size;
    mutable size_t idx;

    explicit AreaCalculator(const std::vector<Point>& pts)
        : pts(pts), size(pts.size()), idx(0)
    {
    }

    long long operator()(long long acc, const Point& curr) const
    {
        const Point& next = pts[(idx + 1) % size];
        long long term = static_cast<long long>(curr.x) * next.y
            - static_cast<long long>(next.x) * curr.y;
        ++idx;
        return acc + term;
    }
};

inline double getArea(const Polygon& p)
{
    if (p.points.size() < 3)
        return 0.0;

    long long sum = std::accumulate(p.points.begin(), p.points.end(),
        0LL, AreaCalculator(p.points));
    return std::abs(sum) / 2.0;
}

struct CountIfEvenOrOdd
{
    std::string arg;

    explicit CountIfEvenOrOdd(const std::string& s)
        : arg(s)
    {
    }

    bool operator()(const Polygon& p) const
    {
        if (arg == "EVEN")
        {
            return p.points.size() % 2 == 0;
        }
        return p.points.size() % 2 != 0;
    }
};

struct SumArea
{
    double operator()(double acc, const Polygon& p) const
    {
        return acc + getArea(p);
    }
};

struct SumAreaIfEven
{
    double operator()(double acc, const Polygon& p) const
    {
        if (p.points.size() % 2 == 0)
        {
            return acc + getArea(p);
        }
        else
        {
            return acc;
        }
    }
};

struct SumAreaIfOdd
{
    double operator()(double acc, const Polygon& p) const
    {
        if (p.points.size() % 2 != 0)
        {
            return acc + getArea(p);
        }
        else
        {
            return acc;
        }
    }
};

struct SumAreaByVertexes
{
    size_t verx;

    explicit SumAreaByVertexes(size_t n)
        : verx(n)
    {
    }

    double operator()(double acc, const Polygon& p) const
    {
        if (p.points.size() == verx)
        {
            return acc + getArea(p);
        }
        else
        {
            return acc;
        }
    }
};

struct CompareByArea
{
    bool operator()(const Polygon& a, const Polygon& b) const
    {
        return getArea(a) < getArea(b);
    }
};

struct HasRightAngle
{
    bool operator()(const Polygon& p) const
    {
        size_t n = p.points.size();
        if (n < 3)
            return false;

        std::vector<size_t> indices(n);
        std::iota(indices.begin(), indices.end(), 0);

        return std::any_of(indices.begin(), indices.end(),
            [&p, n](size_t i)
            {
                const Point& A = p.points[i];
                const Point& B = p.points[(i + 1) % n];
                const Point& C = p.points[(i + 2) % n];

                long long ABx = static_cast<long long>(B.x) - A.x;
                long long ABy = static_cast<long long>(B.y) - A.y;
                long long BCx = static_cast<long long>(C.x) - B.x;
                long long BCy = static_cast<long long>(C.y) - B.y;

                return ABx * BCx + ABy * BCy == 0;
            });
    }
};

inline Point normalizePoint(Point p)
{
    if (p.x > p.y)
    {
        std::swap(p.x, p.y);
    }
    return p;
}

struct ComparePoints
{
    bool operator()(const Point& a, const Point& b) const
    {
        if (a.x == b.x)
        {
            return a.y < b.y;
        }
        return a.x < b.x;
    }
};

struct IsPermutationOf
{
    Polygon target;

    explicit IsPermutationOf(const Polygon& sample)
        : target(sample)
    {
        std::transform(target.points.begin(), target.points.end(),
            target.points.begin(), normalizePoint);
        std::sort(target.points.begin(), target.points.end(), ComparePoints());
    }

    bool operator()(const Polygon& p) const
    {
        Polygon copy = p;
        std::transform(copy.points.begin(), copy.points.end(),
            copy.points.begin(), normalizePoint);
        std::sort(copy.points.begin(), copy.points.end(), ComparePoints());
        return copy.points == target.points;
    }
};

#endif // POLYGON_COMMANDS_H
