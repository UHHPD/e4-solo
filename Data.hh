#ifndef DATA_HH
#define DATA_HH

#include <vector>
#include <string>
#include <functional>

class Data {
 public:
  Data(const std::string& filename);
  int checkCompatibility(const Data& in, int n);
  double chi_square(std::function<double (double)> func);

  unsigned int size() const { return m_data.size(); }
  double measurement(int i) const { return m_data[i]; }
  double binCenter(int i) const { return m_bins[i] + (m_bins[i+1] - m_bins[i]) / 2; }
  double binLow(int i) const { return m_bins[i]; }
  double binHigh(int i) const { return m_bins[i+1]; }
  double error(int i) const { return m_errors[i]; }

 private:
  Data() {}  // disallow empty data set
  void assertSizes();
  std::vector<double> m_data;
  std::vector<double> m_bins;
  std::vector<double> m_errors;

};

#endif
