#ifndef KD_TREE_HPP
#define KD_TREE_HPP

#include <vector>

using namespace std;
typedef vector<double> Point;

struct HRect
{
    vector<double> mins;
    vector<double> maxs;
    HRect(Point p1, Point p2, int dim);
};

class KDTree
{
private:
    int dim;
    int cur_dim;
    int leaf_size;
    Point median;
    vector<Point> points;
    KDTree *left;
    KDTree *right;
    void split(int cur_dim);

public:
    // The constructor for root node
    KDTree(int n_dim, int n_leaf, vector<Point> ps);
    KDTree(int n_dim, int c_dim, int n_leaf, vector<Point> ps);
    ~KDTree();
    void insert(Point p);
    vector<Point> range_query(HRect& rect);
    Point point_query(Point& p);
};

#endif