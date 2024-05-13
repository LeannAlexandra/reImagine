/**
 * @file reimagine_library.h
 * @brief Declares the ReImagineLibrary class.
 */

#ifndef REIMAGINELIBRARY_H
#define REIMAGINELIBRARY_H


#include <opencv2/core.hpp>
#include <QImage>

/**
 * @class ReImagineLibrary
 * @brief Provides image processing functionalities using OpenCV.
 */

class reImagineLibrary{
public:
    /**
     * @brief Constructs a ReImagineLibrary object.
     */
    reImagineLibrary();
    /**
     * @brief Destructs the ReImagineLibrary object.
     */
    ~reImagineLibrary();
    /**
 * @brief Applies a texture to an input image while preserving color and clarity.
 * @param inputImage Input image to apply the texture to (3-channel color image in BGR format).
 * @param textureImage Texture image used for blending (single-channel grayscale image).
 * @param valMargin Magnitude of change the texture has on the pixel value (default: 0.3).
 * @param textureMidpoint Normalized midpoint of the texture (0.0 - 1.0) determining shading and lighting effects.
 * @param blendBalance Normalized influence of the direct blending of the two images (0.0 - 1.0).
 * @return Processed image result with applied texture.
 *
 * This function blends a texture image onto an input image while preserving color and clarity.
 * The texture is applied based on a specified midpoint, where pixels lighter than this point increase brightness,
 * and darker pixels darken the input image. The algorithm operates in the HSV color space to manipulate brightness (V value)
 * and consider saturation for color preservation during blending.
 * The blendBalance parameter controls the influence of direct blending between the input and texture images.
 */
    cv::Mat applyImageTexture(const cv::Mat& inputImage, const cv::Mat& textureImage,  float valMargin = 0.7,
                              float textureMidpoint = 0.96, float blendBalance = 0.2);
    /**
 * @brief Converts a cv::Mat object to a QImage object.
 *
 * This function takes a cv::Mat object as input and converts it into a QImage object.
 *
 * @param mat Input cv::Mat object to be converted.
 * @return Converted QImage object.
 */
    QImage mat2Q(cv::Mat &mat);
    /**
 * @brief Normalize a grayscale image to the full range between black and white (0-255).
 *
 * This function takes a grayscale cv::Mat object as input and linearly scales its pixel values
 * to the full spectrum range (0-255).
 *
 * @param mat Input grayscale cv::Mat object to be normalized.
 * @return Normalized cv::Mat object with pixel values scaled to the range [0, 255].
 */
    cv::Mat normalize(cv::Mat &texture);
    /**
 * @brief Resize a cv::Mat object to a specified size while preserving the aspect ratio.
 *
 * This function takes a cv::Mat object as input and resizes it to the specified dimensions,
 * preserving the aspect ratio by centering and cropping if necessary.
 *
 * @param texture Input cv::Mat object to be resized.
 * @param width Desired width in pixels.
 * @param height Desired height in pixels.
 * @return Resized cv::Mat object of the specified size.
 */
    cv::Mat matchSize(cv::Mat &texture, int width,int height);

};
#endif // REIMAGINELIBRARY_H
