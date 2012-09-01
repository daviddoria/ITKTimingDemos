#include "itkTimeProbe.h"
#include "itkImage.h"
#include "itkImageRegionIterator.h"

static void ITKImage();
static void Vector();

int main(int, char *[])
{
  ITKImage();
  Vector();

  return EXIT_SUCCESS;
}

void ITKImage()
{
  typedef itk::Image<float, 2> ImageType;
  ImageType::Pointer image = ImageType::New();

  itk::Index<2> corner = {{0,0}};
  itk::Size<2> size = {{20, 20}};
  itk::ImageRegion<2> region(corner, size);
  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIterator<ImageType> imageIterator(image, region);

  itk::TimeProbe clock1;

  clock1.Start();

  while(!imageIterator.IsAtEnd())
    {
    float a = imageIterator.Get() - imageIterator.Get();

    ++imageIterator;
    }

  clock1.Stop();
  std::cout << "Total: " << clock1.GetTotal() << std::endl;
}

void Vector()
{
  std::vector<float> vec(400);

  itk::TimeProbe clock1;

  clock1.Start();
  for(unsigned int i = 0; i < vec.size(); ++i)
    {
    float a = vec[i] - vec[i];
    }

  clock1.Stop();
  std::cout << "Total: " << clock1.GetTotal() << std::endl;
}
