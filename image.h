#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/core.hpp>
#include <string>
#include <QPixmap>

class Image
{
public:
    Image() = default;
    Image(const std::string& image_directory);

    // Check image if it has allowed resolution values
    bool check_image() const;

    void set_original_image(cv::Mat image);
    void set_original_image(const std::string& image_directory);

    cv::Mat get_original_image() const;
    cv::Mat get_final_image() const;
    QPixmap get_final_pixmap() const;

    int get_width() const;
    int get_height() const;

    int get_max_x_pos() const;
    int get_max_y_pos() const;

    // Necessary image operations are done here
    void image_adjustment(int height, int width, int pos_x, int pos_y,
                          float red_proportion, float green_proportion, float blue_proportion);
private:
    // Width and height of selected image
    int width;
    int height;

    // This part is used to set maximum x and y positions to place inner image
    //  during execution of program
    // Formula is minimum = 0, 0 maximum = resolution of image - resolution of inner image
    //  for x and y separately
    int max_x_pos;
    int max_y_pos;

    // Image itself
    cv::Mat original_image;

    // Cropped blurry and resized image
    cv::Mat blurry_image;

    // Image created after certain operations;
    cv::Mat final_image;
    QPixmap final_pixmap;

    void set_final_image(cv::Mat image);
    void set_final_pixmap(const cv::Mat& image);
};

#endif // IMAGE_H
