#include <algorithm>
#include "curve.h"

using namespace std;
using namespace Fannn;

void Curve::chopLeft(vector<Point>& points) {
    size_t numOutside = 0;

    while (numOutside != points.size() && points[numOutside].x < minX)
        ++numOutside;

    if (numOutside > 0) {
        bool alreadyHavePointOnBorder =
                points.size() > numOutside &&
                points[numOutside].x == minX;

        if (alreadyHavePointOnBorder){
            eraseIfValidRange(points, 0, numOutside);
        } else {
            //erase all but one of them
            eraseIfValidRange(points, 0, numOutside-1);
            //move it in inside
            Point first = points[0];
            if (points.size() > 1)
                first.y = lerpYBasedOnX(first, points[1], minX);
            first.x = minX;
            points[0] = first;
        }
    }
}

void Curve::chopRight(vector<Point>& points) {
    size_t numOutside = 0;

    //todo: ensure inlined
    auto indexOfFirstOutside = [&](){ return points.size()-numOutside; };
    auto indexOfLastInside = [&](){ return indexOfFirstOutside()-1; };

    while (numOutside != points.size() && points[indexOfLastInside()].x > maxX)
        ++numOutside;

    if (numOutside > 0) {
        bool alreadyHavePointOnBorder =
                points.size() > numOutside &&
                points[indexOfLastInside()].x == minX;

        if (alreadyHavePointOnBorder){
            eraseIfValidRange(points, indexOfFirstOutside(), points.size());
        } else {
            //erase all but one of them
            eraseIfValidRange(points, indexOfFirstOutside()+1, points.size());

            //move last one into range
            Point last = points[points.size()-1];
            if (points.size() > 1)
                last.y = lerpYBasedOnX(last, points[points.size()-2], maxX);
            last.x = maxX;
            points[points.size()-1] = last;
            }
    }
}

void Curve::chopTopAndBottom(vector<Point>& points) {
    for (int i=0; i<points.size(); ++i) {
        if (points[i].y < minY) {
            //TRUNCATE RANGE BELOW minY

            int from = i;
            while (++i < points.size() && points[i].y < minY);
            int to = i-1;

            eraseIfValidRange(points, from+1, to);

            bool atBegining = from == 0;
            bool atEnd = to == points.size()-1;
            bool onlyOnePoint = from == to;

            Point p = points[from];
            if (!onlyOnePoint && !atBegining || onlyOnePoint && atEnd && !atBegining)
                p.x = lerpXBasedOnY(points[from], points[from-1], minY);
            p.y = minY;
            points[from] = p;

            if (!onlyOnePoint){
                p.x = atEnd ?
                        points[to].x :
                        lerpXBasedOnY(points[to], points[to+1], minY);
                points[to] = p;
            }

        } else if (points[i].y > maxY) {
            //TRUNCATE RANGE ABOVE maxY

            int from = i;
            while (++i < points.size() && points[i].y > maxY);
            int to = i-1;

            eraseIfValidRange(points, from+1, to);

            bool atBegining = from == 0;
            bool atEnd = to == points.size()-1;
            bool onlyOnePoint = from == to;

            Point p = points[from];
            if (!onlyOnePoint && !atBegining || onlyOnePoint && atEnd && !atBegining)
                p.x = lerpXBasedOnY(points[from], points[from-1], maxY);
            p.y = maxY;
            points[from] = p;

            if (!onlyOnePoint){
                p.x = atEnd ?
                        points[to].x :
                        lerpXBasedOnY(points[to], points[to+1], maxY);
                points[to] = p;
            }

        }
    }
}

void Curve::setDomain(double minX, double maxX) {
    if (minX > maxX) throw out_of_range("minX > maxX");

    bool shrinkingLeft = this->minX < minX;
    bool shrinkingRight = this->maxX > maxX;

    this->minX = minX;
    this->maxX = maxX;

    if (shrinkingLeft) chopLeft(points);
    if (shrinkingRight) chopRight(points);
}

void Curve::setRange(double minY, double maxY) {
    if (minY > maxY) throw out_of_range("minY > maxY");

    bool shrinkingBottom = this->minY < minY;
    bool shrinkingTop = this->maxY > maxY;

    this->minY = minY;
    this->maxY = maxY;

    if (shrinkingBottom || shrinkingTop)
        chopTopAndBottom(points);
}

void Curve::removePoint(int index) {
    points.erase(points.begin()+index, points.begin()+index+1);
}

int Curve::addPoint(Point point) {
    //todo: coerce or reject point
    return sortedInsert(points, point);
}

void Curve::updatePoint(int index, Point point) {
    point.x = clamp(point.x,
                    index>0? points[index-1].x : minX,
                    (index<points.size()-1)? points[index+1].x : maxX);
    point.y = clamp(point.y, minY, maxY);
    points[index] = point;
}

void Curve::removePoints(int first, int last) {
    if (first > last || first < 0 || last >= points.size())
        throw invalid_argument("invalid indexes to remove");
    points.erase(points.begin()+first, points.begin()+last+1);
}

void Curve::setPoints(vector<Point> const & points) {
    vector<Point> sorted;
    if (points.empty()) {
        sorted.push_back({0, 0});
    } else {
        sortPoints(sorted, points);
    }
    chopPoints(sorted);
    this->points = sorted;
}

double Curve::getY(double x) const {
    if(x < minX || x > maxX) throw out_of_range("x is out of range");

    double prevPointX = points[0].x;//will always be atleast 1 point
    double prevPointY = points[0].y;
    for(auto point : points){
        if(point.x >= x){
            //EARLY RETURN
            return lerp(
                prevPointY,
                point.y,
                point.x == prevPointX ?
                    1 : (x-prevPointX)/(point.x-prevPointX));
        }else{
            prevPointX = point.x;
            prevPointY = point.y;
        }
    }

    //only reached if no 2 points to lerp
    return prevPointY;
}
