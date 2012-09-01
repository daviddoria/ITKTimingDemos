/**
 * Demo: Compare the process of visiting a set of pixels in a region
 *       using GetPixel() on a container of indices, versus
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
typename TImage::PixelType SumPixelsManual(const TImage* const image, const itk::Index<2>& queryIndex,
                                           const std::vector<itk::Offset<TImage::ImageDimension> >& offsets)
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
template<typename TShapedIterator>
typename TShapedIterator::PixelType SumPixelsIterator(const itk::Index<2>& queryIndex,
                                             TShapedIterator& shapedNeighborhoodIterator)
{
  // Construct a 1x1 region (a single pixel)
  typename TShapedIterator::SizeType regionSize = {{1,1}};
  typename TShapedIterator::RegionType region(queryIndex, regionSize);

  shapedNeighborhoodIterator.SetRegion(region);

  // Iterate over every activiated offset in the ShapedNeighborhood applied to the 'region' (single pixel) constructed above

  typename TShapedIterator::ConstIterator pixelIterator = shapedNeighborhoodIterator.Begin();

  typename TShapedIterator::PixelType pixelSum = 0;

  while (!pixelIterator.IsAtEnd())
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

  itk::Index< Dimension > imageCorner = {{0, 0}};
  itk::Size< Dimension > imageSize = {{31, 31}}; // Both dimensions of the size must be odd for the logic in this code to work
  itk::ImageRegion< Dimension > imageRegion(imageCorner, imageSize);
  image->SetRegions(imageRegion);
  image->Allocate();
  image->FillBuffer(2);

  // This is the pixel we will repeatedly query
  itk::Index<2> queryIndex = {{imageSize[0]/2, imageSize[0]/2}};

  // Create a list of all of the offsets relative to the queryIndex
  std::vector<itk::Offset<2> > offsets;

  itk::ImageRegionConstIteratorWithIndex<ImageType> imageIterator(image, imageRegion);

  while(!imageIterator.IsAtEnd())
  {
    offsets.push_back(imageIterator.GetIndex() - queryIndex);

    ++imageIterator;
  }

  std::cout << "There are " << offsets.size() << " offsets." << std::endl;

  // Call these functions many times for timing stability
  const unsigned int numberOfRuns = 1e6;

  int totalSum = 0; // This variable is used so the compiler doesn't optimize away the loop

  /////////////////// Method 1 - manual //////////////////////
  std::cout << "Running manual function..." << std::endl;
  itk::TimeProbe manualTimeProbe;
  manualTimeProbe.Start();
  for( unsigned int manualIteration = 0; manualIteration < numberOfRuns; ++manualIteration )
  {
    ImageType::PixelType pixelSum = SumPixelsManual(image.GetPointer(), queryIndex, offsets);
    totalSum += pixelSum;
  }
  manualTimeProbe.Stop();
  std::cout << "SumPixelsManual time: " << manualTimeProbe.GetMean() << std::endl;
  std::cout << "totalSum " << totalSum << std::endl;

  /////////////////// Method 2 - shaped iterator //////////////////////
  itk::TimeProbe iteratorTimeProbe;

  // Construct a 1x1 region (a single pixel)
  ImageType::SizeType regionSize = {{1,1}};
  ImageType::RegionType region(queryIndex, regionSize);

  // Construct a region that will surround the 1x1 region (pixel) created above
  unsigned int patchRadius = image->GetLargestPossibleRegion().GetSize()[0]/2;
  ImageType::SizeType radius = {{patchRadius, patchRadius}};

  typedef itk::ConstShapedNeighborhoodIterator<ImageType> ShapedIteratorType;
  ShapedIteratorType shapedNeighborhoodIterator(radius, image, region);

  // Activate all of the offsets
  for(size_t i = 0; i < offsets.size(); ++i)
  {
    shapedNeighborhoodIterator.ActivateOffset(offsets[i]);
  }

  totalSum = 0;
  std::cout << "Running iterator function..." << std::endl;
  iteratorTimeProbe.Start();
  for( unsigned int iteratiorIterations = 0; iteratiorIterations < numberOfRuns; ++iteratiorIterations )
  {
    ImageType::PixelType pixelSum = SumPixelsIterator(queryIndex, shapedNeighborhoodIterator);
    totalSum += pixelSum;
  }
  iteratorTimeProbe.Stop();
  std::cout << "SumPixelsIterator time: " << iteratorTimeProbe.GetMean() << std::endl;
  std::cout << "totalSum " << totalSum << std::endl;

  std::cout << "(Iterator time)/(Manual time): " << iteratorTimeProbe.GetMean() / manualTimeProbe.GetMean() << std::endl;

  return 0;
}
