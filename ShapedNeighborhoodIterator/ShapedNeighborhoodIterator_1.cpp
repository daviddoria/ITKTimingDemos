/**
 * Demo: Compare the process of visiting a set of pixels in a region using GetPixel() on a container of indices, versus
 *       versus using a ShapedNeighborhoodIterator.
 *
 * Conclusion:
 *
 */

// ITK
#include "itkImage.h"
#include "itkConstShapedNeighborhoodIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkTimeProbe.h"

// STL
#include <vector>

///////////////////////////////////////////// Method 1 //////////////////////
template<typename TImage>
typename TImage::PixelType SumPixelsManual(const TImage* const image, const itk::Index<2>& queryIndex, const std::vector<itk::Offset<TImage::ImageDimension> >& offsets)
{
  // Sum the pixels at 'offsets' relative to 'index'
  typename TImage::PixelType pixelSum = 0;

  for( size_t ii = 0; ii < offsets.size(); ++ii )
    {
    pixelSum += image->GetPixel(queryIndex + offsets[ii]);
    }

  return pixelSum;
}

///////////////////////////////////////////// Method 2 //////////////////////
template<typename TImage>
typename TImage::PixelType SumPixelsIterator(const TImage* const image, const itk::Index<2>& queryIndex,
                                             const std::vector<itk::Offset<2> >& offsets)
{
  // Sum the pixels at 'offsets' relative to 'index'

  // Construct a 1x1 region (a single pixel)
  typename TImage::SizeType regionSize = {{1,1}};
  typename TImage::RegionType region(queryIndex, regionSize);

  // Construct a region that will surround the 1x1 region (pixel) created above
  unsigned int patchRadius = image->GetLargestPossibleRegion().GetSize()[0]/2;
  typename TImage::SizeType radius = {{patchRadius, patchRadius}};

  typedef itk::ConstShapedNeighborhoodIterator<TImage> ShapedIteratorType;
  ShapedIteratorType shapedNeighborhoodIterator(radius, image, region);

  // Activate all of the offsets
  for(size_t i = 0; i < offsets.size(); ++i)
  {
    shapedNeighborhoodIterator.ActivateOffset(offsets[i]);
  }

  // Iterate over every activiated offset in the ShapedNeighborhood applied to the 'region' (single pixel) constructed above
  typename ShapedIteratorType::ConstIterator pixelIterator = shapedNeighborhoodIterator.Begin();

  typename TImage::PixelType pixelSum = 0;

  while (! pixelIterator.IsAtEnd())
  {
    pixelSum += pixelIterator.Get();
    ++pixelIterator;
  }

  return pixelSum;
}

int main(int, char* [] )
{
  typedef unsigned char PixelType;
  const static unsigned int Dimension = 2;
  typedef itk::Image< PixelType, Dimension > ImageType;
  ImageType::Pointer image = ImageType::New();

  itk::Index< Dimension > corner = {{0, 0}};
  itk::Size< Dimension > size = {{31, 31}}; // Both dimensions of the size must be odd for the logic in this code to work
  itk::ImageRegion< Dimension > region(corner, size);
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  // This is the pixel we will repeatedly query
  itk::Index<2> queryIndex = {{size[0]/2, size[0]/2}};

  // Create a list of all of the offsets relative to the queryIndex
  std::vector<itk::Offset<2> > offsets;

  itk::ImageRegionConstIteratorWithIndex<ImageType> imageIterator(image, region);

  while(!imageIterator.IsAtEnd())
  {
    offsets.push_back(imageIterator.GetIndex() - queryIndex);

    ++imageIterator;
  }

  // Call these functions many times for timing stability
  const unsigned int numberOfRuns = 1000;

  int totalSum = 0; // This variable is used so the compiler doesn't optimize away the loop

  itk::TimeProbe manualTimeProbe;
  for( unsigned int ii = 0; ii < numberOfRuns; ++ii )
  {
    manualTimeProbe.Start();
    ImageType::PixelType pixelSum = SumPixelsManual(image.GetPointer(), queryIndex, offsets);
    manualTimeProbe.Stop();

    totalSum += pixelSum;
  }
  std::cout << "SumPixelsManual: " << manualTimeProbe.GetMean() << std::endl;
  std::cout << "totalSum " << totalSum << std::endl;

  itk::TimeProbe iteratorTimeProbe;
  for( unsigned int ii = 0; ii < numberOfRuns; ++ii )
  {
    iteratorTimeProbe.Start();
    ImageType::PixelType pixelSum = SumPixelsIterator(image.GetPointer(), queryIndex, offsets);
    iteratorTimeProbe.Stop();

    totalSum += pixelSum;
  }
  std::cout << "SumPixelsIterator: " << iteratorTimeProbe.GetMean() << std::endl;
  std::cout << "totalSum " << totalSum << std::endl;

  std::cout << "Iterator/Manual: " << iteratorTimeProbe.GetMean() / manualTimeProbe.GetMean() << std::endl;

  return 0;
}
