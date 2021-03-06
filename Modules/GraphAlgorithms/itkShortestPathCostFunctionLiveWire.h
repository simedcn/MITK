/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef __itkShortestPathCostFunctionLiveWire_h
#define __itkShortestPathCostFunctionLiveWire_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkShortestPathCostFunction.h" // Superclass of Metrics

#include <itkImageRegionConstIterator.h>




namespace itk
{
  /** \brief Cost function for LiveWire purposes.
  Specific features are considered to calculate cummulative
  costs of a link between two pixels. These are:

  - Gradient Magnitude
  - Gradient Direction
  - Laplacian Zero Crossing

  By default the Gradient Magnitude is mapped linear to costs
  between 0 (good) and 1 (bad). Via SetDynamicCostMap( std::map< int, int > &costMap)
  a cost map can be set to dynamically map Gradient Magnitude (non
  linear). Thus lower values can be considered with lower costs
  than higher values of gradient magnitudes.
  To compute  the costs of the gradient magnitude dynamically
  a iverted map of the histogram of gradient magnitude image is used.

  */
  template <class TInputImageType>
  class ITK_EXPORT ShortestPathCostFunctionLiveWire : public ShortestPathCostFunction<TInputImageType>
  {
  public:
    /** Standard class typedefs. */
    typedef ShortestPathCostFunctionLiveWire                Self;
    typedef ShortestPathCostFunction<TInputImageType>       Superclass;
    typedef SmartPointer<Self>                              Pointer;
    typedef SmartPointer<const Self>                        ConstPointer;
    typedef itk::ImageRegionConstIterator<TInputImageType>  ConstIteratorType;


    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(ShortestPathCostFunctionLiveWire, ShortestPathCostFunction);


    typedef itk::Image<unsigned char, 2>  UnsignedCharImageType;
    typedef itk::Image<float, 2>  FloatImageType;

    typedef float ComponentType;
    typedef itk::CovariantVector< ComponentType, 2  > OutputPixelType;
    typedef itk::Image< OutputPixelType, 2 > VectorOutputImageType;

    typedef typename TInputImageType::IndexType             IndexType;
    typedef TInputImageType                                 ImageType;
    typedef itk::ImageRegion<2>                             RegionType;



    /** \brief calculates the costs for going from p1 to p2*/
    virtual double GetCost(IndexType p1, IndexType p2);

    /** \brief returns the minimal costs possible (needed for A*)*/
    virtual double GetMinCost();

    /** \brief Initialize the metric*/
    virtual void Initialize ();


    /** \brief Set repulsive path*/
    virtual void AddRepulsivePoint( itk::Index<3>  );

    /** \brief Clear repulsive path*/
    virtual void ClearRepulsivePoints(  );



    ShortestPathCostFunctionLiveWire();


    itkSetMacro (RequestedRegion, RegionType);
    itkGetMacro (RequestedRegion, RegionType);

    // Set/Get function for sigma parameter
    itkSetMacro (UseApproximateGradient, bool);
    itkGetMacro (UseApproximateGradient, bool);

    virtual void SetImage(const TInputImageType* _arg)
    {
      if (this->m_Image != _arg)
      {
        this->m_Image = _arg;
        this->Modified();
        this->m_Initialized = false;
      }
    }

    void SetDynamicCostMap( std::map< int, int > &costMap)
    {
      this->m_CostMap = costMap;
      this->m_UseCostMap = true;
      this->m_MaxMapCosts = -1;
      this->Modified();
    }

    void SetUseCostMap(bool useCostMap)
    {
      this->m_UseCostMap = useCostMap;
    }

    /**
     \brief Set the maximum of the dynamic cost map to save computation time.
    */
    void SetCostMapMaximum(double max)
    {
      this->m_MaxMapCosts = max;
    }


    enum Constants{
      MAPSCALEFACTOR = 10
    };

    /** \brief Returns the y value of gaussian with given offset and amplitude

       gaussian approximation
       f(x) = v(bin) * e^ ( -1/2 * (|x-k(bin)| / sigma)^2 )

       \param x
       \param xOfGaussian - offset
       \param yOfGaussian - amplitude
    */
    static double Gaussian(double x, double xOfGaussian, double yOfGaussian);

  protected:

    virtual ~ShortestPathCostFunctionLiveWire() {};


    typename ImageType::Pointer m_GradientMagnImage;
    typename UnsignedCharImageType::Pointer m_ZeroCrossingsImage;
    typename FloatImageType::Pointer m_EdgeImage;
    typename VectorOutputImageType::Pointer m_GradientImage;

    double minCosts;

    bool m_UseRepulsivePoint;

    std::vector<  itk::Index<3>  > m_RepulsivePoints;

    typename Superclass::PixelType val;

    typename Superclass::PixelType startValue;
    typename Superclass::PixelType endValue;

    double m_GradientMax;

    RegionType m_RequestedRegion;

    bool m_UseApproximateGradient;

    bool m_Initialized;

    std::map< int, int > m_CostMap;

    bool m_UseCostMap;

    double m_MaxMapCosts;

  private:

    double SigmoidFunction(double I, double max, double min, double alpha, double beta);


  };

} // end namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkShortestPathCostFunctionLiveWire.txx"
#endif

#endif /* __itkShortestPathCostFunctionLiveWire_h */
