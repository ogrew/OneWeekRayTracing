#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.h"

class hitable_list: public hitable {
    public:
        hitable_list() {}
        hitable_list(hitable **l, int n) {
            list = l;
            list_size = n;
        }
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
        hitable **list;
        int list_size;
};

bool hitable_list::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
    hit_record tmp_rec;
    bool hit_something = false;
    double closest_so_far = tmax;
    for (int i = 0; i < list_size; i++) {
        // 先に手前の物体に衝突すると
        // 後ろの物体にはclosest_so_farの範囲でhit判定に入らない
        if(list[i]->hit(r, tmin, closest_so_far, tmp_rec)) {
            hit_something = true;;
            closest_so_far = tmp_rec.t;
            rec = tmp_rec;
        }
    }
    return hit_something;
}

#endif
