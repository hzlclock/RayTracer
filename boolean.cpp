#include "boolean.h"
#include "ray.h"
#include <limits>

// Determine if the ray intersects with the boolean of A and B.
bool Boolean::Intersection(const Ray &ray, std::vector<Hit> &hits) const {
    // TODO
    std::vector<Hit> subhitA;
    std::vector<Hit> subhitB;
    Hit finalHit;
    bool didHitA = A->Intersection(ray, subhitA);
    bool didHitB = B->Intersection(ray, subhitB);
    if (type == type_union) {
        finalHit.t = std::numeric_limits<double>::max();
        if (didHitA) {
            for (auto &ahit: subhitA) {
                if (ahit.t < finalHit.t) {
                    ahit.object = this;
                    ahit.part = ahit.part * 2;
                    finalHit = ahit;
                }
            }
        }
        if (didHitB) {
            for (auto &bhit: subhitB) {
                if (bhit.t < finalHit.t) {
                    bhit.object = this;
                    bhit.part = bhit.part * 2 + 1;
                    finalHit = bhit;
                }
            }
        }
        if (didHitA || didHitB) {
            hits.push_back(finalHit);
            return true;
        }
    } else if (type == type_intersection) {
        if (didHitA && didHitB) {
            Hit hita, hitb;
            hita.t = std::numeric_limits<double>::max();
            hitb.t = std::numeric_limits<double>::max();
            finalHit.t = 0;
            for (auto &einHit: subhitA) {
                if (einHit.t < hita.t) {
                    hita = einHit;
                }
            }
            for (auto &einHit: subhitB) {
                if (einHit.t < hitb.t) {
                    hitb = einHit;
                }
            }
            if (hita.t > finalHit.t) {
                hita.object = this;
                hita.part = hita.part * 2;
                finalHit = hita;
            }
            if (hitb.t > finalHit.t) {
                hitb.object = this;
                hitb.part = hitb.part * 2 + 1;
                finalHit = hitb;
            }
            hits.push_back(finalHit);
        }
    } else if (type == type_difference) {
        if (didHitA && didHitB) {
            bool maxIsB = true, minIsB = true;
            double tMax = std::numeric_limits<double>::min();
            double tMin = std::numeric_limits<double>::max();
            for (auto &einHit: subhitB) {
                if (einHit.t > tMax) tMax = einHit.t;
                if (einHit.t < tMin) tMin = einHit.t;
            }
            for (auto &einHit: subhitA) {
                if (einHit.t > tMax) {
                    tMax = einHit.t;
                    maxIsB = false;
                }
                if (einHit.t < tMin) {
                    tMin = einHit.t;
                    minIsB = false;
                }
            }
            if (maxIsB && minIsB)
                return false;

            Hit hitb;
            hitb.t = 0;
            for (auto &einHit: subhitB) {
                if (einHit.t < hitb.t) {
                    hitb = einHit;
                }
            }
            hitb.part = hitb.part * 2 + 1;
            hitb.object = this;
            hits.push_back(hitb);
            return true;
        } else if (didHitA) {
            finalHit.t = std::numeric_limits<double>::max();
            for (auto &ahit: subhitA) {
                if (ahit.t < finalHit.t) {
                    ahit.object = this;
                    ahit.part = ahit.part * 2;
                    finalHit = ahit;
                }
            }
            hits.push_back(finalHit);
            return true;
        }
    }
    return false;
}

// This should never be called.
vec3 Boolean::Normal(const vec3 &point, int part) const {
    //assert(false);
    if (part % 2 == 0)
        return A->Normal(point, part / 2);
    else
        return B->Normal(point, (part - 1) / 2);
}
