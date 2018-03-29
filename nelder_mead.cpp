/*****************************
 * Implements basic features *
 * of Nelder-Mead Algorithm  *
 * Author: Haoyuan Sun       *
 *****************************/

#include "nelder_mead.hpp"

Node::~Node() {
    for (Node* n : this->children) delete n;
}

void Node::edit() {
    if (this->t == type::terminal) return;

    for(Node* n : this->children) n->edit();

    if (children[0]->t == type::func || children[1]->t == type::func)
        return;

    if ((func)this->id == func::ifelse) {
        int a = children[0]->id, b = children[1]->id;
        if ((terminal)a != terminal::cent && (terminal)b != terminal::cent) {
            int x = (a<b) ? 2 : 3;
            this->t = this->children[x]->t;
            this->id = this->children[x]->id;

            std::vector<Node*> tmp = this->children[x]->children;
            this->children[x]->children.clear();
            for(int i = 0; i < 4; ++i) delete this->children[i];
            this->children = tmp;
            for(Node* n : this->children) n->parent = this;

            propagate();
        }
    }
    else 
        if (children[0]->id == children[1]->id) {
            this->t = type::terminal;
            this->id = children[0]->id;
            this->children.clear();
            propagate();
        }

}

void Node::propagate() {
    if (this->t == type::terminal){
        this->num_term = 1;
        this->num_func = 0;
    }
    else {
        this->num_func = 1;
        this->num_term = 0;

        for(Node* n : children) {
            this->num_func += n->num_func;
            this->num_term += n->num_term;
        }
    }

    if(this->parent) this->parent->propagate(); 
}

simplex::simplex(int d, cost_function* f, std::vector<coord> initial) : 
        dim(d), cf(f) {
    centroid = coord(dim, 0);
    for(coord c : initial) {
        for(int i = 0; i < dim; ++i) 
            centroid[i] += c[i] / (dim+1);
        pts.insert({(*cf)(c), c});
    }
}

simplex simplex::make_copy() {
    simplex ret = simplex();
    ret.dim = this->dim;
    ret.cf = this->cf;
    ret.centroid = this->centroid;
    ret.pts = this->pts;

    return ret;
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

double simplex::compute(Node* root) {
	int count = 2500;
    while (true) {
    	double best = pts.begin()->first;
    	double worst = (--pts.end())->first;
    	if (worst - best < 0.0001) break;
    	if (worst / best > 50) break; // diverging, bad
    	
        replace_worst(eval(root));
        
        if (--count == 0) break; // in case converges too slowly
    }

    return pts.begin()->first;
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
