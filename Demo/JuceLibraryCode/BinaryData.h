/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   btn1_png;
    const int            btn1_pngSize = 9337;

    extern const char*   btn2_png;
    const int            btn2_pngSize = 9320;

    extern const char*   btn3_png;
    const int            btn3_pngSize = 9398;

    extern const char*   btn4_png;
    const int            btn4_pngSize = 9150;

    extern const char*   bulbGrid_png;
    const int            bulbGrid_pngSize = 7468;

    extern const char*   grillGrid_png;
    const int            grillGrid_pngSize = 6490;

    extern const char*   knob_png;
    const int            knob_pngSize = 33707;

    extern const char*   panel_png;
    const int            panel_pngSize = 86122;

    extern const char*   switchBtn_png;
    const int            switchBtn_pngSize = 3357;

    extern const char*   switchBtn2_png;
    const int            switchBtn2_pngSize = 3223;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 10;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
