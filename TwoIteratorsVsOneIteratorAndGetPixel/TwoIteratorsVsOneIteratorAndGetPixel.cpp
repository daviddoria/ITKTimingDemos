// ITK
#include "itkImage.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"

// STL
#include <vector>

////////////// Method 1 /////////////////////////
template <typename TImage>
int TwoIterators(const TImage* image)
{
  itk::ImageRegionConstIterator<TImage> imageIterator1(image, image->GetLargestPossibleRegion());
  itk::ImageRegionConstIterator<TImage> imageIterator2(image, image->GetLargestPossibleRegion());

  unsigned int counter = 0;
  while(!imageIterator1.IsAtEnd())
  {
    if(imageIterator1.Get() == imageIterator2.Get())
    {
      counter++;
    }

    ++imageIterator1;
    ++imageIterator2;
  }

  return counter;
}

///////////////////////////////////////////// Method 2 //////////////////////
template <typename TImage>
int OneIteratorAndGetPixel(const TImage* image)
{
  itk::ImageRegionConstIteratorWithIndex<TImage> imageIterator(image, image->GetLargestPossibleRegion());

  unsigned int counter = 0;
  while(!imageIterator.IsAtEnd())
  {
    if(imageIterator.Get() == image->GetPixel(imageIterator.GetIndex()))
    {
      counter++;
    }

    ++imageIterator;
  }

  return counter;
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

  unsigned int counter = 0; // To make sure the loop isn't optimized away
  for(unsigned int i = 0; i < 1e6; ++i)
  {
    //counter += TwoIterators(image.GetPointer()); // about 7.6 seconds
    counter += OneIteratorAndGetPixel(image.GetPointer()); // about 9.5 seconds
  }

  std::cout << "counter " << counter << std::endl; // To make sure the loop isn't optimized away
  return 0;
}
