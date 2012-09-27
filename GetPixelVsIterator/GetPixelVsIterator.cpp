/**
 * Demo: Iterate over an image and do something with Get() at each pixel.
 *       Compare this to calling GetPixel() on a vector of known indices.
 *
 * Conclusion:
 * Using the iterator is about 3-4x faster.
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
    counter += imageIterator.Get();

    ++imageIterator;
  }

  return counter;
}

template <typename TImage>
int GetPixel(const TImage* image, const std::vector<itk::Index<2> >& indices)
{
  unsigned int counter = 0;
  for(unsigned int i = 0; i < indices.size(); ++i)
  {
    counter += image->GetPixel(indices[i]);
  }

  return counter;
}

template <typename TImage>
void CreateImage(TImage* const image)
{
  itk::Index<2> corner = {{0,0}};
  itk::Size<2> size = {{100,100}};
  itk::ImageRegion<2> region(corner,size);
  image->SetRegions(region);
  image->Allocate();
}

int main(int, char* [] )
{
  typedef itk::Image<unsigned char, 2> ImageType;
  ImageType::Pointer image = ImageType::New();
  CreateImage(image.GetPointer());

  const unsigned int numberOfIterations = 1e5;

  // Create a list of the indices in the image
  itk::ImageRegionConstIteratorWithIndex<ImageType> imageIterator(image, image->GetLargestPossibleRegion());
  std::vector<itk::Index<2> > indices;

  while(!imageIterator.IsAtEnd())
  {
    indices.push_back(imageIterator.GetIndex());
    ++imageIterator;
  }

  unsigned int total = 0; // To make sure the loop isn't optimized away
  for(unsigned int i = 0; i < numberOfIterations; ++i)
  {
//    total += Iterator(image.GetPointer()); // 1.4s
    total += GetPixel(image.GetPointer(), indices); // 5.9s
  }

  std::cout << "total " << total << std::endl; // To make sure the loop isn't optimized away

  return 0;
}
