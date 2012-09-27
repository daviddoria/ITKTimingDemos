/** Timings:
 * -O3 and -03 -msse2 seem to be the same:
 * Built in time: 3.049
 * Custom time: 2.00261
 *
 */

#include "itkCovariantVector.h"
#include "itkImage.h"
#include "itkNumericTraits.h"
#include "itkTimeProbe.h"

template< class T, unsigned int NVectorDimension >
 typename itk::CovariantVector< T, NVectorDimension >::ValueType
 CustomSquaredNorm(const typename itk::CovariantVector< T, NVectorDimension >& v)
 {
  typedef typename itk::CovariantVector< T, NVectorDimension >::ValueType ValueType;
   typename itk::CovariantVector< T, NVectorDimension >::ValueType sum =
      itk::NumericTraits< ValueType >::Zero;

   for ( unsigned int i = 0; i < NVectorDimension; ++i )
     {
     const ValueType value = v[i];
     sum += value * value;
     }
   return sum;
 }

int main(int, char *[])
{
  typedef itk::CovariantVector<int, 3> VectorType;

  // Create random vectors
  unsigned int numberOfVectors = 1e5;
  std::vector<VectorType> vectors(numberOfVectors);

  for(size_t i = 0; i < vectors.size(); ++i)
    {
    VectorType v;
//    v[0] = drand48();
//    v[1] = drand48();
//    v[2] = drand48();
    v[0] = rand() % 255;
    v[1] = rand() % 255;
    v[2] = rand() % 255;
    vectors[i] = v;
    }

  unsigned int numberOfIterations = 1e4;

  ////////// Method 1: Built in /////////////
  double builtInTotal = 0; // Use this to ensure the loop doesn't get optimized out

  itk::TimeProbe builtInTimeProbe;
  builtInTimeProbe.Start();
  for(unsigned int iteration = 0; iteration < numberOfIterations; ++iteration)
    {
    for(size_t i = 0; i < vectors.size(); ++i)
      {
      builtInTotal += vectors[i].GetSquaredNorm();
      }
    }
  builtInTimeProbe.Stop();
  std::cout << "Built in time: " << builtInTimeProbe.GetMean() << std::endl;
  std::cout << "built in sum: " << builtInTotal << std::endl;

  ////////// Method 2: Custom /////////////
  VectorType::ComponentType customTotal = 0; // Use this to ensure the loop doesn't get optimized out

  itk::TimeProbe customTimeProbe;
  customTimeProbe.Start();
  for(unsigned int iteration = 0; iteration < numberOfIterations; ++iteration)
    {
    for(size_t i = 0; i < vectors.size(); ++i)
      {
      customTotal += CustomSquaredNorm(vectors[i]);
      }
    }
  customTimeProbe.Stop();
  std::cout << "Custom time: " << customTimeProbe.GetMean() << std::endl;
  std::cout << "Custom sum: " << customTotal << std::endl;

  return EXIT_SUCCESS;
}
