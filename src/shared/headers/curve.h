#pragma once

#include <map>
#include <stdexcept>

class Curve{
    private:
        int minX=0;
        int maxX=100;
        int minY=0;
        int maxY=100;
        static inline int lerp(int a, int b, float t){
            int diff = b - a;
            return diff * t + a + .5f;
        }

    public:    
        void setDomain(int minX, int maxX) { 
            if (minX > maxX) throw std::out_of_range("minX > maxX");
            this->minX = minX; 
            this->maxX = maxX; 
        }
        void setRange(int minY, int maxY) { 
            if (minY > maxY) throw std::out_of_range("minY > maxY");
            this->minY = minY; 
            this->maxY = maxY; 
        }
        int getMinX() { return minX; }
        int getMaxX() { return maxX; }
        int getMinY() { return minY; }
        int getMaxY() { return maxY; }

        //todo: replace this with something stripped down, with range checking 
        std::map<int, int> points;

        //todo: test
        int getY(int x){
            if(x < minX || x > maxX) throw std::out_of_range("x is out of range");
            //todo: maintain lookup table for fast calculation

            //todo: this is only needed cause we arent enforcing good points yet
            auto coerce = [](int n, int min, int max){ 
                if (n > max) return max;
                if (n < min) return min;
                return n;
            };

            int prevX = minX;
            int prevY = minY;
            for(auto point : points){
                int pointX = coerce(point.first, minX, maxX);
                int pointY = coerce(point.second, minY, maxY);
                if(pointX >= x){
                    //EARLY RETURN
                    return lerp(
                        prevY, 
                        pointY, 
                        pointX == prevX ? 
                            1 : (x-prevX)/(float)(pointX-prevX));
                }else{
                    prevX = pointX;
                    prevY = pointY;
                }
            }

            //only reached if no 2 points to lerp
            return prevY;
        }

};