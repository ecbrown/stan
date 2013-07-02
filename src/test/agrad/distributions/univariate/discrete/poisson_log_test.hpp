// Arguments: Ints, Doubles
#include <stan/prob/distributions/univariate/discrete/poisson.hpp>

#include <stan/math/functions/multiply_log.hpp>

using std::vector;
using std::numeric_limits;
using stan::agrad::var;

class AgradDistributionsPoisson : public AgradDistributionTest {
public:
  void valid_values(vector<vector<double> >& parameters,
                    vector<double>& log_prob) {

    using std::log;

    vector<double> param(2);

    param[0] = 17;           // n
    param[1] = log(13.0);         // alpha
    parameters.push_back(param);
    log_prob.push_back(-2.900934); // expected log_prob

    param[0] = 192;          // n
    param[1] = log(42.0);         // alpha
    parameters.push_back(param);
    log_prob.push_back(-145.3547); // expected log_prob

    param[0] = 0;            // n
    param[1] = log(3.0);          // alpha
    parameters.push_back(param);
    log_prob.push_back(-3.0); // expected log_prob
  }
 
  void invalid_values(vector<size_t>& index,
                      vector<double>& value) {
    // n
    index.push_back(0U);
    value.push_back(-1);

    // alpha
    // all OK
  }

 double num_params() {
    return 2;
  }

  std::vector<double> lower_bounds() {
    std::vector<double> lb;
    lb.push_back(0.0); //n
    lb.push_back(-numeric_limits<double>::infinity()); //lambda

    return lb;
  }

  std::vector<std::vector<double> > lower_bound_vals() {
    std::vector<std::vector<double> > lb;
    std::vector<double> lb1;
    std::vector<double> lb2;
   
    lb1.push_back(-13.0); //n for valid values 1
    lb1.push_back(-42.0); //n for valid values 2
    lb1.push_back(-3.0); //n for valid values 3
    lb2.push_back(-numeric_limits<double>::infinity()); //lambda for valid values 1
    lb2.push_back(-numeric_limits<double>::infinity()); //lambda for valid values 2
    lb2.push_back(0.0); //lambda for valid values 3

    lb.push_back(lb1);
    lb.push_back(lb2);

    return lb;
  }

  std::vector<double> upper_bounds() {
    std::vector<double> ub;
    ub.push_back(numeric_limits<int>::infinity()); //n
    ub.push_back(numeric_limits<double>::infinity()); //lambda

    return ub;
  }

  std::vector<std::vector<double> > upper_bound_vals() {
    std::vector<std::vector<double> > ub;
    std::vector<double> ub1;
    std::vector<double> ub2;
   
    ub1.push_back(-13.0); //n for valid values 1
    ub1.push_back(-42.0); //n for valid values 2
    ub1.push_back(-3.0); //n for valid values 3
    ub2.push_back(-numeric_limits<double>::infinity()); //lambda for valid values 1
    ub2.push_back(-numeric_limits<double>::infinity()); //lambda for valid values 2
    ub2.push_back(-numeric_limits<double>::infinity()); //lambda for valid values 3

    ub.push_back(ub1);
    ub.push_back(ub2);

    return ub;
  }

  template <class T_n, class T_rate, typename T2,
      typename T3, typename T4, typename T5, 
      typename T6, typename T7, typename T8, 
      typename T9>
  typename stan::return_type<T_n, T_rate>::type 
  log_prob(const T_n& n, const T_rate& alpha, const T2&,
     const T3&, const T4&, const T5&, const T6&, const T7&, const T8&, const T9&) {
    return stan::prob::poisson_log_log(n, alpha);
  }

  template <bool propto, 
      class T_n, class T_rate, typename T2,
      typename T3, typename T4, typename T5, 
      typename T6, typename T7, typename T8, 
      typename T9>
  typename stan::return_type<T_n, T_rate>::type 
  log_prob(const T_n& n, const T_rate& alpha, const T2&,
     const T3&, const T4&, const T5&, const T6&, const T7&, const T8&, const T9&) {
    return stan::prob::poisson_log_log<propto>(n, alpha);
  }
  
  
  template <class T_n, class T_rate, typename T2,
      typename T3, typename T4, typename T5, 
      typename T6, typename T7, typename T8, 
      typename T9>
  var log_prob_function(const T_n& n, const T_rate& alpha, const T2&,
      const T3&, const T4&, const T5&, const T6&, const T7&, const T8&, const T9&) {
    using std::exp;
    using boost::math::lgamma;
    using stan::math::multiply_log;
    using stan::prob::LOG_ZERO;
    using stan::prob::include_summand;

    var logp(0);

    if (alpha == -std::numeric_limits<double>::infinity())
      return n == 0 ? 0 : LOG_ZERO;
    
    if (alpha == std::numeric_limits<double>::infinity())
      return LOG_ZERO;
    
    if (include_summand<true>::value)
      logp -= lgamma(n + 1.0);
    if (include_summand<true,T_rate>::value)
      logp += n * alpha - exp(alpha);
    return logp;
  }
};
