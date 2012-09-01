#include "itkCovariantVector.h"
#include "itkImage.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkTimeProbe.h"
#include "itkVariableLengthVector.h"
#include "itkVectorImage.h"

// Conclusion: Image<CovariantVector> is almost 4x faster than VectorImage when performing lots of pixel differences!
const unsigned int pixelDimension = 100;

typedef itk::Image<itk::CovariantVector<float, pixelDimension>, 2> ImageFixedLengthType;
typedef itk::Image<itk::VariableLengthVector<float>, 2> ImageVariableLengthType;
typedef itk::VectorImage<float, 2> VectorImageType;

const unsigned int imageSize = 500;
const unsigned int numberOfOuterLoops = 1000;

template <typename TImage>
void CompareImage(TImage* const image)
{
  itk::TimeProbe clock1;

  clock1.Start();

  float totalDifference = 0.0f;
  for(unsigned int outerLoop = 0; outerLoop < numberOfOuterLoops; ++outerLoop)
    {
    itk::ImageRegionIterator<TImage> imageIterator(image, image->GetLargestPossibleRegion());
    typename TImage::PixelType p = image->GetPixel(image->GetLargestPossibleRegion().GetIndex());
    while(!imageIterator.IsAtEnd())
      {
      totalDifference += (p - imageIterator.Get()).GetNorm();
      ++imageIterator;
      }
    }

  clock1.Stop();
  std::cout << "Total time: " << clock1.GetTotal() << std::endl;
  std::cout << "Total difference: " << totalDifference << std::endl;
  
}


// template <typename TImage>
// void CompareImage(TImage* const image);

static void CreateImages(ImageFixedLengthType* const fixedLengthImage, ImageVariableLengthType* const variableLengthImage, VectorImageType* const vectorImage);

int main(int, char *[])
{
  ImageFixedLengthType::Pointer fixedLengthImage = ImageFixedLengthType::New();
  ImageVariableLengthType::Pointer variableLengthImage = ImageVariableLengthType::New();
  VectorImageType::Pointer vectorImage = VectorImageType::New();
  
  CreateImages(fixedLengthImage, variableLengthImage, vectorImage);

  std::cout << "Image<CovariantVector>()" << std::endl;
  CompareImage(fixedLengthImage.GetPointer());
  
  std::cout << "Image<VariableLengthVector>()" << std::endl;
  CompareImage(variableLengthImage.GetPointer());
  
  std::cout << "VectorImage()" << std::endl;
  CompareImage(vectorImage.GetPointer());

  return EXIT_SUCCESS;
}

static void CreateImages(ImageFixedLengthType* const fixedLengthImage, ImageVariableLengthType* const variableLengthImage, VectorImageType* const vectorImage)
{
  itk::Index<2> corner = {{0,0}};
  itk::Size<2> size = {{imageSize, imageSize}};
  itk::ImageRegion<2> fullRegion(corner, size);

  vectorImage->SetRegions(fullRegion);
  vectorImage->SetNumberOfComponentsPerPixel(pixelDimension);
  vectorImage->Allocate();

  fixedLengthImage->SetRegions(fullRegion);
  fixedLengthImage->Allocate();

  variableLengthImage->SetRegions(fullRegion);
  variableLengthImage->Allocate();

  // Iterate over any one of the images (we just want the indices)
  itk::ImageRegionIteratorWithIndex<VectorImageType> imageIterator(vectorImage, vectorImage->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    itk::Index<2> index = imageIterator.GetIndex();

    VectorImageType::PixelType p_vectorImage;
    p_vectorImage.SetSize(pixelDimension);
  
    ImageVariableLengthType::PixelType p_variableLengthImage;
    p_variableLengthImage.SetSize(pixelDimension);
  
    ImageFixedLengthType::PixelType p_fixedLengthImage;

    for(unsigned int i = 0; i < pixelDimension; ++i)
      {
      float value = drand48();
      p_fixedLengthImage[i] = value;
      p_variableLengthImage[i] = value;
      p_vectorImage[i] = value;
      }

    fixedLengthImage->SetPixel(index, p_fixedLengthImage);
    variableLengthImage->SetPixel(index, p_variableLengthImage);
    vectorImage->SetPixel(index, p_vectorImage);

    ++imageIterator;
    }

}

