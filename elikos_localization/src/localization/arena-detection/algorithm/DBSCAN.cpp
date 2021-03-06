//
// Created by Antonio Sanniravong
//

#include <iostream>

#include "DBSCAN.h"

#include <opencv2/core/core.hpp>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

namespace DBSCAN
{

void DBSCAN (const std::vector<Eigen::Vector2f> &dataset, double epsilon, int minPts, std::vector<int>& clusterMemberships) 
{
    if (dataset.empty()) return;

    // copy dataset into pointcloud.
    pcl::PointCloud<pcl::PointXY>::Ptr pc(new pcl::PointCloud<pcl::PointXY>());
    for (int i = 0; i < dataset.size(); ++i) {
        pc->push_back({ dataset[i].x(), dataset[i].y() });
    } 

    pcl::KdTreeFLANN<pcl::PointXY> tree;
    tree.setInputCloud(pc);

    int clusterID = 0;
    clusterMemberships.resize(dataset.size(), 0);
    std::vector<bool> visited(dataset.size(), false);
    std::vector<pcl::PointXY, Eigen::aligned_allocator<pcl::PointXY>>& points = pc->points;

    for (int i = 0; i < points.size(); i++) {
        if (visited[i]) continue;
        visited[i] = true;

        
        std::vector<int> neighborIndices;
        std::vector<float> distances;
        tree.radiusSearch(points[i], epsilon, neighborIndices, distances);

        if (neighborIndices.size() < minPts) {
            clusterMemberships[i] = -1;
        } else {
            clusterID++;
            clusterMemberships[i] = clusterID;
            for (int j = 0; j < neighborIndices.size(); ++j) {
                if (!visited[neighborIndices[j]]) {
                    visited[neighborIndices[j]] = true;

                     
                    std::vector<int> nestedNeighborIndices;
                    std::vector<float> nestedDistances;
                    tree.radiusSearch(points[neighborIndices[j]], epsilon, nestedNeighborIndices, nestedDistances);

                    if (nestedNeighborIndices.size() >= minPts){
                        neighborIndices.insert(neighborIndices.end(), nestedNeighborIndices.begin(), nestedNeighborIndices.end());
                    }
                }
                if (clusterMemberships[neighborIndices[j]] == 0) {
                    clusterMemberships[neighborIndices[j]] = clusterID;
                }
            }
        }
    }
}

Eigen::MatrixXd calculateDistMap (const std::vector<cv::Point2f> &dataset) {
    Eigen::MatrixXd distanceMap(dataset.size(), dataset.size());
    for (int i = 0; i < dataset.size(); i++){
        for (int j = i; j < dataset.size(); j++) {
            distanceMap(i, j) = calculateDistance(dataset, i, j);
            distanceMap(j, i) = distanceMap(i, j);
        }
    }
    return distanceMap;
}

Eigen::MatrixXd calculateDistMap (const std::vector<cv::Point2f> &dataset1, const std::vector<cv::Point2f> &dataset2) {
    Eigen::MatrixXd distanceMap(dataset1.size(), dataset2.size());
    for (int i = 0; i < dataset1.size(); i++){
        for (int j = 0; j < dataset2.size(); j++) {
            distanceMap(i, j) = calculateDistance(dataset1, dataset2, i, j);
        }
    }
    return distanceMap;
}

double calculateDistance (const std::vector<cv::Point2f> &dataset, int i, int j) {
    cv::Point2f difference = dataset[i] - dataset[j];
    return ((difference.x * difference.x) + (difference.y * difference.y));
}

double calculateDistance (const std::vector<cv::Point2f> &dataset1, const std::vector<cv::Point2f> &dataset2, int i, int j) {
    cv::Point2f difference = dataset1[i] - dataset2[j];
    return ((difference.x * difference.x) + (difference.y * difference.y));
}

std::vector<int> regionQuery(Eigen::MatrixXd distanceMap, double epsilon, int i) {
    std::vector<int> neighborIndexes;
    for (int j = 0; j < distanceMap.rows(); j++) {
        if (distanceMap(i, j) < epsilon) {
            neighborIndexes.push_back(j);
        }
    }
    return neighborIndexes;
}

}
