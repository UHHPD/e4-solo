#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <cmath>
#include <functional>

using namespace std;

Data::Data(const std::string& filename) {
  ifstream file(filename);

  // check if file opened correctly
  if (!file.is_open()) {
    throw std::runtime_error("Error could not open file with name: " +
                             filename);
  }

  int size = -1;
  file >> size;

  // read in data from file: m_siz+1 bin edges
  for (int i = 0; i < size + 1; ++i) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }
  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; ++i) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }
  //read in data from file: m_siz bin erros
  for (int i = 0; i < size; ++i) {
    double errors;
    file >> errors;
    m_errors.push_back(errors);
  }

  // done! close the file
  file.close();

  assertSizes();
};

int Data::checkCompatibility(const Data& in, int n){
  int data_points = m_data.size();
  int compatible_points = 0;
  for (int i = 0; i < data_points; ++i){
    double diff = abs(m_data[i] - in.measurement(i));
    double diff_error = abs(m_errors[i] - in.error(i));

    if (diff > n * diff_error){
      ++compatible_points;
    }
  }
  return compatible_points;
}

double Data::chi_square(std::function<double (double)> func){
  int data_points = m_data.size();
  double chi_square_value = 0;
  double n_dof = 52;
  for (int i = 0; i < data_points; ++i){
    double energy_value = m_bins[i] + (m_bins[i+1] - m_bins[i]) / 2;
    double value = pow(m_data[i] - func(energy_value), 2) / pow(m_errors[i], 2);
    chi_square_value += value;
  }
  double result = chi_square_value / n_dof;

  return result;
}

void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }
