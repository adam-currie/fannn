#pragma once

#include <vector>
#include <stdexcept>
#include <assert.h>

namespace Fannn {

    class Curve {
        public:
            typedef struct Point {
                double x, y;
                Point(double x, double y) : x(x), y(y) {}
                bool operator==(const Point&) const = default;
            } Point;

            static constexpr double MIN_SPAN = 1;
        private:
            double minX=0;
            double maxX=100;
            double minY=0;
            double maxY=100;
            std::vector<Point> points;

            static inline double lerp(double a, double b, double t){
                double diff = b - a;
                return diff * t + a;
            }

            //scales a reference lerp to a lerp between a and b
            static inline double lerp(double a, double b, double refStart, double refEnd, double refBetween) {
                return lerp(a, b, (refBetween - refStart) / (refEnd - refStart));
            }

            static inline double lerpXBasedOnY(const Point a, const Point b, double y) {
                return lerp(a.x, b.x, a.y, b.y, y);
            }

            static inline double lerpYBasedOnX(const Point a, const Point b, double x) {
                return lerp(a.y, b.y, a.x, b.x, x);
            }

            inline int sortedInsert(std::vector<Point>& sorted, Point point) {
                int i = sorted.size();
                while (i>0 && point.x < sorted[i-1].x) --i;
                sorted.insert(sorted.begin()+i, point);
                return i;
            }

            //if points share an x coordinate their original order is preserved
            inline void sortPoints(std::vector<Point>& sorted, std::vector<Point> const & points) {
                assert(sorted.empty());
                sorted.reserve(points.size());
                sorted.push_back(points[0]);
                for (int i=1; i<points.size(); ++i)
                    sortedInsert(sorted, points[i]);
            }

            template<typename T>
            inline void eraseIfValidRange(std::vector<T>& v, size_t from, size_t to) {
                if (from >= to) 
                    return;
                v.erase(v.begin() + from, v.begin() + to);
            }

            void chopLeft(std::vector<Point>& points);
            void chopRight(std::vector<Point>& points);
            void chopTopAndBottom(std::vector<Point>& points);

            void chopPoints(std::vector<Point>& points) {
                chopLeft(points);
                chopRight(points);
                chopTopAndBottom(points);
            }

        public:
            std::string name;

            Curve(std::string name = "") : name(name) {}

            bool operator==(const Curve&) const = default;

            void setDomain(double minX, double maxX);
            void setRange(double minY, double maxY);

            void setMinX(double minX);
            void setMaxX(double maxX);
            void setMinY(double minY);
            void setMaxY(double maxY);

            double getMinX() const { return minX; }
            double getMaxX() const { return maxX; }
            double getMinY() const { return minY; }
            double getMaxY() const { return maxY; }

            std::vector<Point> const & getPoints() const { return points; }

            void setPoints(std::vector<Point> const & points);
            //returns the new index of the point
            int addPoint(Point point);
            bool updatePoint(int index, Point point);
            void removePoints(int first, int last);
            void removePoint(int index);

            double getY(double x) const;

    };

}
