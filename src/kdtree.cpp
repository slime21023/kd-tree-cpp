#include "./kdtree.hpp"
#include <vector>
#include <algorithm>

using namespace std;

KDTree::KDTree(int n_dim, int n_leaf, vector<Point> ps)
{
    dim = n_dim;
    cur_dim = 0;
    leaf_size = n_leaf;
    left = nullptr;
    right = nullptr;
    points = ps;

    if(points.size() > leaf_size)
    {
        split(cur_dim);
    }
}


KDTree::KDTree(int n_dim, int c_dim, int n_leaf, vector<Point> ps)
{
    dim = n_dim;
    cur_dim = c_dim;
    leaf_size = n_leaf;
    left = nullptr;
    right = nullptr;
    points = ps;

    if(points.size() > leaf_size)
    {
        split(cur_dim);
    }
}


KDTree::~KDTree()
{
    delete left;
    delete right;
}


void KDTree::split(int cur_dim) 
{
    sort(points.begin(), points.end(),
        [cur_dim](Point a, Point b){
            return (a[cur_dim] < b[cur_dim]);
        }
    );

    int m = points.size() / 2;
    int next_dim = (cur_dim +1) % dim;
    median = points[m];
    vector<Point> left_points = vector<Point>(points.begin(), points.begin() + m);
    vector<Point> right_points = vector<Point>(points.begin() + m, points.end());
    
    left = new KDTree(dim, next_dim, leaf_size, left_points);
    right = new KDTree(dim, next_dim, leaf_size, right_points);
    points.clear();
}

void KDTree::insert(Point p)
{
    bool is_leaf = (left == nullptr) && (right == nullptr);

    if (is_leaf)
    {
        points.push_back(p);
    }
    else
    {
        (p[cur_dim] < median[cur_dim]) ? left->insert(p) : right->insert(p);
    }

    if (is_leaf && points.size() > leaf_size) {
        split(cur_dim);
    }
}


vector<Point> KDTree::range_query(MBR mbr)
{
    bool is_leaf = (left == nullptr) && (right == nullptr);

    if(!is_leaf)
    {
        
    }
}