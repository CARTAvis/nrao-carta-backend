//
// Re-write from the file: "casa/code/imageanalysis/ImageAnalysis/ImageMoments.h"
//
#ifndef CARTA_BACKEND_MOMENT_IMAGEMOMENTS_H_
#define CARTA_BACKEND_MOMENT_IMAGEMOMENTS_H_

#include <casacore/lattices/Lattices/MaskedLattice.h>
#include <imageanalysis/ImageAnalysis/ImageMoments.h>
#include <imageanalysis/ImageAnalysis/ImageMomentsProgress.h>

namespace carta {

template <class T>
class ImageMoments : public casa::ImageMoments<T> {
public:
    ImageMoments(const casacore::ImageInterface<T>& image, casacore::LogIO& os, casacore::Bool over_write_output = false,
        casacore::Bool show_progress = true);

    ~ImageMoments(){};

    // This is the function that does all the computational work.
    // The returned vector will hold PagedImages or TempImages (do_temp = true).
    // If do_temp is true, the out_file_name is not used. If you create PagedImages, out_file_name is the root name for the output files.
    // If you don't set this variable, the default state of the class is to set the output name root to the name of the input file.
    std::vector<std::shared_ptr<casacore::MaskedLattice<T>>> CreateMoments(
        casacore::Bool do_temp, const casacore::String& out_file_name, casacore::Bool remove_axis = true);

    // Stop the calculation
    void StopCalculation();

private:
    SPCIIT _image = SPCIIT(nullptr);
    casa::ImageMomentsProgressMonitor* _progress_monitor = nullptr;

    // Stop moment calculation
    volatile bool _stop;

    // Private in casa::ImageMoments, include here
    SPIIT SmoothImage();
    void WhatIsTheNoise(T& noise, const casacore::ImageInterface<T>& image);

    // Iterate through a cube image with the moments calculator. Re-write from the casacore::LatticeApply<T,U>::lineMultiApply() function
    void LineMultiApply(casacore::PtrBlock<casacore::MaskedLattice<T>*>& lattice_out, const casacore::MaskedLattice<T>& lattice_in,
        casacore::LineCollapser<T, T>& collapser, casacore::uInt collapse_axis, casacore::LatticeProgress* tell_progress = 0);

    // Get a suitable chunk shape for the iteration
    casacore::IPosition ChunkShape(casacore::uInt axis, const casacore::MaskedLattice<T>& lattice_in);

protected:
    using casa::MomentsBase<T>::os_p;
    using casa::MomentsBase<T>::showProgress_p;
    using casa::MomentsBase<T>::momentAxisDefault_p;
    using casa::MomentsBase<T>::peakSNR_p;
    using casa::MomentsBase<T>::stdDeviation_p;
    using casa::MomentsBase<T>::yMin_p;
    using casa::MomentsBase<T>::yMax_p;
    using casa::MomentsBase<T>::out_p;
    using casa::MomentsBase<T>::smoothOut_p;
    using casa::MomentsBase<T>::goodParameterStatus_p;
    using casa::MomentsBase<T>::doWindow_p;
    using casa::MomentsBase<T>::doFit_p;
    using casa::MomentsBase<T>::doSmooth_p;
    using casa::MomentsBase<T>::noInclude_p;
    using casa::MomentsBase<T>::noExclude_p;
    using casa::MomentsBase<T>::fixedYLimits_p;
    using casa::MomentsBase<T>::momentAxis_p;
    using casa::MomentsBase<T>::worldMomentAxis_p;
    using casa::MomentsBase<T>::kernelTypes_p;
    using casa::MomentsBase<T>::kernelWidths_p;
    using casa::MomentsBase<T>::moments_p;
    using casa::MomentsBase<T>::selectRange_p;
    using casa::MomentsBase<T>::smoothAxes_p;
    using casa::MomentsBase<T>::overWriteOutput_p;
    using casa::MomentsBase<T>::error_p;
    using casa::MomentsBase<T>::convertToVelocity_p;
    using casa::MomentsBase<T>::velocityType_p;
    using casa::MomentsBase<T>::_checkMethod;
};

} // namespace carta

#include "ImageMoments.tcc"

#endif // CARTA_BACKEND_MOMENT_IMAGEMOMENTS_H_
