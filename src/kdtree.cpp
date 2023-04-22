#include "./kdtree.hpp"
#include <vector>
#include <algorithm>

using namespace std;

HRect::HRect(Point p1, Point p2, int dim)
{
    vector<double> new_mins(dim, 0);
    vector<double> new_maxs(dim, 0);

    for (int i = 0; i < dim; i++)
    {
        bool is_large = p1[i] > p2[i];
        new_mins[i] = is_large ? p2[i] : p1[i];
        new_maxs[i] = is_large ? p1[i] : p2[i];
    }

    mins = new_mins;
    maxs = new_maxs;
}

bool interact(HRect &r1, HRect &r2)
{
    bool result = true;
    int dim = r1.mins.size();
    for (int i = 0; i < dim; i++)
    {
        bool c1 = r2.mins[i] >= r1.mins[i] && r2.mins[i] < r1.maxs[i];
        bool c2 = r2.maxs[i] >= r1.mins[i] && r2.maxs[i] < r1.maxs[i];

        if (!(c1 || c2))
        {
            result = false;
            break;
        }
    }
    return result;
}

bool is_include(const HRect &r, Point p)
{
    bool result = true;
    int dim = r.mins.size();
    for (int i = 0; i < dim; i++)
    {
        if (!(p[i] >= r.mins[i] && p[i] <= r.maxs[i]))
        {
            result = false;
            break;
        }
    }
    return result;
}

bool is_equal(Point& p1, Point& p2)
{
    bool result = true;
    int dim = p1.size();
    for (int i = 0; i < dim; i++)
    {
        if(p1[i] != p2[i])
        {
            result = false;
            break;
        }
    }
    return result;
}

KDTree::KDTree(int n_dim, int n_leaf, vector<Point> ps)
{
    dim = n_dim;
    cur_dim = 0;
    leaf_size = n_leaf;
    left = nullptr;
    right = nullptr;
    points = ps;

    if (points.size() > leaf_size)
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

    if (points.size() > leaf_size)
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
         [cur_dim](Point a, Point b)
         {
             return (a[cur_dim] < b[cur_dim]);
         });

    int m = points.size() / 2;
    int next_dim = (cur_dim + 1) % dim;
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

    if (is_leaf && points.size() > leaf_size)
    {
        split(cur_dim);
    }
}

vector<Point> KDTree::range_query(HRect &rect)
{
    bool is_leaf = (left == nullptr) && (right == nullptr);
    vector<Point> ps;
    if (!is_leaf)
    {
        if (rect.mins[cur_dim] <= median[cur_dim])
        {
            vector<Point> lpoints = left->range_query(rect);
            ps.insert(ps.end(), lpoints.begin(), lpoints.end());
        }

        if (rect.maxs[cur_dim] >= median[cur_dim])
        {
            vector<Point> rpoints = right->range_query(rect);
            ps.insert(ps.end(), rpoints.begin(), rpoints.end());
        }
    }
    else
    {
        for (auto p : points)
        {
            if (is_include(rect, p))
            {
                ps.push_back(p);
            }
        }
    }
    return ps;
}

Point KDTree::point_query(Point& p) 
{
    bool is_leaf = (left == nullptr) && (right == nullptr);
    Point result;

    if (!is_leaf)
    {
        if (p[cur_dim] <= median[cur_dim])
        {
            result = left->point_query(p);
        }

        if (p[cur_dim] >= median[cur_dim])
        {
            result = right->point_query(p);
        }
    }
    else
    {
        for (auto op : points)
        {
            if(is_equal(p, op))
                return op;
        }
    }
    return result;
}