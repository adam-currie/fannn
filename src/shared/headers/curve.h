#pragma once

#include <map>
#include <stdexcept>

namespace Fannn {

    class Curve {
        private:
            double minX=0;
            double maxX=100;
            double minY=0;
            double maxY=100;
            static inline double lerp(double a, double b, double t){
                double diff = b - a;
                return diff * t + a;
            }

        public:    
            bool operator==(const Curve&) const = default;

            void setDomain(double minX, double maxX) { 
                if (minX > maxX) throw std::out_of_range("minX > maxX");
                this->minX = minX; 
                this->maxX = maxX; 
            }
            void setRange(double minY, double maxY) { 
                if (minY > maxY) throw std::out_of_range("minY > maxY");
                this->minY = minY; 
                this->maxY = maxY; 
            }
            double getMinX() const { return minX; }
            double getMaxX() const { return maxX; }
            double getMinY() const { return minY; }
            double getMaxY() const { return maxY; }

            //todo: replace this with something stripped down, with range checking 
            std::map<double, double> points;

            double getY(double x) const {
                if(x < minX || x > maxX) throw std::out_of_range("x is out of range");

                //todo: this is only here cause we arent enforcing good points yet
                auto coerce = [](double n, double min, double max){ 
                    if (n > max) return max;
                    if (n < min) return min;
                    return n;
                };

                double prevPointX = points.begin()->first;//should always be atleast 1 point
                double prevPointY = points.begin()->second;
                for(auto point : points){
                    double pointX = coerce(point.first, minX, maxX);
                    double pointY = coerce(point.second, minY, maxY);
                    if(pointX >= x){
                        //EARLY RETURN
                        return lerp(
                            prevPointY, 
                            pointY, 
                            pointX == prevPointX ? 
                                1 : (x-prevPointX)/(pointX-prevPointX));
                    }else{
                        prevPointX = pointX;
                        prevPointY = pointY;
                    }
                }

                //only reached if no 2 points to lerp
                return prevPointY;
            }

    };

}