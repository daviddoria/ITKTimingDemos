// ITK
#include "itkImage.h"
#include "itkConstNeighborhoodIterator.h"

// STL
#include <vector>

// All of these functions seem to take about the same time

template <typename TImage>
unsigned int GetLargestPossibleRegion(const TImage* const image)
{
  return image->GetLargestPossibleRegion().GetSize()[0];
}

template <typename TImage>
unsigned int GetBufferedRegion(const TImage* const image)
{
  return image->GetBufferedRegion().GetSize()[0];
}

unsigned int SavedRegion(const itk::ImageRegion<2>& region)
{
  return region.GetSize()[0];
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

  int counter = 0;
  for(unsigned int i = 0; i < 1e8; ++i)
  {
    counter += SavedRegion(region); // about 3 seconds
    // counter += GetBufferedRegion(image.GetPointer()); // about 3 seconds
    //counter += GetLargestPossibleRegion(image.GetPointer()); // about 3 - 3.5 seconds
  }

  std::cout << "counter " << counter << std::endl;
  return 0;
}
