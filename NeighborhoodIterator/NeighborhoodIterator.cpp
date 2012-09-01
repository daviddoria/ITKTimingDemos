/**
 * Demo: Compare the process of visiting each neighbor of a pixel by computing its neighbors and using GetPixel()
 *       versus using a NeighborhoodIterator and skipping the center pixel.
 *
 * Conclusion:
 * It is about 2x as fast to use a NeighborhoodIterator.
 */

// ITK
#include "itkImage.h"
#include "itkConstNeighborhoodIterator.h"

// STL
#include <vector>

////////////// Method 1 /////////////////////////
std::vector<itk::Offset<2> > Get8NeighborOffsets()
{
  std::vector<itk::Offset<2> > offsets;

  for(int i = -1; i <= 1; ++i)
    {
    for(int j = -1; j <= 1; ++j)
      {
      if(i == 0 && j == 0)
        {
        continue;
        }
      itk::Offset<2> offset;
      offset[0] = i;
      offset[1] = j;
      offsets.push_back(offset);
      }
    }
  return offsets;
}

std::vector<itk::Index<2> > Get8Neighbors(const itk::Index<2>& pixel)
{
  std::vector<itk::Index<2> > neighborsInRegion;

  std::vector<itk::Offset<2> > neighborOffsets = Get8NeighborOffsets();
  for(unsigned int i = 0; i < neighborOffsets.size(); ++i)
    {
    itk::Index<2> index = pixel + neighborOffsets[i];
    neighborsInRegion.push_back(index);
    }
  return neighborsInRegion;
}

template<typename TImage>
std::vector<itk::Index<2> > Get8NeighborsWithValue(const itk::Index<2>& pixel, const TImage* const image,
                                                   const typename TImage::PixelType& value)
{
  std::vector<itk::Index<2> > neighbors = Get8Neighbors(pixel);
  std::vector<itk::Index<2> > neighborsWithValue;
  for(unsigned int i = 0; i < neighbors.size(); ++i)
    {
    if(image->GetPixel(neighbors[i]) == value)
      {
      neighborsWithValue.push_back(neighbors[i]);
      }
    }
  return neighborsWithValue;
}

///////////////////////////////////////////// Method 2 //////////////////////
// (Faster and more concise)
template<typename TImage>
std::vector<itk::Index<2> > Get8NeighborsWithValueFast(const itk::Index<2>& pixel, const TImage* const image,
                                                       const typename TImage::PixelType& value)
{
  std::vector<itk::Index<2> > neighborsWithValue;

  // Construct a 1x1 region (a single pixel)
  typename TImage::SizeType regionSize = {{1,1}};

  typename TImage::RegionType region(pixel, regionSize);

  // Construct a 1x1 radius (to make a 3x3 patch, or the 8-neighborhood)
  typename TImage::SizeType radius = {{1,1}};

  itk::ConstNeighborhoodIterator<TImage> neighborhoodIterator(radius, image, region);

  typename TImage::SizeType neighborhoodSize = neighborhoodIterator.GetSize();

  unsigned int numberOfPixelsInNeighborhood = neighborhoodSize[0] * neighborhoodSize[1];
//  std::cout << "numberOfPixelsInNeighborhood: " << numberOfPixelsInNeighborhood << std::endl; // This outputs '9'

  // Do not need to wrap this in a while(!iterator.IsAtEnd()) because we only want to visit the single pixel in 'region'
  for(unsigned int i = 0; i < numberOfPixelsInNeighborhood; i++)
  {
    if(i != neighborhoodIterator.GetCenterNeighborhoodIndex()) // Skip the center, as it is not a neighbor, but the query pixel itself
    {
      bool inBounds = false;
      if(neighborhoodIterator.GetPixel(i, inBounds) == value)
      {
        if(inBounds)
        {
          neighborsWithValue.push_back(neighborhoodIterator.GetIndex(i));
        }
      }
    }
  }

  return neighborsWithValue;
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

  // This is the pixel we will repeatedly query
  itk::Index<2> center = {{5,5}};

  unsigned char searchValue = 255;

  // This pixel (an 8-neighbor of center) we set to the search value
  itk::Index<2> otherPixel = {{4,4}};
  image->SetPixel(otherPixel, searchValue);

  int totalSize = 0;
  for(unsigned int i = 0; i < 1e6; ++i)
  {
    std::vector<itk::Index<2> > neighbors = Get8NeighborsWithValue(center, image.GetPointer(), searchValue);
//    std::vector<itk::Index<2> > neighbors = Get8NeighborsWithValueFast(center, image.GetPointer(), searchValue);
    totalSize += neighbors.size();
  }

  std::cout << "totalSize " << totalSize << std::endl;
  return 0;
}
