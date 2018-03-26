/*****************************
 * Implements basic features *
 * of Nelder-Mead Algorithm  *
 * Author: Haoyuan Sun       *
 *****************************/

#include "nelder_mead.hpp"

simplex::simplex(int d, cost_function* f, std::vector<coord> initial) : 
        dim(d), cf(f) {
    centroid = coord(dim, 0);
    for(coord c : initial) {
        for(int i = 0; i < dim; ++i) 
            centroid[i] += c[i] / (dim+1);
        pts.insert({(*cf)(c), c});
    }
}

coord& simplex::get_pt(terminal x) {
    if (x == terminal::cent) return centroid;

    auto it = pts.begin();
    if (x == terminal::vb) return it->second;
    if (x == terminal::vsb) {
        ++it;
        return it->second; 
    }
    it = pts.end();
    --it;
    if(x == terminal::vw) return it->second;
    else { // x == terminal::vsw
        --it;
        return it->second;
    }
}

coord simplex::eval(Node* root) {
    if (root->t == type::terminal) 
        return get_pt((terminal)root->id);

    std::vector<coord> args;
    for(Node* n : root->children) {
        args.push_back(eval(n));
    }

    func p = (func)root->id;
    if(p == func::ifelse) {
        return primitive::ifelse(this->cf, args[0], args[1], 
                    args[2], args[3]);
    }
    else if(p == func::refl) {
        return primitive::refl(args[0], args[1]);
    }
    else if(p == func::expan) {
        return primitive::expan(args[0], args[1]);
    }
    else { // p == func::contra 
        return primitive::contr(args[0], args[1]);
    }
}

void simplex::replace_worst(coord np) {
    auto it = --(pts.end());

    for (int i = 0; i < dim; ++i) {
        centroid[i] += (np[i] - it->second[i]) / (dim+1);    
    }

    pts.erase(it);
    pts.insert({(*cf)(np), np});
}


coord primitive::ifelse(cost_function* f, coord& a, coord& b,
        coord& c, coord& d) {
    if ((*f)(a) < (*f)(b)) return c;
    else return d;
}

coord primitive::refl(coord& a, coord& b) {
    coord ret;

    for(std::size_t i = 0; i < a.size(); ++i)
        ret.push_back(2 * a[i] - b[i]);

    return ret;
}

coord primitive::expan(coord& a, coord& b) {
    coord ret;

    for(std::size_t i = 0; i < a.size(); ++i)
        ret.push_back(3 * a[i] - b[i]);

    return ret;
}

coord primitive::contr(coord& a, coord& b) {
    coord ret;

    for(std::size_t i = 0; i < a.size(); ++i)
        ret.push_back(0.5 * a[i] + b[i]);

    return ret;
}
