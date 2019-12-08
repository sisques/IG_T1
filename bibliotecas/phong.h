#ifndef _PHONG_H_
#define _PHONG_H_

#include <math.h>
#include <stdlib.h>
#include "matrix.h"
#include "punto_direccion.h"
#include "russian_roulette.h"

using namespace std;

class Phong{
private:
    dir reflexion(const dir in, const dir n){
        dir inputRay = normalize(in);
        dir normal = normalize(n);
        dir output = 2*dot(normal, inputRay)*normal -inputRay;
        return -normalize(output);
    }

public:
    Phong(){}

    /*double phong(const dir indir, const dir outdir, const dir n, double specexp) {
        dir refindir = reflexion(indir, n);
        double s = dot(refindir, outdir);
        if (s < 0.0) {
            return 0.0;
        }
        return pow(s, specexp);
    }*/



    double lambert(const dir &indir, const dir &n) {
        double d = dot(n, indir);
        if (d < 0.0) {
            d = 0.0;
        }
        return d;
    }

    dir sample_lambert(const dir &n) {
        double rndx, rndy, rndz;
        double magnitude;
        do {
            rndx = 2.0 * (double) rand() / RAND_MAX - 1.0;
            rndy = 2.0 * (double) rand() / RAND_MAX - 1.0;
            rndz = 2.0 * (double) rand() / RAND_MAX - 1.0;
            magnitude = sqrt(rndx * rndx + rndy * rndy + rndz * rndz);
        } while (magnitude > 1.0);

        dir rnd_dir = newDir(rndx / magnitude, rndy / magnitude, rndz / magnitude);

        if (dot(rnd_dir, n) < 0) {
            return -rnd_dir;
        }

        return rnd_dir;
    }

    dir sample_phong(const dir &outdir, const dir &n, double specexp) {
        Matrix mat;
        dir ldir = normalize(outdir);

        dir ref = reflexion(ldir, n);

        double ndotl = dot(ldir, n);

        if(1.0 - ndotl > EPSILON) {
            dir ivec, kvec, jvec;

            // build orthonormal basis
            if(fabs(ndotl) < EPSILON) {
                kvec = -normalize(ldir);
                jvec = n;
                ivec = cross(jvec, kvec);
            } else {
                ivec = normalize(cross(ldir, ref));
                jvec = ref;
                kvec = cross(ref, ivec);
            }

            mat = originalBase(ivec, jvec, kvec, newPoint(0,0,0));
        }

        double rnd1 = (double)rand() / RAND_MAX;
        double rnd2 = (double)rand() / RAND_MAX;

        double phi = acos(pow(rnd1, 1.0 / (specexp + 1)));
        double theta = 2.0 * M_PI * rnd2;

        dir v;
        v.x = cos(theta) * sin(phi);
        v.y = cos(phi);
        v.z = sin(theta) * sin(phi);
        v = v*mat;

        return v;
    }

};

#endif