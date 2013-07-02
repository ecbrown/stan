// Arguments: Ints, Ints, Doubles
#include <stan/prob/distributions/univariate/discrete/binomial.hpp>

#include <stan/math/functions/logit.hpp>
#include <stan/math/functions/binomial_coefficient_log.hpp>
#include <stan/math/functions/log1m.hpp>
#include <stan/math/functions/multiply_log.hpp>
#include <stan/math/functions/inv_logit.hpp>

using std::vector;
using std::numeric_limits;
using stan::agrad::var;

class AgradDistributionsBinomialLogit : public AgradDistributionTest {
public:
    void valid_values(vector<vector<double> >& parameters,
                    vector<double>& log_prob) {
    vector<double> param(3);
    using stan::math::logit;

    param[0] = 10;           // n
    param[1] = 20;           // N
    param[2] = logit(0.4);          // alpha
    parameters.push_back(param);
    log_prob.push_back(-2.144372); // expected log_prob

    param[0] = 5;            // n
    param[1] = 15;           // N
    param[2] = logit(0.8);          // alpha
    parameters.push_back(param);
    log_prob.push_back(-9.20273); // expected log_prob
  }
 
  void invalid_values(vector<size_t>& index, 
                      vector<double>& value) {
    // n
    index.push_back(0U);
    value.push_back(-1);
    
    // N
    index.push_back(1U);
    value.push_back(-1);
    
    // alpha
    index.push_back(2U);
    value.push_back(std::numeric_limits<double>::infinity());
  }

 double num_params() {
    return 3;
  }

  std::vector<double> lower_bounds() {
    std::vector<double> lb;
    lb.push_back(0); //n
    lb.push_back(10); //N
    lb.push_back(0.0); //theta

    return lb;
  }

  std::vector<std::vector<double> > lower_bound_vals() {
    std::vector<std::vector<double> > lb;
    std::vector<double> lb1;
    std::vector<double> lb2;
    std::vector<double> lb3;
   
    lb1.push_back(-10.216513); //n for valid values 1
    lb1.push_back(-24.141569); //n for valid values 2
    lb2.push_back(-9.1629073); //N for valid values 1
    lb2.push_back(-3.6334780); //N for valid values 2
    lb3.push_back(-1.7361523); //theta for valid values 1
    lb3.push_back(-2.3898406); //theta for valid values 2

    lb.push_back(lb1);
    lb.push_back(lb2);
    lb.push_back(lb3);

    return lb;
  }

  std::vector<double> upper_bounds() {
    std::vector<double> ub;
    ub.push_back(15); //n
    ub.push_back(20); //N
    ub.push_back(1.0); //theta

    return ub;
  }

  std::vector<std::vector<double> > upper_bound_vals() {
    std::vector<std::vector<double> > ub;
    std::vector<double> ub1;
    std::vector<double> ub2;
    std::vector<double> ub3;
   
    ub1.push_back(-6.6496360); //n for valid values 1
    ub1.push_back(-3.3471533); //n for valid values 2
    ub2.push_back(-2.1443723); //N for valid values 1
    ub2.push_back(-15.608433); //N for valid values 2
    ub3.push_back(-4.1384423); //theta for valid values 1
    ub3.push_back(-6.6915583); //theta for valid values 2

    ub.push_back(ub1);
    ub.push_back(ub2);
    ub.push_back(ub3);

    return ub;
  }

  template <class T_n, class T_N, class T_prob,
      typename T3, typename T4, typename T5, 
      typename T6, typename T7, typename T8, 
      typename T9>
  typename stan::return_type<T_prob>::type 
  log_prob(const T_n& n, const T_N& N, const T_prob& alpha,
     const T3&, const T4&, const T5&,
     const T6&, const T7&, const T8&,
     const T9&) {
    return stan::prob::binomial_logit_log(n, N, alpha);
  }

  template <bool propto, 
      class T_n, class T_N, class T_prob,
      typename T3, typename T4, typename T5, 
      typename T6, typename T7, typename T8, 
      typename T9>
  typename stan::return_type<T_prob>::type 
  log_prob(const T_n& n, const T_N& N, const T_prob& alpha,
     const T3&, const T4&, const T5&,
     const T6&, const T7&, const T8&,
     const T9&) {
    return stan::prob::binomial_logit_log<propto>(n, N, alpha);
  }
  
  
  template <class T_n, class T_N, class T_prob,
      typename T3, typename T4, typename T5, 
      typename T6, typename T7, typename T8, 
      typename T9>
  var log_prob_function(const T_n& n, const T_N& N, const T_prob& alpha,
      const T3&, const T4&, const T5&,
      const T6&, const T7&, const T8&,
      const T9&) {
    using std::log;
    using stan::math::binomial_coefficient_log;
    using stan::math::log1m;
    using stan::math::multiply_log;
    using stan::prob::include_summand;

    using stan::math::inv_logit;

    var logp(0);
    if (include_summand<true>::value)
      logp += binomial_coefficient_log(N,n);
    if (include_summand<true,T_prob>::value) 
      logp += multiply_log(n,inv_logit(alpha))
        + (N - n) * log1m(inv_logit(alpha));
    return logp;
  }
};
