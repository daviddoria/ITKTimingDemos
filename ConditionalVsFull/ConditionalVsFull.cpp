/**
 * Demo: Iterate over an image looking for a pixel with a particular value.
 *       Do this by using a conditional and returning immediately if the value is found,
 *       and compare to not using a conditional in the loop and simply &= each value test.
 *
 * Conclusion:
 * Using a conditional in the loop is marginally slower (< 10%) if the entire image is searched,
 * but allows for the possibility of a much earlier return, so it should be favored.
 */

// ITK
#include "itkImage.h"
#include "itkImageRegionConstIterator.h"

template <typename TImage>
bool HasValueConditional(const TImage* const image, const typename TImage::PixelType& value)
{
  itk::ImageRegionConstIterator<TImage> iterator(image, image->GetLargestPossibleRegion());

  while(!iterator.IsAtEnd())
  {
    if(iterator.Get() == value)
    {
      return true;
    }

    ++iterator;
  }
  return false;
}

template <typename TImage>
bool HasValue(const TImage* const image, const typename TImage::PixelType& value)
{
  itk::ImageRegionConstIterator<TImage> iterator(image, image->GetLargestPossibleRegion());

  bool hasValue = true;

  while(!iterator.IsAtEnd())
  {
    hasValue &= (iterator.Get() == value);

    ++iterator;
  }
  return hasValue;
}

int main(int, char* [] )
{
  typedef itk::Image<unsigned char, 2> ImageType;
  ImageType::Pointer image = ImageType::New();

  itk::Index<2> corner={{0,0}};
  itk::Size<2> size = {{10,10}};
  itk::ImageRegion<2> region(corner,size);
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  unsigned char searchValue = 255; // This value does not appear in the image, so both functions
  // will have to search the entire image.

  int counter = 0;
  for(unsigned int i = 0; i < 1e7; ++i)
  {
//    counter += HasValue(image.GetPointer(), searchValue); // About 3 seconds
    counter += HasValueConditional(image.GetPointer(), searchValue); // About 3.3 seconds
  }

  std::cout << "counter " << counter << std::endl;
  return 0;
}
