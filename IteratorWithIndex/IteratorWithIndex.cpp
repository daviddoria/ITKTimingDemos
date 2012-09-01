/**
 * Demo: Iterate over an image and do something with GetIndex() at each pixel.
 *       Do this with a ImageRegionConstIteratorWithIndex and a
 *       ImageRegionConstIterator to compare.
 *
 * Conclusion:
 * ImageRegionConstIteratorWithIndex is about 3x faster than ImageRegionConstIterator if you need to use GetIndex() at each pixel!
 */

// ITK
#include "itkImage.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"

// STL
#include <vector>

template <typename TImage>
int Iterator(const TImage* image)
{
  itk::ImageRegionConstIterator<TImage> imageIterator(image, image->GetLargestPossibleRegion());

  unsigned int counter = 0;
  while(!imageIterator.IsAtEnd())
  {
    counter += imageIterator.GetIndex()[0];

    ++imageIterator;
  }

  return counter;
}

template <typename TImage>
int IteratorWithIndex(const TImage* image)
{
  itk::ImageRegionConstIteratorWithIndex<TImage> imageIterator(image, image->GetLargestPossibleRegion());

  unsigned int counter = 0;
  while(!imageIterator.IsAtEnd())
  {
    counter += imageIterator.GetIndex()[0];

    ++imageIterator;
  }

  return counter;
}

int main(int, char* [] )
{
  typedef itk::Image<unsigned char, 2> ImageType;
  ImageType::Pointer image = ImageType::New();

  itk::Index<2> corner = {{0,0}};
  itk::Size<2> size = {{10,10}};
  itk::ImageRegion<2> region(corner,size);
  image->SetRegions(region);
  image->Allocate();

  unsigned int counter = 0; // To make sure the loop isn't optimized away
  for(unsigned int i = 0; i < 1e7; ++i)
  {
    counter += Iterator(image.GetPointer()); // about 7.2 seconds
//    counter += IteratorWithIndex(image.GetPointer()); // about 2.6 seconds
  }

  std::cout << "counter " << counter << std::endl; // To make sure the loop isn't optimized away
  return 0;
}
