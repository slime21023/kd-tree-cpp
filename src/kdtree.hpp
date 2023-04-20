#include <vector>

using namespace std;
typedef vector<double> Point;

struct MBR
{
    vector<double> mins;
    vector<double> max;
};

class KDTree
{
private:
    int dim;
    int cur_dim;
    int leaf_size;
    Point median;
    vector<Point> points;
    KDTree* left;
    KDTree* right;

public:
    // The constructor for root node
    KDTree(int n_dim, int n_leaf, vector<Point> ps);
    KDTree(int n_dim, int c_dim, int n_leaf, vector<Point> ps);
    ~KDTree();
    void split(int cur_dim);
    void insert(Point p);
    vector<Point> range_query(MBR mbr);
};