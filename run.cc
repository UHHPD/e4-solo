#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <string>
#include <cmath>

#include "Data.hh"

// generic function comparing two values of some type T used later for int and
// double
template <class T>
bool testEqual(const std::string& name, T expected, T real) {
  if (expected != real) {
    std::cout << "(" << name << ": <" << expected << ">"
              << ", " << real << ") ";
    return false;
  }
  return true;
}

bool testReadingSize() {
  std::cout << "testReadingSize...";
  Data datA("testA");
  return testEqual<int>("size", 1, datA.size());
}

bool testReadingMeasurement() {
  std::cout << "testReadingMeasurement...";
  Data datA("testA");
  return testEqual("measurement", 10., datA.measurement(0));
}

bool testReadingBinEdges() {
  std::cout << "testReadingBinEdges...";
  Data datA("testA");
  return testEqual("bin low", 0., datA.binLow(0)) &&
         testEqual("bin high", 1., datA.binHigh(0));
}

bool testReadingErrors() {
  std::cout << "testReadingErrors...";
  Data datA("testA");
  return testEqual("error", 2., datA.error(0));
}

bool testCopyConstructor() {
  std::cout << "testCopyConstructor...";
  Data a("testA");
  Data b(a);
  Data c = a;  // equivalent to Data c(a)
  return testEqual("measurement", 10., b.measurement(0)) &&
         testEqual("measurement", 10., c.measurement(0));
}

bool testCompatibility(){
  std::cout << "testCompatibility...";
  Data a("comp_testA");
  Data b("comp_testB");
  return testEqual<int>("compatibility", 3., a.checkCompatibility(b,6));
}

void runTests() {
  std::cout << "running tests...\n";
  std::vector<std::function<bool()> > tests(
      {testReadingSize, testReadingMeasurement, testReadingBinEdges,
       testReadingErrors, testCopyConstructor, testCompatibility});
  for (auto test : tests)
    std::cout << (test() ? " ok" : " FAILED!") << std::endl;
}

void average(const std::string filename, const Data& one, const Data& two){
  std::ofstream fout(filename);
  int data_points = one.size();
  std::vector<double> bins;
  std::vector<double> measurement_values;
  std::vector<double> error_values;
  fout << data_points << std::endl;
  for (int i = 0; i < data_points + 1; i++){
    double bin = one.binLow(i);
    bins.push_back(bin);
  }
  for (const auto &e : bins) fout << e << "  ";
  fout << std::endl;
  for (int i = 0; i < data_points; ++i){
    double value_one = one.measurement(i);
    double value_two = two.measurement(i);
    double error_one = one.error(i);
    double error_two = two.error(i);
    double w_one = 1 / (error_one * error_one);
    double w_two = 1 / (error_two * error_two);
    double average_value = (w_one * value_one + w_two * value_two) / (w_one + w_two);
    double sigma_average_value = sqrt(1 / (w_one + w_two));
    measurement_values.push_back(average_value);
    error_values.push_back(sigma_average_value);
  }
    for (const auto &e : measurement_values) fout << e << "  ";
    fout << std::endl;
    for (const auto &e : error_values) fout << e << "  ";
  fout.close();
}

double background_model(double energy){
  double alpha =  0.005;
  double beta = -0.00001;
  double gamma = 0.08;
  double delta = 0.015;
  double value = alpha + beta * energy + gamma * exp(-delta * energy);

  return value;
}

int main() {
  using namespace std;

  cout << "******************************************************" << endl;
  runTests();
  cout << "******************************************************" << endl;
  // create an object which holds data of experiment A,B,C,D
  Data datA("exp_A");
  Data datB("exp_B");
  Data datC("exp_C");
  Data datD("exp_D");
  vector<Data> data_sets({datA, datB, datC, datD});

  average("average_exp_A_exp_B",datA,datB);
  Data datAvg_A_B("average_exp_A_exp_B");
  average("average_exp_A_exp_B_exp_C",datAvg_A_B,datC);
  Data datAvg_A_B_C("average_exp_A_exp_B_exp_C");
  average("average_exp_A_exp_B_exp_C_exp_D",datAvg_A_B_C,datD);
  Data datAvg_A_B_C_D("average_exp_A_exp_B_exp_C_exp_D");




  // here is the data from experiment A,B,C,D
  cout << "bin 27: from " << data_sets[0].binLow(27) << " to " << data_sets[0].binHigh(27)
       << endl;
  cout << "measurement of experiment A in bin 27: " << data_sets[0].measurement(27)
       << " +/- " << data_sets[0].error(27) << endl;
  cout << "measurement of experiment B in bin 27: " << data_sets[1].measurement(27)
       << " +/- " << data_sets[1].error(27) << endl;
  cout << "measurement of experiment C in bin 27: " << data_sets[2].measurement(27)
       << " +/- " << data_sets[2].error(27) << endl;
  cout << "measurement of experiment D in bin 27: " << data_sets[3].measurement(27)
       << " +/- " << data_sets[3].error(27) << endl;
  cout << "compatibility of experiment A and B (two standard deviations): " <<
          data_sets[0].checkCompatibility(data_sets[1], 2) << endl;
  cout << "compatibility of experiment A and C (two standard deviations): " <<
          data_sets[0].checkCompatibility(data_sets[2], 2) << endl;
  cout << "compatibility of experiment A and D (two standard deviations): " <<
          data_sets[0].checkCompatibility(data_sets[3], 2) << endl;
  cout << "compatibility of experiment B and C (two standard deviations): " <<
          data_sets[1].checkCompatibility(data_sets[2], 2) << endl;
  cout << "compatibility of experiment B and D (two standard deviations): " <<
          data_sets[1].checkCompatibility(data_sets[3], 2) << endl;
  cout << "compatibility of experiment C and D (two standard deviations): " <<
          data_sets[2].checkCompatibility(data_sets[3], 2) << endl;
  cout << "average of experiment A and B in bin 27: " << datAvg_A_B.measurement(27)
       << " +/- " << datAvg_A_B.error(27) << endl;
  cout << "Chi-Square-Test of A TEST: " << data_sets[0].chi_square(background_model) << endl;
  cout << "Chi-Square-Test combination of all four datasets: " << datAvg_A_B_C_D.chi_square(background_model) << endl;



  return 0;
}
