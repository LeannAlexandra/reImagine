#include "reImagineLibrary.h"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>


reImagineLibrary::reImagineLibrary (){
    //LOAD IMAGES
}
reImagineLibrary::~reImagineLibrary (){}
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
cv::Mat reImagineLibrary::applyImageTexture(const cv::Mat& inputImage, const cv::Mat& textureImage,  float valMargin,
                          float textureMidpoint, float blendBalance){
    cv::Mat img=inputImage.clone();//preserve orignal input image.
    //cv::Mat texture=matchSize(textureImage, img.cols, img.rows);
    //tempTexture.convertTo(texture, CV_32F, 1.0 / 255.0);

    //cv::Mat normalizedTexture = normalize(texture);

    // cv::Mat originalColor = img.clone();
    cv::Mat
        ogColorHSV; // preserve the orginal image's colors to reintroduce later.
    cvtColor(img, ogColorHSV, cv::COLOR_BGR2HSV);
    // this is only used in  v2*

    if (blendBalance >= 0.01) { // else skipp
        // added step: blend the normalized image with the input, then do the next
        // step:
        cv::Mat matchingNormTexture;
        // matchingNormTexture=normalizedTexture.clone();
        cv::cvtColor(textureImage, matchingNormTexture, cv::COLOR_GRAY2BGR);
        cv::addWeighted(img, 1.0 - blendBalance, matchingNormTexture, blendBalance,
                        0.0, img);
    }
    // make the HSV image after the blend, to include newly added darkness
    // information.
    cv::Mat hsvImage;
    cvtColor(img, hsvImage, cv::COLOR_BGR2HSV);
    cv::Mat outputImage = img.clone();
    // display(img, "debug");
    // cv::waitKey(0);

    if (!(valMargin <= 0.01 &&
          textureMidpoint <= 0.01)) { // otherwise it is just a lengthy "copy img
        // to output image"

        for (int y = 0; y < textureImage.rows; y++) {
            for (int x = 0; x < textureImage.cols; x++) {
                // Retrieve the normalized texture intensity [0, 1]
                float texIntensityNormalized =
                    static_cast<float>(textureImage.at<uchar>(y, x)) / 255.0;

                // Calculate the brightness adjustment based on texture intensity
                float brightnessChange = (texIntensityNormalized - textureMidpoint) *
                                         valMargin; // Calculate change from midpoint

                // Apply brightness adjustment to the corresponding pixel in the image
                cv::Vec3b &hsvPixel = hsvImage.at<cv::Vec3b>(y, x);

                // takes the amount of change from the midpoint and adds 70% of that (in
                // essence 35% of the deviation from mid)
                hsvPixel[2] = cv::saturate_cast<uchar>(
                    hsvPixel[2] + brightnessChange * 255.0f); // Modify value component
                hsvPixel[1] = cv::saturate_cast<uchar>(
                    hsvPixel[1] + ((brightnessChange * 255.0f))); // Modify saturation component by quarter value component
                //
                /**
          Awesome thing to not is that when something gains a lot of "value" we
        want to have an upper cut off that actually translates "extra" value
        added to desaturation.

        **/

                // recolor the image HUE from the regular one (only if we didn't skip the
                // blend)
                if (blendBalance >= 0.01) {
                    cv::Vec3b &ogColor = ogColorHSV.at<cv::Vec3b>(y, x);
                    //   // take the hue value directly from the original image. and
                    //   // blend the saturation to the average between original and
                    hsvPixel[0] = ogColor[0];
                    hsvPixel[1] = (ogColor[1]+ hsvPixel[1])/2;//average out the saturation.


                    /// THIS IS AN "EXTRA STEP" THIS WILL BE NECESSARY WHEN THE DEFAULT VALUES CHANGE, TO COMBAT OVER-SATURATION.
                    //   // if the value is above a certain point, we substract the
                    //   difference
                    //   // from saturation. arbitrary point chosen 70%
                    //   float arbitrary_crossover_point = 0.7;
                    //   if (hsvPixel[2] - (arbitrary_crossover_point*255) > 0) { //
                    //   desaturate up to hard limit of 0
                    //     float newVal = hsvPixel[1] - (hsvPixel[2] -
                    //     (arbitrary_crossover_point*255)); hsvPixel[1] = (newVal > 0) ?
                    //     newVal : 0.0; hsvPixel[1] = (hsvPixel[1]>255)?255: hsvPixel[1];
                    //     //std:: cout<< "hsv2: "<<hsvPixel[2]<< std::endl;
                    //     hsvPixel[2] = (arbitrary_crossover_point*255);
                    //   }
                }
            }
        }
        // Convert HSV image back to BGR color space
        cv::cvtColor(hsvImage, outputImage, cv::COLOR_HSV2BGR);
    }
    /// EXTRA STEP TO RE-BLEND THE ORIGINAL IMAGE, SPECIFICALLY HELPFUL TO GIVE A KIND OF "FLATTENIN" LIKE QUALITY
    //re-add oG image.
    cv::addWeighted(outputImage,0.6,img, 0.4,0.0,outputImage);
    return outputImage.clone();

}

/**
 * @brief Converts a cv::Mat object to a QImage object.
 *
 * This function takes a cv::Mat object as input and converts it into a QImage object.
 *
 * @param mat Input cv::Mat object to be converted.
 * @return Converted QImage object.
 */
QImage reImagineLibrary::mat2Q(cv::Mat &mat){
    // Convert OpenCV Mat image to QImage
    QImage img;
    if (mat.channels() == 1) {
        // Grayscale image
        img = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
    } else if (mat.channels() == 3) {
        // RGB/BGR color image
        cv::Mat temp;
        cv::cvtColor(mat, temp, cv::COLOR_BGR2RGB); // OpenCV uses BGR order
        img = QImage(temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);

    }
    return img;
}
/**
 * @brief Normalize a grayscale image to the full range between black and white (0-255).
 *
 * This function takes a grayscale cv::Mat object as input and linearly scales its pixel values
 * to the full spectrum range (0-255).
 *
 * @param mat Input grayscale cv::Mat object to be normalized.
 * @return Normalized cv::Mat object with pixel values scaled to the range [0, 255].
 */
cv::Mat reImagineLibrary::normalize(cv::Mat &texture){    // Normalize the texture image to the range [0, 255]
    double minVal, maxVal;
    minMaxLoc(texture, &minVal, &maxVal);
    // Find min and max intensity values in the texture image
    cv::Mat normalizedTexture;
    texture.convertTo(normalizedTexture, CV_8U, 255.0 / (maxVal - minVal),
                      -minVal * 255.0 / (maxVal - minVal));
    return normalizedTexture;}
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
cv::Mat reImagineLibrary::matchSize(cv::Mat &texture, int width,int height) {
    // test input image size, extrapolate the bigger differencce, to maintain
    // aspect ratio.
    float x_ratio = 0.0;
    float y_ratio = 0.0;

    x_ratio = texture.cols / width;
    y_ratio = texture.rows / height;

    if (x_ratio > 1 && y_ratio > 1) {
        // // // Crop the texture to match the input image's size.

        //simple matching
        //cv::Rect matchRoi(0, 0, size.width, size.height);

        //centered matching
        cv::Rect matchRoi( (texture.cols - width) /2, (texture.rows - height)/2, width,height );
        cv::Mat tempTexture = texture(matchRoi);
        return tempTexture;
            // return ;
    }
    float smaller = std::min(x_ratio, y_ratio);
    float scaling = 1 / smaller;

    cv::Mat resized_image;
    cv::resize(texture, resized_image, cv::Size(), scaling, scaling);

    // find the center of the new image -
    int center_X = resized_image.cols / 2;
    int center_y = resized_image.rows / 2;

    cv::Rect matchRoi(
        center_X - width / 2,
        center_y - height / 2,
        width,height); // match the size in the center of the newly scaled image

    cv::Mat sizeCorrectedTexture = resized_image(matchRoi);

    return sizeCorrectedTexture;
}
