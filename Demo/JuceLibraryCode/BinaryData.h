/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   bulbGrid_png;
    const int            bulbGrid_pngSize = 7468;

    extern const char*   grillGrid_png;
    const int            grillGrid_pngSize = 6490;

    extern const char*   knob_png;
    const int            knob_pngSize = 33707;

    extern const char*   switchBtn_png;
    const int            switchBtn_pngSize = 3357;

    extern const char*   switchBtn2_png;
    const int            switchBtn2_pngSize = 3223;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 5;

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
