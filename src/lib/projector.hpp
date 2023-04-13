#inclue <vector>


class Projector
{
  public:
    // Project an array of points
    virtual void project_points(std::vecitor<std::vector<float>> &points);
    
    // Project a single point
    virtual void project_point(std::vector<float>  &point);
}
