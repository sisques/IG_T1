#ifndef COLOR_H
#define COLOR_H

class color {
public:
    double r, g, b;

    color() = default;
    ~color() = default;

    double clamp(double t, double min, double max){
        if ( t > max ){ return max; }
        if (t < min ){ return  min; }
        return t;
    }

    color(double _rgb) {
        this -> r = clamp(_rgb,0,1);
        this -> g = clamp(_rgb,0,1);
        this -> b = clamp(_rgb,0,1);
    }


    color(double _r, double _g, double _b) {
        this -> r = clamp(_r,0,1);
        this -> g = clamp(_g,0,1);
        this -> b = clamp(_b,0,1);
    }

    color(color const &c) {
        this -> r = clamp(c.r,0,1);
        this -> g = clamp(c.g,0,1);
        this -> b = clamp(c.b,0,1);
    }

};


#endif //COLOR_H
