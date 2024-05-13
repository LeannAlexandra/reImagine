import qbs

QtApplication {
    Depends { name: "Qt.widgets" }

    cpp.cxxLanguageVersion: "c++11" // Specify C++ language version (optional)

    // Specify the OpenCV module and include directories
    cpp.includePaths: ["/usr/include/opencv4"] // Path to your OpenCV headers

    cpp.dynamicLibraries: ["opencv_core", "opencv_highgui", "opencv_imgproc", "opencv_imgcodecs"]

    // Specify source files for your project
    files: [
        "main.cpp",
        "mainwindow.cpp",
        "mainwindow.h",
        "mainwindow.ui",
        "ReImagine_en_ZA.ts",
        "reImagineLibrary.cpp",
        "reImagineLibrary.h",
    ]

    // Optional: Add resource files (e.g., translation files)
    Group {
        fileTagsFilter: "qm"
        qbs.install: true
        qbs.installDir: qbs.targetOS.contains("qnx") ? qbs.FileInfo.joinPaths("/tmp", name, "bin") : qbs.base
        qbs.fileTags: "qt.core.resource_data"
        qbs.Qt.core.resourcePrefix: "/i18n"
    }
}
