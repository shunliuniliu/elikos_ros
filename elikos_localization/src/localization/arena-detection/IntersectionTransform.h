#ifndef INTERSECTION_TRANSFORM_H
#define INTERSECTION_TRANSFORM_H

#include <vector>

#include <Eigen/Core>
#include <Eigen/Dense>

#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

//Here is the temporary ros stuff
#include <ros/ros.h>
#include <geometry_msgs/PoseArray.h>

namespace localization
{

class QuadState;

class IntersectionTransform
{
public:

    IntersectionTransform(double focalLength, QuadState* state);
    ~IntersectionTransform() = default;

    void transformIntersections(const std::vector<Eigen::Vector2f>& imageIntersections, const ros::Time& stamp);

    inline void setFocalLength(double focalLength){focalLength_ = focalLength;}
    //TODO remove
    std::string frameId_ = "r200_front";
private:

    const double GRID_SIDE_LENGTH_M = 1.0;
    const double ALT_ERROR_THRESHOLD = 0.3;

    void updateKDTree(const std::vector<Eigen::Vector2f>& imageIntersections);
    double estimateAltitude(const std::vector<Eigen::Vector2f>& imageIntersections);
    void transformIntersectionXY(const Eigen::Vector2f& imageIntersection, Eigen::Vector3f& intersection) const;
    void publishTransformedIntersections(const std::vector<Eigen::Vector3f>& intersections, const ros::Time& stamp) const;


    double focalLength_;
    QuadState* const state_;

    pcl::KdTreeFLANN<pcl::PointXY> kdTree_;
    pcl::PointCloud<pcl::PointXY>::Ptr pointCloud_;

    ros::Publisher posePublisher_;
};

}

#endif // INTERSECTION_TRANSFORM_H