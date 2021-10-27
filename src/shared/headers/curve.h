#pragma once

#include <vector>
#include <stdexcept>
#include <assert.h>

namespace Fannn {

    class Curve {
        public:
            typedef struct Point {
                double x, y;
                bool operator==(const Point&) const = default;
            } Point;

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
                return lerp(
                        a, 
                        b, 
                        (refBetween - refStart) / (refBetween - refStart));
            }

            static inline double lerpXBasedOnY(const Point a, const Point b, double y) {
                return lerp(a.x, b.x, a.y, b.y, y);
            }

            static inline double lerpYBasedOnX(const Point a, const Point b, double x) {
                return lerp(a.y, b.y, a.x, b.x, x);
            }

            //if points share an x coordinate their original order is preserved
            inline void sortPoints(std::vector<Point>& sorted, std::vector<Point> const & points) {
                assert(sorted.empty());
                sorted.reserve(points.size());
                sorted.push_back(points[0]);
                for (int i=1; i<points.size(); ++i) {
                    auto r = sorted.rbegin();
                    while (r != sorted.rend() && points[i].x < r->x) ++r;
                    sorted.insert(r.base(), points[i]);
                }
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

            double getMinX() const { return minX; }
            double getMaxX() const { return maxX; }
            double getMinY() const { return minY; }
            double getMaxY() const { return maxY; }

            std::vector<Point> const & getPoints() const { return points; }

            void setPoints(std::vector<Point> const & points);

            double getY(double x) const;

    };

}
