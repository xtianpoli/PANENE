#ifndef panene_naive_data_source_h
#define panene_naive_data_source_h

#include <string>
#include <fstream>
#include <vector>
#include <cmath>

namespace panene
{

class NaiveDataSource
{

public:

  typedef size_t IDType;
  typedef float ElementType;
  typedef float DistanceType;

  NaiveDataSource() {
  }

  ~NaiveDataSource() {
    if(opened) delete[] data;
  }

  void open(const std::string& path, size_t n_, size_t d_) {
    n = n_;
    d = d_;
    data = new ElementType[n * d];    
    opened = true;
    
    std::ifstream ifs(path);

    for(size_t i = 0; i < n * d; ++i) {
      ifs >> data[i]; 
    }
  }

  ElementType get(const IDType &id, const IDType &dim) const {
    return *(data + id * d + dim);
  }

  IDType findDimWithMaxSpan(const IDType &id1, const IDType &id2) {
    size_t dim = 0;
    ElementType maxSpan = 0;

    for(size_t i = 0; i < d; ++i) {
      ElementType span = std::abs(this->get(id1, i) - this->get(id2, i));
      if(maxSpan < span) {
        maxSpan = span;
        dim = i;
      }
    }

    return dim;
  }

  void sampleMeanAndVar(const IDType *ids, int count, std::vector<DistanceType> &mean, std::vector<DistanceType> &var) {
    mean.resize(d);
    var.resize(d);

    for (size_t i = 0; i < d; ++i) 
      mean[i] = var[i] = 0;

    for (int j = 0; j < count; ++j) {
      for (size_t i = 0; i < d; ++i) {
        mean[i] += this->get(ids[j], i);
      }
    }

    DistanceType divFactor = DistanceType(1)/count;

    for (size_t i = 0 ; i < d; ++i) {
      mean[i] *= divFactor;
    }

    /* Compute variances */
    for (int j = 0; j < count; ++j) {
      for (size_t i = 0; i < d; ++i) {
        DistanceType dist = this->get(ids[j], i) - mean[i];
        var[i] += dist * dist;
      }
    }

    for(size_t i = 0; i < d; ++i) {
      var[i] *= divFactor;
    }
  }  

/*  DistanceType distL2Squared(const IDType &id1, const IDType &id2) const {
    DistanceType sum = 0;
    ElementType* ele1 = (*this)[id1];
    ElementType* ele2 = (*this)[id2];

    for(size_t i = 0; i < d; ++i)
      sum += (ele1[i] - ele2[i]) * (ele1[i] - ele2[i]);
    
    return sum;
  }*/

  DistanceType distL2Squared(const IDType &id1, const ElementType *p2) const {
    DistanceType sum = 0;

    for(size_t i = 0; i < d; ++i) {
      ElementType v = this->get(id1, i);
      sum += (v - p2[i]) * (v - p2[i]);
    }
    
    return sum;
  }

/*  DistanceType sum(const IDType &id) const {
    DistanceType sum = 0;
    ElementType* ele = (*this)[id];

    for(size_t i = 0; i <d; ++i)
      sum += ele[i];

    return sum;
  }*/
  
  size_t size() const {
    return n;
  }

  size_t loaded() const {
    return n;
  }

  size_t dim() const {
    return d;
  }

  size_t n;
  size_t d;
  bool opened = false;
protected:  
  ElementType* data;
};

}
#endif
