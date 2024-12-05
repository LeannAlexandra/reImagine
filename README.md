reImagine

reImagine is a QT-based extension of my previous project, "imageTexturing." Both projects focus on understanding and implementing image blending and conversion techniques using OpenCV. This project introduces a reactive Qt window that allows resizing and image saving through the menu bar, offering a sleek and simple design coupled with an effective algorithm for adding texture to input images.

## Table of Contents
- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Setup](#setup)
- [Usage](#usage)
- [Functionality](#functionality)
- [Contributing](#contributing)
- [License](#license)

 ## Overview
reImagine is an iteration on the original ["imageTexturing" project](https://github.com/LeannAlexandra/imageTexturing), now featuring a dynamic Qt interface. The goal remains to apply textures to input images using OpenCV, but with improved usability and design in a minimum viable product (MVP) version 1.0.
## MVP 1.0
A sleek stylish interface, that uniformly scrolls across all three images, should you find artefacts or inspect certain regions before saving. This window is sized dynamically and will ensure a great indication of tweeks in later iterations.
<img src="https://github.com/LeannAlexandra/reImagine/blob/main/reImagine_screenshot.png?raw=true" alt="Input Image" style="max-width: 100%; max-height: 200px; margin-bottom: 10px;">


## Image Texturing
Image Texturing explores several approaches on how texture can be transformed on a pixel level, including using different approached. This approached used here is a combination of blending the two images, as well as taking the HSV (Value/ Darkness) into account to add more dramatic shaddow from the texture image. - note: most ineffective and low-yielding methods are removed for simplicity. 
<table align="center">
  <tr>
    <td align="center">
      <img src="https://github.com/LeannAlexandra/imageTexturing/blob/main/inputImage.jpg?raw=true" alt="Input Image" style="max-width: 100%; max-height: 200px; margin-bottom: 10px;">
      <br>
      <figcaption>Input Image: Original image used for texture mapping. - <a href="https://unsplash.com/@lucabravo">Luca Bravo</a> @<a href="https://unsplash.com/photos/brown-house-near-body-of-water-zAjdgNXsMeg">Unsplash</a></figcaption>
    </td>
    <td align="center">
      <img src="https://github.com/LeannAlexandra/imageTexturing/blob/main/canvas.jpg?raw=true" alt="Texture" style="max-width: 100%; max-height: 200px; margin-bottom: 10px;">
      <br>
      <figcaption>Texture: Image used as a texture overlay. -<a href="https://unsplash.com/@anniespratt">Annie Sprat</a> @<a href="https://unsplash.com/photos/white-textile-with-black-line-xz485Eku8O4">Unsplash</a></figcaption>
    </td>
    <td align="center">
      <img src="https://github.com/LeannAlexandra/imageTexturing/blob/main/exampleoutput.jpg?raw=true" alt="Final Image" style="max-width: 100%; max-height: 200px; margin-bottom: 10px;">
      <br>
      <figcaption>Final Image: Resulting image with applied texture.</figcaption>
    </td>
  </tr>
</table>


## Prerequisites

Before running the program, ensure you have the following installed:
  - C++ compiler
  - OpenCV (version 4.x)
  - Qt (version 5.x or 6.x)
  - Q build system (QBS)

## Setup
Clone the Repository with submodules:
```bash
  git clone --recurse-submodules https://github.com/LeannAlexandra/reImagine.git
  cd reImagine
```
Optional: update submodules
```bash
  git submodule update --init --recursive
```
Compile the Program:
```bash
  qmake reImagine.qbs
  make
```
## Usage
Run the Program:
```bash
    ./reImagine
```
Expected Output:
        The program will display the input image, output image and the image with the applied texture in a Qt window.
        You can resize the window and save the output image from the menu bar.
        You can grab and pan all three uniformly to always see the exact region of interest.

## Functionality
The program reads an input image (inputImage.jpg) and a texture image (canvas.jpg) from the project directory. It overlays the texture onto the input image, adjusts brightness based on texture intensity, and displays the resulting image. Users can save the output image directly from the Qt window.

## Contributing
Contributions are welcome! If you have ideas for improvements or encounter any issues, please open an issue or submit a pull request.

## License
This project is licensed under the MIT License.
