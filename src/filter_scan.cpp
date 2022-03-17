#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <ros/ros.h>

class FilterScan
{
public:
    FilterScan()
    {
        pub_ = nh_.advertise<sensor_msgs::LaserScan>("/pico_scan_filtered", 1);
        sub_ = nh_.subscribe("/pico_scan", 1, &FilterScan::laserCallback, this);
    }
    ~FilterScan() {}
    void laserCallback(const sensor_msgs::LaserScanConstPtr lms)
    {
        sensor_msgs::LaserScan fls;
        fls.angle_min = lms->angle_min;
        fls.angle_max = lms->angle_max;
        fls.range_min = lms->range_min;
        fls.range_max = lms->range_max;
        fls.scan_time = lms->scan_time;
        fls.header.stamp = lms->header.stamp;
        fls.header.frame_id = lms->header.frame_id;
        fls.time_increment = lms->time_increment;
        int nr = 0;
        int num_readings = lms->ranges.size();
        // for (int i = 0; i < lms->ranges.size(); i++)
        // {
        //     if (lms->ranges[i] != lms->ranges[i])
        //     {
        //         printf("found NAN\n");
        //     }
        //     else
        //     {
        //         num_readings++;
        //         printf("%f\n", lms->ranges[i]);
        //         // fls.ranges[nr] = lms->ranges[i];
        //         // nr++;
        //     }
        // }
        fls.ranges.resize(num_readings);
        float last = 0.0;
        float next = 0.0;
        for (int i = 0; i < lms->ranges.size(); i++)
        {
            if (lms->ranges[i] != lms->ranges[i])
            {
                printf("found NAN\n");
                int j = i + 1;
                while (lms->ranges[j] != lms->ranges[j])
                {
                    j++;
                }
                if (lms->ranges.size() <= j)
                    next = 0.0;
                else
                    next = lms->ranges[j];
                fls.ranges[nr] = (last + next) / 2;
            }
            else
            {
                // num_readings++;
                // printf("%f\n", lms->ranges[i]);
                last = lms->ranges[i];
                fls.ranges[nr] = lms->ranges[i];
                nr++;
            }
        }
        fls.angle_increment = (fls.angle_max - fls.angle_min) / nr;
        pub_.publish(fls);
    }

private:
    ros::Subscriber sub_;
    ros::Publisher pub_;
    ros::NodeHandle nh_;
};
int main(int argc, char **argv)
{
    ros::init(argc, argv, "filter_scan");

    FilterScan fs;

    ros::spin();
}