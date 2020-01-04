#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include "TrafficObject.h"

class Graphics
{
public:
    // constructor / desctructor

    // getters / setters
    void set_bg_file_name(std::string filename) { bg_filename_ = filename; }
    void set_traffic_objects(
        std::vector<std::shared_ptr<TrafficObject>> &traffic_objects) { traffic_objects_ = traffic_objects; };

    // typical behaviour methods
    void simulate();

private:
    // typical behaviour methods
    void load_background_img();
    void draw_traffic_objects();

    // member variables
    std::vector<std::shared_ptr<TrafficObject>> traffic_objects_;
    std::string bg_filename_;
    std::string window_name_;
    std::vector<cv::Mat> images_;
};

#endif
