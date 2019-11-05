#include "boolean.h"
#include "ray.h"
#include <limits>
#include <algorithm>
// Determine if the ray intersects with the boolean of A and B.
struct AoBhit {
    Hit hit;
    bool isObjectB;
};
enum DiffStatus {
    INSIDE_A, INSIDE_B, INSIDE_AB, OUTSIDE
};

bool diffHit_cmp(AoBhit &a, AoBhit &b) {
    if (a.hit.t < b.hit.t) return true;
    else return false;
}
bool Boolean::Intersection(const Ray &ray, std::vector<Hit> &hits) const {
    // TODO
    std::vector<Hit> subhitA;
    std::vector<Hit> subhitB;
    Hit finalHit;
    bool didHitA = A->Intersection(ray, subhitA);
    bool didHitB = B->Intersection(ray, subhitB);

    if (type == type_union) {
        bool inside = false;
        bool isFirstHit = true;
        bool didHit = true;
        std::vector<AoBhit> todosHits;

        if (subhitA.size() % 2 == 1) inside = true;
        if (subhitB.size() % 2 == 1) inside = true;
        for (auto &&a : subhitA) {
            todosHits.push_back(AoBhit{a, false});
        }
        for (auto &&b : subhitB) {
            todosHits.push_back(AoBhit{b, true});
        }
        if (todosHits.size() == 0)return false;
        AoBhit lastHit = todosHits[0];
        std::sort(todosHits.begin(), todosHits.end(), diffHit_cmp);
        for (auto &&hit : todosHits) {
            if (hit.hit.ray_exiting) {
                hit.hit.part = hit.hit.part * 2 + hit.isObjectB;
                hit.hit.object = this;
                hits.push_back(hit.hit);
                didHit = true;
                if (!isFirstHit && inside) {
                    lastHit.hit.part = lastHit.hit.part * 2 + lastHit.isObjectB;
                    lastHit.hit.object = this;
                    hits.push_back(lastHit.hit);
                    inside = false;
                    didHit = true;
                }
            } else {
                inside = true;
            }
            isFirstHit = false;
            lastHit = hit;
        }
        if (!isFirstHit && inside) {
            lastHit.hit.part = lastHit.hit.part * 2 + lastHit.isObjectB;
            lastHit.hit.object = this;
            hits.push_back(lastHit.hit);
            inside = false;
            didHit = true;
        }
        return didHit;

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
        DiffStatus diff_status;
        std::vector<AoBhit> diffHit;
        for (auto &&a : subhitA) {
            diffHit.push_back(AoBhit{a, false});
        }
        for (auto &&b : subhitB) {
            diffHit.push_back(AoBhit{b, true});
        }
        std::sort(diffHit.begin(), diffHit.end(), diffHit_cmp);

        diff_status = OUTSIDE;
        if (A->isInside(ray.endpoint))
            diff_status = INSIDE_A;
        if (B->isInside(ray.endpoint))
            diff_status = INSIDE_B;
        if (A->isInside(ray.endpoint) && B->isInside(ray.endpoint))
            diff_status = INSIDE_AB;

        for (auto &&hit : diffHit) {
            //ray_exiting is true when the watchpoint is outside
            if (hit.isObjectB && hit.hit.ray_exiting) {
                switch (diff_status) {
                    case OUTSIDE: //#1
                        diff_status = INSIDE_B;
                        break;
                    case INSIDE_A: //#7
                        hit.hit.object = this;
                        hit.hit.part = hit.hit.part * 2 + 1;
                        hits.push_back(hit.hit);
                        return true;
                        break;
                    default:
                        break;
                }
            } else if (hit.isObjectB && !hit.hit.ray_exiting) {
                switch (diff_status) {
                    case INSIDE_AB: //#8
                        hit.hit.object = this;
                        hit.hit.part = hit.hit.part * 2 + 1;
                        hits.push_back(hit.hit);
                        return true;
                        break;
                    case INSIDE_B: //#2
                        diff_status = OUTSIDE;
                        break;
                    default:
                        break;
                }
            } else if (!hit.isObjectB && hit.hit.ray_exiting) {
                switch (diff_status) {
                    case OUTSIDE: //#5
                        hit.hit.object = this;
                        hit.hit.part *= 2;
                        hits.push_back(hit.hit);
                        return true;
                        break;
                    case INSIDE_B: //#4
                        diff_status = INSIDE_AB;
                        break;
                    default:
                        break;
                }
            } else if (!hit.isObjectB && !hit.hit.ray_exiting) {
                switch (diff_status) {
                    case INSIDE_AB: //#3
                        diff_status = INSIDE_B;
                        break;
                    case INSIDE_A: //#6
                        hit.hit.object = this;
                        hit.hit.part *= 2;
                        hits.push_back(hit.hit);
                        return true;
                        break;
                    case INSIDE_B:
                        break;
                    default:
                        break;
                }
            }
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
        return B->Normal(point, (part) / 2);
}

bool Boolean::isInside(const vec3 &point) {
    switch (type) {
        case type_intersection:
            if (A->isInside(point) && B->isInside(point))return true;
        case type_difference:
            if (A->isInside(point) && !B->isInside(point))return true;
        case type_union:
            if (A->isInside(point) || B->isInside(point))return true;

    }
    return false;
}
