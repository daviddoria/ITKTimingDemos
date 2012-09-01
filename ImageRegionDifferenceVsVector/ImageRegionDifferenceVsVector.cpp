#include "itkTimeProbe.h"
#include "itkImage.h"
#include "itkImageRegionIterator.h"

typedef itk::Image<float, 2> ImageType;

const unsigned int patchRadius = 10;
const unsigned int imageSize = 100;
const unsigned int numberOfOuterLoops = 1000;

static void ITKImage();
static void Vector();

static std::vector<float> MakeDescriptor(const itk::ImageRegion<2>& a, ImageType* image);

static void CreateImage(ImageType* image);

static float Difference(const std::vector<float>& a, const std::vector<float>& b);
static float Difference(const itk::ImageRegion<2>& a, const itk::ImageRegion<2>& b, ImageType* const image);

static itk::ImageRegion<2> GetRegionInRadiusAroundPixel(const itk::Index<2>& pixel, const unsigned int radius);

int main(int, char *[])
{
  ITKImage();
  Vector();

  return EXIT_SUCCESS;
}


itk::ImageRegion<2> GetRegionInRadiusAroundPixel(const itk::Index<2>& pixel, const unsigned int radius)
{
  // This function returns a Region with the specified 'radius' centered at 'pixel'.
  // By the definition of the radius of a square patch, the output region is (radius*2 + 1)x(radius*2 + 1).
  // Note: This region is not necessarily entirely inside the image!

  // The "index" is the lower left corner, so we need to subtract the radius from the center to obtain it
  itk::Index<2> lowerLeft;
  lowerLeft[0] = pixel[0] - radius;
  lowerLeft[1] = pixel[1] - radius;

  itk::ImageRegion<2> region;
  region.SetIndex(lowerLeft);
  itk::Size<2> size;
  size[0] = radius*2 + 1;
  size[1] = radius*2 + 1;
  region.SetSize(size);

  return region;
}

void CreateImage(ImageType* image)
{
  itk::Index<2> corner = {{0,0}};
  itk::Size<2> size = {{imageSize, imageSize}};
  itk::ImageRegion<2> fullRegion(corner, size);
  image->SetRegions(fullRegion);
  image->Allocate();

  itk::ImageRegionIterator<ImageType> imageIterator(image, image->GetLargestPossibleRegion());

  int i = 0;
  while(!imageIterator.IsAtEnd())
    {
    imageIterator.Set(i);
    i++;
    ++imageIterator;
    }
}

void ITKImage()
{
  std::cout << "ITKImage()" << std::endl;
  
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);

  itk::Index<2> center = {{imageSize/2, imageSize/2}};
  itk::ImageRegion<2> centerRegion = GetRegionInRadiusAroundPixel(center, patchRadius);

  std::vector<itk::ImageRegion<2> > allRegions;

  {
  itk::ImageRegionIterator<ImageType> imageIterator(image, image->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    itk::ImageRegion<2> region = GetRegionInRadiusAroundPixel(imageIterator.GetIndex(), patchRadius);
    if(image->GetLargestPossibleRegion().IsInside(region))
      {
      allRegions.push_back(region);
      }
    ++imageIterator;
    }
  }

  itk::TimeProbe clock1;

  clock1.Start();

  float totalDifference = 0.0f;
  for(unsigned int outerLoop = 0; outerLoop < numberOfOuterLoops; ++outerLoop)
    {
    for(size_t regionId = 0; regionId < allRegions.size(); ++regionId)
      {
      totalDifference += Difference(allRegions[regionId], centerRegion, image);
      }
    }

  clock1.Stop();
  std::cout << "Total time: " << clock1.GetTotal() << std::endl;
  std::cout << "Total difference: " << totalDifference << std::endl;
  
}

void Vector()
{
  std::cout << "Vector()" << std::endl;
  
  std::vector<float> vec(patchRadius*patchRadius);

  ImageType::Pointer image = ImageType::New();
  CreateImage(image);

  itk::Index<2> center = {{imageSize/2, imageSize/2}};
  itk::ImageRegion<2> centerRegion = GetRegionInRadiusAroundPixel(center, patchRadius);
  std::vector<float> centerDescriptor = MakeDescriptor(centerRegion, image);

  std::vector<std::vector<float> > allDescriptors;

  {
  itk::ImageRegionIterator<ImageType> imageIterator(image, image->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    itk::ImageRegion<2> region = GetRegionInRadiusAroundPixel(imageIterator.GetIndex(), patchRadius);
    if(image->GetLargestPossibleRegion().IsInside(region))
      {
      allDescriptors.push_back(MakeDescriptor(region, image));
      }

    ++imageIterator;
    }
  }

  std::cout << "There are " << allDescriptors.size() << " descriptors." << std::endl;
  std::cout << "There are " << allDescriptors[0].size() << " elements per descriptor." << std::endl;
  itk::TimeProbe clock1;

  clock1.Start();
  float totalDifference = 0.0f;
  for(unsigned int outerLoop = 0; outerLoop < numberOfOuterLoops; ++outerLoop)
    {
    for(unsigned int i = 0; i < allDescriptors.size(); ++i)
      {
      totalDifference += Difference(centerDescriptor, allDescriptors[i]);
      }
    }

  clock1.Stop();
  std::cout << "Total time: " << clock1.GetTotal() << std::endl;
  std::cout << "Total difference: " << totalDifference << std::endl;
}

std::vector<float> MakeDescriptor(const itk::ImageRegion<2>& region, ImageType* image)
{
  if(!image->GetLargestPossibleRegion().IsInside(region))
    {
    throw std::runtime_error("Cannot compute descriptor for region outside of image bounds!");
    }

  std::vector<float> descriptor;

  itk::ImageRegionIterator<ImageType> imageIterator(image, region);

  while(!imageIterator.IsAtEnd())
    {
    descriptor.push_back(imageIterator.Get());

    ++imageIterator;
    }
  return descriptor;
}

float Difference(const std::vector<float>& a, const std::vector<float>& b)
{
  float difference = 0.0f;
  for(size_t i = 0; i < a.size(); ++i)
    {
    difference += fabs(a[i] - b[i]);
    }
  return difference;
}

float Difference(const itk::ImageRegion<2>& a, const itk::ImageRegion<2>& b, ImageType* const image)
{
  itk::ImageRegionIterator<ImageType> imageIteratorA(image, a);
  itk::ImageRegionIterator<ImageType> imageIteratorB(image, b);

  float difference = 0.0f;
  while(!imageIteratorA.IsAtEnd())
    {
    difference += fabs(imageIteratorA.Get() - imageIteratorB.Get());

    ++imageIteratorA;
    ++imageIteratorB;
    }

  return difference;
}
