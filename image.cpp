#include "image.h"
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QPixmap>
#include <QMessageBox>
#include <cstdlib>

Image::Image(const std::string& image_directory)
{
    original_image = cv::imread(image_directory);

    if (original_image.empty()) {
        QMessageBox::information(nullptr, "Error", "An error occured during opening image");
        std::exit(EXIT_FAILURE);
    }

    // Necessary controls are made here
    // Images greated than 1024x1024 and less than 250x250 are not allowed
    if (!check_image()) {
        QMessageBox::critical(nullptr, "Error", "Resolution of image cannot be greater than 1024x1024 or less that 250x250");
        std::exit(EXIT_FAILURE);
    }

    // Selection of ROI is done here
    cv::Rect2d selected_roi = cv::selectROI(original_image);
    blurry_image = original_image(selected_roi);
    cv::destroyAllWindows();

    width = original_image.cols;
    height = original_image.rows;

    max_x_pos = 100;
    max_y_pos = 100;

    // Other necessary operations such as resizing cropped image and making it blurred
    cv::resize(blurry_image, blurry_image, cv::Size(original_image.cols, original_image.rows), 0, 0, cv::INTER_CUBIC);
    cv::blur(blurry_image, blurry_image, cv::Size(11, 11));

    image_adjustment(original_image.rows - 100, original_image.cols - 100, 0, 0, 1, 1, 1);
}

bool Image::check_image() const
{
    if ((original_image.rows < 250 || original_image.cols < 250) ||
            (original_image.rows > 1024 || original_image.cols > 1024)) {
        return false;
    }

    return true;
}

void Image::set_original_image(cv::Mat image)
{
    original_image = image.clone();
}

void Image::set_original_image(const std::string& image_directory)
{
    original_image = cv::imread(image_directory);
}

void Image::set_final_image(cv::Mat image)
{
    final_image = image.clone();
}

cv::Mat Image::get_original_image() const
{
    return original_image;
}

cv::Mat Image::get_final_image() const
{
    return final_image;
}

void Image::set_final_pixmap(const cv::Mat& image)
{
    cv::Mat temp = image.clone();
    cv::cvtColor(image, temp, cv::COLOR_BGR2RGB);
    final_pixmap = QPixmap::fromImage(QImage(temp.data,
                                             temp.cols,
                                             temp.rows,
                                             static_cast<int>(temp.step),
                                             QImage::Format_RGB888));
}

void Image::image_adjustment(int height, int width, int pos_x, int pos_y,
                             float red_proportion, float green_proportion, float blue_proportion)
{
    cv::Mat temp_blurry_image = blurry_image.clone();
    cv::Mat temp_original_image = original_image.clone();

    cv::resize(original_image, temp_original_image, cv::Size(width, height), 0, 0, cv::INTER_AREA);

    for (int i = 0; i < blurry_image.rows; ++i) {
        for (int j = 0; j < blurry_image.cols; ++j) {
            temp_blurry_image.at<cv::Vec3b>(i, j)[0] = static_cast<uchar>(cvRound(temp_blurry_image.at<cv::Vec3b>(i, j)[0] * blue_proportion));
            temp_blurry_image.at<cv::Vec3b>(i, j)[1] = static_cast<uchar>(cvRound(temp_blurry_image.at<cv::Vec3b>(i, j)[1] * green_proportion));
            temp_blurry_image.at<cv::Vec3b>(i, j)[2] = static_cast<uchar>(cvRound(temp_blurry_image.at<cv::Vec3b>(i, j)[2] * red_proportion));
        }
    }

    final_image = temp_blurry_image.clone();

    for (int i = pos_y; i < pos_y + temp_original_image.rows; ++i) {
        for (int j = pos_x; j < pos_x + temp_original_image.cols; ++j) {
            final_image.at<cv::Vec3b>(i, j)[0] = temp_original_image.at<cv::Vec3b>(i - pos_y, j - pos_x)[0];
            final_image.at<cv::Vec3b>(i, j)[1] = temp_original_image.at<cv::Vec3b>(i - pos_y, j - pos_x)[1];
            final_image.at<cv::Vec3b>(i, j)[2] = temp_original_image.at<cv::Vec3b>(i - pos_y, j - pos_x)[2];
        }
    }

    max_x_pos = this->width - width;
    max_y_pos = this->height - height;

    set_final_pixmap(final_image);
}

QPixmap Image::get_final_pixmap() const
{
    return final_pixmap;
}

int Image::get_width() const
{
    return width;
}

int Image::get_height() const
{
    return height;
}

int Image::get_max_x_pos() const
{
    return max_x_pos;
}

int Image::get_max_y_pos() const
{
    return max_y_pos;
}
